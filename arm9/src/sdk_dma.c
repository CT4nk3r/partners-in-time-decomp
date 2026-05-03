/**
 * SDK DMA — OS threading, timers, DMA transfers, decompression (0x020398dc-0x0203ca54)
 * Decompiled from 0x020398dc-0x0203ca54 region (58 functions)
 * Note: Several functions contain ASM-only constructs (CPSR, coprocessor ops)
 */
#include "types.h"

/* ------------------------------------------------------------------ */
/*  External function declarations                                     */
/* ------------------------------------------------------------------ */
extern void FUN_02038c80(...);
extern void FUN_02038cc8(...);
extern u32  FUN_02038d34(...);
extern void FUN_02038d80(...);
extern void FUN_02038dcc(...);
extern u32  FUN_02038e5c(...);
extern u16  FUN_02038e7c(...);
extern u32  FUN_02038f14(...);
extern int  FUN_02038fb4(...);
extern void FUN_02038fd4(...);
extern u32  FUN_02038ff4(...);
extern void FUN_0203919c(...);
extern void FUN_020394d4(...);
extern void FUN_02039548(...);
extern void FUN_02039714(...);
extern int  FUN_02039be8(...);
extern int  FUN_02039d64(...);
extern void FUN_02039d8c(...);
extern void FUN_02039db0(...);
extern u32  FUN_0203a0f0(...);
extern u32  FUN_0203a104(...);
extern void FUN_0203a4ec(...);
extern void FUN_0203a4fc(...);
extern void FUN_0203a50c(...);
extern void FUN_0203a514(...);
extern void FUN_0203a738(...);
extern void FUN_0203aeb4(...);
extern void FUN_0203aea8(...);
extern void FUN_020432a8(...);
extern void FUN_02041c60(...);
extern void FUN_020441e8(...);
extern s64  FUN_020462a8(...);
extern int  FUN_0203b0d0(...);
extern void FUN_0203b160(...);
extern void FUN_0203b7c0(...);
extern void FUN_0203bd84(...);
extern int  FUN_0203c4b8(...);
extern void FUN_0203c67c(...);
extern int  FUN_0203c920(...);
extern void FUN_0203cb6c(...);
extern void FUN_0203cc4c(...);
extern int  FUN_0203d518(...);
extern int  FUN_0203d5cc(...);
extern void FUN_0203d5f4(...);
extern void thunk_FUN_0203d660(...);
extern void func_0x01ff85cc(...);
extern void func_0x01ff8520(...);
extern void func_0x01ff8558(...);
extern void func_0x01ff84c0(...);

/* ------------------------------------------------------------------ */
/*  External data references (DAT_ labels from Ghidra)                 */
/* ------------------------------------------------------------------ */
extern u32  DAT_02039940;
extern u32  DAT_02039a20;
extern u32  DAT_02039a64;
extern u16 *DAT_02039a90;
extern u16 *DAT_02039a94;
extern u16 *DAT_02039a98;
extern u32  DAT_02039b60;
extern u32  DAT_02039b64;
extern u32 *DAT_02039b68;
extern u32  DAT_02039b6c;
extern u32 *DAT_02039be0;
extern u32 *DAT_02039be4;
extern u32  DAT_02039e44;
extern u32  DAT_02039f04;
extern u32  DAT_02039f90;
extern u32 *DAT_0203a3b0;
extern u32 *DAT_0203a3b4;
extern u32  DAT_0203a3b8;
extern u32  DAT_0203a3bc;
extern u32 *DAT_0203a4d4;
extern u32  DAT_0203a4e8;
extern u32 *DAT_0203a638;
extern u32 *DAT_0203a698;
extern u32  DAT_0203a69c;
extern u32 *DAT_0203a6a0;
extern u32  DAT_0203a6a4;
extern u32 *DAT_0203a724;
extern u32 *DAT_0203a728;
extern u32  DAT_0203a72c;
extern u32  DAT_0203a730;
extern u32 *DAT_0203a734;
extern u16 *DAT_0203a7f4;
extern u32 *DAT_0203a7f8;
extern u32  DAT_0203a7fc;
extern u32 *DAT_0203a800;
extern u32 *DAT_0203a878;
extern int *DAT_0203a87c;
extern u16 *DAT_0203a880;
extern u16 *DAT_0203a884;
extern u32  DAT_0203a888;
extern u32 *DAT_0203a910;
extern u32 *DAT_0203a914;
extern u16 *DAT_0203a918;
extern u16 *DAT_0203a91c;
extern u32  DAT_0203a920;
extern u32 *DAT_0203a924;
extern u32 *DAT_0203aa44;
extern u32  DAT_0203aa48;
extern u32 *DAT_0203aa4c;
extern u32  DAT_0203ac9c;
extern u16 *DAT_0203ada4;
extern u32  DAT_0203ada8;
extern u32  DAT_0203adac;
extern s16 *DAT_0203adb0;
extern u32 *DAT_0203ae14;
extern u32 *DAT_0203ae18;
extern u32 *DAT_0203ae1c;
extern u32 *DAT_0203ae20;
extern u32 *DAT_0203b01c;
extern u32  DAT_0203b020;
extern u32  DAT_0203b0b0;
extern u32  DAT_0203b0c8;
extern u32  DAT_0203b0cc;
extern u32 *DAT_0203b154;
extern u32  DAT_0203b158;
extern u32  DAT_0203b15c;
extern u32  DAT_0203b2c4;
extern u32  DAT_0203b344;
extern u32  DAT_0203b348;
extern u32  DAT_0203b3b4;
extern u32  DAT_0203b3b8;
extern u32  DAT_0203b538;
extern u32  DAT_0203b5b0;
extern u32  DAT_0203b6b8;
extern u32  DAT_0203b6bc;
extern u32  DAT_0203b730;
extern u32  DAT_0203b7a4;
extern u32  DAT_0203bd7c;
extern u32  DAT_0203bd80;
extern int *DAT_0203c38c;
extern u32 *DAT_0203c3e0;
extern u32 *DAT_0203c3e4;
extern u32  DAT_0203c484;
extern u32 *DAT_0203c514;
extern u32 *DAT_0203c554;
extern u32 **DAT_0203c87c;
extern u32 *DAT_0203c880;
extern u32 *DAT_0203cb34;
extern u32 *DAT_0203cb38;
extern u32  DAT_0203cb3c;
extern u32 *DAT_0203cb40;
extern u32 *DAT_0203cb44;
extern u32 *DAT_0203cb48;
extern u32 *DAT_0203cb4c;
extern u32 *DAT_0203cb50;
extern u32 *DAT_0203cb54;
extern u32  DAT_0203cb58;
extern u32 *DAT_0203cb5c;
extern u32 *DAT_0203cb60;
extern u32  DAT_0203cb64;
extern u32 *DAT_0203cb68;
extern u32  DAT_0203a1e8;
extern u32  DAT_0203a1ec;
extern u32  DAT_0203a1f0;
extern u32  DAT_0203a1f4;
extern u32  DAT_0203a1f8;
extern u32  DAT_0203a1fc;
extern u32  DAT_0203a30c;
extern u32 *DAT_0203a310;
extern u32  DAT_0203a314;
extern u32  DAT_0203a318;
extern u32  DAT_0203a31c;
extern u32  DAT_0203a320;
extern u32  DAT_0203a324;
extern u32  DAT_0203a328;

extern vu32 _DAT_040000b0;
extern vu32 _DAT_08000000;

/* ------------------------------------------------------------------ */
/*  Forward declarations for functions in this file                    */
/* ------------------------------------------------------------------ */
void FUN_020398dc(int param_1);
void FUN_02039974(u32 *param_1, int param_2, int param_3);
/* FUN_020399d8: ASM-only (CPSR register access) */
/* FUN_02039a24: ASM-only (indirect jump) */
BOOL FUN_02039a68(void);
u32  FUN_02039a9c(void);
u32  FUN_02039b70(void);
void FUN_02039d44(u16 *param_1, u32 param_2, u32 param_3);
u32  FUN_02039dd4(int param_1);
void FUN_02039e48(int param_1);
void FUN_02039e98(int param_1);
void FUN_02039f08(int param_1);
/* FUN_02039fe0: ASM-only (coprocessor instructions) */
void FUN_0203a0a4(void);
u32  FUN_0203a118(u32 param_1);
u32  FUN_0203a200(u32 param_1);
void FUN_0203a32c(void);
void FUN_0203a3c0(void);
/* FUN_0203a4d8: ASM-only (coprocessor movefrom) */
/* FUN_0203a5ac: ASM-only (CPSR register access) */
void FUN_0203a63c(void);
void FUN_0203a6a8(void);
u64  FUN_0203a754(void);
void FUN_0203a804(void);
void FUN_0203a88c(void);
void FUN_0203a938(void);
void FUN_0203ab60(int param_1, u32 param_2, u32 param_3);
void FUN_0203ad18(int param_1);
void FUN_0203adb4(void);
/* FUN_0203ae24: ASM-only (CPSR register access) */
/* FUN_0203ae64: ASM-only (CPSR register access) */
void FUN_0203afbc(void);
void FUN_0203b038(u8 *param_1);
void FUN_0203b0b4(u32 param_1);
void FUN_0203b0d8(u32 param_1, u32 param_2);
void FUN_0203b1a4(int param_1, u32 param_2, int param_3, int param_4);
void FUN_0203b214(int param_1, u32 param_2);
void FUN_0203b2c8(int param_1);
void FUN_0203b34c(int param_1);
void FUN_0203b3bc(u32 param_1, u32 param_2, u32 param_3, u32 param_4, void *param_5, u32 param_6);
void FUN_0203b464(int param_1, u32 param_2, u32 param_3, u32 param_4, void *param_5, u32 param_6);
void FUN_0203b53c(int param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_0203b630(int param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_0203b6c0(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_0203b734(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_0203bae0(u32 *param_1, u16 *param_2);
void FUN_0203bbb8(u32 *param_1, u32 *param_2);
void FUN_0203bcf8(int param_1, u32 param_2, u32 param_3, int param_4);
BOOL FUN_0203c348(void);
int *FUN_0203c390(void);
void FUN_0203c3e8(void);
void FUN_0203c41c(void);
void FUN_0203c488(u32 param_1, u32 param_2);
int  FUN_0203c4d8(void);
int  FUN_0203c518(void);
void FUN_0203c83c(u32 *param_1);
int  FUN_0203c884(u32 param_1);
void FUN_0203ca54(void);

/* Declarations for ASM-only functions */
u32  FUN_020399d8(int *param_1, u32 param_2, u32 param_3, int param_4);
void FUN_02039a24(int param_1);
void FUN_02039fe0(void);
u32  FUN_0203a4d8(void);
int  FUN_0203a5ac(u32 param_1);
s64  FUN_0203ae24(void);
s64  FUN_0203ae64(void);
u32  FUN_0203ae38(...);
void FUN_0203ae4c(...);

/* ================================================================== */
/*  Function implementations (address order)                           */
/* ================================================================== */

/* 0x020398dc */
void FUN_020398dc(int param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = DAT_02039940;
    iVar2 = 0;
    for (iVar3 = *(int *)(DAT_02039940 + 0xc);
        (iVar3 != 0 && (*(u32 *)(iVar3 + 0x70) < *(u32 *)(param_1 + 0x70)));
        iVar3 = *(int *)(iVar3 + 0x68)) {
        iVar2 = iVar3;
    }
    if (iVar2 == 0) {
        *(int *)(param_1 + 0x68) = *(int *)(DAT_02039940 + 0xc);
        *(int *)(iVar1 + 0xc) = param_1;
    }
    else {
        *(u32 *)(param_1 + 0x68) = *(u32 *)(iVar2 + 0x68);
        *(int *)(iVar2 + 0x68) = param_1;
    }
}

/* 0x02039974 */
void FUN_02039974(u32 *param_1, int param_2, int param_3)
{
    u32 uVar1;

    param_1[0x10] = param_2 + 4U;
    param_1[0x11] = param_3;
    param_1[0xe] = param_3 + -0x40;
    if ((param_2 + 4U & 1) == 0) {
        uVar1 = 0x1f;
    }
    else {
        uVar1 = 0x3f;
    }
    *param_1 = uVar1;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    param_1[0xd] = 0;
    param_1[0xf] = 0;
}

/* 0x020399d8 — ASM-only: saves context/CPSR, needs handwritten ASM */
/* u32 FUN_020399d8(int *param_1, u32 param_2, u32 param_3, int param_4) */

/* 0x02039a24 — ASM-only: restores context via indirect jump */
/* void FUN_02039a24(int param_1) */

/* 0x02039a68 */
BOOL FUN_02039a68(void)
{
    u16 *psVar1;

    psVar1 = DAT_02039a94;
    if (*DAT_02039a90 != 0) {
        psVar1 = DAT_02039a98;
    }
    return *psVar1 == 1;
}

/* 0x02039a9c */
u32 FUN_02039a9c(void)
{
    BOOL bVar1;
    u16 uVar2;
    u32 uVar3;
    u32 uVar4;
    int iVar5;
    int iVar6;
    u32 uVar7;
    u32 local_30;

    bVar1 = FALSE;
    uVar2 = FUN_02038e7c();
    local_30 = 0x2000000;
    uVar7 = 0;
    do {
        iVar6 = -1;
        uVar3 = FUN_0203ae38();
        uVar4 = FUN_02038f14(DAT_02039b60);
        if ((uVar4 & 0x40) == 0) {
            iVar5 = FUN_02038fb4(uVar2);
            iVar6 = 0;
            if (iVar5 == 0) goto LAB_02039b00;
        }
        else {
LAB_02039b00:
            uVar7 = local_30;
            if ((_DAT_08000000 == DAT_02039b64) && (*DAT_02039b68 == DAT_02039b6c)) {
                uVar7 = 0x1000000;
            }
            if (iVar6 == 0) {
                FUN_02038fd4(uVar2);
                bVar1 = TRUE;
            }
        }
        FUN_0203ae4c(uVar3);
        if (bVar1) {
            return uVar7;
        }
    } while (TRUE);
}

/* 0x02039b70 */
u32 FUN_02039b70(void)
{
    u32 uVar1;
    int iVar2;

    if (*DAT_02039be0 == 0xffffffff) {
        uVar1 = FUN_02039a9c();
        iVar2 = FUN_02039be8();
        if (iVar2 == 0) {
            iVar2 = FUN_02039a68();
            if (iVar2 == 0) {
                if ((uVar1 & 0x1000000) == 0) {
                    uVar1 = uVar1 | 0x80000000;
                }
                else {
                    uVar1 = uVar1 | 0x20000000;
                }
            }
            else {
                uVar1 = uVar1 | 0x40000000;
            }
        }
        else {
            uVar1 = uVar1 | 0x10000000;
        }
        *DAT_02039be0 = uVar1 | *DAT_02039be4;
    }
    return *DAT_02039be0;
}

/* 0x02039d44 */
void FUN_02039d44(u16 *param_1, u32 param_2, u32 param_3)
{
    *param_1 = 0;
    param_1[1] = 0;
    *(u32 *)(param_1 + 2) = param_2;
    *(u32 *)(param_1 + 4) = param_3;
    *(u32 *)(param_1 + 6) = 0;
    *(u32 *)(param_1 + 8) = 0;
}

/* 0x02039dd4 */
u32 FUN_02039dd4(int param_1)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;

    uVar1 = FUN_0203ae38();
    iVar2 = *(int *)(DAT_02039e44 + 8);
    if (*(int *)(param_1 + 4) == 0) {
        *(int *)(param_1 + 4) = iVar2;
        *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
        FUN_02039db0(iVar2, param_1);
        uVar3 = 1;
    }
    else if (*(int *)(param_1 + 4) == iVar2) {
        uVar3 = 1;
        *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    }
    else {
        uVar3 = 0;
    }
    FUN_0203ae4c(uVar1);
    return uVar3;
}

/* 0x02039e48 */
void FUN_02039e48(int param_1)
{
    int iVar1;

    if (*(int *)(param_1 + 0x7c) == 0) {
        return;
    }
    do {
        iVar1 = FUN_02039d64(param_1);
        *(u32 *)(iVar1 + 8) = 0;
        *(u32 *)(iVar1 + 4) = 0;
        FUN_020394d4();
    } while (*(int *)(param_1 + 0x7c) != 0);
}

/* 0x02039e98 */
void FUN_02039e98(int param_1)
{
    u32 uVar1;
    int iVar2;

    uVar1 = FUN_0203ae38();
    iVar2 = *(int *)(DAT_02039f04 + 8);
    if (*(int *)(param_1 + 4) == iVar2) {
        *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + -1;
        if (*(int *)(param_1 + 8) == 0) {
            FUN_02039d8c(iVar2, param_1);
            *(u32 *)(param_1 + 4) = 0;
            FUN_020394d4(param_1);
        }
    }
    FUN_0203ae4c(uVar1);
}

/* 0x02039f08 */
void FUN_02039f08(int param_1)
{
    u32 uVar1;
    int iVar2;

    uVar1 = FUN_0203ae38();
    iVar2 = *(int *)(DAT_02039f90 + 8);
    do {
        if (*(int *)(param_1 + 4) == 0) {
            *(int *)(param_1 + 4) = iVar2;
            *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
            FUN_02039db0(iVar2, param_1);
            FUN_0203ae4c(uVar1);
            return;
        }
        if (*(int *)(param_1 + 4) == iVar2) {
            *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
            FUN_0203ae4c(uVar1);
            return;
        }
        *(int *)(iVar2 + 0x78) = param_1;
        FUN_02039548(param_1);
        *(u32 *)(iVar2 + 0x78) = 0;
    } while (TRUE);
}

/* 0x02039fe0 — ASM-only: cache flush using coprocessor instructions */
/* void FUN_02039fe0(void) */

/* 0x0203a0a4 */
void FUN_0203a0a4(void)
{
    FUN_0203a3c0();
    thunk_FUN_0203d660();
    FUN_0203919c();
    FUN_0203a32c();
    FUN_02038e5c();
    FUN_0203a6a8();
    FUN_0203bd84();
    FUN_0203adb4();
    FUN_0203b160();
    FUN_02039714();
    FUN_0203afbc();
    FUN_020441e8();
    FUN_020432a8();
    FUN_02041c60();
}

/* 0x0203a118 */
u32 FUN_0203a118(u32 param_1)
{
    u32 uVar1;

    switch (param_1) {
    case 0:
        return DAT_0203a1e8;
    case 1:
        return 0;
    case 2:
        break;
    case 3:
        return DAT_0203a1f0;
    case 4:
        return DAT_0203a1f4;
    case 5:
        return DAT_0203a1f8;
    case 6:
        return DAT_0203a1fc;
    default:
        return 0;
    }
    if ((*(u32 *)DAT_0203a1ec != 0) && (uVar1 = FUN_02039b70(), (uVar1 & 3) != 1)) {
        return 0x2400000;
    }
    return 0;
}

/* 0x0203a200 */
u32 FUN_0203a200(u32 param_1)
{
    u32 uVar1;

    switch (param_1) {
    case 0:
        return DAT_0203a30c;
    case 1:
        return 0;
    case 2:
        break;
    case 3:
        return 0x2000000;
    case 4:
        if (DAT_0203a318 != 0) {
            if ((int)DAT_0203a318 < 0) {
                uVar1 = DAT_0203a314 - DAT_0203a318;
            }
            else {
                uVar1 = ((DAT_0203a314 + 0x3f80) - DAT_0203a31c) - DAT_0203a318;
            }
            return uVar1;
        }
        uVar1 = DAT_0203a314;
        if (DAT_0203a314 < DAT_0203a320) {
            uVar1 = DAT_0203a320;
        }
        return uVar1;
    case 5:
        return DAT_0203a324;
    case 6:
        return DAT_0203a328;
    default:
        return 0;
    }
    if ((*DAT_0203a310 != 0) && (uVar1 = FUN_02039b70(), (uVar1 & 3) != 1)) {
        return 0x2700000;
    }
    return 0;
}

/* 0x0203a32c */
void FUN_0203a32c(void)
{
    u32 uVar1;
    u32 uVar2;

    uVar1 = FUN_0203a200(2);
    FUN_0203a104(2, uVar1);
    uVar1 = FUN_0203a118(2);
    FUN_0203a0f0(2, uVar1);
    if (((*DAT_0203a3b0 != 0) && (*DAT_0203a3b4 != 2)) && (uVar2 = FUN_02039b70(), (uVar2 & 3) != 1))
    {
        return;
    }
    FUN_0203a50c(DAT_0203a3b8);
    FUN_0203a514(DAT_0203a3bc);
}

/* 0x0203a3c0 */
void FUN_0203a3c0(void)
{
    u32 uVar1;

    if (*DAT_0203a4d4 == 0) {
        *DAT_0203a4d4 = 1;
        uVar1 = FUN_0203a200(0);
        FUN_0203a104(0, uVar1);
        uVar1 = FUN_0203a118(0);
        FUN_0203a0f0(0, uVar1);
        FUN_0203a0f0(2, 0);
        FUN_0203a104(2, 0);
        uVar1 = FUN_0203a200(3);
        FUN_0203a104(3, uVar1);
        uVar1 = FUN_0203a118(3);
        FUN_0203a0f0(3, uVar1);
        uVar1 = FUN_0203a200(4);
        FUN_0203a104(4, uVar1);
        uVar1 = FUN_0203a118(4);
        FUN_0203a0f0(4, uVar1);
        uVar1 = FUN_0203a200(5);
        FUN_0203a104(5, uVar1);
        uVar1 = FUN_0203a118(5);
        FUN_0203a0f0(5, uVar1);
        uVar1 = FUN_0203a200(6);
        FUN_0203a104(6, uVar1);
        uVar1 = FUN_0203a118(6);
        FUN_0203a0f0(6, uVar1);
    }
}

/* 0x0203a4d8 — ASM-only: reads coprocessor register, needs handwritten ASM */
/* u32 FUN_0203a4d8(void) */

/* 0x0203a5ac — ASM-only: saves exception context/CPSR, needs handwritten ASM */
/* int FUN_0203a5ac(u32 param_1) */

/* 0x0203a63c */
void FUN_0203a63c(void)
{
    u8 auStack_8[8];

    if (*DAT_0203a698 != 0) {
        FUN_0203a4ec(auStack_8, DAT_0203a69c);
        (*(void (*)(...))*DAT_0203a698)(DAT_0203a6a4, *DAT_0203a6a0);
        FUN_0203a4fc();
    }
}

/* 0x0203a6a8 */
void FUN_0203a6a8(void)
{
    int iVar1;
    u32 uVar2;

    uVar2 = *DAT_0203a724;
    if ((uVar2 < 0x2600000) || (0x27fffff < uVar2)) {
        *DAT_0203a728 = 0;
    }
    else {
        *DAT_0203a728 = uVar2;
    }
    if ((*DAT_0203a728 == 0) || (uVar2 = FUN_02039b70(), (uVar2 & 0x40000000) == 0)) {
        iVar1 = DAT_0203a730;
        uVar2 = DAT_0203a72c;
        *DAT_0203a724 = DAT_0203a72c;
        *(u32 *)(iVar1 + 0xfdc) = uVar2;
    }
    *DAT_0203a734 = 0;
}

/* 0x0203a754 */
u64 FUN_0203a754(void)
{
    u16 uVar1;
    BOOL bVar2;
    u32 local_c;
    u32 local_8;

    FUN_0203ae38();
    uVar1 = *DAT_0203a7f4;
    local_c = *DAT_0203a7f8;
    local_8 = DAT_0203a7f8[1] & DAT_0203a7fc;
    if (((*DAT_0203a800 & 8) != 0) && ((uVar1 & 0x8000) == 0)) {
        bVar2 = 0xfffffffe < local_c;
        local_c = local_c + 1;
        local_8 = local_8 + bVar2;
    }
    FUN_0203ae4c();
    return ((u64)local_8 << 0x10 | local_c >> 0x10) |
           ((u64)((u32)uVar1 | local_c << 0x10) & 0xFFFFFFFF);
}

/* 0x0203a804 */
void FUN_0203a804(void)
{
    u32 *puVar1;
    u32 *puVar2;
    int *piVar3;
    u16 *puVar4;
    u16 *puVar5;
    int iVar6;
    u32 uVar7;

    piVar3 = DAT_0203a87c;
    puVar2 = DAT_0203a878;
    uVar7 = *DAT_0203a878;
    puVar1 = DAT_0203a878 + 1;
    iVar6 = *DAT_0203a87c;
    *DAT_0203a878 = uVar7 + 1;
    puVar2[1] = *puVar1 + (u32)(0xfffffffe < uVar7);
    puVar5 = DAT_0203a884;
    puVar4 = DAT_0203a880;
    if (iVar6 != 0) {
        *DAT_0203a880 = 0;
        *puVar5 = 0;
        *puVar4 = 0xc1;
        *piVar3 = 0;
    }
    FUN_02038d34(0, DAT_0203a888, 0);
}

/* 0x0203a88c */
void FUN_0203a88c(void)
{
    u32 *puVar1;
    u16 *puVar2;
    u16 *puVar3;
    u32 uVar4;

    if (*DAT_0203a910 == 0) {
        *DAT_0203a910 = 1;
        FUN_0203a738(0);
        puVar1 = DAT_0203a914;
        *DAT_0203a914 = 0;
        puVar2 = DAT_0203a918;
        puVar1[1] = 0;
        puVar3 = DAT_0203a91c;
        *puVar2 = 0;
        uVar4 = DAT_0203a920;
        *puVar3 = 0;
        *puVar2 = 0xc1;
        FUN_02038dcc(8, uVar4);
        FUN_02038cc8(8);
        *DAT_0203a924 = 0;
    }
}

/* 0x0203a938 */
void FUN_0203a938(void)
{
    int *piVar1;
    u32 uVar2;
    int iVar3;
    u32 *puVar4;
    void *pcVar5;
    BOOL bVar6;
    u64 uVar7;

    *DAT_0203aa44 = 0;
    FUN_02038c80(0x10);
    *(u32 *)(DAT_0203aa48 + 0x3ff8) = *(u32 *)(DAT_0203aa48 + 0x3ff8) | 0x10;
    uVar7 = FUN_0203a754();
    piVar1 = (int *)DAT_0203aa4c;
    uVar2 = (u32)(uVar7 >> 0x20);
    puVar4 = (u32 *)*DAT_0203aa4c;
    if (puVar4 == (u32 *)0) {
        return;
    }
    bVar6 = (u32)puVar4[4] <= uVar2;
    if (uVar2 == puVar4[4]) {
        bVar6 = (u32)puVar4[3] <= (u32)uVar7;
    }
    if (bVar6) {
        iVar3 = puVar4[6];
        if (iVar3 == 0) {
            DAT_0203aa4c[1] = 0;
        }
        *piVar1 = iVar3;
        if (iVar3 != 0) {
            *(u32 *)(iVar3 + 0x14) = 0;
        }
        pcVar5 = (void *)*puVar4;
        if (puVar4[8] == 0 && puVar4[7] == 0) {
            *puVar4 = 0;
        }
        if (pcVar5 != (void *)0) {
            ((void (*)(...))(pcVar5))(puVar4[1]);
        }
        if (puVar4[8] != 0 || puVar4[7] != 0) {
            *puVar4 = (u32)pcVar5;
            FUN_0203ab60((int)puVar4, 0, 0);
        }
        if (*DAT_0203aa4c != 0) {
            FUN_0203ad18(*DAT_0203aa4c);
            return;
        }
        return;
    }
    FUN_0203ad18((int)puVar4);
}

/* 0x0203ab60 */
void FUN_0203ab60(int param_1, u32 param_2, u32 param_3)
{
    s64 lVar1;
    s64 lVar2;
    int *piVar3;
    int *piVar4;
    u32 uVar5;
    u32 uVar6;
    int iVar7;
    u32 uVar8;
    int iVar9;
    u32 uVar10;
    BOOL bVar11;
    u64 uVar12;
    s64 lVar13;

    lVar1 = ((s64)param_3 << 32) | param_2;
    if (*(int *)(param_1 + 0x20) != 0 || *(int *)(param_1 + 0x1c) != 0) {
        uVar12 = FUN_0203a754();
        uVar6 = (u32)(uVar12 >> 0x20);
        uVar5 = (u32)uVar12;
        uVar8 = *(u32 *)(param_1 + 0x28);
        uVar10 = *(u32 *)(param_1 + 0x24);
        lVar1 = *(s64 *)(param_1 + 0x24);
        bVar11 = uVar6 <= uVar8;
        if (uVar8 == uVar6) {
            bVar11 = uVar5 <= uVar10;
        }
        if (!bVar11) {
            lVar2 = *(s64 *)(param_1 + 0x1c);
            lVar13 = FUN_020462a8(uVar5 - uVar10, uVar6 - (uVar8 + (u32)(uVar5 < uVar10)),
                                  *(u32 *)(param_1 + 0x1c), *(u32 *)(param_1 + 0x20));
            lVar1 = (lVar13 + 1) * lVar2 + lVar1;
        }
    }
    iVar9 = (int)((u64)lVar1 >> 0x20);
    *(u32 *)(param_1 + 0xc) = (u32)lVar1;
    piVar3 = (int *)DAT_0203ac9c;
    *(int *)(param_1 + 0x10) = iVar9;
    piVar4 = (int *)DAT_0203ac9c;
    for (iVar7 = *piVar3; iVar7 != 0; iVar7 = *(int *)(iVar7 + 0x18)) {
        if ((int)(iVar9 - (*(int *)(iVar7 + 0x10) + (u32)((u32)lVar1 < *(u32 *)(iVar7 + 0xc)))) < 0)
        {
            *(u32 *)(param_1 + 0x14) = *(u32 *)(iVar7 + 0x14);
            *(int *)(iVar7 + 0x14) = param_1;
            *(int *)(param_1 + 0x18) = iVar7;
            if (*(int *)(param_1 + 0x14) == 0) {
                *(int *)DAT_0203ac9c = param_1;
                FUN_0203ad18(param_1);
                return;
            }
            *(int *)(*(int *)(param_1 + 0x14) + 0x18) = param_1;
            return;
        }
    }
    *(u32 *)(param_1 + 0x18) = 0;
    iVar7 = piVar4[1];
    piVar4[1] = param_1;
    *(int *)(param_1 + 0x14) = iVar7;
    if (iVar7 == 0) {
        piVar4[1] = param_1;
        *piVar4 = param_1;
        FUN_0203ad18(param_1);
        return;
    }
    *(int *)(iVar7 + 0x18) = param_1;
}

/* 0x0203ad18 */
void FUN_0203ad18(int param_1)
{
    u16 *puVar1;
    u32 uVar2;
    int iVar3;
    u32 uVar4;
    u64 uVar5;

    uVar5 = FUN_0203a754();
    *DAT_0203ada4 = 0;
    uVar4 = *(u32 *)(param_1 + 0xc) - (u32)uVar5;
    iVar3 = *(int *)(param_1 + 0x10) -
            ((int)(uVar5 >> 0x20) + (u32)(*(u32 *)(param_1 + 0xc) < (u32)uVar5));
    FUN_02038d34(1, DAT_0203ada8);
    puVar1 = DAT_0203ada4;
    uVar2 = DAT_0203adac;
    if ((-1 < iVar3) && (uVar2 = 0, iVar3 < (int)(u32)(uVar4 < 0x10000))) {
        uVar2 = ~uVar4 & 0xffff;
    }
    *DAT_0203adb0 = (s16)uVar2;
    *puVar1 = 0xc1;
    FUN_02038cc8(0x10);
}

/* 0x0203adb4 */
void FUN_0203adb4(void)
{
    u32 *puVar1;

    puVar1 = DAT_0203ae18;
    if (*DAT_0203ae14 == 0) {
        *DAT_0203ae14 = 1;
        *puVar1 = 0;
        puVar1[1] = 0;
        FUN_02038c80(4);
        puVar1 = DAT_0203ae20;
        *DAT_0203ae1c = 0;
        *puVar1 = 0;
    }
}

/* 0x0203ae24 — ASM-only: reads CPSR, needs handwritten ASM */
/* s64 FUN_0203ae24(void) */

/* 0x0203ae64 — ASM-only: reads/modifies CPSR with IRQ disable, needs handwritten ASM */
/* s64 FUN_0203ae64(void) */

/* 0x0203afbc */
void FUN_0203afbc(void)
{
    int iVar1;

    if (*DAT_0203b01c == 0) {
        *DAT_0203b01c = 1;
        thunk_FUN_0203d660();
        do {
            iVar1 = FUN_0203d5cc(0xc, 1);
        } while (iVar1 == 0);
        FUN_0203d5f4(0xc, DAT_0203b020);
    }
}

/* 0x0203b038 */
void FUN_0203b038(u8 *param_1)
{
    int iVar1;
    int iVar2;

    iVar1 = DAT_0203b0b0;
    iVar2 = DAT_0203b0b0 + 6;
    *param_1 = (u8)(((u32)*(u16 *)(DAT_0203b0b0 + 100) << 0x1d) >> 0x1d);
    param_1[1] = (u8)(((u32)*(u8 *)(iVar1 + 2) << 0x1c) >> 0x1c);
    param_1[2] = *(u8 *)(iVar1 + 3);
    param_1[3] = *(u8 *)(iVar1 + 4);
    *(u16 *)(param_1 + 0x18) = (u16)*(u8 *)(iVar1 + 0x1a);
    *(u16 *)(param_1 + 0x4e) = (u16)*(u8 *)(iVar1 + 0x50);
    FUN_0203b7c0(iVar2, param_1 + 4, 0x14);
    FUN_0203b7c0(DAT_0203b0b0 + 0x1c, param_1 + 0x1a, 0x34);
}

/* 0x0203b0b4 */
void FUN_0203b0b4(u32 param_1)
{
    (*(void (*)(...))DAT_0203b0c8)(DAT_0203b0cc, param_1, 6);
}

/* 0x0203b0d8 */
void FUN_0203b0d8(u32 param_1, u32 param_2)
{
    u32 *puVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;
    u32 uVar8;
    BOOL bVar9;

    uVar3 = FUN_0203ae38();
    iVar2 = DAT_0203b15c;
    puVar1 = DAT_0203b154;
    uVar8 = param_1 & *DAT_0203b154 & DAT_0203b158;
    while (TRUE) {
        iVar4 = FUN_0203b0d0(uVar8);
        uVar6 = 0x1f - iVar4;
        if ((int)uVar6 < 0) break;
        uVar5 = (u32)*(u16 *)(iVar2 + uVar6 * 2);
        uVar7 = ~(1 << (uVar6 & 0xff));
        bVar9 = param_2 == uVar5;
        if (bVar9) {
            uVar5 = *puVar1;
        }
        uVar8 = uVar8 & uVar7;
        if (bVar9) {
            *(u16 *)(iVar2 + uVar6 * 2) = 0;
            *puVar1 = uVar5 & uVar7;
        }
    }
    FUN_0203ae4c(uVar3);
}

/* 0x0203b1a4 */
void FUN_0203b1a4(int param_1, u32 param_2, int param_3, int param_4)
{
    u32 uVar1;

    if (param_1 != 0) {
        return;
    }
    uVar1 = param_2 & 0xff000000;
    if (param_4 == 0) {
        param_2 = param_2 + param_3;
    }
    else if (param_4 == 0x800000) {
        param_2 = param_2 - param_3;
    }
    if ((((uVar1 != 0x4000000) && (uVar1 < 0x8000000)) && ((param_2 & 0xff000000) != 0x4000000)) &&
       ((param_2 & 0xff000000) < 0x8000000)) {
        return;
    }
    FUN_0203aeb4();
}

/* 0x0203b214 */
void FUN_0203b214(int param_1, u32 param_2)
{
    u32 uVar1;
    u32 *puVar2;
    int iVar3;

    iVar3 = 0;
    puVar2 = (u32 *)DAT_0203b2c4;
    do {
        if (((((iVar3 != param_1) && ((*puVar2 & 0x80000000) != 0)) &&
             (uVar1 = *puVar2 & 0x38000000, uVar1 != param_2)) &&
            (((uVar1 != 0x8000000 || (param_2 != 0x10000000)) &&
             ((uVar1 != 0x10000000 || (param_2 != 0x8000000)))))) &&
           ((((uVar1 == 0x18000000 || (uVar1 == 0x20000000)) ||
             ((uVar1 == 0x28000000 ||
              (((uVar1 == 0x30000000 || (uVar1 == 0x38000000)) || (uVar1 == 0x8000000)))))) ||
            (uVar1 == 0x10000000)))) {
            FUN_0203aeb4();
        }
        iVar3 = iVar3 + 1;
        puVar2 = puVar2 + 3;
    } while (iVar3 < 3);
}

/* 0x0203b2c8 */
void FUN_0203b2c8(int param_1)
{
    int iVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;

    uVar4 = FUN_0203ae38();
    iVar1 = (param_1 * 6 + 5) * 2;
    *(u16 *)(&_DAT_040000b0 + iVar1) = *(u16 *)(&_DAT_040000b0 + iVar1) & 0xc5ff;
    *(u16 *)(&_DAT_040000b0 + iVar1) = *(u16 *)(&_DAT_040000b0 + iVar1) & 0x7fff;
    uVar3 = DAT_0203b348;
    uVar2 = DAT_0203b344;
    uVar5 = (u32)*(u16 *)(&_DAT_040000b0 + iVar1);
    uVar6 = (u32)*(u16 *)(&_DAT_040000b0 + iVar1);
    if (param_1 == 0) {
        _DAT_040000b0 = 0;
        *(u32 *)(DAT_0203b344 + 4) = 0;
        *(u32 *)(uVar2 + 8) = uVar3;
        uVar5 = uVar3;
        uVar6 = uVar2;
    }
    FUN_0203ae4c(uVar4, uVar5, uVar6);
}

/* 0x0203b34c */
void FUN_0203b34c(int param_1)
{
    int iVar1;
    u32 uVar2;

    FUN_0203ae38();
    uVar2 = DAT_0203b3b8;
    iVar1 = DAT_0203b3b4;
    do {
    } while ((*(u32 *)(DAT_0203b3b4 + (param_1 * 3 + 2) * 4) & 0x80000000) != 0);
    if (param_1 == 0) {
        _DAT_040000b0 = 0;
        *(u32 *)(DAT_0203b3b4 + 4) = 0;
        *(u32 *)(iVar1 + 8) = uVar2;
    }
    FUN_0203ae4c();
}

/* 0x0203b3bc */
void FUN_0203b3bc(u32 param_1, u32 param_2, u32 param_3, u32 param_4, void *param_5, u32 param_6)
{
    FUN_0203b1a4(param_1, param_2, param_4, 0);
    if (param_4 != 0) {
        FUN_0203b34c(param_1);
        if (param_5 == (void *)0) {
            func_0x01ff85cc(param_1, param_2, param_3, param_4 >> 2 | 0x84000000);
            return;
        }
        FUN_02038d80(param_1, param_5, param_6);
        func_0x01ff85cc(param_1, param_2, param_3, param_4 >> 2 | 0xc4000000);
        return;
    }
    if (param_5 != (void *)0) {
        ((void (*)(...))(param_5))(param_6);
    }
}

/* 0x0203b464 */
void FUN_0203b464(int param_1, u32 param_2, u32 param_3, u32 param_4, void *param_5, u32 param_6)
{
    u32 uVar1;
    int iVar2;

    if (param_4 != 0) {
        FUN_0203b34c(param_1);
        if (param_5 == (void *)0) {
            uVar1 = FUN_0203ae38();
            iVar2 = DAT_0203b538 + param_1 * 4;
            *(u32 *)(param_1 * 4 + 0x40000e0) = param_3;
            func_0x01ff8520(param_1, iVar2, param_2, param_4 >> 2 | 0x85000000);
            FUN_0203ae4c(uVar1);
            return;
        }
        FUN_02038d80(param_1, param_5, param_6);
        uVar1 = FUN_0203ae38();
        iVar2 = DAT_0203b538;
        *(u32 *)(param_1 * 4 + 0x40000e0) = param_3;
        func_0x01ff8520(param_1, iVar2 + param_1 * 4, param_2, param_4 >> 2 | 0xc5000000);
        FUN_0203ae4c(uVar1);
        return;
    }
    if (param_5 != (void *)0) {
        ((void (*)(...))(param_5))(param_6);
    }
}

/* 0x0203b53c */
void FUN_0203b53c(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 *puVar1;

    if (param_4 != 0) {
        FUN_0203b1a4(param_1, param_2, param_4, 0);
        puVar1 = (u32 *)(DAT_0203b5b0 + (param_1 * 3 + 2) * 4);
        do {
        } while ((*puVar1 & 0x80000000) != 0);
        func_0x01ff8558(param_1, param_2, param_3, param_4 >> 1 | 0x80000000);
        do {
        } while ((*puVar1 & 0x80000000) != 0);
    }
}

/* 0x0203b630 */
void FUN_0203b630(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 uVar1;
    int iVar2;
    u32 *puVar3;

    if (param_4 != 0) {
        puVar3 = (u32 *)(DAT_0203b6b8 + (param_1 * 3 + 2) * 4);
        do {
        } while ((*puVar3 & 0x80000000) != 0);
        uVar1 = FUN_0203ae38();
        iVar2 = DAT_0203b6bc + param_1 * 4;
        *(u32 *)(param_1 * 4 + 0x40000e0) = param_3;
        func_0x01ff84c0(param_1, iVar2, param_2, param_4 >> 2 | 0x85000000);
        FUN_0203ae4c(uVar1);
        do {
        } while ((*puVar3 & 0x80000000) != 0);
    }
}

/* 0x0203b6c0 */
void FUN_0203b6c0(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0203b214(param_1, 0x10000000);
    FUN_0203b1a4(param_1, param_2, param_4, 0);
    if (param_4 != 0) {
        FUN_0203b34c(param_1);
        func_0x01ff85cc(param_1, param_2, param_3, DAT_0203b730 | param_4 >> 1);
    }
}

/* 0x0203b734 */
void FUN_0203b734(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0203b214(param_1, 0x10000000);
    FUN_0203b1a4(param_1, param_2, param_4, 0);
    if (param_4 != 0) {
        FUN_0203b34c(param_1);
        func_0x01ff85cc(param_1, param_2, param_3, DAT_0203b7a4 | param_4 >> 2);
    }
}

/* 0x0203bae0 */
void FUN_0203bae0(u32 *param_1, u16 *param_2)
{
    BOOL bVar1;
    int iVar2;
    u32 *puVar3;
    u32 *puVar4;
    u16 *puVar5;
    u32 uVar6;
    u32 uVar7;
    u32 uVar8;
    int iVar9;
    int iVar10;
    u8 bVar11;
    int iVar12;
    u32 uVar13;
    u32 uVar14;

    uVar7 = 0;
    uVar13 = *param_1 >> 8;
    uVar6 = 0;
    puVar4 = param_1 + 1;
    do {
        if ((int)uVar13 < 1) {
            return;
        }
        bVar11 = (u8)*puVar4;
        puVar3 = (u32 *)((int)puVar4 + 1);
        puVar5 = param_2;
        iVar12 = 8;
        while (puVar4 = puVar3, param_2 = puVar5, 0 < iVar12) {
            if ((bVar11 & 0x80) == 0) {
                puVar4 = (u32 *)((int)puVar3 + 1);
                uVar7 = uVar7 | (u32)(u8)*(u8 *)puVar3 << uVar6;
                iVar2 = -1;
                uVar6 = uVar6 ^ 8;
                if (uVar6 == 0) {
                    param_2 = puVar5 + 1;
                    *puVar5 = (u16)uVar7;
                    uVar7 = 0;
                }
            }
            else {
                iVar9 = ((int)(u32)(u8)*(u8 *)puVar3 >> 4) + 3;
                puVar4 = (u32 *)((int)puVar3 + 2);
                uVar8 = ((u32)*(u8 *)((int)puVar3 + 1) | ((u8)*(u8 *)puVar3 & 0xf) << 8) + 1;
                uVar14 = 8 - uVar6 ^ (uVar8 & 1) << 3;
                iVar2 = -iVar9;
                do {
                    uVar14 = uVar14 ^ 8;
                    uVar7 = uVar7 | ((int)((u32)*(u16 *)
                                            ((int)puVar5 - (uVar8 + (8 - uVar6 >> 3) & 0xfffffffe)) &
                                    0xff << (uVar14 & 0xff)) >> (uVar14 & 0xff)) << uVar6;
                    uVar6 = uVar6 ^ 8;
                    if (uVar6 == 0) {
                        *puVar5 = (u16)uVar7;
                        uVar7 = 0;
                        puVar5 = puVar5 + 1;
                    }
                    iVar10 = iVar9 + -1;
                    bVar1 = 0 < iVar9;
                    param_2 = puVar5;
                    iVar9 = iVar10;
                } while (iVar10 != 0 && bVar1);
            }
            uVar13 = uVar13 + iVar2;
            if ((int)uVar13 < 1) break;
            bVar11 = bVar11 << 1;
            puVar3 = puVar4;
            puVar5 = param_2;
            iVar12 = iVar12 + -1;
        }
    } while (TRUE);
}

/* 0x0203bbb8 */
void FUN_0203bbb8(u32 *param_1, u32 *param_2)
{
    u8 bVar1;
    u32 uVar2;
    u8 *pbVar3;
    u8 *pbVar4;
    u32 *puVar5;
    u8 *pbVar6;
    u32 uVar7;
    u32 uVar8;
    int iVar9;
    u8 *pbVar10;
    int iVar11;
    u32 uVar12;
    int iVar13;

    pbVar10 = (u8 *)((int)param_1 + 5);
    uVar2 = *param_1;
    uVar8 = (u8)uVar2 & 0xf;
    uVar7 = 0;
    iVar13 = 0;
    uVar12 = *param_1 >> 8;
    pbVar3 = (u8 *)((int)(param_1 + 1) + ((u8)param_1[1] + 1) * 2);
    pbVar6 = pbVar10;
    do {
        if ((int)uVar12 < 1) {
            return;
        }
        pbVar4 = pbVar3 + 4;
        iVar9 = *(int *)pbVar3;
        puVar5 = param_2;
        iVar11 = 0x20;
        while (pbVar3 = pbVar4, param_2 = puVar5, 0 < iVar11) {
            bVar1 = *pbVar6;
            pbVar6 = (u8 *)(((u32)(int)pbVar6 & 0xfffffffe) + (*pbVar6 & 0x3f) * 2 + 2 - (u32)(iVar9 >> 0x1f));
            if (((u32)bVar1 << -(iVar9 >> 0x1f) & 0x80) != 0) {
                uVar7 = uVar7 >> uVar8 | (u32)*pbVar6 << (0x20 - uVar8 & 0xff);
                iVar13 = iVar13 + 1;
                pbVar6 = pbVar10;
                if (iVar13 == ((u8)uVar2 & 7) + 4) {
                    param_2 = puVar5 + 1;
                    *puVar5 = uVar7;
                    uVar12 = uVar12 - 4;
                    iVar13 = 0;
                }
            }
            if ((int)uVar12 < 1) break;
            iVar9 = iVar9 << 1;
            puVar5 = param_2;
            iVar11 = iVar11 + -1;
        }
    } while (TRUE);
}

/* 0x0203bcf8 */
void FUN_0203bcf8(int param_1, u32 param_2, u32 param_3, int param_4)
{
    FUN_0203b214(param_1, 0xffffffff);
    FUN_0203b1a4(param_1, param_2, param_4, 0x1000000);
    if (param_4 != 0) {
        do {
        } while ((*(u32 *)(DAT_0203bd7c + (param_1 * 3 + 2) * 4) & 0x80000000) != 0);
        func_0x01ff85cc(param_1, param_2, param_3, DAT_0203bd80);
    }
}

/* 0x0203c348 */
BOOL FUN_0203c348(void)
{
    int *piVar1;
    int iVar2;

    iVar2 = FUN_02039be8();
    if (iVar2 != 0) {
        FUN_0203ae38(iVar2);
        piVar1 = DAT_0203c38c;
        *DAT_0203c38c = 0x10;
        iVar2 = *piVar1;
        FUN_0203ae4c();
        return iVar2 != 0;
    }
    return TRUE;
}

/* 0x0203c390 */
int *FUN_0203c390(void)
{
    int iVar1;
    int *piVar2;

    FUN_0203ae38();
    piVar2 = (int *)*DAT_0203c3e0;
    if (piVar2 != (int *)0) {
        iVar1 = *piVar2;
        *DAT_0203c3e0 = iVar1;
        if (iVar1 == 0) {
            *DAT_0203c3e4 = 0;
        }
        FUN_0203ae4c();
        return piVar2;
    }
    FUN_0203ae4c();
    return (int *)0;
}

/* 0x0203c3e8 */
void FUN_0203c3e8(void)
{
    int iVar1;

    do {
        iVar1 = FUN_0203d518(7, 0, 0);
    } while (iVar1 < 0);
}

/* 0x0203c41c */
void FUN_0203c41c(void)
{
    int iVar1;

    FUN_0203d5f4(7, DAT_0203c484);
    iVar1 = FUN_0203c348();
    if (iVar1 == 0) {
        return;
    }
    iVar1 = FUN_0203d5cc(7, 1);
    if (iVar1 != 0) {
        /* UNRECOVERED_JUMPTABLE — indirect branch */
        return;
    }
    do {
        FUN_0203aea8(100);
        iVar1 = FUN_0203d5cc(7, 1);
    } while (iVar1 == 0);
}

/* 0x0203c488 */
void FUN_0203c488(u32 param_1, u32 param_2)
{
    u32 uVar1;

    uVar1 = FUN_0203ae38();
    FUN_0203cb6c(param_2);
    FUN_0203ae4c(uVar1);
}

/* 0x0203c4d8 */
int FUN_0203c4d8(void)
{
    int *piVar1;
    int iVar2;

    FUN_0203ae38();
    iVar2 = 0;
    for (piVar1 = (int *)*DAT_0203c514; piVar1 != (int *)0; piVar1 = (int *)*piVar1) {
        iVar2 = iVar2 + 1;
    }
    FUN_0203ae4c();
    return iVar2;
}

/* 0x0203c518 */
int FUN_0203c518(void)
{
    int *piVar1;
    int iVar2;

    FUN_0203ae38();
    iVar2 = 0;
    for (piVar1 = (int *)*DAT_0203c554; piVar1 != (int *)0; piVar1 = (int *)*piVar1) {
        iVar2 = iVar2 + 1;
    }
    FUN_0203ae4c();
    return iVar2;
}

/* 0x0203c83c */
void FUN_0203c83c(u32 *param_1)
{
    u32 **puVar1;
    u32 *puVar2;

    FUN_0203ae38();
    puVar2 = DAT_0203c880;
    puVar1 = DAT_0203c87c;
    if (*DAT_0203c87c == (u32 *)0) {
        *DAT_0203c87c = param_1;
        *puVar2 = (u32)param_1;
    }
    else {
        **DAT_0203c87c = (u32)param_1;
        *puVar1 = param_1;
    }
    *param_1 = 0;
    FUN_0203ae4c();
}

/* 0x0203c884 */
int FUN_0203c884(u32 param_1)
{
    int iVar1;

    iVar1 = FUN_0203c348();
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = (int)FUN_0203c390();
    if (iVar1 != 0) {
        /* UNRECOVERED_JUMPTABLE — indirect branch */
        return iVar1;
    }
    if ((param_1 & 1) == 0) {
        return 0;
    }
    iVar1 = FUN_0203c4b8(param_1 & 1);
    if (iVar1 < 1) {
        FUN_0203c67c(1);
    }
    else {
        do {
            iVar1 = FUN_0203c920(0);
        } while (iVar1 != 0);
        iVar1 = (int)FUN_0203c390();
        if (iVar1 != 0) {
            return iVar1;
        }
    }
    FUN_0203c3e8();
    do {
        FUN_0203c920(1);
        iVar1 = (int)FUN_0203c390();
    } while (iVar1 == 0);
    return iVar1;
}

/* 0x0203ca54 */
void FUN_0203ca54(void)
{
    int *piVar1;
    int iVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 *puVar5;
    u32 *puVar6;
    u32 *puVar7;
    u32 *puVar8;
    u32 *puVar9;
    u32 uVar10;
    u32 *puVar11;
    int *piVar12;
    int iVar13;

    FUN_0203c41c();
    piVar1 = (int *)DAT_0203cb34;
    iVar13 = 0;
    *DAT_0203cb38 = (u32)DAT_0203cb34;
    piVar12 = piVar1;
    do {
        iVar13 = iVar13 + 1;
        *piVar12 = (int)(piVar1 + iVar13 * 6);
        puVar11 = DAT_0203cb68;
        uVar10 = DAT_0203cb64;
        puVar9 = DAT_0203cb60;
        puVar8 = DAT_0203cb54;
        puVar7 = DAT_0203cb50;
        puVar6 = DAT_0203cb4c;
        puVar5 = DAT_0203cb48;
        puVar4 = DAT_0203cb44;
        puVar3 = DAT_0203cb40;
        iVar2 = DAT_0203cb3c;
        piVar12 = piVar12 + 6;
    } while (iVar13 < 0xff);
    *DAT_0203cb5c = DAT_0203cb58;
    *(u32 *)(iVar2 + 0x7e8) = 0;
    *puVar3 = 0;
    *puVar4 = 0;
    *puVar5 = 0;
    *puVar6 = 0;
    *puVar7 = 0;
    *puVar9 = 1;
    *puVar8 = 0;
    *puVar11 = uVar10;
    FUN_0203cc4c();
    iVar13 = FUN_0203c884(1);
    if (iVar13 != 0) {
        *(u32 *)(iVar13 + 4) = 0x1d;
        *(u32 *)(iVar13 + 8) = *puVar11;
        FUN_0203c83c((u32 *)iVar13);
        FUN_0203c67c(1);
    }
}
