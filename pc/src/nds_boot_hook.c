/*
 * nds_boot_hook.c - Pre-game VRAM population and diagnostic asset viewer
 *
 * Three modes:
 *  1. boot_hook_vram()           – synthetic test pattern (no assets needed)
 *  2. boot_hook_real_tiles()     – load first recognisable tiles from the game
 *                                  data (requires asset pack)
 *  3. show_asset_in_vram(index)  – raw FAT file bytes treated as 4bpp tiles
 *
 * boot_hook_real_tiles() strategy:
 *   - Asset 0x62 (PACK_ID_FILE(0x62) = 0x2062) is a large raw 4bpp tile sheet.
 *     The first 32 KB covers char block 0 (up to 1024 tiles).
 *   - A game palette is constructed from asset 0x01 (overlay 1 palette at
 *     PACK_ID_OVERLAY(1) = 0x1001) when present and non-zero, otherwise a
 *     synthetic greyscale palette is used.
 *   - Both write to VRAM banks A/E and configure IO shadow registers so
 *     bg_render_top() draws them on the first frame.
 *
 * VRAM layout (bank A, 128 KB):
 *   offset 0x00000 – 0x1FFFF  char block 0  (tiles, up to 4096 × 32 B)
 *   offset 0x08000 – 0x087FF  screen block 16  (32×32 tilemap, 2 KB)
 *
 * BG0CNT = 0x1000  (char_base=0, screen_base=16, 4bpp, 32×32, priority=0)
 * DISPCNT = 0x0100  (mode 0, BG0 enabled)
 */
#include "nds_boot_hook.h"
#include "nds_platform.h"
#include "asset_pack.h"
#include "mlpit_pack.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ── NDS register addresses (same as in nds_bg_render.c) ──────── */
#define REG_DISPCNT   0x04000000u
#define REG_BG0CNT    0x04000008u
#define REG_BG1CNT    0x0400000Au
#define REG_BG2CNT    0x0400000Cu
#define REG_BG3CNT    0x0400000Eu

/* ── VRAM layout constants ────────────────────────────────────── */
#define CHAR_BASE_OFFSET    0x00000u   /* tiles start here */
#define SCREEN_BASE_IDX     16u        /* screen_base = 16 → 16 × 0x800 = 0x8000 */
#define SCREEN_BASE_OFFSET  0x08000u   /* map starts here */
#define TILES_4BPP_BYTES    32u        /* bytes per 8×8 4bpp tile */
#define MAP_ENTRIES         (32 * 32)  /* 32×32 tile map */

/* ── BGR555 rainbow palette (16 colours) ─────────────────────── */
/* NDS BGR555: bits[4:0]=R, bits[9:5]=G, bits[14:10]=B            */
static const uint16_t k_rainbow_pal[16] = {
    0x0000,  /*  0: black / transparent (never drawn)  */
    0x001F,  /*  1: red                                */
    0x03E0,  /*  2: green                              */
    0x7C00,  /*  3: blue                               */
    0x03FF,  /*  4: yellow   (R+G)                     */
    0x7FE0,  /*  5: cyan     (G+B)                     */
    0x7C1F,  /*  6: magenta  (R+B)                     */
    0x7FFF,  /*  7: white                              */
    0x000A,  /*  8: dark red                           */
    0x0140,  /*  9: dark green                         */
    0x2800,  /* 10: dark blue                          */
    0x014A,  /* 11: olive                              */
    0x2940,  /* 12: teal                               */
    0x280A,  /* 13: purple                             */
    0x3DEF,  /* 14: silver                             */
    0x0C63,  /* 15: dark grey                          */
};

/* ── Private helpers ─────────────────────────────────────────── */

/* Write the 16-colour rainbow palette into VRAM bank E (main palette). */
static void write_palette(void)
{
    uint8_t *bank_e = (uint8_t *)nds_vram_bank('E');
    if (!bank_e) return;
    for (int i = 0; i < 16; i++) {
        bank_e[i * 2]     = (uint8_t)(k_rainbow_pal[i] & 0xFF);
        bank_e[i * 2 + 1] = (uint8_t)(k_rainbow_pal[i] >> 8);
    }
}

/*
 * Validate a 32-byte buffer as a 16-colour BGR555 palette.
 *   - bit 15 of every entry must be 0 (NDS hardware ignores it but real
 *     palettes set it to 0 universally; non-zero hits indicate misclassified
 *     binary code or graphics data).
 *   - At least 6 of 16 entries must be non-zero (rules out empty/stub slots).
 *   - At least 4 distinct values among the non-zero entries (rules out runs
 *     of 0xFFFF, debug fill bytes, etc.).
 */
static int looks_like_palette16(const uint8_t *b)
{
    int nz = 0, distinct = 0;
    uint16_t seen[16];
    for (int i = 0; i < 16; i++) {
        uint16_t v = (uint16_t)(b[i * 2] | (b[i * 2 + 1] << 8));
        if (v & 0x8000) return 0;
        if (v) {
            nz++;
            int dup = 0;
            for (int j = 0; j < distinct; j++) if (seen[j] == v) { dup = 1; break; }
            if (!dup) seen[distinct++] = v;
        }
    }
    return nz >= 6 && distinct >= 4;
}

/*
 * Try to parse a Nitro NCLR ("RLCN") palette container.  Layout:
 *   off   sz   field
 *    0    4   "RLCN"            (file magic, little-endian = "NCLR")
 *    4    2   byte-order mark   (0xFFFE)
 *    6    2   version
 *    8    4   total file size
 *   12    2   header size       (typically 0x10)
 *   14    2   section count     (1)
 *   16    4   "TTLP"            (PLTT section magic, "PLTT")
 *   20    4   section size
 *   24    4   palette bit-depth (3 = 4bpp/16-col, 4 = 8bpp/256-col)
 *   28    4   padding
 *   32    4   palette data size
 *   36    4   colours per palette (often 0)
 *   40    N   BGR555 colour data
 *
 * On success copies up to *out_max bytes of BGR555 data to out and updates
 * *out_max to the number of bytes written.  Returns 1 on success.
 */
static int parse_nclr(const uint8_t *src, size_t src_size,
                      uint8_t *out, uint32_t *out_max)
{
    if (src_size < 40 + 32) return 0;
    if (src[0] != 'R' || src[1] != 'L' || src[2] != 'C' || src[3] != 'N') return 0;
    if (src[16] != 'T' || src[17] != 'T' || src[18] != 'L' || src[19] != 'P') return 0;

    uint32_t pal_data_size = (uint32_t)src[32] | ((uint32_t)src[33] << 8) |
                             ((uint32_t)src[34] << 16) | ((uint32_t)src[35] << 24);
    if (pal_data_size == 0 || pal_data_size > src_size - 40) return 0;

    uint32_t copy = pal_data_size < *out_max ? pal_data_size : *out_max;
    memcpy(out, src + 40, copy);
    *out_max = copy;
    return 1;
}

/*
 * Search all asset-pack entries for the first plausible 16-colour BGR555
 * palette and copy it into out (32 bytes).  Looks at:
 *   1. Direct overlay-palette entries (0x1001..0x1024).
 *   2. NCLR-formatted blobs (any entry starting with "RLCN").
 *   3. 32-byte sub-blocks within offset-table archives (RAW FAT entries).
 *
 * Returns the asset id that supplied the palette (or 0 on failure).
 * out_count is set to the number of 16-colour palettes copied (1..16).
 */
static uint32_t find_game_palette(uint8_t *out, int max_palettes, int *out_count)
{
    *out_count = 0;
    if (!pack_is_loaded()) return 0;

    /* Pass 1: overlay-style palette IDs (most likely to be real). */
    for (uint32_t oid = 1; oid <= 0x24; oid++) {
        void *data = NULL; size_t sz = 0;
        if (!pack_get_overlay(oid, &data, &sz) || !data || sz < 32) continue;
        if (looks_like_palette16((const uint8_t *)data)) {
            int n = (int)(sz / 32);
            if (n > max_palettes) n = max_palettes;
            memcpy(out, data, (size_t)n * 32);
            *out_count = n;
            return 0x1000u | oid;
        }
    }

    /* Pass 2: scan FAT files for NCLR or palette-shaped sub-blocks. */
    for (uint32_t fid = 0; fid < 0x80; fid++) {
        void *data = NULL; size_t sz = 0;
        if (!pack_get_file(fid, &data, &sz) || !data || sz < 32) continue;
        const uint8_t *bytes = (const uint8_t *)data;

        /* NCLR container? */
        uint32_t cap = (uint32_t)max_palettes * 32u;
        if (parse_nclr(bytes, sz, out, &cap)) {
            *out_count = (int)(cap / 32);
            if (*out_count > 0) return 0x2000u | fid;
        }

        /* Offset-table archive sub-blocks. */
        if (sz < 8) continue;
        uint32_t first_off = (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
                             ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
        if (first_off < 4 || first_off > sz || (first_off % 4) != 0) continue;
        uint32_t n_ent = first_off / 4;
        if (n_ent < 1 || n_ent > 4096) continue;

        for (uint32_t j = 0; j < n_ent; j++) {
            uint32_t base = j * 4;
            if (base + 8 > sz) break;
            uint32_t a = (uint32_t)bytes[base]   | ((uint32_t)bytes[base+1] << 8) |
                         ((uint32_t)bytes[base+2] << 16) | ((uint32_t)bytes[base+3] << 24);
            uint32_t b = (j + 1 < n_ent)
                ? ((uint32_t)bytes[base+4] | ((uint32_t)bytes[base+5] << 8) |
                   ((uint32_t)bytes[base+6] << 16) | ((uint32_t)bytes[base+7] << 24))
                : (uint32_t)sz;
            if (b <= a || b > sz) continue;
            uint32_t sub_sz = b - a;
            if (sub_sz != 32) continue;
            if (looks_like_palette16(bytes + a)) {
                memcpy(out, bytes + a, 32);
                *out_count = 1;
                return 0x2000u | fid;
            }
        }
    }

    return 0;
}

/*
 * Write up to 16 16-colour palette banks (256 entries × 2B = 512 B) into
 * VRAM bank E starting at offset 0.  Replicates the first palette across
 * all 16 banks so tiles using palette-bank > 0 still have valid colours.
 */
static int write_real_palette(void)
{
    uint8_t *bank_e = (uint8_t *)nds_vram_bank('E');
    if (!bank_e) return 0;

    uint8_t pal[16 * 32];
    int     n_pal  = 0;
    uint32_t src_id = find_game_palette(pal, 16, &n_pal);
    if (n_pal <= 0) return 0;

    /* Lay each found palette in its own 32-byte slot, then replicate
     * the first palette across the remaining 16 banks so any tile that
     * references palette-bank N (N >= n_pal) still resolves to colours. */
    memset(bank_e, 0, 16 * 32);
    memcpy(bank_e, pal, (size_t)n_pal * 32);
    for (int i = n_pal; i < 16; i++) {
        memcpy(bank_e + i * 32, pal, 32);
    }

    /* Force colour 0 of palette bank 0 to a visible dark colour so the
     * background isn't pure black if the rasterizer ever draws idx 0
     * (it currently treats 0 as transparent, so this is purely cosmetic
     * for any future code paths that read palette[0] directly). */
    /* (left as-is — bank E[0..1] stays whatever the asset said) */

    nds_log("[boot_hook] Real palette loaded from asset 0x%04X "
            "(%d × 16 colours)\n", src_id, n_pal);
    return 1;
}

/*
 * Build a 32×32 tilemap (MAP_ENTRIES × 2 bytes) into the screen-block
 * area of VRAM bank A.
 *
 * tile_at(x,y) = (y * 32 + x) % num_tiles, clamped so index 0 is avoided
 * when possible (transparent tile).
 */
static void write_tilemap(uint8_t *bank_a, int num_tiles)
{
    uint8_t *map = bank_a + SCREEN_BASE_OFFSET;
    int usable = (num_tiles > 1) ? num_tiles - 1 : num_tiles;

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            int idx = (y * 32 + x) % usable;
            /* Skip tile 0 (transparent) by shifting indices up by 1 */
            if (usable > 1) idx += 1;
            uint16_t entry = (uint16_t)(idx & 0x3FF);
            int off = (y * 32 + x) * 2;
            map[off]     = (uint8_t)(entry & 0xFF);
            map[off + 1] = (uint8_t)(entry >> 8);
        }
    }
}

/*
 * Generate 256 synthetic 4bpp tiles.
 * Tile N is a checkerboard of two colours derived from N.
 */
static void generate_synthetic_tiles(uint8_t *bank_a)
{
    for (int t = 0; t < 256; t++) {
        /* Two non-transparent colours for this tile */
        uint8_t c1 = (uint8_t)((t % 15) + 1);       /* 1-15 cycling */
        uint8_t c2 = (uint8_t)(((t + 7) % 15) + 1); /* offset by 7 */

        for (int py = 0; py < 8; py++) {
            for (int bx = 0; bx < 4; bx++) {
                /* Two adjacent pixels packed in one byte */
                int px0 = bx * 2;
                int px1 = bx * 2 + 1;
                uint8_t col0 = (((px0 ^ py) & 1) ? c2 : c1);
                uint8_t col1 = (((px1 ^ py) & 1) ? c2 : c1);
                bank_a[t * TILES_4BPP_BYTES + py * 4 + bx] =
                    (uint8_t)(col0 | (col1 << 4));
            }
        }
    }
}

/* Configure the IO shadow to enable BG0 in mode 0. */
static void configure_bg_regs(void)
{
    /*
     * BG0CNT = 0x1000:
     *   bits[1:0] = 0  (priority 0, drawn in front)
     *   bits[5:2] = 0  (char_base = 0 → tiles at VRAM+0x0000)
     *   bit 7     = 0  (4bpp colour mode)
     *   bits[12:8]= 16 (screen_base=16 → map at VRAM+0x8000)
     *   bits[15:14]=0  (32×32 tile map)
     */
    nds_reg_write16(REG_BG0CNT, 0x1000u);

    /* Clear other BG layers (disabled by DISPCNT, but zero them anyway) */
    nds_reg_write16(REG_BG1CNT, 0x0000u);
    nds_reg_write16(REG_BG2CNT, 0x0000u);
    nds_reg_write16(REG_BG3CNT, 0x0000u);

    /*
     * DISPCNT = 0x0100:
     *   bits[2:0] = 0 (BG mode 0, text backgrounds)
     *   bit 8     = 1 (BG0 visible)
     *   bits[11:9]= 0 (BG1-3 hidden)
     */
    nds_reg_write32(REG_DISPCNT, 0x0100u);
}

/* ── Public API ──────────────────────────────────────────────── */

void boot_hook_vram(void)
{
    uint8_t *bank_a = (uint8_t *)nds_vram_bank('A');
    if (!bank_a) {
        nds_log("[boot_hook] VRAM bank A not available\n");
        return;
    }

    /* Clear the portion of bank A we'll use (tile area + map area) */
    memset(bank_a, 0, SCREEN_BASE_OFFSET + MAP_ENTRIES * 2);

    generate_synthetic_tiles(bank_a);
    write_tilemap(bank_a, 256);
    write_palette();
    configure_bg_regs();

    nds_log("[boot_hook] Synthetic VRAM loaded: 256 tiles, rainbow palette, "
            "32x32 tilemap -- BG0 enabled\n");
}

/*
 * boot_hook_real_tiles() — load the first 1024 tiles from the game's raw
 * tile sheet (FAT index 0x62 = asset 0x2062, 4bpp, ~206 K tiles) together
 * with the palette from overlay 1 (asset 0x1001, 16 colours).
 *
 * Returns 1 on success, 0 when the asset pack isn't loaded or files are absent.
 */
int boot_hook_real_tiles(void)
{
    if (!pack_is_loaded()) return 0;

    void  *tile_data  = NULL;
    size_t tile_size  = 0;

    /* FAT index 0x62 = asset ID 0x2062 (raw 4bpp tile sheet) */
    if (!pack_get_file(0x62, &tile_data, &tile_size) || !tile_data || tile_size == 0) {
        nds_log("[boot_hook] Real tiles: FAT[0x62] not found\n");
        return 0;
    }

    uint8_t *bank_a = (uint8_t *)nds_vram_bank('A');
    if (!bank_a) return 0;

    /* Load up to char-block 0 capacity (32 KB = 1024 tiles) */
    uint32_t max_bytes  = SCREEN_BASE_OFFSET;          /* 0x8000 = 32 KB */
    uint32_t copy_bytes = (uint32_t)tile_size < max_bytes
                        ? (uint32_t)tile_size : max_bytes;
    copy_bytes -= copy_bytes % TILES_4BPP_BYTES;       /* whole tiles only */

    int num_tiles = (int)(copy_bytes / TILES_4BPP_BYTES);
    if (num_tiles < 1) return 0;

    memset(bank_a, 0, SCREEN_BASE_OFFSET + MAP_ENTRIES * 2);
    memcpy(bank_a, tile_data, copy_bytes);

    /* Search the asset pack for a real BGR555 palette.  If none is found
     * fall back to the rainbow palette so we still see coloured shapes. */
    int pal_loaded = write_real_palette();
    if (!pal_loaded) {
        write_palette();   /* rainbow fallback */
        nds_log("[boot_hook] No real palette found in pack — rainbow fallback\n");
    }

    write_tilemap(bank_a, num_tiles);
    configure_bg_regs();

    nds_log("[boot_hook] Real tiles loaded: FAT[0x62] %u tiles (%u bytes), "
            "palette=%s\n",
            (unsigned)num_tiles, (unsigned)copy_bytes,
            pal_loaded ? "asset-pack" : "rainbow-fallback");
    return 1;
}

int show_asset_in_vram(int fat_index)
{
    void  *data = NULL;
    size_t size = 0;

    if (!pack_get_file((uint32_t)fat_index, &data, &size) || !data || size == 0) {
        nds_log("[show_asset] FAT index %d not found in pack\n", fat_index);
        return 0;
    }

    uint8_t *bank_a = (uint8_t *)nds_vram_bank('A');
    if (!bank_a) {
        nds_log("[show_asset] VRAM bank A not available\n");
        return 0;
    }

    /* Copy asset bytes into the tile area (char block 0, up to the map offset) */
    uint32_t max_tile_bytes = SCREEN_BASE_OFFSET;  /* tiles must end before map at 0x8000 */
    uint32_t tile_bytes = (uint32_t)size < max_tile_bytes ? (uint32_t)size : max_tile_bytes;

    /* Round down to whole 4bpp tile boundary */
    tile_bytes -= tile_bytes % TILES_4BPP_BYTES;

    int num_tiles = (int)(tile_bytes / TILES_4BPP_BYTES);
    if (num_tiles < 1) num_tiles = 1;

    memset(bank_a, 0, (size_t)nds_vram_bank_size('A'));
    memcpy(bank_a + CHAR_BASE_OFFSET, data, tile_bytes);

    write_tilemap(bank_a, num_tiles);
    write_palette();
    configure_bg_regs();

    nds_log("[show_asset] FAT[%d]: %zu bytes → %d tiles loaded into VRAM\n",
            fat_index, size, num_tiles);
    return 1;
}

/* ── boot_hook_paired_screen ─────────────────────────────────────
 *
 * Try to render an actual identifiable game screen by loading a known
 * triple of (tilemap, tile sheet, palette) sub-blocks from FAT[0x45]
 * (asset 0x2045 — a 793-entry offset-table archive that contains, among
 * other things, complete BG screen sets).
 *
 * Heuristic mapping (verified by inspection):
 *   sub[181]  32 768 B  →  8bpp tile sheet (512 tiles × 64 B)
 *   sub[178]   4 096 B  →  64×32 tilemap   (sequential indices in BG order)
 *   sub[185]     512 B  →  256-colour BGR555 palette (8bpp)
 *
 * On success, configures BG0 in 8bpp mode (BG0CNT bit 7 = 1) with
 * screen_size = 1 (64×32 = 4 KB tilemap) and returns 1.
 *
 * Returns 0 if the asset pack is not loaded or any sub-block is the
 * wrong shape — caller should then fall back to boot_hook_real_tiles().
 * ──────────────────────────────────────────────────────────────── */

/* Resolve one sub-block of an offset-table archive.
 * Sets *out_data / *out_size on success, returns 1. */
static int archive_get_subblock(uint32_t fat_id, uint32_t sub_index,
                                const uint8_t **out_data, uint32_t *out_size)
{
    void *data = NULL; size_t sz = 0;
    if (!pack_get_file(fat_id, &data, &sz) || !data || sz < 8) return 0;
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t first_off = (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
                         ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
    if (first_off < 4 || first_off > sz || (first_off & 3)) return 0;
    uint32_t n_ent = first_off / 4;
    if (sub_index >= n_ent) return 0;
    uint32_t a = (uint32_t)bytes[sub_index*4]   | ((uint32_t)bytes[sub_index*4+1] << 8) |
                 ((uint32_t)bytes[sub_index*4+2] << 16) | ((uint32_t)bytes[sub_index*4+3] << 24);
    uint32_t b;
    if (sub_index + 1 < n_ent) {
        uint32_t k = (sub_index + 1) * 4;
        b = (uint32_t)bytes[k] | ((uint32_t)bytes[k+1] << 8) |
            ((uint32_t)bytes[k+2] << 16) | ((uint32_t)bytes[k+3] << 24);
    } else {
        b = (uint32_t)sz;
    }
    if (b <= a || b > sz) return 0;
    *out_data = bytes + a;
    *out_size = b - a;
    return 1;
}

/* Parameterised loader used by top, sub, and MLPIT_BOOT_TRIPLE override.
 * engine: 0 = top (bank A, palette E[0..]), 1 = sub (bank D, palette E[0x400..])
 * Returns 1 on success. */
static int paired_screen_load(int engine, uint32_t fat_id,
                              uint32_t sub_tiles, uint32_t sub_map,
                              uint32_t sub_pal)
{
    if (!pack_is_loaded()) return 0;

    const uint8_t *tiles = NULL, *map = NULL, *pal = NULL;
    uint32_t       tiles_sz = 0, map_sz = 0, pal_sz = 0;

    if (!archive_get_subblock(fat_id, sub_tiles, &tiles, &tiles_sz) || tiles_sz < 4096) return 0;
    if (!archive_get_subblock(fat_id, sub_map,   &map,   &map_sz)   || map_sz   != 4096)     return 0;
    if (!archive_get_subblock(fat_id, sub_pal,   &pal,   &pal_sz)   || pal_sz   < 32)        return 0;

    /* Sub engine: write to bank D (the conventional sub-engine BG VRAM
     * bank — see GBATEK VRAMCNT_D = 0x84 maps D at 0x06200000).
     * Old behaviour wrote to bank C, which on real hardware is a main-
     * engine BG bank, and caused the bottom screen to mirror the top in
     * the SDL composite (see milestone 02). */
    char vram_bank = (engine == 0) ? 'A' : 'D';
    uint32_t pal_off = (engine == 0) ? 0u : 0x400u;
    uint32_t dispcnt_reg = (engine == 0) ? 0x04000000u : 0x04001000u;
    uint32_t bg0cnt_reg  = (engine == 0) ? REG_BG0CNT  : 0x04001008u;
    uint32_t bg1cnt_reg  = (engine == 0) ? REG_BG1CNT  : 0x0400100Au;
    uint32_t bg2cnt_reg  = (engine == 0) ? REG_BG2CNT  : 0x0400100Cu;
    uint32_t bg3cnt_reg  = (engine == 0) ? REG_BG3CNT  : 0x0400100Eu;

    uint8_t *bank   = (uint8_t *)nds_vram_bank(vram_bank);
    uint8_t *bank_e = (uint8_t *)nds_vram_bank('E');
    if (!bank || !bank_e) return 0;

    memset(bank, 0, SCREEN_BASE_OFFSET + 4096);
    /* Cap tiles copy at the char-block size to avoid clobbering the screen base. */
    uint32_t tiles_copy = tiles_sz < SCREEN_BASE_OFFSET ? tiles_sz : SCREEN_BASE_OFFSET;
    memcpy(bank + CHAR_BASE_OFFSET, tiles, tiles_copy);
    memcpy(bank + SCREEN_BASE_OFFSET, map, 4096);

    uint32_t pal_copy = pal_sz < 512 ? pal_sz : 512;
    memcpy(bank_e + pal_off, pal, pal_copy);
    if (pal_copy < 512) memset(bank_e + pal_off + pal_copy, 0, 512 - pal_copy);

    /*
     * BG0CNT for 4bpp 64×32 text BG (this archive's tilemaps reference
     * pal_bank 0..14 in their entries, so the BG must be 4bpp; the 512-byte
     * palette blob is 16 × 16-colour sub-palettes, not a single 256-colour
     * one):
     *   bits[1:0]   = 0   priority 0
     *   bits[5:2]   = 0   char_base = 0  (32 B per 4bpp tile, 1024 max)
     *   bit 7       = 0   4bpp colour mode
     *   bits[12:8]  = 16  screen_base = 16 → map at VRAM+0x8000
     *   bits[15:14] = 1   screen_size 1 → 64×32 tilemap
     */
    nds_reg_write16(bg0cnt_reg, (uint16_t)(0x1000u | (1u << 14)));
    nds_reg_write16(bg1cnt_reg, 0);
    nds_reg_write16(bg2cnt_reg, 0);
    nds_reg_write16(bg3cnt_reg, 0);
    nds_reg_write32(dispcnt_reg, 0x0100u);

    nds_log("[boot_hook] Paired screen %s: FAT[0x%X] tiles=sub[%u] map=sub[%u] pal=sub[%u] (%uB, 4bpp)\n",
            engine == 0 ? "TOP" : "SUB", fat_id, sub_tiles, sub_map, sub_pal,
            (unsigned)pal_copy);
    return 1;
}

/* Parse "fatHex:tile:map:pal" into 4 unsigned values.
 * Returns 1 on success, 0 on parse failure. Uses strtoul to avoid the
 * MinGW sscanf locale-init pause that can hang the boot path. */
static int parse_triple(const char *s,
                        uint32_t *fat, uint32_t *t, uint32_t *m, uint32_t *p)
{
    char *end;
    *fat = (uint32_t)strtoul(s, &end, 16);
    if (*end != ':') return 0;
    *t = (uint32_t)strtoul(end + 1, &end, 10);
    if (*end != ':') return 0;
    *m = (uint32_t)strtoul(end + 1, &end, 10);
    if (*end != ':') return 0;
    *p = (uint32_t)strtoul(end + 1, &end, 10);
    return 1;
}

int boot_hook_paired_screen(void)
{
    /* Allow runtime override via MLPIT_BOOT_TRIPLE=fat_hex:tile_sub:map_sub:pal_sub
     * (decimal sub-indices). Example: "45:181:178:185". */
    const char *env = getenv("MLPIT_BOOT_TRIPLE");
    if (env && *env) {
        uint32_t fat_id, st, sm, sp;
        if (parse_triple(env, &fat_id, &st, &sm, &sp)) {
            if (paired_screen_load(0, fat_id, st, sm, sp)) {
                return 1;
            }
            nds_log("[boot_hook] MLPIT_BOOT_TRIPLE=%s failed; using default 0x45:181:194:177\n", env);
        } else {
            nds_log("[boot_hook] MLPIT_BOOT_TRIPLE=%s malformed (want fatHex:tile:map:pal)\n", env);
        }
    }
    /* Default chosen by tools/scripts/sweep_boot_triples.py — best
     * scene-likeness (low tile-grid regularity, balanced palette, ~104
     * unique colors) of the 57 candidate triples. */
    return paired_screen_load(0, 0x45, 181, 194, 177);
}

int boot_hook_paired_screen_sub(void)
{
    const char *env = getenv("MLPIT_BOOT_TRIPLE_SUB");
    if (env && *env) {
        uint32_t fat_id, st, sm, sp;
        if (parse_triple(env, &fat_id, &st, &sm, &sp)) {
            if (paired_screen_load(1, fat_id, st, sm, sp)) return 1;
        }
    }
    /* Default: mirror the top screen contents on sub engine. */
    return paired_screen_load(1, 0x45, 181, 194, 177);
}
