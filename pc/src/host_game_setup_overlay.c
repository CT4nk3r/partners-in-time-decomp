/*
 * host_game_setup_overlay.c — host port of FUN_02004EF8 (game_setup_overlay).
 *
 * Decompiled directly from arm9.bin @ 0x02004EF8 (28 instructions, 116 bytes).
 *
 * ARM disassembly (verified):
 *   push  {lr}
 *   sub   sp, sp, #12
 *   ldr   r3, =0x02059c6c               ; param_4 to FUN_02004e38
 *   bl    FUN_02004e38                  ; (overlay_id, flags(r1), r2, &cfg)
 *   ldr   r0, =0x02059bd8
 *   mov   r1, #0
 *   strb  r1, [r0]                       ; *(u8*)0x02059bd8 = 0
 *   bl    FUN_0200762c
 *   ldr   r2, =0x0204ff64               ; vtable pointer (param_3 to alloc/ctor)
 *   mov   r0, #0x2c                      ; alloc size = 44 bytes
 *   mov   r1, #1                         ; alloc dir/flags = 1
 *   mov   r3, #0
 *   bl    FUN_02029c1c                  ; obj = OS_Alloc(0x2c, 1, vtable, 0)
 *   cmp   r0, #0
 *   beq   .skip
 *   mov   r1, #9
 *   str   r1, [sp]                       ; stack[0] = 9
 *   mov   r3, #0
 *   ldr   ip, =0x02059c6c
 *   str   r3, [sp, #4]                   ; stack[1] = 0
 *   ldr   r2, =0x0204ff64
 *   mov   r1, #8
 *   str   ip, [sp, #8]                   ; stack[2] = 0x02059c6c
 *   bl    FUN_02006304                  ; ctor(obj, 8, vtable, 0, 9, 0, &cfg)
 * .skip:
 *   ldr   r1, =0x02059c7c
 *   str   r0, [r1]                       ; *(u32*)0x02059c7c = obj
 *   add   sp, sp, #12
 *   pop   {lr}
 *   bx    lr
 *
 * Behaviour:
 *   1. Initialise display config block at NDS 0x02059C6C via FUN_02004e38.
 *   2. Clear u8 flag at 0x02059BD8.
 *   3. Run FUN_0200762C (sound-related side effect).
 *   4. OS_Alloc 44-byte scene-anchor object (vtable=0x0204FF64).
 *   5. If alloc succeeded, run scene constructor FUN_02006304 (which
 *      internally calls FUN_02005b70 → installs sGameState->current,
 *      and FUN_02005d54 → tail-calls scene_jmp / FUN_0202A56C).
 *   6. Stash scene anchor pointer at NDS 0x02059C7C.
 *
 * Risk on host: FUN_02006304 → FUN_02004f7c calls archive loaders whose
 * .data globals may not be initialised here yet.  Therefore the real
 * path is gated behind MLPIT_REAL_SETUP_OVERLAY=1.  Default behaviour
 * remains the prior smoke-test (calls FUN_02005d54 directly so the
 * scene_jmp trampoline still fires for verification).
 */
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void FUN_02004e38(u32 param_1, u16 param_2, u16 param_3, u16 *param_4);
extern void FUN_0200762c(void);
extern u32  FUN_02029c1c(u32 size, u32 dir, u32 type, u32 flag);
extern u32 *FUN_02006304(u32 *param_1, u32 param_2, u16 *param_3);
extern void FUN_02005d54(int param_1, u8 param_2);
extern void FUN_0202a74c_real(u32 node_addr, u8 priority, u32 r2_unused, u32 r3_param);
extern int  nds_arm9_ram_is_mapped(void);

/* Scene-tick vtable installed by FUN_02006304: 0x0204FFDC.  Slot 2
 * (offset +8) = FUN_02005D6C — the per-frame scene tick that dispatches
 * by phase byte at obj+0x10 (state byte at obj+0x28).  See arm9_full
 * disassembly @ 0x0204FFDC.  We install this directly to mirror the
 * real ctor without invoking FUN_02004F7C / FUN_02005B70 (whose
 * archive / clGameMain init paths still fault on host). */
#define SCENE_VTABLE_DATA_NDS  0x0204FFDCu

/* Original NDS .data slots used by FUN_02004EF8.  These all live inside
 * arm9.bin's .data section (mapped at 0x02004000+ on host via
 * nds_arm9_ram.c VirtualAlloc), so direct dereferences are safe once
 * arm9 RAM is mapped. */
#define DISP_CFG_NDS    0x02059C6Cu  /* display config struct */
#define BOOT_FLAG_NDS   0x02059BD8u  /* u8 flag */
#define SCENE_PTR_NDS   0x02059C7Cu  /* u32 scene-anchor pointer slot */
#define SCENE_VTABLE_NDS 0x0204FF64u /* scene anchor vtable / type desc */

/* Smoke-test fallback when MLPIT_REAL_SETUP_OVERLAY is not set. */
#define HOST_FAKE_SCENE_NDS_ADDR  0x02300000u
#define HOST_FAKE_SCENE_SIZE      0x80u

static int g_setup_call_count = 0;

void game_setup_overlay(u32 overlay_id, u32 flags)
{
    g_setup_call_count++;
    fflush(stderr);
    fprintf(stderr,
            "[setup_overlay] call#%d overlay_id=0x%08X flags=0x%08X\n",
            g_setup_call_count, (unsigned)overlay_id, (unsigned)flags);
    fflush(stderr);

    if (g_setup_call_count > 1) return;

    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr,
                "[setup_overlay] arm9 RAM not mapped — skipping\n");
        return;
    }

    {
        /* Faithful FUN_02004EF8 port (partial — see notes below). */

        /* Step 1: FUN_02004e38(overlay_id, flags, r2_unused, &cfg).
         * Pure bit-twiddling on the 0x02059C6C halfword array — safe. */
        FUN_02004e38(overlay_id,
                     (u16)flags,
                     (u16)0,
                     (u16 *)(uintptr_t)DISP_CFG_NDS);

        /* Step 2: clear boot flag.  Safe. */
        *(volatile u8 *)(uintptr_t)BOOT_FLAG_NDS = 0;

        /* Step 3: FUN_0200762c — zeros 0x38 bytes of sound state at
         * 0x02059C9C.  Now safe to invoke because misc_funcs.c uses a
         * HOST_PORT short-circuit that calls MI_CpuFill32Fast directly
         * instead of indirecting through DAT_02007640 (which would jump
         * into raw arm9.bin bytes on host). */
        FUN_0200762c();

        /* Step 4: alloc 44-byte scene-anchor object (now actually
         * returns a real pointer because FUN_02029c1c forwards to
         * OS_Alloc — see pc/src/host_undefined_stubs.c). */
        /* OS_Alloc on host returns a host-arena pointer (~0x10000000)
         * which is not a valid NDS-mapped address — the scene queue
         * tick treats nodes as NDS pointers, so we must allocate from
         * the NDS-mapped 0x02300000 region instead.  We bump-allocate
         * from a fixed slab past arm9.bin's high water mark; not freed. */
        static u32 s_scene_arena_off = 0x02301000u;
        u32 obj_u32 = s_scene_arena_off;
        s_scene_arena_off += 0x40u;  /* round 0x2c up to 0x40 */
        memset((void *)(uintptr_t)obj_u32, 0, 0x40);
        fprintf(stderr,
                "[setup_overlay] alloc(44) -> 0x%08X (NDS arena)\n",
                (unsigned)obj_u32);

        u32 *obj = NULL;
        if (obj_u32 != 0) {
            obj = (u32 *)(uintptr_t)obj_u32;

            /* === Replicate FUN_02006304 effects without FUN_02004F7C /
             * FUN_02005B70 (those still fault on host).  The replicated
             * ctor just needs to:
             *   1. Insert obj into the scene queue (FUN_0202A74C)
             *   2. Install scene vtable at obj[0]
             *   3. Set state byte (obj+0x28) and phase byte (obj+0x10)
             *      via FUN_02005D54.
             *
             * The tick processor (FUN_0202A33C, wired in main loop via
             * FUN_0202a33c_safe) then walks the queue, calls
             * vtable[2]=FUN_02005D6C(obj), which dispatches by phase=0
             * to state-9 (or whatever caller set) — the boot-asset
             * loader path that hits LZ77→VRAM. */

            FUN_0202a74c_real(obj_u32, /*priority*/ 8, /*r2*/ 0,
                              /*r3=user param*/ 0);

            /* Install the real scene vtable so vtable[2] = FUN_02005D6C
             * resolves naturally through the host fnptr trampoline. */
            obj[0] = SCENE_VTABLE_DATA_NDS;

            /* obj+0x10 = phase byte (=0 dispatches to state switch);
             * obj+0x28 = state byte.  FUN_02005D54 sets both via the
             * trampoline DAT_02005D68 = FUN_0202A56C (which OR-sets
             * obj[18] |= 3 for "needs update / scheduled"). */
            FUN_02005d54((int)(uintptr_t)obj, /*state*/ 9);
            fprintf(stderr,
                    "[setup_overlay] FUN_02005d54 returned\n");
        }

        /* Step 6: stash anchor pointer at the NDS .data slot the rest
         * of the engine reads through. */
        *(volatile u32 *)(uintptr_t)SCENE_PTR_NDS =
            (u32)(uintptr_t)obj;

        fprintf(stderr,
                "[setup_overlay] real path complete; *(u32*)0x%08X = 0x%08X\n",
                (unsigned)SCENE_PTR_NDS,
                (unsigned)(uintptr_t)obj);
    }
}
