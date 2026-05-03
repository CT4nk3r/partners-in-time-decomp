/*
 * host_scene_queue.c — HOST_PORT scene-queue probe + optional fake populator.
 *
 * The decompiled game has a doubly-linked-list "scene/object queue" anchor
 * struct at NDS 0x02060A04 (per gap_0301.s data dump) shared by:
 *   - DAT_0202a510 (master list head ptr, used by FUN_0202a33c — the
 *                   per-frame list processor that dispatches per-node
 *                   update via vtable[2])
 *   - DAT_0202a568 (gx list head ptr, used by FUN_0202a51c — marks
 *                   the "needs update" bit at +0x12 of every node)
 *
 * Anchor struct layout (recovered from FUN_0202a56c.c hand-decomp):
 *   +00 head ptr
 *   +04 tail ptr
 *   +08 count (u16)
 *   +0a link_offset (u16)   — offset within each node where prev/next live
 *
 * Per-node fields used by FUN_0202a33c / FUN_0202a51c:
 *   +00 vtable ptr (its slot +8 is invoked when active)
 *   +0c next (when link_offset==0xc; conventional default)
 *   +12 (u16) flag bits — bit0 = "needs update", bit1 = scheduled
 *   +11 (s8)  active flag (FUN_0202a51c only updates when != 0 && != -1)
 *
 * Currently observed on this host build:
 *   - The wholesale arm9.bin → ARM9 RAM init places the static .data words
 *     at NDS 0x0202a510..0x0202a51c so they hold the proper anchor
 *     pointer 0x02060A04 — verified.
 *   - The anchor at 0x02060A04 is zeroed (no populator has run yet);
 *     its head/tail/count are all 0.
 *   - On x86_64 the C `extern int *DAT_0202a510;` declarations clash
 *     with the 4-byte `uint32_t DAT_0202a510;` definitions in
 *     host_undefined_stubs.c, so reading via the symbol is unsafe.
 *     This file only ever reads/writes through direct NDS pointer
 *     literals into the VirtualAlloc'd 4 MiB ARM9 RAM, sidestepping
 *     the type-mismatch entirely.
 *
 * Behaviour:
 *   - host_scene_queue_log_state(tag): one-shot log of the anchor's
 *     head/tail/count/link_offset and the first 4 nodes' (vtable, next,
 *     flags) tuples.  Throttled by tag-uniqueness (we only log once
 *     per distinct tag).
 *   - host_scene_queue_inject_fake(): if MLPIT_FAKE_QUEUE_NODE=1 is set
 *     in the environment, install a single zero-flag fake node at
 *     0x02300100 and point the anchor at it.  The flag bits are zero so
 *     FUN_0202a33c will skip the indirect vtable[2] call (the test
 *     `((u8)flags << 0x1f) < 0` requires bit 0 set).  Goal: confirm the
 *     processor actually walks the chain (visible because the count
 *     stays 1 and the flag-mark in FUN_0202a51c won't fire either).
 */

#include "types.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern int  nds_arm9_ram_is_mapped(void);

/* The anchor struct address is hard-coded from gap_0301.s.  We do NOT
 * read DAT_0202a510 because of the host 32-vs-64-bit pointer storage
 * mismatch described above. */
#define SCENE_ANCHOR_NDS_ADDR  0x02060A04u

/* Fake node we own: well past arm9.bin's 0x0205C000 high water mark and
 * outside any other host-allocated region in the 4 MiB ARM9 mapping. */
#define FAKE_NODE_NDS_ADDR     0x02300100u
#define FAKE_NODE_SIZE         0x40u

static int g_logged_post_init   = 0;
static int g_logged_post_dispatch = 0;
static int g_fake_injected      = 0;

static u32 nds_r32(u32 nds_addr)
{
    return *(volatile u32 *)(uintptr_t)nds_addr;
}

static u16 nds_r16(u32 nds_addr)
{
    return *(volatile u16 *)(uintptr_t)nds_addr;
}

static void nds_w32(u32 nds_addr, u32 v)
{
    *(volatile u32 *)(uintptr_t)nds_addr = v;
}

static void nds_w16(u32 nds_addr, u16 v)
{
    *(volatile u16 *)(uintptr_t)nds_addr = v;
}

void host_scene_queue_log_state(const char *tag)
{
    if (!nds_arm9_ram_is_mapped()) return;

    /* Tag-uniqueness gate: single log per unique tag.  We compare against
     * the two known tags only — any new tag passed in is logged once. */
    if (tag && strcmp(tag, "post_init") == 0) {
        if (g_logged_post_init) return;
        g_logged_post_init = 1;
    } else if (tag && strcmp(tag, "post_dispatch") == 0) {
        if (g_logged_post_dispatch) return;
        g_logged_post_dispatch = 1;
    }

    u32 head     = nds_r32(SCENE_ANCHOR_NDS_ADDR + 0x00);
    u32 tail     = nds_r32(SCENE_ANCHOR_NDS_ADDR + 0x04);
    u16 count    = nds_r16(SCENE_ANCHOR_NDS_ADDR + 0x08);
    u16 link_off = nds_r16(SCENE_ANCHOR_NDS_ADDR + 0x0a);

    fprintf(stderr,
            "[scene-queue] %s anchor=0x%08X head=0x%08X tail=0x%08X "
            "count=%u link_off=0x%x\n",
            tag ? tag : "(no-tag)",
            (unsigned)SCENE_ANCHOR_NDS_ADDR,
            (unsigned)head, (unsigned)tail,
            (unsigned)count, (unsigned)link_off);

    /* Walk up to 4 nodes for visibility. */
    u32 node = head;
    int hop = 0;
    while (node && hop < 4) {
        /* Sanity: NDS addresses must be in [0x02000000, 0x02400000). */
        if (node < 0x02000000u || node >= 0x02400000u) {
            fprintf(stderr,
                    "[scene-queue] %s   node[%d]=0x%08X (OUT OF MAPPED RAM)\n",
                    tag ? tag : "(no-tag)", hop, (unsigned)node);
            break;
        }
        u32 vtable = nds_r32(node + 0x00);
        u32 next   = nds_r32(node + (link_off ? link_off : 0x0c));
        u16 flags  = nds_r16(node + 0x12);
        fprintf(stderr,
                "[scene-queue] %s   node[%d]=0x%08X  vtable=0x%08X  "
                "next=0x%08X  flags=0x%04X\n",
                tag ? tag : "(no-tag)", hop,
                (unsigned)node, (unsigned)vtable,
                (unsigned)next, (unsigned)flags);
        node = next;
        hop++;
    }
    fflush(stderr);
}

/* Synthetic NDS address that maps to host_draw_test_node — populated
 * via host_test_node_register() at startup. The fake node's vtable[2]
 * points here so the trampoline dispatches to host C code. */
#define HOST_TEST_NODE_FNPTR  0xFA000000u

/* Place the fake vtable inside the same 0x40-byte slab. Layout:
 *   FAKE_NODE_NDS_ADDR + 0x00 .. 0x1F  — node fields
 *   FAKE_NODE_NDS_ADDR + 0x20 .. 0x3F  — vtable (4 ptrs)
 * Node.vtable_ptr (+0x00) -> FAKE_NODE_NDS_ADDR + 0x20
 * vtable[2] (+0x28 from node base) -> HOST_TEST_NODE_FNPTR */
#define FAKE_VTABLE_OFFSET   0x20u
#define FAKE_VTABLE_NDS_ADDR (FAKE_NODE_NDS_ADDR + FAKE_VTABLE_OFFSET)

void host_scene_queue_inject_fake(void)
{
    if (g_fake_injected) return;
    if (!getenv("MLPIT_FAKE_QUEUE_NODE")) return;
    if (!nds_arm9_ram_is_mapped()) return;

    g_fake_injected = 1;

    /* Build a zeroed fake node. */
    void *node = (void *)(uintptr_t)FAKE_NODE_NDS_ADDR;
    memset(node, 0, FAKE_NODE_SIZE);

    /* Build an embedded vtable so vtable[2] resolves to our synthetic
     * host trampoline address. The trampoline dispatcher
     * (host_fnptr_resolver) will translate HOST_TEST_NODE_FNPTR ->
     * host_draw_test_node(node, anchor). */
    nds_w32(FAKE_NODE_NDS_ADDR + 0x00, FAKE_VTABLE_NDS_ADDR);  /* node->vtable */
    nds_w32(FAKE_VTABLE_NDS_ADDR + 0x00, 0);
    nds_w32(FAKE_VTABLE_NDS_ADDR + 0x04, 0);
    /* Default vtable[2] = host test trampoline. May be overridden via
     * MLPIT_FAKE_NODE_FN=0xNNNNNNNN (e.g. 0x0200FCB4 to exercise the
     * decompiled state-machine processor). */
    u32 vtbl2_fn = HOST_TEST_NODE_FNPTR;
    {
        const char *override = getenv("MLPIT_FAKE_NODE_FN");
        if (override) {
            vtbl2_fn = (u32)strtoul(override, NULL, 0);
        }
    }
    nds_w32(FAKE_VTABLE_NDS_ADDR + 0x08, vtbl2_fn); /* vtable[2] */
    nds_w32(FAKE_VTABLE_NDS_ADDR + 0x0c, 0);
    nds_w32(FAKE_NODE_NDS_ADDR + 0x0c, 0);   /* next = NULL */

    /* flags bit0 = "needs update" so vtable[2] fires; bit1 (sched)
     * starts clear. The processor's second pass will clear bit0 again
     * after the dispatch — host_scene_queue_rearm_fake() re-sets it
     * every frame so we can observe a continuous tick. */
    nds_w16(FAKE_NODE_NDS_ADDR + 0x12, 0x0001);

    /* Install into the anchor. */
    nds_w32(SCENE_ANCHOR_NDS_ADDR + 0x00, FAKE_NODE_NDS_ADDR);  /* head */
    nds_w32(SCENE_ANCHOR_NDS_ADDR + 0x04, FAKE_NODE_NDS_ADDR);  /* tail */
    nds_w16(SCENE_ANCHOR_NDS_ADDR + 0x08, 1);                   /* count */
    nds_w16(SCENE_ANCHOR_NDS_ADDR + 0x0a, 0x0c);

    fprintf(stderr,
            "[scene-queue] INJECTED fake node at 0x%08X (vtable=0x%08X "
            "vtable[2]=0x%08X) into anchor 0x%08X (count=1, flags=1)\n",
            (unsigned)FAKE_NODE_NDS_ADDR,
            (unsigned)FAKE_VTABLE_NDS_ADDR,
            (unsigned)vtbl2_fn,
            (unsigned)SCENE_ANCHOR_NDS_ADDR);
    fflush(stderr);
}

/* Re-arm the fake node's "needs update" bit so vtable[2] fires every
 * frame (the queue processor's second pass clears bit0 after dispatch).
 * Called from the per-frame tick path in arm9/src/link_stubs.c. */
void host_scene_queue_rearm_fake(void)
{
    if (!g_fake_injected) return;
    if (!nds_arm9_ram_is_mapped()) return;
    u16 flags = nds_r16(FAKE_NODE_NDS_ADDR + 0x12);
    nds_w16(FAKE_NODE_NDS_ADDR + 0x12, (u16)(flags | 1));
}

/* ------------------------------------------------------------------
 * 64-bit-safe replacement for FUN_0202a33c (gx_util.c).
 *
 * The decompiled FUN_0202a33c reads the master-anchor pointer through
 *   extern int *DAT_0202a510;
 *   if (*DAT_0202a510 != 0) ...
 * but pc/src/host_undefined_stubs.c defines DAT_0202a510 as a 4-byte
 * `uint32_t` slot, while the decomp accesses it as an 8-byte pointer.
 * On x86_64 that reads 8 bytes spanning DAT_0202a510 + DAT_0202a514,
 * which forms a bogus 64-bit pointer and segfaults on dereference.
 *
 * We sidestep the problem entirely: read the anchor address straight
 * from the wholesale-initialised arm9.bin slot at NDS 0x0202A510 (which
 * holds 0x02060A04 by construction) and walk through direct NDS
 * pointers into the VirtualAlloc'd 4 MiB ARM9 RAM.
 *
 * Behaviour vs. the original ARM:
 *   - Walks every node via +0xc until null.
 *   - When flags bit 0 is set AND bit 1 is clear AND bit 3 of the
 *     "schedule countdown" sub-field is clear, the original invoked
 *     `(*(u32*)node)[+8]` (vtable[2]).  We CANNOT execute an arbitrary
 *     ARM-domain function pointer here (the vtable lives at an NDS
 *     address but a host call needs a 64-bit host fn ptr), so we log
 *     the request and skip — the bookkeeping bits below still run.
 *   - The flag-mask updates and the second pass (clear-bit-1 / clear
 *     low-6-bits-when-bit-3-clear) are exact ports.
 *
 * No-op when head=NULL (queue empty), so calling this every frame is
 * cheap.
 * ------------------------------------------------------------------ */

#include "arm_compat.h"
#include "host_fnptr_resolver.h"

#define DAT_0202A510_ADDR  0x0202A510u  /* slot holding anchor ptr */

static u32 nds_anchor_addr(void)
{
    if (!nds_arm9_ram_is_mapped()) return 0;
    /* Read the 32-bit slot value from the wholesale-init'd ARM9 RAM. */
    u32 v = *(volatile u32 *)(uintptr_t)DAT_0202A510_ADDR;
    /* Defensive: anchor must point into our 4 MiB mapping. */
    if (v < 0x02000000u || v >= 0x02400000u) return 0;
    return v;
}

static int nds_addr_in_arm9(u32 a)
{
    return a >= 0x02000000u && a < 0x02400000u;
}

void FUN_0202a33c_safe(void)
{
    static int s_skipped_vtable_warn = 0;

    u32 anchor = nds_anchor_addr();
    if (!anchor) return;
    u32 head = *(volatile u32 *)(uintptr_t)(anchor + 0x00);
    if (!nds_addr_in_arm9(head) || head == 0) {
        /* Anchor empty or corrupt — nothing to do. */
        return;
    }

    /* First pass — same outer-do/while bVar2 loop the original used.
     * We bail out after a fixed number of iterations to be safe. */
    int outer_guard = 64;
    int run_again;
    do {
        run_again = 0;
        u32 node = head;
        int hop = 0;
        while (node && hop < 256) {
            if (!nds_addr_in_arm9(node)) break;
            u16 flags = *(volatile u16 *)(uintptr_t)(node + 0x12);

            /* `(u8(flags) << 0x1f) < 0` => bit 0 of flags == 1
             * `-1 < (int)(uVar6 << 0x1e)` => bit 1 of flags == 0  */
            int active   = (flags & 1) != 0;
            int sched    = (flags & 2) != 0;
            if (active && !sched) {
                /* (flags << 0x1a) >> 0x1c == bits[5..2] (signed)  */
                int s_field = (s32)((u32)flags << 0x1a) >> 0x1c;
                if ((s_field & 8) == 0) {
                    /* Counter increment in the +6..+9 nibble.        */
                    u16 counter_nib = (u16)((((u32)flags << 0x16) >> 0x1c) + 1) & 0xf;
                    flags = (u16)((flags & 0xfc3f) | (counter_nib << 6));
                    *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;

                    /* Original ARM: (**(void (**)())(*node + 8))();
                     * Translate the ARM-domain vtable[2] code pointer
                     * to a host fn ptr and dispatch.  Pass (node, anchor)
                     * as the two args because that's the convention
                     * decompiled scene methods expect. */
                    {
                        u32 vtable = *(volatile u32 *)(uintptr_t)node;
                        if (vtable && nds_addr_in_arm9(vtable)) {
                            u32 fn_addr = *(volatile u32 *)(uintptr_t)(vtable + 8);
                            static int s_dispatch_log = 0;
                            if (s_dispatch_log < 100) {
                                s_dispatch_log++;
                                fprintf(stderr,
                                        "[queue-dispatch] #%d node=0x%08X "
                                        "vtable=0x%08X fn=0x%08X "
                                        "flags=0x%04X\n",
                                        s_dispatch_log, (unsigned)node,
                                        (unsigned)vtable,
                                        (unsigned)fn_addr, (unsigned)flags);
                                fflush(stderr);
                            }
                            nds_call_2arg(fn_addr,
                                          (uintptr_t)node,
                                          (uintptr_t)anchor);
                        } else if (vtable >= HOST_FNPTR_SYNTHETIC_BASE) {
                            /* Vtable lives in synthetic host range — read
                             * vtable[2] from a host-side mirror.  Our
                             * fake node embeds its vtable inside the
                             * mapped ARM9 RAM, so this path is rare. */
                            u32 fn_addr = vtable;
                            nds_call_2arg(fn_addr,
                                          (uintptr_t)node,
                                          (uintptr_t)anchor);
                        } else if (!s_skipped_vtable_warn) {
                            s_skipped_vtable_warn = 1;
                            fprintf(stderr,
                                    "[FUN_0202a33c_safe] vtable out of range "
                                    "(node=0x%08X vtable=0x%08X)\n",
                                    (unsigned)node, (unsigned)vtable);
                            fflush(stderr);
                        }
                    }

                    int counter_sf = (s32)((u32)flags << 0x16) >> 0x1c;
                    if (counter_sf < s_field) {
                        run_again = 1;
                    } else {
                        flags |= 2;
                        *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;
                    }
                } else {
                    /* sched-counter path (s_field bit 3 set) */
                    u16 counter_nib = (u16)((((u32)flags << 0x16) >> 0x1c) + 1) & 0xf;
                    flags = (u16)((flags & 0xfc3f) | (counter_nib << 6));
                    *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;

                    int counter_sf = (s32)((u32)flags << 0x16) >> 0x1c;
                    if (-s_field <= counter_sf) {
                        u32 vtable = *(volatile u32 *)(uintptr_t)node;
                        if (vtable && nds_addr_in_arm9(vtable)) {
                            u32 fn_addr = *(volatile u32 *)(uintptr_t)(vtable + 8);
                            nds_call_2arg(fn_addr,
                                          (uintptr_t)node,
                                          (uintptr_t)anchor);
                        } else if (vtable >= HOST_FNPTR_SYNTHETIC_BASE) {
                            nds_call_2arg(vtable,
                                          (uintptr_t)node,
                                          (uintptr_t)anchor);
                        }
                        flags &= 0xfc3f;
                        *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;
                    }
                    flags |= 2;
                    *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;
                }
            }

            /* Walk to next node via +0xc. */
            u32 next = *(volatile u32 *)(uintptr_t)(node + 0x0c);
            if (next == node) break;  /* self-loop guard */
            node = next;
            hop++;
        }
    } while (run_again && --outer_guard > 0);

    /* Second pass: clear bit 1 of flags on every node, plus low-6 bits
     * when s_field bit 3 is clear. */
    {
        u32 node = head;
        int hop = 0;
        while (node && hop < 256) {
            if (!nds_addr_in_arm9(node)) break;
            u16 flags = *(volatile u16 *)(uintptr_t)(node + 0x12);
            flags &= 0xfffd;  /* clear bit 1 */
            int s_field = (s32)((u32)flags << 0x1a) >> 0x1c;
            if ((s_field & 8) == 0) {
                flags &= 0xfc3f;
            }
            *(volatile u16 *)(uintptr_t)(node + 0x12) = flags;
            u32 next = *(volatile u32 *)(uintptr_t)(node + 0x0c);
            if (next == node) break;
            node = next;
            hop++;
        }
    }
}

/* ================================================================
 * Secondary scene queue at *(0x02060A08)
 *
 * The NDS engine has TWO parallel scene queues:
 *   - Main queue: head at *(0x02060A04), dispatched by FUN_0202A33C
 *   - Secondary queue: head at *(0x02060A08), dispatched by FUN_02029EC4
 *
 * The title screen's sub-object (vtable 0x02077EB0, tick at 0x02077A88)
 * is inserted into the secondary queue.  The title screen's callback
 * state machine (FUN_020768F0) is registered via FUN_0206621C and gets
 * dispatched by the animation system called from the derived tick.
 * ================================================================ */

#define SECONDARY_LIST_HEAD  0x02060A08u
#define SECONDARY_CURR_SAVE  0x020609FCu
#define SECONDARY_NEXT_SAVE  0x02060A00u
#define SECONDARY_NODE_COUNT 0x020609ECu
#define SECONDARY_BASE_VTABLE 0x020597B4u

extern void nds_call_2arg(uint32_t nds_addr, uintptr_t a, uintptr_t b);

/* FUN_0202A20C — Insert node into secondary priority-sorted linked list.
 * ARM signature: (R0=node, R1=priority, R2=unused, R3=parent). */
void FUN_0202a20c(u32 node, u8 priority, u32 r2, u32 parent)
{
    (void)r2;
    if (!nds_arm9_ram_is_mapped()) return;
    if (!nds_addr_in_arm9(node)) return;

    /* Set base vtable (overwritten by derived ctor afterward). */
    nds_w32(node + 0x00, SECONDARY_BASE_VTABLE);

    /* Insert into doubly-linked list at *(SECONDARY_LIST_HEAD).
     * Simplified: insert at HEAD (original does priority-sorted). */
    u32 old_head = nds_r32(SECONDARY_LIST_HEAD);

    nds_w32(node + 0x08, 0);          /* prev = NULL (new head) */
    nds_w32(node + 0x0C, old_head);   /* next = old head */
    if (old_head && nds_addr_in_arm9(old_head)) {
        nds_w32(old_head + 0x08, node);  /* old_head.prev = node */
    }
    nds_w32(SECONDARY_LIST_HEAD, node);

    /* Set node fields. */
    nds_w32(node + 0x04, parent);
    *(volatile u8 *)(uintptr_t)(node + 0x10) = 0;           /* state */
    *(volatile u8 *)(uintptr_t)(node + 0x11) = priority;    /* priority */
    u8 flags = *(volatile u8 *)(uintptr_t)(node + 0x12);
    flags = (flags & ~1u) | 2u;  /* active=0, sched=1 */
    *(volatile u8 *)(uintptr_t)(node + 0x12) = flags;
    nds_w32(node + 0x14, 0);                                /* tick counter */

    /* Increment global node count. */
    u8 cnt = *(volatile u8 *)(uintptr_t)SECONDARY_NODE_COUNT;
    *(volatile u8 *)(uintptr_t)SECONDARY_NODE_COUNT = cnt + 1;

    fprintf(stderr,
            "[FUN_0202a20c] node=0x%08X prio=%u parent=0x%08X head=0x%08X\n",
            (unsigned)node, (unsigned)priority,
            (unsigned)parent, (unsigned)nds_r32(SECONDARY_LIST_HEAD));
    fflush(stderr);
}

/* FUN_02029FFC — Activate a scene node (set active + sched flags).
 * On ARM: locks mutex, sets flag bits, unlocks.  We skip the mutex. */
void FUN_02029ffc(u32 node)
{
    if (!nds_arm9_ram_is_mapped()) return;
    if (!nds_addr_in_arm9(node)) return;

    u8 *flags = (u8 *)(uintptr_t)(node + 0x12);
    *flags = (*flags & 0xFEu) | 0x01u;   /* set active */
    *flags = *flags | 0x02u;              /* set sched */

    static int s_log = 0;
    if (s_log < 5) {
        s_log++;
        fprintf(stderr,
                "[FUN_02029ffc] activate node=0x%08X flags=0x%02X\n",
                (unsigned)node, (unsigned)*flags);
        fflush(stderr);
    }
}

/* FUN_02029EC4 — Secondary queue dispatch.
 * Walks the secondary list, calls vtable[2] for active && !sched nodes,
 * increments tick counter, then clears sched flags.
 * HOST_PORT version: uses nds_call_2arg for virtual dispatch. */
void FUN_02029ec4(void)
{
    if (!nds_arm9_ram_is_mapped()) return;

    u32 head = nds_r32(SECONDARY_LIST_HEAD);
    if (!head || !nds_addr_in_arm9(head)) return;

    /* First pass: dispatch active && !sched nodes. */
    u32 cur = head;
    int hop = 0;
    while (cur && nds_addr_in_arm9(cur) && hop < 256) {
        u32 next = nds_r32(cur + 0x0C);   /* save next before dispatch */

        u8 flags = *(volatile u8 *)(uintptr_t)(cur + 0x12);
        int active = (flags & 1) != 0;
        int sched  = (flags & 2) != 0;

        if (active && !sched) {
            u32 vtable = nds_r32(cur);
            if (vtable && nds_addr_in_arm9(vtable)) {
                u32 fn_addr = nds_r32(vtable + 8);

                static int s_disp_log = 0;
                if (s_disp_log < 20) {
                    s_disp_log++;
                    fprintf(stderr,
                            "[sec-dispatch] #%d node=0x%08X vtable=0x%08X "
                            "fn=0x%08X flags=0x%02X\n",
                            s_disp_log, (unsigned)cur, (unsigned)vtable,
                            (unsigned)fn_addr, (unsigned)flags);
                    fflush(stderr);
                }

                nds_call_2arg(fn_addr, (uintptr_t)cur, 0);
            }

            /* Post-dispatch: increment tick counter, set sched. */
            u32 tick = nds_r32(cur + 0x14);
            nds_w32(cur + 0x14, tick + 1);
            flags = *(volatile u8 *)(uintptr_t)(cur + 0x12);
            *(volatile u8 *)(uintptr_t)(cur + 0x12) = flags | 2u;
        }

        if (next == cur) break;  /* self-loop guard */
        cur = next;
        hop++;
    }

    /* Second pass: clear sched flag on all nodes. */
    cur = nds_r32(SECONDARY_LIST_HEAD);
    hop = 0;
    while (cur && nds_addr_in_arm9(cur) && hop < 256) {
        u8 flags = *(volatile u8 *)(uintptr_t)(cur + 0x12);
        *(volatile u8 *)(uintptr_t)(cur + 0x12) = flags & ~2u;
        u32 next = nds_r32(cur + 0x0C);
        if (next == cur) break;
        cur = next;
        hop++;
    }
}

/* FUN_02029F90 — Mark/schedule secondary queue nodes.
 * For each node with priority != 0 && != 0xFF: clear active, set sched. */
void FUN_02029f90(void)
{
    if (!nds_arm9_ram_is_mapped()) return;

    u32 node = nds_r32(SECONDARY_LIST_HEAD);
    int hop = 0;
    while (node && nds_addr_in_arm9(node) && hop < 256) {
        u8 priority = *(volatile u8 *)(uintptr_t)(node + 0x11);
        if (priority != 0 && priority != 0xFF) {
            u8 flags = *(volatile u8 *)(uintptr_t)(node + 0x12);
            flags &= ~1u;   /* clear active */
            flags |= 2u;    /* set sched */
            *(volatile u8 *)(uintptr_t)(node + 0x12) = flags;
        }
        u32 next = nds_r32(node + 0x0C);
        if (next == node) break;
        node = next;
        hop++;
    }
}
