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

static void host_title_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;

    u32 field_2c = *(volatile u32 *)(uintptr_t)(node_addr + 0x2c);

    /* Already torn down — do nothing (node should have been removed from
     * queue by the destructor, but our host_title_dtor is a no-op). */
    if (field_2c >= 2) return;

    if (field_2c == 1) {
        /* Teardown path — mirroring original ov8 FUN_02077444 teardown:
         * 1. Call destructor (cleanup)
         * 2. Stop streaming
         * 3. Reset memory pools
         * 4. Transition scene anchor to phase 2 (next scene)
         *
         * IMPORTANT: Set field_2c=2 FIRST to prevent re-triggering on
         * subsequent frames (the destructor would normally remove us from
         * the queue, but our host_title_dtor is a no-op). */
        *(volatile u32 *)(uintptr_t)(node_addr + 0x2c) = 2;

        static int teardown_logged = 0;
        if (!teardown_logged) {
            teardown_logged = 1;
            fprintf(stderr,
                    "[title_tick] teardown: transitioning to next scene\n");
            fflush(stderr);
        }

        /* Get scene anchor from the global pointer */
        u32 scene_anchor = *(volatile u32 *)(uintptr_t)SCENE_PTR_NDS;
        if (scene_anchor >= 0x02000000u && scene_anchor < 0x02400000u) {
            /* Set scene anchor to phase 2 with state 2 (next scene).
             * FUN_02005d3c: anchor+0x28 = new_state, anchor+0x10 = 2,
             * then tail-calls FUN_0202a56c. */
            FUN_02005d3c((int)scene_anchor, 2);
        }
        return;
    }

    /* Normal frame: check for Start or A button press.
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
                    "[title_tick] Start/A pressed! Setting field_2c=1 "
                    "for teardown next frame\n");
            fflush(stderr);
        }
        *(volatile u32 *)(uintptr_t)(node_addr + 0x2c) = 1;
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

/* NDS addresses for the next scene (distinct from title screen) */
#define OV8_NEXT_TICK_ADDR    0x020739A0u  /* synthetic tick for next scene */
#define OV8_NEXT_DTOR_ADDR    0x020739A4u  /* synthetic dtor for next scene */

static void host_next_scene_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    (void)node_addr;
    /* Idle tick — the next scene just exists in the queue.
     * Future: render file-select screen, handle input. */
    static int logged = 0;
    if (!logged) {
        logged = 1;
        fprintf(stderr,
                "[next_scene] first tick at node=0x%08X — "
                "file select placeholder active\n",
                (unsigned)(u32)node_addr);
        fflush(stderr);
    }
}

static void host_next_scene_dtor(uintptr_t node_addr, uintptr_t unused)
{
    (void)unused;
    fprintf(stderr, "[next_scene_dtor] node=0x%08X destroyed\n",
            (unsigned)(u32)node_addr);
}

void FUN_020739ec(void *ptr, int type, int param)
{
    (void)ptr;
    (void)type;
    (void)param;

    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr, "[FUN_020739ec] ARM9 RAM not mapped, skipping\n");
        return;
    }

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
