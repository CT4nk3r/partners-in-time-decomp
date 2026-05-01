/**
 * CP15 Coprocessor Functions — ARM946E-S Cache & Protection Unit
 *
 * These functions control the ARM9's CP15 coprocessor for cache management,
 * memory protection, and system control.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// FUN_0203a014 @ 0x0203A014 (28 bytes) — Invalidate data cache by MVA range
void CP15_InvalidateDataCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        asm volatile("mcr p15, 0, %0, c7, c6, 1" :: "r"(line));
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

// FUN_0203a030 @ 0x0203A030 (28 bytes) — Clean data cache by MVA range
void CP15_CleanDataCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        asm volatile("mcr p15, 0, %0, c7, c10, 1" :: "r"(line));
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

// FUN_0203a070 @ 0x0203A070 (12 bytes) — Data synchronization barrier
u32 CP15_DataSyncBarrier(void)
{
    asm volatile("mcr p15, 0, %0, c7, c10, 4" :: "r"(0));
    return 0;
}

// FUN_0203a088 @ 0x0203A088 (28 bytes) — Invalidate instruction cache by MVA range
void CP15_InvalidateInsnCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        asm volatile("mcr p15, 0, %0, c7, c5, 1" :: "r"(line));
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

// FUN_0203a4ec @ 0x0203A4EC (16 bytes) — Enable protection unit
u32 CP15_EnableProtectionUnit(void)
{
    u32 ctrl;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(ctrl));
    ctrl |= 1;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" :: "r"(ctrl));
    return ctrl;
}

// FUN_0203a4fc @ 0x0203A4FC (16 bytes) — Disable protection unit
u32 CP15_DisableProtectionUnit(void)
{
    u32 ctrl;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(ctrl));
    ctrl &= ~1;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" :: "r"(ctrl));
    return ctrl;
}

// FUN_0203a50c @ 0x0203A50C (8 bytes) — Set data protection region
void CP15_SetDataRegion(u32 region)
{
    asm volatile("mcr p15, 0, %0, c6, c1, 0" :: "r"(region));
}

// FUN_0203a514 @ 0x0203A514 (8 bytes) — Set instruction protection region
void CP15_SetInsnRegion(u32 region)
{
    asm volatile("mcr p15, 0, %0, c6, c2, 0" :: "r"(region));
}

// FUN_0203a51c @ 0x0203A51C (8 bytes) — Get instruction protection region
u32 CP15_GetInsnRegion(void)
{
    u32 val;
    asm volatile("mrc p15, 0, %0, c6, c2, 0" : "=r"(val));
    return val;
}

// FUN_0203ae9c @ 0x0203AE9C (12 bytes) — Wait for interrupt (WFI / CP15 c7,c0,4)
u32 CP15_WaitForInterrupt(void)
{
    asm volatile("mcr p15, 0, %0, c7, c0, 4" :: "r"(0));
    return 0;
}
