/*
 * arm_compat.h - ARM/host compatibility shim
 *
 * The decompiled ARM9 code uses inline assembly for:
 *   - SWI (BIOS calls)
 *   - CP15 coprocessor (cache + protection unit)
 *   - CLZ (count leading zeros)
 *
 * On the original ARM target these expand to the real instructions.
 * On the host (PC port) they call into stubs defined in pc/src/arm_compat_host.c
 * so that "arm9/src/(all .c files)" can be compiled with host gcc unchanged.
 *
 * Define HOST_PORT to use the host stubs.
 */
#ifndef ARM_COMPAT_H
#define ARM_COMPAT_H

#include "types.h"

#ifdef HOST_PORT
/* ============================================================
 * Host (PC port) implementations - declared here, defined in
 * pc/src/arm_compat_host.c
 * ============================================================ */
void arm_swi_03_stop(void);
void arm_swi_05_vblank_intr_wait(void);
void arm_swi_0b_cpuset(const void *src, void *dst, u32 mode);
/* SWI 0x11: LZ77UnCompVram  — decompress LZ77-encoded data */
void arm_swi_11_lz77_decomp(const void *src, void *dst);
/* SWI 0x12: HuffUnComp       — decompress Huffman-encoded data */
void arm_swi_12_huff_decomp(const void *src, void *dst);
/* SWI 0x13: RLUnCompVram     — decompress RLE-encoded data */
void arm_swi_13_rle_decomp(const void *src, void *dst);
int  arm_clz(u32 value);
void arm_cp15_invalidate_dcache_mva(u32 line);
void arm_cp15_clean_dcache_mva(u32 line);
void arm_cp15_invalidate_icache_mva(u32 line);
void arm_cp15_dsb(void);
void arm_cp15_wfi(void);
u32  arm_cp15_read_control(void);
void arm_cp15_write_control(u32 v);
void arm_cp15_set_data_region(u32 region);
void arm_cp15_set_insn_region(u32 region);
u32  arm_cp15_get_insn_region(void);
#else
/* ============================================================
 * Native ARM implementations
 * ============================================================ */
static inline void arm_swi_03_stop(void) {
    asm("swi 0x03" ::: "r0", "r1", "r2", "r3");
}
static inline void arm_swi_05_vblank_intr_wait(void) {
    asm("swi 0x05" ::: "r0", "r1", "r2", "r3");
}
static inline void arm_swi_0b_cpuset(const void *src, void *dst, u32 mode) {
    register const void *r0 asm("r0") = src;
    register void *r1 asm("r1") = dst;
    register u32 r2 asm("r2") = mode;
    asm volatile("swi 0x0B" : "+r"(r0), "+r"(r1), "+r"(r2) :: "r3", "memory");
}
static inline int arm_clz(u32 value) {
    int r;
    asm("clz %0, %1" : "=r"(r) : "r"(value));
    return r;
}
static inline void arm_cp15_invalidate_dcache_mva(u32 line) {
    asm volatile("mcr p15, 0, %0, c7, c6, 1" :: "r"(line));
}
static inline void arm_cp15_clean_dcache_mva(u32 line) {
    asm volatile("mcr p15, 0, %0, c7, c10, 1" :: "r"(line));
}
static inline void arm_cp15_invalidate_icache_mva(u32 line) {
    asm volatile("mcr p15, 0, %0, c7, c5, 1" :: "r"(line));
}
static inline void arm_cp15_dsb(void) {
    asm volatile("mcr p15, 0, %0, c7, c10, 4" :: "r"(0));
}
static inline void arm_cp15_wfi(void) {
    asm volatile("mcr p15, 0, %0, c7, c0, 4" :: "r"(0));
}
static inline u32 arm_cp15_read_control(void) {
    u32 v;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(v));
    return v;
}
static inline void arm_cp15_write_control(u32 v) {
    asm volatile("mcr p15, 0, %0, c1, c0, 0" :: "r"(v));
}
static inline void arm_cp15_set_data_region(u32 region) {
    asm volatile("mcr p15, 0, %0, c6, c1, 0" :: "r"(region));
}
static inline void arm_cp15_set_insn_region(u32 region) {
    asm volatile("mcr p15, 0, %0, c6, c2, 0" :: "r"(region));
}
static inline u32 arm_cp15_get_insn_region(void) {
    u32 v;
    asm volatile("mrc p15, 0, %0, c6, c2, 0" : "=r"(v));
    return v;
}
#endif /* HOST_PORT */

#endif /* ARM_COMPAT_H */
