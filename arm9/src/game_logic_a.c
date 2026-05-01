/**
 * Game Logic A — Battle system, status effects, animation state, palette management
 *
 * This module handles battle-related logic including VRAM tile lookup, sprite palette
 * configuration, status effect copying, animation state management, and battle
 * script interpretation support functions.
 *
 * Decompiled from:
 *   FUN_0200c1ec @ 0x0200C1EC (120 bytes) - Find VRAM tile by type/screen
 *   FUN_0200c264 @ 0x0200C264 (100 bytes) - Get tile offset with fallback
 *   FUN_0200c2c8 @ 0x0200C2C8 (44 bytes)  - Dispatch VRAM bank shift
 *   FUN_0200c2fc @ 0x0200C2FC (52 bytes)   - Initialize sprite object base
 *   FUN_0200c3a0 @ 0x0200C3A0 (12 bytes)   - Set global palette index
 *   FUN_0200c45c @ 0x0200C45C (124 bytes)  - Set/clear status effect bit
 *   FUN_0200c4d8 @ 0x0200C4D8 (148 bytes)  - Copy status effect array (8 slots)
 *   FUN_0200c56c @ 0x0200C56C (52 bytes)   - Clear status flag and update
 *   FUN_0200c5a0 @ 0x0200C5A0 (296 bytes)  - Set/clear palette for battle slots
 *   FUN_0200c76c @ 0x0200C76C (376 bytes)  - Apply animation state from data
 *   FUN_0200c930 @ 0x0200C930 (88 bytes)   - Set animation frame with offset
 *   FUN_0200c990 @ 0x0200C990 (928 bytes)  - Battle animation state machine (large)
 *   FUN_0200cd30 @ 0x0200CD30 (132 bytes)  - Initialize battle slot arrays
 *   FUN_0200cdb4 @ 0x0200CDB4 (152 bytes)  - Full battle entity setup
 *   FUN_0200ce4c @ 0x0200CE4C (128 bytes)  - Battle entity setup variant
 *   FUN_0200cee4 @ 0x0200CEE4 (32 bytes)   - Construct battle object type A
 *   FUN_0200cf08 @ 0x0200CF08 (40 bytes)   - Construct battle object type B
 *   FUN_0200cf58 @ 0x0200CF58 (52 bytes)   - Construct battle sprite type A
 *   FUN_0200cf90 @ 0x0200CF90 (52 bytes)   - Construct battle sprite type B
 *   FUN_0200cfc8 @ 0x0200CFC8 (16 bytes)   - Get status effect slot address
 *   FUN_0200d028 @ 0x0200D028 (64 bytes)   - Copy tile data by direction
 *   FUN_0200d0c8 @ 0x0200D0C8 (636 bytes)  - Battle damage/effect processor
 *   FUN_0200d348 @ 0x0200D348 (832 bytes)  - Battle turn processor (large)
 */

#include "types.h"

/* Forward declarations */
extern u32 FUN_0200a2c4(void *, int);
extern void FUN_0200b278(int *, int, int);
extern void FUN_0200be14(void);
extern void FUN_0200c2fc(void);
extern void FUN_02009110(void);
extern void FUN_0202cc10(void *, u32, u32);
extern void FUN_0202cd68(void);
extern void FUN_0202cc94(void);
extern u32 FUN_0202d9d0(u32, u8);
extern void thunk_FUN_02029ab8(void *);

/* Static data references */
static u32 *DAT_0200c2f4;     /* VRAM bank config table */
static void (*DAT_0200c2f8)(u32); /* VRAM shift dispatch function */
static u32 DAT_0200c330;      /* vtable for sprite object base */
static u32 *DAT_0200c3ac;     /* global palette index pointer */
static u32 DAT_0200cf04;      /* vtable for battle object type A */
static u32 DAT_0200cf30;      /* vtable for battle object type B */
static u32 DAT_0200cf8c;      /* vtable for battle sprite type A */
static u32 DAT_0200cfc4;      /* vtable for battle sprite type B */
static u32 DAT_0200d068;      /* VRAM boundary threshold */

/* Forward declaration for functions defined later in this file */
static int FUN_0200cfc8(int param_1, int param_2);

// FUN_0200c1ec @ 0x0200C1EC (120 bytes) — Find VRAM tile by type/screen
u32 FUN_0200c1ec_impl(u32 param_1, u32 param_2, int param_3)
{
    u32 uVar1;
    u32 uVar3;
    int iVar2;

    uVar3 = (u32)*(u16 *)(param_3 + 0x14);
    iVar2 = 0;
    while (TRUE) {
        if ((uVar3 & 7) == 0) {
            return 0;
        }
        if (((uVar3 & 7) == param_1) && ((uVar3 & 8) >> 3 == param_2)) break;
        iVar2 = iVar2 + 1;
        uVar3 = uVar3 >> 4;
        if (3 < iVar2) {
            return 0;
        }
    }
    uVar1 = FUN_0200a2c4((void *)param_3, iVar2 + 6);
    return uVar1;
}

// FUN_0200c264 @ 0x0200C264 (100 bytes) — Get tile offset with fallback
int FUN_0200c264(u32 param_1, int param_2, u16 *param_3)
{
    int iVar1;

    if (param_2 == 0) {
        if (param_1 == ((u32)*param_3 << 0x16) >> 0x1d) {
            return 0;
        }
    } else if (param_1 == ((u32)*param_3 << 0x19) >> 0x1d) {
        return 0;
    }
    iVar1 = FUN_0200c1ec_impl(param_1, param_2, (int)param_3);
    if (iVar1 == 0) {
        iVar1 = (u32)param_3[9] << 1;
    } else {
        iVar1 = 0;
    }
    return iVar1;
}

// FUN_0200c2c8 @ 0x0200C2C8 (44 bytes) — Dispatch VRAM bank shift
void FUN_0200c2c8(int param_1)
{
    (*DAT_0200c2f8)(((*(u32 *)(DAT_0200c2f4 + param_1) & 0x300000) >> 0x14) + 5);
    return;
}

// FUN_0200c2fc @ 0x0200C2FC (52 bytes) — Initialize sprite object base
u32 *FUN_0200c2fc_impl(u32 *param_1)
{
    FUN_02009110();
    *param_1 = DAT_0200c330;
    FUN_0202cc10(param_1 + 5, 0, (int)param_1 + (0x80 - (int)(param_1 + 5)));
    return param_1;
}

// FUN_0200c3a0 @ 0x0200C3A0 (12 bytes) — Set global palette index
void FUN_0200c3a0_impl(u32 param_1)
{
    *DAT_0200c3ac = param_1;
    return;
}

// FUN_0200c45c @ 0x0200C45C (124 bytes) — Set/clear status effect bit
void FUN_0200c45c(int param_1, u32 param_2, u32 param_3, int param_4)
{
    int iVar1;
    int iVar2;
    u32 uVar3;

    iVar1 = FUN_0200cfc8(param_2, param_3);
    iVar2 = *(int *)(param_1 + 0x2c);
    if (*(u8 *)(iVar2 + 0x11) < 0x10) {
        uVar3 = 1 << (u32)*(u8 *)(iVar2 + 0x11);
    } else {
        uVar3 = ((1 << *(s8 *)(iVar2 + 0x12)) - 1) << (*(u8 *)(iVar2 + 0x10) + 0x10 & 0xff);
    }
    if (param_4 == 0) {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) & ~uVar3;
    } else {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) | uVar3;
    }
    return;
}

// FUN_0200c4d8 @ 0x0200C4D8 (148 bytes) — Copy status effect array (8 slots)
void FUN_0200c4d8(int param_1, int param_2)
{
    int iVar1;
    u32 uVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;

    iVar5 = 0;
    do {
        iVar3 = param_1 + iVar5 * 4;
        iVar1 = param_2 + iVar5 * 4;
        *(u32 *)(iVar1 + 0xc0) = *(u32 *)(iVar3 + 0xc0);
        iVar6 = param_1 + iVar5;
        *(u16 *)(param_2 + iVar5 * 2 + 0xe0) = *(u16 *)(param_1 + iVar5 * 2 + 0xe0);
        iVar4 = param_2 + iVar5;
        *(u8 *)(iVar4 + 0xf0) = *(u8 *)(iVar6 + 0xf0);
        *(u8 *)(iVar4 + 0xf8) = *(u8 *)(iVar6 + 0xf8);
        *(u8 *)(iVar4 + 0x120) = *(u8 *)(iVar6 + 0x120);
        if (*(int *)(iVar3 + 0x100) == 0) {
            *(u32 *)(iVar1 + 0x100) = 0;
        } else {
            uVar2 = FUN_0202d9d0(*(u32 *)(param_2 + 0x84), *(u8 *)(iVar6 + 0x120));
            *(u32 *)(param_2 + iVar5 * 4 + 0x100) = uVar2;
        }
        iVar5 = iVar5 + 1;
    } while (iVar5 < 8);
    return;
}

// FUN_0200c56c @ 0x0200C56C (52 bytes) — Clear status flag and update
void FUN_0200c56c(int *param_1)
{
    *(u8 *)(param_1[0xb] + 0x13) = *(u8 *)(param_1[0xb] + 0x13) & 0xef;
    (*(void (*)(int *, int))(*(int *)(*param_1 + 0x50)))(param_1, 1);
    return;
}

// FUN_0200c5a0 @ 0x0200C5A0 (296 bytes) — Set/clear palette for battle slots
void FUN_0200c5a0(int param_1, int param_2, int param_3, int param_4)
{
    int iVar1;
    int iVar2;
    u32 uVar3;

    if ((param_3 == -1) || (iVar1 = *(int *)(param_1 + 0x84), iVar1 == 0)) {
        if (param_2 != -1) {
            *(u32 *)(param_1 + param_2 * 4 + 0x100) = 0;
            *(u8 *)(param_1 + param_2 + 0x120) = 0;
            return;
        }
        iVar1 = 0;
        do {
            iVar2 = param_1 + iVar1;
            *(u32 *)(param_1 + iVar1 * 4 + 0x100) = 0;
            iVar1 = iVar1 + 1;
            *(u8 *)(iVar2 + 0x120) = 0;
        } while (iVar1 < 8);
        return;
    }
    if (param_2 != -1) {
        uVar3 = FUN_0202d9d0(iVar1, param_3);
        iVar1 = param_1 + param_2 * 4;
        *(u32 *)(iVar1 + 0x100) = uVar3;
        *(char *)(param_1 + param_2 + 0x120) = (char)param_3;
        if (param_4 != 0) {
            *(u32 *)(iVar1 + 0xc0) = 0;
        }
        return;
    }
    uVar3 = FUN_0202d9d0(iVar1, param_3);
    iVar1 = 0;
    do {
        iVar2 = param_1 + iVar1;
        *(u32 *)(param_1 + iVar1 * 4 + 0x100) = uVar3;
        iVar1 = iVar1 + 1;
        *(char *)(iVar2 + 0x120) = (char)param_3;
    } while (iVar1 < 8);
    if (param_4 != 0) {
        iVar1 = 0;
        do {
            iVar2 = iVar1 * 4;
            iVar1 = iVar1 + 1;
            *(u32 *)(param_1 + iVar2 + 0xc0) = 0;
        } while (iVar1 < 8);
        return;
    }
    return;
}

// FUN_0200c930 @ 0x0200C930 (88 bytes) — Set animation frame with offset
void FUN_0200c930(int *param_1, s16 param_2, u32 param_3, int param_4)
{
    FUN_0200b278(param_1, (int)(param_2 + (s16)param_1[0x4a]) & 0xff, -1);
    if (param_4 != 0) {
        (*(void (*)(int *, int, int))(*(int *)(*param_1 + 0x78)))(param_1, (int)(s16)param_1[0x15], 0);
        return;
    }
    return;
}

// FUN_0200cd30 @ 0x0200CD30 (132 bytes) — Initialize battle slot arrays (8 slots)
void FUN_0200cd30(int param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = 0;
    iVar2 = 0;
    do {
        *(u32 *)(param_1 + iVar1 * 4 + 0x88) = 0;
        iVar3 = iVar1 * 2;
        iVar1 = iVar1 + 1;
        *(u16 *)(param_1 + iVar3 + 0xa8) = *(u16 *)(param_1 + 0x5a);
    } while (iVar1 < 8);
    *(u8 *)(param_1 + 0xb8) = 0;
    *(u8 *)(param_1 + 0xb9) = 2;
    *(u32 *)(param_1 + 0xbc) = 0;
    do {
        iVar3 = param_1 + iVar2 * 4;
        *(u32 *)(iVar3 + 0xc0) = 0;
        iVar1 = param_1 + iVar2;
        *(u16 *)(param_1 + iVar2 * 2 + 0xe0) = *(u16 *)(param_1 + 0x5a);
        *(u8 *)(iVar1 + 0xf0) = 0;
        *(u8 *)(iVar1 + 0xf8) = 2;
        *(u32 *)(iVar3 + 0x100) = 0;
        iVar2 = iVar2 + 1;
        *(u8 *)(iVar1 + 0x120) = 0;
    } while (iVar2 < 8);
    return;
}

// FUN_0200cdb4 @ 0x0200CDB4 (152 bytes) — Full battle entity setup
void FUN_0200cdb4(int *param_1, u32 param_2, int param_3, u16 param_4)
{
    *(u16 *)(param_1 + 0x4a) = param_4;
    *(u16 *)(param_1 + 0x15) = param_4;
    (*(void (*)(void))(*(int *)(*param_1 + 0xb4)))();
    param_1[0x21] = param_3;
    (*(void (*)(void))(*(int *)(*param_1 + 0x60)))();
    (*(void (*)(int *, int, int))(*(int *)(*param_1 + 0x78)))(param_1, (int)(s16)param_1[0x15], 0);
    (*(void (*)(int *, int, int, int))(*(int *)(*param_1 + 0x88)))(param_1, -1, -1, 1);
    (*(void (*)(int *, int, int, int))(*(int *)(*param_1 + 0x68)))(param_1, 0, 0, 1);
    return;
}

// FUN_0200ce4c @ 0x0200CE4C (128 bytes) — Battle entity setup variant
void FUN_0200ce4c(int *param_1, u32 param_2, int param_3, u16 param_4)
{
    *(u16 *)(param_1 + 0x4a) = param_4;
    (*(void (*)(void))(*(int *)(*param_1 + 0xb0)))();
    param_1[0x21] = param_3;
    (*(void (*)(void))(*(int *)(*param_1 + 0x60)))();
    (*(void (*)(int *, int, int, int))(*(int *)(*param_1 + 0x88)))(param_1, -1, -1, 1);
    (*(void (*)(int *, u32, int, int))(*(int *)(*param_1 + 0x68)))(param_1, *(u16 *)(param_1 + 0x15) & 0xff, 0, 1);
    return;
}

// FUN_0200cee4 @ 0x0200CEE4 (32 bytes) — Construct battle object type A
u32 *FUN_0200cee4(u32 *param_1)
{
    *param_1 = DAT_0200cf04;
    FUN_0200be14();
    return param_1;
}

// FUN_0200cf08 @ 0x0200CF08 (40 bytes) — Construct battle object type B
u32 *FUN_0200cf08(u32 *param_1)
{
    *param_1 = DAT_0200cf30;
    FUN_0200be14();
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_0200cf58 @ 0x0200CF58 (52 bytes) — Construct battle sprite type A
u32 *FUN_0200cf58(u32 *param_1)
{
    FUN_0200c2fc_impl(NULL);
    *param_1 = DAT_0200cf8c;
    FUN_0202cc10(param_1 + 0x21, 0, (int)param_1 + (300 - (int)(param_1 + 0x21)));
    return param_1;
}

// FUN_0200cf90 @ 0x0200CF90 (52 bytes) — Construct battle sprite type B
u32 *FUN_0200cf90(u32 *param_1)
{
    FUN_0200c2fc_impl(NULL);
    *param_1 = DAT_0200cfc4;
    FUN_0202cc10(param_1 + 0x21, 0, (int)param_1 + (300 - (int)(param_1 + 0x21)));
    return param_1;
}

// FUN_0200cfc8 @ 0x0200CFC8 (16 bytes) — Get status effect slot address
static int FUN_0200cfc8(int param_1, int param_2)
{
    return param_2 * 0x14 + *(int *)(param_1 + 8);
}

// FUN_0200d028 @ 0x0200D028 (64 bytes) — Copy tile data by direction
void FUN_0200d028(int param_1, int param_2)
{
    if (DAT_0200d068 < (u32)(param_2 * 0x14 + *(int *)(param_1 + 8))) {
        FUN_0202cd68();
        return;
    }
    FUN_0202cc94();
    return;
}
