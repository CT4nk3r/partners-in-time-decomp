/*
 * host_title_screen.c — Load and display the title screen from TitleBG.dat
 *
 * TitleBG.dat is FAT[99] in the ROM. It's an offset-table archive with 24
 * sub-entries containing AlphaDream-compressed tiles, tilemaps, and raw
 * BGR555 palettes.
 *
 * Archive layout (discovered via Python analysis):
 *   sub[0]  = AD-compressed 8bpp tiles (layer 0, sky gradient)     → 49216 B
 *   sub[1]  = AD-compressed tilemap (layer 0, 32×24)               → 1536 B
 *   sub[2]  = Raw 256-colour BGR555 palette (layer 0)              → 512 B
 *   sub[3]  = AD-compressed 8bpp tiles (layer 1, sky gradient 2)   → 49216 B
 *   sub[4]  = AD-compressed tilemap (layer 1, 32×24)               → 1536 B
 *   sub[5]  = Raw 256-colour BGR555 palette (layer 1)              → 512 B
 *   sub[6]  = AD-compressed 8bpp tiles (layer 2, time portal)      → 24384 B
 *   sub[7]  = AD-compressed tilemap (layer 2a, left half)          → 2048 B
 *   sub[8]  = AD-compressed tilemap (layer 2b, right half)         → 2048 B
 *   sub[9]  = Raw 256-colour BGR555 palette (layer 2)              → 512 B
 *   sub[10] = ATXI 32×32
 *   sub[11] = ATXI 32×16
 *   sub[12] = Mini-palette (8 colours)
 *   sub[13..22] = Additional compressed data (logo, stars, etc.)
 *
 * VRAM layout used by this loader:
 *   Bank A: Layer 0 tiles (char base 0) + tilemap (screen base 16)
 *   Bank B: Layer 2 tiles (char base 0) + tilemap (screen base 16)
 *   Bank E: Palettes (slot 0 = layer 0 palette, slot 1 = layer 2 palette)
 *
 * BG registers:
 *   BG0: layer 0 (sky), 8bpp, char=0 screen=16, priority 1
 *   BG1: layer 2 (portal), 8bpp, char=0 screen=16, priority 0
 */
#include "nds_platform.h"
#include "nds_rom.h"
#include "ad_decompress.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* NDS register addresses */
#define REG_DISPCNT   0x04000000u
#define REG_BG0CNT    0x04000008u
#define REG_BG1CNT    0x0400000Au
#define REG_BG2CNT    0x0400000Cu
#define REG_BG3CNT    0x0400000Eu

/* Title screen FAT index */
#define TITLE_BG_FAT_ID  99

/* Parse the offset-table archive header and get sub-entry bounds. */
static int parse_archive(const uint8_t *data, uint32_t data_len,
                         uint32_t *offsets, int *n_entries)
{
    if (data_len < 4) return 0;
    uint32_t first = (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
                     ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    if (first < 4 || first > data_len || (first % 4) != 0) return 0;

    int n = (int)(first / 4);
    if (n > 64) n = 64;  /* sanity cap */

    for (int i = 0; i < n; i++) {
        offsets[i] = (uint32_t)data[i*4] | ((uint32_t)data[i*4+1] << 8) |
                     ((uint32_t)data[i*4+2] << 16) | ((uint32_t)data[i*4+3] << 24);
    }
    offsets[n] = data_len;  /* sentinel */
    *n_entries = n;
    return 1;
}

/* Get a raw sub-entry from the archive. */
static const uint8_t *get_sub(const uint8_t *data, uint32_t data_len,
                               const uint32_t *offsets, int n, int idx,
                               uint32_t *sub_len)
{
    if (idx < 0 || idx >= n) return NULL;
    uint32_t start = offsets[idx];
    uint32_t end   = offsets[idx + 1];
    if (start >= end || end > data_len) return NULL;
    *sub_len = end - start;
    return data + start;
}

/*
 * host_title_screen_load — Load the title screen into VRAM.
 *
 * Reads TitleBG.dat (FAT[99]) from the ROM, decompresses tiles and tilemaps,
 * and writes them into VRAM banks for the BG renderer to display.
 *
 * Returns 1 on success, 0 on failure.
 */
int host_title_screen_load(void)
{
    const uint8_t *rom = rom_data();
    if (!rom || rom_size() < 0x200) {
        nds_log("[title] ROM not loaded\n");
        return 0;
    }

    /* Read FAT to find TitleBG.dat */
    uint32_t fat_off = (uint32_t)rom[0x48] | ((uint32_t)rom[0x49] << 8) |
                       ((uint32_t)rom[0x4A] << 16) | ((uint32_t)rom[0x4B] << 24);
    uint32_t f_start = (uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 0] |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 1] << 8) |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 2] << 16) |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 3] << 24);
    uint32_t f_end   = (uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 4] |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 5] << 8) |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 6] << 16) |
                       ((uint32_t)rom[fat_off + TITLE_BG_FAT_ID * 8 + 7] << 24);
    if (f_start >= f_end || f_end > (uint32_t)rom_size()) {
        nds_log("[title] FAT[%d] invalid: 0x%X..0x%X\n",
                TITLE_BG_FAT_ID, f_start, f_end);
        return 0;
    }

    const uint8_t *td = rom + f_start;
    uint32_t td_len   = f_end - f_start;
    nds_log("[title] TitleBG.dat: %u bytes at ROM 0x%X\n", td_len, f_start);

    /* Parse offset-table archive */
    uint32_t offsets[65];
    int n_entries = 0;
    if (!parse_archive(td, td_len, offsets, &n_entries) || n_entries < 10) {
        nds_log("[title] Archive parse failed (n=%d)\n", n_entries);
        return 0;
    }

    /* ── Layer 0: sky gradient (BG0) ── */
    uint32_t sub_len;
    const uint8_t *sub0 = get_sub(td, td_len, offsets, n_entries, 0, &sub_len);
    const uint8_t *sub1 = get_sub(td, td_len, offsets, n_entries, 1, &sub_len);
    const uint8_t *sub2 = get_sub(td, td_len, offsets, n_entries, 2, &sub_len);

    uint32_t tile0_len, map0_len;
    uint8_t *tiles0 = NULL, *map0 = NULL;

    if (sub0) {
        uint32_t s0_len = offsets[1] - offsets[0];
        tiles0 = ad_decompress(sub0, s0_len, &tile0_len);
    }
    if (sub1) {
        uint32_t s1_len = offsets[2] - offsets[1];
        map0 = ad_decompress(sub1, s1_len, &map0_len);
    }

    /* ── Layer 2: time portal (BG1) ── */
    const uint8_t *sub6 = get_sub(td, td_len, offsets, n_entries, 6, &sub_len);
    const uint8_t *sub7 = get_sub(td, td_len, offsets, n_entries, 7, &sub_len);
    const uint8_t *sub8 = get_sub(td, td_len, offsets, n_entries, 8, &sub_len);
    const uint8_t *sub9 = get_sub(td, td_len, offsets, n_entries, 9, &sub_len);

    uint32_t tile2_len, map2_left_len, map2_right_len;
    uint8_t *tiles2 = NULL, *map2_left = NULL, *map2_right = NULL;

    if (sub6) {
        uint32_t s6_len = offsets[7] - offsets[6];
        tiles2 = ad_decompress(sub6, s6_len, &tile2_len);
    }
    if (sub7) {
        uint32_t s7_len = offsets[8] - offsets[7];
        map2_left = ad_decompress(sub7, s7_len, &map2_left_len);
    }
    if (sub8) {
        uint32_t s8_len = offsets[9] - offsets[8];
        map2_right = ad_decompress(sub8, s8_len, &map2_right_len);
    }

    /* ── Write to VRAM ── */
    uint8_t *bank_a = (uint8_t *)nds_vram_bank('A');
    uint8_t *bank_b = (uint8_t *)nds_vram_bank('B');
    uint8_t *bank_e = (uint8_t *)nds_vram_bank('E');

    if (!bank_a || !bank_e) {
        nds_log("[title] VRAM banks not available\n");
        goto cleanup;
    }

    /* Clear VRAM */
    memset(bank_a, 0, 128 * 1024);  /* 128KB */
    if (bank_b) memset(bank_b, 0, 128 * 1024);
    memset(bank_e, 0, 64 * 1024);   /* palette RAM (64KB) */

    /* Layer 0 tiles → Bank A char base 0 (offset 0) */
    if (tiles0) {
        uint32_t max_tiles = 128 * 1024 - 0x8000;  /* leave room for tilemap */
        uint32_t copy = tile0_len < max_tiles ? tile0_len : max_tiles;
        memcpy(bank_a, tiles0, copy);
        nds_log("[title] Layer 0 tiles: %u bytes → Bank A\n", copy);
    }

    /* Layer 0 tilemap → Bank A screen base 16 (offset 0x8000) */
    if (map0) {
        uint32_t max_map = 2048;  /* 32×32 × 2 = 2048 or 32×24 × 2 = 1536 */
        uint32_t copy = map0_len < max_map ? map0_len : max_map;
        memcpy(bank_a + 0x8000, map0, copy);
        nds_log("[title] Layer 0 tilemap: %u bytes → Bank A+0x8000\n", copy);
    }

    /* Layer 0 palette → Bank E slot 0 (offset 0) */
    if (sub2) {
        uint32_t pal_sz = offsets[3] - offsets[2];
        if (pal_sz > 512) pal_sz = 512;
        memcpy(bank_e, sub2, pal_sz);
        nds_log("[title] Layer 0 palette: %u bytes → Bank E\n", pal_sz);
    }

    /* Layer 2 tiles → Bank B offset 0 (char_base 8 in 256KB space: 0x20000/0x4000) */
    if (tiles2 && bank_b) {
        uint32_t max_tiles = 128 * 1024 - 0x8000;
        uint32_t copy = tile2_len < max_tiles ? tile2_len : max_tiles;
        memcpy(bank_b, tiles2, copy);
        nds_log("[title] Layer 2 tiles: %u bytes → Bank B\n", copy);
    }

    /* Portal tilemaps → Bank A: two separate 32×32 tilemaps for BG1 + BG2.
     * The portal is split across two layers that overlap on the same screen:
     * - sub[7] has the left portion (cols 0-14)
     * - sub[8] has the right portion (cols 17-31) */
    if (map2_left) {
        uint32_t copy = map2_left_len < 2048 ? map2_left_len : 2048;
        memcpy(bank_a + 0xC000, map2_left, copy);   /* screen_base 24 */
        nds_log("[title] Layer 2 tilemap L: %u bytes → Bank A+0xC000\n", copy);
    }
    if (map2_right) {
        uint32_t copy = map2_right_len < 2048 ? map2_right_len : 2048;
        memcpy(bank_a + 0xC800, map2_right, copy);  /* screen_base 25 */
        nds_log("[title] Layer 2 tilemap R: %u bytes → Bank A+0xC800\n", copy);
    }

    /* Layer 2 palette → Bank E: ext palette slots for BG1 and BG2.
     * We store extended palettes at Bank E offset 0x1000+ to avoid conflicts
     * with the standard palette area (0x000-0x7FF) which gets overwritten
     * by boot_hook_paired_screen_sub(). Layout:
     *   0x1000: BG0 ext palette (sky)
     *   0x1200: BG1 ext palette (portal)
     *   0x1400: BG2 ext palette (portal) */
    if (sub2) {
        uint32_t pal_sz = offsets[3] - offsets[2];
        if (pal_sz > 512) pal_sz = 512;
        memcpy(bank_e + 0x1000, sub2, pal_sz);   /* BG0 ext = sky */
    }
    if (sub9) {
        uint32_t pal_sz = offsets[10] - offsets[9];
        if (pal_sz > 512) pal_sz = 512;
        memcpy(bank_e + 0x1200, sub9, pal_sz);   /* BG1 ext = portal */
        memcpy(bank_e + 0x1400, sub9, pal_sz);   /* BG2 ext = portal */
        nds_log("[title] Layer 2 palette: %u bytes → Bank E ext slots 1+2\n", pal_sz);
    }

    /* ── Configure BG registers ── */
    /* BG0: Layer 0 (sky), 8bpp, char_base=0, screen_base=16, priority=1
     * BG0CNT = prio(1) | char_base(0<<2) | 8bpp(1<<7) | screen_base(16<<8)
     *        = 0x1081 */
    nds_reg_write16(REG_BG0CNT, 0x1081);

    /* BG1: Portal left half, 8bpp, char_base=8, screen_base=24, priority=0
     * BG1CNT = 0 | (8<<2) | (1<<7) | (24<<8) = 0x18A0 */
    nds_reg_write16(REG_BG1CNT, 0x18A0);
    /* BG2: Portal right half, 8bpp, char_base=8, screen_base=25, priority=0
     * BG2CNT = 0 | (8<<2) | (1<<7) | (25<<8) = 0x19A0 */
    nds_reg_write16(REG_BG2CNT, 0x19A0);
    nds_reg_write16(REG_BG3CNT, 0x0000);

    /* DISPCNT: mode 0, BG0+BG1+BG2 enabled (bits 8-10)
     * 0x0700 = BG0 + BG1 + BG2 visible */
    nds_reg_write32(REG_DISPCNT, 0x0700);

    nds_log("[title] Title screen loaded! BG0=sky BG1+BG2=portal\n");

cleanup:
    free(tiles0);
    free(map0);
    free(tiles2);
    free(map2_left);
    free(map2_right);
    return (tiles0 && map0) ? 1 : 0;
}
