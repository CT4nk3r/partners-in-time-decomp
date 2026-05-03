/*
 * FUN_0202a74c_real.c — real implementation of queue-insert at 0x0202A74C.
 *
 * Hand-decompiled from arm9.bin @ 0x0202A74C..0x0202A848 (252 bytes).
 *
 * Inserts `node` into the doubly-linked priority-sorted scene queue whose
 * head pointer lives at NDS 0x02060A04.  Each node carries:
 *   +00  vtable ptr               (initialised to 0x020597C8 by this fn,
 *                                   then overwritten by the caller —
 *                                   FUN_02006304 sets it to 0x0204FFDC)
 *   +04  user param (r3 arg)
 *   +08  prev (NDS ptr)
 *   +0C  next (NDS ptr)
 *   +10  u8 phase byte (=0 here)
 *   +11  u8 priority byte (=r1 arg)
 *   +12  u16 flag bits (bit0=needs-update, bit1=scheduled, bit2=set,
 *                        bits 2-5 cleared then bit2=1, bits 6-9 cleared)
 *   +14  u32 zero
 *
 * The list is sorted ascending by priority byte at +0x11; we insert
 * before the first existing node with priority > ours.
 *
 * The original signature takes r0=node, r1=priority, r2=(scratch — bytes
 * shows it's loaded from a literal pool but never used afterwards), and
 * r3=user param.  Most decomp call sites in arm9/src/*.c declare the
 * function as `void FUN_0202a74c(void)` with no args because the legacy
 * auto-decomp couldn't recover them; we keep that compat stub in
 * host_undefined_stubs.c (no-op) and expose this real entry under the
 * `_real` suffix for the host_game_setup_overlay path that needs to
 * actually populate the scene queue.
 */

#include "types.h"

#ifdef HOST_PORT
#include <stdio.h>
#include <stdlib.h>
#endif

#define DEFAULT_VTABLE_NDS  0x020597C8u  /* DAT_0x202a844 */
#define QUEUE_HEAD_NDS      0x02060A04u  /* DAT_0x202a848 */
#define INSERT_COUNTER_NDS  0x020609F0u  /* DAT_0x202a84c (u8) */

void FUN_0202a74c_real(u32 node_addr, u8 priority, u32 r2_unused, u32 r3_param)
{
    (void)r2_unused;

    if (node_addr == 0) return;

    /* Defensive: caller stub may pass an unmapped address.  All real
     * NDS pointers must lie in [0x02000000, 0x02400000). */
    if (node_addr < 0x02000000u || node_addr >= 0x02400000u) {
#ifdef HOST_PORT
        static int s; if (!s) { s = 1;
            fprintf(stderr,
                    "[FUN_0202a74c_real] bogus node_addr=0x%08X — skipping\n",
                    (unsigned)node_addr);
        }
#endif
        return;
    }

    volatile u32 *node = (volatile u32 *)(uintptr_t)node_addr;

    /* node[0] = default vtable (caller may overwrite). */
    node[0] = DEFAULT_VTABLE_NDS;

    volatile u32 *queue_head = (volatile u32 *)(uintptr_t)QUEUE_HEAD_NDS;
    u32 walker_addr = *queue_head;

    if (walker_addr == 0) {
        /* Empty list → install as head. */
        *queue_head      = node_addr;
        node[2 /*+8*/]   = 0;            /* prev = NULL */
        node[3 /*+12*/]  = 0;            /* next = NULL */
        goto finish;
    }

    /* Walk priority-sorted list looking for first node whose priority
     * byte (+0x11) is strictly greater than ours; insert before it. */
    for (;;) {
        if (walker_addr < 0x02000000u || walker_addr >= 0x02400000u) {
#ifdef HOST_PORT
            static int s; if (!s) { s = 1;
                fprintf(stderr,
                        "[FUN_0202a74c_real] walker out of bounds 0x%08X\n",
                        (unsigned)walker_addr);
            }
#endif
            return;
        }
        volatile u32 *walker = (volatile u32 *)(uintptr_t)walker_addr;
        volatile u8  *walker_b = (volatile u8 *)(uintptr_t)walker_addr;
        u8 walker_prio = walker_b[0x11];

        if (walker_prio > priority) {
            /* Insert node before walker. */
            u32 walker_prev = walker[2 /*+8*/];
            if (walker_prev == 0) {
                /* walker is the head — node becomes new head. */
                *queue_head = node_addr;
                node[2 /*+8*/] = 0;
            } else {
                volatile u32 *prev = (volatile u32 *)(uintptr_t)walker_prev;
                prev[3 /*+12*/] = node_addr;       /* prev->next = node */
                node[2 /*+8*/]  = walker[2 /*+8*/]; /* node->prev = walker->prev */
            }
            node[3 /*+12*/]  = walker_addr;        /* node->next = walker */
            walker[2 /*+8*/] = node_addr;          /* walker->prev = node */
            goto finish;
        }

        u32 walker_next = walker[3 /*+12*/];
        if (walker_next == 0) {
            /* walker is tail → append. */
            walker[3 /*+12*/] = node_addr;
            node[2 /*+8*/]    = walker_addr;
            node[3 /*+12*/]   = 0;
            goto finish;
        }
        walker_addr = walker_next;
    }

finish:
    /* Per-node state init, mirroring 0x202A7DC..0x202A828. */
    node[1 /*+4*/] = r3_param;
    {
        volatile u8 *nb = (volatile u8 *)(uintptr_t)node_addr;
        nb[0x10] = 0;          /* phase byte */
        nb[0x11] = priority;
    }
    {
        volatile u16 *flags = (volatile u16 *)(uintptr_t)(node_addr + 0x12);
        u16 f = *flags;
        f = (u16)((f & ~1u) | 1u);     /* bit0 = 1 */
        *flags = f;
        f |= 2u;                       /* bit1 = 1 */
        *flags = f;
        f = (u16)((f & ~0x3Cu) | 4u);  /* clear bits2-5, set bit2 */
        *flags = f;
        f &= ~0x3C0u;                  /* clear bits6-9 */
        *flags = f;
    }
    node[5 /*+20*/] = 0;        /* counter zero */

    /* Bump global insert counter. */
    {
        volatile u8 *ctr = (volatile u8 *)(uintptr_t)INSERT_COUNTER_NDS;
        *ctr = (u8)(*ctr + 1);
    }

#ifdef HOST_PORT
    static int s_logged = 0;
    if (s_logged < 4) {
        s_logged++;
        fprintf(stderr,
                "[FUN_0202a74c_real] inserted node=0x%08X prio=%u r3=0x%08X "
                "head=0x%08X\n",
                (unsigned)node_addr, (unsigned)priority,
                (unsigned)r3_param, (unsigned)*queue_head);
        fflush(stderr);
    }
#endif
}
