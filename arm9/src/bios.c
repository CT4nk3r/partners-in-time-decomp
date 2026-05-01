/**
 * BIOS System Call Wrappers
 *
 * These functions are thin wrappers around ARM SWI (Software Interrupt)
 * instructions that call into the NDS BIOS.
 *
 * Decompiled from:
 *   FUN_02004228 @ 0x02004228 (4 bytes)  - SWI 0x03 = Stop
 *   FUN_0200429c @ 0x0200429C (6 bytes)  - SWI 0x05 = VBlankIntrWait
 *   FUN_0200445a @ 0x0200445A (4 bytes)  - SWI 0x0B = CpuSet
 */

#include "types.h"

// SWI 0x03: Stop - puts the CPU into low-power stop mode
void BIOS_Stop(void)
{
    asm("swi 0x03" ::: "r0", "r1", "r2", "r3");
}

// SWI 0x05: VBlankIntrWait - waits for a VBlank interrupt
void BIOS_VBlankIntrWait(void)
{
    asm("swi 0x05" ::: "r0", "r1", "r2", "r3");
}

// SWI 0x0B: CpuSet - fast memory copy/fill
void BIOS_CpuSet(const void *src, void *dst, u32 mode)
{
    register const void *r0 asm("r0") = src;
    register void *r1 asm("r1") = dst;
    register u32 r2 asm("r2") = mode;
    asm volatile("swi 0x0B" : "+r"(r0), "+r"(r1), "+r"(r2) :: "r3", "memory");
}
