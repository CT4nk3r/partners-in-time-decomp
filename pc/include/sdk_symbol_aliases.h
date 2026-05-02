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

/* 22 mappings */

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
#define FUN_02046ffc LZ77_UncompVram_1
#define FUN_02047010 LZ77_UncompVram_2
#define FUN_02047024 LZ77_UncompVram_3

#endif /* SDK_SYMBOL_ALIASES_H */
