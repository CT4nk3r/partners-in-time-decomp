/**
 * BIOS System Call Wrappers
 *
 * These functions are thin wrappers around ARM SWI (Software Interrupt)
 * instructions that call into the NDS BIOS. On the PC port (HOST_PORT)
 * they route through arm_compat host stubs.
 *
 * Decompiled from:
 *   FUN_02004228 @ 0x02004228 (4 bytes)  - SWI 0x03 = Stop
 *   FUN_0200429c @ 0x0200429C (6 bytes)  - SWI 0x05 = VBlankIntrWait
 *   FUN_0200445a @ 0x0200445A (4 bytes)  - SWI 0x0B = CpuSet
 */

#include "types.h"
#include "arm_compat.h"

void BIOS_Stop(void)
{
    arm_swi_03_stop();
}

void BIOS_VBlankIntrWait(void)
{
    arm_swi_05_vblank_intr_wait();
}

void BIOS_CpuSet(const void *src, void *dst, u32 mode)
{
    arm_swi_0b_cpuset(src, dst, mode);
}
