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
 * outside the test-node region at 0x02300100.
 *
 * INSTANCE_SIZE bumped from 0x200 to 0x400 because FUN_0200fcb4 reads
 * fields up to (param_1 + 0x170) — see .session-work/fcb4_field_reads.txt. */
#define INSTANCE_NDS_ADDR        0x02300200u
#define INSTANCE_SIZE            0x400u

/* Auxiliary scratch buffers (zero-init backing for pointer fields in
 * the entity that FUN_0200fcb4 dereferences). All live in the host-only
 * NDS region 0x02300xxx so they don't collide with arm9.bin contents. */
#define AUX_SPRITE_LIST_NDS      0x02300A00u  /* +0x38: sprite-list head  */
#define AUX_SPRITE_LIST_SIZE     0x40u
#define AUX_FRAME_TABLE_NDS      0x02300A80u  /* +0x148: frame-table base */
#define AUX_FRAME_TABLE_SIZE     0x100u       /*  16-byte entries x 16    */
#define AUX_TEXIMAGE_NDS         0x02300B80u  /* +0x130: texture image    */
#define AUX_TEXIMAGE_SIZE        0x80u
#define AUX_LUT_NDS              0x02300C00u  /* +0x168: LUT-write target */
#define AUX_LUT_SIZE             0x40u
#define AUX_MTX_NDS              0x02300C40u  /* +0x170: matrix/coeff ptr */
#define AUX_MTX_SIZE             0x40u

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

    /* ─────────── populate fields FUN_0200fcb4 reads ───────────
     * Per .session-work/fcb4_field_reads.txt:
     *   +0x07c flag word: bit 0x100 must be set or the function bails.
     *   +0x100+0x62 (=0x162) low 5 bits: must be non-zero or 2nd early-out.
     *   +0x100+0x64 (=0x164) palette index.
     *   +0x100+0x6c..6e (s16 X,Y position)
     *   +0x038 sprite-list head ptr — dereffed (ldrh r1,[r1])
     *   +0x148 frame-table base — frame-idx<<4 added, dereffed
     *   +0x130 texture image base ptr
     *   +0x168 LUT-write target ptr
     *   +0x170 matrix / sin-cos coeff ptr
     *   +0x054 / +0x056 s16 scale X / Y (must be non-zero for a quad)
     *   +0x060 frame index (u16) — used as table[idx<<4] lookup
     *   +0x15f priority/blend mode (u8)
     */
    {
        volatile uint32_t *e32 = (volatile uint32_t *)(uintptr_t)INSTANCE_NDS_ADDR;
        volatile uint16_t *e16 = (volatile uint16_t *)(uintptr_t)INSTANCE_NDS_ADDR;
        volatile uint8_t  *e8  = (volatile uint8_t  *)(uintptr_t)INSTANCE_NDS_ADDR;

        /* Zero the auxiliary regions first. */
        memset((void *)(uintptr_t)AUX_SPRITE_LIST_NDS, 0, AUX_SPRITE_LIST_SIZE);
        memset((void *)(uintptr_t)AUX_FRAME_TABLE_NDS, 0, AUX_FRAME_TABLE_SIZE);
        memset((void *)(uintptr_t)AUX_TEXIMAGE_NDS,    0, AUX_TEXIMAGE_SIZE);
        memset((void *)(uintptr_t)AUX_LUT_NDS,         0, AUX_LUT_SIZE);
        memset((void *)(uintptr_t)AUX_MTX_NDS,         0, AUX_MTX_SIZE);

        /* +0x38 sprite list head -> 16-bit u16 word at [head] read */
        e32[0x38/4] = AUX_SPRITE_LIST_NDS;
        *(volatile uint16_t *)(uintptr_t)AUX_SPRITE_LIST_NDS = 0x0001; /* non-null frame data */

        /* +0x3c, +0x40 secondary sprite-list ptrs (just point at same buf) */
        e32[0x3c/4] = AUX_SPRITE_LIST_NDS;
        e32[0x40/4] = AUX_SPRITE_LIST_NDS;

        /* +0x44, +0x48, +0x4c animation/state ptrs (zero-fill is fine; they
         * are only loaded — caller checks them before use). */

        /* +0x54, +0x56 scale (1.0 in 4.12 fixed = 0x1000) */
        *(volatile int16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x54) = 0x1000;
        *(volatile int16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x56) = 0x1000;

        /* +0x60 frame index = 0 (first entry of frame-table) */
        e16[0x60/2] = 0;

        /* +0x7c FLAG WORD: set bit 0x100 (ACTIVE) so first early-out skipped */
        e32[0x7c/4] |= 0x100u;

        /* +0x130 texture image base */
        e32[0x130/4] = AUX_TEXIMAGE_NDS;

        /* +0x148 frame-table base — entry 0 lives at base+0 (16 bytes) */
        e32[0x148/4] = AUX_FRAME_TABLE_NDS;

        /* +0x15f priority/blend mode (0 = normal) */
        e8[0x15f] = 0;

        /* +0x162 (param_1+0x100+0x62): low 5 bits must be non-zero
         *        for the second early-out (lsl#27 / lsrs#27) to fall through.
         * +0x164 palette index. */
        *(volatile uint16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x162) = 0x0001;
        *(volatile uint16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x164) = 0x0000;

        /* +0x168 LUT-write target */
        e32[0x168/4] = AUX_LUT_NDS;

        /* +0x16c, +0x16e position (centre of screen-ish: 128, 96 in pixels) */
        *(volatile int16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x16c) = 128;
        *(volatile int16_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x16e) = 96;

        /* +0x170 matrix / sin-cos coeff ptr */
        e32[0x170/4] = AUX_MTX_NDS;
    }

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
        uint32_t f7c   = *(volatile uint32_t *)(uintptr_t)(INSTANCE_NDS_ADDR + 0x7c);
        fprintf(stderr,
                "[factory] node installed: addr=0x%08X vtable=0x%08X "
                "flags=0x%04X +0x7c=0x%08X anchor.head=0x%08X DAT_0202A568=0x%08X\n",
                (unsigned)INSTANCE_NDS_ADDR, (unsigned)vt, (unsigned)flags, (unsigned)f7c,
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
