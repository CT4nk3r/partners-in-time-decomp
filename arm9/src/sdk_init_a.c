/**
 * SDK Init A — SDK initialization, OS setup (0x02040xxx-0x02041xxx)
 * Decompiled from 0x020405d4-0x02041a48 region (23 functions)
 */
#include "types.h"

/* Suppress Ghidra type-mismatch artifacts (int/pointer casts) */
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/* Ghidra CONCAT helper */
#define CONCAT22(hi16, lo16) (((u32)(hi16) << 16) | ((u16)(lo16)))

typedef int (*FuncPtr)(...);

/* ================================================================== */
/*  External function declarations                                     */
/* ================================================================== */
extern void FUN_02035218(...);
extern void FUN_020351cc(...);
extern u32  FUN_02038c80(...);
extern void FUN_02038d00(...);
extern int  FUN_020413e4(...);
extern u32  FUN_0203ae38(...);
extern void FUN_0203ae4c(...);
extern void FUN_0203ae9c(...);
extern void FUN_0203aea8(...);
extern void FUN_0203aeb4(...);
extern int  FUN_0203d518(...);
extern int  FUN_0203d5cc(...);
extern void FUN_0203d5f4(...);
extern int  FUN_02041abc(...);
extern int  FUN_0204180c(...);
extern void FUN_02041d80(...);
extern int  FUN_02041d98(...);
extern int  FUN_02043d7c(...);
extern void thunk_FUN_0203d660(...);

/* ================================================================== */
/*  External data references (DAT_ labels from Ghidra)                 */
/* ================================================================== */

/* FUN_020405d4 / FUN_02040640 — touch panel config */
extern u32 *DAT_0204062c;
extern u32 *DAT_02040630;
extern u16 *DAT_02040634;
extern u32 *DAT_02040638;
extern u16 *DAT_0204063c;
extern u32 *DAT_020406a0;
extern u32 *DAT_020406a4;
extern u16 *DAT_020406a8;
extern u32 *DAT_020406ac;
extern u16 *DAT_020406b0;

/* FUN_020406e0 — touch calibration */
extern int   DAT_02040808;
extern u32  *DAT_0204080c;

/* FUN_02040810 — calibration matrix */
extern u16  *DAT_02040a70;
extern int  *DAT_02040a74;
extern int  *DAT_02040a78;
extern int  *DAT_02040a7c;

/* FUN_02040c68 */
extern int   DAT_02040cc4;

/* FUN_02040cc8 */
extern u32  *DAT_02040d64;

/* FUN_02040d68 */
extern int   DAT_02040e9c;
extern u16  *DAT_02040ea0;
extern u32  *DAT_02040ea4;
extern int  *DAT_02040ea8;
extern u32  *DAT_02040eac;

/* FUN_02040eb0 */
extern int   DAT_02040f58;

/* FUN_02040f5c */
extern u32  *DAT_02040fe0;
extern u32  *DAT_02040fe4;
extern u32   DAT_02040fe8;

/* FUN_02040fec */
extern u32  *DAT_020412a0;
extern u16  *DAT_020412a4;
extern u16  *DAT_020412a8;

/* FUN_02041400 */
extern int  *DAT_020414e4;
extern int  *DAT_020414e8;
extern u32  *DAT_020414ec;

/* FUN_020414f0 */
extern u32  *DAT_02041704;
extern u16  *DAT_02041708;
extern u32   DAT_0204170c;
extern u32  *DAT_02041710;
extern u32  *DAT_02041714;
extern vu32 *DAT_02041718;
extern u32   DAT_0204171c;
extern u32  *DAT_02041720;
extern u32  *DAT_02041724;
extern u32   _DAT_04000000;

/* FUN_020417d0 / FUN_02041824 / FUN_02041860 / FUN_020418f0 */
extern u32   DAT_02041808;
extern u32   DAT_0204185c;
extern u32   DAT_020418ec;
extern u32   DAT_02041928;

/* FUN_02041994 */
extern u32   DAT_020419cc;

/* FUN_020419d0 */
extern int   DAT_02041a38;
extern int   DAT_02041a3c;
extern u32   DAT_02041a40;
extern int   DAT_02041a44;

/* FUN_02041a48 */
extern int   DAT_02041aac;
extern u32   DAT_02041ab0;
extern u32   DAT_02041ab4;
extern u32   DAT_02041ab8;

/* ================================================================== */
/*  Forward declarations for functions defined in this file            */
/* ================================================================== */
void FUN_020406e0(u16 *param_1, u16 *param_2);
u32  FUN_02040810(u16 *param_1, u32 param_2, u32 param_3, u32 param_4,
                  u16 param_5, u16 param_6, u16 param_7, u16 param_8, u16 param_9);
void FUN_0204130c(u32 *param_1);
u32  FUN_02041400(int param_1, int param_2, int param_3, int param_4);
void FUN_02041728(u32 param_1);
void FUN_02041758(u32 *param_1, u32 *param_2);
int  FUN_020417d0(u16 param_1);
int  FUN_02041824(u32 param_1, u32 param_2);
u32  FUN_02041860(int param_1, int param_2, u32 param_3, u32 param_4);
int  FUN_020418f0(u32 param_1);
u32  FUN_0204192c(int param_1, u32 param_2, u32 param_3);
int  FUN_02041994(u32 param_1, u32 param_2);
u32  FUN_020419d0(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32  FUN_02041a48(u32 param_1, u32 param_2, u32 param_3);

/* ================================================================== */
/*  Function implementations                                           */
/* ================================================================== */

// FUN_020405d4 @ 0x020405d4 (88 bytes) — store touch panel config data
void FUN_020405d4(u32 *param_1)
{
    u32 *puVar1;
    u32 *puVar2;
    u16 *puVar3;
    u16 *puVar4;
    u32 uVar5;

    puVar2 = DAT_02040630;
    puVar1 = DAT_0204062c;
    uVar5 = param_1[1];
    *DAT_0204062c = *param_1;
    puVar1[1] = uVar5;
    puVar3 = DAT_02040634;
    uVar5 = param_1[3];
    *puVar2 = param_1[2];
    puVar2[1] = uVar5;
    puVar4 = DAT_0204063c;
    puVar1 = DAT_02040638;
    *puVar3 = *(u16 *)(param_1 + 6);
    uVar5 = param_1[5];
    *puVar1 = param_1[4];
    puVar1[1] = uVar5;
    *puVar4 = *(u16 *)((int)param_1 + 0x1a);
    return;
}

// FUN_02040640 @ 0x02040640 (96 bytes) — load touch panel config data
void FUN_02040640(u32 *param_1)
{
    u32 *puVar1;
    u16 *puVar2;
    u32 *puVar3;
    u16 *puVar4;
    u32 uVar5;

    puVar2 = DAT_020406a8;
    puVar1 = DAT_020406a4;
    uVar5 = DAT_020406a0[1];
    *param_1 = *DAT_020406a0;
    param_1[1] = uVar5;
    puVar3 = DAT_020406ac;
    uVar5 = puVar1[1];
    param_1[2] = *puVar1;
    param_1[3] = uVar5;
    puVar4 = DAT_020406b0;
    *(u16 *)(param_1 + 6) = *puVar2 & 3;
    uVar5 = puVar3[1];
    param_1[4] = *puVar3;
    param_1[5] = uVar5;
    *(u16 *)((int)param_1 + 0x1a) = *puVar4 & 1;
    return;
}

// FUN_020406e0 @ 0x020406e0 (296 bytes) — apply touch calibration transform
void FUN_020406e0(u16 *param_1, u16 *param_2)
{
    u16 uVar1;
    s64 lVar2;
    u32 *puVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;

    puVar3 = DAT_0204080c;
    if (*(s16 *)(DAT_02040808 + 0x30) == 0) {
        uVar1 = param_2[1];
        *param_1 = *param_2;
        param_1[1] = uVar1;
        uVar1 = param_2[3];
        param_1[2] = param_2[2];
        param_1[3] = uVar1;
        return;
    }
    param_1[2] = param_2[2];
    param_1[3] = param_2[3];
    if (param_2[2] != 0) {
        uVar4 = *puVar3;
        uVar6 = puVar3[2];
        uVar5 = (u32)*param_2 * 4 - uVar4;
        lVar2 = (u64)uVar5 * (u64)uVar6;
        *param_1 = (u16)((u64)lVar2 >> 0x10) >> 6 |
                   ((s16)((int)uVar6 >> 0x1f) * (s16)uVar5 +
                   (s16)uVar6 * -((s16)((int)uVar4 >> 0x1f) + (u16)((u32)*param_2 * 4 < uVar4))
                   + (s16)((u64)lVar2 >> 0x20)) * 0x400;
        if ((s16)*param_1 < 0) {
            *param_1 = 0;
        }
        else if (0xff < (s16)*param_1) {
            *param_1 = 0xff;
        }
        uVar4 = puVar3[3];
        uVar6 = puVar3[5];
        uVar5 = (u32)param_2[1] * 4 - uVar4;
        lVar2 = (u64)uVar5 * (u64)uVar6;
        param_1[1] = (u16)((u64)lVar2 >> 0x10) >> 6 |
                     ((s16)((int)uVar6 >> 0x1f) * (s16)uVar5 +
                     (s16)uVar6 *
                     -((s16)((int)uVar4 >> 0x1f) + (u16)((u32)param_2[1] * 4 < uVar4)) +
                     (s16)((u64)lVar2 >> 0x20)) * 0x400;
        if (-1 < (s16)param_1[1]) {
            if (0xbf < (s16)param_1[1]) {
                param_1[1] = 0xbf;
            }
            return;
        }
        param_1[1] = 0;
        return;
    }
    *param_1 = 0;
    param_1[1] = 0;
    return;
}

// FUN_02040810 @ 0x02040810 (608 bytes) — compute touch calibration matrix
u32 FUN_02040810(u16 *param_1, u32 param_2, u32 param_3, u32 param_4,
                 u16 param_5, u16 param_6, u16 param_7, u16 param_8, u16 param_9)
{
    int iVar1;
    u16 *puVar2;
    int *piVar3;
    int iVar4;

    if ((((0xfff < param_2) || (0xfff < param_3)) || (0xfff < param_6)) || (0xfff < param_7)) {
        return 1;
    }
    if (((0xff < param_4) || (0xff < param_8)) || ((0xbf < param_5 || (0xbf < param_9)))) {
        return 1;
    }
    if (((param_4 == param_8) || (param_5 == param_9)) ||
       ((param_2 == param_6 || (param_3 == param_7)))) {
        return 1;
    }
    FUN_0203ae38();
    piVar3 = DAT_02040a74;
    puVar2 = DAT_02040a70;
    *DAT_02040a70 = 0;
    *piVar3 = (param_2 - param_6) * 0x100;
    piVar3 = DAT_02040a78;
    *DAT_02040a78 = param_4 - param_8;
    piVar3[1] = 0;
    do {
    } while ((*puVar2 & 0x8000) != 0);
    iVar4 = *DAT_02040a7c;
    *puVar2 = 0;
    *DAT_02040a74 = (param_3 - param_7) * 0x100;
    piVar3 = DAT_02040a78;
    *DAT_02040a78 = (u32)param_5 - (u32)param_9;
    piVar3[1] = 0;
    if ((iVar4 < 0x8000) && (-0x8001 < iVar4)) {
        param_1[2] = (s16)iVar4;
        puVar2 = DAT_02040a70;
        iVar4 = ((param_2 + param_6) * 0x100 - (int)(s16)param_1[2] * (param_4 + param_8)) * 0x200;
        iVar1 = iVar4 >> 0x10;
        if ((0x7fff < iVar1) || (iVar1 < -0x8000)) {
            FUN_0203ae4c();
            return 1;
        }
        *param_1 = (s16)((u32)iVar4 >> 0x10);
        do {
        } while ((*puVar2 & 0x8000) != 0);
        iVar4 = *DAT_02040a7c;
        FUN_0203ae4c();
        if ((iVar4 < 0x8000) && (-0x8001 < iVar4)) {
            param_1[3] = (s16)iVar4;
            iVar4 = ((param_3 + param_7) * 0x100 -
                    (int)(s16)param_1[3] * ((u32)param_5 + (u32)param_9)) * 0x200;
            iVar1 = iVar4 >> 0x10;
            if ((iVar1 < 0x8000) && (-0x8001 < iVar1)) {
                param_1[1] = (s16)((u32)iVar4 >> 0x10);
                return 0;
            }
            return 1;
        }
        return 1;
    }
    FUN_0203ae4c();
    return 1;
}

// FUN_02040c68 @ 0x02040c68 (92 bytes) — get calibrated touch position
u32 FUN_02040c68(u16 *param_1)
{
    u16 uVar1;
    int iVar2;

    iVar2 = DAT_02040cc4;
    if ((*(u16 *)(DAT_02040cc4 + 0x34) & 1) == 0) {
        uVar1 = *(u16 *)(DAT_02040cc4 + 6);
        *param_1 = *(u16 *)(DAT_02040cc4 + 4);
        param_1[1] = uVar1;
        uVar1 = *(u16 *)(iVar2 + 10);
        param_1[2] = *(u16 *)(iVar2 + 8);
        param_1[3] = uVar1;
        FUN_020406e0(param_1, param_1);
        return 0;
    }
    return 1;
}

// FUN_02040cc8 @ 0x02040cc8 (156 bytes) — touch panel auto-sampling start
void FUN_02040cc8(void)
{
    u32 *puVar1;
    u32 uVar2;
    int iVar3;
    FuncPtr pcVar4;

    uVar2 = FUN_0203ae38();
    iVar3 = FUN_0203d518(6, 0x3000000, 0);
    puVar1 = DAT_02040d64;
    if (-1 < iVar3) {
        *(u16 *)((int)DAT_02040d64 + 0x36) = *(u16 *)((int)DAT_02040d64 + 0x36) | 1;
        *(u16 *)(puVar1 + 0xd) = *(u16 *)(puVar1 + 0xd) & 0xfffe;
        FUN_0203ae4c(uVar2);
        return;
    }
    FUN_0203ae4c(uVar2);
    pcVar4 = (FuncPtr)*DAT_02040d64;
    *(u16 *)(DAT_02040d64 + 0xd) = *(u16 *)(DAT_02040d64 + 0xd) | 1;
    if (pcVar4 == (FuncPtr)0x0) {
        return;
    }
    (*pcVar4)(0, 4, 0);
    return;
}

// FUN_02040d68 @ 0x02040d68 (308 bytes) — set touch calibration parameters
void FUN_02040d68(s16 *param_1)
{
    s16 sVar1;
    int iVar2;
    u16 *puVar3;
    u32 *puVar4;
    int *piVar5;

    if (param_1 != (s16 *)0x0) {
        FUN_0203ae38();
        puVar4 = DAT_02040ea4;
        puVar3 = DAT_02040ea0;
        iVar2 = DAT_02040e9c;
        sVar1 = param_1[2];
        if (sVar1 == 0) {
            *(u32 *)(DAT_02040e9c + 0x18) = 0;
            *(u32 *)(iVar2 + 0x1c) = 0;
            *(u32 *)(iVar2 + 0x20) = 0;
        }
        else {
            *DAT_02040ea0 = 0;
            piVar5 = DAT_02040ea8;
            *puVar4 = 0x10000000;
            *piVar5 = (int)sVar1;
            piVar5[1] = 0;
            iVar2 = DAT_02040e9c;
            *(int *)(DAT_02040e9c + 0x18) = (int)*param_1;
            *(int *)(iVar2 + 0x1c) = (int)param_1[2];
            do {
            } while ((*puVar3 & 0x8000) != 0);
            *(u32 *)(DAT_02040e9c + 0x20) = *DAT_02040eac;
        }
        puVar4 = DAT_02040ea4;
        puVar3 = DAT_02040ea0;
        iVar2 = DAT_02040e9c;
        sVar1 = param_1[3];
        if (sVar1 == 0) {
            *(u32 *)(DAT_02040e9c + 0x24) = 0;
            *(u32 *)(iVar2 + 0x28) = 0;
            *(u32 *)(iVar2 + 0x2c) = 0;
        }
        else {
            *DAT_02040ea0 = 0;
            piVar5 = DAT_02040ea8;
            *puVar4 = 0x10000000;
            *piVar5 = (int)sVar1;
            piVar5[1] = 0;
            iVar2 = DAT_02040e9c;
            *(int *)(DAT_02040e9c + 0x24) = (int)param_1[1];
            *(int *)(iVar2 + 0x28) = (int)param_1[3];
            do {
            } while ((*puVar3 & 0x8000) != 0);
            *(u32 *)(DAT_02040e9c + 0x2c) = *DAT_02040eac;
        }
        FUN_0203ae4c();
        *(u16 *)(DAT_02040e9c + 0x30) = 1;
        return;
    }
    *(u16 *)(DAT_02040e9c + 0x30) = 0;
    return;
}

// FUN_02040eb0 @ 0x02040eb0 (168 bytes) — get touch calibration result
u32 FUN_02040eb0(u16 *param_1)
{
    int iVar1;

    if ((((*(s16 *)(DAT_02040f58 + 0x58) != 0) || (*(s16 *)(DAT_02040f58 + 0x5e) != 0)) ||
        (*(s16 *)(DAT_02040f58 + 0x5a) != 0)) || (*(s16 *)(DAT_02040f58 + 0x60) != 0)) {
        iVar1 = FUN_02040810(param_1, *(s16 *)(DAT_02040f58 + 0x58), *(s16 *)(DAT_02040f58 + 0x5a),
                             *(u8 *)(DAT_02040f58 + 0x5c), *(u8 *)(DAT_02040f58 + 0x5d),
                             *(s16 *)(DAT_02040f58 + 0x5e), *(s16 *)(DAT_02040f58 + 0x60),
                             *(u8 *)(DAT_02040f58 + 0x62), *(u8 *)(DAT_02040f58 + 99));
        if (iVar1 == 0) {
            return 1;
        }
    }
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    return 1;
}

// FUN_02040f5c @ 0x02040f5c (132 bytes) — touch panel init
void FUN_02040f5c(void)
{
    u32 *puVar1;
    int iVar2;

    if (*DAT_02040fe0 == 0) {
        *DAT_02040fe0 = 1;
        thunk_FUN_0203d660();
        puVar1 = DAT_02040fe4;
        *(u16 *)((int)DAT_02040fe4 + 0x32) = 0;
        *(u16 *)((int)puVar1 + 0x36) = 0;
        *(u16 *)(puVar1 + 3) = 0;
        *puVar1 = 0;
        puVar1[4] = 0;
        *(u16 *)(puVar1 + 0xc) = 0;
        *(u16 *)(puVar1 + 0xd) = 0;
        do {
            iVar2 = FUN_0203d5cc(6, 1);
        } while (iVar2 == 0);
        FUN_0203d5f4(6, DAT_02040fe8);
        return;
    }
    return;
}

// FUN_02040fec @ 0x02040fec (692 bytes) — touch panel SPI callback
void FUN_02040fec(u32 param_1, u32 param_2, int param_3)
{
    u32 uVar1;
    u16 *puVar2;
    u32 *puVar3;
    int iVar4;
    FuncPtr pcVar5;
    u32 uVar6;
    u32 local_10;
    u32 uStack_c;

    puVar3 = DAT_020412a0;
    uVar1 = ((param_2 & 0x7f00) << 8) >> 0x10;
    if (param_3 != 0) {
        pcVar5 = (FuncPtr)*DAT_020412a0;
        *(u16 *)(DAT_020412a0 + 0xd) = *(u16 *)(DAT_020412a0 + 0xd) | (u16)(1 << uVar1);
        if (pcVar5 != (FuncPtr)0x0) {
            (*pcVar5)(uVar1, 4, 0);
            return;
        }
        return;
    }
    if (uVar1 == 0x10) {
        puVar2 = (u16 *)(DAT_020412a0 + 5);
        *(s16 *)(DAT_020412a0 + 3) = *(s16 *)(DAT_020412a0 + 3) + 1;
        if (*puVar2 <= *(u16 *)(puVar3 + 3)) {
            *(u16 *)(puVar3 + 3) = 0;
        }
        puVar3 = DAT_020412a0;
        local_10 = CONCAT22(*DAT_020412a8, *DAT_020412a4);
        iVar4 = DAT_020412a0[4] + (u32)*(u16 *)(DAT_020412a0 + 3) * 8;
        *(u16 *)(DAT_020412a0[4] + (u32)*(u16 *)(DAT_020412a0 + 3) * 8) =
             (u16)((u32)(local_10 << 0x14) >> 0x14);
        *(u16 *)(iVar4 + 2) = (u16)((u32)local_10 >> 8) >> 4;
        *(u16 *)(iVar4 + 4) = (u16)((u32)(local_10 << 7) >> 0x1f);
        *(u16 *)(iVar4 + 6) = (u16)((u32)(local_10 << 5) >> 0x1e);
        pcVar5 = (FuncPtr)*puVar3;
        if (pcVar5 != (FuncPtr)0x0) {
            (*pcVar5)(0x10, 0, *(u16 *)(puVar3 + 3) & 0xff);
            return;
        }
        return;
    }
    if ((param_2 & 0x1000000) == 0) {
        return;
    }
    switch (param_2 & 0xff) {
    case 0:
        if (uVar1 == 0) {
            uStack_c = CONCAT22(*DAT_020412a8, *DAT_020412a4);
            *(u16 *)((int)DAT_020412a0 + 0x32) = 0;
            *(u16 *)(puVar3 + 1) = (u16)((u32)(uStack_c << 0x14) >> 0x14);
            *(u16 *)((int)puVar3 + 6) = (u16)((u32)uStack_c >> 8) >> 4;
            *(u16 *)(puVar3 + 2) = (u16)((u32)(uStack_c << 7) >> 0x1f);
            *(u16 *)((int)puVar3 + 10) = (u16)((u32)(uStack_c << 5) >> 0x1e);
        }
        else if (uVar1 == 1) {
            *(u16 *)((int)DAT_020412a0 + 0x32) = 2;
        }
        else if (uVar1 == 2) {
            *(u16 *)((int)DAT_020412a0 + 0x32) = 0;
        }
        pcVar5 = (FuncPtr)*DAT_020412a0;
        *(u16 *)((int)DAT_020412a0 + 0x36) =
             *(u16 *)((int)DAT_020412a0 + 0x36) & ~(u16)(1 << uVar1);
        if (pcVar5 != (FuncPtr)0x0) {
            (*pcVar5)(uVar1, 0, 0);
            return;
        }
        return;
    case 1:
    default:
        FUN_0203aeb4();
        return;
    case 2:
        uVar6 = 1;
        break;
    case 3:
        uVar6 = 2;
        break;
    case 4:
        uVar6 = 3;
    }
    pcVar5 = (FuncPtr)*DAT_020412a0;
    *(u16 *)(DAT_020412a0 + 0xd) = *(u16 *)(DAT_020412a0 + 0xd) | (u16)(1 << uVar1);
    *(u16 *)((int)puVar3 + 0x36) = *(u16 *)((int)puVar3 + 0x36) & ~(u16)(1 << uVar1);
    if (pcVar5 != (FuncPtr)0x0) {
        (*pcVar5)(uVar1, uVar6, 0);
        return;
    }
    return;
}

// FUN_0204130c @ 0x0204130c (48 bytes) — execute callback chain
void FUN_0204130c(u32 *param_1)
{
    if (param_1 != (u32 *)0x0) {
        do {
            ((FuncPtr)param_1[0])(param_1[1]);
            param_1 = (u32 *)param_1[2];
        } while (param_1 != (u32 *)0x0);
        return;
    }
    return;
}

// FUN_02041400 @ 0x02041400 (228 bytes) — set power mode
u32 FUN_02041400(int param_1, int param_2, int param_3, int param_4)
{
    int *piVar1;
    int *piVar2;

    piVar2 = DAT_020414e8;
    piVar1 = DAT_020414e4;
    if (param_1 == 0) {
        *DAT_020414ec = *DAT_020414ec & 0xfffe;
        *piVar2 = *piVar1;
        if (param_2 != 0) {
            if (param_4 == 0) {
                FUN_0204192c(param_2, 0, 0);
            }
            else {
                FUN_020418f0(param_2);
            }
        }
    }
    else if (param_1 == 1) {
        if ((param_3 == 0) && ((u32)(*DAT_020414e4 - *DAT_020414e8) < 8)) {
            return 0;
        }
        if (param_2 != 0) {
            if (param_4 == 0) {
                FUN_0204192c(param_2, 0, 0);
            }
            else {
                FUN_020418f0(param_2);
            }
        }
        *DAT_020414ec = *DAT_020414ec | 1;
    }
    return 1;
}

// FUN_020414f0 @ 0x020414f0 (532 bytes) — power management / shutdown sequence
void FUN_020414f0(u32 param_1, u16 param_2, u16 param_3)
{
    u16 uVar1;
    u16 uVar2;
    BOOL bVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;
    int iVar7;
    int iVar8;
    u32 uVar9;
    int local_34;
    int local_30;
    int local_2c;

    bVar3 = FALSE;
    FUN_0204130c(*DAT_02041704);
    uVar1 = *DAT_02041708;
    *DAT_02041708 = 0;
    uVar5 = FUN_0203ae38();
    uVar6 = FUN_02038c80(DAT_0204170c);
    FUN_02038d00(0x40000);
    FUN_0203ae4c(uVar5);
    *DAT_02041708 = 1;
    if (((param_1 & 8) != 0) && (*DAT_02041710 == 2)) {
        param_1 = param_1 & 0xfffffff7;
    }
    if (((param_1 & 0x10) != 0) && (iVar7 = FUN_02043d7c(), iVar7 == 0)) {
        param_1 = param_1 & 0xffffffef;
    }
    uVar4 = _DAT_04000000;
    uVar9 = *DAT_02041714;
    iVar7 = FUN_020413e4();
    FUN_02041758(&local_34, &local_30);
    FUN_02041824(2, 0);
    do {
    } while (*DAT_02041718 == *DAT_02041718);
    local_2c = *DAT_02041718;
    FUN_02035218();
    *DAT_02041714 = *DAT_02041714 & 0xfffeffff;
    do {
    } while (local_2c == (int)*DAT_02041718);
    local_2c = *DAT_02041718;
    do {
    } while (local_2c == (int)*DAT_02041718);
    do {
        iVar8 = FUN_02041abc((param_1 | local_34 << 5 | local_30 << 6) & 0xffff, param_2 | param_3);
    } while (iVar8 != 0);
    FUN_0203ae9c();
    FUN_0203aea8(DAT_0204171c);
    if (((param_1 & 8) != 0) && ((*DAT_02041720 & 0x100000) != 0)) {
        bVar3 = TRUE;
    }
    if (!bVar3) {
        if (iVar7 == 1) {
            FUN_02041400(1, 1, 1, 0);
            FUN_020351cc();
        }
        else {
            FUN_020418f0(1);
        }
        _DAT_04000000 = uVar4;
        *DAT_02041714 = uVar9;
    }
    FUN_0203ae38();
    FUN_02038d00(uVar6);
    FUN_0203ae4c(uVar5);
    uVar2 = *DAT_02041708;
    *DAT_02041708 = uVar1;
    if (bVar3) {
        FUN_020417d0(uVar2);
    }
    FUN_0204130c(*DAT_02041724);
    return;
}

// FUN_02041728 @ 0x02041728 (48 bytes) — send SPI command
void FUN_02041728(u32 param_1)
{
    int iVar1;

    do {
        iVar1 = FUN_0203d518(8, param_1, 0);
    } while (iVar1 != 0);
    return;
}

// FUN_02041758 @ 0x02041758 (120 bytes) — get power flags
void FUN_02041758(u32 *param_1, u32 *param_2)
{
    int iVar1;
    u16 auStack_10[2];

    iVar1 = FUN_02041994(0, (u32)auStack_10);
    if (iVar1 != 0) {
        return;
    }
    if (param_1 != (u32 *)0x0) {
        *param_1 = (u32)((auStack_10[0] & 8) != 0);
    }
    if (param_2 != (u32 *)0x0) {
        *param_2 = (u32)((auStack_10[0] & 4) != 0);
        return;
    }
    return;
}

// FUN_020417d0 @ 0x020417d0 (56 bytes) — execute power command
int FUN_020417d0(u16 param_1)
{
    int iVar1;
    int aiStack_8[2];

    iVar1 = FUN_0204180c(DAT_02041808, aiStack_8);
    if (iVar1 == 0) {
        FUN_02041d80();
        return aiStack_8[0];
    }
    return iVar1;
}

// FUN_02041824 @ 0x02041824 (56 bytes) — send power management command
int FUN_02041824(u32 param_1, u32 param_2)
{
    int iVar1;
    int aiStack_8[2];

    iVar1 = FUN_02041860(param_1, param_2, DAT_0204185c, (u32)aiStack_8);
    if (iVar1 == 0) {
        FUN_02041d80();
        return aiStack_8[0];
    }
    return iVar1;
}

// FUN_02041860 @ 0x02041860 (140 bytes) — map power command ID
u32 FUN_02041860(int param_1, int param_2, u32 param_3, u32 param_4)
{
    u32 uVar1;
    int iVar2;

    iVar2 = 0;
    if (param_1 == 0) {
        if (param_2 == 1) {
            iVar2 = 6;
        }
        if (param_2 == 0) {
            iVar2 = 7;
        }
    }
    else if (param_1 == 1) {
        if (param_2 == 1) {
            iVar2 = 4;
        }
        if (param_2 == 0) {
            iVar2 = 5;
        }
    }
    else if (param_1 == 2) {
        if (param_2 == 1) {
            iVar2 = 8;
        }
        if (param_2 == 0) {
            iVar2 = 9;
        }
    }
    if (iVar2 == 0) {
        return DAT_020418ec;
    }
    uVar1 = FUN_02041a48(iVar2, param_3, param_4);
    return uVar1;
}

// FUN_020418f0 @ 0x020418f0 (56 bytes) — send power state command
int FUN_020418f0(u32 param_1)
{
    int iVar1;
    int aiStack_8[2];

    iVar1 = FUN_0204192c(param_1, DAT_02041928, (u32)aiStack_8);
    if (iVar1 == 0) {
        FUN_02041d80();
        return aiStack_8[0];
    }
    return iVar1;
}

// FUN_0204192c @ 0x0204192c (100 bytes) — map and send power command
u32 FUN_0204192c(int param_1, u32 param_2, u32 param_3)
{
    int iVar1;

    if (param_1 == 1) {
        iVar1 = 1;
    }
    else if (param_1 == 2) {
        iVar1 = 3;
    }
    else if (param_1 == 3) {
        iVar1 = 2;
    }
    else {
        iVar1 = 0;
    }
    if (iVar1 != 0) {
        return FUN_02041a48(iVar1, param_2, param_3);
    }
    return 0;
}

// FUN_02041994 @ 0x02041994 (56 bytes) — read SPI register
int FUN_02041994(u32 param_1, u32 param_2)
{
    int iVar1;
    int aiStack_8[2];

    iVar1 = FUN_020419d0(param_1, param_2, DAT_020419cc, (u32)aiStack_8);
    if (iVar1 == 0) {
        FUN_02041d80();
        return aiStack_8[0];
    }
    return iVar1;
}

// FUN_020419d0 @ 0x020419d0 (104 bytes) — SPI read register low-level
u32 FUN_020419d0(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    int iVar1;
    u32 uVar2;
    int iVar3;
    int iVar4;

    iVar4 = FUN_02041d98();
    iVar3 = DAT_02041a44;
    uVar2 = DAT_02041a40;
    iVar1 = DAT_02041a3c;
    if (iVar4 != 0) {
        *(u16 *)(DAT_02041a38 + param_1 * 8) = 0;
        *(u32 *)(iVar1 + 4) = param_3;
        *(u32 *)(iVar1 + 8) = param_4;
        *(u32 *)(iVar3 + param_1 * 8) = param_2;
        FUN_02041728(param_1 & 0xff | uVar2);
        return 0;
    }
    return 1;
}

// FUN_02041a48 @ 0x02041a48 (100 bytes) — SPI write register low-level
u32 FUN_02041a48(u32 param_1, u32 param_2, u32 param_3)
{
    int iVar1;
    int iVar2;
    u32 uVar3;

    iVar2 = FUN_02041d98();
    iVar1 = DAT_02041aac;
    if (iVar2 != 0) {
        uVar3 = param_1 >> 0x10 & 0xff | DAT_02041ab0;
        *(u32 *)(DAT_02041aac + 4) = param_2;
        *(u32 *)(iVar1 + 8) = param_3;
        FUN_02041728(uVar3);
        FUN_02041728(param_1 & DAT_02041ab4 | DAT_02041ab8);
        return 0;
    }
    return 1;
}
