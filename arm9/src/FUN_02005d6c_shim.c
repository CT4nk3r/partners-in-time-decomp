/*
 * FUN_02005d6c.c — HOST_PORT shim for the per-frame scene tick.
 *
 * Originally at arm9.bin @ 0x02005D6C (1284 bytes), this is the per-frame
 * scene-state-machine driver invoked via the queue tick (FUN_0202A33C →
 * vtable[2]).  It reads the phase byte at obj+0x10 and dispatches:
 *   phase 0: read state byte at obj+0x28, switch on it, load assets.
 *   phase 1: check stream done, post-stream callback, init state object.
 *   phase 2: more update phases (animation init, etc.)
 *   phase 3: further init
 *   phase 4 / default: idle (just returns).
 *
 * Phases 0 and 1 are fully implemented from the ARM disassembly.
 * Phases 2 and 3 are stubbed for now.
 */

#include "types.h"

#include <stdio.h>

extern void FUN_02029788(u32 param_1);
extern int  FUN_0202b8a0(u32 param_1, u32 param_2, u32 param_3);
extern void FUN_0202b92c(u32 param_1, u32 param_2);

/* Phase 1 externs */
extern u32  FUN_0202b848(void);
extern void FUN_0202a58c(int param_1);
extern void FUN_02028f48(void);
extern void FUN_02028f90(void);
extern void FUN_02028fd4(void);
extern void FUN_02029018(void);
extern void FUN_0202905c(void);
extern void FUN_020290a0(void);
extern void FUN_020290e4(void);
extern void FUN_02029128(void);

/* State 0 (gameplay) — inline alloc+construct pattern.
 * Original ARM code at 0x02005F88..0x02005FA4:
 *   BL OS_Alloc(0x30)  →  BL FUN_0206DE6C(ptr, 8, 0)
 * The wrapper is inlined in FUN_02005d6c, unlike states 2/9 which use
 * separate alloc_construct_obj_g/h wrappers. */
extern void *OS_Alloc(u32 size);
extern void FUN_0206DE6C(void *obj, int type, int param);

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
        /* Phase-0 state switch — load assets based on state byte */
        switch (state) {
        case 0:
            /* Gameplay: same base resource triple as other states */
            FUN_02029788(0x02097200u);
            FUN_0202b92c(5u, 0u);
            FUN_0202b8a0(6u, 3u, 0u);
            break;
        case 9:
            fprintf(stderr,
                    "[FUN_02005d6c] state=9 → asset triple "
                    "(FUN_02029788 + FUN_0202b92c + FUN_0202b8a0)\n");
            fflush(stderr);
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
                        "[FUN_02005d6c] phase0 state=%u not handled\n",
                        (unsigned)state);
                fflush(stderr);
            }
            break;
        }
        /* All phase-0 cases end with phase = 1. */
        *(volatile u8 *)(uintptr_t)(obj_addr + 0x10) = 1;
        return;
    }

    if (phase == 1) {
        /* Phase 1: check stream status, then dispatch init function.
         * From ARM disassembly at file offset 0x1F08:
         *   bl FUN_0202b848   ; check stream done
         *   cmp r0, #0
         *   popne {r4, lr}    ; if still streaming, return early
         *   bl FUN_0202a58c   ; post-stream callback
         *   <state switch>    ; init function per state
         *   mov r0, #4
         *   strb r0, [r4, #16] ; phase = 4
         */
        u32 stream = FUN_0202b848();
        if (stream != 0) {
            /* Still streaming — come back next frame */
            return;
        }

        FUN_0202a58c(obj_addr);

        /* State switch for phase 1 init functions */
        switch (state) {
        case 0: {
            /* Gameplay: inline alloc + construct (from ARM @ 0x02005F88-FA4) */
            void *ptr = OS_Alloc(0x30);
            if (ptr != NULL) {
                FUN_0206DE6C(ptr, 8, 0);
            }
            break;
        }
        /* state 1: needs overlay function FUN_0207781C */
        /* state 3: needs overlay function FUN_02072F60 */
        case 1:
        case 3:
            fprintf(stderr,
                    "[FUN_02005d6c] phase1 state=%u needs overlay (stubbed)\n",
                    (unsigned)state);
            break;
        case 2:  FUN_02029128(); break;
        case 4:  FUN_020290a0(); break;
        case 5:  FUN_0202905c(); break;
        case 6:  FUN_02028f90(); break;
        case 7:  FUN_02028fd4(); break;
        case 8:  FUN_02029018(); break;
        case 9:  FUN_020290e4(); break;
        case 10: FUN_02028f48(); break;
        default:
            fprintf(stderr,
                    "[FUN_02005d6c] phase1 state=%u not handled\n",
                    (unsigned)state);
            break;
        }

        fprintf(stderr,
                "[FUN_02005d6c] phase1 complete → phase=4 (state=%u)\n",
                (unsigned)state);
        fflush(stderr);

        /* Phase 1 epilogue: advance to phase 4 */
        *(volatile u8 *)(uintptr_t)(obj_addr + 0x10) = 4;
        return;
    }

    if (phase == 2) {
        /* Phase 2: unload current scene, load NEXT scene's assets.
         * Structurally identical to phase 0 (state-switched asset loading)
         * but for the destination scene. After completion → phase 3. */
        switch (state) {
        case 0:
            /* State 0 (gameplay): same base resources */
            FUN_02029788(0x02097200u);
            FUN_0202b92c(5u, 0u);
            FUN_0202b8a0(6u, 3u, 0u);
            break;
        case 2:
            /* State 2 (file select / post-title): same base resource files.
             * From literal pool: FUN_02029788(0x02097200), FUN_0202b92c(5,0),
             * FUN_0202b8a0(6,3,0). */
            FUN_02029788(0x02097200u);
            FUN_0202b92c(5u, 0u);
            FUN_0202b8a0(6u, 3u, 0u);
            break;
        case 9:
            /* State 9 (title screen): identical resources */
            FUN_02029788(0x02097200u);
            FUN_0202b92c(5u, 0u);
            FUN_0202b8a0(6u, 3u, 0u);
            break;
        default:
            fprintf(stderr,
                    "[FUN_02005d6c] phase2 state=%u: asset load (generic)\n",
                    (unsigned)state);
            fflush(stderr);
            /* Use same base resources as a fallback */
            FUN_02029788(0x02097200u);
            FUN_0202b92c(5u, 0u);
            FUN_0202b8a0(6u, 3u, 0u);
            break;
        }

        fprintf(stderr,
                "[FUN_02005d6c] phase2 complete → phase=3 (state=%u)\n",
                (unsigned)state);
        fflush(stderr);

        /* Advance to phase 3 */
        *(volatile u8 *)(uintptr_t)(obj_addr + 0x10) = 3;
        return;
    }

    if (phase == 3) {
        /* Phase 3: wait for streaming to complete, then construct next scene.
         * Mirrors phase 1 exactly (check stream, dispatch constructor). */
        u32 stream = FUN_0202b848();
        if (stream != 0) {
            return;  /* still streaming, come back next frame */
        }

        FUN_0202a58c(obj_addr);

        switch (state) {
        case 0: {
            /* Gameplay: inline alloc + construct (from ARM @ 0x020061F8-6214) */
            void *ptr = OS_Alloc(0x30);
            if (ptr != NULL) {
                FUN_0206DE6C(ptr, 8, 0);
            }
            break;
        }
        case 2:  FUN_02029128(); break;  /* alloc_construct_obj_h → next scene */
        case 4:  FUN_020290a0(); break;
        case 5:  FUN_0202905c(); break;
        case 6:  FUN_02028f90(); break;
        case 7:  FUN_02028fd4(); break;
        case 8:  FUN_02029018(); break;
        case 9:  FUN_020290e4(); break;  /* title screen constructor */
        case 10: FUN_02028f48(); break;
        default:
            fprintf(stderr,
                    "[FUN_02005d6c] phase3 state=%u not handled\n",
                    (unsigned)state);
            break;
        }

        fprintf(stderr,
                "[FUN_02005d6c] phase3 complete → phase=4 (state=%u)\n",
                (unsigned)state);
        fflush(stderr);

        /* Advance to phase 4 (idle) */
        *(volatile u8 *)(uintptr_t)(obj_addr + 0x10) = 4;
        return;
    }

    /* Phases 2, 3: stubbed for now */
    /* Phase 4 / default: idle (just returns) */
}
