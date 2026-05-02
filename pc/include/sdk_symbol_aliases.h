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

#endif /* SDK_SYMBOL_ALIASES_H */
