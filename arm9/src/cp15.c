/**
 * CP15 Coprocessor Functions - ARM946E-S Cache & Protection Unit
 *
 * These functions control the ARM9's CP15 coprocessor for cache management,
 * memory protection, and system control. Inline asm is wrapped via arm_compat.h
 * so this file can also be compiled for the host PC port.
 */

#include "types.h"
#include "arm_compat.h"

void CP15_InvalidateDataCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        arm_cp15_invalidate_dcache_mva(line);
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

void CP15_CleanDataCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        arm_cp15_clean_dcache_mva(line);
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

u32 CP15_DataSyncBarrier(void)
{
    arm_cp15_dsb();
    return 0;
}

void CP15_InvalidateInsnCacheRange(u32 addr, int size)
{
    u32 line = addr & ~0x1F;
    do {
        arm_cp15_invalidate_icache_mva(line);
        line += 0x20;
    } while ((s32)line < (s32)(addr + size));
}

u32 CP15_EnableProtectionUnit(void)
{
    u32 ctrl = arm_cp15_read_control();
    ctrl |= 1;
    arm_cp15_write_control(ctrl);
    return ctrl;
}

u32 CP15_DisableProtectionUnit(void)
{
    u32 ctrl = arm_cp15_read_control();
    ctrl &= ~1u;
    arm_cp15_write_control(ctrl);
    return ctrl;
}

void CP15_SetDataRegion(u32 region) { arm_cp15_set_data_region(region); }
void CP15_SetInsnRegion(u32 region) { arm_cp15_set_insn_region(region); }
u32  CP15_GetInsnRegion(void)       { return arm_cp15_get_insn_region(); }

u32 CP15_WaitForInterrupt(void)
{
    arm_cp15_wfi();
    return 0;
}
