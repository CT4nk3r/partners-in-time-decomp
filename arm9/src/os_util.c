/**
 * OS Utility Functions — Nitro SDK
 *
 * Small utility functions from the Nitro SDK OS module.
 *
 * Decompiled from Ghidra pseudocode, cleaned up for matching compilation.
 */

#include "types.h"
#include "arm_compat.h"

// FUN_0203928c @ 0x0203928C (4 bytes) — Empty stub / NOP function
void OS_Dummy(void)
{
    return;
}

// FUN_020106f4 @ 0x020106F4 (8 bytes) — Another empty stub
void OS_Nop(void)
{
    return;
}

// FUN_02044644 @ 0x02044644 (4 bytes) — Empty stub
void OS_Stub(void)
{
    return;
}

// FUN_02039be8 @ 0x02039BE8 (8 bytes) — Returns zero
u32 OS_ReturnZero(void)
{
    return 0;
}

// FUN_0203b0d0 @ 0x0203B0D0 (8 bytes) — Count leading zeros (CLZ instruction)
int OS_CountLeadingZeros(u32 value)
{
    return arm_clz(value);
}

// FUN_0203bad8 @ 0x0203BAD8 (8 bytes) — Atomic swap
u32 OS_Swap(u32 new_val, u32 *ptr)
{
    u32 old_val = *ptr;
    *ptr = new_val;
    return old_val;
}

// FUN_02038f14 @ 0x02038F14 (8 bytes) — Getter: read u16 at offset +4
u16 OS_GetField_04(void *obj)
{
    return *(u16 *)((u8 *)obj + 4);
}

// FUN_020392d8 @ 0x020392D8 (8 bytes) — Getter: read u32 at offset +0x70
u32 OS_GetField_70(void *obj)
{
    return *(u32 *)((u8 *)obj + 0x70);
}

// FUN_0203ce28 @ 0x0203CE28 (8 bytes) — Getter: read u32 at offset +0x38
u32 OS_GetField_38(void *obj)
{
    return *(u32 *)((u8 *)obj + 0x38);
}
