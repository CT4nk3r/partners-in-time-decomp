/*
 * nds_bg_render.c - NDS text-mode BG software rasterizer
 *
 * Reads DISPCNT and BG control/scroll registers from the NDS I/O shadow,
 * reads tile/map data from emulated VRAM banks, and outputs 256x192
 * BGR555 framebuffers for top and bottom screens.
 *
 * Only text mode (BG mode 0-2, 4bpp tiles, 32x32 or 64x64 maps) is supported.
 * Affine and 3D modes are not implemented yet.
 */
#include "nds_platform.h"
#include <stdint.h>
#include <string.h>

/* NDS I/O register addresses */
#define REG_DISPCNT     0x04000000
#define REG_BG0CNT      0x04000008
#define REG_BG1CNT      0x0400000A
#define REG_BG2CNT      0x0400000C
#define REG_BG3CNT      0x0400000E
#define REG_BG0HOFS     0x04000010
#define REG_BG0VOFS     0x04000012
#define REG_BG1HOFS     0x04000014
#define REG_BG1VOFS     0x04000016
#define REG_BG2HOFS     0x04000018
#define REG_BG2VOFS     0x0400001A
#define REG_BG3HOFS     0x0400001C
#define REG_BG3VOFS     0x0400001E

#define BG_MAP_BLOCK_SIZE  0x800
#define BG_TILE_BLOCK_SIZE 0x4000
#define TILE_4BPP_SIZE     32

/*
 * Per-engine VRAM mirror layout
 *
 *   Main engine BG VRAM @ NDS 0x06000000:  banks A (128 KB) + B (128 KB)
 *     → g_vram_main[256 KB]
 *
 *   Sub  engine BG VRAM @ NDS 0x06200000:  bank D (128 KB) primary, with
 *                                          bank C as a legacy fallback so
 *                                          old boot-hook content (which used
 *                                          to write bank C for sub) is still
 *                                          visible until it's migrated.
 *     → g_vram_sub[128 KB]
 *
 *   Main palette  @ NDS 0x05000000:  first  512 B of bank E
 *   Sub  palette  @ NDS 0x05000400:  second 512 B of bank E
 */
static uint8_t g_palette_main[512];
static uint8_t g_palette_sub[512];
/* Extended palettes: per-BG 256-color palettes for 8bpp mode.
 * On NDS hardware, these live in VRAM banks F/G; we pack them into Bank E
 * as consecutive 512-byte slots: [0]=BG0, [512]=BG1, [1024]=BG2, [1536]=BG3 */
static uint8_t g_ext_palette_main[4][512];
static uint8_t g_ext_palette_sub[4][512];
static uint8_t g_vram_main[256 * 1024];
static uint8_t g_vram_sub[128 * 1024];

void bg_render_sync_vram(void) {
    void *bank_a = nds_vram_bank('A');
    void *bank_b = nds_vram_bank('B');
    void *bank_c = nds_vram_bank('C');
    void *bank_d = nds_vram_bank('D');

    /* Main engine: A then B contiguous. */
    if (bank_a) memcpy(g_vram_main,              bank_a, 128 * 1024);
    if (bank_b) memcpy(g_vram_main + 128 * 1024, bank_b, 128 * 1024);

    /* Sub engine: prefer bank D (the conventional sub BG VRAM bank).
     * If bank D is all-zero (nothing has been written yet) and bank C
     * is non-zero, fall back to bank C so legacy content stays visible
     * during the migration. */
    if (bank_d) {
        memcpy(g_vram_sub, bank_d, 128 * 1024);
    }
    if (bank_c) {
        int d_empty = 1;
        if (bank_d) {
            const uint8_t *p = (const uint8_t *)bank_d;
            for (int i = 0; i < 4096; i++) { if (p[i]) { d_empty = 0; break; } }
        }
        if (d_empty) memcpy(g_vram_sub, bank_c, 128 * 1024);
    }

    void *bank_e = nds_vram_bank('E');
    if (bank_e) {
        /* Real NDS palette RAM layout (bank E mapped to 0x05000000):
         *   0x000..0x1FF  main BG palette
         *   0x200..0x3FF  main OBJ palette
         *   0x400..0x5FF  sub BG palette
         *   0x600..0x7FF  sub OBJ palette
         */
        memcpy(g_palette_main, bank_e,                    512);
        memcpy(g_palette_sub,  (uint8_t*)bank_e + 0x400,  512);

        /* Extended palettes: stored at Bank E offset 0x1000+, one 512-byte
         * slot per BG (0x1000=BG0, 0x1200=BG1, 0x1400=BG2, 0x1600=BG3).
         * Falls back to copying from the standard palette slot layout
         * (i*512) if the extended area is empty. */
        for (int i = 0; i < 4; i++) {
            int ext_off = 0x1000 + i * 512;
            const uint8_t *src = (const uint8_t*)bank_e + ext_off;
            /* Check if extended palette area has data */
            int has_data = 0;
            for (int j = 0; j < 32; j++) { if (src[j]) { has_data = 1; break; } }
            if (has_data) {
                memcpy(g_ext_palette_main[i], src, 512);
            } else {
                /* Fallback: use standard palette layout */
                int off = i * 512;
                if ((uint32_t)(off + 512) <= 64 * 1024)
                    memcpy(g_ext_palette_main[i], (uint8_t*)bank_e + off, 512);
            }
        }
        /* Sub engine extended palettes: offset 0x400 onward */
        for (int i = 0; i < 4; i++) {
            int off = 0x400 + i * 512;
            if ((uint32_t)(off + 512) <= 64 * 1024)
                memcpy(g_ext_palette_sub[i], (uint8_t*)bank_e + off, 512);
        }
    }
}

static void render_bg_layer(uint16_t *fb, const uint8_t *vram,
                             const uint8_t *palette,
                             const uint8_t ext_pal[4][512],
                             int bg_index, uint16_t bgcnt,
                             int hofs, int vofs, int use_ext_pal)
{
    int char_base   = ((bgcnt >> 2) & 0xF);
    int screen_base = ((bgcnt >> 8) & 0x1F);
    int color_mode  = (bgcnt >> 7) & 1;
    int screen_size = (bgcnt >> 14) & 3;

    int map_tiles_x = (screen_size & 1) ? 64 : 32;
    int map_tiles_y = (screen_size & 2) ? 64 : 32;
    int map_px_w = map_tiles_x * 8;
    int map_px_h = map_tiles_y * 8;

    const uint8_t *tile_base = vram + char_base * BG_TILE_BLOCK_SIZE;
    const uint8_t *map_base  = vram + screen_base * BG_MAP_BLOCK_SIZE;

    for (int y = 0; y < NDS_SCREEN_HEIGHT; y++) {
        int map_y = ((y + vofs) & (map_px_h - 1));
        int tile_row = map_y / 8;
        int tile_py  = map_y % 8;

        for (int x = 0; x < NDS_SCREEN_WIDTH; x++) {
            int map_x = ((x + hofs) & (map_px_w - 1));
            int tile_col = map_x / 8;
            int tile_px  = map_x % 8;

            int block_x = tile_col / 32;
            int block_y = tile_row / 32;
            int local_col = tile_col % 32;
            int local_row = tile_row % 32;

            int block_off = 0;
            if (screen_size == 1) block_off = block_x * 0x800;
            else if (screen_size == 2) block_off = block_y * 0x800;
            else if (screen_size == 3) block_off = (block_y * 2 + block_x) * 0x800;

            int map_off = block_off + (local_row * 32 + local_col) * 2;
            uint16_t entry = (uint16_t)(map_base[map_off] | (map_base[map_off + 1] << 8));

            int tile_num  = entry & 0x3FF;
            int h_flip    = (entry >> 10) & 1;
            int v_flip    = (entry >> 11) & 1;
            int pal_bank  = (entry >> 12) & 0xF;

            int px = h_flip ? (7 - tile_px) : tile_px;
            int py = v_flip ? (7 - tile_py) : tile_py;

            uint8_t color_idx;
            const uint8_t *pal_src;
            if (!color_mode) {
                int byte_off = tile_num * TILE_4BPP_SIZE + py * 4 + px / 2;
                uint8_t b = tile_base[byte_off];
                color_idx = (px & 1) ? (b >> 4) : (b & 0xF);
                if (color_idx == 0) continue;
                /* In extended palette mode (4bpp), use per-BG palette */
                if (use_ext_pal) {
                    color_idx += pal_bank * 16;
                    pal_src = ext_pal[bg_index];
                } else {
                    color_idx += pal_bank * 16;
                    pal_src = palette;
                }
            } else {
                int byte_off = tile_num * 64 + py * 8 + px;
                color_idx = tile_base[byte_off];
                if (color_idx == 0) continue;
                /* 8bpp: use per-BG extended palette if available */
                pal_src = ext_pal[bg_index];
            }

            uint16_t pal_entry = (uint16_t)(pal_src[color_idx * 2] | (pal_src[color_idx * 2 + 1] << 8));
            fb[y * NDS_SCREEN_WIDTH + x] = pal_entry;
        }
    }
}

void bg_render_top(uint16_t *fb) {
    uint32_t dispcnt = nds_reg_read32(REG_DISPCNT);
    int bg_mode = dispcnt & 7;

    /* Fill with backdrop color (palette entry 0) instead of black */
    uint16_t backdrop = (uint16_t)(g_palette_main[0] | (g_palette_main[1] << 8));
    backdrop &= 0x7FFF;  /* mask off alpha bit */
    for (int i = 0; i < NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT; i++)
        fb[i] = backdrop;
    if (bg_mode > 2) return;

    int bg_enable[4];
    bg_enable[0] = (dispcnt >> 8) & 1;
    bg_enable[1] = (dispcnt >> 9) & 1;
    bg_enable[2] = (dispcnt >> 10) & 1;
    bg_enable[3] = (dispcnt >> 11) & 1;

    uint16_t bgcnt[4];
    int hofs[4], vofs[4];
    bgcnt[0] = nds_reg_read16(REG_BG0CNT);
    bgcnt[1] = nds_reg_read16(REG_BG1CNT);
    bgcnt[2] = nds_reg_read16(REG_BG2CNT);
    bgcnt[3] = nds_reg_read16(REG_BG3CNT);
    hofs[0]  = nds_reg_read16(REG_BG0HOFS) & 0x1FF;
    vofs[0]  = nds_reg_read16(REG_BG0VOFS) & 0x1FF;
    hofs[1]  = nds_reg_read16(REG_BG1HOFS) & 0x1FF;
    vofs[1]  = nds_reg_read16(REG_BG1VOFS) & 0x1FF;
    hofs[2]  = nds_reg_read16(REG_BG2HOFS) & 0x1FF;
    vofs[2]  = nds_reg_read16(REG_BG2VOFS) & 0x1FF;
    hofs[3]  = nds_reg_read16(REG_BG3HOFS) & 0x1FF;
    vofs[3]  = nds_reg_read16(REG_BG3VOFS) & 0x1FF;

    /* Check if extended palettes are enabled (DISPCNT bit 30) */
    int use_ext_pal = (dispcnt >> 30) & 1;

    for (int pri = 3; pri >= 0; pri--) {
        for (int bg = 3; bg >= 0; bg--) {
            if (!bg_enable[bg]) continue;
            if ((bgcnt[bg] & 3) != (uint16_t)pri) continue;
            render_bg_layer(fb, g_vram_main, g_palette_main,
                            g_ext_palette_main, bg,
                            bgcnt[bg], hofs[bg], vofs[bg], use_ext_pal);
        }
    }

    /* Apply master brightness (REG_MASTER_BRIGHT at 0x0400006C).
     * Bits 0-4: factor (0=no change, 16=full effect)
     * Bits 14-15: mode (0=off, 1=darken, 2=brighten) */
    uint16_t bright = nds_reg_read16(0x0400006Cu);
    int brt_mode   = (bright >> 14) & 3;
    int brt_factor = bright & 0x1F;
    if (brt_factor > 16) brt_factor = 16;
    if (brt_mode && brt_factor) {
        for (int i = 0; i < NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT; i++) {
            uint16_t c = fb[i];
            int r = c & 0x1F;
            int g = (c >> 5) & 0x1F;
            int b = (c >> 10) & 0x1F;
            if (brt_mode == 1) { /* darken toward black */
                r = r - (r * brt_factor / 16);
                g = g - (g * brt_factor / 16);
                b = b - (b * brt_factor / 16);
            } else { /* brighten toward white */
                r = r + ((31 - r) * brt_factor / 16);
                g = g + ((31 - g) * brt_factor / 16);
                b = b + ((31 - b) * brt_factor / 16);
            }
            fb[i] = (uint16_t)((b << 10) | (g << 5) | r);
        }
    }
}

void bg_render_bottom(uint16_t *fb) {
    uint32_t dispcnt = nds_reg_read32(0x04001000);
    int bg_mode = dispcnt & 7;

    memset(fb, 0, NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT * 2);
    if (bg_mode > 2) return;

    int bg_enable[4];
    bg_enable[0] = (dispcnt >> 8) & 1;
    bg_enable[1] = (dispcnt >> 9) & 1;
    bg_enable[2] = (dispcnt >> 10) & 1;
    bg_enable[3] = (dispcnt >> 11) & 1;

    uint16_t bgcnt[4];
    int hofs[4], vofs[4];
    bgcnt[0] = nds_reg_read16(0x04001008);
    bgcnt[1] = nds_reg_read16(0x0400100A);
    bgcnt[2] = nds_reg_read16(0x0400100C);
    bgcnt[3] = nds_reg_read16(0x0400100E);
    hofs[0]  = nds_reg_read16(0x04001010) & 0x1FF;
    vofs[0]  = nds_reg_read16(0x04001012) & 0x1FF;
    hofs[1]  = nds_reg_read16(0x04001014) & 0x1FF;
    vofs[1]  = nds_reg_read16(0x04001016) & 0x1FF;
    hofs[2]  = nds_reg_read16(0x04001018) & 0x1FF;
    vofs[2]  = nds_reg_read16(0x0400101A) & 0x1FF;
    hofs[3]  = nds_reg_read16(0x0400101C) & 0x1FF;
    vofs[3]  = nds_reg_read16(0x0400101E) & 0x1FF;

    for (int pri = 3; pri >= 0; pri--) {
        for (int bg = 3; bg >= 0; bg--) {
            if (!bg_enable[bg]) continue;
            if ((bgcnt[bg] & 3) != (uint16_t)pri) continue;
            render_bg_layer(fb, g_vram_sub, g_palette_sub,
                            g_ext_palette_sub, bg,
                            bgcnt[bg], hofs[bg], vofs[bg], 0);
        }
    }
}

/* ──────────────────────────────────────────────────────────────
 * OAM rasterizer — proper 4bpp tile-based sprite rendering
 *
 * Walks 128 OAM entries, fetches 4bpp tiles from OBJ VRAM, decodes
 * with OBJ palette, and composites onto the framebuffer with
 * transparency (palette index 0 = transparent).
 * ────────────────────────────────────────────────────────────── */
extern uint8_t g_oam_main[1024];
extern uint8_t g_oam_sub [1024];

#define REG_DISPCNT_MAIN 0x04000000
#define REG_DISPCNT_SUB  0x04001000

/* OBJ VRAM and palette buffers (populated by gameplay/scene code) */
static uint8_t g_obj_vram_main[256 * 1024];  /* main engine OBJ tiles */
static uint8_t g_obj_vram_sub [128 * 1024];  /* sub engine OBJ tiles */
static uint8_t g_obj_palette_main[512];       /* 16 palettes × 16 colors × 2B */
static uint8_t g_obj_palette_sub [512];

/* Public accessors for scenes to load sprite data */
uint8_t *obj_vram_main_ptr(void)    { return g_obj_vram_main; }
uint8_t *obj_vram_sub_ptr(void)     { return g_obj_vram_sub; }
uint8_t *obj_palette_main_ptr(void) { return g_obj_palette_main; }
uint8_t *obj_palette_sub_ptr(void)  { return g_obj_palette_sub; }
uint32_t obj_vram_main_size(void)   { return sizeof(g_obj_vram_main); }

/* Sync OBJ palette from Bank E (offset 0x200 = main OBJ, 0x600 = sub OBJ) */
void obj_render_sync_palette(void) {
    void *bank_e = nds_vram_bank('E');
    if (bank_e) {
        memcpy(g_obj_palette_main, (uint8_t*)bank_e + 0x200, 512);
        if (0x600 + 512 <= 64 * 1024)
            memcpy(g_obj_palette_sub, (uint8_t*)bank_e + 0x600, 512);
    }
}

static const int oam_size_table[4][4][2] = {
    /* shape 0: square */
    { {8,8},   {16,16}, {32,32}, {64,64} },
    /* shape 1: horizontal */
    { {16,8},  {32,8},  {32,16}, {64,32} },
    /* shape 2: vertical */
    { {8,16},  {8,32},  {16,32}, {32,64} },
    /* shape 3: reserved → treat as 8x8 */
    { {8,8},   {8,8},   {8,8},   {8,8}   },
};

/* Render a single 8x8 4bpp tile onto the framebuffer */
static void render_obj_tile_4bpp(uint16_t *fb, const uint8_t *tile_data,
                                  const uint16_t *palette,
                                  int dst_x, int dst_y,
                                  int flip_h, int flip_v)
{
    for (int ty = 0; ty < 8; ty++) {
        int src_y = flip_v ? (7 - ty) : ty;
        int screen_y = dst_y + ty;
        if (screen_y < 0 || screen_y >= NDS_SCREEN_HEIGHT) continue;
        uint16_t *row = fb + screen_y * NDS_SCREEN_WIDTH;

        for (int tx = 0; tx < 8; tx++) {
            int src_x = flip_h ? (7 - tx) : tx;
            int screen_x = dst_x + tx;
            if (screen_x < 0 || screen_x >= NDS_SCREEN_WIDTH) continue;

            /* 4bpp: each byte holds 2 pixels (low nibble first) */
            uint8_t byte = tile_data[src_y * 4 + src_x / 2];
            uint8_t idx = (src_x & 1) ? (byte >> 4) : (byte & 0x0F);

            if (idx == 0) continue;  /* transparent */
            row[screen_x] = palette[idx];
        }
    }
}

void obj_render(uint16_t *fb, int is_sub) {
    uint32_t dispcnt = nds_reg_read32(is_sub ? REG_DISPCNT_SUB : REG_DISPCNT_MAIN);

    /* DISPCNT bit 12 = OBJ enable */
    if (!((dispcnt >> 12) & 1)) return;

    /* DISPCNT bits 20-21: OBJ character mapping
     * 0 = 2D mapping (32-byte boundary)
     * 1 = 1D mapping, boundary = 32 << bits[20:21]
     * For simplicity we default to 1D/32-byte boundary (most common). */
    int obj_1d = (dispcnt >> 4) & 1;  /* bit 4 of DISPCNT = OBJ 1D mapping */
    int tile_boundary = 32;  /* default */
    if (obj_1d) {
        int map_bits = (dispcnt >> 20) & 3;
        tile_boundary = 32 << map_bits;  /* 32, 64, 128, or 256 */
    }

    const uint8_t *oam = is_sub ? g_oam_sub : g_oam_main;
    const uint8_t *obj_vram = is_sub ? g_obj_vram_sub : g_obj_vram_main;
    const uint8_t *obj_pal = is_sub ? g_obj_palette_sub : g_obj_palette_main;
    uint32_t vram_size = is_sub ? sizeof(g_obj_vram_sub) : sizeof(g_obj_vram_main);
    int drawn = 0;

    /* Render back-to-front (higher OAM index = lower priority on ties) */
    for (int i = 127; i >= 0; i--) {
        uint16_t attr0 = (uint16_t)(oam[i*8 + 0] | (oam[i*8 + 1] << 8));
        uint16_t attr1 = (uint16_t)(oam[i*8 + 2] | (oam[i*8 + 3] << 8));
        uint16_t attr2 = (uint16_t)(oam[i*8 + 4] | (oam[i*8 + 5] << 8));

        int rot_scale = (attr0 >> 8) & 1;
        int disabled  = !rot_scale && ((attr0 >> 9) & 1);
        if (disabled) continue;
        if (attr0 == 0 && attr1 == 0 && attr2 == 0) continue;

        int y     = attr0 & 0xFF;
        int shape = (attr0 >> 14) & 3;
        int bpp8  = (attr0 >> 13) & 1;  /* 0=4bpp, 1=8bpp */
        int x     = attr1 & 0x1FF;
        if (x & 0x100) x -= 0x200;
        int size   = (attr1 >> 14) & 3;
        int flip_h = !rot_scale && ((attr1 >> 12) & 1);
        int flip_v = !rot_scale && ((attr1 >> 13) & 1);

        int w = oam_size_table[shape][size][0];
        int h = oam_size_table[shape][size][1];

        int tile_num = attr2 & 0x03FF;
        int pal_num  = (attr2 >> 12) & 0x0F;

        /* Skip 8bpp for now */
        if (bpp8) continue;

        /* 4bpp palette: 16 colors starting at pal_num * 16 in OBJ palette */
        const uint16_t *palette = (const uint16_t *)(obj_pal + pal_num * 32);

        /* Render tile grid (w/8 × h/8 tiles).
         * In 1D mapping: tiles are sequential in memory.
         * In 2D mapping: row stride is 32 tiles (256 pixels / 8). */
        int tiles_w = w / 8;
        int tiles_h = h / 8;

        for (int row_t = 0; row_t < tiles_h; row_t++) {
            for (int col_t = 0; col_t < tiles_w; col_t++) {
                int render_col = flip_h ? (tiles_w - 1 - col_t) : col_t;
                int render_row = flip_v ? (tiles_h - 1 - row_t) : row_t;

                int tile_idx;
                if (obj_1d) {
                    tile_idx = tile_num + render_row * tiles_w + render_col;
                } else {
                    /* 2D mapping: row stride is 32 tiles */
                    tile_idx = tile_num + render_row * 32 + render_col;
                }

                uint32_t tile_offset = (uint32_t)tile_idx * 32;  /* 4bpp = 32 bytes/tile */
                if (tile_offset + 32 > vram_size) continue;

                const uint8_t *tile_data = obj_vram + tile_offset;

                int dst_x = x + col_t * 8;
                int dst_y = y + row_t * 8;
                /* Wrap Y at 256 (NDS OBJ Y wraps) */
                if (dst_y >= 192) dst_y -= 256;

                render_obj_tile_4bpp(fb, tile_data, palette,
                                     dst_x, dst_y, flip_h, flip_v);
            }
        }
        drawn++;
    }

    static int once[2] = {0,0};
    if (drawn && !once[is_sub & 1]) {
        once[is_sub & 1] = 1;
        extern void nds_log(const char *fmt, ...);
        nds_log("[obj-render] %s engine: drew %d sprite(s) (DISPCNT=%08X)\n",
                is_sub ? "SUB" : "MAIN", drawn, dispcnt);
    }
}
