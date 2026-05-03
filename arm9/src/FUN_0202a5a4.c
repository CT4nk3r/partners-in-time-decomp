/*
 * FUN_0202a5a4.c — hand-decompiled from arm9.bin @ 0x0202A5A4.
 *
 * "First insertion" leaf for the doubly-linked-list anchor used by
 * FUN_0202a53c / FUN_0202a56c.  Called by the prologue at 0x0202a53c
 * when the list head is empty (head[0] == NULL) — sets head=tail=node,
 * zeros the node's prev/next, and bumps count.
 *
 * Recovered ARM body (verified against extracted/arm9.bin):
 *
 *   0202a5a4  e1d030ba  ldrh r3, [r0, #10]
 *   0202a5a8  e3a02000  mov  r2, #0
 *   0202a5ac  e081c003  add  ip, r1, r3
 *   0202a5b0  e58c2004  str  r2, [ip, #4]
 *   0202a5b4  e7812003  str  r2, [r1, r3]
 *   0202a5b8  e5801000  str  r1, [r0]
 *   0202a5bc  e5801004  str  r1, [r0, #4]
 *   0202a5c0  e1d010b8  ldrh r1, [r0, #8]
 *   0202a5c4  e2811001  add  r1, r1, #1
 *   0202a5c8  e1c010b8  strh r1, [r0, #8]
 *   0202a5cc  e12fff1e  bx   lr
 */

#include "types.h"

void FUN_0202a5a4(int param_1 /* head */, int param_2 /* node */)
{
    u16 link_off = *(u16 *)(param_1 + 0x0a);

    *(int *)(param_2 + link_off + 4) = 0;
    *(int *)(param_2 + link_off)     = 0;
    *(int *)(param_1 + 0x00)         = param_2;
    *(int *)(param_1 + 0x04)         = param_2;
    *(u16 *)(param_1 + 0x08) =
        (u16)(*(u16 *)(param_1 + 0x08) + 1);
}
