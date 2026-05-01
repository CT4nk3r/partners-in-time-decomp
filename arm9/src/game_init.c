/**
 * Game Initialization, Scene Management, and Script Setup
 *
 * This module handles overlay decompression, scene construction/teardown,
 * display configuration, and the script comparison operator dispatch.
 *
 * Decompiled from:
 *   FUN_02004914 @ 0x02004914 (164 bytes) - LZ decompression with cache invalidation
 *   FUN_02004d40 @ 0x02004D40 (240 bytes) - Scene teardown / clear game state
 *   FUN_02004e38 @ 0x02004E38 (188 bytes) - Initialize display config parameters
 *   FUN_02004f7c @ 0x02004F7C (356 bytes) - Load overlay/archive resources
 *   FUN_020051f4 @ 0x020051F4 (136 bytes) - Fade loop with brightness adjustment
 *   FUN_02005b70 @ 0x02005B70 (440 bytes) - Initialize clGameMain object
 *   FUN_02005d54 @ 0x02005D54 (20 bytes)  - Set scene state and indirect jump
 *   FUN_020062b4 @ 0x020062B4 (40 bytes)  - Construct object type A
 *   FUN_02006304 @ 0x02006304 (92 bytes)  - Construct scene object
 *   FUN_0200636c @ 0x0200636C (112 bytes) - Script interpreter loop
 *   FUN_020063dc @ 0x020063DC (244 bytes) - Comparison operator dispatch
 */

#include "types.h"

/* Forward declarations for external functions */
extern u32 FUN_02029c1c(u32, u32, u32, u32);
extern void FUN_0202cc10(void *, u32, u32);
extern void FUN_0203b9a8(void *, void *, u32);
extern void FUN_020192f8(u32);
extern void FUN_02018ed0(void);
extern u32 FUN_0202bd98(u32, u32, u32, u32, u32);
extern u32 FUN_0202bb54(void *, u32);
extern u32 FUN_02029bf8(u32, u32, u32, u32);
extern void FUN_0202bacc(void *, u32, u32, u32);
extern void FUN_0202ba28(void *, u32);
extern u32 FUN_0202b9b8(void *, u32, u16);
extern void FUN_0202b428(void *, void *, u32, u32, u32, u32);
extern void FUN_0202b944(void *, void *);
extern void thunk_FUN_02029ab8(void *);
extern void FUN_0203b914(u32, u32, u32);
extern u32 FUN_020464b0(u32, u8);
extern void FUN_02035110(u32, u32);
extern void FUN_0202958c(void);
extern void FUN_02028028(void);
extern void FUN_0203aec8(u32);
extern void FUN_0202a5a8(void);
extern void FUN_0202a74c(void);
extern void FUN_02007178(u32, int, void *);
extern int FUN_020064d0(u32, int, void *);

/* Static data references */
static u32 *DAT_02005d28;   /* clGameMain instance pointer */
static u32 DAT_02005d2c;    /* clGameMain struct size */
static u32 DAT_02005d30;    /* offset into clGameMain for config */
static u16 *DAT_02005d34;   /* default config data */
static u8 *DAT_02005d38;    /* display mode flag */
static u32 *DAT_02004e34;   /* game state pointer */
static u32 DAT_02004ef4;    /* system config base */
static u32 DAT_020050e0;    /* archive alloc size A */
static u32 *DAT_020050e4;   /* archive ID pointer */
static u32 DAT_020050e8;    /* archive alloc size B */
static u32 *DAT_020050ec;   /* loaded archive pointer */
static u32 DAT_020050f0;    /* archive alloc size C */
static u8 *DAT_0200527c;    /* fade counter */
static u8 *DAT_02005280;    /* fade target */
static u32 DAT_02005284;    /* brightness reg A */
static u32 DAT_02005288;    /* brightness reg B */
static u32 DAT_020062dc;    /* vtable for object type A */
static u32 DAT_02006360;    /* vtable for scene object */
static u32 *DAT_02006364;   /* scene data pointer */
static u32 *DAT_02006368;   /* scene state pointer */
static void (*DAT_02005d68)(void); /* scene jump target */

/* Forward declaration for functions defined later in this file */
static void FUN_02004e38(u32 param_1, u16 param_2, u16 param_3, u16 *param_4);

// FUN_02004914 @ 0x02004914 (164 bytes) — LZ decompression with cache invalidation
void FUN_02004914(int param_1)
{
    BOOL bVar1;
    u8 *puVar2;
    u16 uVar3;
    u32 uVar4;
    u8 *puVar5;
    u8 *puVar6;
    u8 *puVar7;
    u8 *pbVar8;
    u8 bVar9;
    int iVar10;
    int iVar11;

    if (param_1 != 0) {
        puVar5 = (u8 *)(param_1 + *(int *)(param_1 + -4));
        puVar7 = (u8 *)(param_1 - (*(u32 *)(param_1 + -8) >> 0x18));
        uVar4 = param_1 - (*(u32 *)(param_1 + -8) & 0xffffff);
        puVar6 = puVar5;
        while ((int)uVar4 < (int)puVar7) {
            puVar7 = puVar7 + -1;
            bVar9 = *puVar7;
            iVar10 = 8;
            while (0 < iVar10) {
                if ((bVar9 & 0x80) == 0) {
                    puVar7 = puVar7 + -1;
                    puVar6 = puVar6 + -1;
                    *puVar6 = *puVar7;
                } else {
                    pbVar8 = puVar7 + -1;
                    puVar7 = puVar7 + -2;
                    uVar3 = *(u16 *)puVar7;
                    iVar11 = *pbVar8 + 0x20;
                    do {
                        puVar2 = puVar6 + (uVar3 & 0xfff) + 2;
                        puVar6 = puVar6 + -1;
                        *puVar6 = *puVar2;
                        bVar1 = (BOOL)(0xf < iVar11);
                        iVar11 = iVar11 + -0x10;
                    } while (bVar1);
                }
                bVar9 = bVar9 << 1;
                iVar10 = iVar10 + -1;
                if ((int)puVar7 <= (int)uVar4) goto done;
            }
        }
done:
        /* Invalidate instruction and data cache lines covering decompressed data */
        uVar4 = uVar4 & 0xffffffe0;
        do {
            /* CP15 cache maintenance operations */
            uVar4 = uVar4 + 0x20;
        } while ((int)uVar4 < (int)puVar5);
    }
    return;
}

// FUN_02004d40 @ 0x02004D40 (240 bytes) — Scene teardown / clear game state
void FUN_02004d40(void)
{
    int *piVar1;

    FUN_02004e38(0xffffffff, 0, 1, (u16 *)DAT_02004ef4);
    FUN_0203b914(*DAT_02004e34, 0, 0x40);
    FUN_0203b914(*DAT_02004e34 + 0x40, 0, 8);
    FUN_0203b914(*DAT_02004e34 + 0x48, 0, 8);
    FUN_0203b914(*DAT_02004e34 + 0x50, 0, 0x80);
    FUN_0203b914(*DAT_02004e34 + 0xd0, 0, 0x120);
    FUN_0203b914(*DAT_02004e34 + 0x1f0, 0, 0x200);
    FUN_0203b914(*DAT_02004e34 + 0x3f0, 0, 8);
    piVar1 = (int *)DAT_02004e34;
    *(u8 *)(*piVar1 + 0x516) = 0;
    *(u8 *)(*piVar1 + 0x517) = 0;
    *(u32 *)(*piVar1 + 0x518) = 0;
    FUN_020192f8(0);
    return;
}

// FUN_02004e38 @ 0x02004E38 (188 bytes) — Initialize display config parameters
void FUN_02004e38(u32 param_1, u16 param_2, u16 param_3, u16 *param_4)
{
    int iVar1;
    int iVar2;

    if ((param_1 == 0xffffffff) && (param_1 = 1, *(char *)(DAT_02004ef4 + 0xc) != 1)) {
        param_1 = 1;
    }
    *param_4 = *param_4 & 0xfffe;
    *param_4 = *param_4 & 0xfffd;
    *param_4 = (*param_4 & 0xfffb) | (u16)((param_2 & 1) << 2);
    *param_4 = (*param_4 & 0xfff7) | (u16)((param_3 & 1) << 3);
    *param_4 = (*param_4 & 0xfe0f) | (u16)((param_1 & 0x1f) << 4);
    *param_4 = *param_4 | 0x200;
    param_4[1] = 0x10;
    param_4[2] = 0xf;
    iVar1 = 0;
    do {
        iVar2 = iVar1 + 1;
        (param_4 + iVar1 * 2 + 4)[0] = 0;
        (param_4 + iVar1 * 2 + 4)[1] = 0;
        iVar1 = iVar2;
    } while (iVar2 < 2);
    return;
}

// FUN_02004f7c @ 0x02004F7C (356 bytes) — Load overlay/archive resources
void FUN_02004f7c(int param_1)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;
    u8 auStack_40[12];
    int local_34;
    u32 local_2c;
    u16 local_1c;

    iVar1 = FUN_02029c1c(0x140, 0, DAT_020050e0, 1);
    piVar2 = (int *)0;
    if (iVar1 != 0) {
        piVar2 = (int *)FUN_0202bd98(iVar1, 0, DAT_020050e0, 0, 0xffffffff);
    }
    uVar3 = FUN_0202bb54(piVar2, *DAT_020050e4);
    uVar4 = FUN_02029bf8(uVar3 & 0xfffffffc, 0, DAT_020050e8, 1);
    FUN_0202bacc(piVar2, uVar4, uVar3, *DAT_020050e4);
    FUN_0202ba28(piVar2, *DAT_020050e4);
    if (*DAT_020050ec == 0) {
        if (param_1 == 0) {
            local_1c = 0;
        } else {
            local_1c = 5;
        }
        local_2c = FUN_0202b9b8(piVar2, uVar4, local_1c);
        uVar5 = FUN_0202b9b8(piVar2, uVar4, 0);
        local_34 = FUN_02029bf8(uVar5, 1, DAT_020050f0, 0);
        FUN_0202b428(piVar2, auStack_40, 0, uVar4, 0, 0);
        FUN_0202b944(piVar2, auStack_40);
        *DAT_020050ec = local_34;
    }
    thunk_FUN_02029ab8((void *)uVar4);
    if (piVar2 != (int *)0) {
        (**(void (**)(void))(*piVar2 + 4))();
        return;
    }
    return;
}

// FUN_020051f4 @ 0x020051F4 (136 bytes) — Fade loop with brightness adjustment
void FUN_020051f4(void)
{
    u8 uVar1;
    u8 *pbVar2;
    u8 *puVar3;
    u32 uVar4;
    u32 uVar5;
    int iVar6;

    uVar5 = DAT_02005288;
    uVar4 = DAT_02005284;
    puVar3 = DAT_02005280;
    pbVar2 = DAT_0200527c;
    do {
        do {
            uVar1 = *puVar3;
            if (*pbVar2 != 0) {
                *pbVar2 = *pbVar2 - 1;
            }
            iVar6 = FUN_020464b0((u32)*pbVar2 << 4, uVar1);
            FUN_02035110(uVar4, 0x10 - iVar6);
            iVar6 = FUN_020464b0((u32)*pbVar2 << 4, *puVar3);
            FUN_02035110(uVar5, 0x10 - iVar6);
            FUN_0202958c();
            FUN_02028028();
        } while (*pbVar2 != 0);
        FUN_0203aec8(1);
    } while (TRUE);
}

// FUN_02005b70 @ 0x02005B70 (440 bytes) — Initialize clGameMain object
void FUN_02005b70(u16 *param_1)
{
    int *piVar1;
    u16 *puVar2;
    u8 *pbVar3;
    int iVar4;
    u32 uVar5;
    int iVar6;

    if (*DAT_02005d28 == 0) {
        iVar4 = FUN_02029c1c(DAT_02005d2c, 1, 0, 0);
        uVar5 = DAT_02005d2c;
        *DAT_02005d28 = iVar4;
        FUN_0202cc10((void *)iVar4, 0, uVar5);
        puVar2 = DAT_02005d34;
        iVar4 = DAT_02005d30;
        piVar1 = (int *)DAT_02005d28;
        if (param_1 == (u16 *)0) {
            iVar6 = *DAT_02005d28;
            *(u16 *)(iVar6 + 0x57c) = *DAT_02005d34;
            pbVar3 = DAT_02005d38;
            iVar6 = iVar6 + iVar4;
            *(u16 *)(iVar6 + 2) = puVar2[1];
            *(u16 *)(iVar6 + 4) = puVar2[2];
            uVar5 = *(u32 *)(puVar2 + 6);
            *(u32 *)(iVar6 + 8) = *(u32 *)(puVar2 + 4);
            *(u32 *)(iVar6 + 0xc) = uVar5;
            iVar4 = *piVar1;
            *(u16 *)(iVar4 + 0x57c) = (u16)((*(u16 *)(iVar4 + 0x57c) & 0xfff7) | (u16)((*pbVar3 & 1) << 3));
        } else {
            iVar6 = *DAT_02005d28;
            *(u16 *)(iVar6 + 0x57c) = *param_1;
            iVar6 = iVar6 + iVar4;
            *(u16 *)(iVar6 + 2) = param_1[1];
            *(u16 *)(iVar6 + 4) = param_1[2];
            uVar5 = *(u32 *)(param_1 + 6);
            *(u32 *)(iVar6 + 8) = *(u32 *)(param_1 + 4);
            *(u32 *)(iVar6 + 0xc) = uVar5;
            FUN_0203b9a8(param_1 + 4, (void *)(*piVar1 + 0x48), 8);
        }
        piVar1 = (int *)DAT_02005d28;
        iVar4 = *DAT_02005d28;
        *(u8 *)(iVar4 + 0x514) =
            (*(u8 *)(iVar4 + 0x514) & 0xfe) |
            (u8)(((u32)*(u16 *)(iVar4 + 0x57c) << 0x1d) >> 0x1f);
        *(u8 *)(*piVar1 + 0x515) = (u8)(((u32)*(u16 *)(*piVar1 + 0x57c) << 0x17) >> 0x1b);
        iVar4 = *piVar1;
        *(u8 *)(iVar4 + 0x514) =
            (*(u8 *)(iVar4 + 0x514) & 0xfd) |
            (u8)((((u32)*(u16 *)(iVar4 + 0x57c) << 0x16) >> 0x1f) << 1);
        iVar4 = *piVar1;
        *(u8 *)(iVar4 + 0x514) =
            (*(u8 *)(iVar4 + 0x514) & 0xbf) |
            (u8)((((u32)*(u16 *)(iVar4 + 0x57c) << 0x1c) >> 0x1f) << 6);
        FUN_020192f8(0);
        FUN_02018ed0();
        return;
    }
    return;
}

// FUN_02005d54 @ 0x02005D54 (20 bytes) — Set scene state and indirect jump
void FUN_02005d54(int param_1, u8 param_2)
{
    *(u8 *)(param_1 + 0x28) = param_2;
    *(u8 *)(param_1 + 0x10) = 0;
    (*DAT_02005d68)();
    return;
}

// FUN_020062b4 @ 0x020062B4 (40 bytes) — Construct object type A
u32 *FUN_020062b4(u32 *param_1)
{
    *param_1 = DAT_020062dc;
    FUN_0202a5a8();
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_02006304 @ 0x02006304 (92 bytes) — Construct scene object
u32 *FUN_02006304(u32 *param_1, u32 param_2, u16 *param_3)
{
    FUN_0202a74c();
    *param_1 = DAT_02006360;
    FUN_02004f7c(((u32)*param_3 << 0x17) >> 0x1b);
    FUN_02005b70(param_3);
    *(u32 *)(*DAT_02006368 + 0x51c) = *DAT_02006364;
    FUN_02005d54((int)param_1, (u8)param_2);
    return param_1;
}

// FUN_0200636c @ 0x0200636C (112 bytes) — Script interpreter loop
int FUN_0200636c(u32 param_1, int param_2)
{
    int iVar1;
    u8 auStack_58[76];

    if ((*(s16 *)(param_2 + 0xaa) != 0) &&
       (*(s16 *)(param_2 + 0xaa) = *(s16 *)(param_2 + 0xaa) + -1, *(s16 *)(param_2 + 0xaa) != 0)) {
        return 3;
    }
    do {
        FUN_02007178(param_1, param_2, auStack_58);
        iVar1 = FUN_020064d0(param_1, param_2, auStack_58);
    } while (iVar1 == 0);
    return iVar1;
}

// FUN_020063dc @ 0x020063DC (244 bytes) — Comparison operator dispatch
BOOL FUN_020063dc(u32 param_1, u32 param_2, u32 param_3)
{
    switch (param_1) {
    case 0:
        return param_2 == param_3;
    case 1:
        return param_2 != param_3;
    case 2:
        return (s32)param_2 < (s32)param_3;
    case 3:
        return (s32)param_3 < (s32)param_2;
    case 4:
        return (s32)param_2 <= (s32)param_3;
    case 5:
        return (s32)param_3 <= (s32)param_2;
    case 6:
        return (param_2 & param_3) != 0;
    case 7:
        return param_2 != 0 || param_3 != 0;
    case 8:
        return param_2 != param_3;
    case 9:
        return param_2 == 0;
    case 10:
        return param_2 != 0xffffffff;
    default:
        return FALSE;
    }
}
