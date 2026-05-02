/*
 * FUN_0200c5c8.c — hand-decompiled from arm9.bin @ 0x0200C5C8.
 *
 * Sibling constructor of FUN_0200c58c: same class (vtable 0x02050B6C),
 * but skips the secondary FUN_02025bec init step.  Likely the
 * "lightweight" constructor variant (no audio/anim subsystem hookup).
 *
 * Recovered ARM body (verified against extracted/arm9.bin):
 *
 *   0200c5c8  e92d4010  push {r4, lr}
 *   0200c5cc  e59f1024  ldr  r1, [pc, #36]    ; literal at 0x200c5f8 = 0x02050B6C
 *   0200c5d0  e1a04000  mov  r4, r0
 *   0200c5d4  e5841000  str  r1, [r4]         ; *this = vtable
 *   0200c5d8  e5901000  ldr  r1, [r0]
 *   0200c5dc  e59110c0  ldr  r1, [r1, #0xc0]  ; r1 = vtable[48]
 *   0200c5e0  e12fff31  blx  r1
 *   0200c5e4  e1a00004  mov  r0, r4
 *   0200c5e8  ebfff23d  bl   FUN_02008ee4
 *   0200c5ec  e1a00004  mov  r0, r4
 *   0200c5f0  e8bd4010  pop  {r4, lr}
 *   0200c5f4  e12fff1e  bx   lr
 *   0200c5f8  02050b6c  .word 0x02050B6C
 */

#include "types.h"
#include <stdint.h>

extern void nds_call_1arg(uint32_t nds_addr, uintptr_t a);

void *FUN_0200c5c8(void *this)
{
    *(unsigned int *)this = 0x02050B6Cu;

    {
        unsigned int *vt = *(unsigned int **)this;
        if (vt) {
            unsigned int target = vt[48];
            if (target) nds_call_1arg(target, (uintptr_t)this);
        }
    }

    nds_call_1arg(0x02008ee4u, (uintptr_t)this);
    return this;
}
