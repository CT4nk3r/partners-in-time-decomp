/*
 * host_fnptr_resolver.c — NDS function-pointer → host symbol trampoline.
 *
 * The NDS game stores ARM-domain code pointers (values like 0x0202xxxx)
 * in vtables and DAT_ slots. On x86_64 we cannot jump to those addresses;
 * we have to translate them to host C function pointers obtained via
 * the compiler/linker.
 *
 * Registration happens once at startup via host_fnptr_register_decompiled()
 * (auto-generated, see tools/scripts/gen_fnptr_table.py). The runtime
 * call sites use:
 *   - host_fnptr_lookup(nds_addr): NULL if unknown
 *   - host_fnptr_register(nds_addr, fn): manual registration (e.g. for
 *     synthetic host trampolines mapped under reserved 0xFAxxxxxx range)
 *   - nds_call_2arg(nds_addr, a, b): canonical wrapper used by the
 *     scene-queue dispatcher.
 *
 * Unknown addresses are logged once per unique value to keep the log
 * useful even when a single missing function fires every frame.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNPTR_TABLE_CAP 4096

typedef struct {
    uint32_t nds_addr;
    void *host_fn;
} fnptr_entry_t;

static fnptr_entry_t g_table[FNPTR_TABLE_CAP];
static int g_table_count = 0;
static int g_initialised = 0;

/* Safe-native tracking: addresses registered via host_fnptr_register_overrides()
 * are known safe to call natively (no NDS pointer DAT issues). */
#define SAFE_NATIVE_CAP 256
static uint32_t g_safe_native[SAFE_NATIVE_CAP];
static int g_safe_native_count = 0;

int host_fnptr_is_safe_native(uint32_t addr) {
    for (int i = 0; i < g_safe_native_count; i++) {
        if (g_safe_native[i] == addr) return 1;
    }
    return 0;
}

static void register_safe(uint32_t nds_addr, void *host_fn) {
    host_fnptr_register(nds_addr, host_fn);
    if (g_safe_native_count < SAFE_NATIVE_CAP)
        g_safe_native[g_safe_native_count++] = nds_addr;
}

#define UNMAPPED_LOG_CAP 256
static uint32_t g_unmapped_logged[UNMAPPED_LOG_CAP];
static int g_unmapped_count = 0;

void host_fnptr_register(uint32_t nds_addr, void *host_fn)
{
    if (!host_fn) return;
    if (g_table_count >= FNPTR_TABLE_CAP) {
        static int warned = 0;
        if (!warned) {
            warned = 1;
            fprintf(stderr, "[fnptr] table FULL (cap=%d) — dropping 0x%08x\n",
                    FNPTR_TABLE_CAP, (unsigned)nds_addr);
        }
        return;
    }
    /* O(N) dedupe: a duplicate registration replaces the existing slot. */
    for (int i = 0; i < g_table_count; ++i) {
        if (g_table[i].nds_addr == nds_addr) {
            g_table[i].host_fn = host_fn;
            return;
        }
    }
    g_table[g_table_count].nds_addr = nds_addr;
    g_table[g_table_count].host_fn  = host_fn;
    g_table_count++;
}

extern void host_fnptr_register_decompiled(void); /* generated */

/* Forward declaration — used by native function overrides below */
void *host_fnptr_lookup(uint32_t nds_addr);

/* Manual overrides for functions whose alias points to a no-op but
 * whose real implementation exists under a different name. */
extern void FUN_0202a74c_real(uint32_t, uint8_t, uint32_t, uint32_t);

/* FUN_02007590: simple setter — stores r0 at DAT_02059C98, r1 (byte) at DAT_02059C80.
 * Disassembly: LDR R3,=0x02059C98; LDR R2,=0x02059C80; STR R0,[R3]; STRB R1,[R2]; BX LR */
static void native_FUN_02007590(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r2; (void)r3;
    *(volatile uint32_t *)(uintptr_t)0x02059C98u = r0;
    *(volatile uint8_t  *)(uintptr_t)0x02059C80u = (uint8_t)r1;
}

/* FUN_02035B28: ORs r0 into halfword at DAT_020628D0, then tail-calls FUN_02036430.
 * Disassembly: LDR R1,=0x020628D0; LDR R12,=0x02036430; LDRH R2,[R1];
 *              ORR R2,R2,R0; STRH R2,[R1]; BX R12 */
static void native_FUN_02035B28(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r1; (void)r2; (void)r3;
    volatile uint16_t *flags = (volatile uint16_t *)(uintptr_t)0x020628D0u;
    *flags = (uint16_t)(*flags | (uint16_t)r0);
    /* Tail call to FUN_02036430 — try native, skip if not found */
    void *fn = host_fnptr_lookup(0x02036430u);
    if (fn) ((void (*)(uint32_t, uint32_t, uint32_t, uint32_t))fn)(r0, 0, 0, 0);
}

/* --- Stubs for 7 arm9-base functions the interpreter was skipping --- */

/* FUN_0203564C: 3-instruction trampoline.
 * LDR IP,=0x0203570C; LDR R0,=0x020628D0; BX IP
 * Loads data addr into r0 and tail-calls target. */
static uint32_t native_FUN_0203564C(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0;
    void *fn = host_fnptr_lookup(0x0203570Cu);
    if (fn) return ((uint32_t (*)(uint32_t,uint32_t,uint32_t,uint32_t))fn)(
                     0x020628D0u, r1, r2, r3);
    return 0;
}

/* FUN_020371CC: GX graphics pipeline init (~60 instructions).
 * Calls GX setup, clears GXSTAT/END_VTXS/DISP3DCNT registers, polls GX
 * status.  On PC we set registers to "ready/idle" state and return. */
static uint32_t native_FUN_020371CC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    *(volatile uint32_t *)(uintptr_t)0x04000504u = 0;   /* END_VTXS clear */
    *(volatile uint32_t *)(uintptr_t)0x04000600u = 0;   /* GXSTAT: idle */
    *(volatile uint32_t *)(uintptr_t)0x04000060u = 0;   /* DISP3DCNT: default */
    return 0;
}

/* FUN_02036E64: Resource init/cleanup (~80 instructions).
 * Checks a resource handle; if -1 (uninitialised), does nothing.
 * Safe as no-op since the resource is never opened on PC. */
static uint32_t native_FUN_02036E64(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02037060: GX sync/wait.
 * Sets bit 15 in GXSTAT, polls two wait functions, then writes to
 * MTX_PUSH/MTX_RESTORE.  On PC, just mark GX as idle. */
static uint32_t native_FUN_02037060(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    *(volatile uint32_t *)(uintptr_t)0x04000600u = 0;   /* GXSTAT: idle */
    return 0;
}

/* FUN_02036F60: Display/DMA control (conditional).
 * If r0==0: bit-mask read-modify-write on DISP3DCNT (0x04000060).
 * If r0!=0: configure DISP3DCNT + DMA3SAD (0x0400035C).
 * On PC, write expected values to IO shadow. */
static uint32_t native_FUN_02036F60(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    if (r0 == 0) {
        /* Early path: clear some bits in DISP3DCNT */
        volatile uint16_t *d3d = (volatile uint16_t *)(uintptr_t)0x04000060u;
        *d3d = (uint16_t)(*d3d & 0xCF7F);
        return 0;
    }
    /* Main path: set DISP3DCNT with combined params */
    uint32_t val = (r2 << 8) | (r1 << 6) | 0x80;
    volatile uint16_t *d3d = (volatile uint16_t *)(uintptr_t)0x04000060u;
    uint16_t cur = *d3d;
    cur = (uint16_t)((cur & ~0x3F40u) | val);
    *d3d = cur;
    *(volatile uint16_t *)(uintptr_t)0x0400035Cu = (uint16_t)r3;
    return 0;
}

/* FUN_02036F1C: DMA3 register setup (10 instructions).
 * Combines r0|r1<<16|r3<<24, conditionally sets bit 15, writes to
 * 0x04000350 (DMA3DAD) and 0x04000354 (DMA3CNT).
 * 5th arg (on stack) controls bit 15 — assume 0 for PC. */
static uint32_t native_FUN_02036F1C(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    uint32_t combined = r0 | (r1 << 16) | (r3 << 24);
    *(volatile uint32_t *)(uintptr_t)0x04000350u = combined;
    *(volatile uint16_t *)(uintptr_t)0x04000354u = (uint16_t)r2;
    return 0;
}

/* FUN_02029518: OAM/render transfer.  Called from the display tick with
 * r0 = mode (typically 3).  On PC, nothing to transfer; return 0. */
static uint32_t native_FUN_02029518(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_020294B0: display-engine finalize / swap.  Called at the end of the
 * per-frame display tick (FUN_02065A10).  The title-screen tick checks its
 * return value — non-zero means "frame ready" and gates the entire
 * render / input path.  Return 1 so the game can proceed. */
static uint32_t native_FUN_020294B0(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 1;
}

/* FUN_02077A88: derived title-screen tick.
 * On real NDS, this function's body after BL 0x02065B80 is dead code
 * (the mid-function tail-call never returns).  Its only meaningful
 * effect is calling FUN_02065B80 which handles palette DMA, display mode
 * setup for the sub-scene.  The ARM interpreter can't handle the
 * tail-call (POP reads more stack than was pushed), so we skip it. */
static uint32_t native_FUN_02077A88(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r1; (void)r2; (void)r3;
    /* r0 = sub-scene object (node pointer).
     * On real NDS, BL 0x02065B80 enters FUN_02065A10 mid-function and
     * performs palette/display register writes using the scene object.
     * For now, return 0 (skip the display work — the base tick handles
     * the main display).  As we implement more overlay 5 display helpers,
     * this can be expanded. */
    static int s_log = 0;
    if (s_log < 5) {
        s_log++;
        fprintf(stderr, "[native] FUN_02077A88 (derived tick) node=0x%08X (no-op)\n",
                (unsigned)r0);
        fflush(stderr);
    }
    return 0;
}

/* ── Display pipeline functions called by base tick (FUN_02077444) ──
 * After the display gate (FUN_02065B5C) returns non-zero, the base tick
 * calls these in order.  FUN_02068CBC is already registered.  The rest
 * would run in the ARM interpreter and hit the 5M cycle timeout because
 * they loop on hardware registers that don't exist on the host.
 * Register them as native no-ops until we have real display pipeline. */

/* FUN_0206619C — display timing / VBlank sync.
 * On NDS, waits for specific timing conditions.  No-op on host. */
static uint32_t native_FUN_0206619C(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02077444 — title screen base tick (overlay 8).
 * The ARM interpreter version works correctly EXCEPT for the teardown path:
 * after field_2c is set to 1, the destructor (vtable[1]) doesn't properly
 * unlink the node from the scene queue, causing FUN_02005d3c to be called
 * every frame (resetting phase=2).  This native version handles teardown
 * once and then disables re-entry. */
static uint32_t native_FUN_02077444(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r1; (void)r2; (void)r3;
    uint32_t node = r0;
    if (!node || node < 0x02000000u || node >= 0x03000000u) return 0;

    uint32_t field_2c = *(volatile uint32_t *)(uintptr_t)(node + 0x2C);

    if (field_2c == 1) {
        /* Teardown: transition to next scene.  Do this only once. */
        *(volatile uint32_t *)(uintptr_t)(node + 0x2C) = 2;

        /* Physically unlink this node from the doubly-linked queue.
         * The queue dispatch code overwrites our flag changes (it uses
         * a local copy read before the call), so clearing bit0 doesn't
         * work.  We must remove the node from the linked list entirely. */
        {
            uint32_t prev_addr = *(volatile uint32_t *)(uintptr_t)(node + 0x08);
            uint32_t next_addr = *(volatile uint32_t *)(uintptr_t)(node + 0x0C);

            if (prev_addr >= 0x02000000u && prev_addr < 0x03000000u) {
                /* prev->next = our next */
                *(volatile uint32_t *)(uintptr_t)(prev_addr + 0x0C) = next_addr;
            } else {
                /* We're the head — update queue head pointer */
                #define QUEUE_HEAD_PTR 0x02060A04u
                *(volatile uint32_t *)(uintptr_t)QUEUE_HEAD_PTR = next_addr;
            }

            if (next_addr >= 0x02000000u && next_addr < 0x03000000u) {
                /* next->prev = our prev */
                *(volatile uint32_t *)(uintptr_t)(next_addr + 0x08) = prev_addr;
            }

            /* Clear our own pointers */
            *(volatile uint32_t *)(uintptr_t)(node + 0x08) = 0;
            *(volatile uint32_t *)(uintptr_t)(node + 0x0C) = 0;

            fprintf(stderr,
                    "[native_02077444] unlinked node 0x%08X (prev=0x%08X next=0x%08X)\n",
                    (unsigned)node, (unsigned)prev_addr, (unsigned)next_addr);
            fflush(stderr);
        }

        /* Get scene anchor and trigger scene change */
        uint32_t scene_anchor = *(volatile uint32_t *)(uintptr_t)0x02059C7Cu;
        if (scene_anchor >= 0x02000000u && scene_anchor < 0x03000000u) {
            extern void FUN_02005d3c(int, unsigned char);
            fprintf(stderr,
                    "[native_02077444] teardown: FUN_02005d3c(0x%08X, 2)\n",
                    (unsigned)scene_anchor);
            fflush(stderr);
            FUN_02005d3c((int)scene_anchor, 2);
        } else {
            fprintf(stderr,
                    "[native_02077444] teardown: bad scene_anchor=0x%08X\n",
                    (unsigned)scene_anchor);
            fflush(stderr);
        }
        return 0;
    }

    if (field_2c >= 2) return 0;  /* already torn down */

    /* Normal tick: check display ready, then dispatch callbacks.
     * On host, display is always ready (skip FUN_02065B5C check). */

    /* Dispatch per-frame callbacks (drives the title state machine).
     * FUN_0206619C(0, 10) dispatches 10 priority slots starting at 0. */
    extern void FUN_0206619c(int start, int end);
    FUN_0206619c(0, 10);

    return 0;
}

/* FUN_02068C60 — display layer commit (per-screen).
 * On NDS, flushes BG scroll/control registers.  No-op on host. */
static uint32_t native_FUN_02068C60(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02068B60 — OAM / sprite commit (per-screen).
 * On NDS, pushes OAM attribute buffer to hardware.  No-op on host. */
static uint32_t native_FUN_02068B60(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02009560 — palette / display mode finalize.
 * Called twice per frame (once per screen).  No-op on host. */
static uint32_t native_FUN_02009560(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02073EFC — animation/sound transition trigger in overlay 8.
 * Called from the title state machine (states 6, 7, and others) to
 * kick off screen fades and sound effects.  The deep overlay call
 * chain (→ 02073F64 → 02075160 → 02068028) hangs in the ARM
 * interpreter because sub-functions poll IO or loop on unimplemented
 * state.  Stub as no-op to let the state machine advance. */
static uint32_t native_FUN_02073EFC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* Generic no-op for stubbing overlay functions that crash due to
 * HOST_PORT / NDS memory mismatch (e.g. post-load init). */
static uint32_t native_noop(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02068028 — overlay 5 helper called from 02073EFC's call chain.
 * Likely a display command queue submission.  No-op. */
static uint32_t native_FUN_02068028(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02028CDC — loading/resource readiness check.
 * On NDS, checks if a DMA/resource load is complete.  State machine
 * state 0 calls this; if it returns 0, the state machine skips to
 * state 9 (retry), but state 9 assumes states 1-8 setup.  Return 1
 * (ready) so the state machine progresses normally through states 1→2→... */
static uint32_t native_FUN_02028CDC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 1;
}

/* FUN_02028b80 — save/EEPROM write operation.
 * Calls FUN_020285c8 (which calls FUN_02028710 → returns 0 on PC)
 * then FUN_02028440 (returns 0 on PC).  With both returning 0 the
 * result is FALSE, causing states 7/8 to loop endlessly.
 * Override to return 1 (TRUE = save op succeeded) so the title
 * state machine can transition: state 7→12 or state 8→12. */
static uint32_t native_FUN_02028b80(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 1;
}

/* FUN_02028C20 — save data header/magic check.
 * Compares 6 bytes of a magic string, then checks byte 7 == 7.
 * Returns 1 (valid), 0 (version mismatch), or -1 (magic mismatch).
 * Without real save data this returns 0 → state machine bails to 0xE.
 * Return 1 so the title screen state machine continues normally. */
static uint32_t native_FUN_02028C20(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 1;
}

/* FUN_02028D24 — save data initialisation (reads EEPROM, checksums, copy).
 * The decompiled C body dereferences uninitialised save pointers and crashes.
 * Stub returns 1 (success) so state 1 advances to state 2.
 * Also fills the save data header area (0x0205DFAC..0x0205DFCC) with 0xFF
 * to simulate blank EEPROM — state 2's bit manipulation reads byte at
 * 0x0205DFB4 and ANDs with a mask; 0xFF ensures all bits pass. */
static uint32_t native_FUN_02028D24(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    /* Fill save header region with 0xFF (blank EEPROM default) */
    MI_CpuFill8((void *)(uintptr_t)0x0205DFACu, 0xFF, 0x20);
    return 1;
}

/* FUN_02028B04 — save header CRC check (gx_check_header).
 * Must return 0 so state 1 executes the setup code that writes field_28
 * bit 1 (checked by state 3 to decide state 4 vs state 0xD error). */
static uint32_t native_FUN_02028B04(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02028B34 — clear 20 bytes of save CRC buffer.
 * The decompiled body does ((void(*)(int,...))DAT_02028b48)(DAT_02028b4c,0,0x14)
 * which is an indirect call through a u32 DAT holding MI_CpuFill8's NDS addr.
 * On 64-bit host the function pointer gets truncated → crash.
 * Override: call MI_CpuFill8 directly on the NDS-mapped buffer. */
static uint32_t native_FUN_02028B34(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    /* DAT_02028b4c = 0x0205DFB8, size = 0x14 */
    MI_CpuFill8((void *)(uintptr_t)0x0205DFB8u, 0, 0x14);
    return 0;
}

/* FUN_020289F4 — gx_validate_buffer (display buffer CRC check).
 * Named gx_validate_buffer in gx_display.c so NOT auto-registered
 * in the fnptr table.  Must return 0 (invalid) so the title screen
 * state machine's fallback path in state 2 sets field_28 bit 1.
 * (Returning 1 takes the success path which only touches save buffers,
 * not field_28 — and state 3 needs field_28 >> 1 to be nonzero.) */
static uint32_t native_FUN_020289F4(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 0;
}

/* FUN_02028AB4 — save slot validation (calls FUN_020285c8 + FUN_02028440).
 * Return 1 (valid) so state 1 setup code completes successfully. */
static uint32_t native_FUN_02028AB4(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r1; (void)r2; (void)r3;
    return 1;
}

/* ── SDK memory function wrappers for ARM interpreter ──
 * These decompiled functions live in arm9/src/mem_util.c but the ARM
 * interpreter needs them registered by NDS address.  Wrappers convert
 * uint32_t NDS addresses (in r0-r3) to host pointers via (uintptr_t)
 * — this works because NDS RAM is mmap'd at the NDS address range. */
extern void MI_CpuFill8(void *dest, unsigned char fill, uint32_t size);
extern void MI_CpuCopy8(const void *src, void *dest, uint32_t size);

/* Helper: check if an NDS address is in a VirtualAlloc'd region */
static int nds_addr_valid(uint32_t addr, uint32_t size) {
    uint32_t end = addr + size;
    /* ARM9 main RAM: 0x02000000-0x02FFFFFF */
    if (addr >= 0x02000000u && end <= 0x03000000u) return 1;
    /* VRAM: 0x06000000-0x06900000 */
    if (addr >= 0x06000000u && end <= 0x06900000u) return 1;
    /* Palette: 0x05000000-0x05001000 */
    if (addr >= 0x05000000u && end <= 0x05001000u) return 1;
    /* IO: 0x04000000-0x04001000 */
    if (addr >= 0x04000000u && end <= 0x04001000u) return 1;
    /* OAM: 0x07000000-0x07000800 */
    if (addr >= 0x07000000u && end <= 0x07000800u) return 1;
    /* WRAM: 0x01FF0000-0x02000000 */
    if (addr >= 0x01FF0000u && end <= 0x02000000u) return 1;
    /* Host heap (OS_Alloc arena): 0x10000000+ */
    if (addr >= 0x10000000u && end <= 0x20000000u) return 1;
    return 0;
}

static uint32_t native_MI_CpuFill8(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2)) return 0;
    /* Watchpoint: detect if MI_CpuFill8 overwrites 0x0230200C */
    if (r0 <= 0x0230200Cu && r0 + r2 > 0x0230200Cu) {
        fprintf(stderr, "[WATCH-0C] MI_CpuFill8(0x%08X, 0x%02X, 0x%X) hits 0x0230200C\n",
                (unsigned)r0, (unsigned)(r1 & 0xFF), (unsigned)r2);
        fflush(stderr);
    }
    MI_CpuFill8((void *)(uintptr_t)r0, (unsigned char)r1, r2);
    return 0;
}

static uint32_t native_MI_CpuCopy8(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2) || !nds_addr_valid(r1, r2)) return 0;
    MI_CpuCopy8((const void *)(uintptr_t)r0, (void *)(uintptr_t)r1, r2);
    return 0;
}

/* MI_CpuFill32Fast / MI_CpuCopy16 / MI_CpuFill16 — similar wrappers */
extern void MI_CpuFill32Fast(void *dest, uint32_t fill, uint32_t size);
extern void sdk_mi_cpu_copy16(void *dest, const void *src, uint32_t size);
extern void sdk_mi_cpu_fill16(void *dest, uint16_t fill, uint32_t size);

static uint32_t native_MI_CpuFill32Fast(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2)) return 0;
    MI_CpuFill32Fast((void *)(uintptr_t)r0, r1, r2);
    return 0;
}

static uint32_t native_sdk_mi_cpu_copy16(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2) || !nds_addr_valid(r1, r2)) return 0;
    sdk_mi_cpu_copy16((void *)(uintptr_t)r0, (const void *)(uintptr_t)r1, r2);
    return 0;
}

static uint32_t native_sdk_mi_cpu_fill16(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2)) return 0;
    sdk_mi_cpu_fill16((void *)(uintptr_t)r0, (uint16_t)r1, r2);
    return 0;
}

/* MI_Fill32 / MI_Copy32 — simple 32-bit memory ops (mem_simple.c).
 * These are called with NDS addresses that may be corrupted (uninit
 * pointers in unexercised code paths).  Validate before writing. */
extern void MI_Fill32(uint32_t value, uint32_t *dest, int size);
extern void MI_Copy32(const uint32_t *src, uint32_t *dest, int size);

static uint32_t native_MI_Fill32(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r1, r2)) {
        static int warned = 0;
        if (!warned) { warned = 1; fprintf(stderr, "[mem] MI_Fill32 bad dest 0x%08X size 0x%X — skipped\n", r1, r2); }
        return 0;
    }
    MI_Fill32(r0, (uint32_t *)(uintptr_t)r1, (int)r2);
    return 0;
}

static uint32_t native_MI_Copy32(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    if (!nds_addr_valid(r0, r2) || !nds_addr_valid(r1, r2)) {
        static int warned = 0;
        if (!warned) { warned = 1; fprintf(stderr, "[mem] MI_Copy32 bad src/dest 0x%08X/0x%08X size 0x%X — skipped\n", r0, r1, r2); }
        return 0;
    }
    MI_Copy32((const uint32_t *)(uintptr_t)r0, (uint32_t *)(uintptr_t)r1, (int)r2);
    return 0;
}

/* OS_AllocFromHeap / OS_Alloc — heap allocators */
extern void *OS_AllocFromHeap(uint32_t arena, uint32_t heap, uint32_t size);
extern void *OS_Alloc(uint32_t size);
extern void sdk_os_free(void *ptr);

static uint32_t native_OS_AllocFromHeap(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r3;
    void *p = OS_AllocFromHeap(r0, r1, r2);
    return (uint32_t)(uintptr_t)p;
}

static uint32_t native_OS_Alloc(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r1; (void)r2; (void)r3;
    void *p = OS_Alloc(r0);
    return (uint32_t)(uintptr_t)p;
}

static uint32_t native_sdk_os_free(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r1; (void)r2; (void)r3;
    sdk_os_free((void *)(uintptr_t)r0);
    return 0;
}

/* FUN_0202b9b8 — partition size calculator (5 ARM instructions)
 * R0=unused, R1=u32* array, R2=index
 * Returns array[index+1] - array[index] */
static uint32_t native_FUN_0202b9b8(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
    (void)r0; (void)r3;
    if (r1 == 0) return 0;  /* guard against NULL array */
    uint32_t *arr = (uint32_t *)(uintptr_t)r1;
    return arr[r2 + 1] - arr[r2];
}

static void host_fnptr_register_overrides(void)
{
    /* All registrations here use register_safe() so the ARM interpreter
     * knows these are hand-verified safe for native calls (no NDS pointer
     * DAT issues).  Auto-generated fnptr entries are NOT safe and will
     * be interpreted instead. */
    register_safe(0x0202a74cu, (void *)FUN_0202a74c_real);
    register_safe(0x02007590u, (void *)native_FUN_02007590);
    register_safe(0x02035B28u, (void *)native_FUN_02035B28);

    /* 6 previously-skipped arm9-base functions (GX/DMA/resource stubs) */
    register_safe(0x0203564Cu, (void *)native_FUN_0203564C);
    register_safe(0x020371CCu, (void *)native_FUN_020371CC);
    register_safe(0x02036E64u, (void *)native_FUN_02036E64);
    register_safe(0x02037060u, (void *)native_FUN_02037060);
    register_safe(0x02036F60u, (void *)native_FUN_02036F60);
    register_safe(0x02036F1Cu, (void *)native_FUN_02036F1C);

    /* Render pipeline stubs */
    register_safe(0x02029518u, (void *)native_FUN_02029518);
    register_safe(0x020294B0u, (void *)native_FUN_020294B0);

    /* Derived title-screen tick */
    register_safe(0x02077A88u, (void *)native_FUN_02077A88);

    /* Title screen base tick */
    register_safe(0x02077444u, (void *)native_FUN_02077444);

    /* Display pipeline */
    register_safe(0x02068C60u, (void *)native_FUN_02068C60);
    register_safe(0x02068B60u, (void *)native_FUN_02068B60);
    register_safe(0x02009560u, (void *)native_FUN_02009560);

    /* Overlay 8 helpers */
    register_safe(0x02073EFCu, (void *)native_FUN_02073EFC);
    register_safe(0x02068028u, (void *)native_FUN_02068028);
    register_safe(0x02028b80u, (void *)native_FUN_02028b80);

    /* Title screen state machine helpers */
    register_safe(0x02028CDCu, (void *)native_FUN_02028CDC);
    register_safe(0x02028C20u, (void *)native_FUN_02028C20);
    register_safe(0x02028D24u, (void *)native_FUN_02028D24);
    register_safe(0x02028B04u, (void *)native_FUN_02028B04);
    register_safe(0x02028B34u, (void *)native_FUN_02028B34);
    register_safe(0x020289F4u, (void *)native_FUN_020289F4);
    register_safe(0x02028AB4u, (void *)native_FUN_02028AB4);

    /* SDK memory/heap functions */
    register_safe(0x0203b914u, (void *)native_MI_CpuFill8);
    register_safe(0x0203b9a8u, (void *)native_MI_CpuCopy8);
    register_safe(0x0202cc10u, (void *)native_MI_CpuFill32Fast);
    register_safe(0x0202cc94u, (void *)native_sdk_mi_cpu_copy16);
    register_safe(0x0202cd68u, (void *)native_sdk_mi_cpu_fill16);
    register_safe(0x02029964u, (void *)native_OS_AllocFromHeap);
    register_safe(0x02029c1cu, (void *)native_OS_Alloc);
    register_safe(0x02029ab8u, (void *)native_sdk_os_free);
    register_safe(0x0203b7dcu, (void *)native_MI_Fill32);
    register_safe(0x0203b7f0u, (void *)native_MI_Copy32);

    /* Partition size calculator */
    register_safe(0x0202b9b8u, (void *)native_FUN_0202b9b8);

    /* Post-load init stub (HOST_PORT/NDS mismatch) */
    register_safe(0x0206D24Cu, (void *)native_noop);

    /* Animation tick stub (NDS pointer DATs) */
    register_safe(0x02018748u, (void *)native_noop);

    /* 0x02010CCC (data struct init) is whitelisted for interpretation
     * in arm_interp.c instead — it's pure memory ops + memset calls. */
}

void host_fnptr_init(void)
{
    if (g_initialised) return;
    g_initialised = 1;
    host_fnptr_register_decompiled();
    host_fnptr_register_overrides();
    fprintf(stderr, "[fnptr] resolver initialised, %d entries registered\n",
            g_table_count);
}

void *host_fnptr_lookup(uint32_t nds_addr)
{
    if (!g_initialised) host_fnptr_init();
    /* Linear scan — table is < 1024 entries and lookups are rare
     * (vtable[2] dispatch ~once per scene node per frame). If this
     * shows up in profiling, replace with a hash table. */
    for (int i = 0; i < g_table_count; ++i) {
        if (g_table[i].nds_addr == nds_addr) {
            return g_table[i].host_fn;
        }
    }
    return NULL;
}

static void log_unmapped_once(uint32_t nds_addr, const char *ctx)
{
    for (int i = 0; i < g_unmapped_count; ++i) {
        if (g_unmapped_logged[i] == nds_addr) return;
    }
    if (g_unmapped_count < UNMAPPED_LOG_CAP) {
        g_unmapped_logged[g_unmapped_count++] = nds_addr;
    }
    fprintf(stderr, "[fnptr] unmapped 0x%08x (%s)\n",
            (unsigned)nds_addr, ctx ? ctx : "");
    fflush(stderr);
}

typedef void (*fn_2arg_t)(uintptr_t, uintptr_t);

/* ARM interpreter fallback for unmapped functions */
extern uint32_t arm_interp_call(uint32_t nds_addr, uint32_t arg0, uint32_t arg1,
                                uint32_t arg2, uint32_t arg3);
extern int arm_interp_is_enabled(void);

void nds_call_2arg(uint32_t nds_addr, uintptr_t a, uintptr_t b)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (fn) {
        ((fn_2arg_t)fn)(a, b);
        return;
    }
    /* Fall through to ARM interpreter */
    if (arm_interp_is_enabled()) {
        arm_interp_call(nds_addr, (uint32_t)a, (uint32_t)b, 0, 0);
        return;
    }
    log_unmapped_once(nds_addr, "2arg");
}

typedef void (*fn_1arg_t)(uintptr_t);

void nds_call_1arg(uint32_t nds_addr, uintptr_t a)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (fn) {
        ((fn_1arg_t)fn)(a);
        return;
    }
    if (arm_interp_is_enabled()) {
        arm_interp_call(nds_addr, (uint32_t)a, 0, 0, 0);
        return;
    }
    log_unmapped_once(nds_addr, "1arg");
}

typedef void (*fn_0arg_t)(void);
typedef void (*fn_4arg_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);

void nds_call_0arg(uint32_t nds_addr)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (fn) {
        ((fn_0arg_t)fn)();
        return;
    }
    if (arm_interp_is_enabled()) {
        arm_interp_call(nds_addr, 0, 0, 0, 0);
        return;
    }
    log_unmapped_once(nds_addr, "0arg");
}

void nds_call_4arg(uint32_t nds_addr, uintptr_t a, uintptr_t b,
                    uintptr_t c, uintptr_t d)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (fn) {
        ((fn_4arg_t)fn)(a, b, c, d);
        return;
    }
    if (arm_interp_is_enabled()) {
        arm_interp_call(nds_addr, (uint32_t)a, (uint32_t)b,
                        (uint32_t)c, (uint32_t)d);
        return;
    }
    log_unmapped_once(nds_addr, "4arg");
}

int host_fnptr_count(void)
{
    return g_table_count;
}
