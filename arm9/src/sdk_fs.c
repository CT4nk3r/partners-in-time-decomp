/**
 * SDK Filesystem — file I/O, archive management, card access (0x0203cxxx-0x0203fxxx)
 * Decompiled from 0x0203cb6c-0x0203ff78 region (56 functions)
 * Note: FUN_0203d3e8 has no Ghidra pseudocode available (55 implemented)
 */
#include "types.h"

/* ------------------------------------------------------------------ */
/*  External function declarations                                     */
/* ------------------------------------------------------------------ */
extern u32  FUN_0203ae38(...);
extern void FUN_0203ae4c(...);
extern void FUN_02039548(...);
extern void FUN_020394d4(...);
extern void FUN_0203a04c(...);
extern void FUN_0203a014(...);
extern void FUN_0203b914(...);
extern void FUN_0203b9a8(...);
extern void FUN_02038c4c(...);
extern void FUN_02038dcc(...);
extern void FUN_02038cc8(...);
extern u32  FUN_02038e7c(...);
extern int  FUN_0203edcc(...);
extern void FUN_0203ef08(...);
extern int  FUN_0203efa0(...);
extern void FUN_0203f2a0(...);
extern int  FUN_0203fb40(...);
extern int  FUN_0203f740(...);
extern void FUN_0203f864(...);
extern void FUN_0203fd64(...);
extern void FUN_0203fd8c(...);
extern int  FUN_0203fdd0(...);
extern void FUN_020400c4(...);
extern void FUN_0204056c(...);
extern void FUN_020403c4(...);
extern void FUN_020404b0(...);
extern void FUN_020425e0(...);
extern void FUN_02042600(...);
extern void FUN_020432a8(...);
extern void FUN_0204333c(...);
extern void FUN_0203e328(...);
extern void FUN_0203c0ec(...);

/* ------------------------------------------------------------------ */
/*  External data references (DAT_ labels from Ghidra)                 */
/* ------------------------------------------------------------------ */
extern u32  DAT_0203cbc8;
extern u32  DAT_0203cbf8;
extern u32  DAT_0203cc48;
extern u32 *DAT_0203ccfc;
extern u16 *DAT_0203d50c;
extern int  DAT_0203d510;
extern u32 *DAT_0203d514;
extern u32  _DAT_04100000;
extern u16 *DAT_0203d5c4;
extern u32 *DAT_0203d5c8;
extern u32  DAT_0203d5f0;
extern u32  DAT_0203d658;
extern u32  DAT_0203d65c;
extern int *DAT_0203d750;
extern int  DAT_0203d754;
extern int  DAT_0203d758;
extern u32  DAT_0203d75c;
extern s16 *DAT_0203d760;
extern u32  DAT_0203d764;
extern u16 *DAT_0203d768;
extern u32  DAT_0203dbd4;
extern u32  DAT_0203df00;
extern u32  DAT_0203df98;
extern void (*DAT_0203e0a0)(...);
extern u32  DAT_0203e2e0;
extern u32  DAT_0203e59c;
extern u32  DAT_0203e610;
extern u32  DAT_0203e614;
extern int *DAT_0203e6d0;
extern int *DAT_0203e6d4;
extern int *DAT_0203ec80;
extern int *DAT_0203f29c;
extern u32 *DAT_0203f550;
extern u32 *DAT_0203f554;
extern u32 *DAT_0203f558;
extern u32 *DAT_0203f55c;
extern u32  DAT_0203f560;
extern u32  DAT_0203f564;
extern int *DAT_0203f568;
extern u32  DAT_0203f56c;
extern u32  DAT_0203f578;
extern u32  DAT_0203f57c;
extern u32  DAT_0203f580;
extern int *DAT_0203f584;
extern u32  DAT_0203f628;
extern u32  DAT_0203f67c;
extern u32  DAT_0203f678;
extern u32 *DAT_0203fa00;
extern int *DAT_0203fa04;

/* ------------------------------------------------------------------ */
/*  Forward declarations for functions in this file                    */
/* ------------------------------------------------------------------ */
void FUN_0203cb6c(u32 param_1);
u8   FUN_0203cbcc(int param_1, u32 param_2, u32 param_3);
void FUN_0203cc1c(void);
void FUN_0203cc4c(u32 *param_1);
u32  FUN_0203cccc(void);
void FUN_0203d3f4(void);
u32  FUN_0203d518(u32 param_1, int param_2, u32 param_3);
BOOL FUN_0203d5cc(u32 param_1, int param_2);
void FUN_0203d5f4(u32 param_1, int param_2);
void FUN_0203d660(void);
u32  FUN_0203d79c(int param_1);
u32  FUN_0203d818(int param_1);
u32  FUN_0203dbd8(int param_1);
u32  FUN_0203ddf4(int param_1);
u32  FUN_0203df04(int param_1);
void FUN_0203df9c(int param_1);
void FUN_0203dfd4(int param_1);
int  FUN_0203e00c(int param_1, int param_2, int param_3);
void FUN_0203e070(int param_1, u16 param_2);
void FUN_0203e0a4(int *param_1, void *param_2, int param_3);
int  FUN_0203e154(int param_1, u32 param_2);
void FUN_0203e2e4(int param_1, u32 param_2);
BOOL FUN_0203e348(int param_1);
BOOL FUN_0203e3c4(int param_1);
u32  FUN_0203e474(int param_1, int param_2, u32 param_3);
u32  FUN_0203e5a0(int param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6, int param_7, int param_8);
u32  FUN_0203e618(u32 *param_1, u32 param_2, u32 param_3);
void FUN_0203e6d8(int param_1);
void FUN_0203e700(int param_1, u32 param_2);
u32  FUN_0203e790(int param_1, u32 param_2);
BOOL FUN_0203e920(int param_1);
void FUN_0203e968(int param_1);
int  FUN_0203ea0c(int param_1);
int *FUN_0203ec34(int param_1, int param_2);
u32  FUN_0203eca8(int param_1, u32 param_2, int param_3, u32 param_4);
u32  FUN_0203ecd4(int param_1, u32 param_2, int param_3, u32 param_4);
u32  FUN_0203ecfc(int param_1, int param_2);
void FUN_0203edec(int param_1);
u32  FUN_0203ee38(int param_1);
u32  FUN_0203f01c(int param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
BOOL FUN_0203f074(u32 param_1, u32 param_2);
int  FUN_0203f0b8(int param_1, u32 param_2, int param_3, int param_4);
int  FUN_0203f140(int param_1, char *param_2, u32 param_3, int param_4);
void FUN_0203f304(int *param_1, int param_2);
void FUN_0203f34c(int *param_1);
void FUN_0203f3ec(u32 param_1);
u32  FUN_0203f59c(u32 param_1, int param_2);
u32  FUN_0203f634(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32  FUN_0203f690(u32 param_1, u32 param_2);
u32  FUN_0203f6e4(u32 param_1, u32 param_2);
BOOL FUN_0203f964(int param_1, u32 param_2, u32 param_3);
u32  FUN_0203fa08(int param_1);
u32  FUN_0203fc54(int param_1, int param_2, int param_3, u32 param_4, int param_5, u32 param_6, int param_7, u32 param_8);
void FUN_0203fdec(int param_1, int param_2, int param_3, u8 *param_4, int param_5);
void FUN_0203ff78(int param_1);

/* ================================================================== */
/*  Function implementations (address order)                           */
/* ================================================================== */

// FUN_0203cb6c @ 0x0203cb6c (92 bytes) — dispatch callback by index
void FUN_0203cb6c(u32 param_1)
{
    u32 *puVar1;

    puVar1 = (u32 *)((param_1 & 0xff) * 0xc + DAT_0203cbc8);
    if (((int)param_1 >> 8 & 0xffU) != (u32)*(u8 *)(puVar1 + 2)) {
        return;
    }
    if ((void (*)(...))*puVar1 != (void (*)(...))(void *)0) {
        (*(void (*)(...))*puVar1)(puVar1[1]);
        return;
    }
    return;
}

// FUN_0203cbcc @ 0x0203cbcc (44 bytes) — register callback entry
u8 FUN_0203cbcc(int param_1, u32 param_2, u32 param_3)
{
    int iVar1;

    iVar1 = DAT_0203cbf8;
    *(u32 *)(DAT_0203cbf8 + param_1 * 0xc) = param_2;
    iVar1 = iVar1 + param_1 * 0xc;
    *(u32 *)(iVar1 + 4) = param_3;
    *(char *)(iVar1 + 8) = *(char *)(iVar1 + 8) + '\x01';
    return *(u8 *)(iVar1 + 8);
}

// FUN_0203cc1c @ 0x0203cc1c (44 bytes) — clear callback table
void FUN_0203cc1c(void)
{
    u32 *puVar1;
    int iVar2;

    iVar2 = 0;
    puVar1 = (u32 *)DAT_0203cc48;
    do {
        *puVar1 = 0;
        puVar1[1] = 0;
        iVar2 = iVar2 + 1;
        *(u8 *)(puVar1 + 2) = 0;
        puVar1 = puVar1 + 3;
    } while (iVar2 < 8);
    return;
}

// FUN_0203cc4c @ 0x0203cc4c (128 bytes) — initialize directory table
void FUN_0203cc4c(u32 *param_1)
{
    int iVar1;
    int iVar2;
    u32 *puVar3;
    int iVar4;

    iVar2 = 0;
    param_1[1] = 0;
    *(u16 *)(param_1 + 2) = 0;
    *(u16 *)((int)param_1 + 10) = 0;
    *param_1 = 0;
    puVar3 = param_1;
    do {
        iVar4 = 0;
        puVar3[0x10] = 0;
        do {
            iVar1 = iVar4 * 2;
            iVar4 = iVar4 + 1;
            *(u16 *)((int)puVar3 + iVar1 + 0x20) = 0xffff;
        } while (iVar4 < 0x10);
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 9;
    } while (iVar2 < 0x10);
    iVar2 = 0;
    do {
        iVar4 = iVar2 * 2;
        iVar2 = iVar2 + 1;
        *(u16 *)((int)param_1 + iVar4 + 0x260) = 0xffff;
    } while (iVar2 < 0x10);
    FUN_0203a04c(param_1, 0x280);
    return;
}

// FUN_0203cccc @ 0x0203cccc (48 bytes) — flush and return first field
u32 FUN_0203cccc(void)
{
    FUN_0203a014(*DAT_0203ccfc, 0x280);
    return *(u32 *)*DAT_0203ccfc;
}

// FUN_0203d3f4 @ 0x0203d3f4 (268 bytes) — card command dispatch loop
void FUN_0203d3f4(void)
{
    u32 uVar1;
    u16 *puVar2;
    int iVar3;
    u32 *puVar4;
    int iVar5;
    void (*pcVar6)(...);
    u32 local_28;

    puVar4 = DAT_0203d514;
    iVar3 = DAT_0203d510;
    puVar2 = DAT_0203d50c;
    while (TRUE) {
        if ((*puVar2 & 0x4000) == 0) {
            FUN_0203ae38();
            if ((*puVar2 & 0x100) == 0) {
                local_28 = _DAT_04100000;
                FUN_0203ae4c();
                iVar5 = 0;
            }
            else {
                FUN_0203ae4c();
                iVar5 = -4;
            }
        }
        else {
            *puVar2 = *puVar2 | 0xc000;
            iVar5 = -3;
        }
        if (iVar5 == -4) break;
        if (iVar5 != -3) {
            uVar1 = local_28 & 0x1f;
            if (uVar1 != 0) {
                pcVar6 = *(void (**)(...))((int)iVar3 + uVar1 * 4);
                if (pcVar6 == (void (*)(...))(void *)0) {
                    if (-1 < (int)(local_28 << 0x1a)) {
                        local_28 = local_28 | 0x20;
                        if ((*puVar2 & 0x4000) == 0) {
                            FUN_0203ae38();
                            if ((*puVar2 & 2) == 0) {
                                *puVar4 = local_28;
                                FUN_0203ae4c();
                            }
                            else {
                                FUN_0203ae4c();
                            }
                        }
                        else {
                            *puVar2 = *puVar2 | 0xc000;
                        }
                    }
                }
                else {
                    (*pcVar6)(uVar1, local_28 >> 6, (local_28 << 0x1a) >> 0x1f);
                }
            }
        }
    }
    return;
}

// FUN_0203d518 @ 0x0203d518 (172 bytes) — send card command
u32 FUN_0203d518(u32 param_1, int param_2, u32 param_3)
{
    if ((*DAT_0203d5c4 & 0x4000) != 0) {
        *DAT_0203d5c4 = *DAT_0203d5c4 | 0xc000;
        return 0xffffffff;
    }
    FUN_0203ae38(*DAT_0203d5c4 & 0x4000);
    if ((*DAT_0203d5c4 & 2) == 0) {
        *DAT_0203d5c8 = param_1 & 0x1f | (param_3 & 1) << 5 | param_2 << 6;
        FUN_0203ae4c();
        return 0;
    }
    FUN_0203ae4c();
    return 0xfffffffe;
}

// FUN_0203d5cc @ 0x0203d5cc (36 bytes) — check if interrupt bit is set
BOOL FUN_0203d5cc(u32 param_1, int param_2)
{
    return (1 << (param_1 & 0xff) & *(u32 *)(DAT_0203d5f0 + param_2 * 4 + 0x388)) != 0;
}

// FUN_0203d5f4 @ 0x0203d5f4 (100 bytes) — set/clear interrupt handler
void FUN_0203d5f4(u32 param_1, int param_2)
{
    FUN_0203ae38();
    *(int *)(DAT_0203d658 + param_1 * 4) = param_2;
    if (param_2 == 0) {
        *(u32 *)(DAT_0203d65c + 0x388) = *(u32 *)(DAT_0203d65c + 0x388) & ~(1 << (param_1 & 0xff));
    }
    else {
        *(u32 *)(DAT_0203d65c + 0x388) = *(u32 *)(DAT_0203d65c + 0x388) | 1 << (param_1 & 0xff);
    }
    FUN_0203ae4c();
    return;
}

// FUN_0203d660 @ 0x0203d660 (240 bytes) — initialize card subsystem
void FUN_0203d660(void)
{
    u16 uVar1;
    u16 *puVar2;
    u32 uVar3;
    int iVar4;
    int iVar5;
    u16 uVar6;

    uVar3 = FUN_0203ae38();
    iVar5 = DAT_0203d754;
    if (*DAT_0203d750 == 0) {
        *DAT_0203d750 = 1;
        iVar4 = 0;
        *(u32 *)(iVar5 + 0x388) = 0;
        iVar5 = DAT_0203d758;
        do {
            *(u32 *)(iVar5 + iVar4 * 4) = 0;
            iVar4 = iVar4 + 1;
        } while (iVar4 < 0x20);
        *DAT_0203d760 = (s16)DAT_0203d75c;
        FUN_02038c4c(0x40000);
        FUN_02038dcc(0x40000, DAT_0203d764);
        FUN_02038cc8(0x40000);
        puVar2 = DAT_0203d768;
        iVar5 = 0;
        while (TRUE) {
            uVar1 = *puVar2;
            uVar6 = uVar1 & 0xf;
            *puVar2 = uVar6 << 8;
            if (((uVar1 & 0xf) == 0) && (4 < iVar5)) break;
            iVar4 = 1000;
            uVar1 = *puVar2;
            while ((uVar1 & 0xf) == uVar6) {
                if (iVar4 < 1) {
                    iVar5 = 0;
                    break;
                }
                iVar4 = iVar4 + -1;
                uVar1 = *puVar2;
            }
            iVar5 = iVar5 + 1;
        }
    }
    FUN_0203ae4c(uVar3);
    return;
}

// FUN_0203d79c @ 0x0203d79c (124 bytes) — read FAT entry and dispatch
u32 FUN_0203d79c(int param_1)
{
    u32 uVar1;
    u32 uVar2;
    int iVar3;
    u32 uStack_20;
    u32 uStack_1c;
    int iStack_18;
    int iStack_14;

    iStack_18 = *(int *)(param_1 + 8);
    iVar3 = *(int *)(param_1 + 0x30);
    uVar1 = iVar3 * 8;
    if (uVar1 < *(u32 *)(iStack_18 + 0x24)) {
        iStack_14 = *(int *)(iStack_18 + 0x20) + uVar1;
        FUN_0203e0a4(&iStack_18, &uStack_20, 8);
        *(u32 *)(param_1 + 0x2c) = uStack_20;
        *(u32 *)(param_1 + 0x30) = uStack_1c;
        *(int *)(param_1 + 0x34) = iVar3;
        uVar2 = FUN_0203e154(param_1, 7);
        return uVar2;
    }
    return 1;
}

// FUN_0203d818 @ 0x0203d818 (956 bytes) — resolve file path to entry
u32 FUN_0203d818(int param_1)
{
    int iVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;
    u32 *puVar5;
    u32 uVar6;
    u32 uVar7;
    s16 sVar8;
    u32 uVar9;
    int iVar10;
    u32 uVar11;
    u8 auStack_fc[4];
    u32 local_f8;
    int local_f0;
    int iStack_ec;
    u8 auStack_e8[128];
    u8 auStack_68[8];
    u32 local_60;
    u16 local_48;
    u32 local_40;
    u8 *local_3c;
    u32 local_38;

    puVar5 = *(u32 **)(param_1 + 8);
    FUN_0203f2a0(auStack_68);
    local_60 = *(u32 *)(param_1 + 8);
    if ((*(u32 *)(param_1 + 0xc) & 0x20) == 0) {
        uVar6 = *(u32 *)(param_1 + 0x1c);
        if (*(s16 *)(param_1 + 0x34) == 0) {
            uVar11 = 0;
            uVar9 = 0;
            uVar7 = 0x10000;
            do {
                FUN_0203e070((int)auStack_68, uVar11);
                local_3c = auStack_fc;
                if (uVar11 == 0) {
                    uVar9 = local_40;
                }
                local_38 = 1;
                iVar2 = FUN_0203e154((int)auStack_68, 3);
                while (iVar2 == 0) {
                    if ((local_f0 == 0) && (local_f8 == uVar6)) {
                        uVar7 = (u32)local_48;
                        break;
                    }
                    iVar2 = FUN_0203e154((int)auStack_68, 3);
                }
            } while ((uVar7 == 0x10000) && (uVar11 = uVar11 + 1, uVar11 < uVar9));
        }
        else {
            uVar7 = (u32)*(u16 *)(param_1 + 0x36);
        }
    }
    else {
        uVar7 = (u32)*(u16 *)(param_1 + 0x20);
        uVar6 = 0x10000;
    }
    if (uVar7 == 0x10000) {
        *(u16 *)(param_1 + 0x34) = 0;
        return 1;
    }
    if (*(s16 *)(param_1 + 0x34) == 0) {
        if (*puVar5 < 0x100) {
            iVar2 = 1;
        }
        else if (*puVar5 < 0xff01) {
            iVar2 = 2;
        }
        else {
            iVar2 = 3;
        }
        iVar10 = iVar2 + 2;
        if (uVar6 != 0x10000) {
            iVar10 = iVar2 + 2 + iStack_ec;
        }
        sVar8 = (s16)iVar10;
        if (uVar7 != 0) {
            FUN_0203e070((int)auStack_68, uVar7);
            uVar9 = uVar7;
            do {
                FUN_0203e070((int)auStack_68, local_40);
                local_3c = auStack_fc;
                local_38 = 1;
                iVar2 = FUN_0203e154((int)auStack_68, 3);
                while (iVar2 == 0) {
                    if ((local_f0 != 0) && ((local_f8 & 0xffff) == uVar9)) {
                        iVar10 = iVar10 + iStack_ec + 1;
                        break;
                    }
                    iVar2 = FUN_0203e154((int)auStack_68, 3);
                }
                sVar8 = (s16)iVar10;
                uVar9 = (u32)local_48;
            } while (uVar9 != 0);
        }
        *(s16 *)(param_1 + 0x34) = sVar8 + 1;
        *(s16 *)(param_1 + 0x36) = (s16)uVar7;
    }
    iVar2 = *(int *)(param_1 + 0x2c);
    if (iVar2 == 0) {
        return 0;
    }
    uVar9 = (u32)*(u16 *)(param_1 + 0x34);
    if (*(u32 *)(param_1 + 0x30) < uVar9) {
        return 1;
    }
    if (*puVar5 < 0x100) {
        iVar10 = 1;
    }
    else if (*puVar5 < 0xff01) {
        iVar10 = 2;
    }
    else {
        iVar10 = 3;
    }
    FUN_0203b9a8(puVar5, iVar2, iVar10);
    FUN_0203b9a8(DAT_0203dbd4, iVar2 + iVar10, 2);
    FUN_0203e070((int)auStack_68, uVar7);
    if (uVar6 == 0x10000) {
        *(u8 *)(iVar2 + uVar9 + -1) = 0;
        iVar10 = -1;
    }
    else {
        local_3c = auStack_fc;
        local_38 = 0;
        iVar10 = FUN_0203e154((int)auStack_68, 3);
        if (iVar10 == 0) {
            do {
                if ((local_f0 == 0) && (local_f8 == uVar6)) break;
                iVar10 = FUN_0203e154((int)auStack_68, 3);
            } while (iVar10 == 0);
        }
        iVar10 = iStack_ec + 1;
        FUN_0203b9a8(auStack_e8, (iVar2 + uVar9) - iVar10, iVar10);
        iVar10 = -iVar10;
    }
    iVar10 = uVar9 + iVar10;
    if (uVar7 != 0) {
        do {
            FUN_0203e070((int)auStack_68, local_40);
            local_38 = 0;
            *(u8 *)(iVar2 + iVar10 + -1) = 0x2f;
            iVar10 = iVar10 + -1;
            local_3c = auStack_fc;
            iVar4 = FUN_0203e154((int)auStack_68, 3);
            while (iVar1 = iStack_ec, iVar4 == 0) {
                if ((local_f0 != 0) && ((local_f8 & 0xffff) == uVar7)) {
                    FUN_0203b9a8(auStack_e8, (iVar2 + iVar10) - iStack_ec, iStack_ec);
                    iVar10 = iVar10 - iVar1;
                    break;
                }
                iVar4 = FUN_0203e154((int)auStack_68, 3);
            }
            uVar7 = (u32)local_48;
        } while (uVar7 != 0);
    }
    return 0;
}

// FUN_0203dbd8 @ 0x0203dbd8 (540 bytes) — walk path components
u32 FUN_0203dbd8(int param_1)
{
    char cVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;
    char cVar5;
    u32 uVar6;
    int iVar7;
    u32 uVar8;
    u32 *puVar9;
    int iVar10;
    int iVar11;
    char *pcVar12;
    u32 uStack_b8;
    u32 uStack_b4;
    u32 uStack_b0;
    int iStack_ac;
    int iStack_a8;
    u8 auStack_a4[128];

    iVar7 = *(int *)(param_1 + 0x3c);
    pcVar12 = *(char **)(param_1 + 0x38);
    FUN_0203e154(param_1, 2);
    cVar5 = *pcVar12;
    if (cVar5 != '\0') {
        iVar10 = 0;
        iVar11 = iVar10;
        do {
            while (TRUE) {
                cVar1 = pcVar12[iVar11];
                iVar2 = iVar10;
                if (((cVar1 != '\0') && (cVar1 != '/')) && (cVar1 != '\\')) {
                    iVar2 = 1;
                }
                if (iVar2 == 0) break;
                iVar11 = iVar11 + 1;
            }
            if ((cVar1 != '\0') || (iVar2 = 0, iVar7 != 0)) {
                iVar2 = 1;
            }
            if (iVar11 == 0) {
                return 1;
            }
            if (cVar5 == '.') {
                if (iVar11 == 1) {
                    pcVar12 = pcVar12 + 1;
                }
                else {
                    if (iVar11 != 2 || pcVar12[1] != '.') goto LAB_0203dcd4;
                    if (*(s16 *)(param_1 + 0x20) != 0) {
                        FUN_0203e070(param_1, *(u32 *)(param_1 + 0x28));
                    }
                    pcVar12 = pcVar12 + 2;
                }
            }
            else {
LAB_0203dcd4:
                if (0x7f < iVar11) {
                    return 1;
                }
                *(u32 *)(param_1 + 0x2c) = (u32)&uStack_b8;
                *(u32 *)(param_1 + 0x30) = 0;
                do {
                    iVar4 = FUN_0203e154(param_1, 3);
                    if (iVar4 != 0) {
                        return 1;
                    }
                } while (((iVar2 != iStack_ac) || (iVar11 != iStack_a8)) ||
                         (iVar4 = FUN_0203e00c((int)pcVar12, (int)auStack_a4, iVar11), iVar4 != 0));
                if (iVar2 == 0) {
                    if (iVar7 == 0) {
                        puVar9 = *(u32 **)(param_1 + 0x40);
                        *puVar9 = uStack_b8;
                        puVar9[1] = uStack_b4;
                        return 0;
                    }
                    return 1;
                }
                *(u32 *)(param_1 + 0x2c) = uStack_b8;
                *(u32 *)(param_1 + 0x30) = uStack_b4;
                *(u32 *)(param_1 + 0x34) = uStack_b0;
                pcVar12 = pcVar12 + iVar11;
                FUN_0203e154(param_1, 2);
            }
            pcVar12 = pcVar12 + (*pcVar12 != '\0');
            cVar5 = *pcVar12;
            iVar11 = iVar10;
        } while (cVar5 != '\0');
    }
    if (iVar7 != 0) {
        puVar9 = *(u32 **)(param_1 + 0x40);
        uVar6 = *(u32 *)(param_1 + 0x20);
        uVar8 = *(u32 *)(param_1 + 0x24);
        *puVar9 = *(u32 *)(param_1 + 0x1c);
        puVar9[1] = uVar6;
        puVar9[2] = uVar8;
    }
    return (u32)(iVar7 == 0);
}

// FUN_0203ddf4 @ 0x0203ddf4 (268 bytes) — read FNT entry
u32 FUN_0203ddf4(int param_1)
{
    u32 uVar1;
    u32 *puVar2;
    u8 local_18[2];
    u16 uStack_16;
    u32 local_14;
    int local_10;

    puVar2 = *(u32 **)(param_1 + 0x2c);
    local_14 = *(u32 *)(param_1 + 8);
    local_10 = *(int *)(param_1 + 0x24);
    FUN_0203e0a4((int *)&local_14, local_18, 1);
    puVar2[4] = local_18[0] & 0x7f;
    puVar2[3] = (int)(u32)local_18[0] >> 7;
    if (puVar2[4] != 0) {
        if (*(int *)(param_1 + 0x30) == 0) {
            FUN_0203e0a4((int *)&local_14, puVar2 + 5, 0);
            *(u8 *)((int)puVar2 + puVar2[4] + 0x14) = 0;
        }
        else {
            local_10 = local_10 + puVar2[4];
        }
        if (puVar2[3] == 0) {
            *puVar2 = *(u32 *)(param_1 + 8);
            puVar2[1] = (u32)*(u16 *)(param_1 + 0x22);
            *(s16 *)(param_1 + 0x22) = *(s16 *)(param_1 + 0x22) + 1;
        }
        else {
            FUN_0203e0a4((int *)&local_14, &uStack_16, 2);
            uVar1 = DAT_0203df00;
            *puVar2 = *(u32 *)(param_1 + 8);
            *(u16 *)(puVar2 + 1) = uStack_16 & (u16)uVar1;
            *(u16 *)((int)puVar2 + 6) = 0;
            puVar2[2] = 0;
        }
        *(int *)(param_1 + 0x24) = local_10;
        return 0;
    }
    return 1;
}

// FUN_0203df04 @ 0x0203df04 (148 bytes) — read directory header
u32 FUN_0203df04(int param_1)
{
    int iVar1;
    int local_20;
    u16 local_1c;
    u16 local_1a;
    int local_18;
    int local_14;

    iVar1 = *(int *)(param_1 + 8);
    local_14 = *(int *)(iVar1 + 0x28) + (u32)*(u16 *)(param_1 + 0x30) * 8;
    local_18 = iVar1;
    FUN_0203e0a4(&local_18, &local_20, 8);
    *(u32 *)(param_1 + 0x1c) = *(u32 *)(param_1 + 0x2c);
    *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x30);
    *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x34);
    if ((*(s16 *)(param_1 + 0x32) == 0) && (*(int *)(param_1 + 0x34) == 0)) {
        *(u16 *)(param_1 + 0x22) = local_1c;
        *(int *)(param_1 + 0x24) = *(int *)(iVar1 + 0x28) + local_20;
    }
    *(u32 *)(param_1 + 0x28) = local_1a & DAT_0203df98;
    return 0;
}

// FUN_0203df9c @ 0x0203df9c (56 bytes) — advance position and call write handler
void FUN_0203df9c(int param_1)
{
    *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + *(int *)(param_1 + 0x34);
    (**(void (**)(...))( *(int *)(param_1 + 8) + 0x40))(*(int *)(param_1 + 8), *(u32 *)(param_1 + 0x2c));
    return;
}

// FUN_0203dfd4 @ 0x0203dfd4 (56 bytes) — advance position and call read handler
void FUN_0203dfd4(int param_1)
{
    *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + *(int *)(param_1 + 0x34);
    (**(void (**)(...))( *(int *)(param_1 + 8) + 0x3c))(*(int *)(param_1 + 8), *(u32 *)(param_1 + 0x2c));
    return;
}

// FUN_0203e00c @ 0x0203e00c (100 bytes) — case-insensitive compare
int FUN_0203e00c(int param_1, int param_2, int param_3)
{
    u32 uVar1;
    u32 uVar2;
    int iVar3;

    iVar3 = 0;
    if (0 < param_3) {
        do {
            uVar2 = *(u8 *)(param_1 + iVar3) - 0x41;
            uVar1 = *(u8 *)(param_2 + iVar3) - 0x41;
            if (uVar2 < 0x1a) {
                uVar2 = *(u8 *)(param_1 + iVar3) - 0x21;
            }
            if (uVar1 < 0x1a) {
                uVar1 = *(u8 *)(param_2 + iVar3) - 0x21;
            }
            if (uVar2 != uVar1) {
                return uVar2 - uVar1;
            }
            iVar3 = iVar3 + 1;
        } while (iVar3 < param_3);
    }
    return 0;
}

// FUN_0203e070 @ 0x0203e070 (48 bytes) — set directory ID and dispatch
void FUN_0203e070(int param_1, u16 param_2)
{
    *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 4;
    *(u32 *)(param_1 + 0x2c) = *(u32 *)(param_1 + 8);
    *(u32 *)(param_1 + 0x34) = 0;
    *(u16 *)(param_1 + 0x32) = 0;
    *(u16 *)(param_1 + 0x30) = param_2;
    (*DAT_0203e0a0)(param_1, 2);
    return;
}

// FUN_0203e0a4 @ 0x0203e0a4 (176 bytes) — read data from archive
void FUN_0203e0a4(int *param_1, void *param_2, int param_3)
{
    int iVar1;
    u32 uVar2;
    int iVar3;

    iVar3 = *param_1;
    *(u32 *)(iVar3 + 0x10) = *(u32 *)(iVar3 + 0x10) | 0x200;
    iVar1 = (**(int (**)(...))( iVar3 + 0x44))(iVar3, param_2, param_1[1], param_3);
    if ((iVar1 == 0) || (iVar1 == 1)) {
        *(u32 *)(iVar3 + 0x10) = *(u32 *)(iVar3 + 0x10) & 0xfffffdff;
    }
    else if (iVar1 == 6) {
        uVar2 = FUN_0203ae38();
        while ((*(u32 *)(iVar3 + 0x10) & 0x200) != 0) {
            FUN_02039548(iVar3 + 0xc);
        }
        FUN_0203ae4c(uVar2);
    }
    param_1[1] = param_1[1] + param_3;
    return;
}

// FUN_0203e154 @ 0x0203e154 (396 bytes) — invoke archive command
int FUN_0203e154(int param_1, u32 param_2)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;

    iVar4 = *(int *)(param_1 + 8);
    uVar1 = 1 << (param_2 & 0xff);
    if ((*(u32 *)(param_1 + 0xc) & 4) == 0) {
        *(u32 *)(iVar4 + 0x10) = *(u32 *)(iVar4 + 0x10) | 0x100;
    }
    else {
        *(u32 *)(iVar4 + 0x10) = *(u32 *)(iVar4 + 0x10) | 0x200;
    }
    if ((*(u32 *)(iVar4 + 0x4c) & uVar1) == 0) {
        iVar2 = 7;
        goto switchD_default;
    }
    iVar2 = (**(int (**)(...))( iVar4 + 0x48))(param_1, param_2);
    switch (iVar2) {
    case 0:
    case 1:
    case 4:
        *(int *)(param_1 + 0x14) = iVar2;
        break;
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        *(u32 *)(iVar4 + 0x4c) = *(u32 *)(iVar4 + 0x4c) & ~uVar1;
        iVar2 = 7;
        break;
    }
switchD_default:
    if (iVar2 == 7) {
        iVar2 = (**(int (**)(...))( DAT_0203e2e0 + param_2 * 4))(param_1);
    }
    if (iVar2 == 6) {
        if ((*(u32 *)(param_1 + 0xc) & 4) != 0) {
            uVar3 = FUN_0203ae38();
            while ((*(u32 *)(iVar4 + 0x10) & 0x200) != 0) {
                FUN_02039548(iVar4 + 0xc);
            }
            iVar2 = *(int *)(param_1 + 0x14);
            FUN_0203ae4c(uVar3);
        }
    }
    else if ((*(u32 *)(param_1 + 0xc) & 4) == 0) {
        *(u32 *)(iVar4 + 0x10) = *(u32 *)(iVar4 + 0x10) & 0xfffffeff;
        FUN_0203e2e4(param_1, iVar2);
    }
    else {
        *(u32 *)(iVar4 + 0x10) = *(u32 *)(iVar4 + 0x10) & 0xfffffdff;
        *(int *)(param_1 + 0x14) = iVar2;
    }
    return iVar2;
}

// FUN_0203e2e4 @ 0x0203e2e4 (68 bytes) — finalize file operation
void FUN_0203e2e4(int param_1, u32 param_2)
{
    u32 uVar1;

    uVar1 = FUN_0203ae38();
    FUN_0203f34c((int *)param_1);
    *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) & 0xffffffb0;
    *(u32 *)(param_1 + 0x14) = param_2;
    FUN_020394d4(param_1 + 0x18);
    FUN_0203ae4c(uVar1);
    return;
}

// FUN_0203e348 @ 0x0203e348 (124 bytes) — unload archive if flagged
BOOL FUN_0203e348(int param_1)
{
    u32 uVar1;
    int iVar2;
    BOOL bVar3;

    iVar2 = 0;
    uVar1 = FUN_0203ae38();
    bVar3 = (*(u32 *)(param_1 + 0x10) & 8) == 0;
    if (!bVar3) {
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xfffffff7;
        iVar2 = FUN_0203ea0c(param_1);
    }
    FUN_0203ae4c(uVar1);
    if (iVar2 != 0) {
        FUN_0203e968(iVar2);
    }
    return bVar3;
}

// FUN_0203e3c4 @ 0x0203e3c4 (176 bytes) — load archive synchronously
BOOL FUN_0203e3c4(int param_1)
{
    u32 uVar1;
    BOOL bVar2;

    uVar1 = FUN_0203ae38();
    bVar2 = (*(u32 *)(param_1 + 0x10) & 8) == 0;
    if (bVar2) {
        if ((*(u32 *)(param_1 + 0x10) & 0x10) == 0) {
            *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 8;
        }
        else {
            *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 0x40;
            do {
                FUN_02039548(param_1 + 0xe);
            } while ((*(u32 *)(param_1 + 0x10) & 0x40) != 0);
        }
    }
    FUN_0203ae4c(uVar1);
    return bVar2;
}

// FUN_0203e474 @ 0x0203e474 (296 bytes) — relocate archive buffers
u32 FUN_0203e474(int param_1, int param_2, u32 param_3)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;
    u32 uVar5;
    u8 auStack_5c[72];

    uVar5 = *(int *)(param_1 + 0x24) + *(int *)(param_1 + 0x2c) + 0x3fU & 0xffffffe0;
    if (uVar5 <= param_3) {
        uVar3 = param_2 + 0x1fU & 0xffffffe0;
        FUN_0203f2a0(auStack_5c);
        iVar2 = FUN_0203f01c((int)auStack_5c, param_1, *(int *)(param_1 + 0x20),
                             *(int *)(param_1 + 0x20) + *(int *)(param_1 + 0x24), 0xffffffff);
        if (iVar2 != 0) {
            iVar2 = FUN_0203edcc(auStack_5c, uVar3, *(u32 *)(param_1 + 0x24));
            if (iVar2 < 0) {
                FUN_0203b914(uVar3, 0, *(u32 *)(param_1 + 0x24));
            }
            FUN_0203ef08(auStack_5c);
        }
        *(u32 *)(param_1 + 0x20) = uVar3;
        iVar4 = uVar3 + *(int *)(param_1 + 0x24);
        iVar2 = FUN_0203f01c((int)auStack_5c, param_1, *(int *)(param_1 + 0x28),
                             *(int *)(param_1 + 0x28) + *(int *)(param_1 + 0x2c), 0xffffffff);
        if (iVar2 != 0) {
            iVar2 = FUN_0203edcc(auStack_5c, iVar4, *(u32 *)(param_1 + 0x2c));
            if (iVar2 < 0) {
                FUN_0203b914(iVar4, 0, *(u32 *)(param_1 + 0x2c));
            }
            FUN_0203ef08(auStack_5c);
        }
        *(int *)(param_1 + 0x28) = iVar4;
        uVar1 = DAT_0203e59c;
        *(int *)(param_1 + 0x38) = param_2;
        *(u32 *)(param_1 + 0x44) = uVar1;
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 4;
    }
    return uVar5;
}

// FUN_0203e5a0 @ 0x0203e5a0 (112 bytes) — set archive table pointers
u32 FUN_0203e5a0(int param_1, u32 param_2, u32 param_3, u32 param_4,
                 u32 param_5, u32 param_6, int param_7, int param_8)
{
    *(u32 *)(param_1 + 0x1c) = param_2;
    *(u32 *)(param_1 + 0x24) = param_4;
    *(u32 *)(param_1 + 0x30) = param_3;
    *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x30);
    *(u32 *)(param_1 + 0x2c) = param_6;
    *(u32 *)(param_1 + 0x34) = param_5;
    *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x34);
    if (param_7 == 0) {
        param_7 = DAT_0203e610;
    }
    *(int *)(param_1 + 0x3c) = param_7;
    if (param_8 == 0) {
        param_8 = DAT_0203e614;
    }
    *(int *)(param_1 + 0x40) = param_8;
    *(u32 *)(param_1 + 0x44) = *(u32 *)(param_1 + 0x3c);
    *(u32 *)(param_1 + 0x38) = 0;
    *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 2;
    return 1;
}

// FUN_0203e618 @ 0x0203e618 (184 bytes) — register archive in list
u32 FUN_0203e618(u32 *param_1, u32 param_2, u32 param_3)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;
    int iVar4;
    u32 uVar5;

    uVar5 = 0;
    uVar3 = FUN_0203ae38();
    iVar4 = (int)FUN_0203ec34(param_2, param_3);
    piVar2 = DAT_0203e6d4;
    if (iVar4 == 0) {
        iVar4 = *DAT_0203e6d0;
        if (iVar4 == 0) {
            *DAT_0203e6d0 = (int)param_1;
            *piVar2 = (int)param_1;
            piVar2[2] = 0;
            *(u16 *)((int)piVar2 + 6) = 0;
            *(u16 *)(piVar2 + 1) = 0;
        }
        else {
            for (iVar1 = *(int *)(iVar4 + 4); iVar1 != 0; iVar1 = *(int *)(iVar1 + 4)) {
                iVar4 = iVar1;
            }
            *(u32 **)(iVar4 + 4) = param_1;
            param_1[2] = iVar4;
        }
        uVar5 = FUN_0203ecfc(param_2, param_3);
        *param_1 = uVar5;
        uVar5 = 1;
        param_1[4] = param_1[4] | 1;
    }
    FUN_0203ae4c(uVar3);
    return uVar5;
}

// FUN_0203e6d8 @ 0x0203e6d8 (40 bytes) — clear archive struct
void FUN_0203e6d8(int param_1)
{
    FUN_0203b914(param_1, 0, 0x50);
    *(u16 *)(param_1 + 0xc) = 0;
    *(u16 *)(param_1 + 0xe) = 0;
    return;
}

// FUN_0203e700 @ 0x0203e700 (144 bytes) — complete async I/O or finalize
void FUN_0203e700(int param_1, u32 param_2)
{
    int iVar1;
    u32 uVar2;

    if ((*(u32 *)(param_1 + 0x10) & 0x100) == 0) {
        iVar1 = *(int *)(param_1 + 0x18);
        uVar2 = FUN_0203ae38();
        *(u32 *)(iVar1 + 0x14) = param_2;
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xfffffdff;
        FUN_020394d4(param_1 + 0xc);
        FUN_0203ae4c(uVar2);
        return;
    }
    *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xfffffeff;
    FUN_0203e2e4(*(int *)(param_1 + 0x18), param_2);
    iVar1 = FUN_0203ea0c(param_1);
    if (iVar1 == 0) {
        return;
    }
    FUN_0203e968(iVar1);
    return;
}

// FUN_0203e790 @ 0x0203e790 (400 bytes) — open file handle and begin I/O
u32 FUN_0203e790(int param_1, u32 param_2)
{
    u32 uVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 8);
    *(u32 *)(param_1 + 0x10) = param_2;
    *(u32 *)(param_1 + 0x14) = 2;
    *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 1;
    uVar1 = FUN_0203ae38();
    if ((*(u32 *)(iVar2 + 0x10) & 0x80) != 0) {
        FUN_0203e2e4(param_1, 3);
        FUN_0203ae4c(uVar1);
        return 0;
    }
    if ((1 << (param_2 & 0xff) & 0x1fcU) != 0) {
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 4;
    }
    FUN_0203f304((int *)param_1, iVar2 + 0x14);
    if (((*(u32 *)(iVar2 + 0x10) & 8) == 0) && ((*(u32 *)(iVar2 + 0x10) & 0x10) == 0)) {
        *(u32 *)(iVar2 + 0x10) = *(u32 *)(iVar2 + 0x10) | 0x10;
        FUN_0203ae4c(uVar1);
        if ((*(u32 *)(iVar2 + 0x4c) & 0x200) != 0) {
            (**(void (**)(...))( iVar2 + 0x48))(param_1, 9);
        }
        FUN_0203ae38();
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 0x40;
        if ((*(u32 *)(param_1 + 0xc) & 4) == 0) {
            FUN_0203ae4c();
            FUN_0203e968(param_1);
            return 1;
        }
        FUN_0203ae4c();
    }
    else {
        if ((*(u32 *)(param_1 + 0xc) & 4) == 0) {
            FUN_0203ae4c(uVar1);
            return 1;
        }
        do {
            FUN_02039548(param_1 + 0x18);
        } while ((*(u32 *)(param_1 + 0xc) & 0x40) == 0);
        FUN_0203ae4c(uVar1);
    }
    uVar1 = FUN_0203e920(param_1);
    return uVar1;
}

// FUN_0203e920 @ 0x0203e920 (72 bytes) — execute and finalize I/O command
BOOL FUN_0203e920(int param_1)
{
    u32 uVar1;
    int iVar2;

    uVar1 = FUN_0203e154(param_1, *(u32 *)(param_1 + 0x10));
    FUN_0203e2e4(param_1, uVar1);
    iVar2 = FUN_0203ea0c(*(int *)(param_1 + 8));
    if (iVar2 != 0) {
        FUN_0203e968(iVar2);
    }
    return *(int *)(param_1 + 0x14) == 0;
}

// FUN_0203e968 @ 0x0203e968 (164 bytes) — process pending file operations
void FUN_0203e968(int param_1)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;

    uVar3 = *(u32 *)(param_1 + 8);
    if (param_1 == 0) {
        return;
    }
    while (TRUE) {
        uVar1 = FUN_0203ae38();
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 0x40;
        if ((*(u32 *)(param_1 + 0xc) & 4) != 0) {
            FUN_020394d4(param_1 + 0x18);
            FUN_0203ae4c(uVar1);
            return;
        }
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 8;
        FUN_0203ae4c(uVar1);
        iVar2 = FUN_0203e154(param_1, *(u32 *)(param_1 + 0x10));
        if (iVar2 == 6) break;
        param_1 = FUN_0203ea0c(uVar3);
        if (param_1 == 0) {
            return;
        }
    }
    return;
}

// FUN_0203ea0c @ 0x0203ea0c (552 bytes) — dequeue and start next I/O
int FUN_0203ea0c(int param_1)
{
    int iVar1;
    u32 uVar2;
    int iVar3;
    BOOL bVar4;
    u8 auStack_60[8];
    int local_58;

    uVar2 = FUN_0203ae38();
    if ((*(u32 *)(param_1 + 0x10) & 0x20) != 0) {
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xffffffdf;
        iVar3 = *(int *)(param_1 + 0x18);
        while (iVar1 = iVar3, iVar1 != 0) {
            iVar3 = *(int *)(iVar1 + 4);
            if ((*(u32 *)(iVar1 + 0xc) & 2) != 0) {
                if (*(int *)(param_1 + 0x18) == iVar1) {
                    *(int *)(param_1 + 0x18) = iVar3;
                }
                FUN_0203e2e4(iVar1, 3);
                if (iVar3 == 0) {
                    iVar3 = *(int *)(param_1 + 0x18);
                }
            }
        }
    }
    if ((((*(u32 *)(param_1 + 0x10) & 0x40) == 0) && ((*(u32 *)(param_1 + 0x10) & 8) == 0)) &&
       (iVar3 = *(int *)(param_1 + 0x18), iVar3 != 0)) {
        bVar4 = (*(u32 *)(param_1 + 0x10) & 0x10) == 0;
        if (bVar4) {
            *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 0x10;
        }
        FUN_0203ae4c(uVar2);
        if ((bVar4) && ((*(u32 *)(param_1 + 0x4c) & 0x200) != 0)) {
            (**(void (**)(...))( param_1 + 0x48))(iVar3, 9);
        }
        uVar2 = FUN_0203ae38();
        *(u32 *)(iVar3 + 0xc) = *(u32 *)(iVar3 + 0xc) | 0x40;
        if ((*(u32 *)(iVar3 + 0xc) & 4) != 0) {
            FUN_020394d4(iVar3 + 0x18);
            FUN_0203ae4c(uVar2);
            return 0;
        }
        FUN_0203ae4c(uVar2);
        return iVar3;
    }
    if (((*(u32 *)(param_1 + 0x10) & 0x10) != 0) &&
       (*(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xffffffef,
       (*(u32 *)(param_1 + 0x4c) & 0x400) != 0)) {
        FUN_0203f2a0(auStack_60);
        local_58 = param_1;
        (**(void (**)(...))( param_1 + 0x48))(auStack_60, 10);
    }
    if ((*(u32 *)(param_1 + 0x10) & 0x40) != 0) {
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) & 0xffffffbf;
        *(u32 *)(param_1 + 0x10) = *(u32 *)(param_1 + 0x10) | 8;
        FUN_020394d4(param_1 + 0xe);
    }
    FUN_0203ae4c(uVar2);
    return 0;
}

// FUN_0203ec34 @ 0x0203ec34 (76 bytes) — find archive by name hash
int *FUN_0203ec34(int param_1, int param_2)
{
    int iVar1;
    int *piVar2;

    iVar1 = FUN_0203ecfc(param_1, param_2);
    FUN_0203ae38();
    for (piVar2 = (int *)*DAT_0203ec80; (piVar2 != (int *)0 && (*piVar2 != iVar1));
        piVar2 = (int *)piVar2[1]) {
    }
    FUN_0203ae4c();
    return piVar2;
}

// FUN_0203eca8 @ 0x0203eca8 (44 bytes) — read from archive memory
u32 FUN_0203eca8(int param_1, u32 param_2, int param_3, u32 param_4)
{
    FUN_0203b9a8(param_2, *(int *)(param_1 + 0x1c) + param_3, param_4);
    return 0;
}

// FUN_0203ecd4 @ 0x0203ecd4 (40 bytes) — write to archive memory
u32 FUN_0203ecd4(int param_1, u32 param_2, int param_3, u32 param_4)
{
    FUN_0203b9a8(*(int *)(param_1 + 0x1c) + param_3, param_2, param_4);
    return 0;
}

// FUN_0203ecfc @ 0x0203ecfc (100 bytes) — compute name hash
u32 FUN_0203ecfc(int param_1, int param_2)
{
    u32 uVar1;
    u32 uVar2;
    int iVar3;
    u32 uVar4;

    uVar4 = 0;
    if ((param_2 < 4) && (iVar3 = 0, 0 < param_2)) {
        uVar2 = 0;
        do {
            uVar1 = (u32)*(u8 *)(param_1 + iVar3);
            if (uVar1 == 0) {
                return uVar4;
            }
            if (uVar1 - 0x41 < 0x1a) {
                uVar1 = uVar1 + 0x20;
            }
            iVar3 = iVar3 + 1;
            uVar4 = uVar4 | uVar1 << (uVar2 & 0xff);
            uVar2 = uVar2 + 8;
        } while (iVar3 < param_2);
    }
    return uVar4;
}

// FUN_0203edec @ 0x0203edec (76 bytes) — mark file for cancellation
void FUN_0203edec(int param_1)
{
    FUN_0203ae38();
    if ((*(u32 *)(param_1 + 0xc) & 1) != 0) {
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 2;
        *(u32 *)(*(int *)(param_1 + 8) + 0x10) = *(u32 *)(*(int *)(param_1 + 8) + 0x10) | 0x20;
    }
    FUN_0203ae4c();
    return;
}

// FUN_0203ee38 @ 0x0203ee38 (208 bytes) — wait for I/O completion
u32 FUN_0203ee38(int param_1)
{
    BOOL bVar1;
    u32 uVar2;
    u32 uVar3;

    bVar1 = FALSE;
    uVar2 = FUN_0203ae38();
    if ((*(u32 *)(param_1 + 0xc) & 1) != 0) {
        uVar3 = *(u32 *)(param_1 + 0xc) & 0x44;
        bVar1 = uVar3 == 0;
        if (uVar3 == 0) {
            *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 4;
            do {
                FUN_02039548(param_1 + 0x18);
            } while ((*(u32 *)(param_1 + 0xc) & 0x40) == 0);
        }
        else {
            do {
                FUN_02039548(param_1 + 0x18);
            } while ((*(u32 *)(param_1 + 0xc) & 1) != 0);
        }
    }
    FUN_0203ae4c(uVar2);
    if (bVar1) {
        uVar3 = FUN_0203e920(param_1);
        return uVar3;
    }
    return (u32)(*(int *)(param_1 + 0x14) == 0);
}

// FUN_0203f01c @ 0x0203f01c (88 bytes) — open file for reading
u32 FUN_0203f01c(int param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5)
{
    int iVar1;

    *(u32 *)(param_1 + 8) = param_2;
    *(u32 *)(param_1 + 0x34) = param_5;
    *(u32 *)(param_1 + 0x2c) = param_3;
    *(u32 *)(param_1 + 0x30) = param_4;
    iVar1 = FUN_0203e790(param_1, 7);
    if (iVar1 != 0) {
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 0x10;
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) & 0xffffffdf;
        return 1;
    }
    return 0;
}

// FUN_0203f074 @ 0x0203f074 (68 bytes) — check if path exists
BOOL FUN_0203f074(u32 param_1, u32 param_2)
{
    int iVar1;
    u8 auStack_50[68];

    FUN_0203f2a0(auStack_50);
    iVar1 = FUN_0203f140((int)auStack_50, (char *)(u32)param_2, param_1, 0);
    return iVar1 != 0;
}

// FUN_0203f0b8 @ 0x0203f0b8 (136 bytes) — read data from file
int FUN_0203f0b8(int param_1, u32 param_2, int param_3, int param_4)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar2 = *(int *)(param_1 + 0x28);
    *(u32 *)(param_1 + 0x2c) = param_2;
    iVar1 = *(int *)(param_1 + 0x24) - iVar2;
    iVar3 = param_3;
    if (iVar1 < param_3) {
        iVar3 = iVar1;
    }
    if (iVar3 < 0) {
        iVar3 = 0;
    }
    *(int *)(param_1 + 0x30) = param_3;
    *(int *)(param_1 + 0x34) = iVar3;
    if (param_4 == 0) {
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) | 4;
    }
    FUN_0203e790(param_1, 0);
    if (param_4 == 0) {
        iVar3 = FUN_0203ee38(param_1);
        if (iVar3 == 0) {
            iVar3 = -1;
        }
        else {
            iVar3 = *(int *)(param_1 + 0x28) - iVar2;
        }
    }
    return iVar3;
}

// FUN_0203f140 @ 0x0203f140 (348 bytes) — resolve path to archive entry
int FUN_0203f140(int param_1, char *param_2, u32 param_3, int param_4)
{
    char cVar1;
    int iVar2;
    int local_28;
    u32 local_24;
    int local_20;

    if ((*param_2 == '/') || (*param_2 == '\\')) {
        local_28 = *DAT_0203f29c;
        local_20 = 0;
        local_24 = 0;
        param_2 = param_2 + 1;
    }
    else {
        local_28 = *DAT_0203f29c;
        local_24 = DAT_0203f29c[1];
        local_20 = DAT_0203f29c[2];
        iVar2 = 0;
        do {
            cVar1 = param_2[iVar2];
            if (((cVar1 == '\0') || (cVar1 == '/')) || (cVar1 == '\\')) break;
            if (cVar1 == ':') {
                local_28 = (int)FUN_0203ec34((int)param_2, iVar2);
                if (local_28 == 0) {
                    return 0;
                }
                if ((*(u32 *)(local_28 + 0x10) & 2) == 0) {
                    return 0;
                }
                local_20 = 0;
                local_24 = 0;
                param_2 = param_2 + iVar2 + 1;
                if ((*param_2 == '/') || (*param_2 == '\\')) {
                    param_2 = param_2 + 1;
                }
                break;
            }
            iVar2 = iVar2 + 1;
        } while (iVar2 < 4);
    }
    *(int *)(param_1 + 8) = local_28;
    *(char **)(param_1 + 0x38) = param_2;
    *(int *)(param_1 + 0x2c) = local_28;
    *(u32 *)(param_1 + 0x30) = local_24;
    *(int *)(param_1 + 0x34) = local_20;
    if (param_4 == 0) {
        *(u32 *)(param_1 + 0x3c) = 0;
        *(u32 *)(param_1 + 0x40) = param_3;
    }
    else {
        *(u32 *)(param_1 + 0x3c) = 1;
        *(int *)(param_1 + 0x40) = param_4;
    }
    FUN_0203e790(param_1, 4);
    return 1;
}

// FUN_0203f304 @ 0x0203f304 (72 bytes) — append node to linked list
void FUN_0203f304(int *param_1, int param_2)
{
    int iVar1;
    int *piVar2;
    int iVar3;

    iVar3 = *param_1;
    piVar2 = (int *)param_1[1];
    if (iVar3 != 0) {
        *(int **)(iVar3 + 4) = piVar2;
    }
    if (piVar2 != (int *)0) {
        *piVar2 = iVar3;
    }
    iVar3 = *(int *)(param_2 + 4);
    while (iVar1 = iVar3, iVar1 != 0) {
        param_2 = iVar1;
        iVar3 = *(int *)(iVar1 + 4);
    }
    *(int **)(param_2 + 4) = param_1;
    *param_1 = param_2;
    param_1[1] = 0;
    return;
}

// FUN_0203f34c @ 0x0203f34c (44 bytes) — remove node from linked list
void FUN_0203f34c(int *param_1)
{
    int *piVar1;
    int iVar2;

    iVar2 = *param_1;
    piVar1 = (int *)param_1[1];
    if (iVar2 != 0) {
        *(int **)(iVar2 + 4) = piVar1;
    }
    if (piVar1 != (int *)0) {
        *piVar1 = iVar2;
    }
    *param_1 = 0;
    param_1[1] = *param_1;
    return;
}

// FUN_0203f3ec @ 0x0203f3ec (356 bytes) — initialize filesystem
void FUN_0203f3ec(u32 param_1)
{
    u32 *puVar1;
    u32 *puVar2;
    u32 uVar3;
    int *piVar4;
    int *piVar5;
    u32 uVar6;
    int iVar7;

    *DAT_0203f550 = param_1;
    uVar6 = FUN_02038e7c();
    puVar2 = DAT_0203f55c;
    puVar1 = DAT_0203f558;
    *DAT_0203f554 = uVar6;
    *puVar1 = 0;
    puVar1[1] = 0;
    *puVar2 = 0;
    puVar2[1] = 0;
    FUN_020432a8();
    FUN_0203e6d8(DAT_0203f560);
    FUN_0203e618((u32 *)DAT_0203f560, DAT_0203f564, 3);
    piVar5 = DAT_0203f584;
    piVar4 = (int *)DAT_0203f578;
    uVar3 = DAT_0203f56c;
    uVar6 = DAT_0203f560;
    puVar2 = DAT_0203f55c;
    puVar1 = DAT_0203f558;
    if (*DAT_0203f568 == 2) {
        *DAT_0203f558 = 0xffffffff;
        puVar1[1] = 0;
        *puVar2 = 0xffffffff;
        puVar2[1] = 0;
        FUN_0203e328(uVar6, uVar3);
        FUN_0203e5a0(DAT_0203f560, 0, 0, 0, 0, 0, 0, 0);
        return;
    }
    FUN_0203e328(DAT_0203f560, DAT_0203f57c, DAT_0203f580);
    if (*piVar4 == -1) {
        return;
    }
    if (*piVar4 == 0) {
        return;
    }
    iVar7 = *piVar5;
    if (iVar7 != -1) {
        if (iVar7 != 0) {
            FUN_0203e5a0(DAT_0203f560, 0, iVar7, piVar5[1], 0, 0, 0, 0);
            return;
        }
        return;
    }
    return;
}

// FUN_0203f59c @ 0x0203f59c (140 bytes) — card archive command handler
u32 FUN_0203f59c(u32 param_1, int param_2)
{
    if (param_2 == 1) {
        return 4;
    }
    if (param_2 != 9) {
        if (param_2 != 10) {
            return 8;
        }
        FUN_020425e0(DAT_0203f628 & 0xffff);
        return 0;
    }
    FUN_02042600(DAT_0203f628 & 0xffff);
    return 0;
}

// FUN_0203f634 @ 0x0203f634 (68 bytes) — card read via DMA
u32 FUN_0203f634(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0204333c(DAT_0203f67c, param_3, param_2, param_4, DAT_0203f678, param_1, 1);
    return 6;
}

// FUN_0203f690 @ 0x0203f690 (84 bytes) — verify file header
u32 FUN_0203f690(u32 param_1, u32 param_2)
{
    int iVar1;
    u8 auStack_30[48];

    iVar1 = FUN_0203fb40(auStack_30, param_1, param_2);
    if ((iVar1 != 0) && (iVar1 = FUN_0203f740(auStack_30), iVar1 != 0)) {
        return 1;
    }
    return 0;
}

// FUN_0203f6e4 @ 0x0203f6e4 (92 bytes) — verify and finalize file
// HOST_PORT override in host_nds_fs.c (avoids int pointer truncation on 64-bit)
#ifndef HOST_PORT
u32 FUN_0203f6e4(u32 param_1, u32 param_2)
{
    int iVar1;
    u8 auStack_30[48];

    iVar1 = FUN_0203fb40(auStack_30, param_1, param_2);
    if ((iVar1 != 0) && (iVar1 = FUN_0203fa08((int)auStack_30), iVar1 != 0)) {
        FUN_0203f864(auStack_30);
        return 1;
    }
    return 0;
}
#endif

// FUN_0203f964 @ 0x0203f964 (156 bytes) — HMAC verify
BOOL FUN_0203f964(int param_1, u32 param_2, u32 param_3)
{
    u32 uVar1;
    int *piVar2;
    int local_68;
    int local_64[5];
    u8 auStack_50[64];

    piVar2 = &local_68;
    FUN_0203b914(local_64, 0, 0x14);
    FUN_0203b9a8(*DAT_0203fa00, auStack_50, *DAT_0203fa04);
    local_68 = *DAT_0203fa04;
    FUN_0203fdec((int)local_64, param_2, param_3, auStack_50, 0);
    uVar1 = 0;
    do {
        piVar2 = piVar2 + 1;
        if (*piVar2 != *(int *)(param_1 + uVar1)) break;
        uVar1 = uVar1 + 4;
    } while (uVar1 < 0x14);
    return uVar1 == 0x14;
}

// FUN_0203fa08 @ 0x0203fa08 (160 bytes) — write file data
// HOST_PORT override in host_nds_fs.c (avoids int pointer truncation on 64-bit)
#ifndef HOST_PORT
u32 FUN_0203fa08(int param_1)
{
    int iVar1;
    int iVar2;
    u32 local_58;
    u32 uStack_54;
    u8 auStack_50[68];

    FUN_0203f2a0(auStack_50);
    FUN_0203fd64(&local_58, param_1);
    iVar1 = FUN_0203efa0(auStack_50, local_58, uStack_54);
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = FUN_0203fdd0(param_1);
    FUN_0203fd8c(param_1);
    iVar2 = FUN_0203edcc(auStack_50, *(u32 *)(param_1 + 4), iVar1);
    if (iVar1 == iVar2) {
        FUN_0203ef08(auStack_50);
        return 1;
    }
    FUN_0203ef08(auStack_50);
    return 0;
}
#endif

// FUN_0203fc54 @ 0x0203fc54 (272 bytes) — read file table entry
// HOST_PORT override in host_nds_fs.c (avoids int pointer truncation on 64-bit)
#ifndef HOST_PORT
u32 FUN_0203fc54(int param_1, int param_2, int param_3, u32 param_4,
                 int param_5, u32 param_6, int param_7, u32 param_8)
{
    int iVar1;
    u32 uStack_6c;
    u32 uStack_68;
    u8 auStack_64[32];
    int iStack_44;
    int iStack_40;

    if (param_2 != 0) {
        param_5 = param_7;
        param_6 = param_8;
    }
    if (param_6 <= (u32)(param_3 * 0x20)) {
        return 0;
    }
    FUN_0203f2a0(auStack_64);
    iVar1 = FUN_0203f01c((int)auStack_64, param_4, param_5 + param_3 * 0x20, param_5 + param_6, 0);
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = FUN_0203edcc(auStack_64, param_1, 0x20);
    if (iVar1 != 0x20) {
        FUN_0203ef08(auStack_64);
        return 0;
    }
    FUN_0203ef08(auStack_64);
    *(int *)(param_1 + 0x20) = param_2;
    FUN_0203fd64(&uStack_6c, param_1);
    iVar1 = FUN_0203efa0(auStack_64, uStack_6c, uStack_68);
    if (iVar1 == 0) {
        return 0;
    }
    *(int *)(param_1 + 0x24) = iStack_44;
    *(int *)(param_1 + 0x28) = iStack_40 - iStack_44;
    FUN_0203ef08(auStack_64);
    return 1;
}
#endif

// FUN_0203fdec@ 0x0203fdec (396 bytes) — HMAC-SHA1 computation
void FUN_0203fdec(int param_1, int param_2, int param_3, u8 *param_4, int param_5)
{
    int iVar1;
    u8 *pbVar2;
    u8 *pbVar3;
    u8 abStack_118[64];
    u8 abStack_d8[64];
    u8 abStack_98[20];
    u8 auStack_84[104];

    pbVar2 = abStack_118;
    if (param_1 == 0) {
        return;
    }
    if (param_2 == 0) {
        return;
    }
    if (param_3 == 0) {
        return;
    }
    if (param_4 != (u8 *)0) {
        if (param_5 != 0) {
            if (0x40 < param_5) {
                FUN_0204056c(auStack_84);
                FUN_020403c4(auStack_84, param_4, param_5);
                FUN_020404b0(auStack_84, abStack_98);
                param_5 = 0x14;
                param_4 = abStack_98;
            }
            iVar1 = 0;
            if (0 < param_5) {
                pbVar3 = abStack_d8;
                do {
                    iVar1 = iVar1 + 1;
                    *pbVar2 = *param_4 ^ 0x36;
                    pbVar2 = pbVar2 + 1;
                    *pbVar3 = *param_4 ^ 0x5c;
                    pbVar3 = pbVar3 + 1;
                    param_4 = param_4 + 1;
                } while (iVar1 < param_5);
            }
            if (iVar1 < 0x40) {
                pbVar3 = abStack_118 + iVar1;
                pbVar2 = abStack_d8 + iVar1;
                do {
                    *pbVar3 = 0x36;
                    iVar1 = iVar1 + 1;
                    *pbVar2 = 0x5c;
                    pbVar3 = pbVar3 + 1;
                    pbVar2 = pbVar2 + 1;
                } while (iVar1 < 0x40);
            }
            FUN_0204056c(auStack_84);
            FUN_020403c4(auStack_84, abStack_118, 0x40);
            FUN_020403c4(auStack_84, param_2, param_3);
            FUN_020404b0(auStack_84, abStack_98);
            FUN_0204056c(auStack_84);
            FUN_020403c4(auStack_84, abStack_d8, 0x40);
            FUN_020403c4(auStack_84, abStack_98, 0x14);
            FUN_020404b0(auStack_84, param_1);
            return;
        }
        return;
    }
    return;
}

// FUN_0203ff78 @ 0x0203ff78 (332 bytes) — SHA-1 finalize with padding
void FUN_0203ff78(int param_1)
{
    int iVar1;

    iVar1 = *(int *)(param_1 + 0x1c);
    if (iVar1 < 0x38) {
        *(int *)(param_1 + 0x1c) = iVar1 + 1;
        *(u8 *)(param_1 + iVar1 + 0x20) = 0x80;
        iVar1 = *(int *)(param_1 + 0x1c);
        while (iVar1 < 0x38) {
            *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
            *(u8 *)(param_1 + iVar1 + 0x20) = 0;
            iVar1 = *(int *)(param_1 + 0x1c);
        }
    }
    else {
        *(int *)(param_1 + 0x1c) = iVar1 + 1;
        *(u8 *)(param_1 + iVar1 + 0x20) = 0x80;
        iVar1 = *(int *)(param_1 + 0x1c);
        while (iVar1 < 0x40) {
            *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
            *(u8 *)(param_1 + iVar1 + 0x20) = 0;
            iVar1 = *(int *)(param_1 + 0x1c);
        }
        FUN_020400c4(param_1);
        iVar1 = *(int *)(param_1 + 0x1c);
        if (iVar1 < 0x38) {
            do {
                *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
                *(u8 *)(param_1 + iVar1 + 0x20) = 0;
                iVar1 = *(int *)(param_1 + 0x1c);
            } while (iVar1 < 0x38);
        }
    }
    *(char *)(param_1 + 0x58) = (char)((u32)*(u32 *)(param_1 + 0x18) >> 0x18);
    *(char *)(param_1 + 0x59) = (char)((u32)*(u32 *)(param_1 + 0x18) >> 0x10);
    *(char *)(param_1 + 0x5a) = (char)((u32)*(u32 *)(param_1 + 0x18) >> 8);
    *(char *)(param_1 + 0x5b) = (char)*(u32 *)(param_1 + 0x18);
    *(char *)(param_1 + 0x5c) = (char)((u32)*(u32 *)(param_1 + 0x14) >> 0x18);
    *(char *)(param_1 + 0x5d) = (char)((u32)*(u32 *)(param_1 + 0x14) >> 0x10);
    *(char *)(param_1 + 0x5e) = (char)((u32)*(u32 *)(param_1 + 0x14) >> 8);
    *(char *)(param_1 + 0x5f) = (char)*(u32 *)(param_1 + 0x14);
    FUN_020400c4(param_1);
    return;
}
