/*
 * host_dat_fnptr_patch.c — Patch DAT function-pointer globals after init.
 *
 * Several decompiled modules declare `extern void (*DAT_XXXX)(...)` for
 * globals that store NDS function addresses in the ROM .data section.
 * After nds_apply_dat_inits() populates them with raw NDS addresses
 * (e.g. 0x0203F6E4), those values are NOT executable host code.
 *
 * This file overwrites each known DAT function-pointer with the host-side
 * symbol.  Functions without a host implementation get a safe no-op stub
 * so indirect calls don't crash into NDS memory.
 *
 * Call host_dat_fnptr_patch() once, after nds_apply_dat_inits() and before
 * any game code that may invoke these indirect calls.
 */
#include "nds_platform.h"
#include <stdint.h>
#include <stdio.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/* ------------------------------------------------------------------
 * Forward declarations for host implementations (decompiled or aliased)
 * ------------------------------------------------------------------ */
extern u32  FUN_0203f6e4(u32, u32);       /* sdk_fs.c: verify/finalize file */
extern void FUN_0202c860(void);            /* gx_util.c */
extern u32  FUN_0202f044(u32);             /* sdk_math.c */
extern u32  FUN_0202e5d0(void);            /* sdk_io.c */
extern void FUN_02039014(void);            /* hw_gx.c */
extern void FUN_0203e154(void);            /* sdk_fs.c */
extern void FUN_02042650(void);            /* sdk_init_b.c */
extern void MI_CpuFill32Fast(u32 *, u32, int); /* mem_util.c */
extern void os_timer_dispatch(u32, u32);   /* os_system.c (FUN_0203f378) */

/* These don't have host implementations yet — use safe no-op stubs. */
static void   stub_void(void) {}
static u32    stub_u32(void)  { return 0; }
static int    stub_int(int a) { return 0; }

/* ------------------------------------------------------------------
 * Patch table: each entry maps a DAT address to a host function.
 *
 * The DAT globals are declared as uintptr_t in host_undefined_stubs.c.
 * We cast and overwrite them with host function pointers.  Because
 * sizeof(uintptr_t) == sizeof(void*) on all target platforms, this is
 * safe — the decompiled code's `extern void (*DAT_XXXX)(...)` sees the
 * correct host pointer.
 * ------------------------------------------------------------------ */

/* Declare as extern uintptr_t so we can write to the same storage. */
#define DECL_DAT(addr) extern uintptr_t DAT_##addr

DECL_DAT(0202b940);  /* -> FUN_0203f6e4 */
DECL_DAT(0202b6c0);  /* -> FUN_0203f378 */
DECL_DAT(0202c9bc);  /* -> FUN_0202c860 */
DECL_DAT(02007640);  /* -> MI_CpuFill32Fast (FUN_0202cc10) */
DECL_DAT(0202f698);  /* -> FUN_0202f044 */
DECL_DAT(02031054);  /* -> FUN_0202e5d0 */
DECL_DAT(0202e320);  /* -> FUN_02043ed8 (no impl -> stub) */
DECL_DAT(02036cdc);  /* -> FUN_0203b8a8 (no impl -> stub) */
DECL_DAT(02036d14);  /* -> FUN_02038b10 (no impl -> stub) */
DECL_DAT(02036bf0);  /* -> FUN_0203b8a8 (no impl -> stub) */
DECL_DAT(02036c1c);  /* -> FUN_0203b8e8 (no impl -> stub) */
DECL_DAT(02038f10);  /* -> FUN_0203aea8 (no impl -> stub) */
DECL_DAT(02038fc8);  /* -> FUN_02039014 */
DECL_DAT(020390a0);  /* -> FUN_020390a4 (no impl -> stub) */
DECL_DAT(02039140);  /* -> FUN_02039144 (no impl -> stub) */
DECL_DAT(0203e0a0);  /* -> FUN_0203e154 */
DECL_DAT(02043338);  /* -> FUN_02042650 */

#define PATCH(dat, fn) do { \
    DAT_##dat = (uintptr_t)(void*)(fn); \
    patched++; \
} while(0)

void host_dat_fnptr_patch(void)
{
    int patched = 0;

    /* Patched to real host implementations */
    PATCH(0202b940, FUN_0203f6e4);
    PATCH(0202b6c0, os_timer_dispatch);
    PATCH(0202c9bc, FUN_0202c860);
    PATCH(02007640, MI_CpuFill32Fast);
    PATCH(0202f698, FUN_0202f044);
    PATCH(02031054, FUN_0202e5d0);
    PATCH(02038fc8, FUN_02039014);
    PATCH(0203e0a0, FUN_0203e154);
    PATCH(02043338, FUN_02042650);

    /* Patched to safe stubs (no host implementation yet) */
    PATCH(0202e320, stub_int);
    PATCH(02036cdc, stub_void);
    PATCH(02036d14, stub_void);
    PATCH(02036bf0, stub_void);
    PATCH(02036c1c, stub_void);
    PATCH(02038f10, stub_void);
    PATCH(020390a0, stub_void);
    PATCH(02039140, stub_void);

    nds_log("[fnptr-patch] patched %d DAT function-pointer globals\n",
            patched);
}
