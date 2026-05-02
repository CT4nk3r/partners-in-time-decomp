/*
 * FUN_0200c5fc.c — hand-decompiled from arm9.bin @ 0x0200C5FC.
 *
 * Third constructor of class with vtable 0x02050B6C.  Unlike the other
 * two ctors this one calls a base-ctor BEFORE storing the vtable and
 * then memsets a sub-region of the object before kicking off animation
 * subsystem init.
 *
 * Recovered ARM body (verified against extracted/arm9.bin):
 *
 *   0200c5fc  e92d4010  push {r4, lr}
 *   0200c600  e1a04000  mov  r4, r0
 *   0200c604  ebfff261  bl   FUN_02008f90        ; base/parent ctor
 *   0200c608  e59f3054  ldr  r3, [pc, #84]       ; literal @0x200c664 = 0x02050B6C
 *   0200c60c  e2840e13  add  r0, r4, #0x130
 *   0200c610  e2841f6d  add  r1, r4, #0x1b4
 *   0200c614  e0412000  sub  r2, r1, r0          ; r2 = 0x84 (size to clear)
 *   0200c618  e3a01000  mov  r1, #0
 *   0200c61c  e5843000  str  r3, [r4]            ; *this = vtable
 *   0200c620  eb00717a  bl   FUN_02028c10        ; memset(this+0x130, 0, 0x84)
 *   0200c624  e2841c01  add  r1, r4, #0x100
 *   0200c628  e1d136b2  ldrh r3, [r1, #0x62]
 *   0200c62c  e59f2034  ldr  r2, [pc, #52]       ; literal @0x200c668 = 0x00007FFF
 *   0200c630  e3a00010  mov  r0, #0x10
 *   0200c634  e3c3301f  bic  r3, r3, #0x1f
 *   0200c638  e383301f  orr  r3, r3, #0x1f
 *   0200c63c  e1c136b2  strh r3, [r1, #0x62]     ; (r3 & ~0x1f) | 0x1f
 *   0200c640  e1c126b4  strh r2, [r1, #0x64]     ; 0x7FFF
 *   0200c644  e1c106bc  strh r0, [r1, #0x6c]     ; 0x10
 *   0200c648  e3a02001  mov  r2, #1
 *   0200c64c  e2840f5d  add  r0, r4, #0x174
 *   0200c650  e1c126be  strh r2, [r1, #0x6e]     ; 1
 *   0200c654  eb008ef2  bl   FUN_02030224        ; anim subsystem init
 *   0200c658  e1a00004  mov  r0, r4
 *   0200c65c  e8bd4010  pop  {r4, lr}
 *   0200c660  e12fff1e  bx   lr
 *   0200c664  02050b6c  .word 0x02050B6C
 *   0200c668  00007fff  .word 0x00007FFF
 */

#include "types.h"
#include <string.h>
#include <stdint.h>

extern void nds_call_1arg(uint32_t nds_addr, uintptr_t a);
extern void nds_call_2arg(uint32_t nds_addr, uintptr_t a, uintptr_t b);

void *FUN_0200c5fc(void *this)
{
    unsigned char *p = (unsigned char *)this;

    nds_call_1arg(0x02008f90u, (uintptr_t)this);

    *(unsigned int *)this = 0x02050B6Cu;

    /* memset(this+0x130, 0, 0x84) — host-side we just clear directly to
     * avoid depending on the DS memset symbol; this matches the ARM
     * routine bit-for-bit. */
    memset(p + 0x130, 0, 0x84);

    {
        unsigned char *sub = p + 0x100;
        unsigned short *sub16 = (unsigned short *)sub;
        unsigned short v62 = sub16[0x62 / 2];
        sub16[0x62 / 2] = (unsigned short)((v62 & ~0x1f) | 0x1f);
        sub16[0x64 / 2] = 0x7FFF;
        sub16[0x6c / 2] = 0x0010;
        sub16[0x6e / 2] = 0x0001;
    }

    nds_call_2arg(0x02030224u, (uintptr_t)(p + 0x174), 0x10);
    return this;
}
