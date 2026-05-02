/*
 * FUN_0202a56c.c — hand-decompiled from arm9.bin @ 0x0202A56C.
 *
 * This is an indirectly-called leaf the original decompiler missed.  It
 * lives inside the body of FUN_0202a53c (the "list-push" routine that
 * starts at 0x0202a53c and ends at 0x0202a5a0) as a fast-path entry
 * point that skips the head==NULL check.  External tail-callers (e.g.
 * the real FUN_02005d54 at 0x02001d3c, whose literal pool stores the
 * 0x0202A56C target) jump straight here once they know the list head
 * is already initialised.
 *
 * The shared epilogue at 0x0202a598..0x0202a5a0 (`add sp,sp,#4 / pop
 * {lr} / bx lr`) belongs to FUN_0202a53c's prologue, NOT to the entry
 * at 0x0202a56c — under HOST_PORT we are calling this as a plain C
 * function so the stack-fixup is irrelevant.
 *
 * Recovered ARM body (verified against extracted/arm9.bin):
 *
 *   0202a56c  e781300c  str  r3, [r1, ip]
 *   0202a570  e081300c  add  r3, r1, ip
 *   0202a574  e5832004  str  r2, [r3, #4]
 *   0202a578  e1d020ba  ldrh r2, [r0, #10]
 *   0202a57c  e5903004  ldr  r3, [r0, #4]
 *   0202a580  e0832002  add  r2, r3, r2
 *   0202a584  e5821004  str  r1, [r2, #4]
 *   0202a588  e5801004  str  r1, [r0, #4]
 *   0202a58c  e1d010b8  ldrh r1, [r0, #8]
 *   0202a590  e2811001  add  r1, r1, #1
 *   0202a594  e1c010b8  strh r1, [r0, #8]
 *
 * On entry:
 *   r0 = list head/sentinel  (struct: +00 head, +04 tail, +08 count(u16),
 *                                     +0a link_offset(u16))
 *   r1 = node to append      (the "scene_jmp arg" passed by FUN_02005d54)
 *   r2 = 0                   (loaded just above the entry point at
 *                             0x0202a568, but we re-derive it here)
 *   r3 = *(int *)(r0 + 4)    (loaded at 0x0202a564)
 *   ip = *(u16 *)(r0 + 0xa)  (loaded at 0x0202a560)
 *
 * Behaviour: append `r1` to the doubly-linked list whose anchor lives
 * at `r0`, with the per-node link pair sitting at offset
 * `*(u16 *)(r0+0xa)` inside each node:
 *
 *     node->prev_via_off = old_tail;
 *     node->next_via_off = NULL;
 *     old_tail->next_via_off = node;
 *     head->tail = node;
 *     head->count++;
 */

#include "types.h"

#ifdef HOST_PORT
#include <stdint.h>
#include <stdio.h>
#endif

void FUN_0202a56c(int param_1 /* r0 = list head */,
                  int param_2 /* r1 = node ptr  */)
{
#ifdef HOST_PORT
    /* Defensive: on the real DS the immediate caller (FUN_02005d54 @
     * 0x02001d54 variant) tail-calls us with r1 == 0, relying on ITCM
     * at low addresses to absorb the resulting writes.  Our host build
     * has no ITCM mapping; bail with a one-shot log instead of faulting.
     * The full FUN_0202a53c entry at 0x0202a53c already null-checks
     * r0[0] before this fast-path, but it does NOT validate r1. */
    static int s_warned = 0;
    if (param_1 == 0 || param_2 == 0) {
        if (!s_warned) {
            s_warned = 1;
            fprintf(stderr,
                    "[FUN_0202a56c] guarded: head=0x%08X node=0x%08X "
                    "(would corrupt low memory; needs ITCM) — skipping\n",
                    (unsigned)param_1, (unsigned)param_2);
        }
        return;
    }
#endif

    u16 link_off = *(u16 *)(param_1 + 0x0a);
    int old_tail = *(int *)(param_1 + 0x04);

    /* node->prev = old_tail; node->next = NULL */
    *(int *)(param_2 + link_off)     = old_tail;
    *(int *)(param_2 + link_off + 4) = 0;

    /* Reload (compiler emitted these reloads — preserve cycle behaviour). */
    link_off = *(u16 *)(param_1 + 0x0a);
    old_tail = *(int *)(param_1 + 0x04);

    /* old_tail->next = node; head->tail = node; head->count++ */
    *(int *)(old_tail + link_off + 4) = param_2;
    *(int *)(param_1 + 0x04)          = param_2;
    *(u16 *)(param_1 + 0x08) =
        (u16)(*(u16 *)(param_1 + 0x08) + 1);
}
