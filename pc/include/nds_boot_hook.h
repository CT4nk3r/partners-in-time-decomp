/*
 * nds_boot_hook.h - Pre-game VRAM population helpers
 *
 * Called after the asset pack loads to put visible content into VRAM
 * and configure BG registers so bg_render_top() produces pixels on the
 * first frame — even before (or regardless of) game_start() running.
 */
#ifndef NDS_BOOT_HOOK_H
#define NDS_BOOT_HOOK_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * boot_hook_vram() — fill VRAM with a synthetic 256-tile test pattern.
 *
 * Writes 256 procedural tiles + rainbow palette into VRAM banks A/E, then
 * configures DISPCNT and BG0CNT so the rasterizer renders them.
 * Safe to call before game_start(); it only touches PC-side buffers.
 */
void boot_hook_vram(void);

/*
 * boot_hook_real_tiles() — load the first 1024 tiles from the game's raw
 * tile sheet (FAT[0x62] = asset 0x2062) for a first glimpse of real game
 * graphics.  Falls back to boot_hook_vram() if the asset pack is not loaded
 * or the file is absent.
 *
 * Returns 1 on success (real tiles loaded), 0 on failure.
 */
int boot_hook_real_tiles(void);

/*
 * show_asset_in_vram(fat_index) — treat raw asset bytes as 4bpp tile data.
 *
 * Copies file fat_index raw bytes into VRAM bank A, builds a sequential
 * tilemap, loads a rainbow palette, and sets up DISPCNT/BG0CNT.
 * Used by the --show-asset N diagnostic mode.
 *
 * Returns 1 on success, 0 if the asset is not found.
 */
int show_asset_in_vram(int fat_index);

#ifdef __cplusplus
}
#endif

#endif /* NDS_BOOT_HOOK_H */
