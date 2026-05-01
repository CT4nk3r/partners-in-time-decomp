/*
 * nds_vram.c - VRAM bank emulation
 *
 * The Nintendo DS has 9 VRAM banks (A-I) totaling 656 KB. Each can be mapped
 * to different purposes (BG, OBJ, ARM7, texture, palette) via VRAMCNT.
 * On PC we just allocate them as host memory and ignore mapping — the
 * decompiled code can still write/read by address.
 */
#include "nds_platform.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* NDS VRAM bank sizes */
static const struct {
    char     id;
    uint32_t size;
} VRAM_BANKS[] = {
    { 'A', 128 * 1024 },
    { 'B', 128 * 1024 },
    { 'C', 128 * 1024 },
    { 'D', 128 * 1024 },
    { 'E',  64 * 1024 },
    { 'F',  16 * 1024 },
    { 'G',  16 * 1024 },
    { 'H',  32 * 1024 },
    { 'I',  16 * 1024 },
};

static uint8_t* g_banks[9];
static int g_initialized = 0;

static int bank_index(char bank) {
    if (bank >= 'a' && bank <= 'i') bank -= ('a' - 'A');
    if (bank < 'A' || bank > 'I') return -1;
    return bank - 'A';
}

static void ensure_init(void) {
    if (g_initialized) return;
    for (int i = 0; i < 9; i++) {
        g_banks[i] = (uint8_t*)calloc(1, VRAM_BANKS[i].size);
    }
    g_initialized = 1;
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
