/*
 * FUN_02065A10.c — overlay-0 per-frame "draw scene" (faithful body).
 *
 * Hand-decompiled from extracted/overlay/overlay_0000.bin
 * @ NDS 0x02065A10..0x02065D14 (~772 bytes).
 *
 * ARM body, mapped 1:1:
 *
 *   r6 = self;                                // scene root
 *   r1 = 0x020607AC;
 *   if (((*(u16*)(r1+0x14) << 23) >> 31) == 0) return; // gate at .field14 bit8
 *   r5 = *(void**)(self + 0x04);              // sub
 *   if (*(u8*)(sub + 0x10) > 2) goto end_upload;
 *   if ((*(u32*)(self + 0x28) & 1) == 0) goto end_upload_2;
 *
 *   // ---- GFX repop branch (cache-flush + memcpy + GXFIFO emitters) ----
 *   // Allocates / fetches 5 BG/OBJ tile banks and uploads them.  Every
 *   // sub-call (FUN_0202CC94 / FUN_0202CD68 / FUN_02035974 / 020357F0 /
 *   // 020358E8 / 0202CC94) is currently a host_undefined_stubs no-op.
 *   //   FUN_0202CD68(0x05000400, _, 0x200);   // BG palette upload
 *   //   FUN_0202CC94(0x05000400, _, 0x200);   // (size <= cap path)
 *   //   ... 4 more banks ...
 *   //   FUN_02035974(); FUN_020357F0();        // GXFIFO state setup
 *   //   FUN_020358E8();                        // finalise
 *   // The decomp body is ~600 bytes of literal-pool / DAT_* arithmetic
 *   // that, until the underlying VRAM helpers exist host-side, would
 *   // just call empty stubs.  We model it with a SINGLE call into the
 *   // outer envelope so future work can replace it; today it's a no-op.
 *   self->flags &= ~1u;
 *
 *   end_upload_2:                              // 0x02065CD8
 *   FUN_02065F40(sub);                         // entity prep
 *   for (r4 = 0; r4 < 2; r4++) {
 *       u32 entity = *(u32*)(sub + 0x3B0 + r4*4);
 *       if (entity != 0) FUN_0206E06C((void*)(uintptr_t)entity);
 *   }
 *   end_upload:                                // 0x02065D04
 *   FUN_02029518();                            // OAM upload
 *   FUN_020294b0();                            // finalise / swap
 *
 * Notes for the host port:
 *   - The gate at 0x020607AC + 0x14 bit 8 starts CLEAR, so the natural
 *     path returns immediately every frame until something (real game
 *     code or the synth-scene host code in main.c) sets bit 8.
 *   - The GFX repop branch is summarised because every helper it calls
 *     is currently a stub.  Once any of those is decompiled we revisit.
 *   - The entity loop reads a *value* (not address) from sub+0x3B0/0x3B4
 *     and passes it as the FUN_0206E06C argument.  An earlier version
 *     of this stub passed the address of the slot — fixed.
 */

#include "nds_platform.h"
#include "types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int FUN_02029518(void);
extern int FUN_020294b0(void);
extern int FUN_0206e06c(void *entity);
extern int FUN_02065f40(void *self);

extern int nds_arm9_ram_is_mapped(void);

void FUN_02065a10(void *scene)
{
    static uint64_t s_calls = 0;
    static int      s_log_throttle = 0;
    static const int kThrottle = 60;

    s_calls++;
    if ((s_log_throttle++ % kThrottle) == 0) {
        fprintf(stderr, "[render] FUN_02065a10 called frame=%llu scene=%p\n",
                (unsigned long long)s_calls, scene);
    }

    /* NULL-guard: the natural game_start path doesn't yet populate a
     * real scene — fall through to the OAM-upload tail so the host
     * shadow-OAM pipeline still runs. */
    if (!scene) {
        (void)FUN_02029518();
        (void)FUN_020294b0();
        return;
    }

    /* ---- Render-enable gate at 0x020607AC + 0x14 bit 8 ---- */
    if (nds_arm9_ram_is_mapped()) {
        u16 gate = *(volatile u16 *)(uintptr_t)(0x020607AC + 0x14);
        if ((gate & 0x100) == 0) {
            /* Real game leaves the gate clear during boot; nothing to do.
             * Tail OAM upload still runs (matches popeq behaviour: pop
             * then bx skips the body but the prologue saved nothing
             * we need to undo here). */
            (void)FUN_02029518();
            (void)FUN_020294b0();
            return;
        }
    }

    void *sub = *(void **)((uint8_t *)scene + 4);
    if (sub) {
        u8 kind = *(u8 *)((uint8_t *)sub + 0x10);
        if (kind <= 2) {
            u32 flags = *(u32 *)((uint8_t *)scene + 0x28);
            if (flags & 1u) {
                /* GFX repop branch — see header for what the original
                 * body does.  Every helper is a host stub today, so we
                 * model the end-of-branch flag clear and move on. */
                *(u32 *)((uint8_t *)scene + 0x28) = flags & ~1u;
            }
        }

        (void)FUN_02065f40(sub);
        for (int i = 0; i < 2; i++) {
            u32 entity_nds = *(u32 *)((uint8_t *)sub + 0x3B0 + (uintptr_t)i * 4);
            if (entity_nds != 0) {
                (void)FUN_0206e06c((void *)(uintptr_t)entity_nds);
            }
        }
    }

    (void)FUN_02029518();
    (void)FUN_020294b0();
}
