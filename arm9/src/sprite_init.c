/**
 * Sprite Initialization and Configuration
 * 
 * Handles sprite object construction, matrix deinterleaving,
 * animation controller setup, and VRAM DMA configuration.
 *
 * Decompiled from 0x0201058c-0x0201220c region (24 functions)
 */
#include "types.h"

/* Forward declarations */
extern void FUN_0200cee4(void *);
extern void FUN_0200cf90(void);
extern void FUN_0202cc10(void *, u32, u32);
extern void FUN_02034224(void *);
extern void FUN_0202a068(void);
extern void thunk_FUN_02029ab8(void *);
extern void FUN_0203b7f0(u32, u32, u32);
extern u32 FUN_02029c1c(u32, u32, u32, u32);

/* Static data references */
static u32 DAT_020105c4;  /* vtable for sprite type A */
static u32 DAT_020105f8;  /* vtable for sprite type B */
static u32 DAT_02010664;  /* vtable for sprite type C */
static u32 DAT_02010668;  /* default anim speed */
static u32 DAT_02011b84;  /* vtable for anim controller */

// FUN_0201058c @ 0x0201058c (56 bytes) — Construct sprite type A
int *FUN_0201058c(int *param_1)
{
    *param_1 = DAT_020105c4;
    (*(void (*)(void))(*(int *)(*param_1 + 0xc0)))();
    FUN_0200cee4(param_1);
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_020105c8 @ 0x020105c8 (48 bytes) — Construct sprite type B
int *FUN_020105c8(int *param_1)
{
    *param_1 = DAT_020105f8;
    (*(void (*)(void))(*(int *)(*param_1 + 0xc0)))();
    FUN_0200cee4(param_1);
    return param_1;
}

// FUN_020105fc @ 0x020105fc (104 bytes) — Construct sprite type C with animation defaults
u32 *FUN_020105fc(u32 *param_1)
{
    u32 uVar1;

    FUN_0200cf90();
    *param_1 = DAT_02010664;
    FUN_0202cc10(param_1 + 0x4c, 0, (int)param_1 + (0x1b4 - (int)(param_1 + 0x4c)));
    uVar1 = DAT_02010668;
    *(u16 *)((int)param_1 + 0x162) = (*(u16 *)((int)param_1 + 0x162) & 0xffe0) | 0x1f;
    *(s16 *)(param_1 + 0x59) = (s16)uVar1;
    *(u16 *)(param_1 + 0x5b) = 0x10;
    *(u16 *)((int)param_1 + 0x16e) = 1;
    FUN_02034224(param_1 + 0x5d);
    return param_1;
}

// FUN_0201066c @ 0x0201066c (136 bytes) — Deinterleave 8x8 matrix to strided output
void FUN_0201066c(u32 *param_1, u32 param_2, u32 *param_3, int param_4)
{
    u32 uVar1;
    u32 uVar2, uVar4, uVar5, uVar6, uVar7, uVar8, uVar9, uVar10;
    int iVar3;
    u32 *puVar11;

    uVar1 = param_2 & 0xffff;
    puVar11 = param_3;
    do {
        do {
            uVar2 = *param_1;
            uVar4 = param_1[1];
            uVar5 = param_1[2];
            uVar6 = param_1[3];
            uVar7 = param_1[4];
            uVar8 = param_1[5];
            uVar9 = param_1[6];
            uVar10 = param_1[7];
            param_1 = param_1 + 8;
            *param_3 = uVar2;
            param_3[param_4] = uVar4;
            param_3[param_4 * 2] = uVar5;
            param_3[param_4 * 4] = uVar7;
            param_3[param_4 * 5] = uVar8;
            (param_3 + param_4 * 5)[param_4 * 2] = uVar10;
            param_3[param_4 * 3] = uVar6;
            param_3[param_4 * 6] = uVar9;
            param_3 = param_3 + 1;
            uVar1 = uVar1 - 1;
        } while (0 < (int)uVar1);
        uVar1 = param_2 & 0xffff;
        param_3 = puVar11 + param_4 * 8;
        iVar3 = (param_2 >> 0x10) - 1;
        param_2 = (param_2 & 0xffff) | (u32)(iVar3 * 0x10000);
        puVar11 = param_3;
    } while (0 < iVar3);
    return;
}

// FUN_02011b5c @ 0x02011b5c (40 bytes) — Construct animation controller
u32 *FUN_02011b5c(u32 *param_1)
{
    *param_1 = DAT_02011b84;
    FUN_0202a068();
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_02012054 @ 0x02012054 (48 bytes) — DMA copy sprite palette to VRAM
void FUN_02012054(int param_1)
{
    FUN_0203b7f0(*(u32 *)(param_1 + 0x90), *(u32 *)(param_1 + 0x8c), 0x200);
    *(u8 *)(*(int *)(param_1 + 0x2c) + 0xfd) = *(u8 *)(*(int *)(param_1 + 0x2c) + 0xfd) | 2;
    return;
}

/* ===== Additional function pointer type ===== */
typedef u32 code(...);

/* Forward declarations for additional external functions */
extern void FUN_0203a04c(u32, u32);
extern void FUN_0203867c(u32, u32, u32);
extern void FUN_02038614(u32, u32, u32);
extern void FUN_0202a20c(void);
extern void FUN_0203b5b4(int, u32, u32, u32);
extern void FUN_020369c4(u32, u32, u32, u32, u32);
extern void FUN_020383b4(u32, u32, u32);
extern void FUN_020382e4(u32, u32, u32);
extern void FUN_02038214(u32, u32, u32);
extern void FUN_0203841c(u32, u32, u32);
extern void FUN_0203827c(u32, u32, u32);
extern void FUN_0203834c(u32, u32, u32);
extern int FUN_02018abc(u32, u32, u32, u32);
extern u32 *FUN_02010bfc(u32 *);
extern int FUN_020464b0(int, int);
extern int FUN_020091ec(int *, u32);
extern void FUN_0200937c(int *, u32);
extern void FUN_02007a74(u32, u32, u32, u32);
extern void FUN_0203b7c0(int, int, int);
extern void FUN_0202da48(int, short, u32, u32);

/* Static data references for additional functions */
static u32 DAT_020108b0;
static u32 DAT_020108b4;
static u32 DAT_02010a68;
static u32 DAT_02010a6c;
static u32 DAT_02010be8;
static u16 *DAT_02010bec;
static int *DAT_02010bf0;
static int *DAT_02010bf4;
static int *DAT_02010bf8;
static u32 DAT_02010cb4;
static u32 *DAT_0201113c;
static u32 DAT_02011140;
static u32 DAT_02011144;
static u32 DAT_02011148;
static u32 DAT_020111e8;
static u32 DAT_020111ec;
static u32 DAT_02011b58;
static u32 DAT_02011c1c;
static u32 DAT_02012050;

// FUN_02010784 @ 0x02010784 (300 bytes) — Bresenham line drawing
void FUN_02010784(u32 param_1, int param_2, int param_3, int param_4, int param_5)
{
    code *pcVar1;
    code *extraout_r1 = (code *)0;
    int iVar2;
    int iVar3;
    int iVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;
    int iVar8;
    int iVar9;

    iVar4 = param_2 - param_4;
    if (iVar4 < 0) {
        iVar4 = -iVar4;
    }
    uVar5 = iVar4 + 1;
    iVar4 = param_3 - param_5;
    if (iVar4 < 0) {
        iVar4 = -iVar4;
    }
    uVar7 = iVar4 + 1;
    if ((int)uVar7 < (int)uVar5) {
        iVar4 = param_2;
        iVar8 = param_3;
        if (-1 < param_2 - param_4) {
            iVar4 = param_4;
            param_4 = param_2;
            iVar8 = param_5;
            param_5 = param_3;
        }
        iVar9 = 1;
        if (param_5 < iVar8) {
            iVar9 = -1;
        }
        iVar2 = 0xff;
        iVar3 = 0xbf;
        pcVar1 = (code *)(unsigned long)DAT_020108b4;
        uVar6 = uVar7;
    }
    else {
        iVar4 = param_3;
        iVar9 = param_5;
        iVar8 = param_2;
        iVar2 = param_4;
        if (-1 < param_3 - param_5) {
            iVar4 = param_5;
            iVar9 = param_3;
            iVar8 = param_4;
            iVar2 = param_2;
        }
        param_4 = iVar9;
        iVar9 = 1;
        if (iVar2 < iVar8) {
            iVar9 = -1;
        }
        iVar2 = 0xbf;
        iVar3 = 0xff;
        pcVar1 = (code *)(unsigned long)DAT_020108b0;
        uVar6 = uVar5;
        uVar5 = uVar7;
    }
    uVar7 = uVar6 >> 1;
    if (iVar8 < 0 || iVar3 < iVar8) {
        pcVar1 = (code *)((u32)(unsigned long)pcVar1 | 1);
    }
    for (; iVar4 <= param_4; iVar4 = iVar4 + 1) {
        if ((((u32)(unsigned long)pcVar1 & 1) == 0) && (-1 < iVar4 && iVar4 <= iVar2)) {
            (*pcVar1)();
            pcVar1 = extraout_r1;
        }
        uVar7 = uVar7 + uVar6;
        if ((int)uVar5 < (int)uVar7) {
            uVar7 = uVar7 - uVar5;
            iVar8 = iVar8 + iVar9;
            pcVar1 = (code *)((u32)(unsigned long)pcVar1 & 0xfffffffe);
            if (iVar8 < 0 || iVar3 < iVar8) {
                pcVar1 = (code *)((u32)(unsigned long)pcVar1 | 1);
            }
        }
    }
    return;
}

// FUN_02010958 @ 0x02010958 (160 bytes) — 2x2 matrix multiply
void FUN_02010958(short *param_1, short *param_2, short *param_3)
{
    short sVar1;
    short sVar2;
    short *in_r12;
    short local_18[6];

    sVar1 = param_1[1];
    if (param_3 == param_2) {
        in_r12 = local_18;
    }
    sVar2 = *param_1;
    if (param_3 != param_2) {
        in_r12 = param_3;
    }
    *in_r12 = (short)((u32)((int)sVar2 * (int)*param_2 + (int)sVar1 * (int)param_2[2]) >> 8);
    in_r12[1] = (short)((u32)((int)sVar2 * (int)param_2[1] + (int)sVar1 * (int)param_2[3]) >> 8);
    sVar1 = param_1[3];
    sVar2 = param_1[2];
    in_r12[2] = (short)((u32)((int)sVar2 * (int)*param_2 + (int)sVar1 * (int)param_2[2]) >> 8);
    in_r12[3] = (short)((u32)((int)sVar2 * (int)param_2[1] + (int)sVar1 * (int)param_2[3]) >> 8);
    *(u32 *)param_3 = *(u32 *)in_r12;
    *(u32 *)(param_3 + 2) = *(u32 *)(in_r12 + 2);
    return;
}

// FUN_020109f8 @ 0x020109f8 (112 bytes) — Compute rotation matrix from angle
void FUN_020109f8(u16 *param_1, int param_2, int param_3, u32 param_4)
{
    int iVar1;
    int iVar2;

    iVar2 = (int)(param_4 & DAT_02010a68) >> 4;
    iVar1 = (int)*(short *)(DAT_02010a6c + (iVar2 * 2 + 1) * 2);
    iVar2 = (int)*(short *)(DAT_02010a6c + iVar2 * 4);
    *param_1 = (short)(param_2 * iVar1 >> 0xc);
    param_1[1] = (short)(-param_3 * iVar2 >> 0xc);
    param_1[2] = (short)(param_2 * iVar2 >> 0xc);
    param_1[3] = (short)(param_3 * iVar1 >> 0xc);
    return;
}

// FUN_02010a70 @ 0x02010a70 (376 bytes) — Invert 2x2 matrix
u32 FUN_02010a70(short *param_1, u16 *param_2)
{
    int *piVar1;
    int *piVar2;
    short sVar3;
    u16 uVar4;
    int iVar5;
    int iVar6;

    piVar1 = DAT_02010bf0;
    iVar5 = (int)param_1[3];
    iVar6 = *param_1 * iVar5 - (int)param_1[1] * (int)param_1[2];
    if (iVar6 == 0) {
        return 0xffffffff;
    }
    iVar6 = iVar6 + 0x80 >> 8;
    if (iVar6 == 0) {
        if (iVar5 == 0) {
            uVar4 = 0;
        }
        else if (iVar5 < 0) {
            uVar4 = 0x8000;
        }
        else {
            uVar4 = (u16)DAT_02010be8;
        }
        *param_2 = uVar4;
        if (param_1[1] == 0) {
            uVar4 = 0;
        }
        else if (param_1[1] < 0) {
            uVar4 = 0x8000;
        }
        else {
            uVar4 = (u16)DAT_02010be8;
        }
        param_2[1] = uVar4;
        if (param_1[2] == 0) {
            uVar4 = 0;
        }
        else if (param_1[2] < 0) {
            uVar4 = 0x8000;
        }
        else {
            uVar4 = (u16)DAT_02010be8;
        }
        param_2[2] = uVar4;
        if (*param_1 == 0) {
            uVar4 = 0;
        }
        else if (*param_1 < 0) {
            uVar4 = 0x8000;
        }
        else {
            uVar4 = (u16)DAT_02010be8;
        }
        param_2[3] = uVar4;
        return 0;
    }
    *DAT_02010bec = 0;
    piVar2 = DAT_02010bf4;
    *piVar1 = iVar5 << 8;
    *piVar2 = iVar6;
    piVar2[1] = 0;
    sVar3 = (short)FUN_020464b0((int)param_1[1] << 8, iVar6);
    param_2[1] = (u16)-sVar3;
    sVar3 = (short)FUN_020464b0((int)param_1[2] << 8, iVar6);
    param_2[2] = (u16)-sVar3;
    uVar4 = (u16)FUN_020464b0((int)*param_1 << 8, iVar6);
    param_2[3] = uVar4;
    do {
    } while ((*DAT_02010bec & 0x8000) != 0);
    do {
    } while ((*DAT_02010bec & 0x8000) != 0);
    *param_2 = (short)*DAT_02010bf8;
    return 0;
}

// FUN_02010c1c @ 0x02010c1c (152 bytes) — Add animation node to linked list
int FUN_02010c1c(u32 *param_1, u32 param_2, int param_3, int param_4, u32 param_5,
                u32 param_6)
{
    u32 uVar1;
    int iVar2;
    u32 *puVar3;
    int iVar4;
    int iVar5;

    iVar2 = FUN_02018abc(*param_1, param_2, param_5, param_6);
    puVar3 = FUN_02010bfc(param_1);
    FUN_0202cc10(puVar3 + 1, 0, 0x24);
    iVar4 = 0;
    if (0 < param_4) {
        do {
            iVar5 = iVar4 + 1;
            puVar3[iVar4 + 6] = *(u32 *)(param_3 + iVar4 * 4);
            iVar4 = iVar5;
        } while (iVar5 < param_4);
    }
    *puVar3 = param_1[4];
    param_1[4] = (u32)(unsigned long)puVar3;
    *(u32 **)(iVar2 + 0x2c) = puVar3;
    uVar1 = DAT_02010cb4;
    *(u32 *)(iVar2 + 0x28) = param_5;
    *(u32 *)(iVar2 + 0x30) = uVar1;
    *(u32 *)(iVar2 + 0x34) = 0;
    return iVar2;
}

// FUN_02010e00 @ 0x02010e00 (828 bytes) — OAM sprite render dispatch
void FUN_02010e00(u32 param_1, int *param_2, int param_3)
{
    u8 uVar1;
    u8 bVar2;
    u16 uVar3;
    int iVar4;
    u32 uVar5;
    u32 uVar6;
    int iVar7;
    int iVar8;
    u32 uVar9;
    code *pcVar10;
    int iVar11;
    int unaff_r6 = 0;
    u32 uVar12;
    int *piVar13;
    u32 unaff_r11 = 0;
    u8 *puVar14;
    u32 *puVar15;
    bool bVar16;
    u8 auStack_40[20];
    int *local_2c;
    u8 abStack_28[4];

    puVar15 = (u32 *)auStack_40;
    iVar11 = *(int *)(param_3 + 0x2c);
    local_2c = (int *)*DAT_0201113c;
    iVar7 = *(int *)*DAT_0201113c;
    switch(param_1) {
    case 0:
        goto LAB_02010e70;
    case 1:
        goto LAB_02010e70;
    case 2:
        puVar15 = (u32 *)auStack_40;
        break;
    case 3:
        puVar15 = (u32 *)auStack_40;
        break;
    case 4:
        goto LAB_02010e70;
    case 5:
        goto LAB_02010e70;
    case 6:
        puVar15 = (u32 *)auStack_40;
        break;
    case 7:
        puVar15 = (u32 *)auStack_40;
        break;
    case 8:
        goto LAB_02010e70;
    case 9:
LAB_02010e70:
        param_3 = ((int)(param_1 + ((u32)((int)param_1 >> 1) >> 0x1e)) >> 2) + 1;
        unaff_r11 = param_1 & 3;
        unaff_r6 = 1;
        puVar15 = (u32 *)auStack_40;
        break;
    case 10:
        puVar15 = (u32 *)auStack_40;
        break;
    case 0xb:
        puVar15 = (u32 *)auStack_40;
        break;
    case 0xc:
        goto LAB_02010e8c;
    case 0xd:
        goto LAB_02010e8c;
    case 0xe:
        goto LAB_02010e8c;
    case 0xf:
LAB_02010e8c:
        unaff_r11 = param_1 & 1;
        bVar16 = *(int *)(iVar11 + 0x18) == 0;
        puVar14 = auStack_40;
        if (bVar16) {
            puVar14 = auStack_40; /* Ghidra: &stack0xffffffdc */
        }
        param_3 = 0;
        if (bVar16) {
            return;
        }
        puVar15 = (u32 *)puVar14;
        if ((int)param_1 < 0xe) {
            unaff_r6 = 1;
        }
        else {
            unaff_r6 = 0;
        }
    }
    piVar13 = *(int **)(iVar11 + param_3 * 4 + 0x18);
    if (piVar13 == (int *)0x0) {
        return;
    }
    uVar9 = *(int *)(iVar7 + 0x20) >> 4;
    iVar8 = *(int *)(iVar7 + 0x24) >> 4;
    iVar4 = (**(code **)(*piVar13 + 0x40))();
    if (iVar4 <= (int)uVar9) {
        return;
    }
    iVar4 = FUN_020091ec(piVar13, uVar9);
    if (iVar4 <= iVar8) {
        return;
    }
    (**(code **)(*piVar13 + 0x34))(piVar13, uVar9 & 0xff, (int)(short)iVar8);
    uVar3 = *(u16 *)(piVar13 + 0x18);
    iVar8 = *(int *)(iVar7 + 0x28);
    *puVar15 = uVar3 & 0xff;
    *(u16 *)(piVar13 + 0x18) = (short)(uVar3 & 0xff) + (short)(iVar8 >> 4) & 0xff;
    uVar9 = piVar13[0x1f];
    iVar8 = *(int *)(iVar7 + 0x2c);
    puVar15[3] = (uVar9 << 0x1c) >> 0x1f;
    if (iVar8 >> 4 != 0) {
        piVar13[0x1f] = uVar9 | 8;
    }
    uVar9 = piVar13[0x1f];
    iVar7 = *(int *)(iVar7 + 0x30);
    puVar15[4] = (uVar9 << 5) >> 0x1f;
    if (iVar7 >> 4 != 0) {
        piVar13[0x1f] = uVar9 & 0xfbffffff;
    }
    pcVar10 = *(code **)(*piVar13 + 0x44);
    iVar7 = piVar13[0x17];
    puVar15[1] = (int)*(short *)((int)piVar13 + 0x5e);
    uVar9 = (*pcVar10)();
    puVar15[2] = uVar9;
    uVar9 = piVar13[0x1f];
    (**(code **)(puVar15[5] + 0x14))(piVar13, *(u32 *)(iVar11 + 0x18), iVar11, param_2);
    if (unaff_r11 == 0) {
        *(short *)(piVar13 + 0x1b) = (short)(*param_2 >> 4);
        *(short *)((int)piVar13 + 0x6e) = (short)(param_2[4] >> 4);
        *(short *)(piVar13 + 0x1c) = (short)(param_2[1] >> 4);
        *(short *)((int)piVar13 + 0x72) = (short)(param_2[5] >> 4);
    }
    else {
        piVar13[0x1b] = 0x100;
        piVar13[0x1c] = 0x1000000;
    }
    if (unaff_r6 == 0) {
        (*(code *)(unsigned long)puVar15[0xf])();
        return;
    }
    iVar11 = piVar13[0x1f];
    FUN_0200937c(piVar13, 1);
    uVar12 = (u32)*(u8 *)(piVar13 + 4);
    uVar1 = *(u8 *)(DAT_02011144 + uVar12);
    *(u8 *)(puVar15 + 6) = *(u8 *)(DAT_02011140 + uVar12);
    *(u8 *)((int)puVar15 + 0x19) = uVar1;
    uVar6 = puVar15[6];
    bVar2 = *(u8 *)((int)puVar15 + 0x19);
    (**(code **)(*piVar13 + 8))(piVar13, DAT_02011148 + uVar12 * 0x400, puVar15 + 6, (int)puVar15 + 0x19);
    uVar5 = (**(code **)(*piVar13 + 0x44))();
    FUN_02007a74(uVar12, uVar5, (u32)(u8)puVar15[6] - (u32)(u8)uVar6,
                 (u32)*(u8 *)((int)puVar15 + 0x19) - (u32)bVar2);
    *(u16 *)(piVar13 + 0x18) = (u16)*puVar15 & 0xff;
    uVar6 = puVar15[4];
    piVar13[0x1f] = piVar13[0x1f] & 0xfffffff7U | (puVar15[3] & 1) << 3;
    piVar13[0x1f] = piVar13[0x1f] & 0xfbffffffU | (uVar6 & 1) << 0x1a;
    uVar6 = puVar15[1];
    *(short *)(piVar13 + 0x17) = (short)iVar7;
    *(short *)((int)piVar13 + 0x5e) = (short)uVar6;
    piVar13[0x1f] = piVar13[0x1f] & 0xfffffffcU | uVar9 & 3;
    piVar13[0x1a] = puVar15[2];
    FUN_0200937c(piVar13, (u32)(iVar11 << 0x17) >> 0x1f);
    return;
}

// FUN_02011164 @ 0x02011164 (132 bytes) — Flush sprite palette DMA
void FUN_02011164(int param_1)
{
    if (*(char *)(*(int *)(param_1 + 4) + 0x782) == '\0') {
        FUN_020369c4(DAT_020111e8, *(u32 *)(param_1 + 0xec), *(u32 *)(param_1 + 0xf0),
                     *(u32 *)(param_1 + 0xf4), *(u32 *)(param_1 + 0xf8));
    }
    else {
        FUN_020369c4(DAT_020111ec, *(u32 *)(param_1 + 0xec), *(u32 *)(param_1 + 0xf0),
                     *(u32 *)(param_1 + 0xf4), *(u32 *)(param_1 + 0xf8));
    }
    *(u32 *)(param_1 + 0xec) = 0;
    *(u32 *)(param_1 + 0xf0) = 0;
    *(u32 *)(param_1 + 0xf4) = 0;
    *(u32 *)(param_1 + 0xf8) = 0;
    *(u8 *)(param_1 + 0xfd) = *(u8 *)(param_1 + 0xfd) & 0xfe;
    return;
}

// FUN_02011218 @ 0x02011218 (112 bytes) — Release sprite VRAM layers
void FUN_02011218(int param_1)
{
    int iVar1;
    int iVar2;

    iVar2 = 0;
    if (*(char *)(param_1 + 0xfc) != '\0') {
        do {
            iVar1 = param_1 + iVar2 * 4;
            FUN_0203b5b4(3, *(u32 *)(iVar1 + 0x2c), *(u32 *)(iVar1 + 0x6c),
                         *(u32 *)(iVar1 + 0xac));
            iVar1 = param_1 + iVar2 * 4;
            *(u32 *)(iVar1 + 0x2c) = 0;
            *(u32 *)(iVar1 + 0x6c) = 0;
            *(u32 *)(iVar1 + 0xac) = 0;
            iVar2 = iVar2 + 1;
        } while (iVar2 < (int)(u32)*(u8 *)(param_1 + 0xfc));
    }
    *(u8 *)(param_1 + 0xfc) = 0;
    return;
}

// FUN_02011288 @ 0x02011288 (128 bytes) — Register sprite VRAM layer
void FUN_02011288(int param_1, int param_2, int param_3, u32 param_4)
{
    u32 uVar1;
    int iVar2;
    int iVar3;

    uVar1 = (u32)*(u8 *)(param_1 + 0xfc);
    iVar2 = 0;
    if (uVar1 != 0) {
        do {
            iVar3 = param_1 + iVar2 * 4;
            if ((*(int *)(iVar3 + 0x2c) == param_2) && (*(int *)(iVar3 + 0x6c) == param_3)) {
                return;
            }
            iVar2 = iVar2 + 1;
        } while (iVar2 < (int)uVar1);
    }
    *(int *)(param_1 + uVar1 * 4 + 0x2c) = param_2;
    *(int *)(param_1 + (u32)*(u8 *)(param_1 + 0xfc) * 4 + 0x6c) = param_3;
    *(u32 *)(param_1 + (u32)*(u8 *)(param_1 + 0xfc) * 4 + 0xac) = param_4;
    *(char *)(param_1 + 0xfc) = *(char *)(param_1 + 0xfc) + '\x01';
    return;
}

// FUN_02011330 @ 0x02011330 (100 bytes) — Load sprite palette from ROM
void FUN_02011330(int param_1)
{
    int iVar1;

    iVar1 = *(int *)(param_1 + 4);
    FUN_0203a04c(*(u32 *)(iVar1 + 0x8c), 0x200);
    if (*(char *)(iVar1 + 0x782) == '\0') {
        FUN_0203867c(*(u32 *)(iVar1 + 0x8c), 0, 0x200);
    }
    else {
        FUN_02038614(*(u32 *)(iVar1 + 0x8c), 0, 0x200);
    }
    *(u8 *)(param_1 + 0xfd) = *(u8 *)(param_1 + 0xfd) & 0xfd;
    return;
}

// FUN_020114bc @ 0x020114bc (604 bytes) — Scroll tilemap copy
void FUN_020114bc(int param_1, u32 param_2, int param_3, int param_4, int param_5, int param_6)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int unaff_r9 = 0;

    if (((param_2 & 1) != 0) || ((param_2 & 2) != 0)) {
        if ((param_2 & 1) == 0) {
            if ((param_2 & 2) != 0) {
                param_1 = param_4 + 0x18;
            }
        }
        else {
            param_1 = param_4 + -1;
        }
        iVar2 = param_1 >> 0x1f;
        if (param_1 < 0) {
            iVar2 = ((u32)(param_1 * 0x8000000 + iVar2) >> 0x1b | iVar2 << 5) - iVar2;
            param_1 = iVar2 + 0x20 >> 0x1f;
            param_1 = ((u32)(iVar2 * 0x8000000 + param_1) >> 0x1b | param_1 << 5) - param_1;
        }
        else if (0x1f < param_1) {
            param_1 = ((u32)(param_1 * 0x8000000 + iVar2) >> 0x1b | iVar2 << 5) - iVar2;
        }
        iVar2 = param_1 >> 0x1f;
        iVar2 = (((u32)(param_1 * 0x8000000 + iVar2) >> 0x1b | iVar2 << 5) - iVar2) * 0x20;
        iVar4 = param_3 + -1;
        iVar5 = -1;
        do {
            iVar3 = iVar4 >> 0x1f;
            if (iVar4 < 0) {
                iVar3 = ((u32)(iVar4 * 0x4000000 + iVar3) >> 0x1a | iVar3 << 6) - iVar3;
                iVar1 = iVar3 + 0x40 >> 0x1f;
                unaff_r9 = ((u32)(iVar3 * 0x4000000 + iVar1) >> 0x1a | iVar1 << 6) - iVar1;
            }
            else {
                unaff_r9 = iVar4;
                if (0x3f < iVar4) {
                    unaff_r9 = ((u32)(iVar4 * 0x4000000 + iVar3) >> 0x1a | iVar3 << 6) - iVar3;
                }
            }
            iVar3 = unaff_r9 >> 0x1f;
            if (((int)(unaff_r9 + ((u32)(unaff_r9 >> 4) >> 0x1b)) >> 5 & 1U) == 0) {
                iVar3 = (((u32)(unaff_r9 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3) + iVar2;
            }
            else {
                iVar3 = (((u32)(unaff_r9 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3) +
                        iVar2 + 0x400;
            }
            FUN_0203b7c0(param_5 + iVar3 * 2, param_6 + iVar3 * 2, 2);
            iVar5 = iVar5 + 1;
            iVar4 = iVar4 + 1;
        } while (iVar5 < 0x21);
    }
    if (((param_2 & 4) == 0) && ((param_2 & 8) == 0)) {
        return;
    }
    if ((param_2 & 4) == 0) {
        if ((param_2 & 8) != 0) {
            unaff_r9 = param_3 + 0x20;
        }
    }
    else {
        unaff_r9 = param_3 + -1;
    }
    iVar2 = unaff_r9 >> 0x1f;
    if (unaff_r9 < 0) {
        iVar2 = ((u32)(unaff_r9 * 0x4000000 + iVar2) >> 0x1a | iVar2 << 6) - iVar2;
        iVar4 = iVar2 + 0x40 >> 0x1f;
        unaff_r9 = ((u32)(iVar2 * 0x4000000 + iVar4) >> 0x1a | iVar4 << 6) - iVar4;
    }
    else if (0x3f < unaff_r9) {
        unaff_r9 = ((u32)(unaff_r9 * 0x4000000 + iVar2) >> 0x1a | iVar2 << 6) - iVar2;
    }
    iVar2 = unaff_r9 >> 0x1f;
    param_4 = param_4 + -1;
    iVar4 = -1;
    do {
        iVar5 = param_4 >> 0x1f;
        if (param_4 < 0) {
            iVar5 = ((u32)(param_4 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5;
            iVar3 = iVar5 + 0x20 >> 0x1f;
            iVar3 = ((u32)(iVar5 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3;
        }
        else {
            iVar3 = param_4;
            if (0x3ff < param_4) {
                iVar3 = ((u32)(param_4 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5;
            }
        }
        iVar5 = iVar3 >> 0x1f;
        if (((int)(unaff_r9 + ((u32)(unaff_r9 >> 4) >> 0x1b)) >> 5 & 1U) == 0) {
            iVar5 = (((u32)(iVar3 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5) * 0x20;
        }
        else {
            iVar5 = (((u32)(iVar3 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5) * 0x20 + 0x400;
        }
        iVar5 = (((u32)(unaff_r9 * 0x8000000 + iVar2) >> 0x1b | iVar2 << 5) - iVar2) + iVar5;
        FUN_0203b7c0(param_5 + iVar5 * 2, param_6 + iVar5 * 2, 2);
        iVar4 = iVar4 + 1;
        param_4 = param_4 + 1;
    } while (iVar4 < 0x19);
    return;
}

// FUN_02011718 @ 0x02011718 (264 bytes) — Clear BG character data
void FUN_02011718(int param_1, int param_2)
{
    int iVar1;

    iVar1 = *(int *)(param_1 + 4);
    if (*(char *)(iVar1 + 0x782) != '\0') {
        if (param_2 == 0) {
            FUN_020383b4(*(u32 *)(iVar1 + 0x74), 0, 0x1000);
            return;
        }
        if (param_2 != 1) {
            if (param_2 != 2) {
                return;
            }
            FUN_02038214(*(u32 *)(iVar1 + 0x7c), 0, 0x1000);
            return;
        }
        FUN_020382e4(*(u32 *)(iVar1 + 0x78), 0, 0x1000);
        return;
    }
    if (param_2 == 0) {
        FUN_0203841c(*(u32 *)(iVar1 + 0x74), 0, 0x1000);
        return;
    }
    if (param_2 != 1) {
        if (param_2 != 2) {
            return;
        }
        FUN_0203827c(*(u32 *)(iVar1 + 0x7c), 0, 0x1000);
        return;
    }
    FUN_0203834c(*(u32 *)(iVar1 + 0x78), 0, 0x1000);
    return;
}

// FUN_02011820 @ 0x02011820 (192 bytes) — Process pending sprite changes
void FUN_02011820(int *param_1)
{
    u32 uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    u32 uVar6;

    iVar5 = param_1[1];
    uVar6 = 0;
    do {
        iVar4 = iVar5 + uVar6;
        if (*(char *)(iVar4 + 0x77c) == -1) {
            (**(code **)(*param_1 + 0x10))(param_1, uVar6 & 0xff);
        }
        else if (*(char *)(iVar4 + 0x77c) != '\0') {
            iVar2 = iVar5 + uVar6 * 4;
            iVar3 = *(int *)(iVar2 + 0x734);
            iVar2 = *(int *)(iVar2 + 0x740);
            uVar1 = (**(code **)(*param_1 + 0x20))(param_1, uVar6 & 0xff);
            (**(code **)(*param_1 + 0x14))
                      (param_1, *(u8 *)(iVar4 + 0x77c), (u32)(iVar3 << 5) >> 0x10,
                       (u32)(iVar2 << 5) >> 0x10, *(u32 *)(iVar5 + uVar6 * 4 + 0x74), uVar1);
        }
        uVar6 = uVar6 + 1;
        *(u8 *)(iVar4 + 0x77c) = 0;
    } while ((int)uVar6 < 3);
    return;
}

// FUN_020118e0 @ 0x020118e0 (632 bytes) — Sprite animation state machine
void FUN_020118e0(int *param_1)
{
    int iVar1;
    int *piVar2;

    piVar2 = (int *)param_1[1];
    if (-1 < (int)((u32)*(u16 *)(DAT_02011b58 + 0x14) << 0x17)) {
        return;
    }
    iVar1 = param_1[10];
    if (iVar1 == 0) {
        (**(code **)(*param_1 + 0x18))(param_1, 0, 0, 0);
        (**(code **)(*param_1 + 0x18))(param_1, 1, 0, 0);
        (**(code **)(*param_1 + 0x18))(param_1, 2, 0, 0);
        param_1[10] = param_1[10] + 1;
        return;
    }
    if (iVar1 == 1) {
        iVar1 = (**(code **)(*piVar2 + 0x24))();
        if (iVar1 != 0) {
            param_1[10] = 0xff;
        }
        return;
    }
    if (iVar1 == 0xff) {
        if ((((char)piVar2[0x1df] != '\0') || (*(char *)((int)piVar2 + 0x77d) != '\0')) ||
           (*(char *)((int)piVar2 + 0x77e) != '\0')) {
            FUN_0203a04c(piVar2[0x1d], 0x1000);
            FUN_0203a04c(piVar2[0x1e], 0x1000);
            FUN_0203a04c(piVar2[0x1f], 0x1000);
            (**(code **)(*param_1 + 0xc))();
        }
        (**(code **)(*param_1 + 0x18))(param_1, 0, piVar2[0x1cd] >> 8, piVar2[0x1d0] >> 8);
        (**(code **)(*param_1 + 0x18))(param_1, 1, piVar2[0x1ce] >> 8, piVar2[0x1d1] >> 8);
        (**(code **)(*param_1 + 0x18))(param_1, 2, piVar2[0x1cf] >> 8, piVar2[0x1d2] >> 8);
        iVar1 = (u32)*(u8 *)((int)param_1 + 0xfd) << 0x1e;
        if (iVar1 < 0) {
            (**(code **)(*param_1 + 0x1c))();
        }
        else if ((((*(u8 *)(piVar2 + 0x1e3) & 3) == 1) ||
                 (((u32)*(u8 *)(piVar2 + 0x1e3) << 0x1c) >> 0x1e == 1)) || (iVar1 >> 0x1f == -2)) {
            (**(code **)(*piVar2 + 0x88))();
            if ((int)((u32)*(u8 *)((int)param_1 + 0xfd) << 0x1e) >> 0x1f == -2) {
                *(u8 *)((int)param_1 + 0xfd) = *(u8 *)((int)param_1 + 0xfd) & 0xfd;
            }
        }
        if ((char)param_1[0x3f] != '\0') {
            (**(code **)(*param_1 + 0x28))();
        }
        if ((int)((u32)*(u8 *)((int)param_1 + 0xfd) << 0x1f) < 0) {
            (**(code **)(*param_1 + 0x30))();
            return;
        }
        return;
    }
    return;
}

// FUN_02011bac @ 0x02011bac (112 bytes) — Construct sprite manager object
u32 *FUN_02011bac(u32 *param_1)
{
    int iVar1;
    int iVar2;

    FUN_0202a20c();
    *param_1 = DAT_02011c1c;
    param_1[10] = 0;
    iVar1 = 0;
    do {
        param_1[iVar1 + 0xb] = 0;
        iVar2 = iVar1 + 1;
        param_1[iVar1 + 0x1b] = 0;
        iVar1 = iVar2;
    } while (iVar2 < 0x10);
    *(u8 *)(param_1 + 0x3f) = 0;
    param_1[0x3b] = 0;
    param_1[0x3c] = 0;
    param_1[0x3d] = 0;
    param_1[0x3e] = 0;
    *(u8 *)((int)param_1 + 0xfd) = *(u8 *)((int)param_1 + 0xfd) & 0xfe;
    *(u8 *)((int)param_1 + 0xfd) = *(u8 *)((int)param_1 + 0xfd) & 0xfd;
    return param_1;
}

// FUN_02011c54 @ 0x02011c54 (1020 bytes) — Process sprite animation frames
void FUN_02011c54(int param_1)
{
    u32 uVar1;
    u32 extraout_r1;
    int iVar2;
    short *psVar3;
    u32 uVar4;
    u32 local_80;
    u32 uStack_7c;
    u32 uStack_60;
    int iStack_48;
    u32 uStack_44;
    u32 uStack_3c;
    u32 auStack_34[4];

    psVar3 = *(short **)(param_1 + 0xa8);
    local_80 = 0;
    *(u8 *)(param_1 + 0x78c) = *(u8 *)(param_1 + 0x78c) & 0xf3;
    if (*(char *)(param_1 + 0x78b) != '\0') {
        uStack_44 = 0;
        uStack_60 = 0;
        iStack_48 = 0;
        uStack_3c = 0;
        do {
            if ((*psVar3 != 0) && (*(char *)((int)psVar3 + 0x17) == '\0')) {
                *(int *)(psVar3 + 8) = *(int *)(psVar3 + 8) + (int)psVar3[10];
                if (*(int *)(psVar3 + 6) < *(int *)(psVar3 + 8) >> 8) {
                    if ((char)psVar3[0xb] == '\0') {
                        *(u8 *)((int)psVar3 + 0x17) = 1;
                        *psVar3 = 0;
                        psVar3[8] = 0;
                        psVar3[9] = 0;
                        if ((*(u8 *)(param_1 + 0x78c) & 3) == 1) {
                            *(u8 *)(param_1 + 0x78c) = *(u8 *)(param_1 + 0x78c) & 0xfc | 2;
                        }
                    }
                    else if ((char)psVar3[0xb] == '\x01') {
                        *psVar3 = -1;
                        psVar3[8] = 0;
                        psVar3[9] = 0;
                    }
                    else {
                        extraout_r1 = (u32)FUN_020464b0(*(int *)(psVar3 + 8), *(int *)(psVar3 + 6) << 8);
                        *(u32 *)(psVar3 + 8) = extraout_r1;
                    }
                }
                if (((((psVar3[2] == 0) && (psVar3[3] == 0)) && (psVar3[4] == 0)) && (psVar3[5] == 0)) ||
                   (*psVar3 == 0)) {
                    if (*psVar3 == -1) {
                        *psVar3 = 0;
                        *(u8 *)((int)psVar3 + 0x17) = 1;
                    }
                }
                else {
                    *(u8 *)(param_1 + 0x78c) = *(u8 *)(param_1 + 0x78c) & 0xf3 | 4;
                    if (*psVar3 == -1) {
                        uStack_7c = 0xff;
                        uVar4 = uStack_60;
                        do {
                            uVar1 = uStack_7c;
                            if ((*(int *)(param_1 + local_80 * 8 + 0x618) != 0) &&
                               (uVar1 = local_80 & 0xff,
                               ((u32)*(u8 *)(param_1 + 0x783) & 1 << (uVar4 & 0xff)) != 0)) {
                                iVar2 = param_1 + uVar4 * 8;
                                FUN_0203b7f0(*(u32 *)(iVar2 + 0x61c),
                                             *(u32 *)(param_1 + uVar4 * 4 + 0x80),
                                             *(u32 *)(iVar2 + 0x618));
                                uVar1 = uStack_7c;
                            }
                            uStack_7c = uVar1;
                            uVar4 = uVar4 + 1;
                        } while ((int)uVar4 < 3);
                        if (uStack_7c != 0xffffffff) {
                            FUN_0203b7f0(*(u32 *)(param_1 + 0x90), *(u32 *)(param_1 + 0x8c), 0x200);
                        }
                        *psVar3 = 0;
                        *(u8 *)((int)psVar3 + 0x17) = 1;
                    }
                    else {
                        auStack_34[0] =
                             (u32)((*(u32 *)(param_1 + 0x618) & DAT_02012050) != 0) +
                             (*(u32 *)(param_1 + 0x618) >> 9);
                        auStack_34[1] =
                             (u32)((*(u32 *)(param_1 + 0x620) & DAT_02012050) != 0) +
                             (*(u32 *)(param_1 + 0x620) >> 9);
                        auStack_34[2] =
                             (u32)((*(u32 *)(param_1 + 0x628) & DAT_02012050) != 0) +
                             (*(u32 *)(param_1 + 0x628) >> 9);
                        iVar2 = iStack_48;
                        do {
                            if ((*(int *)(param_1 + iVar2 * 8 + 0x618) != 0) &&
                               (uVar1 = auStack_34[iVar2], uVar4 = uStack_44, uVar1 != 0)) {
                                do {
                                    if ((1 << (uVar4 & 0xff) & (u32)(u16)psVar3[iVar2 + 2]) != 0) {
                                        FUN_0202da48((int)*psVar3, psVar3[1], *(u32 *)(psVar3 + 6),
                                                     *(u32 *)(psVar3 + 8));
                                    }
                                    uVar4 = uVar4 + 1;
                                } while (uVar4 < uVar1);
                            }
                            iVar2 = iVar2 + 1;
                            uVar4 = uStack_3c;
                        } while (iVar2 < 3);
                        do {
                            if (((u32)(u16)psVar3[5] & 1 << (uVar4 & 0xff)) != 0) {
                                FUN_0202da48((int)*psVar3, psVar3[1], *(u32 *)(psVar3 + 6),
                                             *(u32 *)(psVar3 + 8));
                            }
                            uVar4 = uVar4 + 1;
                        } while ((int)uVar4 < 0x10);
                    }
                }
            }
            local_80 = local_80 + 1;
            psVar3 = psVar3 + 0xc;
        } while ((int)local_80 < (int)(u32)*(u8 *)(param_1 + 0x78b));
        return;
    }
    return;
}
