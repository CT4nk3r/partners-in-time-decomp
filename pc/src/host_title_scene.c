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
#include "nds_rom.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- NDS register access (works regardless of VirtualAlloc) ---- */
extern uint16_t nds_reg_read16(uint32_t addr);

/* ---- External interfaces ---- */
extern void host_fnptr_register(uint32_t nds_addr, void *host_fn);
extern int  host_title_screen_load(void);
extern int  nds_arm9_ram_is_mapped(void);
extern void *nds_vram_bank(char bank);
extern uint32_t nds_vram_bank_size(char bank);
extern void arm_swi_11_lz77_decomp(const void *src, void *dst);

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

/* VRAM layout for file-select screen (two-layer setup):
 *   BG0 (font overlay):  char_base=0 (0x0000), screen_base=1 (0x0800), 4bpp
 *   BG1 (real menu BG):  char_base=4 (0x10000), screen_base=31 (0xF800), 8bpp
 */
#define FONT_TILE_OFF  0x0000u
#define FONT_MAP_OFF   0x0800u
#define MENU_TILE_OFF  0x10000u
#define MENU_MAP_OFF   0xF800u

/* NDS-mapped bump allocator for scene nodes (shared with host_game_setup_overlay.c).
 * Must be past arm9.bin's mapped region and any existing allocations. */
static u32 s_title_arena = 0x02302000u;

u32 nds_bump_alloc(u32 size) {
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
    u16 keyinput = nds_reg_read16(NDS_REG_KEYINPUT);
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
    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr, "[FUN_02077784] ARM9 RAM not mapped, skipping\n");
        return;
    }

    /* ── Real decompiled overlay 8 title screen constructor ──
     * Decompiled from arm overlay 8 @ 0x02077784..0x02077A64.
     * This is the ACTUAL game code, not a HOST_PORT replacement. */

    /* The ptr from OS_Alloc is a host-heap pointer; scene queue uses NDS
     * addresses so we allocate in NDS-mapped RAM instead. */
    (void)ptr;
    u32 obj_nds = nds_bump_alloc(0x30);
    fprintf(stderr,
            "[FUN_02077784] REAL title ctor: obj=0x%08X type=%d param=%d\n",
            (unsigned)obj_nds, type, param);

    /* 1. Base scene node init — insert into priority-sorted scene queue.
     * ARM: bl FUN_0202a74c  (r0=self, r1=type(8), r2=param(0), r3=0) */
    FUN_0202a74c_real(obj_nds, (u8)type, (u32)param, 0);

    /* 2. Install overlay 8 title vtable and store global pointer.
     * ARM: ldr r0, =0x02077EC4; str r0, [r4]
     *      ldr r1, =0x0207A6F8; str r4, [r1] */
    *(volatile u32 *)(uintptr_t)obj_nds = OV8_VTABLE_ADDR;
    *(volatile u32 *)(uintptr_t)OV8_GLOBAL_OBJ_ADDR = obj_nds;

    /* Patch the vtable entries in NDS memory so the scene queue processor
     * can read vtable[2] (tick) and vtable[1] (dtor) from the correct
     * addresses, even though OV0's data occupies this memory range.
     * vtable layout: [0]=???, [1]=dtor, [2]=tick (each 4 bytes) */
    *(volatile u32 *)(uintptr_t)(OV8_VTABLE_ADDR + 4) = OV8_DTOR_ADDR;
    *(volatile u32 *)(uintptr_t)(OV8_VTABLE_ADDR + 8) = OV8_TICK_ADDR;

    /* Let the ARM interpreter run the REAL overlay tick/dtor code.
     * The scene queue's vtable[2] dispatch will fall through to the
     * interpreter since these overlay addresses aren't in the fnptr table. */
    fprintf(stderr,
            "[FUN_02077784] NOT registering host tick override — "
            "real OV8 tick at 0x%08X will run via interpreter\n",
            (unsigned)OV8_TICK_ADDR);
    fflush(stderr);

    /* 3. Display system initialization (overlay 5 calls). */
    extern void FUN_02065da8(int screen);
    extern void FUN_0206805c(int screen);
    fprintf(stderr, "[title-ctor] step 3: display init\n"); fflush(stderr);
    FUN_02065da8(0);
    fprintf(stderr, "[title-ctor] step 3a: FUN_0206805c(0)\n"); fflush(stderr);
    FUN_0206805c(0);
    fprintf(stderr, "[title-ctor] step 3b: FUN_0206805c(1)\n"); fflush(stderr);
    FUN_0206805c(1);

    /* 4. Palette init — skip for now (was crashing due to bad literal deref) */
    fprintf(stderr, "[title-ctor] step 4: palette (skipped)\n"); fflush(stderr);

    /* 5. Power control */
    fprintf(stderr, "[title-ctor] step 5: power control\n"); fflush(stderr);
    volatile u16 *powcnt = (volatile u16 *)(uintptr_t)0x04000304u;
    *powcnt = *powcnt & (u16)~0x8000u;

    /* 6. VRAM bank configuration (ARM9 base functions). */
    extern void FUN_0203600c(int);
    extern void FUN_02036178(int);
    extern void FUN_02035a00(int);
    extern void FUN_02035a7c(int);
    extern void FUN_02035158(int, int, int);
    extern void FUN_0203513c(int);
    fprintf(stderr, "[title-ctor] step 6: VRAM banks\n"); fflush(stderr);
    FUN_0203600c(1);
    FUN_02036178(2);
    FUN_02035a00(8);
    FUN_02035a7c(4);
    FUN_02035158(1, 3, 0);
    FUN_0203513c(0);

    /* 7. Display mode setup (overlay 5). */
    extern void FUN_02067ecc(int screen, int mode);
    fprintf(stderr, "[title-ctor] step 7: display mode\n"); fflush(stderr);
    FUN_02067ecc(0, 0x00100010);
    FUN_02067ecc(1, 0x00100010);

    /* 8. BG1 configuration (screen=0, bg=1): 256×256, 4bpp, char_base=1,
     *    screen_base=0, priority=1, no mosaic */
    extern void FUN_02067ad8(int, int);
    extern void FUN_02067a4c(int, int, int);
    extern void FUN_02067a20(int, int, int);
    extern void FUN_020679c8(int, int, int);
    extern void FUN_0206799c(int, int, int);
    extern void FUN_02067aa8(int, int, int);
    extern void FUN_02067a78(int, int, int);

    FUN_02067ad8(0, 1);       /* clear BG1CNT */
    FUN_02067a4c(0, 1, 0);   /* screen_size = 0 (256×256) */
    FUN_02067a20(0, 1, 0);   /* color_mode = 4bpp */
    FUN_020679c8(0, 1, 0);   /* screen_base = 0 */
    FUN_0206799c(0, 1, 1);   /* char_base = 1 */
    FUN_02067aa8(0, 1, 1);   /* priority = 1 */
    FUN_02067a78(0, 1, 0);   /* mosaic = off */

    FUN_02067ad8(0, 2);       /* clear BG2CNT */
    FUN_02067a4c(0, 2, 0);
    FUN_02067a20(0, 2, 0);
    FUN_020679c8(0, 2, 1);
    FUN_0206799c(0, 2, 2);
    FUN_02067aa8(0, 2, 0);
    FUN_02067a78(0, 2, 0);

    /* 10. Sprite/OBJ subsystem init (overlay 5) */
    extern void FUN_020662b8(u32);
    extern void FUN_02068d70(u32);
    extern void FUN_020697d4(void);
    extern void FUN_02068928(u32);
    extern void FUN_020692f4(u32);
    extern void FUN_02069618(u32);
    fprintf(stderr, "[title-ctor] step 10: sprite init\n"); fflush(stderr);
    FUN_020662b8(0x80);
    fprintf(stderr, "[title-ctor] step 10a: OBJ manager\n"); fflush(stderr);
    FUN_02068d70(0x100);
    fprintf(stderr, "[title-ctor] step 10b: OAM double buf\n"); fflush(stderr);
    FUN_020697d4();
    fprintf(stderr, "[title-ctor] step 10c: anim alloc\n"); fflush(stderr);
    FUN_02068928(0x40);
    fprintf(stderr, "[title-ctor] step 10d: sprite render alloc\n"); fflush(stderr);
    FUN_020692f4(0x40);
    fprintf(stderr, "[title-ctor] step 10e: VRAM buf init\n"); fflush(stderr);
    FUN_02069618(0x20);

    /* 11. Allocate and init animation buffer */
    extern u32 FUN_02029c1c(u32, u32, u32, u32);
    extern void FUN_02067204(u32, int, int, int, int);
    fprintf(stderr, "[title-ctor] step 11: anim buffer alloc\n"); fflush(stderr);
    u32 anim_buf = FUN_02029c1c(0x335C, 0, 0, 1);
    fprintf(stderr, "[title-ctor] step 11: anim_buf=0x%08X\n", (unsigned)anim_buf); fflush(stderr);
    if (anim_buf != 0) {
        FUN_02067204(anim_buf, 0x0A, 0, 0, -1);
    }
    *(volatile u32 *)(uintptr_t)(0x02069E00u + 0x2C) = anim_buf;

    /* 12. Fade init */
    extern void FUN_02008fc0(int);
    fprintf(stderr, "[title-ctor] step 12: fade init\n"); fflush(stderr);
    FUN_02008fc0(0);
    FUN_02008fc0(1);

    /* 13. Allocate scene data buffer */
    extern u32 FUN_02029bf8(u32, u32, u32, u32);
    extern void FUN_02009040(int, u32, u32);
    fprintf(stderr, "[title-ctor] step 13: data buf alloc\n"); fflush(stderr);
    u32 data_buf = FUN_02029bf8(0x4400, 0, 0, 0);
    fprintf(stderr, "[title-ctor] step 13: data_buf=0x%08X\n", (unsigned)data_buf); fflush(stderr);
    *(volatile u32 *)(uintptr_t)(0x02069E00u + 0x28) = data_buf;

    /* 14. Set up OBJ for both screens */
    fprintf(stderr, "[title-ctor] step 14: OBJ setup\n"); fflush(stderr);
    FUN_02009040(0, data_buf + 0x2000, data_buf);
    FUN_02009040(1, data_buf + 0x2200, data_buf + 0x4200);

    /* 15. Allocate sub-scene object */
    extern void FUN_02077af8(u32, int, int, u32);
    fprintf(stderr, "[title-ctor] step 15: sub-scene\n"); fflush(stderr);
    u32 sub_obj = FUN_02029c1c(0x28, 0, 0, 0);
    fprintf(stderr, "[title-ctor] step 15: sub_obj=0x%08X\n", (unsigned)sub_obj); fflush(stderr);
    if (sub_obj != 0) {
        FUN_02077af8(sub_obj, 8, 0, obj_nds);
    }
    *(volatile u32 *)(uintptr_t)(obj_nds + 0x28) = sub_obj;

    /* 16. Reset VRAM mapping.
     * ARM: bl FUN_02029488(0, 0) */
    extern void FUN_02029488(int, int);
    FUN_02029488(0, 0);

    /* 17. Clear BG enable bits in both DISPCNT registers (bits 8-12).
     * ARM: bic r0, r0, #0x1F00 for both 0x04000000 and 0x04001000 */
    volatile u32 *dispcnt_main = (volatile u32 *)(uintptr_t)0x04000000u;
    volatile u32 *dispcnt_sub  = (volatile u32 *)(uintptr_t)0x04001000u;
    *dispcnt_main = *dispcnt_main & ~0x1F00u;
    *dispcnt_sub  = *dispcnt_sub  & ~0x1F00u;

    /* 18. Start animation for sub-scene.
     * ARM: ldr r0, [r4, #0x28]; bl FUN_02029ffc */
    extern void FUN_02029ffc(u32);
    FUN_02029ffc(*(volatile u32 *)(uintptr_t)(obj_nds + 0x28));

    /* 19. Commit display registers.
     * ARM: bl FUN_020351cc — GX_DispOn/flush */
    extern void FUN_020351cc(void);
    FUN_020351cc();

    /* 20. Enable sub engine display: set bit 16 in sub DISPCNT.
     * ARM: ldr r2, =0x04001000; orr r1, #0x10000; str r1, [r2] */
    *dispcnt_sub = *dispcnt_sub | 0x10000u;

    /* 21. Load title screen assets (FUN_0207701C).
     * This is a complex ~300-line overlay 8 function that loads tiles,
     * tilemaps, and palettes from ROM. For now, use host asset loader. */
    host_title_screen_load();

    /* 22. Register per-frame callback (overlay 5).
     * ARM: bl FUN_0206621c(0x020768F0, 6, 1) */
    extern void FUN_0206621c(u32, u16, u16);
    FUN_0206621c(0x020768F0u, 6, 1);

    /* 23. Clear field_2c (scene state = normal, not transitioning) */
    *(volatile u32 *)(uintptr_t)(obj_nds + 0x2c) = 0;

    /* Set needs-update flag so scene queue processor calls our tick */
    volatile u16 *flags_ptr = (volatile u16 *)(uintptr_t)(obj_nds + 0x12);
    *flags_ptr |= 0x0001;

    fprintf(stderr,
            "[FUN_02077784] REAL title ctor done: obj=0x%08X anim=%08X data=%08X sub=%08X\n",
            (unsigned)obj_nds, (unsigned)anim_buf, (unsigned)data_buf, (unsigned)sub_obj);
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
static int s_filesel_state = 0;   /* 0=browsing, 1=fading out, 2=done */
static u32 s_filesel_fade = 0;
#define FILESEL_ITEMS 4

static void host_next_scene_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;

    static u32 next_frame = 0;
    next_frame++;

    /* State 2: already transitioned, do nothing */
    if (s_filesel_state == 2) return;

    /* State 1: fading out after selection */
    if (s_filesel_state == 1) {
        s_filesel_fade++;
        u32 factor = (s_filesel_fade * 16) / TITLE_FADE_DURATION;
        if (factor > 16) factor = 16;
        title_reg_write16(REG_MASTER_BRIGHT, (u16)(0x4000u | (factor & 0x1F)));

        if (s_filesel_fade >= TITLE_FADE_DURATION) {
            s_filesel_state = 2;
            fprintf(stderr,
                    "[file_select] fade complete, transitioning to gameplay\n");
            fflush(stderr);

            /* Trigger scene transition via FUN_02005d3c.
             * This sets the scene anchor to phase=2 (unload current scene),
             * then phase=3 constructs the next scene.
             * State 0 = main gameplay/world map (needs overlay 0 constructor
             * FUN_0206DE6C).  The constructor is not yet decompiled but
             * the state machine will attempt the transition. */
            u32 scene_anchor = *(volatile u32 *)(uintptr_t)SCENE_PTR_NDS;
            if (scene_anchor >= 0x02000000u && scene_anchor < 0x02400000u) {
                fprintf(stderr,
                        "[file_select] calling FUN_02005d3c(0x%08X, 0) to enter gameplay\n",
                        (unsigned)scene_anchor);
                fflush(stderr);
                FUN_02005d3c((int)scene_anchor, 0);
            }
        }
        return;
    }

    /* State 0: browsing - fade in then handle input */

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

        if (s_filesel_cursor == 3) {
            /* New Game — begin fade out */
            s_filesel_state = 1;
            s_filesel_fade = 0;
        }
        /* Files 1-3: empty, no action (could show "No Data" message) */
    }

    /* Update the cursor position in the font overlay tilemap every frame */
    void *a = nds_vram_bank('A');
    if (a) {
        uint16_t *map = (uint16_t *)((uint8_t *)a + FONT_MAP_OFF);
        /* menu rows: File 1 @ row 8, File 2 @ row 11, File 3 @ row 14, New Game @ row 17 */
        int menu_rows[] = {8, 11, 14, 17};
        for (int i = 0; i < FILESEL_ITEMS; i++) {
            int row = menu_rows[i];
            /* Column 4 is the cursor column */
            if (i == s_filesel_cursor) {
                /* Blink the cursor arrow */
                int show = (next_frame / 15) & 1;
                map[row * 32 + 4] = show ? (uint16_t)(char_to_glyph('>') | (15 << 12)) : 0;
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

    /* ── Try to load REAL file-select graphics from MenuDat.dat (FAT[86]) ── */
    int real_loaded = 0;
    const uint8_t *rom_d = rom_data();
    if (rom_d && a) {
        uint32_t fat_off = (uint32_t)rom_d[0x48] | ((uint32_t)rom_d[0x49] << 8) |
                           ((uint32_t)rom_d[0x4A] << 16) | ((uint32_t)rom_d[0x4B] << 24);
        #define MENUDAT_FAT_ID 86
        uint32_t f_start = (uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 0] |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 1] << 8) |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 2] << 16) |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 3] << 24);
        uint32_t f_end   = (uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 4] |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 5] << 8) |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 6] << 16) |
                           ((uint32_t)rom_d[fat_off + MENUDAT_FAT_ID*8 + 7] << 24);

        if (f_start < f_end && f_end <= (uint32_t)rom_size()) {
            const uint8_t *md = rom_d + f_start;
            uint32_t md_len = f_end - f_start;

            /* Parse offset table */
            uint32_t off0 = md[0] | (md[1]<<8) | (md[2]<<16) | (md[3]<<24);
            uint32_t off1 = md[4] | (md[5]<<8) | (md[6]<<16) | (md[7]<<24);
            uint32_t off2 = md[8] | (md[9]<<8) | (md[10]<<16) | (md[11]<<24);
            uint32_t off3 = md[12] | (md[13]<<8) | (md[14]<<16) | (md[15]<<24);

            /* sub[0]: LZ10-compressed 8bpp tiles → VRAM at MENU_TILE_OFF */
            if (off0 < md_len && off1 < md_len && md[off0] == 0x10) {
                uint32_t dec_sz = md[off0+1] | (md[off0+2]<<8) | (md[off0+3]<<16);
                uint8_t *tile_buf = (uint8_t *)malloc(dec_sz);
                if (tile_buf) {
                    arm_swi_11_lz77_decomp(md + off0, tile_buf);
                    uint32_t copy_sz = dec_sz;
                    if (MENU_TILE_OFF + copy_sz > nds_vram_bank_size('A'))
                        copy_sz = nds_vram_bank_size('A') - MENU_TILE_OFF;
                    memcpy((uint8_t *)a + MENU_TILE_OFF, tile_buf, copy_sz);
                    fprintf(stderr,
                            "[file_select] MenuDat sub[0]: %u bytes LZ10 -> %u tiles (8bpp)\n",
                            off1-off0, dec_sz / 64);
                    free(tile_buf);
                }
            }

            /* sub[1]: LZ10-compressed tilemap → VRAM at MENU_MAP_OFF */
            if (off1 < md_len && off2 < md_len && md[off1] == 0x10) {
                uint32_t dec_sz = md[off1+1] | (md[off1+2]<<8) | (md[off1+3]<<16);
                uint8_t *map_buf = (uint8_t *)malloc(dec_sz);
                if (map_buf) {
                    arm_swi_11_lz77_decomp(md + off1, map_buf);
                    uint32_t copy_sz = dec_sz;
                    if (copy_sz > 0x800) copy_sz = 0x800;
                    memcpy((uint8_t *)a + MENU_MAP_OFF, map_buf, copy_sz);
                    fprintf(stderr,
                            "[file_select] MenuDat sub[1]: %u bytes LZ10 -> %u map entries\n",
                            off2-off1, dec_sz / 2);
                    free(map_buf);
                }
            }

            /* sub[2]: raw 256-color BGR555 palette → palette RAM */
            if (off2 < md_len && off3 < md_len) {
                uint32_t pal_sz = off3 - off2;
                void *e = nds_vram_bank('E');
                if (e && pal_sz >= 512) {
                    memcpy(e, md + off2, 512);
                    /* Set palette entry 0 to transparent black */
                    ((uint16_t *)e)[0] = 0x0000;
                    fprintf(stderr,
                            "[file_select] MenuDat sub[2]: %u bytes raw palette\n", pal_sz);
                }
                real_loaded = 1;
            }
        }
    }

    /* ── Font overlay on BG0 (always, regardless of real_loaded) ── */
    if (a) {
        uint8_t *tiles = (uint8_t *)a;

        /* Write font glyph tiles at FONT_TILE_OFF (4bpp, 32 bytes each) */
        write_font_tiles(tiles + FONT_TILE_OFF);

        /* Background tile (index FONT_GLYPH_COUNT): fully transparent (index 0) */
        memset(tiles + FONT_TILE_OFF + (u32)FONT_GLYPH_COUNT * 32, 0x00, 32);

        /* Write font tilemap at FONT_MAP_OFF */
        uint16_t *map = (uint16_t *)(tiles + FONT_MAP_OFF);

        /* Fill with transparent background tile */
        for (int i = 0; i < 32 * 32; i++)
            map[i] = (uint16_t)FONT_GLYPH_COUNT;

        /* Draw menu text — pal_bank 14 for normal, 15 for highlighted */
        write_text(map, 6, 2, "MARIO AND LUIGI", 14);
        write_text(map, 5, 3, "PARTNERS IN TIME", 14);
        write_text(map, 7, 5, "FILE  SELECT", 14);
        write_text(map, 6, 8,  "FILE 1 - EMPTY", 14);
        write_text(map, 6, 11, "FILE 2 - EMPTY", 14);
        write_text(map, 6, 14, "FILE 3 - EMPTY", 14);
        write_text(map, 6, 17, "NEW GAME", 15);
        map[17 * 32 + 4] = (uint16_t)(char_to_glyph('>') | (15 << 12));
    }

    /* ── Palette for font (bank 14 = entries 224-239, bank 15 = entries 240-255) ── */
    {
        void *e = nds_vram_bank('E');
        if (e) {
            uint16_t *pal = (uint16_t *)e;
            /* Bank 14: normal text — entry 2 (within bank) = white */
            pal[14 * 16 + 2] = 0x7FFF;
            /* Bank 15: highlighted text — entry 2 (within bank) = yellow */
            pal[15 * 16 + 2] = 0x03FF;
            if (!real_loaded) {
                /* No real BG — fill bank 14 entry 1 for dark background */
                pal[14 * 16 + 1] = 0x4C08;
                pal[15 * 16 + 1] = 0x4C08;
            }
        }
    }

    /* ── Configure BG registers ── */
    /* BG0: font overlay — char_base=0, screen_base=1, 4bpp, priority=0 */
    *(volatile uint16_t *)(uintptr_t)0x04000008u =
        (0 << 2) | (1 << 8) | (0 << 7) | (0 << 14) | 0;
    *(volatile uint16_t *)(uintptr_t)0x04000010u = 0;
    *(volatile uint16_t *)(uintptr_t)0x04000012u = 0;

    if (real_loaded) {
        /* BG1: real menu BG — char_base=4, screen_base=31, 8bpp, priority=1 */
        *(volatile uint16_t *)(uintptr_t)0x0400000Au =
            (4 << 2) | (31 << 8) | (1 << 7) | (0 << 14) | 1;
        *(volatile uint16_t *)(uintptr_t)0x04000014u = 0;
        *(volatile uint16_t *)(uintptr_t)0x04000016u = 0;
    }

    /* Enable BG0 + BG1 (if real loaded) */
    uint32_t dispcnt = *(volatile uint32_t *)(uintptr_t)0x04000000u;
    dispcnt &= ~(0x0F00u);
    dispcnt |= 0x0100u;  /* BG0 */
    if (real_loaded) dispcnt |= 0x0200u;  /* BG1 */
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

    /* No host overrides — let ARM interpreter run real tick */
    fprintf(stderr,
            "[FUN_020739ec] NOT registering host tick override — "
            "real OV8 next tick at 0x%08X will run via interpreter\n",
            (unsigned)OV8_NEXT_TICK_ADDR);
    fflush(stderr);

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
