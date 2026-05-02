/*
 * FUN_02065A10.c — host C stub for overlay-0 per-frame "draw scene".
 *
 * Original (overlay 0, 0x02065A10..0x02065D14, ~770 bytes):
 *
 *   void FUN_02065A10(scene *self) {        // r0=self in r6
 *       *(u16*)0x020607AC.field20 high-bit gate — bail if not set
 *       sub = *(self + 4);                  // r5
 *       if (sub->kind > 2) goto end_upload;
 *       if (self->flags & 1) {
 *           // Re-allocate ~12 KiB of GFX VRAM, copy bg/obj tile data
 *           // into 0x05000400/0x05000600/0x05000800/0x05000A00 etc.
 *           // (lots of FUN_0202CC94 / FUN_0202CD68 cache-flush+memcpy
 *           //  helpers, plus FUN_0203588C / FUN_02035EF8 / FUN_02035868
 *           //  / FUN_02035974 / FUN_020357F0 / FUN_020358E8 GXFIFO
 *           //  command emitters.)
 *           self->flags &= ~1;
 *       }
 *   end_upload:
 *       FUN_02065F40(sub);                  // entity prep
 *       for (i = 0; i < 2; i++) {
 *           fn = ((void(**)(void*))sub)[0x3b0/4 + i];
 *           if (fn) FUN_0206E06C(entity);
 *       }
 *       r0 = 3;                              // gx engine id
 *       FUN_02029518();                      // OAM upload (DMA shadow→OAM)
 *       FUN_020294b0();                      // finalize/swap
 *   }
 *
 * Host port limitations:
 *   - We have no faithful 'scene' struct; offsets +4/+10/+28/+3b0 in r5/r6
 *     reference uninitialised host BSS.  Dereferencing them would NPE.
 *   - The interesting GXFIFO emitters (FUN_0202CD68 etc.) are not yet
 *     decompiled — they would just stub-no-op and the cache-flush+memcpy
 *     would still target real arm9 RAM via raw pointer literals.
 *
 * What we DO do:
 *   1. Validate the scene pointer (NULL-guard).
 *   2. Log entry once per N frames.
 *   3. Call FUN_02029518 + FUN_020294b0.  These are themselves not yet
 *      decompiled (they live as stubs) — but the host shadow-OAM upload
 *      runs separately every frame in render_frame(), so the OAM
 *      pipeline is already wired regardless.
 *
 * Result: providing this C body makes the FUN_02065A10 trampoline
 *  resolve to a real (non-stub) host function, matching the address
 *  baked into overlay-0 vtable @ 0x020BF150 slot 2.  Indirect calls
 *  through that vtable from any decomp path that is reached will land
 *  here instead of the [stub] no-op in host_undefined_stubs.c.
 */

#include "nds_platform.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int FUN_02029518(void);
extern int FUN_020294b0(void);
extern int FUN_0206e06c(void *entity);
extern int FUN_02065f40(void *self);

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

    /* Without a faithful scene*, dereferencing scene->sub@+4 etc. would
     * walk wild pointers.  Skip the entity loop and the GFX VRAM repop
     * branch — those are observable downstream effects, not on the
     * critical OAM-upload path that produces the visual signal. */
    if (!scene) {
        /* Still run the OAM upload tail — that's the data we care
         * about ending up on hardware. */
        (void)FUN_02029518();
        (void)FUN_020294b0();
        return;
    }

    /* Best-effort entity loop, with NULL fn-ptr guards.  We assume
     * scene->sub is at offset +4 and the per-entity render fn-ptr
     * table starts at sub+0x3B0 (matching the disassembly). */
    void *sub = *(void **)((uint8_t *)scene + 4);
    if (sub) {
        (void)FUN_02065f40(sub);
        for (int i = 0; i < 2; i++) {
            void **slot = (void **)((uint8_t *)sub + 0x3B0 + (uintptr_t)i * 4);
            if (*slot) {
                (void)FUN_0206e06c(slot);
            }
        }
    }

    (void)FUN_02029518();
    (void)FUN_020294b0();
}
