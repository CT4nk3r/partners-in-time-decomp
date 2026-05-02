/*
 * nds_boot_hook.c - Pre-game VRAM population and diagnostic asset viewer
 *
 * Two modes:
 *  1. boot_hook_vram()           – synthetic test pattern (no assets needed)
 *  2. show_asset_in_vram(index)  – raw FAT file bytes treated as 4bpp tiles
 *
 * Both write tile data + palette into VRAM banks A/E and configure IO
 * shadow registers so bg_render_top() draws something even before
 * game_start() has run.
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

#include <string.h>
#include <stdint.h>
#include <stdio.h>

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
            "32x32 tilemap — BG0 enabled\n");
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
