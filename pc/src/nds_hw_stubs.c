/*
 * nds_hw_stubs.c - Hardware register stubs
 *
 * The decompiled ARM9 code accesses NDS hardware via memory-mapped I/O at
 * 0x04000000+. On PC we route those accesses through these read/write
 * functions which maintain a shadow register file. Real behaviour (DMA
 * transfers, display compositing, etc.) is not yet emulated; reads return
 * the last written value and writes only update the shadow.
 *
 * This is intentionally minimal — a full NDS hardware emulator is out of
 * scope for the initial port. See pc/README.md for the roadmap.
 */
#include "nds_platform.h"

#include <stdint.h>
#include <string.h>

#define NDS_IO_BASE 0x04000000
#define NDS_IO_SIZE 0x01000000   /* 16 MB region; we only allocate a small shadow */
#define SHADOW_SIZE 0x10000      /* 64 KB covers all common regs (DISPCNT..audio) */

static uint8_t g_io_shadow[SHADOW_SIZE];

static int io_offset(uint32_t addr) {
    if (addr < NDS_IO_BASE) return -1;
    uint32_t off = addr - NDS_IO_BASE;
    if (off >= SHADOW_SIZE) return -1;
    return (int)off;
}

uint32_t nds_reg_read32(uint32_t addr) {
    int off = io_offset(addr);
    if (off < 0) return 0;
    uint32_t v;
    memcpy(&v, g_io_shadow + off, 4);
    return v;
}

uint16_t nds_reg_read16(uint32_t addr) {
    int off = io_offset(addr);
    if (off < 0) return 0;
    uint16_t v;
    memcpy(&v, g_io_shadow + off, 2);
    return v;
}

uint8_t nds_reg_read8(uint32_t addr) {
    int off = io_offset(addr);
    if (off < 0) return 0;
    return g_io_shadow[off];
}

void nds_reg_write32(uint32_t addr, uint32_t val) {
    int off = io_offset(addr);
    if (off < 0) return;
    memcpy(g_io_shadow + off, &val, 4);
}

void nds_reg_write16(uint32_t addr, uint16_t val) {
    int off = io_offset(addr);
    if (off < 0) return;
    memcpy(g_io_shadow + off, &val, 2);
}

void nds_reg_write8(uint32_t addr, uint8_t val) {
    int off = io_offset(addr);
    if (off < 0) return;
    g_io_shadow[off] = val;
}
