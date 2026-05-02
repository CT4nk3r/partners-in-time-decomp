/**
 * Game Main Loop and Initialization
 *
 * This module contains the core game loop and initialization functions.
 * Called from NitroMain (CRT0) after SDK initialization.
 *
 * Decompiled from:
 *   FUN_020050f4 @ 0x020050F4 (228 bytes) - game_init
 *   FUN_02005444 @ 0x02005444 (344 bytes) - game_start (main loop)
 *   FUN_02005380 @ 0x02005380 (176 bytes) - game_check_reset
 *   FUN_02004ef8 @ 0x02004EF8 (116 bytes) - game_setup_overlay
 *   FUN_0200528c @ 0x0200528C (100 bytes) - game_fade_out
 *   FUN_02005300 @ 0x02005300 (112 bytes) - game_check_display_ready
 */

#include "types.h"
#include "game_main.h"
#ifdef HOST_PORT
#include <stdio.h>
#endif

// External SDK functions (to be decompiled separately)
extern void OS_MemClear(void *ptr, u32 offset, u32 size);
extern void GX_SetDispSelect(void);
extern void GX_SetMasterBrightness(u32 screen, u16 val);
extern void GX_SwapDisplay(void);
extern void GX_VBlankWait(void);
extern void GX_SetVisiblePlane(u32 mask, u16 bits);
extern void GX_UpdateDisplay(void);
extern void GX_DisableInterrupts(void);
extern void GX_FlushDisplay(void);
extern void GX_ResetVisiblePlane(void);
extern void FS_InitOverlay(void);
extern u32  FS_LoadOverlay(u32 id, u32 flags, void *callback, u32 param);
extern void FS_AttachOverlay(u32 handle, u32 id);
extern void SND_Init(void);
extern void SND_Update(void);
extern void PAD_Read(void);
extern void OBJ_Init(void);
extern u32  OBJ_Create(void *param);
extern void OBJ_Update(u32 handle);

/* Inner-loop calls reconstructed from FUN_02005444 disassembly:
 *   +0xC4: BL 0x020072D4  (autosave-timer check, body in misc_funcs.c)
 *   +0xC8: BL 0x0202A33C  (linked-list scene-queue processor, body in
 *                          gx_util.c — but the C signature is not
 *                          x86_64-safe; use the FUN_0202a33c_safe wrapper
 *                          from pc/src/host_scene_queue.c instead.)
 * Both BLs sit inside the inner do-while body that runs every frame. */
extern void FUN_020072d4(void);
extern void FUN_0202a33c_safe(void);

// Game state globals (addresses from Ghidra data references)
// These are defined in the BSS section and initialized at runtime
static GameState *sGameState;       // DAT_020055B4
static u8 *sDisplayEnabled;         // DAT_020055BC
static u8 *sPauseFlag;              // DAT_020055B8
static u8 *sResetFlag;              // DAT_020055B0
static u16 *sDispCnt;               // DAT_020055A0

/**
 * game_init — Initialize game subsystems
 * Original: FUN_020050f4 @ 0x020050F4
 *
 * Sets up the game state structure, initializes graphics, overlay system,
 * file system, object system, and sound.
 */
void game_init(void)
{
    u32 overlay_handle;
    u32 obj_handle;
    int date_lo, date_mid, date_hi;

    OS_MemClear(sGameState, 0, 0x14);
    sGameState->frame_rate = 10;
    sGameState->mode = 2;

    GX_SetDispSelect();
    GX_SetMasterBrightness(0, 0);
    GX_SetVisiblePlane(0xFFFFFFFF, 0);

    overlay_handle = FS_LoadOverlay(
        GX_GetCurrentMode(),
        1,
        NULL,  // callback
        0      // param
    );
    sGameState->overlay = overlay_handle;
    FS_AttachOverlay(overlay_handle, GX_GetCurrentMode());

    SND_Init();
    obj_handle = OBJ_Create(NULL);
    sGameState->obj_handle = obj_handle;

    if (obj_handle != 0) {
        OBJ_Update(obj_handle);
    }

    FS_InitOverlay();
    // Additional init...
}

/**
 * game_start — Main game loop
 * Original: FUN_02005444 @ 0x02005444
 *
 * This is the infinite main loop of the game. Called from NitroMain
 * after all initialization is complete. Never returns.
 *
 * Each frame:
 *   1. Wait for VBlank
 *   2. Read input
 *   3. Update file system
 *   4. Check display state
 *   5. Update game state
 *   6. Render frame
 *   7. Handle fade/transition if needed
 */
NORETURN void game_start(void)
{
    fprintf(stderr, "[game_start] step 1: game_init()\n"); fflush(stderr);
    game_init();

    fprintf(stderr, "[game_start] step 2: GX_SetMasterBrightness\n"); fflush(stderr);
    GX_SetMasterBrightness(0, 0x5800);
    fprintf(stderr, "[game_start] step 3: GX_SwapDisplay\n"); fflush(stderr);
    GX_SwapDisplay();
    fprintf(stderr, "[game_start] step 4: read sGameState=%p sDispCnt=%p\n",
            (void*)sGameState, (void*)sDispCnt); fflush(stderr);
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );
    fprintf(stderr, "[game_start] step 5: GX_UpdateDisplay\n"); fflush(stderr);
    GX_UpdateDisplay();
    fprintf(stderr, "[game_start] step 6: GX_SwapDisplay #2\n"); fflush(stderr);
    GX_SwapDisplay();
    fprintf(stderr, "[game_start] step 7: GX_SetVisiblePlane #2\n"); fflush(stderr);
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );

    fprintf(stderr, "[game_start] step 8: sGameState->running=1\n"); fflush(stderr);
    sGameState->running = 1;
    fprintf(stderr, "[game_start] step 9: game_setup_overlay\n"); fflush(stderr);
    game_setup_overlay(0xFFFFFFFF, 0);
    fprintf(stderr, "[game_start] step 10: entering outer loop\n"); fflush(stderr);

    // Main game loop — runs forever
    for (;;) {
        // Inner frame loop
        do {
            GX_VBlankWait();
            PAD_Read();
            FS_Update();

            if (*sDisplayEnabled != 0) {
                game_update_display();
            }

            /* Restored per-frame BLs from FUN_02005444 +0xC4 / +0xC8.
             * On a fresh boot DAT_020073BC ('autosave enabled' flag) is
             * zero so FUN_020072d4 returns immediately; the queue
             * processor is similarly idle until a node is appended.
             * Logged once-per-process so verification is trivial. */
            {
                static int s_logged_missing_calls = 0;
                if (!s_logged_missing_calls) {
                    s_logged_missing_calls = 1;
                    extern void mlpit_log_missing_calls_wired(void);
                    mlpit_log_missing_calls_wired();
                }
            }
            FUN_020072d4();
            FUN_0202a33c_safe();

            // Update frame counter if game state is active
            if (sGameState->current != NULL) {
                GameState *gs = sGameState->current;
                if ((gs->flags & 0x40) != 0) {
                    gs->frame_count++;
                }
            }
        } while ((*sPauseFlag != 0) ||
                 ((s32)(*sDispCnt & 0x8000) >> 15 != 1));

        // Frame transition
        GX_SetMasterBrightness(0xFFFFFFFF, 0);

        if (*sResetFlag != 0) {
            SND_Update();
        }

        // Select transition effect duration
        u32 duration = 0x0C;
        if (*sResetFlag != 0) {
            duration = 0x1C;
        }

        game_do_transition(duration, 0, 0);
    }
}

/**
 * game_check_reset — Check if reset/transition should occur
 * Original: FUN_02005380 @ 0x02005380
 */
void game_check_reset(void)
{
    if (*sDisplayEnabled == 0) {
        return;
    }

    if (sGameState->needs_reset != 0) {
        SND_Update();
    }

    if (sGameState->in_transition == 0) {
        if (sGameState->pending_reset == 0) {
            int ready = game_check_display_ready();
            if (ready != 0) {
                if (sGameState->block_reset == 0) {
                    game_fade_out(0xFFFFFFFF);
                } else {
                    sGameState->pending_reset = 1;
                }
            }
        } else if (sGameState->block_reset == 0) {
            game_fade_out(0xFFFFFFFF);
        }
    }

    GX_UpdateDisplay();
}

/**
 * game_fade_out — Begin a screen fade out
 * Original: FUN_0200528c @ 0x0200528C
 *
 * @param brightness  Target brightness level (0-16, or 0xFFFFFFFF for current)
 */
void game_fade_out(u32 brightness)
{
    if (brightness == 0xFFFFFFFF) {
        brightness = (u32)*sGameState->cur_brightness;
    }

    sGameState->fading = 1;

    if ((s32)brightness < 0 || (s32)brightness > 16) {
        brightness = 16;
    }

    sGameState->fade_target = (u8)brightness;
    sGameState->fade_current = sGameState->fade_target;

    GX_DisableInterrupts();
    GX_FlushDisplay();
    GX_ResetVisiblePlane();
}

/**
 * game_check_display_ready — Check if display is ready for transition
 * Original: FUN_02005300 @ 0x02005300
 *
 * @return 1 if display is ready, 0 otherwise
 */
u32 game_check_display_ready(void)
{
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );

    if (*sGameState->disp_status == 0x30C &&
        (sGameState->disp_status[1] & 0x30C) != 0) {
        return 1;
    }

    return 0;
}

#ifdef HOST_PORT
/**
 * game_state_host_init — PC port helper.
 *
 * On real NDS hardware the file-static pointers above were initialized by
 * CRT0 / data section relocation to point at fixed RAM addresses
 * (DAT_020055A0..DAT_020055BC).  In our host build those addresses do not
 * exist, so we allocate real backing storage and patch the pointers.
 *
 * Must be called once before game_init() / game_start() on the PC port.
 *
 * NOT part of the original game — exists only to satisfy host linkage.
 *
 * --- stateinit-organic session findings ---
 * sGameState->current (== *DAT_020055B4 in the original code) is populated
 * by FUN_02005b70 ("clGameMain init", arm9/src/game_init.c:256), which
 * stores an OS_Alloc'd buffer into *DAT_02005d28 (the same RAM slot at
 * 0x020055B4).  FUN_02005b70 is only reached via FUN_02006304 ("scene
 * object ctor"), and FUN_02006304 has no callers in the present code —
 * the original call edge lives in a C++ vtable / scene factory we have
 * not yet wired up.  Calling FUN_02005b70 directly would NPE because its
 * file-static globals (DAT_02005d28..d38: alloc-size, default config blob,
 * display-mode flag) are zero-initialised here instead of pointing at
 * .data literals like on real hardware.
 *
 * Until the scene-factory edge is decompiled, we provide a small host
 * trampoline (game_state_host_engage) that allocates a sub-state buffer
 * directly and stores it as sGameState->current — enough for the
 * outer-loop frame_count branch in game_start to take, and (more
 * importantly) for any code that gates on `*DAT_020055B4 != NULL` to wake.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static GameState  s_host_game_state;
static u8         s_host_display_enabled = 1;
static u8         s_host_pause_flag      = 0;
static u8         s_host_reset_flag      = 0;
/* Bit 15 must be set for the game_start() inner do-while to exit each frame.
 * The loop condition is: (*sDispCnt & 0x8000) >> 15 != 1
 * When bit 15 is set: (0x8000) >> 15 = 1 → condition is false → loop exits.
 * Without this the game thread spins forever on GX_VBlankWait without advancing. */
static u16        s_host_disp_cnt        = 0x8000;
static u16        s_host_disp_status[2]  = { 0x30C, 0x30C };
static u8         s_host_cur_brightness  = 0;

/* Backing buffer for the "sub-state" pointer.  The original FUN_02005b70
 * allocates ~0x600 bytes (sizeof clGameMain, DAT_02005d2c) and writes the
 * pointer to DAT_02005d28 / 0x020055B4.  We reproduce just the storage. */
static unsigned char s_host_substate[0x600];

void game_state_host_init(void)
{
    memset(&s_host_game_state, 0, sizeof(s_host_game_state));
    s_host_game_state.disp_status    = s_host_disp_status;
    s_host_game_state.cur_brightness = &s_host_cur_brightness;

    sGameState        = &s_host_game_state;
    sDisplayEnabled   = &s_host_display_enabled;
    sPauseFlag        = &s_host_pause_flag;
    sResetFlag        = &s_host_reset_flag;
    sDispCnt          = &s_host_disp_cnt;
}

/* Watcher: called from the main loop once per N frames; logs sGameState->current
 * the first time it changes from NULL (or whenever it moves). */
void game_state_watch_tick(int frame)
{
    static GameState *s_last = (GameState *)(uintptr_t)1; /* sentinel != NULL/anything */
    if (s_host_game_state.current != s_last) {
        fprintf(stderr,
                "[STATE] frame=%d sGameState->current = %p (was %p)\n",
                frame,
                (void *)s_host_game_state.current,
                (void *)s_last);
        s_last = s_host_game_state.current;
    }
}

/* Engage: install a fake sub-state so the outer loop's NULL-guarded branch
 * (and any subsystem gated on `*DAT_020055B4 != NULL`) actually fires.
 * Sets the high bit of byte+0x514 (== bit 6 of `flags`) which the original
 * FUN_02005444 tests as `((u8)b << 0x1a) < 0` → bit 0x40 — so frame_count
 * begins to advance.
 *
 * Idempotent.  Returns the substate pointer. */
void *game_state_host_engage(void)
{
    if (s_host_game_state.current != NULL) {
        return s_host_game_state.current;
    }
    GameState *gs = (GameState *)s_host_substate;
    /* The original struct layout tracks +0x514 / +0x518 (flags / frame_count)
     * inside the alloc'd block, NOT inside the host GameState wrapper.  We
     * reach those fields through the GameState typedef's `flags`/`frame_count`
     * members which the C-port author already aligned to the same offsets. */
    gs->flags       = 0x40;     /* enable frame_count increment branch */
    gs->frame_count = 0;
    s_host_game_state.current = gs;
    fprintf(stderr,
            "[STATE] host_engage: sGameState->current = %p (substate=%zu bytes)\n",
            (void *)gs, sizeof(s_host_substate));
    return gs;
}

/* Expose the address of `s_host_game_state.current` so pc/src/host_data_init.c
 * can install it as DAT_02005d28 (the slot FUN_02005b70 writes into). */
u32 *game_state_host_get_current_slot(void)
{
    return (u32 *)&s_host_game_state.current;
}

/* Try the legitimate path: call FUN_02005b70(NULL).  Requires
 * host_data_init_install() to have run first (otherwise DAT_02005d28
 * is NULL and the very first `*DAT_02005d28 == 0` check would crash).
 *
 * Returns the substate pointer that was written to the slot, or NULL
 * if the call did not produce one. */
extern void FUN_02005b70(u16 *param_1);
void *game_state_host_engage_real(void)
{
    fprintf(stderr, "[STATE] host_engage_real: calling FUN_02005b70(NULL)\n");
    FUN_02005b70((u16 *)0);
    fprintf(stderr,
            "[STATE] host_engage_real: sGameState->current = %p\n",
            (void *)s_host_game_state.current);
    return s_host_game_state.current;
}
#endif /* HOST_PORT */
