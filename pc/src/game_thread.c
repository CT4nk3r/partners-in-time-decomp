/*
 * game_thread.c - Entry point for the game thread on the PC port
 *
 * Runs the decompiled game_init() inside a SIGSEGV-protected region so we
 * can observe how far real game initialization gets before the next stub
 * fault. After init returns (or faults), we fall back to a vblank
 * heartbeat so the SDL window stays responsive.
 */
#include "nds_platform.h"
#include "arm_compat.h"
#include "nds_boot_hook.h"

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

extern void game_init(void);
extern void game_start(void);
extern void game_state_host_init(void);

static jmp_buf g_crash_jmp;
static volatile sig_atomic_t g_in_protected = 0;

static void crash_handler(int sig) {
    if (g_in_protected) {
        longjmp(g_crash_jmp, sig);
    }
    /* Outside protected region — let it crash normally. */
    signal(sig, SIG_DFL);
    raise(sig);
}

int game_thread_main(void* user) {
    (void)user;
    nds_log("[game] thread started\n");

    signal(SIGSEGV, crash_handler);
    signal(SIGABRT, crash_handler);
    signal(SIGFPE,  crash_handler);
    signal(SIGILL,  crash_handler);

    nds_log("[game] initializing host game state...\n");
    game_state_host_init();
    nds_log("[game] host game state initialized\n");

    nds_log("[game] attempting game_start() (protected)...\n");
    g_in_protected = 1;
    int sig = setjmp(g_crash_jmp);
    if (sig == 0) {
        game_start();  /* NORETURN on NDS — runs forever */
        /* Unreachable on NDS, but in our host build the inner loop blocks
         * on platform_wait_vblank() so the thread effectively runs forever
         * until platform_game_should_exit() unblocks via stop_game_thread. */
        g_in_protected = 0;
        nds_log("[game] game_start() returned (unexpected)\n");
    } else {
        g_in_protected = 0;
        nds_log("[game] game_start() FAULTED (signal %d) - "
                "next stub needs implementing\n", sig);
        /* game_start() clobbered DISPCNT/visible-plane bits before crashing
         * (it called GX_SetVisiblePlane(0xFFFFFFFF, 0) early during init).
         * Re-run the boot hook so the user still sees something on screen. */
        if (!boot_hook_paired_screen()) {
            (void)boot_hook_real_tiles();
        }
        nds_log("[game] re-armed boot screen after game_start fault\n");
    }

    nds_log("[game] entering vblank heartbeat fallback\n");
    int frame = 0;
    while (!platform_game_should_exit()) {
        arm_swi_05_vblank_intr_wait();
        if ((frame % 120) == 0) {
            nds_log("[game] heartbeat frame=%d\n", frame);
        }
        frame++;
    }

    nds_log("[game] thread exiting after %d heartbeat frames\n", frame);
    return 0;
}

