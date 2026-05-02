/*
 * host_game_setup_overlay.c — HOST_PORT shim for game_setup_overlay().
 *
 * On the original ROM, game_setup_overlay (FUN_02004EF8) loads the
 * initial scene overlay and constructs the scene object.  That object's
 * vtable is in main ARM9 .data and its scene-jump dispatch ultimately
 * hits *DAT_02005d68 (set to ARM9 0x0202A56C on real hardware,
 * stubbed to host_scene_jmp_trampoline in our build).
 *
 * Until FUN_02004EF8 itself is decompiled we provide a minimal host
 * shim that performs the smallest sequence sufficient to cause the
 * scene_jmp pointer to be exercised, so we can verify (Task 4 of the
 * shadow-buffers-dma session) whether scene dispatch ever fires.
 *
 * Strategy:
 *   1. Allocate a fake scene-object buffer (>= 0x40 bytes so writes at
 *      offsets +0x10 and +0x28 are in-bounds).
 *   2. Call FUN_02005d54(scene, 0) which is the smallest direct path
 *      from real decomp into (*DAT_02005d68)().
 *
 * We log everything; any segfault in the trampoline cascade will be
 * caught by the SIGSEGV protector in game_thread.c because this shim
 * runs from inside game_start() while the protector is armed.
 */
#include "types.h"

#include <stdio.h>
#include <string.h>

extern void FUN_02005d54(int param_1, u8 param_2);
extern int  nds_arm9_ram_is_mapped(void);

/* Use a region of our VirtualAlloc'd 4 MiB ARM9 RAM (which lives at
 * NDS address 0x02000000 on Win64 too).  The decompiled signature
 * `FUN_02005d54(int param_1, ...)` truncates host 64-bit pointers to
 * 32-bit, so the buffer MUST sit at an address that survives the
 * truncation — i.e. inside the [0x02000000..0x023FFFFF] mapping.
 *
 * arm9.bin code+data ends near 0x0205C000.  We pick 0x02300000 which
 * is well past anything game_init touched and stays inside the 4 MiB
 * VirtualAlloc region. */
#define HOST_FAKE_SCENE_NDS_ADDR  0x02300000u
#define HOST_FAKE_SCENE_SIZE      0x80u

static int g_setup_call_count = 0;

void game_setup_overlay(u32 overlay_id, u32 flags)
{
    g_setup_call_count++;
    fflush(stderr);
    fprintf(stderr,
            "[setup_overlay] call#%d overlay_id=0x%08X flags=0x%08X\n",
            g_setup_call_count, (unsigned)overlay_id, (unsigned)flags);
    fflush(stderr);

    /* Only fire scene dispatch on the first call (game_start passes
     * 0xFFFFFFFF / 0).  Re-entrancy from other call sites we simply
     * log and ignore. */
    if (g_setup_call_count > 1) return;

    if (!nds_arm9_ram_is_mapped()) {
        fprintf(stderr,
                "[setup_overlay] arm9 RAM not mapped — cannot place "
                "fake scene at NDS-low address; skipping dispatch\n");
        return;
    }

    void *fake_scene = (void *)(uintptr_t)HOST_FAKE_SCENE_NDS_ADDR;
    memset(fake_scene, 0, HOST_FAKE_SCENE_SIZE);

    fprintf(stderr,
            "[setup_overlay] invoking FUN_02005d54(fake_scene=0x%08X, 0)\n",
            (unsigned)HOST_FAKE_SCENE_NDS_ADDR);
    fflush(stderr);

    FUN_02005d54((int)HOST_FAKE_SCENE_NDS_ADDR, 0);

    fprintf(stderr, "[setup_overlay] FUN_02005d54 returned cleanly\n");
    fflush(stderr);
}
