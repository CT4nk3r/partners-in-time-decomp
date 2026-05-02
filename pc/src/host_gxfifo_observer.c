/*
 * host_gxfifo_observer.c — HOST_PORT periodic snapshot of the 3D GX
 * command MMIO region (0x04000400-0x040006FF).
 *
 * Why a polling observer instead of a write hook:
 *   The transliterated FUN_0200fcb4 (and most decompiled rendering
 *   code) issues raw `*(volatile u32*)0x040004xx = val` writes that go
 *   directly to the IO shadow VirtualAlloc'd at 0x04000000. They bypass
 *   nds_reg_write32 entirely. To detect 3D engine activity without
 *   patching the transliterated code, we sample the GX command range
 *   each frame and log any non-zero word we haven't already reported.
 *
 *   Real NDS GXFIFO semantics consume each write into a FIFO; the host
 *   shadow only retains the *last* value written to each address. That's
 *   still enough to prove "the 3D rendering path is firing" — we'll see
 *   non-zero values in MTX_MODE / MTX_SCALE / BEGIN_VTXS / VTX_16 /
 *   COLOR / TEXIMAGE_PARAM / PLTT_BASE / END_VTXS, which is the smoking
 *   gun the user asked for.
 */
#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GX_BASE   0x04000400u
#define GX_SIZE   0x300u    /* 0x04000400 .. 0x040006FF */

/* Names of every GX command register we care about logging. The 3D
 * engine command map is small and well-known; anything not in this
 * table is logged with "(unk)". */
static const char *gx_name(uint32_t addr)
{
    switch (addr) {
        case 0x04000440: return "GXFIFO";
        case 0x04000444: return "MTX_MODE";
        case 0x04000448: return "MTX_PUSH";
        case 0x0400044C: return "MTX_POP";
        case 0x04000450: return "MTX_STORE";
        case 0x04000454: return "MTX_RESTORE";
        case 0x04000458: return "MTX_IDENTITY";
        case 0x0400045C: return "MTX_LOAD_4x4";
        case 0x04000460: return "MTX_LOAD_4x3";
        case 0x04000464: return "MTX_MULT_4x4";
        case 0x04000468: return "MTX_MULT_4x3";
        case 0x0400046C: return "MTX_SCALE";
        case 0x04000470: return "MTX_TRANS";
        case 0x04000480: return "COLOR";
        case 0x04000484: return "NORMAL";
        case 0x04000488: return "VTX_16";
        case 0x0400048C: return "VTX_10";
        case 0x04000490: return "VTX_XY";
        case 0x04000494: return "TEXCOORD";
        case 0x04000498: return "VTX_DIFF";
        case 0x040004A4: return "TEXIMAGE_PARAM";
        case 0x040004A8: return "PLTT_BASE";
        case 0x040004AC: return "(material/teximage follow-up)";
        case 0x04000500: return "BEGIN_VTXS";
        case 0x04000504: return "END_VTXS";
        case 0x04000540: return "SWAP_BUFFERS";
        case 0x04000580: return "VIEWPORT";
        case 0x040005C8: return "(GX ctl)";
        default:         return "(unk)";
    }
}

/* Snapshot of last-seen value per word. -1 means "never seen". */
static uint32_t s_seen[GX_SIZE / 4];
static int      s_seen_init = 0;
static int      s_logged    = 0;
#define LOG_BUDGET 128   /* hard cap so we don't spam stderr */

extern int nds_arm9_ram_is_mapped(void);

void host_gxfifo_observer_tick(void)
{
    if (!getenv("MLPIT_GXFIFO_OBSERVER")) return;

    if (!s_seen_init) {
        memset(s_seen, 0, sizeof(s_seen));
        s_seen_init = 1;
        fprintf(stderr,
                "[gxfifo] observer armed — sampling 0x%08X..0x%08X each frame "
                "(LOG_BUDGET=%d)\n",
                (unsigned)GX_BASE, (unsigned)(GX_BASE + GX_SIZE), LOG_BUDGET);
    }

    if (s_logged >= LOG_BUDGET) return;

    /* The IO shadow is mapped at 0x04000000 by nds_hw_io_init via
     * VirtualAlloc; if that failed the static shadow is unreachable
     * from a raw NDS-address pointer, so guard the deref. We assume
     * the mapping succeeded (game_thread.c logs it at boot). */
    const volatile uint32_t *p = (const volatile uint32_t *)(uintptr_t)GX_BASE;

    for (uint32_t i = 0; i < GX_SIZE / 4; i++) {
        uint32_t v = p[i];
        if (v == 0) continue;
        if (v == s_seen[i]) continue;
        s_seen[i] = v;
        uint32_t addr = GX_BASE + i * 4u;
        fprintf(stderr, "[gxfifo] %s w=0x%08X v=0x%08X\n",
                gx_name(addr), (unsigned)addr, (unsigned)v);
        if (++s_logged >= LOG_BUDGET) {
            fprintf(stderr, "[gxfifo] LOG_BUDGET reached — silencing\n");
            break;
        }
    }
}
