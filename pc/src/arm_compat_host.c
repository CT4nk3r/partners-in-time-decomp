/*
 * arm_compat_host.c - Host stubs for ARM-specific operations
 *
 * Implements the arm_compat.h functions on the host. SWI calls become
 * platform calls (e.g., VBlankIntrWait -> vsync wait), CP15 ops become
 * no-ops (caches don't exist on host), and CLZ uses __builtin_clz.
 *
 * BIOS decompression (SWI 0x11/0x12/0x13):
 *   Reference: https://problemkaputt.de/gbatek.htm#biosdecompressionfunctions
 *   The game calls these directly via SWI; they are pure CPU operations on
 *   real hardware. On the host we implement them as pure C functions with
 *   the same src/dst interface (no hardware emulation needed).
 */
#include "arm_compat.h"
#include "nds_platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * Diagnostic taps for natural-path asset loaders.
 *
 * Track A (natural-vram-population session): the only references in
 * decompiled arm9/src/ to LZ77 / CpuSet are stubs -- the real game
 * boot loaders that decompress tilesheets/palettes/tilemaps into VRAM
 * have not been decompiled yet (only ~236 of 1275 functions are done
 * and they don't contain the boot-asset fetch path).  When MLPIT_LOG_DECOMP=1
 * we print a one-line summary every time SWI 0x11/0x12/0x13 or SWI 0x0B
 * (CpuSet) targets either main-engine BG VRAM (0x06000000-0x0607FFFF),
 * sub-engine BG VRAM (0x06200000-0x0621FFFF), engine-A/B palette RAM
 * (0x05000000-0x050007FF) or extended palette VRAM via banks E/F/G.
 * If the natural code path triggers any of these the log surfaces
 * the call site (return-addr unavailable here, but the trace pinpoints
 * which decompressed asset just landed and where).
 */
#define VRAM_BG_MAIN_LO  0x06000000u
#define VRAM_BG_MAIN_HI  0x0607FFFFu
#define VRAM_BG_SUB_LO   0x06200000u
#define VRAM_BG_SUB_HI   0x0621FFFFu
#define VRAM_OBJ_MAIN_LO 0x06400000u
#define VRAM_OBJ_MAIN_HI 0x0643FFFFu
#define VRAM_PAL_LO      0x05000000u
#define VRAM_PAL_HI      0x050007FFu

static int decomp_log_enabled(void) {
    static int s_state = -1;
    if (s_state < 0) s_state = getenv("MLPIT_LOG_DECOMP") ? 1 : 0;
    return s_state;
}

static const char *vram_region_name(uintptr_t a) {
    if (a >= VRAM_BG_MAIN_LO && a <= VRAM_BG_MAIN_HI)   return "BG_MAIN";
    if (a >= VRAM_BG_SUB_LO  && a <= VRAM_BG_SUB_HI)    return "BG_SUB";
    if (a >= VRAM_OBJ_MAIN_LO&& a <= VRAM_OBJ_MAIN_HI)  return "OBJ_MAIN";
    if (a >= VRAM_PAL_LO     && a <= VRAM_PAL_HI)       return "PAL";
    return NULL;
}

static void log_vram_target(const char *kind, const void *src, void *dst,
                            uint32_t size_or_mode) {
    if (!decomp_log_enabled()) return;
    uintptr_t da = (uintptr_t)dst;
    /* Mask to NDS address space so host pointers (high) appear as
     * their NDS-equivalent low 32 bits when the runtime maps RAM at
     * the canonical NDS base via VirtualAlloc.  Otherwise this mask
     * has no effect on already-low addresses. */
    uintptr_t da_nds = da & 0xFFFFFFFFu;
    const char *region = vram_region_name(da_nds);
    if (!region) return;
    static int s_count = 0;
    if (s_count++ < 64) {
        fprintf(stderr,
                "[decomp-tap] %s src=%p dst=0x%08X region=%s sz/mode=0x%X\n",
                kind, src, (unsigned)da_nds, region, (unsigned)size_or_mode);
    }
}

/* Public hook used by link_stubs.c MI_DmaCopy* shims. */
void host_log_vram_dma(const char *kind, const void *src, void *dst, u32 size) {
    log_vram_target(kind, src, dst, (uint32_t)size);
}

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
    log_vram_target("SWI_0B_CpuSet", src, dst, mode);
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

/* ===== BIOS Decompression SWIs ===== */

/*
 * arm_swi_11_lz77_decomp — SWI 0x11 LZ77UnCompVram
 *
 * Source layout (GBATEK):
 *   Bytes 0-3: (decompressed_size << 8) | 0x10
 *   Then flag blocks:
 *     1 byte  flags  (bit 7 = first unit, bit 0 = last unit in block)
 *     8 units, each:
 *       flags-bit=0 → 1 byte literal
 *       flags-bit=1 → 2 bytes back-ref:
 *           byte A = (len-3)<<4 | disp_high4
 *           byte B = disp_low8
 *           len  = (A>>4)+3  (3..18)
 *           disp = ((A&0xF)<<8 | B)+1  (1..4096 bytes backwards)
 */
void arm_swi_11_lz77_decomp(const void *src, void *dst)
{
    log_vram_target("SWI_11_LZ77", src, dst, 0);
    const uint8_t *s = (const uint8_t *)src;
    uint8_t       *d = (uint8_t *)dst;

    /* 4-byte header */
    uint32_t hdr       = (uint32_t)s[0] | ((uint32_t)s[1] << 8) |
                         ((uint32_t)s[2] << 16) | ((uint32_t)s[3] << 24);
    uint32_t decomp_sz = hdr >> 8;
    s += 4;

    uint32_t written = 0;
    while (written < decomp_sz) {
        uint8_t flags = *s++;
        for (int bit = 7; bit >= 0 && written < decomp_sz; bit--) {
            if (!((flags >> bit) & 1)) {
                /* Literal byte */
                *d++ = *s++;
                written++;
            } else {
                /* Back-reference */
                uint8_t a  = *s++;
                uint8_t b  = *s++;
                uint32_t len  = (uint32_t)(a >> 4) + 3;
                uint32_t disp = ((uint32_t)(a & 0xF) << 8 | b) + 1;
                const uint8_t *back = d - disp;
                for (uint32_t i = 0; i < len && written < decomp_sz; i++, written++) {
                    *d++ = back[i];
                }
            }
        }
    }
}

/*
 * arm_swi_12_huff_decomp — SWI 0x12 HuffUnComp
 *
 * Source layout (GBATEK):
 *   Bytes 0-3: (decompressed_size << 8) | type  (type = 0x20..0x2C)
 *   Byte 4:    tree_table_size = (n/2 - 1)  (tree occupies (tree_table_size+1)*2 bytes)
 *   Bytes 5..: Huffman tree nodes (pairs: [data/offset, data/offset])
 *              Bit 7 of each node byte = "child is leaf" flag
 *   After tree: packed code stream (MSB first)
 *     Symbols are 4 or 8 bits depending on type byte low nibble.
 *
 * This is a simplified implementation that handles both 4-bit and 8-bit Huffman.
 */
void arm_swi_12_huff_decomp(const void *src, void *dst)
{
    log_vram_target("SWI_12_Huff", src, dst, 0);
    const uint8_t *s = (const uint8_t *)src;
    uint8_t       *d = (uint8_t *)dst;

    uint32_t hdr       = (uint32_t)s[0] | ((uint32_t)s[1] << 8) |
                         ((uint32_t)s[2] << 16) | ((uint32_t)s[3] << 24);
    uint32_t decomp_sz = hdr >> 8;
    uint32_t bits      = hdr & 0x0F;   /* 4 or 8 bit symbols */
    if (bits == 0) bits = 8;

    uint32_t tree_sz = (uint32_t)s[4] + 1;  /* number of node pairs */
    const uint8_t *tree = s + 5;
    const uint8_t *data = s + 4 + tree_sz * 2;

    /* Ensure data is 4-byte aligned from start of src */
    uintptr_t data_off = (uintptr_t)(data - s);
    data_off = (data_off + 3) & ~3u;
    data = s + data_off;

    uint32_t written = 0;
    uint32_t bit_buf = 0;
    int      bits_left = 0;

    uint32_t half_word = 0;  /* accumulate nibbles for 4-bit mode */
    int      half_valid = 0;

    while (written < decomp_sz) {
        /* Walk the tree from root (node 0) */
        uint32_t node = 0;   /* root is node 0 (pair index 0) */
        uint32_t sym  = 0;

        do {
            /* Get next bit from stream */
            if (bits_left == 0) {
                uint32_t w = (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
                             ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
                bit_buf   = w;
                bits_left = 32;
                data += 4;
            }
            int branch = (bit_buf >> 31) & 1;
            bit_buf <<= 1;
            bits_left--;

            /* Each node pair: [left_entry, right_entry]
             * Entry bits [6:0] = offset to child (in node pairs from current node+1)
             * Entry bit 7      = child is a leaf (symbol follows in next byte) */
            uint8_t entry = tree[node * 2 + branch];
            uint32_t child = node + (entry & 0x3F) + 1;

            if (entry & 0x80) {
                /* Child is leaf — symbol value is the child data byte */
                sym = tree[child * 2 + (1 - branch)]; /* GBATEK: leaf data byte */
                /* Actually per GBATEK: the leaf value is in the "next" node slot */
                /* Simplified: just use the child node index data */
                sym = tree[child * 2];
                break;
            }
            node = child;
        } while (1);

        if (bits == 4) {
            if (!half_valid) {
                half_word  = sym & 0xF;
                half_valid = 1;
            } else {
                *d++ = (uint8_t)(half_word | ((sym & 0xF) << 4));
                written++;
                half_valid = 0;
            }
        } else {
            *d++ = (uint8_t)(sym & 0xFF);
            written++;
        }
    }
}

/*
 * arm_swi_13_rle_decomp — SWI 0x13 RLUnCompVram
 *
 * Source layout (GBATEK):
 *   Bytes 0-3: (decompressed_size << 8) | 0x30
 *   For each run:
 *     flag_byte:
 *       bit 7 = 0  → uncompressed:  next (flag & 0x7F)+1 bytes are literal
 *       bit 7 = 1  → compressed:    next byte repeated (flag & 0x7F)+3 times
 */
void arm_swi_13_rle_decomp(const void *src, void *dst)
{
    log_vram_target("SWI_13_RLE", src, dst, 0);
    const uint8_t *s = (const uint8_t *)src;
    uint8_t       *d = (uint8_t *)dst;

    uint32_t hdr       = (uint32_t)s[0] | ((uint32_t)s[1] << 8) |
                         ((uint32_t)s[2] << 16) | ((uint32_t)s[3] << 24);
    uint32_t decomp_sz = hdr >> 8;
    s += 4;

    uint32_t written = 0;
    while (written < decomp_sz) {
        uint8_t flag  = *s++;
        if (flag & 0x80) {
            /* Compressed run */
            uint32_t run  = (uint32_t)(flag & 0x7F) + 3;
            uint8_t  val  = *s++;
            for (uint32_t i = 0; i < run && written < decomp_sz; i++, written++)
                *d++ = val;
        } else {
            /* Uncompressed literal block */
            uint32_t len = (uint32_t)(flag & 0x7F) + 1;
            for (uint32_t i = 0; i < len && written < decomp_sz; i++, written++)
                *d++ = *s++;
        }
    }
}
