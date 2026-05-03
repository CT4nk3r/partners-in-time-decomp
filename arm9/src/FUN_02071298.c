/*
 * FUN_02071298.c — host stub for overlay-6 cell→OAM emit primitive.
 *
 * Original (overlay 6, 0x02071298, ~360 bytes):
 *   - Reads cell descriptor at r0 (offsets +0x0C/+0x10 = X/Y, +0x24 =
 *     enable flag, +0x40 = OBJ-attr template, +0x70 = palette, +0x64
 *     = priority).
 *   - Walks visible OBJ entries, computes screen-space coords, emits
 *     8 stores into shadow OAM @ 0x020603C0+i*8 (sub-OAM slot 0).
 *   - 8 stores: attr0/attr1/attr2 across two halves, plus extension
 *     bytes for rotation/scale.
 *
 * Host stub: NULL-guard the cell pointer.  We don't have a synthetic
 * cell descriptor wired up, so a real call would NPE on the +0x24
 * field load.  Returning early is benign.
 *
 * Decompiling this function fully requires reverse-engineering the
 * cell-descriptor struct layout (used across overlays 6/7/8/9), which
 * is several sessions of work.  For now we just let the trampoline
 * resolve to a real C symbol matching the overlay-baked address, so
 * indirect calls don't fall through to the [stub] no-op.
 */

#include "nds_platform.h"
#include <stdint.h>
#include <stdio.h>

void FUN_02071298(void *cell)
{
    static uint64_t s_calls = 0;
    s_calls++;
    if ((s_calls & 0xFF) == 1) {
        fprintf(stderr, "[render] FUN_02071298 (cell→OAM) cell=%p calls=%llu\n",
                cell, (unsigned long long)s_calls);
    }
    if (!cell) return;
    /* Out of caution: read the +0x24 enable flag, bail if unset.  This
     * matches the first dynamic check in the real function and avoids
     * walking further offsets if the cell is in an inactive state. */
    uint8_t enabled = *((uint8_t *)cell + 0x24);
    if (!enabled) return;
    /* TODO: emit 8 stores into 0x020603C0 (sub-OAM slot 0).  Without
     * a verified cell-descriptor layout, we leave this empty. */
}
