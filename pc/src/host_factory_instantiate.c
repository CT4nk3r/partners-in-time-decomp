/*
 * host_factory_instantiate.c — HOST_PORT manual instantiation of a real
 * scene-class node from the 53-entry vtable @0x02050B6C.
 *
 * Background: the three constructors FUN_0200c58c / c5c8 / c5fc that
 * build instances of the vtable@0x02050B6C class have ZERO direct
 * callers in arm9.bin or any overlay. They are entered via a runtime-
 * built object factory we have not yet replicated. So nothing real ever
 * gets enqueued into the scene processor's master queue (anchor at
 * NDS 0x02060A04, drained per frame by FUN_0202a33c which dispatches
 * vtable[2]).
 *
 * To unblock organic graphics work this file:
 *   1. Allocates a 0x200-byte slab inside the host-only NDS RAM region
 *      (0x02300200, well above arm9.bin's 0x0205C000 high water mark).
 *   2. Calls FUN_0200c58c on it (writes vtable, runs vt[48], runs the
 *      base/secondary inits — all dispatched through the host fnptr
 *      resolver so any NDS-domain function pointer is either a no-op
 *      or a decompiled C body).
 *   3. Installs the slab as the head of both queue anchors:
 *        - 0x02060A04: master scene queue (FUN_0202a33c walks this and
 *          calls vtable[2] every frame on each active node).
 *        - 0x0202A568: secondary "gx" queue head pointer slot used by
 *          FUN_0202a51c per the prior session's analysis.
 *   4. Sets node.flags bit 0 ("needs update") so the queue processor
 *      actually fires vtable[2] on it.
 *
 * Gated by MLPIT_INSTANTIATE_REAL=1.  Mutually exclusive with
 * MLPIT_FAKE_QUEUE_NODE — the synthetic test node would otherwise stomp
 * the same anchor.
 */

#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int   nds_arm9_ram_is_mapped(void);
extern void *FUN_0200c58c(void *self);

/* Slab address chosen to be well past the 4 MiB ARM9 RAM contents
 * actually populated from arm9.bin (0x0205C000 high water mark) and
 * outside the test-node region at 0x02300100. */
#define INSTANCE_NDS_ADDR        0x02300200u
#define INSTANCE_SIZE            0x200u

/* Master scene queue anchor + per-node link offset. */
#define SCENE_ANCHOR_NDS_ADDR    0x02060A04u

/* Per-node fields (see host_scene_queue.c for layout). */
#define NODE_LINK_OFFSET         0x0Cu
#define NODE_FLAGS_OFFSET        0x12u

/* Secondary head ptr slot (per task description). */
#define DAT_0202A568_ADDR        0x0202A568u

static int g_instantiated = 0;

void host_factory_instantiate(void)
{
    if (g_instantiated) return;
    if (!getenv("MLPIT_INSTANTIATE_REAL")) return;
    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr, "[factory] arm9 RAM not mapped — skipping\n");
        return;
    }
    if (getenv("MLPIT_FAKE_QUEUE_NODE")) {
        fprintf(stderr, "[factory] MLPIT_FAKE_QUEUE_NODE also set — "
                "fake test node already owns the anchor; refusing to "
                "double-install. Unset one of them.\n");
        return;
    }

    g_instantiated = 1;

    fprintf(stderr,
            "[factory] manually instantiating vtable@0x02050B6C class "
            "(FUN_0200c58c) at NDS 0x%08X (size 0x%X)\n",
            (unsigned)INSTANCE_NDS_ADDR, (unsigned)INSTANCE_SIZE);
    fflush(stderr);

    void *self = (void *)(uintptr_t)INSTANCE_NDS_ADDR;
    memset(self, 0, INSTANCE_SIZE);

    /* Run the constructor through the resolver-protected wrapper. */
    (void)FUN_0200c58c(self);

    /* link/next pointers explicitly NULL (already zero from memset, but
     * be explicit so the layout is documented at the call site). */
    *(volatile uint32_t *)(uintptr_t)(INSTANCE_NDS_ADDR + NODE_LINK_OFFSET) = 0;

    /* flags bit 0 = "needs update" -> vtable[2] fires every frame. */
    *(volatile uint16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + NODE_FLAGS_OFFSET) = 0x0001;

    /* Install as the master scene queue head/tail. */
    *(volatile uint32_t *)(uintptr_t)(SCENE_ANCHOR_NDS_ADDR + 0x00) = INSTANCE_NDS_ADDR;
    *(volatile uint32_t *)(uintptr_t)(SCENE_ANCHOR_NDS_ADDR + 0x04) = INSTANCE_NDS_ADDR;
    *(volatile uint16_t *)(uintptr_t)(SCENE_ANCHOR_NDS_ADDR + 0x08) = 1;
    *(volatile uint16_t *)(uintptr_t)(SCENE_ANCHOR_NDS_ADDR + 0x0A) = NODE_LINK_OFFSET;

    /* Per-task: also write the secondary gx-queue head pointer. */
    *(volatile uint32_t *)(uintptr_t)DAT_0202A568_ADDR = INSTANCE_NDS_ADDR;

    {
        uint32_t vt    = *(volatile uint32_t *)(uintptr_t)INSTANCE_NDS_ADDR;
        uint16_t flags = *(volatile uint16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + NODE_FLAGS_OFFSET);
        fprintf(stderr,
                "[factory] node installed: addr=0x%08X vtable=0x%08X "
                "flags=0x%04X anchor.head=0x%08X DAT_0202A568=0x%08X\n",
                (unsigned)INSTANCE_NDS_ADDR, (unsigned)vt, (unsigned)flags,
                (unsigned)*(volatile uint32_t *)(uintptr_t)SCENE_ANCHOR_NDS_ADDR,
                (unsigned)*(volatile uint32_t *)(uintptr_t)DAT_0202A568_ADDR);
        fflush(stderr);
    }
}

/* Re-arm the "needs update" bit each frame so vtable[2] keeps firing —
 * the queue processor's second pass clears bit 0 after dispatch.
 * Wired into the same per-frame tick path used by host_scene_queue_rearm_fake. */
void host_factory_rearm(void)
{
    if (!g_instantiated) return;
    if (!nds_arm9_ram_is_mapped()) return;
    volatile uint16_t *p =
        (volatile uint16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + NODE_FLAGS_OFFSET);
    *p = (uint16_t)(*p | 1);
}
