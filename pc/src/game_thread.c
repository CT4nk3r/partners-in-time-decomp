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
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

/* Task 1+3 (shadow-buffers-dma): the decompiled game stages display
 * updates into u16 arrays at NDS RAM 0x0201977c..0x02019790 before
 * DMAing them to real VRAM/OAM/PAL during VBlank.  Our VirtualAlloc'd
 * 4 MiB ARM9 RAM at 0x02000000 covers this range, so a write through a
 * raw pointer literal lands in host memory.  We snapshot the region at
 * startup and again periodically — if the bytes change, the staging
 * path is alive and we can follow it.
 */
#define SHADOW_PROBE_LO   0x02019700u   /* 124 bytes before 0x0201977c */
#define SHADOW_PROBE_HI   0x02019820u   /* 144 bytes after 0x02019790 */
#define SHADOW_PROBE_LEN  (SHADOW_PROBE_HI - SHADOW_PROBE_LO)

static uint8_t  g_shadow_baseline[SHADOW_PROBE_LEN];
static int      g_shadow_baseline_taken = 0;

static void shadow_buffer_snapshot(const char *tag) {
    if (!nds_arm9_ram_is_mapped()) {
        nds_log("[shadow] %s: arm9 RAM not mapped — skipping snapshot\n", tag);
        return;
    }
    const uint8_t *p = (const uint8_t *)(uintptr_t)SHADOW_PROBE_LO;
    uint32_t w0 = 0, w1 = 0, w2 = 0, w3 = 0;
    memcpy(&w0, p + (0x0201977cu - SHADOW_PROBE_LO), 4);
    memcpy(&w1, p + (0x02019780u - SHADOW_PROBE_LO), 4);
    memcpy(&w2, p + (0x02019784u - SHADOW_PROBE_LO), 4);
    memcpy(&w3, p + (0x0201978cu - SHADOW_PROBE_LO), 4);
    uint32_t hash = 0;
    for (uint32_t i = 0; i < SHADOW_PROBE_LEN; i++) {
        hash = hash * 131u + p[i];
    }
    int diff_bytes = 0;
    if (g_shadow_baseline_taken) {
        for (uint32_t i = 0; i < SHADOW_PROBE_LEN; i++)
            if (p[i] != g_shadow_baseline[i]) diff_bytes++;
    }
    nds_log("[shadow] %s: 0x%08X..0x%08X w[77c]=%08X w[780]=%08X "
            "w[784]=%08X w[78c]=%08X hash=%08X diff_bytes_vs_baseline=%d\n",
            tag, SHADOW_PROBE_LO, SHADOW_PROBE_HI,
            w0, w1, w2, w3, hash, diff_bytes);
    if (!g_shadow_baseline_taken) {
        memcpy(g_shadow_baseline, p, SHADOW_PROBE_LEN);
        g_shadow_baseline_taken = 1;
    }
}

extern void game_init(void);
extern void game_start(void);
extern void game_state_host_init(void);
extern void *game_state_host_engage(void); /* HOST_PORT trampoline (fake) */
extern void *game_state_host_engage_real(void); /* calls FUN_02005b70(NULL) */
extern void host_data_init_install(void); /* installs DAT_02005d28..d38 */
extern void host_display_data_init_install(void); /* installs DAT_02019730..d */

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
    setbuf(stderr, NULL);  /* unbuffered breadcrumbs survive SIGSEGV */
    nds_log("[game] thread started\n");

    signal(SIGSEGV, crash_handler);
    signal(SIGABRT, crash_handler);
    signal(SIGFPE,  crash_handler);
    signal(SIGILL,  crash_handler);

    nds_log("[game] initializing host game state...\n");
    game_state_host_init();

    /* HOST_PORT: map ARM9 main RAM at 0x02000000 from arm9.bin so that
     * any decomp path reading via raw NDS pointer literal sees real
     * .data values.  Must run before any decompiled code executes. */
    nds_arm9_ram_init();

    /* HOST_PORT: copy .data initial values from the mapped arm9.bin into
     * every C-level DAT_<addr> global (1267 of them).  Generated table
     * lives in build/generated/dat_init_table.h. */
    nds_apply_dat_inits();

    /* Task 1: snapshot the post-loop u16 staging arrays
     * DAT_0201977c..02019790 at startup.  We re-check after the game
     * has run a while; if the bytes change, code is staging there. */
    shadow_buffer_snapshot("after_dat_init");

    /* Probe + optional fake-populate the linked-list scene queue at
     * NDS 0x02060A04.  See pc/src/host_scene_queue.c. */
    extern void host_scene_queue_log_state(const char *tag);
    extern void host_scene_queue_inject_fake(void);
    extern void host_fnptr_init(void);
    extern void host_test_node_register(void);
    /* Initialise the NDS->host fnptr trampoline (auto-registers every
     * decompiled FUN_<addr>) before injecting the fake node so its
     * vtable[2]==HOST_FNPTR_SYNTHETIC_BASE+0 resolves. */
    host_fnptr_init();
    host_test_node_register();
    host_scene_queue_log_state("post_init");
    host_scene_queue_inject_fake();
    host_scene_queue_log_state("post_inject");

    /* HOST_PORT: install the .data literals FUN_02005b70 needs (slot ptr,
     * alloc size, default config, display flag).  Must run before any
     * call into FUN_02005b70 / game_state_host_engage_real. */
    host_data_init_install();
    host_display_data_init_install();

    /* HOST_PORT: install a fake sub-state so the outer-loop NULL guard
     * (`*DAT_020055B4 != 0`) becomes true and the frame_count branch fires.
     * Skip via MLPIT_NO_STATE_ENGAGE=1 to compare baseline behaviour.
     * Set MLPIT_STATE_ENGAGE_REAL=1 to instead call the legitimate
     * FUN_02005b70(NULL) initializer (allocates via OS_Alloc, sets flag
     * bits from default config, calls FUN_020192f8 / FUN_02018ed0). */
    if (!getenv("MLPIT_NO_STATE_ENGAGE")) {
        if (getenv("MLPIT_STATE_ENGAGE_REAL")) {
            /* engage_real may NPE deep inside FUN_020192f8 (whose own
             * statics are also zero today) — protect the call. */
            g_in_protected = 1;
            int esig = setjmp(g_crash_jmp);
            if (esig == 0) {
                (void)game_state_host_engage_real();
                nds_log("[game] engage_real returned cleanly\n");
            } else {
                nds_log("[game] engage_real FAULTED (signal %d) - "
                        "FUN_02005b70 deep-call statics still zero; "
                        "falling back to fake engage\n", esig);
                (void)game_state_host_engage();
            }
            g_in_protected = 0;
        } else {
            (void)game_state_host_engage();
        }
    }
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
        if (!getenv("MLPIT_DISABLE_BOOT_HOOK")) {
            if (!boot_hook_paired_screen()) {
                (void)boot_hook_real_tiles();
            }
            (void)boot_hook_paired_screen_sub();
            nds_log("[game] re-armed boot screen after game_start fault\n");
        } else {
            nds_log("[game] MLPIT_DISABLE_BOOT_HOOK=1 — leaving VRAM as-is\n");
        }
    }

    nds_log("[game] entering vblank heartbeat fallback\n");
    int frame = 0;
    while (!platform_game_should_exit()) {
        arm_swi_05_vblank_intr_wait();
        if ((frame % 120) == 0) {
            nds_log("[game] heartbeat frame=%d\n", frame);
        }
        if (frame == 60 || frame == 240 || frame == 600) {
            char tag[24];
            snprintf(tag, sizeof(tag), "frame_%d", frame);
            shadow_buffer_snapshot(tag);
        }
        frame++;
    }

    nds_log("[game] thread exiting after %d heartbeat frames\n", frame);
    return 0;
}

