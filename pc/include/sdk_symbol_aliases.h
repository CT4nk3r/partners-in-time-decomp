/*
 * sdk_symbol_aliases.h - Auto-generated FUN_<addr> -> real_name macros.
 *
 * Force-included into every arm9/src/*.c on HOST_PORT builds via CMake
 * `-include`. This makes anonymous Ghidra calls land on the hand-named
 * implementations (game_start, BIOS_Stop, …) instead of the no-op
 * shadow stubs in pc/src/host_undefined_stubs.c.
 *
 * Regenerate with:  python tools/scripts/gen_sdk_aliases.py
 */
#ifndef SDK_SYMBOL_ALIASES_H
#define SDK_SYMBOL_ALIASES_H

/* 33 mappings */

/* === Top-20 hot anonymous helpers (Task 2) ============================
 * These FUN_<addr> have NO body in arm9/src (only externs from many TUs).
 * Without aliases they fall through to host_undefined_stubs.c no-ops.
 * We map them to hand-written impls in arm9/src/link_stubs.c so the
 * underlying behaviour (memcpy / udiv / IRQ critical-section) is real.
 *
 * Identification was done by:
 *  - matching the signatures used at every call site
 *  - cross-referencing the address ranges with NitroSDK ARM9 source
 *  - reading the few defined neighbour functions (hw_gx.c) that call them
 * ==================================================================== */
#define FUN_0202cc94 sdk_mi_cpu_copy16        /* MI_CpuCopy16-shaped (8 TUs) */
#define FUN_0202cd68 sdk_mi_cpu_fill16        /* MI_CpuFill16-shaped (9 TUs) */
#define FUN_02029ab8 sdk_os_free              /* OS_Free / OS_FreeToHeap (5 TUs) */
#define FUN_0202a74c sdk_os_destroy_heap      /* OS_DestroyHeap (5 TUs)        */
#define FUN_0203ae38 sdk_os_disable_irq       /* OS_DisableInterrupts → u32   */
#define FUN_0203ae4c sdk_os_restore_irq       /* OS_RestoreInterrupts(u32)    */
#define FUN_0203aeb4 sdk_os_reschedule        /* OS_RescheduleThread (no-op)  */
#define FUN_020466bc sdk_arm_udiv             /* libgcc-style unsigned divide */
#define FUN_0203a04c sdk_os_send_message      /* OS_SendMessage-shaped         */

#define FUN_02004228 BIOS_Stop
#define FUN_0200429c BIOS_VBlankIntrWait
#define FUN_0200445a BIOS_CpuSet
#define FUN_02004914 crt0_copy_sections
#define FUN_020050f4 game_init
#define FUN_0200528c game_fade_out
#define FUN_02005300 game_check_display_ready
#define FUN_02005380 game_check_reset
#define FUN_02005444 game_start
#define FUN_020055c0 GameProp_Set
#define FUN_02005900 GameProp_Get
#define FUN_02028ea8 OS_InitHeap
#define FUN_02029964 OS_AllocFromHeap
#define FUN_02029bf8 OS_AllocBack
#define FUN_02029c1c OS_Alloc
#define FUN_0202cc00 RTC_SetDate
#define FUN_0202cc10 MI_CpuFill32Fast
#define FUN_0203b914 MI_CpuFill8
#define FUN_0203b9a8 MI_CpuCopy8

/* The three FUN_02046ffc/02047010/02047024 entries previously aliased to
 * LZ77_UncompVram_* are actually console-I/O SWIs (write/read/flush) with
 * full bodies in arm9/src/sdk_init_d.c.  Aliasing them caused a double
 * definition with link_stubs.c.  Aliases removed. */

/* === OBJ engine symbolication (Task 5, staticinit-scenefactory) ============
 * The SDK names OBJ_LoadChar4 / OBJ_LoadOam have 0 callsites — the game
 * uses anonymous FUN_<addr> entry points instead.  Identification was by
 * ripgrep for OBJ-VRAM destination addresses (0x06400000, 0x06600000) —
 * the only writers are `param_2 + 0x6400000` (top OBJ char) /
 * `param_2 + 0x6600000` (sub OBJ char).  These are the engine.  Keep
 * the FUN_ names as the canonical decomp identifier; add comment-tagged
 * gx_obj_* shorthand for human readability when the SDK label is
 * confirmed.  No runtime behaviour changes from these aliases — the
 * impls already live in arm9/src/hw_gx.c and arm9/src/game_engine_a.c.
 * ========================================================================= */
/* Top-engine OBJ char tile loader.  sig: (src, dst_off_in_obj, len). */
#define gx_obj_load_char_top   FUN_020385b4
/* Sub-engine OBJ char tile loader. sig: (src, dst_off_in_obj, len). */
#define gx_obj_load_char_sub   FUN_02038554

/* OBJ-side high-level blit/tile-submit helpers in game_engine_a.c.  All
 * dereference state structs and ultimately call FUN_0203b854 (raw OBJ
 * VRAM copy).  No straight SDK mapping — keep FUN_ names. */
/* FUN_02020560 — 760-byte OBJ tile dispatcher (3+ inner loops)        */
/* FUN_02020990 — 284-byte OBJ tile blit                               */
/* FUN_02021298 — 736-byte OBJ frame-step                              */
/* FUN_02021588 — 348-byte OBJ submit (writes to 0x6400000/0x6600000)  */
/* FUN_02021f7c — wrapper: FUN_02021588 + FUN_02021298 (OBJ tile B)    */
/* FUN_02021fd4 — wrapper: FUN_02021588 + FUN_02021298 (OBJ tile A)    */

/* OAM (0x07000000-0x070003FF): NO direct writers found in any
 * decompiled C (arm9/src or config/decompiled).  All OAM updates
 * therefore route through one of:
 *   (a) NNS DMA function whose target address is computed at runtime
 *       (caught by the IO/VRAM watcher, not by static grep), or
 *   (b) a function pointer dereferenced via a DAT_ slot whose writer
 *       lives in an undecompiled overlay.
 * Documenting this for the next session.  No alias added.
 */

/* MI_Fill32 — used as the memset-like primitive by FUN_02029c40 (heap init),
 * hw_gx.c, sdk_io.c, etc. Defined in arm9/src/mem_simple.c. */
#define FUN_0203b7dc MI_Fill32

#endif /* SDK_SYMBOL_ALIASES_H */
