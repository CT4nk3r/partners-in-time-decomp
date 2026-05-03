/*
 * host_test_node.c — synthetic scene-queue node used to prove the
 * NDS→host trampoline path end-to-end.
 *
 * Registered under HOST_FNPTR_SYNTHETIC_BASE+0 so the queue processor
 * can dispatch to it via the same nds_call_2arg() path it uses for real
 * decompiled vtable entries. host_draw_test_node writes a recognisable
 * pattern (an animated diagonal stripe) into the top-screen BG VRAM
 * region, which the BG renderer paints to the framebuffer. If we see
 * the stripe move, the full pipeline is alive:
 *   queue processor -> trampoline -> host C -> VRAM -> renderer -> screen.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "host_fnptr_resolver.h"

extern int   nds_arm9_ram_is_mapped(void);
extern void *nds_vram_bank(char bank);
extern uint32_t nds_vram_bank_size(char bank);
extern void *nds_oam_ram(int is_sub);

#define BG_VRAM_TOP_BASE   0x06000000u  /* main engine BG, 512 KiB window */
#define TEST_TILE_BYTES    0x80         /* 4bpp 8x8 tile = 32 bytes; 4 tiles */

static uint32_t g_tick = 0;

/*
 * host_draw_test_node — vtable[2] target for the synthetic test node.
 *
 * Signature follows the ARM convention: (node_self, anchor_or_ctx).
 * We only use the call as a "tick" — write a moving 16-byte stripe
 * into a fixed VRAM offset.
 */
void host_draw_test_node(uintptr_t node_self, uintptr_t anchor)
{
    (void)anchor;
    if (!nds_arm9_ram_is_mapped()) return;

    /* Bank A is the default top-screen BG VRAM provider (0x06000000+). */
    void *dst = nds_vram_bank('A');
    uint32_t cap = nds_vram_bank_size('A');
    if (!dst || cap < 0x100) {
        static int warned = 0;
        if (!warned) {
            warned = 1;
            fprintf(stderr, "[host_draw_test_node] VRAM bank A unavailable\n");
        }
        return;
    }

    g_tick++;
    /* Write a recognisable, slowly-moving 64-byte stripe.  The pattern is
     * 0x10..0x4F bytes laid down at an offset that scrolls one byte per
     * call.  With one call per frame this gives a clear visual delta the
     * VRAM watcher and screenshot diff will both pick up. */
    uint8_t *p = (uint8_t *)dst;
    uint32_t off = (g_tick & 0xff);
    for (uint32_t i = 0; i < 0x40; ++i) {
        p[(off + i) & 0xff] = (uint8_t)(0x10 + (i & 0x3f));
    }

    if ((g_tick & 0x3f) == 1) {
        fprintf(stderr,
                "[host_draw_test_node] tick=%u node=0x%lx wrote stripe @0x%08x+0x%02x\n",
                (unsigned)g_tick, (unsigned long)node_self,
                (unsigned)BG_VRAM_TOP_BASE, (unsigned)off);
        fflush(stderr);
    }

    /* Task 6: also populate OAM[0] so obj_render() draws a placeholder
     * box.  Sprite is 16x16, position scrolls with g_tick so motion is
     * visible.  attr0: y, shape=square; attr1: x, size=1 (16x16);
     * attr2: tile id (any non-zero so the entry is "visible"). */
    {
        uint8_t *oam = (uint8_t *)nds_oam_ram(0);
        if (oam) {
            int x = 32 + ((g_tick / 4) & 0x7f);
            int y = 64 + ((g_tick / 8) & 0x3f);
            uint16_t attr0 = (uint16_t)(y & 0xff) | (0u << 14);   /* shape=square */
            uint16_t attr1 = (uint16_t)(x & 0x1ff) | (1u << 14);  /* size=1 -> 16x16 */
            uint16_t attr2 = 0x0001;                              /* tile id 1 */
            oam[0] = (uint8_t)attr0; oam[1] = (uint8_t)(attr0 >> 8);
            oam[2] = (uint8_t)attr1; oam[3] = (uint8_t)(attr1 >> 8);
            oam[4] = (uint8_t)attr2; oam[5] = (uint8_t)(attr2 >> 8);
            oam[6] = 0; oam[7] = 0;
        }
    }
}

/* Register under the synthetic NDS address so vtables can point at it. */
void host_test_node_register(void)
{
    host_fnptr_register(HOST_FNPTR_SYNTHETIC_BASE + 0u,
                        (void *)host_draw_test_node);
}

uint32_t host_test_node_nds_addr(void)
{
    return HOST_FNPTR_SYNTHETIC_BASE + 0u;
}
