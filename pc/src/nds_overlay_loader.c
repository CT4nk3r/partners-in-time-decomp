/*
 * nds_overlay_loader.c — copy NDS arm9 overlay binaries into the
 * fixed-mapped 0x02000000..0x023FFFFF host region, so any decomp
 * literal-pool reads (vtables, string tables, DAT_<addr> globals
 * baked into overlay .data) see real bytes.
 *
 * The overlay binaries themselves are ARM machine code; we cannot
 * EXECUTE them on x86.  But static decomp identified two important
 * pieces of overlay-resident DATA:
 *
 *   - overlay 0 vtable @ 0x020BF150 — slot[2] holds 0x02065A10
 *     (per-frame "draw scene" entry point).
 *   - overlay 0/2/5 .data + overlay 6/7/8/9 .data — string tables,
 *     palette/cell descriptors, scene descriptor literals.
 *
 * Loading the binaries makes the .data visible to host C code.
 * Indirect calls to FUN_02065A10/FUN_02071298/FUN_0206E06C are then
 * dispatched through the host fnptr trampoline to whatever C
 * implementation we register in arm9/src/.
 *
 * NDS overlays SHARE memory ranges (overlay 0 and overlay 2 both
 * load at 0x020659C0).  For our HOST_PORT we have all 4 MiB of
 * arm9 RAM mapped, so we load only the OVERLAYS NEEDED FOR THE
 * RENDER PIPELINE — currently overlay 0 (top-level scene render)
 * and overlay 6 (cell→OAM emit primitive).  Overlap between these
 * two pairs is checked and reported.
 *
 * Compression: per ndstool docs each overlay table entry's 8th
 * word ("reserved"/flags) encodes a "compressed" bit at LSB and a
 * compressed-size in bits 8..31.  For PiT every reserved word in
 * y9.bin is 0 — no compressed overlays.  We assert this on load
 * and skip if encountered.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nds_platform.h"

extern void *nds_arm9_ram_base(void);
extern int   nds_arm9_ram_is_mapped(void);

/* y9.bin overlay table entry — 32 bytes per overlay. */
typedef struct {
    uint32_t id;
    uint32_t ram;
    uint32_t size;
    uint32_t bss;
    uint32_t sinit_init;
    uint32_t sinit_init_end;
    uint32_t file_id;
    uint32_t reserved;
} OvtEntry;

#define ARM9_NDS_BASE 0x02000000u
#define ARM9_NDS_SIZE (4u * 1024u * 1024u)

#define MAX_OVERLAYS 64

static OvtEntry g_ovt[MAX_OVERLAYS];
static int      g_ovt_count = 0;
static uint8_t  g_loaded[MAX_OVERLAYS];

/* The render-pipeline-critical overlays (per overlay0_decomp_plan.md
 * Session: populate-shadow-oam):
 *   0 — overlay-0 per-frame scene render (FUN_02065A10)
 *   6 — cell→OAM emit primitive         (FUN_02071298)
 *
 * Overlay 0 covers 0x020659C0..0x020BFA00 (368 672 bytes).
 * Overlay 6 covers 0x0206A800..0x0207A040 ( 63 232 bytes).
 *
 * Overlay 6 falls INSIDE overlay 0's range — they are designed
 * to be active mutually-exclusively.  Loading 6 last would clobber
 * the FUN_02065A10 instruction bytes that 0 wrote, but it would
 * NOT clobber the vtable @ 0x020BF150 (which is past 0x0207A040).
 * Since we don't execute either binary's code (only consult the
 * vtables and decompiled C), this overlap is harmless.  We log it.
 */
static const int kRenderOverlays[] = { 5, 8, -1 };

static const char *kOvtCandidates[] = {
    "extracted/y9.bin",
    "../extracted/y9.bin",
    "../../extracted/y9.bin",
    "../../../extracted/y9.bin",
    NULL,
};

static const char *kOverlayDirs[] = {
    "extracted/overlay",
    "../extracted/overlay",
    "../../extracted/overlay",
    "../../../extracted/overlay",
    NULL,
};

static FILE *open_first(const char *const *cands, const char **used) {
    for (int i = 0; cands[i]; ++i) {
        FILE *f = fopen(cands[i], "rb");
        if (f) { if (used) *used = cands[i]; return f; }
    }
    return NULL;
}

static const char *find_overlay_dir(void) {
    for (int i = 0; kOverlayDirs[i]; ++i) {
        char path[256];
        snprintf(path, sizeof(path), "%s/overlay_0000.bin", kOverlayDirs[i]);
        FILE *f = fopen(path, "rb");
        if (f) { fclose(f); return kOverlayDirs[i]; }
    }
    return NULL;
}

static int load_ovt_table(void) {
    if (g_ovt_count > 0) return g_ovt_count;
    const char *used = NULL;
    FILE *f = open_first(kOvtCandidates, &used);
    if (!f) {
        nds_log("[overlay] y9.bin not found; cannot load any overlay\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = (int)(sz / 32);
    if (n > MAX_OVERLAYS) n = MAX_OVERLAYS;
    if (fread(g_ovt, 32, (size_t)n, f) != (size_t)n) {
        nds_log("[overlay] short read on %s\n", used);
        fclose(f);
        return 0;
    }
    fclose(f);
    g_ovt_count = n;
    nds_log("[overlay] loaded OVT from %s: %d entries\n", used, n);
    return n;
}

void nds_load_overlay(int ovid) {
    if (!nds_arm9_ram_is_mapped()) {
        nds_log("[overlay] cannot load ov%d: arm9 RAM not mapped\n", ovid);
        return;
    }
    if (load_ovt_table() <= 0) return;
    if (ovid < 0 || ovid >= g_ovt_count) {
        nds_log("[overlay] ov%d out of range (have %d entries)\n", ovid, g_ovt_count);
        return;
    }
    if (g_loaded[ovid]) return;
    OvtEntry *e = &g_ovt[ovid];
    if (e->id != (uint32_t)ovid) {
        nds_log("[overlay] ov%d table mismatch (entry says id=%u)\n", ovid, e->id);
    }
    if (e->size == 0) {
        g_loaded[ovid] = 1;
        return;
    }
    if (e->reserved & 1u) {
        nds_log("[overlay] ov%d is compressed (reserved=0x%08X) — skipping (LZ77 not wired)\n",
                ovid, e->reserved);
        return;
    }
    if (e->ram < ARM9_NDS_BASE ||
        (uint64_t)e->ram + e->size + e->bss > (uint64_t)ARM9_NDS_BASE + ARM9_NDS_SIZE) {
        nds_log("[overlay] ov%d ram=0x%08X size=%u outside mapped region\n",
                ovid, e->ram, e->size);
        return;
    }

    const char *dir = find_overlay_dir();
    if (!dir) {
        nds_log("[overlay] overlay binary directory not found\n");
        return;
    }
    char path[256];
    snprintf(path, sizeof(path), "%s/overlay_%04d.bin", dir, ovid);
    FILE *f = fopen(path, "rb");
    if (!f) {
        nds_log("[overlay] open %s failed\n", path);
        return;
    }
    uint8_t *dst = (uint8_t *)(uintptr_t)e->ram;
    size_t got = fread(dst, 1, e->size, f);
    fclose(f);
    if (got != e->size) {
        nds_log("[overlay] ov%d short read: got %zu expected %u\n",
                ovid, got, e->size);
        return;
    }
    /* Zero-fill BSS region directly after the binary. */
    if (e->bss) {
        memset(dst + e->size, 0, e->bss);
    }
    g_loaded[ovid] = 1;
    nds_log("[overlay] ov%d loaded: %u bytes at 0x%08X..0x%08X (bss %u)\n",
            ovid, e->size, e->ram, e->ram + e->size, e->bss);
}

/* Force-reload an overlay even if already marked as loaded.
 * Needed when overlay 0 clobbers overlay 5 at the same base address. */
void nds_reload_overlay(int ovid) {
    if (ovid >= 0 && ovid < MAX_OVERLAYS)
        g_loaded[ovid] = 0;
    nds_load_overlay(ovid);
}

void nds_load_render_overlays(void) {
    if (!nds_arm9_ram_is_mapped()) return;
    if (load_ovt_table() <= 0) return;

    /* Detect overlap between the overlays we plan to load and warn. */
    for (int i = 0; kRenderOverlays[i] >= 0; ++i) {
        int ai = kRenderOverlays[i];
        if (ai >= g_ovt_count) continue;
        OvtEntry *a = &g_ovt[ai];
        for (int j = i + 1; kRenderOverlays[j] >= 0; ++j) {
            int bi = kRenderOverlays[j];
            if (bi >= g_ovt_count) continue;
            OvtEntry *b = &g_ovt[bi];
            uint32_t a_end = a->ram + a->size;
            uint32_t b_end = b->ram + b->size;
            if (!(a_end <= b->ram || b_end <= a->ram)) {
                nds_log("[overlay] WARNING ov%d (0x%08X..0x%08X) overlaps "
                        "ov%d (0x%08X..0x%08X) — second load will partially "
                        "overwrite first\n",
                        ai, a->ram, a_end, bi, b->ram, b_end);
            }
        }
    }

    for (int i = 0; kRenderOverlays[i] >= 0; ++i) {
        nds_load_overlay(kRenderOverlays[i]);
    }
}

void nds_load_all_overlays(void) {
    if (load_ovt_table() <= 0) return;
    /* Loading every overlay would clobber overlapping ranges in
     * unpredictable order.  We expose this only for diagnostics. */
    for (int i = 0; i < g_ovt_count; ++i) {
        nds_load_overlay(i);
    }
}

int nds_overlay_is_loaded(int ovid) {
    if (ovid < 0 || ovid >= MAX_OVERLAYS) return 0;
    return g_loaded[ovid];
}

uint32_t nds_overlay_ram_base(int ovid) {
    if (load_ovt_table() <= 0) return 0;
    if (ovid < 0 || ovid >= g_ovt_count) return 0;
    return g_ovt[ovid].ram;
}
