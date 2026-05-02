/*
 * FUN_02005d6c.c — minimal HOST_PORT shim for the per-frame scene tick.
 *
 * Originally at arm9.bin @ 0x02005D6C (1284 bytes), this is the per-frame
 * scene-state-machine driver invoked via the queue tick (FUN_0202A33C →
 * vtable[2]).  It reads the phase byte at obj+0x10 and dispatches:
 *   phase 0: read state byte at obj+0x28, switch on it, load assets.
 *            For state 9 (used by host_game_setup_overlay) the body
 *            calls FUN_02029788(0x02097200), FUN_0202B92C(5, 0),
 *            FUN_0202B8A0(6, 3, 0) — the asset-load entry points that
 *            ultimately drive LZ77 → BG VRAM through the file system.
 *            On exit, sets phase byte to 1.
 *   phase 1..3: more update phases (asset post-process, anim init, etc.)
 *   phase 4 / default: idle (just returns).
 *
 * Faithfully decompiling the full 1284-byte body is out of scope here;
 * we provide only the phase-0/state-9 dispatch case so the natural
 * boot path can attempt its first asset load.  Once we observe what
 * faults inside the helpers, the next session will widen the cases.
 */

#include "types.h"

#include <stdio.h>

extern void FUN_02029788(u32 param_1);
extern int  FUN_0202b8a0(u32 param_1, u32 param_2, u32 param_3);
extern void FUN_0202b92c(u32 param_1, u32 param_2);

static int s_dispatch_count = 0;

void FUN_02005d6c(int obj_addr)
{
    if (obj_addr == 0) return;

    u8 phase = *(volatile u8 *)(uintptr_t)(obj_addr + 0x10);
    u8 state = *(volatile u8 *)(uintptr_t)(obj_addr + 0x28);

    s_dispatch_count++;
    if (s_dispatch_count <= 6) {
        fprintf(stderr,
                "[FUN_02005d6c] tick#%d obj=0x%08X phase=%u state=%u\n",
                s_dispatch_count, (unsigned)obj_addr,
                (unsigned)phase, (unsigned)state);
        fflush(stderr);
    }

    if (phase == 0) {
        /* Phase-0 state switch (subset) — matches the dispatch table at
         * 0x02005D98 in the original ARM body. */
        switch (state) {
        case 9:
            fprintf(stderr,
                    "[FUN_02005d6c] state=9 → asset triple "
                    "(FUN_02029788 + FUN_0202b92c + FUN_0202b8a0)\n");
            fflush(stderr);
            /* The original sequence:
             *   ldr r0, =0x02097200 ; bl FUN_02029788
             *   ldr r0, =0x00000005 ; mov r1, #0    ; bl FUN_0202b92c
             *   ldr r0, =0x00000006 ; mov r1, #3 ; mov r2, #0 ; bl FUN_0202b8a0
             */
            FUN_02029788(0x02097200u);
            fprintf(stderr, "[FUN_02005d6c] FUN_02029788 returned\n"); fflush(stderr);
            FUN_0202b92c(5u, 0u);
            fprintf(stderr, "[FUN_02005d6c] FUN_0202b92c returned\n"); fflush(stderr);
            FUN_0202b8a0(6u, 3u, 0u);
            fprintf(stderr, "[FUN_02005d6c] FUN_0202b8a0 returned\n"); fflush(stderr);
            break;
        default:
            if (s_dispatch_count <= 6) {
                fprintf(stderr,
                        "[FUN_02005d6c] state=%u not handled — phase→1\n",
                        (unsigned)state);
                fflush(stderr);
            }
            break;
        }
        /* All phase-0 cases end with phase = 1. */
        *(volatile u8 *)(uintptr_t)(obj_addr + 0x10) = 1;
        return;
    }

    /* Phases 1..4: idle for now. */
}
