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

static uint8_t g_palette_main[512];
static uint8_t g_palette_sub[512];
static uint8_t g_vram_main[256 * 1024];
static uint8_t g_vram_sub[128 * 1024];

void bg_render_sync_vram(void) {
    void *bank_a = nds_vram_bank('A');
    void *bank_b = nds_vram_bank('B');
    void *bank_c = nds_vram_bank('C');

    if (bank_a) memcpy(g_vram_main,              bank_a, 128 * 1024);
    if (bank_b) memcpy(g_vram_main + 128 * 1024, bank_b, 128 * 1024);
    if (bank_c) memcpy(g_vram_sub,               bank_c, 128 * 1024);

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
    }
}

static void render_bg_layer(uint16_t *fb, const uint8_t *vram,
                             const uint8_t *palette, uint16_t bgcnt,
                             int hofs, int vofs)
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
            if (!color_mode) {
                int byte_off = tile_num * TILE_4BPP_SIZE + py * 4 + px / 2;
                uint8_t b = tile_base[byte_off];
                color_idx = (px & 1) ? (b >> 4) : (b & 0xF);
                if (color_idx == 0) continue;
                color_idx += pal_bank * 16;
            } else {
                int byte_off = tile_num * 64 + py * 8 + px;
                color_idx = tile_base[byte_off];
                if (color_idx == 0) continue;
            }

            uint16_t pal_entry = (uint16_t)(palette[color_idx * 2] | (palette[color_idx * 2 + 1] << 8));
            fb[y * NDS_SCREEN_WIDTH + x] = pal_entry;
        }
    }
}

void bg_render_top(uint16_t *fb) {
    uint32_t dispcnt = nds_reg_read32(REG_DISPCNT);
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

    for (int pri = 3; pri >= 0; pri--) {
        for (int bg = 3; bg >= 0; bg--) {
            if (!bg_enable[bg]) continue;
            if ((bgcnt[bg] & 3) != (uint16_t)pri) continue;
            render_bg_layer(fb, g_vram_main, g_palette_main,
                            bgcnt[bg], hofs[bg], vofs[bg]);
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
                            bgcnt[bg], hofs[bg], vofs[bg]);
        }
    }
}
