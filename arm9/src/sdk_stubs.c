/**
 * SDK Stub Functions & Simple Wrappers
 *
 * These are thin wrappers, indirect calls through function pointers,
 * and simple global accessors from the Nitro SDK.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// Empty stubs / NOPs
// ============================================================

// FUN_0203d008 @ 0x0203D008 (32 bytes) — Clear 2-word struct
void SDK_ClearPair(u32 *ptr)
{
    ptr[0] = 0;
    ptr[1] = 0;
}

// FUN_0203aca0 @ 0x0203ACA0 (16 bytes) — Clear 2-field struct (different offsets)
void SDK_ClearFields(u32 *ptr)
{
    ptr[0] = 0;
    ptr[2] = 0;
}

// FUN_02039f94 @ 0x02039F94 (20 bytes) — Clear 3-field struct (u16 + two u32)
void SDK_ClearStruct12(u16 *ptr)
{
    *ptr = 0;
    *(u32 *)(ptr + 2) = 0;
    *(u32 *)(ptr + 4) = 0;
}

// ============================================================
// Simple getters
// ============================================================

// FUN_0203acb0 @ 0x0203ACB0 (12 bytes) — Read global u16
static u16 *sGlobal_0203acbc;
u16 SDK_GetGlobal16(void)
{
    return *sGlobal_0203acbc;
}

// FUN_0203b024 @ 0x0203B024 (16 bytes) — Read u64 at offset 0x68 from global
static u32 *sGlobal_0203b034;
u64 SDK_GetField68(void)
{
    return *(u64 *)((u8 *)*sGlobal_0203b034 + 0x68);
}

// FUN_02042620 @ 0x02042620 (16 bytes) — Read u32 through double pointer
static u32 **sGlobal_02042630;
u32 SND_GetStatus(void)
{
    return **sGlobal_02042630;
}

// FUN_02042634 @ 0x02042634 (24 bytes) — Check if field at +0x34 is nonzero
static u32 *sGlobal_0204264c;
BOOL SND_IsPlaying(void)
{
    return *(s32 *)((u8 *)sGlobal_0204264c + 0x34) != 0;
}

// FUN_020426e8 @ 0x020426E8 (12 bytes) — Read global u32
static u32 *sGlobal_020426f4;
u32 SND_GetVolume(void)
{
    return *sGlobal_020426f4;
}

// FUN_020413e4 @ 0x020413E4 (24 bytes) — Check bit 0 of global
static u32 *sGlobal_020413fc;
BOOL FS_IsReady(void)
{
    return (*sGlobal_020413fc & 1) != 0;
}

// FUN_02048414 @ 0x02048414 (28 bytes) — Read byte from double pointer at +8
u8 FS_GetType(int handle)
{
    u8 *ptr = *(u8 **)(handle + 8);
    if (ptr == NULL) {
        return 0;
    }
    return *ptr & 0x1F;
}

// FUN_0203fdd0 @ 0x0203FDD0 (28 bytes) — Get address/size based on flag
u32 FS_GetDataAddr(int obj)
{
    if ((*(u8 *)(obj + 0x1F) & 1) == 0) {
        return *(u32 *)(obj + 8);
    }
    return *(u32 *)(obj + 0x1C) & 0xFFFFFF;
}

// ============================================================
// Simple setters
// ============================================================

// FUN_0203b194 @ 0x0203B194 (12 bytes) — Write global u8
static u8 *sGlobal_0203b1a0;
void OS_SetDebugFlag(u8 value)
{
    *sGlobal_0203b1a0 = value;
}

// FUN_020426ac @ 0x020426AC (12 bytes) — Write global u32
static u32 *sGlobal_020426b8;
void SND_SetVolume(u32 value)
{
    *sGlobal_020426b8 = value;
}

// FUN_02043f70 @ 0x02043F70 (12 bytes) — Write global u32
static u32 *sGlobal_02043f7c;
void OS_SetArenaLo(u32 value)
{
    *sGlobal_02043f7c = value;
}

// FUN_02048ce0 @ 0x02048CE0 (12 bytes) — Write vtable pointer
static u32 sVtable_02048cec;
void FS_SetVtable(u32 *obj)
{
    *obj = sVtable_02048cec;
}

// ============================================================
// IRQ table setters
// ============================================================

// FUN_0203a0f0 @ 0x0203A0F0 (20 bytes) — Set IRQ handler
void IRQ_SetHandler(int slot, u32 handler)
{
    *(u32 *)(slot * 4 + 0x027FFDA0) = handler;
}

// FUN_0203a104 @ 0x0203A104 (20 bytes) — Set IRQ priority
void IRQ_SetPriority(int slot, u32 priority)
{
    *(u32 *)(slot * 4 + 0x027FFDC4) = priority;
}

// ============================================================
// Bit manipulation
// ============================================================

// FUN_0203a738 @ 0x0203A738 (24 bytes) — Set bit in global u16
static u16 *sGlobal_0203a750;
void OS_SetEnableBit(u32 bit)
{
    *sGlobal_0203a750 |= (u16)(1 << (bit & 0xFF));
}

// ============================================================
// Indirect call wrappers (call through function pointer globals)
// ============================================================

// FUN_0203edcc @ 0x0203EDCC (12 bytes) — Call function pointer
static void (**sFuncPtr_0203edd8)(void);
void SDK_CallHook1(void)
{
    (*sFuncPtr_0203edd8)();
}

// FUN_0203eddc @ 0x0203EDDC (12 bytes) — Call function pointer
static void (**sFuncPtr_0203ede8)(void);
void SDK_CallHook2(void)
{
    (*sFuncPtr_0203ede8)();
}

// FUN_020425a0 @ 0x020425A0 (12 bytes) — Call function pointer with 0x27
static void (**sFuncPtr_020425ac)(u32);
void SND_SendCommand1(void)
{
    (*sFuncPtr_020425ac)(0x27);
}

// FUN_020425b0 @ 0x020425B0 (12 bytes) — Call function pointer with 0x12
static void (**sFuncPtr_020425bc)(u32);
void SND_SendCommand2(void)
{
    (*sFuncPtr_020425bc)(0x12);
}

// FUN_020425c0 @ 0x020425C0 (12 bytes) — Call function pointer with (param, 2)
static void (**sFuncPtr_020425cc)(u32, u32);
void SND_SetParam1(u32 param)
{
    (*sFuncPtr_020425cc)(param, 2);
}

// FUN_020425d0 @ 0x020425D0 (12 bytes) — Call function pointer with (param, 2)
static void (**sFuncPtr_020425dc)(u32, u32);
void SND_SetParam2(u32 param)
{
    (*sFuncPtr_020425dc)(param, 2);
}

// ============================================================
// GX Wrappers (display subsystem passthrough)
// ============================================================

// FUN_020288a8 @ 0x020288A8 (40 bytes) — Forward to texture load
extern void FUN_02042eec(void);
void GX_LoadTexture1(void)
{
    FUN_02042eec();
}

// FUN_020288d0 @ 0x020288D0 (40 bytes) — Forward to texture setup
extern void FUN_02042d4c(void);
void GX_SetupTexture(void)
{
    FUN_02042d4c();
}

// FUN_020288f8 @ 0x020288F8 (40 bytes) — Forward to texture load (same as 1)
void GX_LoadTexture2(void)
{
    FUN_02042eec();
}

// FUN_02028920 @ 0x02028920 (40 bytes) — Forward to texture operation
extern void FUN_02042e1c(void);
void GX_UpdateTexture(void)
{
    FUN_02042e1c();
}

// ============================================================
// Return-constant functions
// ============================================================

// FUN_0203ae4c @ 0x0203AE4C (24 bytes) — Return 0
u32 OS_ReturnZero2(void) { return 0; }

// FUN_0203ae78 @ 0x0203AE78 (24 bytes) — Return 0
u32 OS_ReturnZero3(void) { return 0; }

// FUN_0203ae90 @ 0x0203AE90 (12 bytes) — Return 0
u32 OS_ReturnZero4(void) { return 0; }

// FUN_0203f740 @ 0x0203F740 (28 bytes) — Call function, return 1
extern void FUN_0203f75c(void);
u32 FS_InitResult(void)
{
    FUN_0203f75c();
    return 1;
}
