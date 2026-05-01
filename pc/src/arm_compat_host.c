/*
 * arm_compat_host.c - Host stubs for ARM-specific operations
 *
 * Implements the arm_compat.h functions on the host. SWI calls become
 * platform calls (e.g., VBlankIntrWait -> vsync wait), CP15 ops become
 * no-ops (caches don't exist on host), and CLZ uses __builtin_clz.
 */
#include "arm_compat.h"
#include "nds_platform.h"

#include <string.h>

/* ===== BIOS / SWI ===== */

void arm_swi_03_stop(void) {
    /* SWI 0x03 = Stop CPU. On host, just no-op. */
}

void arm_swi_05_vblank_intr_wait(void) {
    /* SWI 0x05 = wait for VBlank. The game thread blocks here until the
     * main SDL thread finishes a frame and posts the vblank semaphore. */
    platform_wait_vblank();
}

void arm_swi_0b_cpuset(const void *src, void *dst, u32 mode) {
    /* SWI 0x0B = CpuSet (memcpy/memfill).
     * mode bits:
     *   [20:0]  = word/halfword count
     *   [24]    = 0 -> 16-bit ops, 1 -> 32-bit ops
     *   [26]    = 0 -> copy,        1 -> fill (read src once, write N times) */
    u32 count    = mode & 0x1FFFFF;
    int word_op  = (mode >> 24) & 1;
    int fill     = (mode >> 26) & 1;

    if (word_op) {
        u32 v = fill ? *(const u32 *)src : 0;
        for (u32 i = 0; i < count; i++) {
            ((u32 *)dst)[i] = fill ? v : ((const u32 *)src)[i];
        }
    } else {
        u16 v = fill ? *(const u16 *)src : 0;
        for (u32 i = 0; i < count; i++) {
            ((u16 *)dst)[i] = fill ? v : ((const u16 *)src)[i];
        }
    }
}

/* ===== CLZ ===== */

int arm_clz(u32 value) {
    if (value == 0) return 32;
    return __builtin_clz(value);
}

/* ===== CP15 (no-ops on host) =====
 * The host has no caches or MPU regions to manage, so these are stubs. */

void arm_cp15_invalidate_dcache_mva(u32 line) { (void)line; }
void arm_cp15_clean_dcache_mva(u32 line)      { (void)line; }
void arm_cp15_invalidate_icache_mva(u32 line) { (void)line; }
void arm_cp15_dsb(void)                       { /* no-op */ }
void arm_cp15_wfi(void)                       { /* no-op */ }

static u32 g_cp15_control = 0;
u32  arm_cp15_read_control(void)              { return g_cp15_control; }
void arm_cp15_write_control(u32 v)            { g_cp15_control = v; }

static u32 g_data_region, g_insn_region;
void arm_cp15_set_data_region(u32 region)     { g_data_region = region; }
void arm_cp15_set_insn_region(u32 region)     { g_insn_region = region; }
u32  arm_cp15_get_insn_region(void)           { return g_insn_region; }
