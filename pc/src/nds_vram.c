/*
 * nds_vram.c - VRAM bank emulation
 *
 * The Nintendo DS has 9 VRAM banks (A-I) totaling 656 KB. Each can be mapped
 * to different purposes (BG, OBJ, ARM7, texture, palette) via VRAMCNT.
 *
 * On PC we VirtualAlloc the entire 0x06000000-0x06900000 range (9 MB) so
 * that decompiled code with raw NDS pointer literals can directly read/write
 * VRAM addresses (including LCDC mode at 0x06800000). The per-bank pointers
 * returned by nds_vram_bank() point INTO this VirtualAlloc'd region at the
 * LCDC offsets so data written via 0x068xxxxx is visible to rendering code.
 *
 * We also VirtualAlloc palette RAM (0x05000000, 4KB) and OAM (0x07000000,
 * 4KB) so those raw pointer dereferences don't crash either.
 */
#include "nds_platform.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

/* NDS VRAM bank sizes and LCDC-mode offsets from 0x06800000 */
static const struct {
    char     id;
    uint32_t size;
    uint32_t lcdc_offset;  /* offset from 0x06000000 to LCDC address */
} VRAM_BANKS[] = {
    { 'A', 128 * 1024, 0x800000 },  /* 0x06800000 */
    { 'B', 128 * 1024, 0x820000 },  /* 0x06820000 */
    { 'C', 128 * 1024, 0x840000 },  /* 0x06840000 */
    { 'D', 128 * 1024, 0x860000 },  /* 0x06860000 */
    { 'E',  64 * 1024, 0x880000 },  /* 0x06880000 */
    { 'F',  16 * 1024, 0x890000 },  /* 0x06890000 */
    { 'G',  16 * 1024, 0x894000 },  /* 0x06894000 */
    { 'H',  32 * 1024, 0x898000 },  /* 0x06898000 */
    { 'I',  16 * 1024, 0x8A0000 },  /* 0x068A0000 */
};

#define VRAM_NDS_BASE  0x06000000u
#define VRAM_NDS_SIZE  0x00900000u  /* 9 MB covers 0x06000000-0x068FFFFF */

#define PAL_NDS_BASE   0x05000000u
#define PAL_NDS_SIZE   0x00001000u  /* 4 KB (main 512B + sub 512B + padding) */

#define OAM_NDS_BASE   0x07000000u
#define OAM_NDS_SIZE   0x00001000u  /* 4 KB (main 1KB + sub 1KB) */

static uint8_t *g_vram_base = NULL;  /* VirtualAlloc at 0x06000000 */
static uint8_t *g_pal_base  = NULL;  /* VirtualAlloc at 0x05000000 */
static uint8_t *g_banks[9];
static int g_initialized = 0;

static int bank_index(char bank) {
    if (bank >= 'a' && bank <= 'i') bank -= ('a' - 'A');
    if (bank < 'A' || bank > 'I') return -1;
    return bank - 'A';
}

static void ensure_init(void) {
    if (g_initialized) return;
    g_initialized = 1;

#ifdef _WIN32
    /* Map the full VRAM region so raw NDS pointer dereferences work */
    g_vram_base = (uint8_t *)VirtualAlloc(
        (LPVOID)(uintptr_t)VRAM_NDS_BASE,
        VRAM_NDS_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
    if (g_vram_base && (uintptr_t)g_vram_base == (uintptr_t)VRAM_NDS_BASE) {
        memset(g_vram_base, 0, VRAM_NDS_SIZE);
        nds_log("[vram] mapped 9 MB at 0x%08X (VRAM + LCDC)\n", VRAM_NDS_BASE);
        /* Point bank pointers into the LCDC region of the VirtualAlloc */
        for (int i = 0; i < 9; i++) {
            g_banks[i] = g_vram_base + VRAM_BANKS[i].lcdc_offset;
        }
    } else {
        if (g_vram_base) VirtualFree(g_vram_base, 0, MEM_RELEASE);
        g_vram_base = NULL;
        nds_log("[vram] VirtualAlloc at 0x%08X failed — using heap fallback\n",
                VRAM_NDS_BASE);
        for (int i = 0; i < 9; i++)
            g_banks[i] = (uint8_t *)calloc(1, VRAM_BANKS[i].size);
    }

    /* Map palette RAM */
    g_pal_base = (uint8_t *)VirtualAlloc(
        (LPVOID)(uintptr_t)PAL_NDS_BASE,
        PAL_NDS_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
    if (g_pal_base && (uintptr_t)g_pal_base == (uintptr_t)PAL_NDS_BASE) {
        memset(g_pal_base, 0, PAL_NDS_SIZE);
        nds_log("[vram] mapped palette RAM at 0x%08X\n", PAL_NDS_BASE);
    } else {
        if (g_pal_base) VirtualFree(g_pal_base, 0, MEM_RELEASE);
        g_pal_base = NULL;
    }
#else
    for (int i = 0; i < 9; i++)
        g_banks[i] = (uint8_t *)calloc(1, VRAM_BANKS[i].size);
#endif
}

void* nds_vram_bank(char bank) {
    ensure_init();
    int idx = bank_index(bank);
    return idx < 0 ? NULL : g_banks[idx];
}

uint32_t nds_vram_bank_size(char bank) {
    int idx = bank_index(bank);
    return idx < 0 ? 0 : VRAM_BANKS[idx].size;
}
