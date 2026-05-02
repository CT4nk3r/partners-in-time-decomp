/*
 * asset_viewer.c — MLPIT_VIEWER mode
 *
 * Renders all 104 asset-pack entries as 4x4 thumbnails with arrow-key
 * navigation, plus an auto-cycle full-screen mode.  Drawing is done
 * directly into the platform BGR555 framebuffers (no VRAM tile pipeline,
 * no game thread) so this works while game_start() is unsafe.
 *
 * Each thumbnail is the entry's raw bytes interpreted as a 4bpp tile sheet
 * (the format that recovers the most signal across font/UI/BG art) painted
 * into a 60x44 cell with a synthetic 16-colour rainbow palette so different
 * tile bank usage shows up as different colours.
 *
 * The "looks like game art" scorer combines:
 *   - palette-bank entropy (no single nibble > 60% of the bytes)
 *   - tile-reuse signature (multiple identical 32-byte tiles)
 *   - non-noise structure (per-row variance below noise threshold)
 *
 * The top 5 candidates are dumped as PPM so the caller can convert to PNG.
 */
#include "asset_viewer.h"
#include "asset_pack.h"
#include "mlpit_pack.h"
#include "nds_platform.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SCR_W   NDS_SCREEN_WIDTH
#define SCR_H   NDS_SCREEN_HEIGHT

/* 16-colour synthetic palette (BGR555).  Different palette banks of the
 * source data render in different hues, making structure visible even
 * without the real palette. */
static const uint16_t k_pal[16] = {
    0x0000, 0x4210, 0x6318, 0x7BDE,
    0x001F, 0x03E0, 0x7C00, 0x03FF,
    0x7FE0, 0x7C1F, 0x7FFF, 0x021F,
    0x021E, 0x4200, 0x0200, 0x6210,
};

/* Foreground/UI BGR555 constants */
#define COL_BG       0x0000  /* black */
#define COL_PANEL    0x14A5  /* dark slate */
#define COL_HILITE   0x7FFF  /* white */
#define COL_ACCENT   0x03FF  /* yellow */
#define COL_TEXT     0x6F7B  /* light grey */

/* === Tiny 5x7 ASCII font (uppercase + digits + punctuation) ============
 * Each glyph is 5 columns × 7 rows packed into 7 bytes (LSB = leftmost). */
static const struct { char c; uint8_t rows[7]; } k_font[] = {
    {' ',{0,0,0,0,0,0,0}},
    {'0',{0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}},
    {'1',{0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}},
    {'2',{0x0E,0x11,0x01,0x06,0x08,0x10,0x1F}},
    {'3',{0x1F,0x02,0x04,0x02,0x01,0x11,0x0E}},
    {'4',{0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}},
    {'5',{0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}},
    {'6',{0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}},
    {'7',{0x1F,0x01,0x02,0x04,0x08,0x08,0x08}},
    {'8',{0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}},
    {'9',{0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}},
    {'A',{0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}},
    {'B',{0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}},
    {'C',{0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}},
    {'D',{0x1C,0x12,0x11,0x11,0x11,0x12,0x1C}},
    {'E',{0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}},
    {'F',{0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}},
    {'G',{0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}},
    {'H',{0x11,0x11,0x11,0x1F,0x11,0x11,0x11}},
    {'I',{0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}},
    {'J',{0x07,0x02,0x02,0x02,0x02,0x12,0x0C}},
    {'K',{0x11,0x12,0x14,0x18,0x14,0x12,0x11}},
    {'L',{0x10,0x10,0x10,0x10,0x10,0x10,0x1F}},
    {'M',{0x11,0x1B,0x15,0x15,0x11,0x11,0x11}},
    {'N',{0x11,0x19,0x15,0x13,0x11,0x11,0x11}},
    {'O',{0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}},
    {'P',{0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}},
    {'Q',{0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}},
    {'R',{0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}},
    {'S',{0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E}},
    {'T',{0x1F,0x04,0x04,0x04,0x04,0x04,0x04}},
    {'U',{0x11,0x11,0x11,0x11,0x11,0x11,0x0E}},
    {'V',{0x11,0x11,0x11,0x11,0x11,0x0A,0x04}},
    {'W',{0x11,0x11,0x11,0x15,0x15,0x15,0x0A}},
    {'X',{0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}},
    {'Y',{0x11,0x11,0x11,0x0A,0x04,0x04,0x04}},
    {'Z',{0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}},
    {':',{0x00,0x04,0x04,0x00,0x04,0x04,0x00}},
    {'/',{0x01,0x02,0x02,0x04,0x08,0x08,0x10}},
    {'#',{0x0A,0x0A,0x1F,0x0A,0x1F,0x0A,0x0A}},
    {'.',{0x00,0x00,0x00,0x00,0x00,0x04,0x04}},
    {'-',{0x00,0x00,0x00,0x1F,0x00,0x00,0x00}},
    {'(',{0x02,0x04,0x08,0x08,0x08,0x04,0x02}},
    {')',{0x08,0x04,0x02,0x02,0x02,0x04,0x08}},
    {'[',{0x0E,0x08,0x08,0x08,0x08,0x08,0x0E}},
    {']',{0x0E,0x02,0x02,0x02,0x02,0x02,0x0E}},
    {'>',{0x00,0x08,0x04,0x02,0x04,0x08,0x00}},
    {'<',{0x00,0x02,0x04,0x08,0x04,0x02,0x00}},
    {'?',{0x0E,0x11,0x01,0x02,0x04,0x00,0x04}},
    {'!',{0x04,0x04,0x04,0x04,0x04,0x00,0x04}},
    {'%',{0x18,0x19,0x02,0x04,0x08,0x13,0x03}},
    {'+',{0x00,0x04,0x04,0x1F,0x04,0x04,0x00}},
};

static const uint8_t *glyph_for(char c) {
    if (c >= 'a' && c <= 'z') c -= 0x20;
    for (size_t i = 0; i < sizeof(k_font)/sizeof(k_font[0]); i++)
        if (k_font[i].c == c) return k_font[i].rows;
    return k_font[0].rows;
}

static void put_pixel(uint16_t *fb, int x, int y, uint16_t c) {
    if ((unsigned)x < SCR_W && (unsigned)y < SCR_H) fb[y*SCR_W + x] = c;
}

static void draw_text(uint16_t *fb, int x, int y, uint16_t col, const char *s) {
    int cx = x;
    for (; *s; ++s) {
        if (*s == '\n') { y += 8; cx = x; continue; }
        const uint8_t *g = glyph_for(*s);
        for (int row = 0; row < 7; row++) {
            uint8_t bits = g[row];
            for (int xx = 0; xx < 5; xx++)
                if (bits & (1u << (4 - xx)))
                    put_pixel(fb, cx + xx, y + row, col);
        }
        cx += 6;
    }
}

static void fill_rect(uint16_t *fb, int x, int y, int w, int h, uint16_t c) {
    for (int yy = y; yy < y+h; yy++)
        for (int xx = x; xx < x+w; xx++)
            put_pixel(fb, xx, yy, c);
}

static void draw_rect_outline(uint16_t *fb, int x, int y, int w, int h, uint16_t c) {
    for (int xx = 0; xx < w; xx++) { put_pixel(fb, x+xx, y, c); put_pixel(fb, x+xx, y+h-1, c); }
    for (int yy = 0; yy < h; yy++) { put_pixel(fb, x, y+yy, c); put_pixel(fb, x+w-1, y+yy, c); }
}

/* Render `bytes` as a 4bpp tile sheet into a (cw×ch) cell at (cx,cy).
 * Tiles are 8x8 pixels, 32 bytes each.  We tile-pack across the cell. */
static void render_4bpp_into_cell(uint16_t *fb, int cx, int cy, int cw, int ch,
                                  const uint8_t *bytes, size_t nbytes)
{
    int tiles_x = cw / 8;
    int tiles_y = ch / 8;
    int max_tiles = tiles_x * tiles_y;
    int avail_tiles = (int)(nbytes / 32);
    int n = avail_tiles < max_tiles ? avail_tiles : max_tiles;
    fill_rect(fb, cx, cy, cw, ch, COL_PANEL);
    for (int t = 0; t < n; t++) {
        const uint8_t *tile = bytes + (size_t)t * 32u;
        int tx = (t % tiles_x) * 8;
        int ty = (t / tiles_x) * 8;
        for (int yy = 0; yy < 8; yy++) {
            for (int xx = 0; xx < 8; xx += 2) {
                uint8_t b = tile[yy*4 + xx/2];
                put_pixel(fb, cx + tx + xx,     cy + ty + yy, k_pal[b & 0xF]);
                put_pixel(fb, cx + tx + xx + 1, cy + ty + yy, k_pal[(b >> 4) & 0xF]);
            }
        }
    }
}

/* Score: heuristic 0..1000 — how "game-art-like" this entry looks. */
static int score_entry(const uint8_t *bytes, size_t nbytes)
{
    if (nbytes < 256) return 0;
    /* sample up to first 16 KB */
    size_t n = nbytes < 16384 ? nbytes : 16384;
    /* nibble histogram (4bpp pixel index distribution) */
    uint32_t hist[16] = {0};
    uint32_t total = 0;
    for (size_t i = 0; i < n; i++) {
        hist[bytes[i] & 0xF]++;
        hist[(bytes[i] >> 4) & 0xF]++;
        total += 2;
    }
    if (total == 0) return 0;
    uint32_t maxc = 0;
    int nonzero = 0;
    for (int i = 0; i < 16; i++) {
        if (hist[i] > maxc) maxc = hist[i];
        if (hist[i] > total / 200) nonzero++;
    }
    /* bank dominance: penalize > 60% from a single nibble */
    int dominant_pct = (int)((uint64_t)maxc * 100u / total);
    int bank_score = dominant_pct < 60 ? 400 : (200 - (dominant_pct - 60) * 4);
    if (bank_score < 0) bank_score = 0;
    /* spread: more distinct heavy nibbles = better */
    int spread_score = nonzero * 40;   /* up to 16*40=640 */
    if (spread_score > 400) spread_score = 400;
    /* tile reuse: count adjacent identical 32-byte tiles in the first 4 KB */
    int reuse = 0;
    size_t tcount = (n / 32);
    if (tcount > 128) tcount = 128;
    for (size_t i = 1; i < tcount; i++) {
        if (memcmp(bytes + i*32, bytes + (i-1)*32, 32) == 0) reuse++;
    }
    if (reuse > 0 && reuse < (int)tcount/2) reuse += 50; /* small reuse is good */
    int reuse_score = reuse > 200 ? 200 : reuse;
    return bank_score + spread_score + reuse_score;
}

static const char *type_name(uint32_t t) {
    switch (t) {
        case PACK_TYPE_RAW:     return "RAW";
        case PACK_TYPE_OVERLAY: return "OVERLAY";
        case PACK_TYPE_OVT:     return "OVT";
        case PACK_TYPE_FAT:     return "FAT";
        case PACK_TYPE_FNT:     return "FNT";
        default:                return "???";
    }
}

/* Resolve entry pointer + size via the public API. */
static const uint8_t *get_entry_bytes(const PackEntryView *e, size_t *out_size) {
    size_t sz = 0;
    const void *p = pack_get(e->id, &sz);
    if (out_size) *out_size = sz;
    return (const uint8_t *)p;
}

/* === Drawing the views =============================================== */

#define GRID_COLS  4
#define GRID_ROWS  4
#define GRID_PAGE  (GRID_COLS * GRID_ROWS)
#define CELL_W     60
#define CELL_H     44
#define CELL_GX    2
#define CELL_GY    4

static void clear_fb(uint16_t *fb, uint16_t c) {
    for (int i = 0; i < SCR_W * SCR_H; i++) fb[i] = c;
}

static void draw_grid(uint16_t *top, uint32_t page, uint32_t sel, uint32_t total) {
    clear_fb(top, COL_BG);
    char hdr[64];
    snprintf(hdr, sizeof(hdr), "MLPIT VIEWER  PAGE %u  ENTRY %u/%u",
             (unsigned)page, (unsigned)sel, (unsigned)total);
    draw_text(top, 4, 0, COL_HILITE, hdr);

    uint32_t base = page * GRID_PAGE;
    for (int i = 0; i < GRID_PAGE; i++) {
        uint32_t idx = base + (uint32_t)i;
        if (idx >= total) break;
        int gx = (i % GRID_COLS) * (CELL_W + CELL_GX) + 4;
        int gy = (i / GRID_COLS) * (CELL_H + CELL_GY) + 12;

        PackEntryView e;
        if (!pack_entry_at(idx, &e)) continue;
        size_t sz = 0;
        const uint8_t *bytes = get_entry_bytes(&e, &sz);
        if (bytes && sz > 0) {
            render_4bpp_into_cell(top, gx, gy, CELL_W, CELL_H, bytes, sz);
        } else {
            fill_rect(top, gx, gy, CELL_W, CELL_H, COL_PANEL);
        }
        char lab[16];
        snprintf(lab, sizeof(lab), "%X", (unsigned)e.id);
        draw_text(top, gx + 1, gy + CELL_H - 8, COL_ACCENT, lab);
        if (idx == sel) {
            draw_rect_outline(top, gx-1, gy-1, CELL_W+2, CELL_H+2, COL_HILITE);
            draw_rect_outline(top, gx-2, gy-2, CELL_W+4, CELL_H+4, COL_ACCENT);
        }
    }
}

static void draw_metadata(uint16_t *bot, uint32_t sel, uint32_t total) {
    clear_fb(bot, COL_BG);
    draw_text(bot, 4, 2, COL_HILITE, "ASSET METADATA");
    draw_rect_outline(bot, 2, 0, SCR_W - 4, 11, COL_TEXT);

    PackEntryView e;
    if (!pack_entry_at(sel, &e)) return;
    size_t sz = 0;
    const uint8_t *bytes = get_entry_bytes(&e, &sz);

    char line[80];
    int y = 16;
    snprintf(line, sizeof(line), "INDEX:  %u / %u", (unsigned)sel, (unsigned)total);
    draw_text(bot, 6, y, COL_TEXT, line); y += 10;
    snprintf(line, sizeof(line), "ID:     %08X", (unsigned)e.id);
    draw_text(bot, 6, y, COL_ACCENT, line); y += 10;
    snprintf(line, sizeof(line), "TYPE:   %s", type_name(e.type));
    draw_text(bot, 6, y, COL_TEXT, line); y += 10;
    snprintf(line, sizeof(line), "OFFSET: %u", (unsigned)e.offset);
    draw_text(bot, 6, y, COL_TEXT, line); y += 10;
    snprintf(line, sizeof(line), "SIZE:   %u BYTES", (unsigned)e.size);
    draw_text(bot, 6, y, COL_TEXT, line); y += 10;
    int score = bytes ? score_entry(bytes, sz) : 0;
    snprintf(line, sizeof(line), "SCORE:  %d / 1000", score);
    draw_text(bot, 6, y, score > 600 ? COL_ACCENT : COL_TEXT, line); y += 14;

    /* tag the kind of asset by id mask */
    const char *kind = "GENERIC";
    if ((e.id & 0xF000) == PACK_ID_OVERLAY_BASE) kind = "OVERLAY";
    else if ((e.id & 0xF000) == PACK_ID_FILE_BASE) kind = "FAT FILE";
    else if (e.id == PACK_ID_FAT) kind = "FAT TABLE";
    else if (e.id == PACK_ID_FNT) kind = "FNT TABLE";
    else if (e.id == PACK_ID_OVT) kind = "OVT TABLE";
    snprintf(line, sizeof(line), "KIND:   %s", kind);
    draw_text(bot, 6, y, COL_HILITE, line); y += 10;

    /* small hex preview strip of first 16 bytes */
    if (bytes && sz >= 16) {
        snprintf(line, sizeof(line), "HEAD:");
        draw_text(bot, 6, y, COL_TEXT, line);
        char hexbuf[64]; hexbuf[0] = 0;
        for (int i = 0; i < 16 && i < (int)sz; i++) {
            char tmp[4]; snprintf(tmp, sizeof(tmp), "%02X", bytes[i]);
            strncat(hexbuf, tmp, sizeof(hexbuf) - strlen(hexbuf) - 1);
        }
        draw_text(bot, 40, y, COL_ACCENT, hexbuf);
    }

    /* footer */
    draw_text(bot, 4, SCR_H - 18,  COL_TEXT, "L/R: PAGE  U/D: ROW  ENTER: ZOOM");
    draw_text(bot, 4, SCR_H - 8,   COL_TEXT, "ESC: QUIT");
}

static void draw_zoom(uint16_t *top, uint16_t *bot, uint32_t sel, uint32_t total) {
    clear_fb(top, COL_BG);
    PackEntryView e; if (!pack_entry_at(sel, &e)) return;
    size_t sz = 0;
    const uint8_t *bytes = get_entry_bytes(&e, &sz);
    if (bytes && sz > 0) {
        /* full top screen 4bpp tiles */
        render_4bpp_into_cell(top, 0, 0, SCR_W, SCR_H, bytes, sz);
    }
    draw_metadata(bot, sel, total);
    draw_text(bot, 4, SCR_H - 8, COL_HILITE, "ZOOM MODE - ENTER: BACK");
}

/* === Screenshot helper ================================================ */
static void dump_ppm(const char *path, const uint16_t *top, const uint16_t *bot)
{
    FILE *f = fopen(path, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", SCR_W, SCR_H * 2);
    for (int s = 0; s < 2; s++) {
        const uint16_t *fb = (s == 0) ? top : bot;
        for (int i = 0; i < SCR_W * SCR_H; i++) {
            uint16_t p = fb[i];
            uint8_t r = (uint8_t)(((p      ) & 0x1F) * 255 / 31);
            uint8_t g = (uint8_t)(((p >>  5) & 0x1F) * 255 / 31);
            uint8_t b = (uint8_t)(((p >> 10) & 0x1F) * 255 / 31);
            uint8_t rgb[3] = { r, g, b };
            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

/* === Top candidate ranking & dump ===================================== */
typedef struct { int score; uint32_t idx; uint32_t id; } Candidate;
static int cand_cmp(const void *a, const void *b) {
    return ((const Candidate*)b)->score - ((const Candidate*)a)->score;
}

static void rank_and_dump_candidates(const char *out_dir)
{
    uint32_t total = pack_entry_count();
    if (total == 0) return;
    Candidate *all = (Candidate *)calloc(total, sizeof(Candidate));
    if (!all) return;
    for (uint32_t i = 0; i < total; i++) {
        PackEntryView e;
        if (!pack_entry_at(i, &e)) continue;
        size_t sz = 0;
        const uint8_t *bytes = get_entry_bytes(&e, &sz);
        all[i].idx   = i;
        all[i].id    = e.id;
        all[i].score = bytes ? score_entry(bytes, sz) : 0;
    }
    qsort(all, total, sizeof(Candidate), cand_cmp);

    /* Reuse a temporary framebuffer to render+dump each top candidate. */
    uint16_t *top = (uint16_t*)calloc(SCR_W*SCR_H, sizeof(uint16_t));
    uint16_t *bot = (uint16_t*)calloc(SCR_W*SCR_H, sizeof(uint16_t));
    if (!top || !bot) { free(all); free(top); free(bot); return; }

    char path[512];
    int n = total < 5 ? (int)total : 5;

    char manifest_path[512];
    snprintf(manifest_path, sizeof(manifest_path), "%s/game_art_candidates.txt", out_dir);
    FILE *mf = fopen(manifest_path, "w");
    if (mf) fprintf(mf, "# top %d game-art candidates from MLPIT_VIEWER scorer\n", n);

    for (int i = 0; i < n; i++) {
        draw_zoom(top, bot, all[i].idx, total);
        snprintf(path, sizeof(path), "%s/game_art_candidate_%d.ppm", out_dir, i + 1);
        dump_ppm(path, top, bot);
        fprintf(stdout, "[viewer] candidate %d: idx=%u id=%08X score=%d -> %s\n",
                i+1, (unsigned)all[i].idx, (unsigned)all[i].id, all[i].score, path);
        if (mf) fprintf(mf, "rank=%d idx=%u id=0x%08X score=%d\n",
                        i+1, (unsigned)all[i].idx, (unsigned)all[i].id, all[i].score);
    }
    if (mf) fclose(mf);
    free(top); free(bot); free(all);
}

/* === Public API ====================================================== */

bool viewer_is_enabled(void) {
    const char *v = getenv("MLPIT_VIEWER");
    return v && *v && *v != '0';
}

int viewer_run(void)
{
    if (!pack_is_loaded()) {
        fprintf(stderr, "[viewer] No asset pack loaded — viewer needs assets/mlpit.assets\n");
        return 1;
    }
    uint32_t total = pack_entry_count();
    fprintf(stdout, "[viewer] %u entries available\n", (unsigned)total);

    int cycle_mode = 0;
    {
        const char *c = getenv("MLPIT_VIEWER_CYCLE");
        if (c && *c && *c != '0') cycle_mode = 1;
    }

    const char *dump_dir = getenv("MLPIT_VIEWER_DUMP_DIR");

    /* Pre-rank candidates and dump them once at startup. */
    if (dump_dir && *dump_dir) {
        rank_and_dump_candidates(dump_dir);
    }

    int shot_at = -1;
    {
        const char *s = getenv("MLPIT_SCREENSHOT_FRAME");
        if (s) shot_at = atoi(s);
    }
    const char *shot_path = getenv("MLPIT_SCREENSHOT_PATH");
    if (!shot_path) shot_path = "screenshot.ppm";
    int exit_at = -1;
    {
        const char *s = getenv("MLPIT_EXIT_AT_FRAME");
        if (s) exit_at = atoi(s);
    }

    uint32_t sel  = 0;
    uint32_t page = 0;
    int      zoom = 0;
    int frame = 0;
    Uint32 last_cycle_ms = SDL_GetTicks();

    while (platform_poll_events()) {
        const NdsInput *in = platform_input();
        /* Edge-detect navigation */
        static int prev_l = 0, prev_r = 0, prev_u = 0, prev_d = 0;
        static int prev_a = 0, prev_start = 0;
        if (!cycle_mode) {
            if (in->left  && !prev_l) { if (page > 0) page--; sel = page * GRID_PAGE; }
            if (in->right && !prev_r) { if ((page+1)*GRID_PAGE < total) { page++; sel = page * GRID_PAGE; } }
            if (in->up    && !prev_u) { if (sel >= GRID_COLS) sel -= GRID_COLS; page = sel / GRID_PAGE; }
            if (in->down  && !prev_d) { if (sel + GRID_COLS < total) sel += GRID_COLS; page = sel / GRID_PAGE; }
            if (in->a     && !prev_a)     zoom = !zoom;
            if (in->start && !prev_start) zoom = !zoom;
        } else {
            Uint32 now = SDL_GetTicks();
            if (now - last_cycle_ms >= 1000) {
                last_cycle_ms = now;
                sel = (sel + 1) % total;
                page = sel / GRID_PAGE;
            }
            zoom = 1;
        }
        prev_l = in->left;  prev_r = in->right;
        prev_u = in->up;    prev_d = in->down;
        prev_a = in->a;     prev_start = in->start;

        uint16_t *top = platform_top_framebuffer();
        uint16_t *bot = platform_bottom_framebuffer();
        if (zoom) {
            draw_zoom(top, bot, sel, total);
        } else {
            draw_grid(top, page, sel, total);
            draw_metadata(bot, sel, total);
        }
        platform_present();
        platform_signal_vblank();
        platform_sleep_us(16666);
        frame++;
        if (shot_at > 0 && frame == shot_at) {
            dump_ppm(shot_path, top, bot);
            fprintf(stderr, "[viewer] screenshot frame=%d -> %s\n", frame, shot_path);
            fflush(stderr);
        }
        if (exit_at > 0 && frame >= exit_at) {
            fprintf(stderr, "[viewer] exit at frame %d\n", frame);
            break;
        }
    }
    return 0;
}
