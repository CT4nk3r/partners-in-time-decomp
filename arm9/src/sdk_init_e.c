/**
 * SDK Initialization Functions (Part E)
 *
 * C++ runtime support: exception handling tables, type matching,
 * destructor iteration, and object unwinding.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wint-conversion"

/* Extern data references */
extern u32 DAT_02048550;
extern u32 *DAT_02048614;
extern u32 DAT_02048c44;
extern u32 DAT_02048cb8;
extern u32 *DAT_020488a0;

/* Extern function declarations */
extern char *FUN_020470d8(...);
extern void FUN_02046ef0(...);
extern void FUN_02046e1c(...);
extern u32 FUN_02046f98(...);
extern int FUN_02046ed0(...);
extern u32 FUN_02046e98(...);
extern int FUN_020466bc(...);
extern void FUN_02048880(...);
extern void FUN_02048ce0(...);
extern void FUN_02048c6c(...);
extern void thunk_FUN_02029ab8(...);

/* Forward declarations for functions in this file */
char *FUN_02047180(char *param_1, u32 *param_2);
void FUN_02047228(int *param_1);
void FUN_02047270(int *param_1);
void FUN_02047354(u32 *param_1, int param_2, int param_3);
int FUN_020473c4(u32 *param_1, u32 *param_2, u32 *param_3);
void FUN_020475a4(u32 *param_1, int *param_2, int param_3, u32 param_4);
u32 FUN_02047610(u32 param_1, u32 *param_2);
int FUN_0204769c(u32 *param_1, u32 *param_2);
void FUN_020477f0(int param_1, int param_2, u8 *param_3);
u32 FUN_02048078(int param_1);
void FUN_02048430(int param_1, int *param_2);
int FUN_02048554(int param_1, int param_2, u32 param_3);
void FUN_020485c0(void);
u32 FUN_02048650(char *param_1, char *param_2, int *param_3);
void FUN_020488a4(void);
void FUN_020488b0(int param_1, u32 param_2, int param_3, int param_4);
void FUN_020488f8(int param_1, int param_2, int param_3, void (*param_4)(...));
void FUN_02048970(int param_1, int param_2, int param_3, void (*param_4)(...));
void FUN_02048a1c(int param_1, int param_2, int param_3, void (*param_4)(...), int param_5);
int FUN_02048ae8(int param_1, int param_2, int param_3, u32 param_4, void (*param_5)(...), int param_6);
void FUN_02048bb0(u32 param_1, u32 param_2, int param_3, void (*param_4)(...));
u32 *FUN_02048c1c(u32 *param_1);
u32 *FUN_02048c90(u32 *param_1);
u8 FUN_02048414(int *param_1);

// FUN_02047180@ 0x02047180 (168 bytes) — Decode variable-length integer
char *FUN_02047180(char *param_1, u32 *param_2)
{
    u32 uVar1;
    u32 uVar2;

    uVar1 = (u32)*param_1;
    if ((uVar1 & 1) == 0) {
        *param_2 = (int)uVar1 >> 1;
        return param_1 + 1;
    }
    uVar2 = (u32)(u8)param_1[1];
    if ((uVar1 & 2) != 0) {
        if ((uVar1 & 4) != 0) {
            *param_2 = (u32)(u8)param_1[3] |
                       ((int)uVar1 >> 3) << 0x18 | uVar2 << 0x10 | (u32)(u8)param_1[2] << 8;
            return param_1 + 4;
        }
        *param_2 = (u32)(u8)param_1[2] | ((int)uVar1 >> 3) << 0x10 | uVar2 << 8;
        return param_1 + 3;
    }
    *param_2 = uVar2 | ((int)uVar1 >> 2) << 8;
    return param_1 + 2;
}

// FUN_02047228 @ 0x02047228 (72 bytes) — Call destructor if object is live
void FUN_02047228(int *param_1)
{
    if (*param_1 == 0) {
        return;
    }
    if ((void *)param_1[2] != (void *)0x0) {
        ((void (*)(...))(param_1[2]))(*param_1, 0xffffffff);
        return;
    }
    return;
}

// FUN_02047270 @ 0x02047270 (228 bytes) — Process exception table entry
void FUN_02047270(int *param_1)
{
    int iVar1;
    u32 uVar2;
    u32 local_38;
    int local_34;
    int local_30;
    u32 local_1c;
    int local_18;
    u32 local_14[2];

    FUN_02048430(param_1[4], &local_34);
    if (local_30 == 0) {
        FUN_02048880();
    }
    FUN_02046ef0(param_1, &local_34);
    if (*param_1 == 0) {
        iVar1 = FUN_0204769c(param_1, &local_34);
        param_1[3] = iVar1;
    }
    else {
        param_1[3] = 0;
    }
    iVar1 = FUN_020473c4(param_1, &local_34, &local_38);
    local_1c = *(u32 *)(iVar1 + 1);
    uVar2 = (u32)FUN_020470d8(iVar1 + 5, &local_18);
    FUN_02047180((char *)uVar2, local_14);
    FUN_020477f0(param_1, &local_34, iVar1);
    FUN_02047354(param_1, local_14[0], local_38);
    FUN_02046e1c(param_1, &local_34, local_34 + local_18);
    return;
}

// FUN_02047354 @ 0x02047354 (112 bytes) — Store exception context into unwind area
void FUN_02047354(u32 *param_1, int param_2, int param_3)
{
    int iVar1;

    iVar1 = param_1[6] + param_2;
    *(u32 *)(param_1[6] + param_2) = param_1[1];
    *(u32 *)(iVar1 + 4) = *param_1;
    *(u32 *)(iVar1 + 8) = param_1[2];
    if (*(char *)*param_1 == '*') {
        *(int *)(iVar1 + 0xc) = iVar1 + 0x10;
        *(int *)(iVar1 + 0x10) = *(int *)param_1[1] + param_3;
        return;
    }
    *(int *)(iVar1 + 0xc) = param_1[1] + param_3;
    return;
}

// FUN_020473c4 @ 0x020473C4 (480 bytes) — Search exception table for matching catch
int FUN_020473c4(u32 *param_1, u32 *param_2, u32 *param_3)
{
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    int iVar5;
    u32 *puVar6;
    u32 *puVar7;
    u8 auStack_c4[4];
    u8 auStack_c0[4];
    u8 auStack_bc[4];
    u8 auStack_b8[4];
    u8 auStack_b4[4];
    u32 local_b0;
    u32 local_ac;
    u32 local_a8;
    int local_a4;
    u32 local_a0;
    u32 local_9c;
    u32 local_98;
    u32 local_94;
    u32 local_90;
    u32 local_8c;
    u32 local_88;
    u32 local_84;
    u32 local_80;
    u32 local_7c;
    u32 local_78[21];

    local_ac = *param_2;
    local_a8 = param_2[1];
    puVar6 = local_78;
    local_a4 = param_2[2];
    local_a0 = param_2[3];
    puVar7 = param_1 + 7;
    local_9c = param_2[4];
    iVar5 = 5;
    local_98 = param_2[5];
    local_94 = *param_1;
    local_90 = param_1[1];
    local_8c = param_1[2];
    local_88 = param_1[3];
    local_84 = param_1[4];
    local_80 = param_1[5];
    local_7c = param_1[6];
    do {
        uVar1 = *puVar7;
        uVar2 = puVar7[1];
        uVar3 = puVar7[2];
        uVar4 = puVar7[3];
        puVar7 = puVar7 + 4;
        *puVar6 = uVar1;
        puVar6[1] = uVar2;
        puVar6[2] = uVar3;
        puVar6[3] = uVar4;
        puVar6 = puVar6 + 4;
        iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    *puVar6 = *puVar7;
    uVar1 = (u32)FUN_02048414(&local_ac);
    do {
        switch (uVar1) {
        case 0:
            break;
        case 1:
        default:
        switchD_default:
            FUN_02048880();
            return local_a4;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 0xb:
            break;
        case 0xc:
            uVar1 = *(u32 *)(local_a4 + 1);
            uVar2 = (u32)FUN_020470d8(local_a4 + 5, auStack_c4);
            FUN_02047180((char *)uVar2, auStack_c0);
            iVar5 = FUN_02048650((char *)*param_1, (char *)uVar1, (int *)param_3);
            if (iVar5 != 0) {
                return local_a4;
            }
            break;
        case 0xd:
            break;
        case 0xe:
            goto switchD_default;
        case 0xf:
            uVar1 = (u32)FUN_020470d8(local_a4 + 1, auStack_bc);
            uVar1 = (u32)FUN_020470d8(uVar1, auStack_b8);
            local_b0 = (u32)FUN_02047180((char *)uVar1, auStack_b4);
            iVar5 = FUN_02047610(*param_1, auStack_bc);
            if (iVar5 == 0) {
                FUN_020475a4(param_1, param_2, auStack_bc, local_a4);
            }
            break;
        case 0x10:
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        case 0x13:
            break;
        }
        uVar1 = (u32)FUN_02048078(&local_ac);
    } while (1);
}

// FUN_020475a4 @ 0x020475A4 (108 bytes) — Handle exception with partial unwind
void FUN_020475a4(u32 *param_1, int *param_2, int param_3, u32 param_4)
{
    int iVar1;

    FUN_020477f0(param_1, param_2, param_4);
    iVar1 = param_1[6] + *(int *)(param_3 + 8);
    *(u32 *)(param_1[6] + *(int *)(param_3 + 8)) = param_1[1];
    *(u32 *)(iVar1 + 4) = *param_1;
    *(u32 *)(iVar1 + 8) = param_1[2];
    *(u32 *)(iVar1 + 0x14) = param_4;
    FUN_02046e1c(param_1, param_2, *param_2 + *(int *)(param_3 + 4));
    return;
}

// FUN_02047610 @ 0x02047610 (140 bytes) — Check exception spec list for type match
u32 FUN_02047610(u32 param_1, u32 *param_2)
{
    int iVar1;
    u32 uVar2;
    u32 *puVar3;
    u8 auStack_1c[4];

    puVar3 = (u32 *)param_2[3];
    uVar2 = 0;
    if (*param_2 != 0) {
        do {
            iVar1 = FUN_02048650((char *)param_1, (char *)*puVar3, (int *)auStack_1c);
            if (iVar1 != 0) {
                return 1;
            }
            uVar2 = uVar2 + 1;
            puVar3 = puVar3 + 1;
        } while (uVar2 < *param_2);
    }
    return 0;
}

// FUN_0204769c @ 0x0204769C (340 bytes) — Find catch handler via exception table scan
int FUN_0204769c(u32 *param_1, u32 *param_2)
{
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    int iVar5;
    u32 *puVar6;
    u32 *puVar7;
    int local_98;
    u32 local_94;
    u32 local_90;
    int local_8c;
    u32 local_88;
    u32 local_84;
    u32 local_80;
    u32 local_7c;
    u32 local_78;
    u32 local_74;
    u32 local_70;
    u32 local_6c;
    u32 local_68;
    int local_64;
    u32 local_60[21];

    local_94 = *param_2;
    local_90 = param_2[1];
    puVar6 = local_60;
    local_8c = param_2[2];
    puVar7 = param_1 + 7;
    local_88 = param_2[3];
    iVar5 = 5;
    local_84 = param_2[4];
    local_80 = param_2[5];
    local_7c = *param_1;
    local_78 = param_1[1];
    local_74 = param_1[2];
    local_70 = param_1[3];
    local_6c = param_1[4];
    local_68 = param_1[5];
    local_64 = param_1[6];
    do {
        uVar1 = *puVar7;
        uVar2 = puVar7[1];
        uVar3 = puVar7[2];
        uVar4 = puVar7[3];
        puVar7 = puVar7 + 4;
        *puVar6 = uVar1;
        puVar6[1] = uVar2;
        puVar6[2] = uVar3;
        puVar6[3] = uVar4;
        puVar6 = puVar6 + 4;
        iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    *puVar6 = *puVar7;
    uVar1 = (u32)FUN_02048414(&local_94);
    do {
        switch (uVar1) {
        case 0:
            break;
        case 1:
        default:
        switchD_default:
            FUN_02048880();
        LAB_return:
            FUN_02047180((char *)(local_8c + 1), &local_98);
            *param_1 = *(u32 *)(local_64 + local_98 + 4);
            param_1[1] = *(u32 *)(local_64 + local_98);
            param_1[2] = 0;
            param_1[3] = local_64 + local_98;
            return 0;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 0xb:
            break;
        case 0xc:
            break;
        case 0xd:
            goto LAB_return;
        case 0xe:
            goto switchD_default;
        case 0xf:
            break;
        case 0x10:
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        }
        uVar1 = (u32)FUN_02048078(&local_94);
    } while (1);
}

// FUN_020477f0 @ 0x020477F0 (2172 bytes) — Execute unwind actions (destructor dispatch)
void FUN_020477f0(int param_1, int param_2, u8 *param_3)
{
    u8 bVar1;
    u8 bVar2;
    short sVar3;
    u32 uVar4;
    u32 uVar5;
    int *piVar6;
    u32 *puVar7;
    int iVar8;
    u8 *pbVar9;
    void (*pcVar10)(...);
    int iVar11;
    int local_c8;
    int local_c4;
    int local_c0;
    int local_bc;
    int local_b8;
    int local_b4;
    int local_b0;
    int local_ac;
    int local_a8;
    int local_a4;
    int local_a0;
    int local_9c;
    int local_98;
    int local_94;
    int local_90;
    int local_8c;
    int local_88;
    int local_84;
    int local_80;
    int local_7c;
    int local_78;
    int local_74;
    u8 auStack_70[4];
    u8 auStack_6c[4];
    int local_68;
    int iStack_64;
    u8 auStack_60[4];
    u8 auStack_5c[4];
    int local_58;
    int local_54;
    int local_50;
    int local_4c;
    int local_48;
    int local_44;
    int local_40;
    int local_3c;
    int local_38;
    int local_34;
    int local_30;
    u8 auStack_2c[8];

LAB_02047810:
    pbVar9 = *(u8 **)(param_2 + 8);
    if (pbVar9 == (u8 *)0x0) goto code_r0x0204781c;
    goto LAB_02047858;
code_r0x0204781c:
    uVar4 = (u32)FUN_02046f98(param_1, param_2);
    FUN_02048430(uVar4, param_2);
    if (*(int *)(param_2 + 4) == 0) {
        FUN_02048880();
    }
    FUN_02046ef0(param_1, param_2);
    pbVar9 = *(u8 **)(param_2 + 8);
    if (pbVar9 == (u8 *)0x0) goto LAB_02047810;
LAB_02047858:
    bVar1 = *pbVar9;
    switch (bVar1 & 0x1f) {
    case 0:
    default:
    switchD_default:
        FUN_02048880();
        break;
    case 1:
        FUN_02047180((char *)(pbVar9 + 1), &local_c8);
        *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + local_c8;
        break;
    case 2:
        puVar7 = (u32 *)FUN_02047180((char *)(pbVar9 + 1), &local_c4);
        (*(void (*)(...))*puVar7)(*(int *)(param_1 + 0x18) + local_c4, 0xffffffff);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 3:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_c0);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_bc);
        if ((bVar1 & 0x40) == 0) {
            uVar5 = (u32)*(u8 *)(*(int *)(param_1 + 0x18) + local_c0);
        }
        else {
            uVar5 = *(u32 *)(param_1 + local_c0 * 4 + 0x1c) & 0xff;
        }
        if (uVar5 != 0) {
            (*(void (*)(...))*puVar7)(*(int *)(param_1 + 0x18) + local_bc, 0xffffffff);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 4:
        puVar7 = (u32 *)FUN_02047180((char *)(pbVar9 + 1), &local_b8);
        if ((bVar1 & 0x20) == 0) {
            uVar4 = *(u32 *)(*(int *)(param_1 + 0x18) + local_b8);
        }
        else {
            uVar4 = *(u32 *)(param_1 + local_b8 * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(uVar4, 0xffffffff);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 5:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_b4);
        uVar4 = (u32)FUN_020470d8(uVar4, &local_b0);
        puVar7 = (u32 *)FUN_020470d8(uVar4, &local_ac);
        pcVar10 = (void (*)(...))*puVar7;
        iVar8 = local_b0 * local_ac + *(int *)(param_1 + 0x18) + local_b4;
        for (iVar11 = local_b0; iVar11 != 0; iVar11 = iVar11 + -1) {
            iVar8 = iVar8 - local_ac;
            pcVar10(iVar8, 0xffffffff);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 6:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_a8);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_a4);
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_a8);
        }
        else {
            iVar11 = *(int *)(param_1 + local_a8 * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(iVar11 + local_a4, 0);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 7:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_a0);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_9c);
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_a0);
        }
        else {
            iVar11 = *(int *)(param_1 + local_a0 * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(iVar11 + local_9c, 0xffffffff);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 8:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_98);
        uVar4 = (u32)FUN_02047180((char *)uVar4, &local_94);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_90);
        if ((bVar1 & 0x40) == 0) {
            sVar3 = *(short *)(*(int *)(param_1 + 0x18) + local_98);
        }
        else {
            sVar3 = (short)*(u32 *)(param_1 + local_98 * 4 + 0x1c);
        }
        if (sVar3 != 0) {
            if ((bVar1 & 0x20) == 0) {
                iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_94);
            }
            else {
                iVar11 = *(int *)(param_1 + local_94 * 4 + 0x1c);
            }
            (*(void (*)(...))*puVar7)(iVar11 + local_90, 0xffffffff);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 9:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_8c);
        uVar4 = (u32)FUN_02047180((char *)uVar4, &local_88);
        uVar4 = (u32)FUN_020470d8(uVar4, &local_84);
        puVar7 = (u32 *)FUN_020470d8(uVar4, &local_80);
        pcVar10 = (void (*)(...))*puVar7;
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_8c);
        }
        else {
            iVar11 = *(int *)(param_1 + local_8c * 4 + 0x1c);
        }
        iVar8 = local_84 * local_80 + iVar11 + local_88;
        for (iVar11 = local_84; iVar11 != 0; iVar11 = iVar11 + -1) {
            iVar8 = iVar8 - local_80;
            pcVar10(iVar8, 0xffffffff);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 10:
        puVar7 = (u32 *)FUN_02047180((char *)(pbVar9 + 1), &local_7c);
        if ((bVar1 & 0x20) == 0) {
            uVar4 = *(u32 *)(*(int *)(param_1 + 0x18) + local_7c);
        }
        else {
            uVar4 = *(u32 *)(param_1 + local_7c * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(uVar4);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 0xb:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_78);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_74);
        if ((bVar1 & 0x40) == 0) {
            uVar5 = (u32)*(u8 *)(*(int *)(param_1 + 0x18) + local_78);
        }
        else {
            uVar5 = *(u32 *)(param_1 + local_78 * 4 + 0x1c) & 0xff;
        }
        if (uVar5 != 0) {
            if ((bVar1 & 0x20) == 0) {
                uVar4 = *(u32 *)(*(int *)(param_1 + 0x18) + local_74);
            }
            else {
                uVar4 = *(u32 *)(param_1 + local_74 * 4 + 0x1c);
            }
            (*(void (*)(...))*puVar7)(uVar4);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 0xc:
        if (param_3 == pbVar9) {
            return;
        }
        uVar4 = (u32)FUN_020470d8(pbVar9 + 5, auStack_70);
        uVar4 = (u32)FUN_02047180((char *)uVar4, auStack_6c);
        *(u32 *)(param_2 + 8) = uVar4;
        break;
    case 0xd:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_68);
        piVar6 = (int *)(*(int *)(param_1 + 0x18) + local_68);
        pcVar10 = (void (*)(...))piVar6[2];
        if (pcVar10 != (void (*)(...))0x0) {
            iVar11 = *piVar6;
            if (*(int *)(param_1 + 4) == iVar11) {
                *(void (**)(...))(param_1 + 8) = pcVar10;
            }
            else {
                pcVar10(iVar11, 0xffffffff);
            }
        }
        *(u32 *)(param_2 + 8) = uVar4;
        break;
    case 0xe:
        goto switchD_default;
    case 0xf:
        if (param_3 == pbVar9) {
            return;
        }
        uVar4 = (u32)FUN_020470d8(pbVar9 + 1, &iStack_64);
        uVar4 = (u32)FUN_020470d8(uVar4, auStack_60);
        iVar11 = (int)FUN_02047180((char *)uVar4, auStack_5c);
        *(int *)(param_2 + 8) = iVar11 + iStack_64 * 4;
        break;
    case 0x10:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_58);
        piVar6 = (int *)FUN_02047180((char *)uVar4, &local_54);
        local_4c = *piVar6;
        puVar7 = (u32 *)FUN_02047180((char *)(piVar6 + 1), &local_50);
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_58);
        }
        else {
            iVar11 = *(int *)(param_1 + local_58 * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(iVar11 + local_54, local_4c + local_50);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 0x11:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_48);
        pbVar9 = (u8 *)FUN_02047180((char *)uVar4, &local_40);
        bVar2 = *pbVar9;
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_44);
        puVar7 = (u32 *)FUN_02047180((char *)uVar4, &local_3c);
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_48);
        }
        else {
            iVar11 = *(int *)(param_1 + local_48 * 4 + 0x1c);
        }
        if ((bVar2 & 0x20) == 0) {
            iVar8 = *(int *)(*(int *)(param_1 + 0x18) + local_44);
        }
        else {
            iVar8 = *(int *)(param_1 + local_44 * 4 + 0x1c);
        }
        (*(void (*)(...))*puVar7)(iVar11 + local_40, iVar8 + local_3c);
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 0x12:
        pbVar9 = (u8 *)FUN_02047180((char *)(pbVar9 + 1), &local_38);
        bVar2 = *pbVar9;
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), &local_34);
        puVar7 = (u32 *)FUN_020470d8(uVar4, &local_30);
        pcVar10 = (void (*)(...))*puVar7;
        if ((bVar1 & 0x20) == 0) {
            iVar11 = *(int *)(*(int *)(param_1 + 0x18) + local_38);
        }
        else {
            iVar11 = *(int *)(param_1 + local_38 * 4 + 0x1c);
        }
        if ((bVar2 & 0x20) == 0) {
            iVar8 = *(int *)(*(int *)(param_1 + 0x18) + local_34);
        }
        else {
            iVar8 = *(int *)(param_1 + local_34 * 4 + 0x1c);
        }
        iVar11 = iVar11 + iVar8;
        for (iVar8 = FUN_020466bc(iVar8, local_30); iVar8 != 0; iVar8 = iVar8 + -1) {
            iVar11 = iVar11 - local_30;
            pcVar10(iVar11, 0xffffffff);
        }
        *(u32 **)(param_2 + 8) = puVar7 + 1;
        break;
    case 0x13:
        uVar4 = (u32)FUN_02047180((char *)(pbVar9 + 1), auStack_2c);
        *(u32 *)(param_2 + 8) = uVar4;
        break;
    }
    if ((bVar1 & 0x80) != 0) {
        *(u32 *)(param_2 + 8) = 0;
    }
    goto LAB_02047810;
}

// FUN_02048078 @ 0x02048078 (924 bytes) — Advance to next exception table action
u32 FUN_02048078(int param_1)
{
    u32 uVar1;
    u32 *puVar2;
    int iVar3;
    u8 *pbVar4;
    u8 auStack_b0[4];
    u8 auStack_ac[4];
    u8 auStack_a8[4];
    u8 auStack_a4[4];
    u8 auStack_a0[4];
    u8 auStack_9c[4];
    u8 auStack_98[4];
    u8 auStack_94[4];
    u8 auStack_90[4];
    u8 auStack_8c[4];
    u8 auStack_88[4];
    u8 auStack_84[4];
    u8 auStack_80[4];
    u8 auStack_7c[4];
    u8 auStack_78[4];
    u8 auStack_74[4];
    u8 auStack_70[4];
    u8 auStack_6c[4];
    u8 auStack_68[4];
    u8 auStack_64[4];
    u8 auStack_60[4];
    u8 auStack_5c[4];
    u8 auStack_58[4];
    u8 auStack_54[4];
    int local_50;
    u8 auStack_4c[4];
    u8 auStack_48[4];
    u8 auStack_44[4];
    u8 auStack_40[4];
    u8 auStack_3c[4];
    u32 local_38;
    u8 auStack_34[4];
    u8 auStack_30[4];
    u8 auStack_2c[4];
    u8 auStack_28[4];
    u8 auStack_24[4];
    u8 auStack_20[4];
    u8 auStack_1c[4];
    u8 auStack_18[4];
    int iStack_14;

    do {
        pbVar4 = *(u8 **)(param_1 + 8);
        if ((pbVar4 != (u8 *)0x0) && ((*pbVar4 & 0x80) == 0)) {
            switch (*pbVar4 & 0x1f) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                iVar3 = (int)FUN_02047180((char *)(pbVar4 + 1), auStack_b0);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 3:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_ac);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_a8);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 4:
                iVar3 = (int)FUN_02047180((char *)(pbVar4 + 1), auStack_a4);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 5:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_a0);
                uVar1 = (u32)FUN_020470d8(uVar1, auStack_9c);
                iVar3 = (int)FUN_020470d8(uVar1, auStack_98);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 6:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_94);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_90);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 7:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_8c);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_88);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 8:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_84);
                uVar1 = (u32)FUN_02047180((char *)uVar1, auStack_80);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_7c);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 9:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_78);
                uVar1 = (u32)FUN_02047180((char *)uVar1, auStack_74);
                uVar1 = (u32)FUN_020470d8(uVar1, auStack_70);
                iVar3 = (int)FUN_020470d8(uVar1, auStack_6c);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 10:
                iVar3 = (int)FUN_02047180((char *)(pbVar4 + 1), auStack_68);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 0xb:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_64);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_60);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 0xc:
                uVar1 = (u32)FUN_020470d8(pbVar4 + 5, auStack_5c);
                uVar1 = (u32)FUN_02047180((char *)uVar1, auStack_58);
                *(u32 *)(param_1 + 8) = uVar1;
                goto LAB_020483a8;
            case 0xd:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_54);
                *(u32 *)(param_1 + 8) = uVar1;
                goto LAB_020483a8;
            case 0xe:
                break;
            case 0xf:
                uVar1 = (u32)FUN_020470d8(pbVar4 + 1, &local_50);
                uVar1 = (u32)FUN_020470d8(uVar1, auStack_4c);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_48);
                *(int *)(param_1 + 8) = iVar3 + local_50 * 4;
                goto LAB_020483a8;
            case 0x10:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_44);
                puVar2 = (u32 *)FUN_02047180((char *)uVar1, auStack_40);
                local_38 = *puVar2;
                iVar3 = (int)FUN_02047180((char *)(puVar2 + 1), auStack_3c);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 0x11:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_34);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_2c);
                uVar1 = (u32)FUN_02047180((char *)(iVar3 + 1), auStack_30);
                iVar3 = (int)FUN_02047180((char *)uVar1, auStack_28);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 0x12:
                iVar3 = (int)FUN_02047180((char *)(pbVar4 + 1), auStack_24);
                uVar1 = (u32)FUN_02047180((char *)(iVar3 + 1), auStack_20);
                iVar3 = (int)FUN_020470d8(uVar1, auStack_1c);
                *(int *)(param_1 + 8) = iVar3 + 4;
                goto LAB_020483a8;
            case 0x13:
                uVar1 = (u32)FUN_02047180((char *)(pbVar4 + 1), auStack_18);
                *(u32 *)(param_1 + 8) = uVar1;
                goto LAB_020483a8;
            }
            FUN_02048880();
            break;
        }
        uVar1 = (u32)FUN_02046f98(param_1 + 0x18, param_1);
        FUN_02048430(uVar1, param_1);
        if (*(int *)(param_1 + 4) == 0) {
            FUN_02048880();
        }
        FUN_02046ef0(param_1 + 0x18, param_1);
    } while (*(int *)(param_1 + 8) == 0);
LAB_020483a8:
    if ((*(u8 **)(param_1 + 8) != (u8 *)0x0) && (**(u8 **)(param_1 + 8) & 0x1f) != 1) {
        return 0;
    }
    do {
        FUN_02047180((char *)(*(int *)(param_1 + 8) + 1), &iStack_14);
        *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + iStack_14;
    } while ((**(u8 **)(param_1 + 8) & 0x1f) == 1);
    return 0;
}

// FUN_02048430 @ 0x02048430 (288 bytes) — Lookup exception table entry by address
void FUN_02048430(int param_1, int *param_2)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;
    u32 uVar4;
    void (*UNRECOVERED_JUMPTABLE)(...);
    int iStack_28;
    int iStack_24;
    int iStack_20;

    param_2[1] = 0;
    param_2[2] = 0;
    iVar1 = (int)FUN_02046ed0(param_2, param_1);
    if (iVar1 == 0) {
        return;
    }
    iVar1 = param_2[4] - param_2[3];
    piVar2 = (int *)FUN_02048554(param_2[3],
                                 (int)((long long)DAT_02048550 * (long long)iVar1 >> 0x21) -
                                 (iVar1 >> 0x1f), param_1);
    if (piVar2 != (int *)0x0) {
        if ((piVar2[1] & 1U) == 0) {
            param_2[1] = piVar2[2];
        }
        else {
            param_2[1] = (int)(piVar2 + 2);
        }
        *param_2 = *piVar2;
        iVar1 = *piVar2;
        uVar3 = (u32)FUN_02046e98(param_2[1]);
        uVar4 = 0;
        while (1) {
            uVar3 = (u32)FUN_020470d8(uVar3, &iStack_28);
            if (iStack_28 == 0) {
                (*UNRECOVERED_JUMPTABLE)();
                return;
            }
            uVar3 = (u32)FUN_020470d8(uVar3, &iStack_24);
            uVar3 = (u32)FUN_020470d8(uVar3, &iStack_20);
            if ((u32)(param_1 - iVar1) < uVar4 + iStack_28) break;
            uVar4 = uVar4 + iStack_28 + iStack_24;
            if ((u32)(param_1 - iVar1) <= uVar4) {
                param_2[2] = param_2[1] + iStack_20;
                return;
            }
        }
        (*UNRECOVERED_JUMPTABLE)();
        return;
    }
    (*UNRECOVERED_JUMPTABLE)();
    return;
}

// FUN_02048554 @ 0x02048554 (108 bytes) — Binary search exception table
int FUN_02048554(int param_1, int param_2, u32 param_3)
{
    int iVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;

    param_2 = param_2 + -1;
    iVar2 = 0;
    if (-1 < param_2) {
        do {
            iVar1 = iVar2 + param_2 >> 1;
            uVar3 = *(u32 *)(param_1 + iVar1 * 0xc);
            iVar4 = param_1 + iVar1 * 0xc;
            if (param_3 < uVar3) {
                param_2 = iVar1 + -1;
            }
            else {
                if (param_3 <= uVar3 + (*(u32 *)(iVar4 + 4) & 0xfffffffe)) {
                    return iVar4;
                }
                iVar2 = iVar1 + 1;
            }
        } while (iVar2 <= param_2);
    }
    return 0;
}

// FUN_020485c0 @ 0x020485C0 (84 bytes) — Run atexit destructor chain
void FUN_020485c0(void)
{
    u32 *puVar1;
    u32 *puVar2;

    puVar1 = DAT_02048614;
    puVar2 = (u32 *)*DAT_02048614;
    if (puVar2 == (u32 *)0x0) {
        return;
    }
    do {
        *puVar1 = *puVar2;
        (*(void (*)(...))(puVar2[1]))(puVar2[2], 0xffffffff);
        puVar2 = (u32 *)*puVar1;
    } while (puVar2 != (u32 *)0x0);
    return;
}

// FUN_02048650 @ 0x02048650 (528 bytes) — Match exception type name strings
u32 FUN_02048650(char *param_1, char *param_2, int *param_3)
{
    u8 bVar1;
    char *pcVar2;
    u8 *pbVar3;
    char cVar4;
    char *pcVar5;
    u8 *pbVar6;
    int iVar7;

    *param_3 = 0;
    if (param_2 == (char *)0x0) {
        return 1;
    }
    if (*param_2 == 'P') {
        pcVar5 = param_2 + 1;
        if (*pcVar5 == 'V') {
            pcVar5 = param_2 + 2;
        }
        if (*pcVar5 == 'K') {
            pcVar5 = pcVar5 + 1;
        }
        if ((*pcVar5 == 'v') && ((*param_1 == 'P' || (*param_1 == '*')))) {
            return 1;
        }
    }
    if ((*param_1 == '!') || (*param_1 == '*')) {
        pbVar3 = (u8 *)(param_1 + 1);
        pbVar6 = (u8 *)(param_2 + 1);
        if (*param_1 != *param_2) {
            return 0;
        }
        while (1) {
            while (1) {
                bVar1 = *pbVar3;
                if (bVar1 != *pbVar6) break;
                pbVar3 = pbVar3 + 1;
                pbVar6 = pbVar6 + 1;
                if (bVar1 == 0x21) {
                    iVar7 = 0;
                    bVar1 = *pbVar3;
                    while (bVar1 != 0x21) {
                        bVar1 = *pbVar3;
                        pbVar3 = pbVar3 + 1;
                        iVar7 = iVar7 * 10 + (u32)bVar1 + -0x30;
                        bVar1 = *pbVar3;
                    }
                    *param_3 = iVar7;
                    return 1;
                }
            }
            do {
                bVar1 = *pbVar3;
                pbVar6 = pbVar3 + 1;
                pbVar3 = pbVar3 + 1;
            } while (bVar1 != 0x21);
            do {
                pbVar3 = pbVar6 + 1;
                bVar1 = *pbVar6;
                pbVar6 = pbVar3;
            } while (bVar1 != 0x21);
            if (*pbVar3 == 0) break;
            pbVar6 = (u8 *)(param_2 + 1);
        }
        return 0;
    }
    while (1) {
        cVar4 = *param_1;
        if (((cVar4 != 'P') && (cVar4 != 'R')) || (cVar4 != *param_2)) {
            if (cVar4 == *param_2) {
                do {
                    if (cVar4 == '\0') {
                        return 1;
                    }
                    param_1 = param_1 + 1;
                    cVar4 = *param_1;
                    param_2 = param_2 + 1;
                } while (cVar4 == *param_2);
            }
            return 0;
        }
        pcVar5 = param_1 + 1;
        pcVar2 = param_2 + 1;
        if ((param_2[1] == 'K') && (pcVar2 = param_2 + 2, *pcVar5 == 'K')) {
            pcVar5 = param_1 + 2;
        }
        param_2 = pcVar2;
        param_1 = pcVar5;
        if (*param_1 == 'K') break;
        if (*param_2 == 'V') {
            if (*param_1 == 'V') {
                param_1 = param_1 + 1;
            }
            param_2 = param_2 + 1;
        }
        if (*param_1 == 'V') {
            return 0;
        }
    }
    return 0;
}

// FUN_020488a4 @ 0x020488A4 (12 bytes) — Stub / thunk
void FUN_020488a4(void)
{
    return;
}

// FUN_020488b0 @ 0x020488B0 (72 bytes) — Destruct and free object
void FUN_020488b0(int param_1, u32 param_2, int param_3, int param_4)
{
    if (param_1 != 0) {
        if (param_4 != 0) {
            FUN_02048970(param_1, *(u32 *)(param_1 + -4), param_2, (void (*)(...))*(u32 *)(param_1 + -4));
        }
        thunk_FUN_02029ab8(param_1 - param_3);
        return;
    }
    return;
}

// FUN_020488f8 @ 0x020488F8 (92 bytes) — Iterate array destructors (reverse)
void FUN_020488f8(int param_1, int param_2, int param_3, void (*param_4)(...))
{
    u8 auStack_30[20];
    u8 *local_1c;

    local_1c = auStack_30;
    if (param_4 == (void (*)(...))0x0) {
        return;
    }
    param_1 = param_2 * param_3 + param_1;
    if (param_2 != 0) {
#ifdef HOST_PORT
        {
            extern void nds_call_1arg(u32, uintptr_t);
            u32 fn_addr = (u32)(uintptr_t)param_4;
            do {
                param_1 = param_1 - param_3;
                nds_call_1arg(fn_addr, (uintptr_t)param_1);
                param_2 = param_2 + -1;
            } while (param_2 != 0);
        }
#else
        do {
            param_1 = param_1 - param_3;
            param_4(param_1);
            param_2 = param_2 + -1;
        } while (param_2 != 0);
#endif
        return;
    }
    return;
}

// FUN_02048970 @ 0x02048970 (92 bytes) — Iterate array destructors (reverse, variant)
void FUN_02048970(int param_1, int param_2, int param_3, void (*param_4)(...))
{
    u8 auStack_48[20];
    u8 *local_34;

    local_34 = auStack_48;
    if (param_4 == (void (*)(...))0x0) {
        return;
    }
    param_1 = param_2 * param_3 + param_1;
    if (param_2 != 0) {
#ifdef HOST_PORT
        {
            extern void nds_call_1arg(u32, uintptr_t);
            u32 fn_addr = (u32)(uintptr_t)param_4;
            do {
                param_1 = param_1 - param_3;
                nds_call_1arg(fn_addr, (uintptr_t)param_1);
                param_2 = param_2 + -1;
            } while (param_2 != 0);
        }
#else
        do {
            param_1 = param_1 - param_3;
            param_4(param_1);
            param_2 = param_2 + -1;
        } while (param_2 != 0);
#endif
        return;
    }
    return;
}

// FUN_02048a1c @ 0x02048A1C (156 bytes) — Iterate array constructors (forward)
void FUN_02048a1c(int param_1, int param_2, int param_3, void (*param_4)(...), int param_5)
{
    u8 auStack_38[20];
    u8 *local_24;

    local_24 = auStack_38;
    if (param_4 == (void (*)(...))0x0) {
        return;
    }
#ifdef HOST_PORT
    /* On PC, param_4 may be an NDS code address (e.g. overlay function).
     * Route through fnptr resolver / ARM interpreter instead of direct call. */
    {
        extern void nds_call_1arg(u32, uintptr_t);
        u32 fn_addr = (u32)(uintptr_t)param_4;
        if (param_2 != 0) {
            do {
                nds_call_1arg(fn_addr, (uintptr_t)param_1);
                param_1 = param_1 + param_3;
                param_2 = param_2 + -1;
            } while (param_2 != 0);
        }
        return;
    }
#endif
    if (param_5 != 0) {
        if (param_2 != 0) {
            do {
                param_4(param_1);
                param_1 = param_1 + param_3;
                param_2 = param_2 + -1;
            } while (param_2 != 0);
            return;
        }
        return;
    }
    if (param_2 != 0) {
        do {
            param_4(param_1);
            param_1 = param_1 + param_3;
            param_2 = param_2 + -1;
        } while (param_2 != 0);
        return;
    }
    return;
}

// FUN_02048ae8 @ 0x02048AE8 (160 bytes) — Construct array with header
int FUN_02048ae8(int param_1, int param_2, int param_3, u32 param_4, void (*param_5)(...), int param_6)
{
    int iVar1;
    u8 auStack_40[20];
    u8 *local_2c;

    local_2c = auStack_40;
    if (param_4 != 0) {
        *(int *)(param_1 + param_4 + -4) = param_2;
        if (7 < param_4) {
            *(int *)(param_1 + param_4 + -8) = param_3;
        }
    }
    if (param_5 != (void (*)(...))0x0) {
        iVar1 = param_1 + param_4;
#ifdef HOST_PORT
        {
            extern void nds_call_1arg(u32, uintptr_t);
            u32 fn_addr = (u32)(uintptr_t)param_5;
            for (; param_2 != 0; param_2 = param_2 + -1) {
                nds_call_1arg(fn_addr, (uintptr_t)iVar1);
                iVar1 = iVar1 + param_3;
            }
        }
#else
        if (param_6 == 0) {
            for (; param_2 != 0; param_2 = param_2 + -1) {
                param_5(iVar1);
                iVar1 = iVar1 + param_3;
            }
        }
        else {
            for (; param_2 != 0; param_2 = param_2 + -1) {
                param_5(iVar1);
                iVar1 = iVar1 + param_3;
            }
        }
#endif
    }
    return param_1 + param_4;
}

// FUN_02048bb0 @ 0x02048BB0 (80 bytes) — Destruct range of array elements
void FUN_02048bb0(u32 param_1, u32 param_2, int param_3, void (*param_4)(...))
{
    u8 auStack_30[20];
    u8 *local_1c;

    local_1c = auStack_30;
    if (param_1 < param_2) {
#ifdef HOST_PORT
        {
            extern void nds_call_1arg(u32, uintptr_t);
            u32 fn_addr = (u32)(uintptr_t)param_4;
            do {
                param_2 = param_2 - param_3;
                nds_call_1arg(fn_addr, (uintptr_t)param_2);
            } while (param_1 < param_2);
        }
#else
        do {
            param_2 = param_2 - param_3;
            param_4(param_2);
        } while (param_1 < param_2);
#endif
        return;
    }
    return;
}

// FUN_02048c1c @ 0x02048C1C (40 bytes) — Virtual destructor wrapper A
u32 *FUN_02048c1c(u32 *param_1)
{
    *param_1 = DAT_02048c44;
    FUN_02048c6c();
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_02048c90 @ 0x02048C90 (40 bytes) — Virtual destructor wrapper B
u32 *FUN_02048c90(u32 *param_1)
{
    *param_1 = DAT_02048cb8;
    FUN_02048ce0();
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_02048414 @ 0x02048414 (28 bytes) — Get action byte from exception record
u8 FUN_02048414(int *param_1)
{
    u8 bVar1;

    if (*(u8 **)(param_1 + 2) == (u8 *)0x0) {
        bVar1 = 0;
    }
    else {
        bVar1 = **(u8 **)(param_1 + 2) & 0x1f;
    }
    return bVar1;
}
