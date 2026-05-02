/*
 * nds_hw_stubs.c - Hardware register stubs with DMA emulation
 *
 * The decompiled ARM9 code accesses NDS hardware via memory-mapped I/O at
 * 0x04000000+. On PC we route those accesses through these read/write
 * functions which maintain a shadow register file.
 *
 * Task 4 additions:
 *  - On Windows, try to allocate the IO shadow AT the NDS address 0x04000000
 *    so that decompiled code performing raw pointer writes (e.g. REG_DISPCNT =
 *    0x0100) doesn't segfault — those writes land in our shadow directly.
 *  - DMA emulation: nds_reg_write32 intercepts writes to DMAxCNT registers.
 *    If the enable bit is set (bit 31) and we can resolve both src/dst to host
 *    pointers, we perform the memcpy immediately.
 *  - DISPCNT / BGxCNT writes are already fully handled by the shadow — the
 *    rasterizer reads from the same shadow on every frame.
 */
#include "nds_platform.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

/* ── IO shadow region ────────────────────────────────────────── */

#define NDS_IO_BASE  0x04000000u
#define NDS_IO_SIZE  0x00010000u   /* 64 KB covers DISPCNT..audio */

/*
 * On Windows we try to commit the IO shadow AT address 0x04000000 so
 * that raw writes from decompiled code (*(vu32*)0x04000000 = val) land
 * here without a SIGSEGV.  If VirtualAlloc fails we fall back to a
 * statically-allocated array.
 */
static uint8_t  g_io_shadow_static[NDS_IO_SIZE];
static uint8_t *g_io_shadow = NULL;
static int      g_io_mapped = 0;   /* 1 if VirtualAlloc at NDS address succeeded */

void nds_hw_io_init(void)
{
    if (g_io_shadow) return;   /* already initialised */

#ifdef _WIN32
    void *p = VirtualAlloc((LPVOID)(uintptr_t)NDS_IO_BASE,
                           NDS_IO_SIZE,
                           MEM_COMMIT | MEM_RESERVE,
                           PAGE_READWRITE);
    if (p) {
        g_io_shadow = (uint8_t *)p;
        g_io_mapped = 1;
        memset(g_io_shadow, 0, NDS_IO_SIZE);
        nds_log("[hw] IO shadow mapped at NDS address 0x%08X (raw writes work)\n",
                NDS_IO_BASE);
        return;
    }
    nds_log("[hw] IO VirtualAlloc at 0x%08X failed (err=%lu), using static shadow\n",
            NDS_IO_BASE, (unsigned long)GetLastError());
#endif

    g_io_shadow = g_io_shadow_static;
    memset(g_io_shadow, 0, NDS_IO_SIZE);
}

/* ── NDS address → host pointer resolver ────────────────────── */

/*
 * Translate an NDS physical address to a host pointer.
 * Covers IO shadow, Palette RAM (VRAM bank E), and VRAM banks A/B.
 * Returns NULL for addresses we cannot resolve.
 */
static void *nds_addr_to_host(uint32_t addr, uint32_t len)
{
    /* IO registers 0x04000000 – 0x0400FFFF */
    if (g_io_shadow && addr >= NDS_IO_BASE &&
        addr + len <= NDS_IO_BASE + NDS_IO_SIZE)
        return g_io_shadow + (addr - NDS_IO_BASE);

    /* Palette RAM 0x05000000 (main engine: first 512 B of bank E) */
    {
        void *e = nds_vram_bank('E');
        uint32_t esz = nds_vram_bank_size('E');
        if (e && addr >= 0x05000000u && addr + len <= 0x05000000u + esz)
            return (uint8_t *)e + (addr - 0x05000000u);
    }

    /* VRAM bank A 0x06000000 – 0x0601FFFF (main engine BG, typical mapping) */
    {
        void *a = nds_vram_bank('A');
        uint32_t asz = nds_vram_bank_size('A');
        if (a && addr >= 0x06000000u && addr + len <= 0x06000000u + asz)
            return (uint8_t *)a + (addr - 0x06000000u);
    }

    /* VRAM bank B 0x06020000 – 0x0603FFFF */
    {
        void *b = nds_vram_bank('B');
        uint32_t bsz = nds_vram_bank_size('B');
        if (b && addr >= 0x06020000u && addr + len <= 0x06020000u + bsz)
            return (uint8_t *)b + (addr - 0x06020000u);
    }

    /* VRAM bank C 0x06040000 – (sub engine) */
    {
        void *c = nds_vram_bank('C');
        uint32_t csz = nds_vram_bank_size('C');
        if (c && addr >= 0x06040000u && addr + len <= 0x06040000u + csz)
            return (uint8_t *)c + (addr - 0x06040000u);
    }

    return NULL;
}

/* ── DMA emulation ───────────────────────────────────────────── */

/*
 * DMA channel register layout (each channel spans 12 bytes):
 *   +0  DMAx_SAD   (source address, u32)
 *   +4  DMAx_DAD   (destination address, u32)
 *   +8  DMAx_CNT   (count + control, u32)
 *
 * Control word bits:
 *   [20:0]  word count
 *   [26:25] dst addr mode (0=inc, 1=inc+reload, 2=fix, 3=fix)
 *   [28:27] src addr mode (0=inc, 1=dec, 2=fix)
 *   [29]    repeat
 *   [30]    word size (0=16-bit, 1=32-bit)
 *   [31]    enable
 */
#define DMA0_SAD_REG  0x040000B0u
#define DMA0_DAD_REG  0x040000B4u
#define DMA0_CNT_REG  0x040000B8u
#define DMA_CHANNEL_STRIDE  12u
#define DMA_NUM_CHANNELS    4u
#define DMA_ENABLE_BIT      (1u << 31)
#define DMA_WORD32_BIT      (1u << 26)   /* NDS: bit 26 selects 32-bit */

static void dma_execute(int ch)
{
    uint32_t base = DMA0_SAD_REG + (uint32_t)ch * DMA_CHANNEL_STRIDE;
    uint32_t src_nds = 0, dst_nds = 0, cnt_val = 0;
    memcpy(&src_nds, g_io_shadow + (base   - NDS_IO_BASE), 4);
    memcpy(&dst_nds, g_io_shadow + (base+4 - NDS_IO_BASE), 4);
    memcpy(&cnt_val, g_io_shadow + (base+8 - NDS_IO_BASE), 4);

    if (!(cnt_val & DMA_ENABLE_BIT)) return;

    uint32_t count    = cnt_val & 0x1FFFFFu;
    int      is_32bit = (cnt_val >> 26) & 1;
    uint32_t byte_len = count * (uint32_t)(is_32bit ? 4 : 2);

    if (byte_len == 0) return;

    void *dst_host = nds_addr_to_host(dst_nds, byte_len);
    if (!dst_host) {
        /* dst is not in any known NDS region — skip silently */
        return;
    }

    /*
     * For the src: if it looks like a valid host pointer (> 0x10000000 on
     * Win64 where user space starts high), keep it as-is.  Otherwise try
     * the NDS address resolver.
     */
    void *src_host = NULL;
#ifdef _WIN32
    /* On Win64 typical user-space base is 0x140000000+; NDS RAM < 0x0CFFFFFF */
    if ((uintptr_t)src_nds > 0x0FFFFFFFu) {
        /* Likely a truncated 64-bit host pointer — we can't fix it */
        return;
    }
#endif
    src_host = nds_addr_to_host(src_nds, byte_len);
    if (!src_host) return;

    memcpy(dst_host, src_host, byte_len);

    /* Clear the enable bit to signal completion */
    cnt_val &= ~DMA_ENABLE_BIT;
    memcpy(g_io_shadow + (base+8 - NDS_IO_BASE), &cnt_val, 4);

    nds_log("[dma] ch%d: src=%08X dst=%08X count=%u %s -> done\n",
            ch, src_nds, dst_nds, count, is_32bit ? "32b" : "16b");
}

/* ── IO offset helper ────────────────────────────────────────── */

static int io_offset(uint32_t addr) {
    if (addr < NDS_IO_BASE) return -1;
    uint32_t off = addr - NDS_IO_BASE;
    if (off >= NDS_IO_SIZE) return -1;
    return (int)off;
}

/* ── Lazy init shim ──────────────────────────────────────────── */

static inline void ensure_io_init(void) {
    if (!g_io_shadow) nds_hw_io_init();
}

/* ── nds_dma_immediate ───────────────────────────────────────── */

/*
 * Public DMA helper used by func_0x01ff85cc stubs and any code path
 * that triggers a DMA without going through the hardware register path.
 *
 * ctrl layout (ARM9 DMA control register):
 *   bits [20:0]  transfer count (words when bit 26=1, halfwords otherwise)
 *   bit  [26]    word size: 0=16-bit halfwords, 1=32-bit words
 *   bit  [31]    enable (if clear, this is a no-op)
 */
void nds_dma_immediate(uint32_t dst_nds, uint32_t src_nds, uint32_t ctrl)
{
    ensure_io_init();

    if (!(ctrl & DMA_ENABLE_BIT)) return;

    uint32_t count    = ctrl & 0x1FFFFFu;
    int      is_32bit = (ctrl >> 26) & 1;
    uint32_t byte_len = count * (uint32_t)(is_32bit ? 4 : 2);
    if (byte_len == 0) return;

    void *dst_host = nds_addr_to_host(dst_nds, byte_len);
    if (!dst_host) return;

    if (src_nds == 0) {
        /* Fill with zeros */
        memset(dst_host, 0, byte_len);
        nds_log("[dma] fill_zero dst=%08X len=%u\n", dst_nds, byte_len);
        return;
    }

    void *src_host = nds_addr_to_host(src_nds, byte_len);
    if (!src_host) return;

    memcpy(dst_host, src_host, byte_len);
    nds_log("[dma] immediate: src=%08X dst=%08X bytes=%u\n",
            src_nds, dst_nds, byte_len);
}

/* ── Public register access ──────────────────────────────────── */

uint32_t nds_reg_read32(uint32_t addr) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return 0;
    uint32_t v;
    memcpy(&v, g_io_shadow + off, 4);
    return v;
}

uint16_t nds_reg_read16(uint32_t addr) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return 0;
    uint16_t v;
    memcpy(&v, g_io_shadow + off, 2);
    return v;
}

uint8_t nds_reg_read8(uint32_t addr) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return 0;
    return g_io_shadow[off];
}

void nds_reg_write32(uint32_t addr, uint32_t val) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return;
    memcpy(g_io_shadow + off, &val, 4);

    /* DMA control register hook */
    if (val & DMA_ENABLE_BIT) {
        for (int ch = 0; ch < (int)DMA_NUM_CHANNELS; ch++) {
            uint32_t cnt_addr = DMA0_CNT_REG + (uint32_t)ch * DMA_CHANNEL_STRIDE;
            if (addr == cnt_addr) {
                dma_execute(ch);
                break;
            }
        }
    }
}

void nds_reg_write16(uint32_t addr, uint16_t val) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return;
    memcpy(g_io_shadow + off, &val, 2);
}

void nds_reg_write8(uint32_t addr, uint8_t val) {
    ensure_io_init();
    int off = io_offset(addr);
    if (off < 0) return;
    g_io_shadow[off] = val;
}
