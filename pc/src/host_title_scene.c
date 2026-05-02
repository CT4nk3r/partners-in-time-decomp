/*
 * host_title_scene.c — HOST_PORT implementation of overlay 8's title screen
 * constructor (FUN_02077784) and tick function (FUN_02077444).
 *
 * Background:
 *   The original game's title screen path runs through overlay 8, which is
 *   loaded at 0x0206A800.  Multiple overlays (6, 7, 8, 9) share this RAM
 *   range; overlay 8 is the one active during the title screen scene.
 *
 *   The scene flow is:
 *     game_setup_overlay → creates scene anchor (vtab[2]=FUN_02005d6c)
 *       → FUN_02005d6c phase 0: loads 77KB of assets from ROM
 *       → FUN_02005d6c phase 1, state 9: calls alloc_construct_obj_g
 *         → OS_Alloc(0x30) + FUN_02077784(ptr, 8, 0)
 *
 *   FUN_02077784 (ov8) is the title screen constructor:
 *     - Inserts the object into the scene queue (FUN_0202a74c)
 *     - Installs vtable at obj[0] pointing to 0x02077EC4
 *     - Sets up BG layers, sprites, palettes for the title screen
 *     - Allocates sub-objects for animation
 *
 *   FUN_02077444 (ov8) is the per-frame tick function (vtable[2]):
 *     - field_2c == 0: normal frame (render + input)
 *     - field_2c == 1: teardown (call dtor, transition to next scene)
 *
 *   On the host we can't execute ARM overlay binary.  Instead we provide
 *   C implementations that:
 *     - Use the existing host_title_screen_load() for VRAM setup
 *     - Register a host tick function with the fnptr resolver
 *     - Handle scene transitions when Start is pressed
 */
#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- External interfaces ---- */
extern void host_fnptr_register(uint32_t nds_addr, void *host_fn);
extern int  host_title_screen_load(void);
extern int  nds_arm9_ram_is_mapped(void);
extern void *nds_vram_bank(char bank);
extern uint32_t nds_vram_bank_size(char bank);

/* FUN_0202a74c_real inserts a node into the scene linked-list.
 * We use the same call host_game_setup_overlay.c uses. */
extern void FUN_0202a74c_real(u32 node_addr, u8 priority, u32 r2_unused, u32 r3_param);

/* ---- NDS addresses for ov8 title screen ---- */
#define OV8_VTABLE_ADDR       0x02077EC4u  /* vtable installed at obj[0] */
#define OV8_TICK_ADDR         0x02077444u  /* vtable[2] = per-frame tick */
#define OV8_DTOR_ADDR         0x0207756Cu  /* vtable[1] = destructor     */
#define OV8_GLOBAL_OBJ_ADDR   0x0207A6F8u  /* global ptr to title obj    */

/* Scene anchor pointer (used by teardown to trigger scene transition) */
#define SCENE_PTR_NDS         0x02059C7Cu

/* NDS-mapped bump allocator for scene nodes (shared with host_game_setup_overlay.c).
 * Must be past arm9.bin's mapped region and any existing allocations. */
static u32 s_title_arena = 0x02302000u;

static u32 nds_bump_alloc(u32 size) {
    u32 addr = s_title_arena;
    size = (size + 0xF) & ~0xFu;  /* align to 16 */
    s_title_arena += size;
    memset((void *)(uintptr_t)addr, 0, size);
    return addr;
}

/* ---- Title screen tick function (replaces ov8 FUN_02077444) ---- */

/*
 * Original ov8 FUN_02077444 logic:
 *   if (obj->field_2c == 1):
 *     call vtable[1] (destructor)
 *     FUN_0202b720(8, 0) — stop streaming
 *     FUN_0202b720(5, 0) — stop streaming
 *     FUN_02029788(0)     — reset memory pools
 *     scene_anchor = *(u32*)0x02059C7C
 *     FUN_02005d3c(scene_anchor, 2) — set scene state=2 (next scene)
 *     return
 *
 *   ready = FUN_02065b5c()  — check display ready
 *   if (!ready) return
 *
 *   FUN_02068cbc(0/1)      — update/render both screens
 *   FUN_0206619c(0, 10)    — animation timing
 *   FUN_02068c60(0/1)      — commit screens
 *   FUN_02068b60(0/1, ...)  — read pad input
 *   FUN_02009560(...)       — process input
 *
 * For the host port, we:
 *   - Skip the display-ready check (always ready)
 *   - Don't call ov8 rendering functions (manual title screen already rendered)
 *   - Check SDL input for Start/Enter to trigger teardown
 */
/* Scene transition: sets scene anchor phase=2 (teardown/next scene) */
extern void FUN_02005d3c(int param_1, u8 param_2);

/* NDS REG_KEYINPUT bits (active-low: 0 = pressed) */
#define NDS_REG_KEYINPUT  0x04000130u
#define KEY_START         (1u << 3)
#define KEY_A             (1u << 0)

/* NDS register addresses for BG scroll and brightness */
#define REG_BG0HOFS       0x04000010u
#define REG_BG0VOFS       0x04000012u
#define REG_MASTER_BRIGHT 0x0400006Cu  /* main engine master brightness */

/* Title animation state */
static u32 s_title_frame_count = 0;
static u32 s_fade_frame = 0;
#define TITLE_FADE_DURATION 30  /* ~0.5 sec at 60fps */

/* Write a 16-bit value to an NDS IO register */
static void title_reg_write16(u32 addr, u16 val) {
    *(volatile u16 *)(uintptr_t)addr = val;
}

static void host_title_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;

    u32 field_2c = *(volatile u32 *)(uintptr_t)(node_addr + 0x2c);

    /* Already torn down — do nothing (node should have been removed from
     * queue by the destructor, but our host_title_dtor is a no-op). */
    if (field_2c >= 2) return;

    if (field_2c == 1) {
        /* Fade-to-black transition before scene change.
         * Master brightness register: bits 0-4 = factor (0=normal, 16=black),
         * bit 14 = mode (1 = darken). Write increasing factor each frame. */
        s_fade_frame++;
        u32 factor = (s_fade_frame * 16) / TITLE_FADE_DURATION;
        if (factor > 16) factor = 16;
        title_reg_write16(REG_MASTER_BRIGHT, (u16)(0x4000u | (factor & 0x1F)));

        if (s_fade_frame < TITLE_FADE_DURATION) {
            return;  /* still fading */
        }

        /* Fade complete — now actually transition */
        *(volatile u32 *)(uintptr_t)(node_addr + 0x2c) = 2;

        static int teardown_logged = 0;
        if (!teardown_logged) {
            teardown_logged = 1;
            fprintf(stderr,
                    "[title_tick] teardown: fade complete, transitioning to next scene\n");
            fflush(stderr);
        }

        /* Get scene anchor from the global pointer */
        u32 scene_anchor = *(volatile u32 *)(uintptr_t)SCENE_PTR_NDS;
        if (scene_anchor >= 0x02000000u && scene_anchor < 0x02400000u) {
            FUN_02005d3c((int)scene_anchor, 2);
        }
        return;
    }

    /* Normal frame: animate title screen */
    s_title_frame_count++;

    /* 1. Scroll the sky background (BG0) slowly to the right.
     *    The original game scrolls the sky ~0.5px/frame for a gentle drift. */
    u16 sky_scroll = (u16)(s_title_frame_count / 2);
    title_reg_write16(REG_BG0HOFS, sky_scroll);

    /* 2. Check for Start or A button press.
     * SDL keyboard events are mapped to NDS REG_KEYINPUT at 0x04000130
     * by platform_sdl.c → pump_input_to_io(). */
    u16 keyinput = *(volatile u16 *)(uintptr_t)NDS_REG_KEYINPUT;
    int start_pressed = !(keyinput & KEY_START);
    int a_pressed     = !(keyinput & KEY_A);

    static int first_frame = 1;
    if (first_frame) {
        first_frame = 0;
        fprintf(stderr,
                "[title_tick] first tick at node=0x%08X — "
                "press START (Enter) or A (Z) to continue\n",
                (unsigned)(u32)node_addr);
        fflush(stderr);
    }

    if (start_pressed || a_pressed) {
        static int press_logged = 0;
        if (!press_logged) {
            press_logged = 1;
            fprintf(stderr,
                    "[title_tick] Start/A pressed! Fading to black...\n");
            fflush(stderr);
        }
        /* Begin fade transition (field_2c=1) */
        *(volatile u32 *)(uintptr_t)(node_addr + 0x2c) = 1;
        s_fade_frame = 0;
    }
}

/* ---- Title screen destructor (replaces ov8 FUN_0207756C) ---- */
static void host_title_dtor(uintptr_t node_addr, uintptr_t unused)
{
    (void)unused;
    fprintf(stderr, "[title_dtor] node=0x%08X destroyed\n",
            (unsigned)(u32)node_addr);
}

/* ---- FUN_02077784 — title screen constructor (replaces ov8 version) ---- */

/*
 * Called from alloc_construct_obj_g (FUN_020290e4) with:
 *   ptr  = OS_Alloc'd 0x30-byte buffer (host heap pointer)
 *   type = 8
 *   param = 0
 *
 * Problem: the ptr from OS_Alloc is a host 64-bit pointer that doesn't fit
 * in a u32 scene queue node.  We allocate our own node from NDS-mapped RAM
 * and ignore the host ptr.
 *
 * Original ov8 FUN_02077784 does:
 *   1. FUN_0202a74c(obj, 8, 0, 0)       — insert into scene queue
 *   2. obj[0] = 0x02077EC4              — install vtable
 *   3. *(u32*)0x0207A6F8 = obj          — store global ptr
 *   4. FUN_02065da8(0)                   — display init
 *   5. FUN_0206805c(0), FUN_0206805c(1)  — BG layer init (both screens)
 *   6. FUN_0206749c(2, 0x10)             — palette init
 *   7. Clear POWCNT1 bit 15
 *   8. FUN_0203600c(1), FUN_02036178(2)  — VRAM bank config
 *   9. FUN_02035a00(8), FUN_02035a7c(4)  — tile/map base config
 *   10. FUN_02035158(1,3,0), FUN_0203513c(0) — BG priority
 *   11. Set up BG scroll for both screens (BG layers 1 & 2)
 *   12. FUN_020662b8(0x80), FUN_02068d70(0x100) — sprite alloc
 *   13. FUN_020697d4(), FUN_02068928(0x40) — more sprite init
 *   14. OS_Alloc(0x335C) → animation data buffer
 *   15. FUN_02067204 with the animation buffer
 *   16. FUN_02008fc0(0), FUN_02008fc0(1) — screen fade
 *   17. OS_AllocBack(0x4400) → OAM double buffer
 *   18. FUN_02009040 for both screens (OAM setup)
 *   19. OS_Alloc(0x28) → sub-object (FUN_02077AF8 ctor)
 *   20. Store sub-object at obj+0x28
 *   21. Configure DISPCNT (clear BG mode bits)
 *   22. FUN_02029ffc(sub_obj) — schedule sub-object
 *   23. FUN_020351cc() — commit BG config
 *   24. Set sub-engine DISPCNT bit 16
 *   25. FUN_0207701c(obj) — final init
 *   26. FUN_0206621c(0x020768F0, 6, 1) — register callback
 *   27. obj->field_2c = 0
 */
void FUN_02077784(void *ptr, int type, int param)
{
    (void)ptr;   /* ignore host-heap ptr; allocate our own in NDS RAM */
    (void)type;
    (void)param;

    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr, "[FUN_02077784] ARM9 RAM not mapped, skipping\n");
        return;
    }

    /* Allocate the title scene node in NDS-mapped RAM so the scene queue
     * (which stores u32 NDS addresses) can access it. */
    u32 obj_nds = nds_bump_alloc(0x30);
    fprintf(stderr,
            "[FUN_02077784] title scene ctor: obj=0x%08X type=%d param=%d\n",
            (unsigned)obj_nds, type, param);

    /* 1. Insert into the scene queue FIRST.
     * FUN_0202a74c_real writes a default vtable (0x020597C8) at node[0],
     * so we must install our real vtable AFTER this call. */
    FUN_0202a74c_real(obj_nds, /*priority*/ 8, 0, 0);

    /* 2. Create vtable in NDS-mapped RAM.
     * vtable[0] = destructor address (0x0207756C → host_title_dtor)
     * vtable[1] = handler address    (unused for now)
     * vtable[2] = tick address       (0x02077444 → host_title_tick) */
    u32 vtab_nds = nds_bump_alloc(16);
    volatile u32 *vtab = (volatile u32 *)(uintptr_t)vtab_nds;
    vtab[0] = OV8_DTOR_ADDR;
    vtab[1] = OV8_DTOR_ADDR;  /* placeholder */
    vtab[2] = OV8_TICK_ADDR;

    /* 3. Install vtable pointer at obj[0] — MUST be after FUN_0202a74c_real
     * which writes a default vtable that we need to overwrite. */
    *(volatile u32 *)(uintptr_t)obj_nds = vtab_nds;

    /* 4. Register host functions with the fnptr resolver so nds_call_2arg
     * can translate the NDS addresses to host function pointers. */
    host_fnptr_register(OV8_TICK_ADDR, (void *)host_title_tick);
    host_fnptr_register(OV8_DTOR_ADDR, (void *)host_title_dtor);

    /* 4. Store global pointer so other code can find the title scene object */
    if (nds_arm9_ram_is_mapped()) {
        *(volatile u32 *)(uintptr_t)OV8_GLOBAL_OBJ_ADDR = obj_nds;
    }

    /* 5. Set up the title screen graphics.
     * Our existing host_title_screen_load() handles:
     *   - Decompressing TitleBG.dat (FAT[99])
     *   - Setting up sky (BG0), portal left (BG1), portal right (BG2)
     *   - Palettes and extended palettes
     *   - BG register configuration (DISPCNT, BGxCNT, BGxOFS) */
    host_title_screen_load();

    /* 6. Initialize field_2c = 0 (normal state, not transitioning) */
    *(volatile u32 *)(uintptr_t)(obj_nds + 0x2c) = 0;

    /* 7. Set the "needs update" flag so the scene queue processor
     * actually calls our tick function.
     * The queue processor checks (flags & 1) && !(flags & 2).
     * flags are at node + link_offset + 6 = node + 0x0c + 6 = node + 0x12 */
    volatile u16 *flags_ptr = (volatile u16 *)(uintptr_t)(obj_nds + 0x12);
    *flags_ptr |= 0x0001;  /* needs_update = 1 */

    fprintf(stderr,
            "[FUN_02077784] title scene created: obj=0x%08X vtab=0x%08X "
            "tick=0x%08X\n",
            (unsigned)obj_nds, (unsigned)vtab_nds, (unsigned)OV8_TICK_ADDR);
    fflush(stderr);
}

/* ==================================================================
 * FUN_020739ec — "file select" / next scene constructor (ov8)
 * ==================================================================
 *
 * Called from alloc_construct_obj_h (FUN_02029128) during phase 3
 * state 2 of the scene state machine, after the title screen
 * transitions.  On real hardware this creates the file-select or
 * story-intro scene.
 *
 * For the HOST_PORT we create a minimal scene node that ticks
 * without crashing — proving the full scene transition pipeline
 * works end-to-end.  Actual rendering will come in later phases.
 */

/* ==================================================================
 * File Select Screen — HOST_PORT implementation
 * ==================================================================
 *
 * After the title screen, the game shows a file select menu.
 * We implement this as a tile-based UI using the existing BG renderer:
 * - A simple 1-bit pixel font for text (A-Z, 0-9, symbols)
 * - Menu items: File 1, File 2, File 3 (empty), New Game
 * - Arrow key navigation, Enter/Z to select
 * - Selecting "New Game" transitions to the next scene (gameplay intro)
 */

/* ---- Minimal 8x8 pixel font (uppercase + digits) ---- */
/* Each glyph is 8 rows of 8 pixels, stored as 8 bytes (1 bit per pixel).
 * We convert these to 4bpp tiles when writing to VRAM. */
static const uint8_t s_font_glyphs[][8] = {
    /* ' ' (0) */ {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    /* A   (1) */ {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},
    /* B   (2) */ {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
    /* C   (3) */ {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
    /* D   (4) */ {0x7C,0x66,0x66,0x66,0x66,0x66,0x7C,0x00},
    /* E   (5) */ {0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00},
    /* F   (6) */ {0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00},
    /* G   (7) */ {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3E,0x00},
    /* H   (8) */ {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
    /* I   (9) */ {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
    /* J  (10) */ {0x1E,0x0C,0x0C,0x0C,0x6C,0x6C,0x38,0x00},
    /* K  (11) */ {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
    /* L  (12) */ {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
    /* M  (13) */ {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
    /* N  (14) */ {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
    /* O  (15) */ {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    /* P  (16) */ {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
    /* Q  (17) */ {0x3C,0x66,0x66,0x66,0x6A,0x6C,0x36,0x00},
    /* R  (18) */ {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00},
    /* S  (19) */ {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
    /* T  (20) */ {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    /* U  (21) */ {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    /* V  (22) */ {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
    /* W  (23) */ {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
    /* X  (24) */ {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
    /* Y  (25) */ {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00},
    /* Z  (26) */ {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00},
    /* 0  (27) */ {0x3C,0x66,0x6E,0x7E,0x76,0x66,0x3C,0x00},
    /* 1  (28) */ {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
    /* 2  (29) */ {0x3C,0x66,0x06,0x1C,0x30,0x60,0x7E,0x00},
    /* 3  (30) */ {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
    /* :  (31) */ {0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00},
    /* -  (32) */ {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00},
    /* >  (33) */ {0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00},
    /* (  (34) */ {0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00},
    /* )  (35) */ {0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00},
};
#define FONT_GLYPH_COUNT (sizeof(s_font_glyphs)/sizeof(s_font_glyphs[0]))

/* Map ASCII char to glyph index */
static int char_to_glyph(char c) {
    if (c >= 'A' && c <= 'Z') return 1 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 1 + (c - 'a');
    if (c >= '0' && c <= '9') return 27 + (c - '0');
    if (c == ':') return 31;
    if (c == '-') return 32;
    if (c == '>') return 33;
    if (c == '(') return 34;
    if (c == ')') return 35;
    return 0;  /* space */
}

/* Write all font glyphs as 4bpp tiles to VRAM */
static void write_font_tiles(uint8_t *vram_tiles) {
    for (u32 g = 0; g < FONT_GLYPH_COUNT; g++) {
        uint8_t *tile = vram_tiles + g * 32;  /* 4bpp = 32 bytes per tile */
        for (int row = 0; row < 8; row++) {
            uint8_t bits = s_font_glyphs[g][row];
            for (int col = 0; col < 8; col += 2) {
                uint8_t lo = (bits >> (7 - col)) & 1 ? 2 : 0;
                uint8_t hi = (bits >> (7 - col - 1)) & 1 ? 2 : 0;
                tile[row * 4 + col / 2] = lo | (hi << 4);
            }
        }
    }
}

/* Write a string to the tilemap at (tx, ty) in tile coordinates */
static void write_text(uint16_t *tilemap, int tx, int ty,
                       const char *text, uint16_t pal_bank)
{
    while (*text) {
        if (tx < 32 && ty < 32) {
            int g = char_to_glyph(*text);
            tilemap[ty * 32 + tx] = (uint16_t)(g | (pal_bank << 12));
        }
        tx++;
        text++;
    }
}

/* File select state */
static int s_filesel_cursor = 3;  /* 0-2=files, 3=new game */
#define FILESEL_ITEMS 4

static void host_next_scene_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    (void)node_addr;

    static u32 next_frame = 0;
    next_frame++;

    /* Fade-in from black over 30 frames when entering this scene */
    if (next_frame <= TITLE_FADE_DURATION) {
        u32 factor = 16 - (next_frame * 16) / TITLE_FADE_DURATION;
        if (factor > 16) factor = 16;
        title_reg_write16(REG_MASTER_BRIGHT, (u16)(0x4000u | (factor & 0x1F)));
    } else if (next_frame == TITLE_FADE_DURATION + 1) {
        title_reg_write16(REG_MASTER_BRIGHT, 0);
    }

    /* Read input (same mechanism as title screen) */
    u16 keyinput = *(volatile u16 *)(uintptr_t)NDS_REG_KEYINPUT;
    static u16 prev_keyinput = 0xFFFF;
    u16 pressed = prev_keyinput & ~keyinput;  /* newly pressed (active-low) */
    prev_keyinput = keyinput;

    #define KEY_UP    (1u << 6)
    #define KEY_DOWN  (1u << 7)

    if (pressed & KEY_UP) {
        s_filesel_cursor--;
        if (s_filesel_cursor < 0) s_filesel_cursor = FILESEL_ITEMS - 1;
    }
    if (pressed & KEY_DOWN) {
        s_filesel_cursor++;
        if (s_filesel_cursor >= FILESEL_ITEMS) s_filesel_cursor = 0;
    }

    /* Confirm selection */
    if ((pressed & KEY_START) || (pressed & KEY_A)) {
        fprintf(stderr,
                "[file_select] selected item %d%s\n",
                s_filesel_cursor,
                s_filesel_cursor == 3 ? " (NEW GAME)" : " (empty file)");
        fflush(stderr);
        /* TODO: transition to gameplay intro for New Game */
    }

    /* Update the cursor position in the tilemap every frame */
    void *a = nds_vram_bank('A');
    if (a) {
        uint16_t *map = (uint16_t *)((uint8_t *)a + 0x8000);
        /* menu rows: File 1 @ row 8, File 2 @ row 11, File 3 @ row 14, New Game @ row 17 */
        int menu_rows[] = {8, 11, 14, 17};
        for (int i = 0; i < FILESEL_ITEMS; i++) {
            int row = menu_rows[i];
            /* Column 4 is the cursor column */
            if (i == s_filesel_cursor) {
                /* Blink the cursor arrow */
                int show = (next_frame / 15) & 1;
                map[row * 32 + 4] = show ? (uint16_t)(char_to_glyph('>') | (1 << 12)) : 0;
            } else {
                map[row * 32 + 4] = 0;  /* blank */
            }
        }
    }

    static int logged = 0;
    if (!logged) {
        logged = 1;
        fprintf(stderr,
                "[file_select] active - use Up/Down arrows + Enter to select\n");
        fflush(stderr);
    }
}

static void host_next_scene_dtor(uintptr_t node_addr, uintptr_t unused)
{
    (void)unused;
    fprintf(stderr, "[file_select_dtor] node=0x%08X destroyed\n",
            (unsigned)(u32)node_addr);
}

/* NDS addresses for the file select scene (distinct from title screen) */
#define OV8_NEXT_TICK_ADDR    0x020739A0u
#define OV8_NEXT_DTOR_ADDR    0x020739A4u

void FUN_020739ec(void *ptr, int type, int param)
{
    (void)ptr;
    (void)type;
    (void)param;

    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr, "[FUN_020739ec] ARM9 RAM not mapped, skipping\n");
        return;
    }

    /* Clear title screen graphics from VRAM */
    void *a = nds_vram_bank('A');
    void *b = nds_vram_bank('B');
    if (a) memset(a, 0, nds_vram_bank_size('A'));
    if (b) memset(b, 0, nds_vram_bank_size('B'));

    /* Set up palette for the file select screen */
    void *e = nds_vram_bank('E');
    if (e) {
        uint16_t *pal = (uint16_t *)e;
        /* Palette bank 0: menu text */
        pal[0] = 0x0000;   /* transparent / backdrop */
        pal[1] = 0x4C08;   /* dark blue-purple background */
        pal[2] = 0x7FFF;   /* white text */
        /* Palette bank 1: highlighted/cursor (yellow) */
        pal[16] = 0x0000;
        pal[17] = 0x4C08;
        pal[18] = 0x03FF;  /* yellow (R=31, G=31, B=0 in BGR555) */
    }

    if (a) {
        uint8_t *tiles = (uint8_t *)a;

        /* Write font glyph tiles starting at tile 0 */
        write_font_tiles(tiles);

        /* Also write a solid-fill tile for the background (after font glyphs).
         * Tile index = FONT_GLYPH_COUNT, filled with palette index 1. */
        uint8_t *bg_tile = tiles + (u32)FONT_GLYPH_COUNT * 32;
        memset(bg_tile, 0x11, 32);

        /* Write tilemap at bank A offset 0x8000 (screen base 16) */
        uint16_t *map = (uint16_t *)(tiles + 0x8000);

        /* Fill entire map with background tile */
        for (int i = 0; i < 32 * 32; i++)
            map[i] = (uint16_t)FONT_GLYPH_COUNT;

        /* Draw the file select menu text */
        write_text(map, 6, 2, "MARIO AND LUIGI", 0);
        write_text(map, 5, 3, "PARTNERS IN TIME", 0);

        write_text(map, 7, 5, "FILE  SELECT", 0);

        write_text(map, 6, 8,  "FILE 1 - EMPTY", 0);
        write_text(map, 6, 11, "FILE 2 - EMPTY", 0);
        write_text(map, 6, 14, "FILE 3 - EMPTY", 0);
        write_text(map, 6, 17, "NEW GAME", 1);  /* highlighted */

        /* Draw initial cursor arrow */
        map[17 * 32 + 4] = (uint16_t)(char_to_glyph('>') | (1 << 12));
    }

    /* Configure BG0: char_base=0, screen_base=16, 4bpp, 32x32, priority 0 */
    *(volatile uint16_t *)(uintptr_t)0x04000008u =
        (0 << 2) | (16 << 8) | (0 << 7) | (0 << 14) | 0;
    *(volatile uint16_t *)(uintptr_t)0x04000010u = 0;
    *(volatile uint16_t *)(uintptr_t)0x04000012u = 0;
    /* BG0 only */
    uint32_t dispcnt = *(volatile uint32_t *)(uintptr_t)0x04000000u;
    dispcnt &= ~(0x0F00u);
    dispcnt |= 0x0100u;
    *(volatile uint32_t *)(uintptr_t)0x04000000u = dispcnt;

    u32 obj_nds = nds_bump_alloc(0x30);
    fprintf(stderr,
            "[FUN_020739ec] next scene ctor: obj=0x%08X type=%d param=%d\n",
            (unsigned)obj_nds, type, param);

    /* Insert into scene queue */
    FUN_0202a74c_real(obj_nds, /*priority*/ 8, 0, 0);

    /* Create and install vtable */
    u32 vtab_nds = nds_bump_alloc(16);
    volatile u32 *vtab = (volatile u32 *)(uintptr_t)vtab_nds;
    vtab[0] = OV8_NEXT_DTOR_ADDR;
    vtab[1] = OV8_NEXT_DTOR_ADDR;
    vtab[2] = OV8_NEXT_TICK_ADDR;

    /* Install vtable AFTER FUN_0202a74c_real (which overwrites node[0]) */
    *(volatile u32 *)(uintptr_t)obj_nds = vtab_nds;

    /* Register host functions with fnptr resolver */
    host_fnptr_register(OV8_NEXT_TICK_ADDR, (void *)host_next_scene_tick);
    host_fnptr_register(OV8_NEXT_DTOR_ADDR, (void *)host_next_scene_dtor);

    /* Set field_2c = 0 (normal state) */
    *(volatile u32 *)(uintptr_t)(obj_nds + 0x2c) = 0;

    /* Set "needs update" flag so tick fires */
    volatile u16 *flags_ptr = (volatile u16 *)(uintptr_t)(obj_nds + 0x12);
    *flags_ptr |= 0x0001;

    fprintf(stderr,
            "[FUN_020739ec] next scene created: obj=0x%08X vtab=0x%08X\n",
            (unsigned)obj_nds, (unsigned)vtab_nds);
    fflush(stderr);
}
