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

static void host_fnptr_register_overrides(void)
{
    /* FUN_0202a74c is aliased to sdk_os_destroy_heap (no-op) for native
     * callers with wrong 0-arg signature.  The real implementation is
     * FUN_0202a74c_real (scene queue insert, 4 args).  The ARM interpreter
     * calls with r0-r3 so it needs the real function. */
    host_fnptr_register(0x0202a74cu, (void *)FUN_0202a74c_real);
    host_fnptr_register(0x02007590u, (void *)native_FUN_02007590);
    host_fnptr_register(0x02035B28u, (void *)native_FUN_02035B28);

    /* 6 previously-skipped arm9-base functions (GX/DMA/resource stubs) */
    host_fnptr_register(0x0203564Cu, (void *)native_FUN_0203564C);
    host_fnptr_register(0x020371CCu, (void *)native_FUN_020371CC);
    host_fnptr_register(0x02036E64u, (void *)native_FUN_02036E64);
    host_fnptr_register(0x02037060u, (void *)native_FUN_02037060);
    host_fnptr_register(0x02036F60u, (void *)native_FUN_02036F60);
    host_fnptr_register(0x02036F1Cu, (void *)native_FUN_02036F1C);
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
