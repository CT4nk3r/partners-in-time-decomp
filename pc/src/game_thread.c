/*
 * game_thread.c - Entry point for the game thread on the PC port
 *
 * The decompiled game_start() in arm9/src/game_main.c is an infinite NDS
 * main loop. We run it in a separate SDL thread so the SDL main thread can
 * keep the window responsive and drive frame pacing via the vblank semaphore.
 *
 * Currently this is a SAFE STUB: directly calling game_start() crashes
 * because:
 *   1. The static globals in game_main.c (sGameState, sDispCnt, etc.) are
 *      uninitialized pointers. They were meant to alias fixed NDS RAM
 *      addresses (DAT_020055B4 etc.), which our host_undefined_stubs.c
 *      provides as plain u32 storage - not the GameState structure layout
 *      the decompiled code dereferences.
 *   2. The SDK functions (GX_*, FS_*, OBJ_*, SND_*) are auto-generated
 *      no-op stubs that all return 0 instead of doing real work.
 *
 * To make game_start() actually run we need to incrementally:
 *   - Allocate real backing storage for the game-state pointers and pass
 *     them through a small init shim (see GAME_INIT_FOR_HOST below)
 *   - Replace the most critical SDK stubs with host implementations
 *     (GX_VBlankWait already works via arm_compat_host.c)
 *
 * For now, the thread runs a minimal heartbeat so we can verify the
 * threading + vblank-sync infrastructure works before tackling SDK porting.
 */
#include "nds_platform.h"
#include "arm_compat.h"

#include <stdio.h>

/* Forward decls for decompiled entry points. We do NOT call them yet. */
extern void game_init(void);
extern void game_start(void);  /* NORETURN on NDS, would crash on host */

int game_thread_main(void* user) {
    (void)user;
    nds_log("[game] thread started\n");

    int frame = 0;
    while (!platform_game_should_exit()) {
        /* This routes through arm_swi_05_vblank_intr_wait -> platform_wait_vblank
         * which blocks until the main SDL thread posts the vblank semaphore. */
        arm_swi_05_vblank_intr_wait();

        if ((frame % 60) == 0) {
            nds_log("[game] tick frame=%d (vblank sync OK)\n", frame);
        }
        frame++;
    }

    nds_log("[game] thread exiting after %d frames\n", frame);
    return 0;
}
