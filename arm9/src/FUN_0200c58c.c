/*
 * FUN_0200c58c.c — hand-decompiled from arm9.bin @ 0x0200C58C.
 *
 * Constructor for the class whose vtable is at NDS 0x02050B6C
 * ("scene/game-object", 53-entry vtable, see .session-work/vtable_0x02050b6c.txt).
 *
 * Recovered ARM body (verified against extracted/arm9.bin):
 *
 *   0200c58c  e92d4010  push {r4, lr}
 *   0200c590  e59f102c  ldr  r1, [pc, #44]    ; literal at 0x200c5c4 = 0x02050B6C
 *   0200c594  e1a04000  mov  r4, r0
 *   0200c598  e5841000  str  r1, [r4]         ; *this = vtable
 *   0200c59c  e5901000  ldr  r1, [r0]
 *   0200c5a0  e59110c0  ldr  r1, [r1, #0xc0]  ; r1 = vtable[48]
 *   0200c5a4  e12fff31  blx  r1               ; call this->vtable[48](this)
 *   0200c5a8  e1a00004  mov  r0, r4
 *   0200c5ac  ebfff24c  bl   FUN_02008ee4     ; base ctor / init helper
 *   0200c5b0  e1a00004  mov  r0, r4
 *   0200c5b4  eb00658c  bl   FUN_02025bec     ; secondary init
 *   0200c5b8  e1a00004  mov  r0, r4
 *   0200c5bc  e8bd4010  pop  {r4, lr}
 *   0200c5c0  e12fff1e  bx   lr
 *   0200c5c4  02050b6c  .word 0x02050B6C      ; literal pool: vtable
 *
 * Notes:
 * - vtable[48] is the per-class virtual init slot; for the sole class
 *   that uses vtable 0x02050B6C it resolves to FUN_0200F7CC (game_logic_b.c).
 * - Returns `this` in r0 (the AAPCS return register).
 *
 * Static-callsite analysis (see .session-work/vtable_0x02050b6c_writers.txt)
 * shows NO direct caller in arm9.bin or any overlay, neither as an ARM
 * BL nor as a THUMB BLX, and no data-section pointer to this address.
 * The constructor must therefore be entered via a runtime-built dispatch
 * table (object factory) that the host port has not yet wired up.
 */

#include "types.h"
#include <stdint.h>

/* Helpers live in arm9.bin (still undecomp).  Route through the host
 * fnptr resolver so calls are no-ops until the underlying function is
 * decompiled or trampolined. */
extern void nds_call_1arg(uint32_t nds_addr, uintptr_t a);

void *FUN_0200c58c(void *this)
{
    *(unsigned int *)this = 0x02050B6Cu;

    /* Virtual call: (*((void(**)(void*))*(void**)this)[48])(this). */
    void (**vt)(void *) = *(void (***)(void *))this;
    if (vt && vt[48]) vt[48](this);

    nds_call_1arg(0x02008ee4u, (uintptr_t)this);
    nds_call_1arg(0x02025becu, (uintptr_t)this);
    return this;
}
