/*
 * nds_arm9_ram.c - Map ARM9 main RAM at NDS address 0x02000000.
 *
 * On real DS hardware, the cartridge boot loader copies arm9.bin from
 * ROM into ARM9 main RAM at the load address recorded in the NDS
 * header (0x02004000 for this title). Decompiled C that reads through
 * raw pointer literals - e.g. `*(volatile u32*)0x020055B4` - therefore
 * sees the .data initial values that the linker baked into arm9.bin.
 *
 * Our HOST_PORT build links each `DAT_<addr>` symbol as a host-side C
 * global at whatever virtual address the loader chooses, so that path
 * is broken UNLESS we can make 0x02000000..0x023FFFFF a real, mapped,
 * writable region on the host process. On Windows we can: just call
 * VirtualAlloc with a fixed base. (Linux: future MAP_FIXED via mmap.)
 *
 * We then memcpy `extracted/arm9.bin` into 0x02004000 so any decomp
 * path that still computes a raw NDS pointer literal sees real bytes.
 *
 * Legality: arm9.bin is the user's own dump. It is NEVER bundled with
 * the source tree; we read it at runtime from the user's filesystem.
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

#define ARM9_NDS_BASE         0x02000000u
#define ARM9_NDS_SIZE         (4u * 1024u * 1024u)
#define ARM9_NDS_LOAD_OFFSET  0x00004000u   /* arm9.bin loads at 0x02004000 */

static int   g_arm9_ram_mapped = 0;
static void *g_arm9_ram_base   = NULL;

void *nds_arm9_ram_base(void) { return g_arm9_ram_base; }
int   nds_arm9_ram_is_mapped(void) { return g_arm9_ram_mapped; }

static const char *kArm9CandidatePaths[] = {
    "extracted/arm9.bin",
    "../extracted/arm9.bin",
    "../../extracted/arm9.bin",
    "../../../extracted/arm9.bin",
    NULL,
};

void nds_arm9_ram_init(void)
{
    if (g_arm9_ram_mapped) return;

#ifdef _WIN32
    void *p = VirtualAlloc((LPVOID)(uintptr_t)ARM9_NDS_BASE,
                           ARM9_NDS_SIZE,
                           MEM_COMMIT | MEM_RESERVE,
                           PAGE_READWRITE);
    if (!p) {
        nds_log("[arm9] VirtualAlloc at 0x%08X failed (err=%lu) - "
                "raw NDS-address reads will SIGSEGV\n",
                ARM9_NDS_BASE, (unsigned long)GetLastError());
        return;
    }
    if ((uintptr_t)p != (uintptr_t)ARM9_NDS_BASE) {
        nds_log("[arm9] VirtualAlloc returned 0x%p (wanted 0x%08X) - "
                "fixed-address mapping unavailable\n",
                p, ARM9_NDS_BASE);
        VirtualFree(p, 0, MEM_RELEASE);
        return;
    }
    g_arm9_ram_base = p;
    memset(p, 0, ARM9_NDS_SIZE);
#else
    /* Non-Windows: stub - decomp via raw pointer literal will crash. */
    nds_log("[arm9] fixed mapping not implemented for this platform\n");
    return;
#endif

    /* Locate and load arm9.bin from the user's extracted ROM tree. */
    FILE *f = NULL;
    const char *used_path = NULL;
    for (int i = 0; kArm9CandidatePaths[i]; ++i) {
        f = fopen(kArm9CandidatePaths[i], "rb");
        if (f) { used_path = kArm9CandidatePaths[i]; break; }
    }
    if (!f) {
        nds_log("[arm9] extracted/arm9.bin not found - "
                "0x%08X mapped but ZERO-FILLED\n", ARM9_NDS_BASE);
        g_arm9_ram_mapped = 1;
        return;
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0 || (uint32_t)sz > ARM9_NDS_SIZE - ARM9_NDS_LOAD_OFFSET) {
        nds_log("[arm9] arm9.bin size %ld out of range\n", sz);
        fclose(f);
        g_arm9_ram_mapped = 1;
        return;
    }

    uint8_t *dst = (uint8_t *)g_arm9_ram_base + ARM9_NDS_LOAD_OFFSET;
    size_t got = fread(dst, 1, (size_t)sz, f);
    fclose(f);

    nds_log("[arm9] mapped %zu bytes from %s at 0x%08X (NDS base 0x%08X)\n",
            got, used_path,
            ARM9_NDS_BASE + ARM9_NDS_LOAD_OFFSET,
            ARM9_NDS_BASE);
    g_arm9_ram_mapped = 1;
}
