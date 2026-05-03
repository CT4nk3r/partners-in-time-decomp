/*
 * host_gameplay_scene.c — HOST_PORT implementation of overlay 0's gameplay
 * scene constructor (FUN_0206DE6C), tick (FUN_0206C34C), and destructor
 * (FUN_0206DE48).
 *
 * Loads real game map backgrounds from FMapData.dat and renders them
 * using the NDS BG tile engine. Player can scroll the map with arrow keys.
 *
 * Original vtable at 0x020BF164:
 *   [0] = 0x0206DE48 (dtor)
 *   [1] = 0x0206DE1C (setup)
 *   [2] = 0x0206C34C (tick)
 */

#include "types.h"
#include "nds_rom.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- External interfaces ---- */
extern void  host_fnptr_register(uint32_t nds_addr, void *host_fn);
extern int   nds_arm9_ram_is_mapped(void);
extern void *nds_vram_bank(char bank);
extern uint32_t nds_vram_bank_size(char bank);
extern uint8_t *ad_decompress(const uint8_t *src, uint32_t src_len, uint32_t *out_size);

/* Scene queue insert */
extern void FUN_0202a74c_real(u32 node_addr, u8 priority, u32 r2_unused, u32 r3_param);

/* NDS memory helpers */
extern u32   nds_bump_alloc(u32 size);

/* Scene transition */
extern void FUN_02005d3c(int scene_anchor, int next_state);

/* ---- Overlay 0 NDS addresses ---- */
#define OV0_VTABLE_ADDR     0x020BF164u
#define OV0_TICK_ADDR       0x0206C34Cu
#define OV0_DTOR_ADDR       0x0206DE48u
#define OV0_SETUP_ADDR      0x0206DE1Cu

/* Register addresses */
#define REG_DISPCNT_TOP     0x04000000u
#define REG_BG0CNT          0x04000008u
#define REG_BG1CNT          0x0400000Au
#define REG_BG2CNT          0x0400000Cu
#define REG_BG0HOFS         0x04000010u
#define REG_BG0VOFS         0x04000012u
#define REG_BG1HOFS         0x04000014u
#define REG_BG1VOFS         0x04000016u
#define REG_BG2HOFS         0x04000018u
#define REG_BG2VOFS         0x0400001Au
#define REG_MASTER_BRIGHT   0x0400006Cu
#define REG_DISPCNT_SUB     0x04001000u
#define REG_SUB_BRIGHT      0x0400106Cu

/* FMapData.dat ROM offset (from NDS filesystem analysis) */
#define FMAP_ROM_OFFSET     0xD42600u
#define MAX_MAP_GROUPS      200

/* FObjPc.dat ROM offset — character sprite archive */
#define FOBJPC_ROM_OFFSET   0x2AA9800u
#define FOBJPC_SIZE         900608u

/* Auto-detected map groups from FMapData.dat offset table.
 * Each group = 3 tile entries + 1 descriptor entry (entries [base..base+3]).
 * Scanned at startup from the offset table header. */
static int s_map_groups[MAX_MAP_GROUPS][2];  /* [i][0]=base, [i][1]=desc */
static int s_num_map_groups = 0;

static void scan_fmap_groups(void)
{
    if (s_num_map_groups > 0) return;

    const uint8_t *rom = rom_data();
    size_t rom_sz = rom_size();
    if (!rom || rom_sz < FMAP_ROM_OFFSET + 16) return;

    const uint8_t *fmap = rom + FMAP_ROM_OFFSET;
    uint32_t first_off = fmap[0] | (fmap[1]<<8) | (fmap[2]<<16) | (fmap[3]<<24);
    int num_entries = (int)(first_off / 4);
    if (num_entries > 3000) num_entries = 3000;

    int i = 0;
    while (i + 3 < num_entries && s_num_map_groups < MAX_MAP_GROUPS) {
        /* Check if entries i..i+3 form a valid group:
         * all 4 entries must have non-zero size (> 500 bytes compressed) */
        int valid = 1;
        for (int j = 0; j < 4 && valid; j++) {
            uint32_t off  = fmap[(i+j)*4] | (fmap[(i+j)*4+1]<<8) |
                            (fmap[(i+j)*4+2]<<16) | (fmap[(i+j)*4+3]<<24);
            uint32_t noff = fmap[(i+j+1)*4] | (fmap[(i+j+1)*4+1]<<8) |
                            (fmap[(i+j+1)*4+2]<<16) | (fmap[(i+j+1)*4+3]<<24);
            if (noff <= off || (noff - off) < 500) valid = 0;
        }
        if (valid) {
            s_map_groups[s_num_map_groups][0] = i;      /* base entry */
            s_map_groups[s_num_map_groups][1] = i + 3;  /* descriptor entry */
            s_num_map_groups++;
            i += 4;
        } else {
            i++;
        }
    }

    fprintf(stderr, "[gameplay] scanned %d map groups from %d FMapData entries\n",
            s_num_map_groups, num_entries);
}

/* Map descriptor internal structure:
 * u32[0] = offset to BG0 tilemap (typically 0x38)
 * u32[1] = offset to BG1 tilemap
 * u32[2] = offset to BG2 tilemap
 * u32[3] = offset to palette 0 (512 bytes)
 * u32[4] = offset to palette 1 (512 bytes)
 * u32[5] = offset to palette 2 (512 bytes)
 * u32[6] = end/extra offset
 */

/* Gameplay state */
static u32  s_gameplay_obj_nds = 0;
static u32  s_gameplay_frame   = 0;
static int  s_gameplay_loaded  = 0;
static int  s_gameplay_state   = 0;  /* 0=fadein, 1=active */
static int  s_scroll_x         = 0;
static int  s_scroll_y         = 0;
static int  s_map_loaded       = 0;
static int  s_current_map      = 0;
static int  s_sprites_loaded   = 0;

/* Player state */
static int  s_player_x     = 128;  /* world position (pixels) */
static int  s_player_y     = 128;
static int  s_player_vy    = 0;    /* vertical velocity (subpixels, /16) */
static int  s_player_on_ground = 1;
static int  s_player_facing = 0;   /* 0=right, 1=left */
static int  s_player_moving = 0;   /* 1 = moving horizontally this frame */

/* Luigi state (follows Mario) */
static int  s_luigi_x      = 96;
static int  s_luigi_y      = 128;
static int  s_luigi_facing = 0;

/* Animation state */
static int  s_anim_frame   = 0;    /* current animation frame (0-3) */
static int  s_anim_timer   = 0;    /* frame counter for animation */
#define ANIM_SPEED 8  /* frames per animation step */
#define MARIO_TILES_PER_FRAME 16  /* tiles per 32x32 animation frame (4x4) */
#define LUIGI_OBJ_VRAM_OFFSET (32 * 1024)  /* Luigi tiles start after Mario in OBJ VRAM */

#define GAMEPLAY_FADE_DURATION 30
#define PLAYER_SPEED 2
#define PLAYER_GRAVITY 1
#define PLAYER_JUMP_VEL -48  /* subpixels/frame */
#define PLAYER_MAX_FALL 64
#define MAP_PX_W 512  /* 64 tiles * 8px */
#define MAP_PX_H 512

/* Load a full FMapData map group into VRAM (all 3 BG layers).
 * grp_base: first entry index of the group (e.g., 0 for entries 0,1,2)
 * desc_entry: descriptor entry index (e.g., 3)
 * Returns number of layers loaded (0-3).
 */
static int load_fmap_all_layers(int grp_base, int desc_entry)
{
    const uint8_t *rom = rom_data();
    size_t rom_sz = rom_size();
    if (!rom || rom_sz < FMAP_ROM_OFFSET + 16) return 0;

    const uint8_t *fmap = rom + FMAP_ROM_OFFSET;

    /* Read offset table header */
    uint32_t first_off = fmap[0] | (fmap[1]<<8) | (fmap[2]<<16) | (fmap[3]<<24);
    int num_entries = (int)(first_off / 4);
    if (desc_entry >= num_entries || grp_base + 2 >= num_entries) return 0;

    #define FMAP_OFF(idx) (fmap[(idx)*4] | (fmap[(idx)*4+1]<<8) | \
                          (fmap[(idx)*4+2]<<16) | (fmap[(idx)*4+3]<<24))

    /* Decompress descriptor */
    uint32_t d_off = FMAP_OFF(desc_entry);
    uint32_t d_next = FMAP_OFF(desc_entry + 1);
    uint32_t desc_size = 0;
    uint8_t *desc = ad_decompress(fmap + d_off, d_next - d_off, &desc_size);
    if (!desc || desc_size < 28) { free(desc); return 0; }

    uint32_t sec_off[7];
    for (int i = 0; i < 7; i++)
        sec_off[i] = desc[i*4] | (desc[i*4+1]<<8) | (desc[i*4+2]<<16) | (desc[i*4+3]<<24);

    void *bank_a = nds_vram_bank('A');
    void *bank_b = nds_vram_bank('B');
    void *bank_e = nds_vram_bank('E');
    void *bank_f = nds_vram_bank('F');
    if (!bank_a || !bank_e) { free(desc); return 0; }

    /* Clear VRAM */
    memset(bank_a, 0, nds_vram_bank_size('A'));
    if (bank_b) memset(bank_b, 0, nds_vram_bank_size('B'));
    if (bank_f) memset(bank_f, 0, nds_vram_bank_size('F'));

    /* Write per-BG palettes into VRAM banks F/G (NDS extended palette).
     * NDS ext palette: each BG gets 8KB (16 sub-palettes × 256 entries × 2B).
     * Bank F = 16KB: BG0 at offset 0, BG1 at offset 0x2000.
     * Bank G = 16KB: BG2 at offset 0, BG3 at offset 0x2000.
     *
     * The 512-byte palette from FMapData is a 256-color palette.
     * Replicate it to ALL 16 sub-palette slots so any pal_bank works. */
    if (sec_off[3] + 512 <= desc_size) {
        /* BG0 palette (section 3) → Bank F slot 0 */
        if (bank_f) {
            for (int sp = 0; sp < 16; sp++)
                memcpy((uint8_t*)bank_f + 0*8192 + sp*512, desc + sec_off[3], 512);
        }
        memcpy((uint8_t*)bank_e + 0x1000 + 0*512, desc + sec_off[3], 512);
    }
    if (sec_off[4] + 512 <= desc_size) {
        /* BG1 palette (section 4) → Bank F slot 1 */
        if (bank_f) {
            for (int sp = 0; sp < 16; sp++)
                memcpy((uint8_t*)bank_f + 1*8192 + sp*512, desc + sec_off[4], 512);
        }
        memcpy((uint8_t*)bank_e + 0x1000 + 1*512, desc + sec_off[4], 512);
    }
    if (sec_off[5] + 512 <= desc_size) {
        /* BG2 palette (section 5) → Bank G slot 0 */
        void *bank_g = nds_vram_bank('G');
        if (bank_g) {
            for (int sp = 0; sp < 16; sp++)
                memcpy((uint8_t*)bank_g + 0*8192 + sp*512, desc + sec_off[5], 512);
        }
        memcpy((uint8_t*)bank_e + 0x1000 + 2*512, desc + sec_off[5], 512);
    }
    /* Also write section 5 to standard palette for backdrop color */
    if (sec_off[5] + 512 <= desc_size)
        memcpy(bank_e, desc + sec_off[5], 512);

    /* VRAM layout (within 256KB main BG VRAM = Bank A + Bank B):
     * BG2 (background/sky): char_base=0 (0x00000), screen_base=16 (0x08000)
     * BG1 (middle layer):   char_base=2 (0x08000)  — SHARES space after BG2 tilemap
     *                        screen_base=20 (0x0A000)
     * BG0 (foreground):     char_base=4 (0x10000), screen_base=24 (0x0C000)
     *
     * char_base is in 16KB units, screen_base in 2KB units.
     * Tile data for each layer is at most ~30KB.
     * Tilemaps are 8KB each (64x64).
     */
    int layers_ok = 0;
    uint8_t *vram = (uint8_t *)bank_a;  /* 128KB */
    uint8_t *vram_b = bank_b ? (uint8_t *)bank_b : NULL;

    /* Load BG2 (lowest priority = drawn first = background) */
    {
        int tile_entry = grp_base + 2;
        if (tile_entry < num_entries) {
            uint32_t t_off = FMAP_OFF(tile_entry);
            uint32_t t_next = FMAP_OFF(tile_entry + 1);
            uint32_t tile_sz = 0;
            uint8_t *tiles = ad_decompress(fmap + t_off, t_next - t_off, &tile_sz);
            if (tiles && tile_sz > 0) {
                /* Tiles at char_base=0 → offset 0x0000 */
                uint32_t copy = tile_sz > 0x8000 ? 0x8000 : tile_sz;
                memcpy(vram + 0x0000, tiles, copy);
                free(tiles);

                /* Tilemap at screen_base=16 → offset 0x8000 */
                uint32_t tmap_off = sec_off[2];
                uint32_t tmap_end = sec_off[3];
                if (tmap_off < desc_size && tmap_end <= desc_size) {
                    uint32_t tmap_sz = tmap_end - tmap_off;
                    if (tmap_sz > 8192) tmap_sz = 8192;
                    memcpy(vram + 0x8000, desc + tmap_off, tmap_sz);
                    layers_ok |= (1 << 2);
                }
            } else { free(tiles); }
        }
    }

    /* Load BG0 (highest priority = drawn last = foreground) */
    {
        int tile_entry = grp_base + 0;
        if (tile_entry < num_entries) {
            uint32_t t_off = FMAP_OFF(tile_entry);
            uint32_t t_next = FMAP_OFF(tile_entry + 1);
            uint32_t tile_sz = 0;
            uint8_t *tiles = ad_decompress(fmap + t_off, t_next - t_off, &tile_sz);
            if (tiles && tile_sz > 0) {
                /* Tiles at char_base=4 → offset 0x10000 (in bank B if bank A is 128KB) */
                uint32_t copy = tile_sz > 0x8000 ? 0x8000 : tile_sz;
                if (0x10000 + copy <= 128*1024) {
                    memcpy(vram + 0x10000, tiles, copy);
                } else if (vram_b) {
                    /* Overflows into Bank B */
                    memcpy(vram_b, tiles, copy);
                }
                free(tiles);

                /* Tilemap at screen_base=24 → offset 0xC000 */
                uint32_t tmap_off = sec_off[0];
                uint32_t tmap_end = sec_off[1];
                if (tmap_off < desc_size && tmap_end <= desc_size) {
                    uint32_t tmap_sz = tmap_end - tmap_off;
                    if (tmap_sz > 8192) tmap_sz = 8192;
                    memcpy(vram + 0xC000, desc + tmap_off, tmap_sz);
                    layers_ok |= (1 << 0);
                }
            } else { free(tiles); }
        }
    }

    /* Load BG1 (middle layer — between foreground and background) */
    {
        int tile_entry = grp_base + 1;
        if (tile_entry < num_entries) {
            uint32_t t_off = FMAP_OFF(tile_entry);
            uint32_t t_next = FMAP_OFF(tile_entry + 1);
            uint32_t tile_sz = 0;
            uint8_t *tiles = ad_decompress(fmap + t_off, t_next - t_off, &tile_sz);
            if (tiles && tile_sz > 0) {
                /* Tiles at char_base=2 → offset 0x8000 (after BG2 tilemap)
                 * Need to offset so it doesn't clash with BG2 tilemap at 0x8000.
                 * Use char_base=6 → offset 0x18000 in bank B. */
                uint32_t copy = tile_sz > 0x8000 ? 0x8000 : tile_sz;
                if (vram_b) {
                    memcpy(vram_b + 0x8000, tiles, copy);
                }
                free(tiles);

                /* Tilemap at screen_base=28 → offset 0xE000 */
                uint32_t tmap_off = sec_off[1];
                uint32_t tmap_end = sec_off[2];
                if (tmap_off < desc_size && tmap_end <= desc_size) {
                    uint32_t tmap_sz = tmap_end - tmap_off;
                    if (tmap_sz > 8192) tmap_sz = 8192;
                    memcpy(vram + 0xE000, desc + tmap_off, tmap_sz);
                    layers_ok |= (1 << 1);
                }
            } else { free(tiles); }
        }
    }

    fprintf(stderr, "[gameplay] multi-layer load: layers_ok=0x%X "
            "(BG0=%s BG1=%s BG2=%s)\n",
            layers_ok,
            (layers_ok & 1) ? "yes" : "no",
            (layers_ok & 2) ? "yes" : "no",
            (layers_ok & 4) ? "yes" : "no");

    free(desc);
    return layers_ok ? 1 : 0;
    #undef FMAP_OFF
}

/* ================================================================
 * Character sprite loading from FObjPc.dat
 * ================================================================
 *
 * FObjPc.dat structure:
 *   Entry 0:  header (152 sprites, 8-byte records)
 *   Entries 1-304: 152 pairs (odd=metadata, even=tile data), AD-compressed
 *   Entries 305-307: 3 palettes (512 bytes each, raw RGB555)
 *   Entries 308+: cell arrangement data
 *
 * Tile data: 4bpp (32 bytes per 8x8 tile), AD-compressed.
 * Palette bank 1 = Mario (blue overalls, red hat)
 */

static int load_character_sprites(int sprite_idx)
{
    const uint8_t *rom = rom_data();
    size_t rom_sz = rom_size();
    if (!rom || rom_sz < FOBJPC_ROM_OFFSET + FOBJPC_SIZE) return 0;

    const uint8_t *fobj = rom + FOBJPC_ROM_OFFSET;

    /* Read offset table */
    uint32_t first_off = fobj[0] | (fobj[1]<<8) | (fobj[2]<<16) | (fobj[3]<<24);
    int num_entries = (int)(first_off / 4);
    if (num_entries < 310) return 0;

    #define FOBJ_OFF(idx) (fobj[(idx)*4] | (fobj[(idx)*4+1]<<8) | \
                          (fobj[(idx)*4+2]<<16) | (fobj[(idx)*4+3]<<24))

    /* Tile data entry = sprite_idx * 2 + 2 */
    int tile_entry = sprite_idx * 2 + 2;
    if (tile_entry >= num_entries - 1) return 0;

    uint32_t t_off = FOBJ_OFF(tile_entry);
    uint32_t t_next = FOBJ_OFF(tile_entry + 1);
    if (t_off >= FOBJPC_SIZE || t_next > FOBJPC_SIZE || t_next <= t_off) return 0;

    uint32_t tile_sz = 0;
    uint8_t *tiles = ad_decompress(fobj + t_off, t_next - t_off, &tile_sz);
    if (!tiles || tile_sz == 0) { free(tiles); return 0; }

    /* Write to OBJ VRAM */
    extern uint8_t *obj_vram_main_ptr(void);
    extern uint32_t obj_vram_main_size(void);
    uint8_t *obj_vram = obj_vram_main_ptr();
    uint32_t max_sz = obj_vram_main_size();
    uint32_t copy_sz = tile_sz > max_sz ? max_sz : tile_sz;
    memcpy(obj_vram, tiles, copy_sz);
    free(tiles);

    /* Load palette from entry 305 */
    int pal_entry = 305;
    uint32_t p_off = FOBJ_OFF(pal_entry);
    uint32_t p_next = FOBJ_OFF(pal_entry + 1);
    if (p_off < FOBJPC_SIZE && p_next <= FOBJPC_SIZE && (p_next - p_off) >= 512) {
        void *bank_e = nds_vram_bank('E');
        if (bank_e) {
            memcpy((uint8_t*)bank_e + 0x200, fobj + p_off, 512);
        }
        extern uint8_t *obj_palette_main_ptr(void);
        memcpy(obj_palette_main_ptr(), fobj + p_off, 512);
    }

    fprintf(stderr, "[gameplay] loaded sprite %d: %u bytes (%u tiles)\n",
            sprite_idx, (unsigned)copy_sz, (unsigned)(copy_sz / 32));

    #undef FOBJ_OFF
    return 1;
}

/* Set up an OAM entry for a sprite — writes to NDS shadow OAM (0x0205FFC0)
 * so the upload tick picks it up naturally. */
static void setup_player_oam(int oam_slot, int x, int y,
                              int tile_start, int pal_bank, int w, int h,
                              int flip_h)
{
    /* Write to NDS shadow OAM at 0x0205FFC0 */
    uint8_t *entry = (uint8_t *)(uintptr_t)(0x0205FFC0u + oam_slot * 8);

    int shape = 0, size_bits = 0;
    if (w == h) {
        shape = 0;
        if (w <= 8) size_bits = 0;
        else if (w <= 16) size_bits = 1;
        else if (w <= 32) size_bits = 2;
        else size_bits = 3;
    } else if (w > h) {
        shape = 1;
        if (w == 32 && h == 16) size_bits = 2;
        else if (w == 32 && h == 8) size_bits = 1;
        else size_bits = 3;
    } else {
        shape = 2;
        if (w == 16 && h == 32) size_bits = 2;
        else if (w == 8 && h == 32) size_bits = 1;
        else size_bits = 3;
    }

    uint16_t attr0 = (uint16_t)((y & 0xFF) | (shape << 14));
    uint16_t attr1 = (uint16_t)((x & 0x1FF) | (size_bits << 14));
    if (flip_h) attr1 |= (1 << 12);  /* attr1 bit 12 = horizontal flip */
    uint16_t attr2 = (uint16_t)((tile_start & 0x3FF) | ((pal_bank & 0xF) << 12));

    entry[0] = (uint8_t)(attr0 & 0xFF);
    entry[1] = (uint8_t)(attr0 >> 8);
    entry[2] = (uint8_t)(attr1 & 0xFF);
    entry[3] = (uint8_t)(attr1 >> 8);
    entry[4] = (uint8_t)(attr2 & 0xFF);
    entry[5] = (uint8_t)(attr2 >> 8);
    entry[6] = 0;
    entry[7] = 0;

    /* Also write directly to g_oam_main so obj_render sees it immediately,
     * bypassing the shadow-OAM upload path which may have timing issues. */
    {
        extern uint8_t g_oam_main[1024];
        memcpy(g_oam_main + oam_slot * 8, entry, 8);
    }
}

/* ---- Destructor ---- */
static void host_gameplay_dtor(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    fprintf(stderr, "[gameplay] dtor: node=0x%08X\n", (unsigned)node_addr);
    s_gameplay_obj_nds = 0;
    s_gameplay_frame = 0;
    s_gameplay_loaded = 0;
    s_gameplay_state = 0;
    s_map_loaded = 0;
    s_anim_frame = 0;
    s_anim_timer = 0;
    s_player_moving = 0;
}

/* Global flag: set to 1 once gameplay takes over display.
 * Checked by keep_boot_screen_visible() in platform_sdl.c to stop
 * overriding DISPCNT with the title-screen configuration. */
int g_game_display_active = 0;

/* ---- Per-frame tick ---- */
static void host_gameplay_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    s_gameplay_frame++;

    /* One-time setup: load map from ROM */
    if (!s_gameplay_loaded) {
        s_gameplay_loaded = 1;
        g_game_display_active = 1;

        /* Clear all VRAM banks */
        for (char bank = 'A'; bank <= 'I'; bank++) {
            void *vram = nds_vram_bank(bank);
            u32 sz = nds_vram_bank_size(bank);
            if (vram && sz > 0) memset(vram, 0, sz);
        }

        /* Try to load map from FMapData.dat — auto-scan map groups */
        scan_fmap_groups();
        {
            const char *map_env = getenv("MLPIT_MAP_INDEX");
            if (map_env && s_num_map_groups > 0)
                s_current_map = atoi(map_env) % s_num_map_groups;
        }
        if (s_num_map_groups > 0) {
            s_map_loaded = load_fmap_all_layers(
                s_map_groups[s_current_map][0],
                s_map_groups[s_current_map][1]);
        }

        if (s_map_loaded) {
            /* Set up top screen: Mode 0, BG0+BG1+BG2+OBJ, extended palettes (bit 30)
             * Bit 4: OBJ 1D mapping, Bit 8: BG0, Bit 9: BG1, Bit 10: BG2, Bit 12: OBJ */
            *(volatile u32 *)(uintptr_t)REG_DISPCNT_TOP =
                0x40011710u;  /* ext_pal(30) + OBJ_1D(4) + mode 0 + BG0(8) + BG1(9) + BG2(10) + OBJ(12) */

            /* BG2 (background): char_base=0, screen_base=16, 8bpp, 64x64, priority 2 */
            *(volatile u16 *)(uintptr_t)REG_BG2CNT =
                (0 << 2) | (16 << 8) | (1 << 7) | (3 << 14) | 2;
            *(volatile u16 *)(uintptr_t)REG_BG2HOFS = 0;
            *(volatile u16 *)(uintptr_t)REG_BG2VOFS = 0;

            /* BG1 (middle layer): char_base=6 (0x18000 in bank B), screen_base=28 (0xE000), 8bpp, 64x64, priority 1 */
            *(volatile u16 *)(uintptr_t)REG_BG1CNT =
                (6 << 2) | (28 << 8) | (1 << 7) | (3 << 14) | 1;
            *(volatile u16 *)(uintptr_t)REG_BG1HOFS = 0;
            *(volatile u16 *)(uintptr_t)REG_BG1VOFS = 0;

            /* BG0 (foreground): char_base=4, screen_base=24, 8bpp, 64x64, priority 0 */
            *(volatile u16 *)(uintptr_t)REG_BG0CNT =
                (4 << 2) | (24 << 8) | (1 << 7) | (3 << 14) | 0;
            *(volatile u16 *)(uintptr_t)REG_BG0HOFS = (u16)(s_scroll_x & 0x1FF);
            *(volatile u16 *)(uintptr_t)REG_BG0VOFS = (u16)(s_scroll_y & 0x1FF);

            fprintf(stderr,
                    "[gameplay] real map loaded! Use arrow keys to scroll\n");
        } else {
            /* Fallback: just show text + OBJ */
            *(volatile u32 *)(uintptr_t)REG_DISPCNT_TOP = 0x00011110u;
            fprintf(stderr, "[gameplay] map load failed, using text fallback\n");
        }

        /* Load character sprites — Mario (index 0) and Luigi (index 1) */
        s_sprites_loaded = load_character_sprites(0);
        if (s_sprites_loaded) {
            /* Also load Luigi's tile data into the same OBJ VRAM (offset past Mario) */
            {
                const uint8_t *rom = rom_data();
                const uint8_t *fobj = rom + FOBJPC_ROM_OFFSET;
                int num_entries = (int)((fobj[0] | (fobj[1]<<8) | (fobj[2]<<16) | (fobj[3]<<24)) / 4);
                if (num_entries >= 310) {
                    /* Luigi = sprite index 1 → tile entry 4 */
                    uint32_t t_off = fobj[4*4] | (fobj[4*4+1]<<8) | (fobj[4*4+2]<<16) | (fobj[4*4+3]<<24);
                    uint32_t t_next = fobj[5*4] | (fobj[5*4+1]<<8) | (fobj[5*4+2]<<16) | (fobj[5*4+3]<<24);
                    uint32_t tile_sz = 0;
                    uint8_t *tiles = ad_decompress(fobj + t_off, t_next - t_off, &tile_sz);
                    if (tiles && tile_sz > 0) {
                        extern uint8_t *obj_vram_main_ptr(void);
                        extern uint32_t obj_vram_main_size(void);
                        uint32_t max = obj_vram_main_size();
                        uint32_t copy = tile_sz;
                        if (LUIGI_OBJ_VRAM_OFFSET + copy > max)
                            copy = max - LUIGI_OBJ_VRAM_OFFSET;
                        memcpy(obj_vram_main_ptr() + LUIGI_OBJ_VRAM_OFFSET, tiles, copy);
                        fprintf(stderr, "[gameplay] loaded Luigi sprite: %u bytes\n", (unsigned)copy);
                    }
                    free(tiles);

                    /* Load Luigi palette (entry 306 = palette 1, green overalls) */
                    uint32_t p_off = fobj[306*4] | (fobj[306*4+1]<<8) | (fobj[306*4+2]<<16) | (fobj[306*4+3]<<24);
                    uint32_t p_next = fobj[307*4] | (fobj[307*4+1]<<8) | (fobj[307*4+2]<<16) | (fobj[307*4+3]<<24);
                    if ((p_next - p_off) >= 512) {
                        extern uint8_t *obj_palette_main_ptr(void);
                        /* Luigi palette in OBJ palette bank 2 (offset 32*2=64 bytes) */
                        memcpy(obj_palette_main_ptr() + 64, fobj + p_off, 32);
                        void *bank_e = nds_vram_bank('E');
                        if (bank_e)
                            memcpy((uint8_t*)bank_e + 0x200 + 64, fobj + p_off, 32);
                    }
                }
            }

            /* Initial OAM placement for Mario and Luigi */
            setup_player_oam(0, 112, 80, 0, 1, 32, 32, 0);
            /* Luigi tile starts at LUIGI_OBJ_VRAM_OFFSET/32 = 1024 tiles in */
            int luigi_tile = LUIGI_OBJ_VRAM_OFFSET / 32;
            setup_player_oam(1, 80, 80, luigi_tile, 2, 32, 32, 0);
            fprintf(stderr, "[gameplay] Mario + Luigi sprites placed in OAM\n");
        }

        /* Sub screen: mirror the map for bottom display.
         * Copy tile/map data from bank A→C for sub BG VRAM.
         * Copy palette to bank E+0x400 (sub palette source).
         * Configure sub DISPCNT and BGCNT to match main screen. */
        {
            void *bank_a = nds_vram_bank('A');
            void *bank_c = nds_vram_bank('C');
            if (bank_a && bank_c) {
                memcpy(bank_c, bank_a, 128 * 1024);
            }
            /* Copy main palette to sub palette location (bank E + 0x400) */
            void *bank_e = nds_vram_bank('E');
            if (bank_e) {
                memcpy((uint8_t *)bank_e + 0x400, bank_e, 512);
            }

            /* Sub DISPCNT: mode 0, BG2 enabled, OBJ 1D */
            *(volatile u32 *)(uintptr_t)0x04001000u = 0x00001410u;
            /* Sub BG2: char_base=0, screen_base=16, 8bpp, 64x64, priority 2 */
            *(volatile u16 *)(uintptr_t)0x0400100Cu =
                (0 << 2) | (16 << 8) | (1 << 7) | (3 << 14) | 2;
            *(volatile u16 *)(uintptr_t)0x04001018u = 0;
            *(volatile u16 *)(uintptr_t)0x0400101Au = 0;
        }

        fprintf(stderr, "[gameplay] scene initialized (frame %u)\n",
                (unsigned)s_gameplay_frame);
    }

    /* Fade-in from black */
    if (s_gameplay_state == 0) {
        u32 factor = 16 - (s_gameplay_frame * 16) / GAMEPLAY_FADE_DURATION;
        if (factor > 16) factor = 16;
        *(volatile u16 *)(uintptr_t)REG_MASTER_BRIGHT =
            (u16)(0x4000u | (factor & 0x1F));

        if (s_gameplay_frame >= GAMEPLAY_FADE_DURATION) {
            s_gameplay_state = 1;
            *(volatile u16 *)(uintptr_t)REG_MASTER_BRIGHT = 0;
            fprintf(stderr, "[gameplay] fade-in complete, now active\n");
        }
    }

    /* Active gameplay: player movement + camera follow */
    if (s_gameplay_state == 1 && s_map_loaded) {
        /* Read NDS keypad register (active low) — use nds_reg_read16 to
         * ensure we read from the IO shadow regardless of VirtualAlloc. */
        extern uint16_t nds_reg_read16(uint32_t addr);
        u16 keyinput = nds_reg_read16(0x04000130u);
        u16 held = ~keyinput & 0x03FFu;
        static u16 s_prev_held = 0;
        u16 pressed_new = held & ~s_prev_held;
        s_prev_held = held;

        /* Player horizontal movement */
        s_player_moving = 0;
        if (held & 0x0040) { s_player_x -= PLAYER_SPEED; s_player_facing = 1; s_player_moving = 1; }
        if (held & 0x0080) { s_player_x += PLAYER_SPEED; s_player_facing = 0; s_player_moving = 1; }

        /* Jump (A button = Z key) */
        if ((pressed_new & 0x0001) && s_player_on_ground) {
            s_player_vy = PLAYER_JUMP_VEL;
            s_player_on_ground = 0;
        }

        /* Gravity */
        s_player_vy += PLAYER_GRAVITY;
        if (s_player_vy > PLAYER_MAX_FALL) s_player_vy = PLAYER_MAX_FALL;
        s_player_y += s_player_vy / 4;

        /* Simple ground at y=160 (bottom of visible area relative to map) */
        if (s_player_y >= 160) {
            s_player_y = 160;
            s_player_vy = 0;
            s_player_on_ground = 1;
        }

        /* Wrap player within map bounds */
        if (s_player_x < 0) s_player_x += MAP_PX_W;
        if (s_player_x >= MAP_PX_W) s_player_x -= MAP_PX_W;
        if (s_player_y < 0) s_player_y = 0;

        /* Luigi follows Mario with slight delay (trails behind) */
        {
            int dx = s_player_x - s_luigi_x;
            int dy = s_player_y - s_luigi_y;
            /* Handle wrap-around */
            if (dx > MAP_PX_W / 2) dx -= MAP_PX_W;
            if (dx < -MAP_PX_W / 2) dx += MAP_PX_W;
            if (dx > 2 || dx < -2) s_luigi_x += dx / 4;
            if (dy > 2 || dy < -2) s_luigi_y += dy / 4;
            if (dx != 0) s_luigi_facing = (dx < 0) ? 1 : 0;
        }

        /* Animation frame cycling */
        s_anim_timer++;
        if (s_player_moving || !s_player_on_ground) {
            if (s_anim_timer >= ANIM_SPEED) {
                s_anim_timer = 0;
                s_anim_frame = (s_anim_frame + 1) % 4;
            }
        } else {
            s_anim_frame = 0;  /* idle = frame 0 */
            s_anim_timer = 0;
        }

        /* Camera follows player (centered) */
        s_scroll_x = s_player_x - 128;  /* center horizontally */
        s_scroll_y = s_player_y - 80;   /* slightly above center */
        if (s_scroll_x < 0) s_scroll_x += MAP_PX_W;
        if (s_scroll_x >= MAP_PX_W) s_scroll_x -= MAP_PX_W;
        if (s_scroll_y < 0) s_scroll_y += MAP_PX_H;
        if (s_scroll_y >= MAP_PX_H) s_scroll_y -= MAP_PX_H;

        /* Update BG scroll registers */
        *(volatile u16 *)(uintptr_t)REG_BG0HOFS = (u16)(s_scroll_x & 0x1FF);
        *(volatile u16 *)(uintptr_t)REG_BG0VOFS = (u16)(s_scroll_y & 0x1FF);
        /* BG1 scrolls at 3/4 speed for mid-layer parallax */
        *(volatile u16 *)(uintptr_t)REG_BG1HOFS = (u16)(((s_scroll_x * 3) / 4) & 0x1FF);
        *(volatile u16 *)(uintptr_t)REG_BG1VOFS = (u16)(((s_scroll_y * 3) / 4) & 0x1FF);
        /* BG2 scrolls at half speed for background parallax */
        *(volatile u16 *)(uintptr_t)REG_BG2HOFS = (u16)((s_scroll_x / 2) & 0x1FF);
        *(volatile u16 *)(uintptr_t)REG_BG2VOFS = (u16)((s_scroll_y / 2) & 0x1FF);

        /* Update character sprites with animation */
        if (s_sprites_loaded) {
            int mario_tile = s_anim_frame * MARIO_TILES_PER_FRAME;
            int luigi_tile = LUIGI_OBJ_VRAM_OFFSET / 32 + s_anim_frame * MARIO_TILES_PER_FRAME;

            /* Mario position (screen-relative) */
            int m_scr_x = s_player_x - s_scroll_x;
            int m_scr_y = s_player_y - s_scroll_y;
            if (m_scr_x < 0) m_scr_x += MAP_PX_W;
            if (m_scr_y < 0) m_scr_y += MAP_PX_H;
            setup_player_oam(0, m_scr_x - 16, m_scr_y - 16, mario_tile, 1, 32, 32,
                             s_player_facing);

            /* Luigi position (screen-relative) */
            int l_scr_x = s_luigi_x - s_scroll_x;
            int l_scr_y = s_luigi_y - s_scroll_y;
            if (l_scr_x < 0) l_scr_x += MAP_PX_W;
            if (l_scr_y < 0) l_scr_y += MAP_PX_H;
            setup_player_oam(1, l_scr_x - 16, l_scr_y - 16, luigi_tile, 2, 32, 32,
                             s_luigi_facing);
        }

        /* L/R buttons cycle through maps */
        if (s_num_map_groups > 1) {
            if (pressed_new & 0x0200) {  /* R button = next map */
                s_current_map = (s_current_map + 1) % s_num_map_groups;
                if (load_fmap_all_layers(s_map_groups[s_current_map][0],
                                          s_map_groups[s_current_map][1])) {
                    s_player_x = 128; s_player_y = 128;
                    fprintf(stderr, "[gameplay] switched to map %d/%d (entries %d-%d)\n",
                            s_current_map, s_num_map_groups,
                            s_map_groups[s_current_map][0],
                            s_map_groups[s_current_map][1]);
                }
            }
            if (pressed_new & 0x0100) {  /* L button = prev map */
                s_current_map = (s_current_map + s_num_map_groups - 1) % s_num_map_groups;
                if (load_fmap_all_layers(s_map_groups[s_current_map][0],
                                          s_map_groups[s_current_map][1])) {
                    s_player_x = 128; s_player_y = 128;
                    fprintf(stderr, "[gameplay] switched to map %d/%d (entries %d-%d)\n",
                            s_current_map, s_num_map_groups,
                            s_map_groups[s_current_map][0],
                            s_map_groups[s_current_map][1]);
                }
            }
        }
    }
}

/* ---- Constructor (FUN_0206DE6C HOST_PORT) ---- */
/* Forward to the ARM interpreter so the REAL overlay code runs. */
extern uint32_t arm_interp_call(uint32_t addr, uint32_t r0, uint32_t r1,
                                uint32_t r2, uint32_t r3);
extern int arm_interp_is_enabled(void);

void FUN_0206DE6C(void *obj_ptr, int type, int param)
{
    /* The shim's OS_Alloc returns a host-heap pointer (0x1000xxxx) which
     * is not in NDS-mapped memory.  Allocate from NDS space instead. */
    (void)obj_ptr;
    u32 obj_nds = nds_bump_alloc(0x3B8);

    fprintf(stderr,
            "[FUN_0206DE6C] gameplay ctor via ARM interpreter: "
            "obj=0x%08X type=%d param=%d\n",
            (unsigned)obj_nds, type, param);
    fflush(stderr);

    s_gameplay_obj_nds = obj_nds;

    /* Execute the REAL ARM overlay code at 0x0206DE6C.
     * The interpreter will:
     *   - fill in the scene object fields
     *   - install the real vtable (with tick/dtor/setup addresses)
     *   - call FUN_0202a74c (scene queue insert) via native bridge
     * When the scene queue later calls vtable[2] (tick), nds_call_2arg
     * falls through to the interpreter since those overlay addresses
     * aren't in the fnptr table. */
    if (arm_interp_is_enabled()) {
        extern uint64_t arm_interp_total_cycles(void);
        uint64_t cyc_before = arm_interp_total_cycles();
        arm_interp_call(0x0206DE6Cu, obj_nds, (uint32_t)type, (uint32_t)param, 0);
        uint64_t cyc_after = arm_interp_total_cycles();
        fprintf(stderr,
                "[FUN_0206DE6C] interpreter returned (%llu cycles). "
                "Checking scene queue...\n",
                (unsigned long long)(cyc_after - cyc_before));

        /* Diagnostic: check what the constructor wrote to the object */
        u32 vtab_ptr = *(volatile u32 *)(uintptr_t)obj_nds;
        u16 flags = *(volatile u16 *)(uintptr_t)(obj_nds + 0x12);
        u32 next = *(volatile u32 *)(uintptr_t)(obj_nds + 0x0c);
        fprintf(stderr,
                "[FUN_0206DE6C] obj=0x%08X vtab=0x%08X flags=0x%04X next=0x%08X\n",
                (unsigned)obj_nds, (unsigned)vtab_ptr,
                (unsigned)flags, (unsigned)next);
        if (vtab_ptr >= 0x02000000u && vtab_ptr < 0x02400000u) {
            u32 vt0 = *(volatile u32 *)(uintptr_t)(vtab_ptr + 0);
            u32 vt1 = *(volatile u32 *)(uintptr_t)(vtab_ptr + 4);
            u32 vt2 = *(volatile u32 *)(uintptr_t)(vtab_ptr + 8);
            fprintf(stderr,
                    "[FUN_0206DE6C] vtable: [0]=0x%08X [1]=0x%08X [2]=0x%08X\n",
                    (unsigned)vt0, (unsigned)vt1, (unsigned)vt2);
        }

        /* Check scene queue anchor */
        u32 anchor = 0x02060A04u;
        u32 q_head = *(volatile u32 *)(uintptr_t)(anchor + 0x00);
        u16 q_count = *(volatile u16 *)(uintptr_t)(anchor + 0x08);
        fprintf(stderr,
                "[FUN_0206DE6C] scene queue: head=0x%08X count=%u\n",
                (unsigned)q_head, (unsigned)q_count);

        /* Dump key object fields to understand constructor output */
        fprintf(stderr, "[FUN_0206DE6C] obj dump:\n");
        for (int off = 0x28; off <= 0x60; off += 4) {
            u32 val = *(volatile u32 *)(uintptr_t)(obj_nds + off);
            if (val != 0) {
                fprintf(stderr, "  +0x%03X = 0x%08X\n", off, (unsigned)val);
            }
        }
        /* Check sub-object pointers */
        for (int off = 0x390; off <= 0x3B8; off += 4) {
            u32 val = *(volatile u32 *)(uintptr_t)(obj_nds + off);
            if (val != 0) {
                fprintf(stderr, "  +0x%03X = 0x%08X\n", off, (unsigned)val);
            }
        }
        fflush(stderr);

        /* By default, let the REAL overlay 0 tick run via ARM interpreter
         * so the game's own code drives VRAM/OAM/palette writes.
         * Set MLPIT_FAKE_TICK=1 to use the test-harness tick instead. */
        if (getenv("MLPIT_FAKE_TICK") && getenv("MLPIT_FAKE_TICK")[0] == '1') {
            host_fnptr_register(OV0_TICK_ADDR, (void *)host_gameplay_tick);
            fprintf(stderr,
                    "[FUN_0206DE6C] FAKE tick registered (MLPIT_FAKE_TICK=1)\n");
        } else {
            /* The tick's phase 0 checks *(u16*)(obj[0x44]+0x440) == 2.
             * This is the BG layer 2 index written by FUN_020192f8 into
             * the display context at *DAT_02019730.  The constructor
             * doesn't set obj[0x44] — it's supposed to be connected to
             * the display context by the game's init chain.
             *
             * Read the display context pointer from NDS memory at
             * 0x02019730 and wire it into obj+0x44 so the tick can
             * see the layer-init flags that FUN_020192f8 already set. */
            u32 disp_ctx = *(volatile u32 *)(uintptr_t)0x02019730u;
            fprintf(stderr,
                    "[FUN_0206DE6C] NDS DAT_02019730 = 0x%08X\n",
                    (unsigned)disp_ctx);
            if (disp_ctx >= 0x02000000u && disp_ctx < 0x03000000u) {
                *(volatile u32 *)(uintptr_t)(obj_nds + 0x44) = disp_ctx;
                /* FUN_020192f8 (display init) writes layer indices to the
                 * HOST_PORT g_disp_DAT_02019730 variable, not to NDS RAM.
                 * The ARM interpreter reads from NDS RAM at disp_ctx+0x440.
                 * Sync all 4 layer index flags so the tick can see them:
                 *   +0x3F8 = 0 (layer 0), +0x41C = 1 (layer 1),
                 *   +0x440 = 2 (layer 2), +0x464 = 3 (layer 3) */
                *(volatile u16 *)(uintptr_t)(disp_ctx + 0x3F8) = 0;
                *(volatile u16 *)(uintptr_t)(disp_ctx + 0x41C) = 1;
                *(volatile u16 *)(uintptr_t)(disp_ctx + 0x440) = 2;
                *(volatile u16 *)(uintptr_t)(disp_ctx + 0x464) = 3;
                u16 flag = *(volatile u16 *)(uintptr_t)(disp_ctx + 0x440);
                fprintf(stderr,
                        "[FUN_0206DE6C] wired obj+0x44 → display ctx 0x%08X "
                        "(flag@+0x440 = %u, synced layer indices)\n",
                        (unsigned)disp_ctx, (unsigned)flag);
            } else {
                fprintf(stderr,
                        "[FUN_0206DE6C] WARNING: DAT_02019730 not in NDS RAM "
                        "(0x%08X) — cannot wire display ctx\n",
                        (unsigned)disp_ctx);
            }
            fprintf(stderr,
                    "[FUN_0206DE6C] REAL overlay 0 tick at 0x%08X\n",
                    (unsigned)OV0_TICK_ADDR);
        }
        fflush(stderr);
    } else {
        /* Fallback: manual setup (old behavior) — still use real vtable */
        FUN_0202a74c_real(obj_nds, (u8)type, (u32)param, 0);

        u32 vtab_nds = nds_bump_alloc(16);
        volatile u32 *vtab = (volatile u32 *)(uintptr_t)vtab_nds;
        vtab[0] = OV0_DTOR_ADDR;
        vtab[1] = OV0_SETUP_ADDR;
        vtab[2] = OV0_TICK_ADDR;
        *(volatile u32 *)(uintptr_t)obj_nds = vtab_nds;

        /* No host overrides — let ARM interpreter run real tick */

        *(volatile u32 *)(uintptr_t)(obj_nds + 0x2c) = 0;
        *(volatile u16 *)(uintptr_t)(obj_nds + 0x12) |= 0x0001;
    }

    fprintf(stderr,
            "[FUN_0206DE6C] gameplay scene created: obj=0x%08X\n",
            (unsigned)obj_nds);
    fflush(stderr);
}
