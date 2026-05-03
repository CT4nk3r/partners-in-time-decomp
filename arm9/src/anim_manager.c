/**
 * Animation Manager — frame processing, state machines, timeline control
 *
 * Handles character animation lifecycle: construction, destruction, matrix
 * rotation transforms, OBJ layer display configuration, and curve interpolation.
 *
 * Decompiled from 0x02012340-0x02016254 region (24 functions)
 */
#include "types.h"

/* Forward declarations */
extern void FUN_02029e20(u32);
extern void FUN_02029ab8(u32);
extern void FUN_0202a5a8(void *);
extern void FUN_0202a74c(void);
extern u32 FUN_02029c1c(u32, u32, u32, u32);
extern u32 FUN_02011bac(int, u32, u32, void *);
extern int FUN_020464b0(...);
extern void FUN_020369c4(void *, u32, u32, u16, u16);

/* Static data references */
static u32 DAT_02015b1c;   /* vtable for character destructor A */
static u32 DAT_02015c34;   /* vtable for character destructor B */
static u32 DAT_02015d9c;   /* vtable for character init */
static u32 DAT_02015da0;   /* game instance pointer */

// FUN_02015a00 @ 0x02015a00 (284 bytes) — Destroy character instance and free resources
u32 *FUN_02015a00(u32 *param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    *param_1 = DAT_02015b1c;
    FUN_02029e20(param_1[0xb]);
    iVar3 = 0xf;
    do {
        iVar4 = param_1[iVar3 + 0xd];
        if (iVar4 != 0) {
            FUN_02029ab8(iVar4);
            iVar1 = 0;
            if (0 < iVar3) {
                do {
                    iVar2 = iVar1 + 1;
                    if ((u32)iVar4 == param_1[iVar1 + 0xd]) {
                        param_1[iVar1 + 0xd] = 0;
                    }
                    iVar1 = iVar2;
                } while (iVar2 < iVar3);
            }
        }
        iVar3 = iVar3 + -1;
    } while (-1 < iVar3);
    if (param_1[0x1d7] != 0) { FUN_02029ab8(param_1[0x1d7]); }
    if (param_1[0x1d6] != 0) { FUN_02029ab8(param_1[0x1d6]); }
    if (param_1[0x2b] != 0) { FUN_02029ab8(param_1[0x2b]); }
    FUN_02029ab8(param_1[0x24]);
    if (param_1[0x22] != 0) { FUN_02029ab8(param_1[0x22]); }
    if (param_1[0x21] != 0) { FUN_02029ab8(param_1[0x21]); }
    if (param_1[0x20] != 0) { FUN_02029ab8(param_1[0x20]); }
    FUN_02029ab8(param_1[0x23]);
    FUN_02029ab8(param_1[0x3c]);
    FUN_02029ab8(param_1[0x1f]);
    FUN_02029ab8(param_1[0x1e]);
    FUN_02029ab8(param_1[0x1d]);
    FUN_0202a5a8(param_1);
    FUN_02029ab8((u32)param_1);
    return param_1;
}

// FUN_02015b20 @ 0x02015b20 (276 bytes) — Destroy character instance variant (no self-free)
u32 *FUN_02015b20(u32 *param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    *param_1 = DAT_02015c34;
    FUN_02029e20(param_1[0xb]);
    iVar3 = 0xf;
    do {
        iVar4 = param_1[iVar3 + 0xd];
        if (iVar4 != 0) {
            FUN_02029ab8(iVar4);
            iVar1 = 0;
            if (0 < iVar3) {
                do {
                    iVar2 = iVar1 + 1;
                    if ((u32)iVar4 == param_1[iVar1 + 0xd]) {
                        param_1[iVar1 + 0xd] = 0;
                    }
                    iVar1 = iVar2;
                } while (iVar2 < iVar3);
            }
        }
        iVar3 = iVar3 + -1;
    } while (-1 < iVar3);
    if (param_1[0x1d7] != 0) { FUN_02029ab8(param_1[0x1d7]); }
    if (param_1[0x1d6] != 0) { FUN_02029ab8(param_1[0x1d6]); }
    if (param_1[0x2b] != 0) { FUN_02029ab8(param_1[0x2b]); }
    FUN_02029ab8(param_1[0x24]);
    if (param_1[0x22] != 0) { FUN_02029ab8(param_1[0x22]); }
    if (param_1[0x21] != 0) { FUN_02029ab8(param_1[0x21]); }
    if (param_1[0x20] != 0) { FUN_02029ab8(param_1[0x20]); }
    FUN_02029ab8(param_1[0x23]);
    FUN_02029ab8(param_1[0x3c]);
    FUN_02029ab8(param_1[0x1f]);
    FUN_02029ab8(param_1[0x1e]);
    FUN_02029ab8(param_1[0x1d]);
    FUN_0202a5a8(param_1);
    return param_1;
}

// FUN_02015ff4 @ 0x02015ff4 (220 bytes) — Apply 2D rotation matrix to 4-element vector
void FUN_02015ff4(int param_1, int param_2, int *param_3, int *param_4)
{
    int iVar1;
    int iVar2;

    iVar1 = *param_3;
    iVar2 = *param_4;
    *param_3 = (param_1 * (iVar1 >> 6) - param_2 * (iVar2 >> 6)) >> 6;
    *param_4 = (param_2 * (iVar1 >> 6) + param_1 * (iVar2 >> 6)) >> 6;
    iVar1 = param_3[4] >> 6;
    iVar2 = param_4[4] >> 6;
    param_3[4] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[4] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    iVar1 = param_3[8] >> 6;
    iVar2 = param_4[8] >> 6;
    param_3[8] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[8] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    iVar1 = param_3[0xc] >> 6;
    iVar2 = param_4[0xc] >> 6;
    param_3[0xc] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[0xc] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    return;
}

// FUN_02016178 @ 0x02016178 (220 bytes) — Apply 2D rotation to 3x3 subset of transform
void FUN_02016178(int param_1, int param_2, int *param_3, int *param_4)
{
    int iVar1;
    int iVar2;

    iVar1 = *param_3;
    iVar2 = *param_4;
    *param_3 = (param_1 * (iVar1 >> 6) - param_2 * (iVar2 >> 6)) >> 6;
    *param_4 = (param_2 * (iVar1 >> 6) + param_1 * (iVar2 >> 6)) >> 6;
    iVar1 = param_3[3] >> 6;
    iVar2 = param_4[3] >> 6;
    param_3[3] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[3] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    iVar1 = param_3[6] >> 6;
    iVar2 = param_4[6] >> 6;
    param_3[6] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[6] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    iVar1 = param_3[9] >> 6;
    iVar2 = param_4[9] >> 6;
    param_3[9] = (param_1 * iVar1 - param_2 * iVar2) >> 6;
    param_4[9] = (param_2 * iVar1 + param_1 * iVar2) >> 6;
    return;
}

// FUN_02013e58 @ 0x02013e58 (252 bytes) — Apply character position offset to sprite transform
void FUN_02013e58(int *param_1, int param_2, int param_3)
{
    u32 uVar1;
    int local_30;
    int local_2c[2];

    uVar1 = 0;
    do {
        local_30 = 0;
        local_2c[0] = 0;
        (*(void (*)(int *, u32, int *, int *))(*(int *)(*(int *)param_1 + 0x9c)))(param_1, uVar1 & 0xff, &local_30, local_2c);
        if (((u32)(*(int *)(param_1[0x18d] + 8) << 0x18) >> 0x1d & (1 << (uVar1 & 0xff))) == 0) {
            local_30 = local_30 * param_2;
            local_2c[0] = local_2c[0] * param_3;
        } else {
            local_30 = local_30 + param_1[uVar1 + 0x1cd] + (param_2 - param_1[0x1d3]) * 0x100;
            local_2c[0] = local_2c[0] + param_1[uVar1 + 0x1d0] + (param_3 - param_1[0x1d4]) * 0x100;
        }
        (*(void (*)(int *, u32, int, int))(*(int *)(*(int *)param_1 + 0x14)))(param_1, uVar1 & 0xff, local_30, local_2c[0]);
        uVar1 = uVar1 + 1;
    } while ((int)uVar1 < 3);
    param_1[0x1d3] = param_2;
    param_1[0x1d4] = param_3;
    return;
}

/* ===================================================================
 * Additional forward declarations for functions below
 * =================================================================== */
typedef int code(...);

extern void FUN_0202cd68(...);
extern void FUN_0202cc94(...);
extern u32  FUN_02029bf8(u32, u8, u32, u32);
extern void FUN_0202cc10(u32, u32, u32);
extern void FUN_0203b914(u32, u32, u32);
extern u32  FUN_0202d964(...);
extern void FUN_0203b7f0(u32, u32, u32);
extern void FUN_0203a04c(int, u32);
extern void FUN_02037dd0(void);
extern void FUN_02037d50(u32, u32);
extern void FUN_02037cec(void);
extern void FUN_0203867c(u32, u32, u32);
extern void FUN_02037b8c(void);
extern void FUN_02037b18(u32, u32);
extern void FUN_02037acc(void);
extern void FUN_02038614(u32, u32, u32);
extern int  FUN_0202dce4(int, u32);

/* Static data references for new functions */
static u32  DAT_02013264;
static u32  DAT_0201315c;
static u32  DAT_02013160;
static u16  DAT_020133b4;
static u32 *DAT_020133b8;
static u32  DAT_020133bc;
static u32  DAT_02013024;
static u32  DAT_02013028;
static u32  DAT_02013e54;
static u32  DAT_02013b2c;
static u32  DAT_02013b30;
static u32  DAT_02013b34;
static u16 *DAT_020143dc;
static u16 *DAT_020143e0;
static u16 *DAT_020143e4;
static u16 *DAT_020143ec;
static void *DAT_020143e8;
static u16 *DAT_020143f0;
static u16 *DAT_020143f4;
static void *DAT_020143f8;
static u16 *DAT_02012880;
static u32  DAT_02012320[15];

#define _DAT_04000000 (*(vu32 *)0x04000000)
#define _DAT_05000000 (*(vu16 *)0x05000000)

// FUN_02012174 @ 0x02012174 (108 bytes) — Set animation bank and optionally init sub-animations
void FUN_02012174(int *param_1, int param_2, u8 param_3, int param_4)
{
    u32 uVar1;

    param_1[0x2a] = param_2;
    *(u8 *)((int)param_1 + 0x78b) = param_3;
    if (param_4 == 0) {
        return;
    }
    uVar1 = 0;
    if (*(char *)((int)param_1 + 0x78b) != '\0') {
        do {
            (**(code **)(*param_1 + 0xac))(param_1, uVar1 & 0xff);
            uVar1 = uVar1 + 1;
        } while ((int)uVar1 < (int)(u32)*(u8 *)((int)param_1 + 0x78b));
        return;
    }
    return;
}

// FUN_0201220c @ 0x0201220c (276 bytes) — Look up display config from static table by axis index
void FUN_0201220c(int param_1, int param_2, u32 *param_3, u32 *param_4)
{
    u32 local_58[16];

    local_58[0] = *DAT_02012320;
    local_58[1] = DAT_02012320[1];
    local_58[2] = DAT_02012320[2];
    local_58[3] = DAT_02012320[3];
    local_58[4] = DAT_02012320[4];
    local_58[5] = DAT_02012320[5];
    local_58[6] = DAT_02012320[6];
    local_58[7] = DAT_02012320[7];
    local_58[8] = DAT_02012320[8];
    local_58[9] = DAT_02012320[9];
    local_58[10] = DAT_02012320[10];
    local_58[0xb] = DAT_02012320[0xb];
    local_58[0xc] = DAT_02012320[0xc];
    local_58[0xd] = DAT_02012320[0xd];
    local_58[0xe] = DAT_02012320[0xe];
    if (param_2 == 0) {
        *param_3 = local_58[(u32)(*(int *)(*(int *)(param_1 + 0x634) + 8) << 0x14) >> 0x1c];
        *param_4 = local_58[(u32)(*(int *)(*(int *)(param_1 + 0x634) + 8) << 0x10) >> 0x1c];
        return;
    }
    if (param_2 != 1) {
        if (param_2 != 2) {
            return;
        }
        *param_3 = local_58[(u32)(*(int *)(*(int *)(param_1 + 0x634) + 8) << 4) >> 0x1c];
        *param_4 = local_58[*(u32 *)(*(int *)(param_1 + 0x634) + 8) >> 0x1c];
        return;
    }
    *param_3 = local_58[(u32)(*(int *)(*(int *)(param_1 + 0x634) + 8) << 0xc) >> 0x1c];
    *param_4 = local_58[(u32)(*(int *)(*(int *)(param_1 + 0x634) + 8) << 8) >> 0x1c];
    return;
}

// FUN_02012340 @ 0x02012340 (280 bytes) — Advance timeline frame and update sprite cell index
void FUN_02012340(int param_1)
{
    u16 *puVar1;
    int iVar2;

    if (*(int *)(param_1 + 0x728) == 0) {
        return;
    }
    *(int *)(param_1 + 0x72c) = *(int *)(param_1 + 0x72c) - *(int *)(param_1 + 0x760);
    if (*(int *)(param_1 + 0x72c) < 1) {
        *(short *)(param_1 + 0x730) = *(short *)(param_1 + 0x730) + 1;
        puVar1 = *(u16 **)(param_1 + 0x728);
        iVar2 = *(int *)(param_1 + 0x728) + 4;
        if (*puVar1 <= *(u16 *)(param_1 + 0x730)) {
            *(u16 *)(param_1 + 0x730) = 0;
            puVar1 = *(u16 **)(param_1 + 0x728);
            if ((int)((u32)puVar1[1] << 0x1f) < 0) {
                *(u32 *)(param_1 + 0x728) = 0;
                return;
            }
        }
        (**(code **)(**(int **)(param_1 + 0x2c) + 0x2c))
                  (*(int **)(param_1 + 0x2c), (((u32)puVar1[1] << 0x19) >> 0x1b) << 1,
                   (((u32)puVar1[1] << 0x14) >> 0x1b) << 1,
                   *(u16 *)(iVar2 + (u32)*(u16 *)(param_1 + 0x730) * 4) & 0x1f);
        *(u32 *)(param_1 + 0x72c) =
             *(int *)(param_1 + 0x72c) +
             (u32)*(u16 *)(iVar2 + (u32)*(u16 *)(param_1 + 0x730) * 4 + 2) * 0x100;
        return;
    }
    return;
}

// FUN_02012458 @ 0x02012458 (184 bytes) — Start animation keyframe and set up initial cell display
u32 FUN_02012458(int param_1, int param_2)
{
    int iVar1;
    u32 uVar2;

    iVar1 = *(int *)(param_1 + 0x64c) +
            (*(u32 *)(*(int *)(param_1 + 0x64c) + param_2 * 4) & 0xfffffffc);
    if ((int)((u32)*(u16 *)(iVar1 + 2) << 0x1e) < 0) {
        *(int *)(param_1 + 0x728) = iVar1;
        uVar2 = (u32)*(u16 *)(*(int *)(param_1 + 0x728) + 2);
        (**(code **)(**(int **)(param_1 + 0x2c) + 0x2c))
                  (*(int **)(param_1 + 0x2c), ((uVar2 << 0x19) >> 0x1b) << 1,
                   ((uVar2 << 0x14) >> 0x1b) << 1, *(u16 *)(*(int *)(param_1 + 0x728) + 4) & 0x1f);
        *(u32 *)(param_1 + 0x72c) = (u32)*(u16 *)(*(int *)(param_1 + 0x728) + 6) << 8;
        *(u16 *)(param_1 + 0x730) = 0;
        return 1;
    }
    return 0;
}

// FUN_02012530 @ 0x02012530 (300 bytes) — Allocate and initialize OBJ tile buffers per layer
void FUN_02012530(int param_1)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;
    int iVar4;
    int iVar5;
    u32 uVar6;

    iVar5 = -1;
    uVar1 = FUN_02029bf8(0x200, *(u8 *)(param_1 + 0x784), 0, 0);
    *(u32 *)(param_1 + 0x90) = uVar1;
    FUN_0202cc10(*(u32 *)(param_1 + 0x90), 0, 0x200);
    uVar6 = 0;
    do {
        uVar3 = *(u32 *)(param_1 + uVar6 * 8 + 0x618);
        if (uVar3 != 0) {
            if (((u32)*(u8 *)(param_1 + 0x783) & 1 << (uVar6 & 0xff)) == 0) {
                iVar5 = (int)(char)uVar6;
            }
            else {
                uVar1 = FUN_02029bf8(uVar3 & 0xfffffffe, *(u8 *)(param_1 + 0x784), 0, 0);
                iVar2 = param_1 + uVar6 * 4;
                *(u32 *)(iVar2 + 0x80) = uVar1;
                iVar4 = param_1 + uVar6 * 8;
                FUN_0203b7f0(*(u32 *)(iVar4 + 0x61c), *(u32 *)(iVar2 + 0x80),
                             *(u32 *)(iVar4 + 0x618));
            }
        }
        uVar6 = uVar6 + 1;
    } while ((int)uVar6 < 3);
    uVar1 = FUN_02029bf8(0x200, *(u8 *)(param_1 + 0x784), 0, 0);
    *(u32 *)(param_1 + 0x8c) = uVar1;
    if (iVar5 != -1) {
        uVar3 = *(u32 *)(param_1 + 0x8c);
        uVar6 = *(u32 *)(param_1 + iVar5 * 8 + 0x61c);
        if (uVar6 < uVar3) {
            FUN_0202cd68(uVar6, uVar3, 0x200);
        }
        else {
            FUN_0202cc94(uVar6, uVar3, 0x200);
        }
        uVar3 = *(u32 *)(param_1 + 0x90);
        uVar6 = *(u32 *)(param_1 + iVar5 * 8 + 0x61c);
        if (uVar3 <= uVar6) {
            FUN_0202cc94(uVar6, uVar3, 0x200);
            return;
        }
        FUN_0202cd68(uVar6, uVar3, 0x200);
        return;
    }
    return;
}

// FUN_0201265c @ 0x0201265c (548 bytes) — Flush OBJ tile data to VRAM for main or sub screen
void FUN_0201265c(int param_1)
{
    u8 bVar1;

    if (*(int *)(param_1 + 0x80) != 0) {
        FUN_0203a04c(*(int *)(param_1 + 0x80), *(u32 *)(param_1 + 0x618));
    }
    if (*(int *)(param_1 + 0x84) != 0) {
        FUN_0203a04c(*(int *)(param_1 + 0x84), *(u32 *)(param_1 + 0x620));
    }
    if (*(int *)(param_1 + 0x88) != 0) {
        FUN_0203a04c(*(int *)(param_1 + 0x88), *(u32 *)(param_1 + 0x628));
    }
    FUN_0203a04c(*(u32 *)(param_1 + 0x8c), 0x200);
    if (*(char *)(param_1 + 0x782) == '\0') {
        FUN_02037dd0();
        if (((*(u8 *)(param_1 + 0x783) & 1) != 0) && (*(int *)(param_1 + 0x618) != 0)) {
            FUN_02037d50(*(u32 *)(param_1 + 0x80), 0x2000);
        }
        if (((*(u8 *)(param_1 + 0x783) & 2) != 0) && (*(int *)(param_1 + 0x620) != 0)) {
            FUN_02037d50(*(u32 *)(param_1 + 0x84), 0x4000);
        }
        if (((*(u8 *)(param_1 + 0x783) & 4) != 0) && (*(int *)(param_1 + 0x628) != 0)) {
            FUN_02037d50(*(u32 *)(param_1 + 0x88), 0x6000);
        }
        FUN_02037cec();
        bVar1 = *(u8 *)(param_1 + 0x783);
        if (((((bVar1 & 1) != 0) && ((bVar1 & 2) != 0)) && ((bVar1 & 4) != 0)) &&
           (-1 < (int)((u32)*(u8 *)(param_1 + 0x78c) << 0x1b))) {
            _DAT_05000000 = **(u16 **)(param_1 + 0x62c);
            **(u16 **)(param_1 + 0x90) = **(u16 **)(param_1 + 0x62c);
            return;
        }
        FUN_0203867c(*(u32 *)(param_1 + 0x8c), 0, 0x200);
        return;
    }
    FUN_02037b8c();
    if (((*(u8 *)(param_1 + 0x783) & 1) != 0) && (*(int *)(param_1 + 0x618) != 0)) {
        FUN_02037b18(*(u32 *)(param_1 + 0x80), 0x2000);
    }
    if (((*(u8 *)(param_1 + 0x783) & 2) != 0) && (*(int *)(param_1 + 0x620) != 0)) {
        FUN_02037b18(*(u32 *)(param_1 + 0x84), 0x4000);
    }
    if (((*(u8 *)(param_1 + 0x783) & 4) != 0) && (*(int *)(param_1 + 0x628) != 0)) {
        FUN_02037b18(*(u32 *)(param_1 + 0x88), 0x6000);
    }
    FUN_02037acc();
    bVar1 = *(u8 *)(param_1 + 0x783);
    if (((((bVar1 & 1) != 0) && ((bVar1 & 2) != 0)) && ((bVar1 & 4) != 0)) &&
       (-1 < (int)((u32)*(u8 *)(param_1 + 0x78c) << 0x1b))) {
        *DAT_02012880 = **(u16 **)(param_1 + 0x62c);
        **(u16 **)(param_1 + 0x90) = **(u16 **)(param_1 + 0x62c);
        return;
    }
    FUN_02038614(*(u32 *)(param_1 + 0x8c), 0, 0x200);
    return;
}

// FUN_02012884 @ 0x02012884 (424 bytes) — Process blend keyframe timers and update blend state
void FUN_02012884(int *param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int local_30;

    iVar3 = -1;
    (**(code **)(*param_1 + 0x7c))();
    iVar4 = 0;
    if (*(u8 *)((int)param_1 + 0x78a) != 0xff) {
        local_30 = 0;
        do {
            if ((*(u8 *)(param_1[0x1d7] + iVar4) & 0xf) != 0) {
                iVar1 = FUN_0202d964(param_1[0x191], 1, iVar4 + -1);
                param_1[0x27] = iVar1;
                iVar1 = FUN_0202d964(param_1[0x191], 2, iVar4 + -1);
                param_1[0x28] = iVar1;
                iVar1 = FUN_0202d964(param_1[0x191], 3, iVar4 + -1);
                param_1[0x29] = iVar1;
                *(int *)(param_1[0x1d6] + iVar4 * 4) = *(int *)(param_1[0x1d6] + iVar4 * 4) + param_1[0x1d8];
                iVar1 = local_30;
                do {
                    if ((param_1[iVar1 * 2 + 0x186] != 0) &&
                       (iVar2 = FUN_0202dce4(param_1[iVar1 + 0x27], *(u32 *)(param_1[0x1d6] + iVar4 * 4)),
                        iVar3 < iVar2)) {
                        iVar3 = iVar2;
                    }
                    iVar1 = iVar1 + 1;
                } while (iVar1 < 3);
                if (iVar3 * 0x100 <= *(int *)(param_1[0x1d6] + iVar4 * 4)) {
                    if ((*(u8 *)(param_1[0x1d7] + iVar4) & 0xf0) == 0) {
                        *(u8 *)(param_1[0x1d7] + iVar4) = 0;
                    }
                    *(u32 *)(param_1[0x1d6] + iVar4 * 4) = 0;
                }
            }
            iVar4 = iVar4 + 1;
        } while (iVar4 < (int)(*(u8 *)((int)param_1 + 0x78a) + 1));
    }
    iVar3 = (**(code **)(*param_1 + 0x5c))();
    if (iVar3 == 0) {
        *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xfc;
    }
    else {
        *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xfc | 1;
    }
    return;
}

// FUN_02012a2c @ 0x02012a2c (184 bytes) — Sync OBJ tile data between active layers
void FUN_02012a2c(int param_1)
{
    u32 uVar1;
    int iVar2;
    u32 uVar3;
    u32 uVar4;

    uVar4 = 0;
    uVar1 = 0xffffffff;
    do {
        iVar2 = param_1 + uVar4 * 8;
        uVar3 = uVar1;
        if ((*(int *)(iVar2 + 0x618) != 0) &&
           (uVar3 = uVar4, ((u32)*(u8 *)(param_1 + 0x783) & 1 << (uVar4 & 0xff)) != 0)) {
            if (*(u32 *)(iVar2 + 0x61c) < *(u32 *)(param_1 + uVar4 * 4 + 0x80)) {
                FUN_0202cd68();
                uVar3 = uVar1;
            }
            else {
                FUN_0202cc94();
                uVar3 = uVar1;
            }
        }
        uVar4 = uVar4 + 1;
        uVar1 = uVar3;
    } while ((int)uVar4 < 3);
    if (uVar3 == 0xffffffff) {
        return;
    }
    uVar4 = *(u32 *)(param_1 + 0x8c);
    uVar1 = *(u32 *)(param_1 + 0x90);
    if (uVar1 < uVar4) {
        FUN_0202cd68(uVar1, uVar4, 0x200);
        return;
    }
    FUN_0202cc94(uVar1, uVar4, 0x200);
    return;
}

// FUN_02012ae4 @ 0x02012ae4 (44 bytes) — Trigger virtual update and mark refresh flag
void FUN_02012ae4(int *param_1)
{
    (**(code **)(*param_1 + 0x7c))();
    *(u8 *)(param_1[0xb] + 0xfd) = *(u8 *)(param_1[0xb] + 0xfd) | 2;
    return;
}

// FUN_02012b44 @ 0x02012b44 (64 bytes) — Clear blend channel lower nibble and check state
void FUN_02012b44(int *param_1, int param_2)
{
    int iVar1;

    *(u8 *)(param_1[0x1d7] + param_2 + 1) = *(u8 *)(param_1[0x1d7] + param_2 + 1) & 0xf0;
    iVar1 = (**(code **)(*param_1 + 0x5c))();
    if (iVar1 == 0) {
        *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xfc;
    }
    return;
}

// FUN_02012b84 @ 0x02012b84 (104 bytes) — Reset all blend channel entries
void FUN_02012b84(int *param_1)
{
    int iVar1;

    iVar1 = 0;
    if (*(u8 *)((int)param_1 + 0x78a) != 0xff) {
        do {
            *(u8 *)(param_1[0x1d7] + iVar1) = 0;
            *(u32 *)(param_1[0x1d6] + iVar1 * 4) = 0;
            iVar1 = iVar1 + 1;
        } while (iVar1 < (int)(*(u8 *)((int)param_1 + 0x78a) + 1));
    }
    *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xfc;
    (**(code **)(*param_1 + 0x78))();
    return;
}

// FUN_02012ca0 @ 0x02012ca0 (280 bytes) — Initialize blend resource pointers and curve data
void FUN_02012ca0(int param_1)
{
    u32 uVar1;
    u16 *puVar2;

    puVar2 = *(u16 **)(param_1 + 0x644);
    *(char *)(param_1 + 0x78a) = (char)puVar2[1] - (char)*puVar2;
    uVar1 = FUN_02029bf8((*(u8 *)(param_1 + 0x78a) + 1) * 4, *(u8 *)(param_1 + 0x784), 0, 0);
    *(u32 *)(param_1 + 0x758) = uVar1;
    FUN_0202cc10(*(u32 *)(param_1 + 0x758), 0, (*(u8 *)(param_1 + 0x78a) + 1) * 4);
    uVar1 = FUN_02029bf8(*(u8 *)(param_1 + 0x78a) + 1, *(u8 *)(param_1 + 0x784), 0, 0);
    *(u32 *)(param_1 + 0x75c) = uVar1;
    FUN_0203b914(*(u32 *)(param_1 + 0x75c), 0, *(u8 *)(param_1 + 0x78a) + 1);
    uVar1 = FUN_0202d964(puVar2, 1, 0xffffffff);
    *(u32 *)(param_1 + 0x9c) = uVar1;
    uVar1 = FUN_0202d964(puVar2, 2, 0xffffffff);
    *(u32 *)(param_1 + 0xa0) = uVar1;
    uVar1 = FUN_0202d964(puVar2, 3, 0xffffffff);
    *(u32 *)(param_1 + 0xa4) = uVar1;
    if (((*(int *)(param_1 + 0x9c) == 0) && (*(int *)(param_1 + 0xa0) == 0)) &&
       (*(int *)(param_1 + 0xa4) == 0)) {
        return;
    }
    *(u8 *)(param_1 + 0x78c) = *(u8 *)(param_1 + 0x78c) & 0xfc | 1;
    **(u8 **)(param_1 + 0x75c) = 0x11;
    return;
}

// FUN_02012e6c @ 0x02012e6c (440 bytes) — Advance multi-channel animation timers and copy tile data
void FUN_02012e6c(int *param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    u32 *puVar4;
    u32 *puVar5;
    u32 *puVar6;
    int *piVar7;
    int iVar8;

    iVar8 = 0;
    if ((char)param_1[0x1e2] == '\0') {
        return;
    }
    puVar6 = (u32 *)(param_1 + 0x19c);
    piVar7 = param_1;
    do {
        if (((char)piVar7[0x19c] == '\x01') &&
           (piVar7[0x19b] = piVar7[0x19b] - param_1[0x1d8], piVar7[0x19b] < 1)) {
            puVar4 = (u32 *)piVar7[0x19a];
            iVar1 = (**(code **)(*param_1 + 0xc0))(param_1, *puVar4 & 3);
            iVar3 = *(int *)piVar7[0x19a];
            *puVar6 = *puVar6 & DAT_02013024 | (((*puVar6 << 8) >> 0x10) + 1 & DAT_02013028) << 8;
            puVar5 = (u32 *)piVar7[0x19a];
            iVar3 = iVar1 * ((u32)(iVar3 << 8) >> 0x16);
            if ((u32)*(u16 *)((int)puVar5 + 6) <= (*puVar6 << 8) >> 0x10) {
                *puVar6 = *puVar6 & DAT_02013024;
                puVar5 = (u32 *)piVar7[0x19a];
                if (((*puVar5 << 0x1c) >> 0x1e & 1) != 0) {
                    *(u8 *)(piVar7 + 0x19c) = 0;
                    goto LAB_02013000;
                }
            }
            iVar2 = (**(code **)(*param_1 + 0x98))
                              (param_1, *(u8 *)((int)param_1 + (*puVar5 & 3) + 0x77f));
            (**(code **)(*(int *)param_1[0xb] + 0x24))
                      ((int *)param_1[0xb],
                       param_1[iVar8 + 0xd] +
                       ((int)(iVar3 * (u32)(u16)puVar4[((*puVar6 << 8) >> 0x10) + 2]) >> 2) * 4,
                       iVar2 + (iVar1 * ((*puVar5 << 0x12) >> 0x16) & 0xfffffffe), iVar3);
            piVar7[0x19b] =
                 piVar7[0x19b] +
                 (u32)*(u16 *)((int)puVar4 + ((*puVar6 << 8) >> 0x10) * 4 + 10) * 0x100;
        }
LAB_02013000:
        iVar8 = iVar8 + 1;
        piVar7 = piVar7 + 3;
        puVar6 = puVar6 + 3;
        if ((int)(u32)*(u8 *)(param_1 + 0x1e2) <= iVar8) {
            return;
        }
    } while (1);
}

// FUN_0201302c @ 0x0201302c (304 bytes) — Start channel animation and copy first frame tile data
u32 FUN_0201302c(int *param_1, int param_2)
{
    int iVar1;
    u32 uVar2;
    int iVar3;
    int iVar4;
    u32 *puVar5;
    int iVar6;
    int iVar7;

    iVar1 = DAT_0201315c;
    iVar6 = param_2 * 0xc;
    if ((1 < (*(u32 *)param_1[param_2 * 3 + 0x19a] << 0x1c) >> 0x1e) &&
       ((char)param_1[param_2 * 3 + 0x19c] == '\0')) {
        iVar3 = (**(code **)(*param_1 + 0xc0))(param_1, *(u32 *)param_1[param_2 * 3 + 0x19a] & 3);
        iVar4 = **(int **)((int)param_1 + iVar6 + iVar1);
        *(u8 *)(param_1 + param_2 * 3 + 0x19c) = 1;
        puVar5 = *(u32 **)((int)param_1 + iVar6 + iVar1);
        iVar7 = iVar3 * ((u32)(iVar4 << 8) >> 0x16);
        iVar4 = (**(code **)(*param_1 + 0x98))
                          (param_1, *(u8 *)((int)param_1 + (*puVar5 & 3) + 0x77f));
        (**(code **)(*(int *)param_1[0xb] + 0x24))
                  ((int *)param_1[0xb],
                   param_1[param_2 + 0xd] +
                   ((int)(iVar7 * (u32)*(u16 *)(*(int *)((int)param_1 + iVar6 + iVar1) + 8)) >> 2) *
                   4, iVar4 + (iVar3 * ((*puVar5 << 0x12) >> 0x16) & 0xfffffffe), iVar7);
        uVar2 = DAT_02013160;
        param_1[param_2 * 3 + 0x19b] =
             (u32)*(u16 *)(*(int *)((int)param_1 + iVar6 + iVar1) + 10) << 8;
        param_1[param_2 * 3 + 0x19c] = param_1[param_2 * 3 + 0x19c] & uVar2;
        return 1;
    }
    return 0;
}

// FUN_02013178 @ 0x02013178 (236 bytes) — Copy tilemap data from animation resource to layer buffers
void FUN_02013178(int param_1, int param_2)
{
    u16 uVar1;
    u16 uVar2;
    u16 uVar3;
    u16 uVar4;
    u32 uVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    int iVar10;
    int iVar11;
    int iVar12;
    u32 uVar13;
    u32 local_30;

    uVar5 = DAT_02013264;
    param_2 = param_2 * 0x14;
    local_30 = 0;
    do {
        iVar7 = param_2 + *(int *)(param_1 + 0xac);
        uVar13 = (u32)*(u16 *)(iVar7 + 0xc);
        iVar9 = 0;
        iVar10 = uVar13 * *(u16 *)(iVar7 + 0xe);
        iVar6 = *(int *)(param_2 + *(int *)(param_1 + 0xac));
        iVar8 = *(int *)(param_1 + local_30 * 8 + 0x604);
        iVar12 = iVar9;
        if (*(u16 *)(iVar7 + 0xe) != 0) {
            do {
                uVar1 = *(u16 *)(iVar7 + 10);
                uVar2 = *(u16 *)(param_1 + 0x76a);
                uVar3 = *(u16 *)(iVar7 + 8);
                iVar11 = 0;
                if (uVar13 != 0) {
                    do {
                        uVar4 = *(u16 *)(iVar6 + local_30 * iVar10 * 2 + iVar9 * 2);
                        iVar9 = iVar9 + 1;
                        if (uVar4 != uVar5) {
                            *(u16 *)
                             (iVar11 * 2 + iVar8 + ((u32)uVar2 * (iVar12 + (u32)uVar1) + (u32)uVar3) * 2) =
                                 uVar4;
                        }
                        iVar11 = iVar11 + 1;
                        iVar7 = param_2 + *(int *)(param_1 + 0xac);
                        uVar13 = (u32)*(u16 *)(iVar7 + 0xc);
                    } while (iVar11 < (int)uVar13);
                }
                iVar12 = iVar12 + 1;
            } while (iVar12 < (int)(u32)*(u16 *)(iVar7 + 0xe));
        }
        local_30 = local_30 + 1;
    } while (local_30 < 3);
    return;
}

// FUN_02013268 @ 0x02013268 (332 bytes) — Apply animation event: set/clear OBJ enable bits and reposition
u32 FUN_02013268(int *param_1, u32 param_2)
{
    u16 uVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar3 = param_2 * 0x14;
    uVar1 = *(u16 *)(param_1[0x2b] + iVar3 + 0x10);
    if (uVar1 == DAT_020133b4) {
        iVar2 = (int)param_2 >> 0x1f;
        iVar4 = DAT_020133bc + (u32)*(u8 *)((int)param_1 + 0x782) * 8;
        *(u32 *)(iVar4 + (param_2 >> 5) * 4) =
             *(u32 *)(iVar4 + (param_2 >> 5) * 4) |
             1 << ((param_2 * 0x8000000 + iVar2 >> 0x1b | iVar2 << 5) - iVar2 & 0xff);
    }
    else {
        *(u32 *)(*DAT_020133b8 + 0x50 + (u32)(uVar1 >> 5) * 4) =
             *(u32 *)(*DAT_020133b8 + 0x50 + (u32)(uVar1 >> 5) * 4) | 1 << (uVar1 & 0x1f);
    }
    uVar1 = *(u16 *)(param_1[0x2b] + iVar3 + 0x12);
    if (uVar1 != DAT_020133b4) {
        *(u32 *)(*DAT_020133b8 + 0x50 + (u32)(uVar1 >> 5) * 4) =
             *(u32 *)(*DAT_020133b8 + 0x50 + (u32)(uVar1 >> 5) * 4) & ~(1 << (uVar1 & 0x1f));
    }
    if (*(int *)(param_1[0x2b] + iVar3) != 0) {
        (**(code **)(*param_1 + 0x3c))();
        (**(code **)(*param_1 + 0x14))(param_1, 0, param_1[0x1cd], param_1[0x1d0]);
        (**(code **)(*param_1 + 0x14))(param_1, 1, param_1[0x1ce], param_1[0x1d1]);
        (**(code **)(*param_1 + 0x14))(param_1, 2, param_1[0x1cf], param_1[0x1d2]);
    }
    return *(u32 *)(param_1[0x2b] + iVar3 + 4);
}

// FUN_02013438 @ 0x02013438 (1780 bytes) — Incremental BG scroll update with tile wrapping
void FUN_02013438(int param_1, u32 param_2, int param_3, int param_4)
{
    BOOL bVar1;
    BOOL bVar2;
    int iVar3;
    u16 uVar4;
    int iVar5;
    int extraout_r1 = 0;
    int extraout_r1_00 = 0;
    int extraout_r1_01 = 0;
    int extraout_r1_02 = 0;
    int extraout_r1_03 = 0;
    int extraout_r1_04 = 0;
    int extraout_r1_05 = 0;
    int extraout_r1_06 = 0;
    int extraout_r1_07 = 0;
    int extraout_r1_08 = 0;
    int extraout_r1_09 = 0;
    int extraout_r1_10 = 0;
    int *piVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    u32 uVar10;
    int iVar11;
    int iVar12;
    int iVar13;
    int iStack_68;
    int iStack_64;
    int iStack_60;
    int iStack_5c;
    int iStack_58;
    int iStack_54;

    bVar1 = FALSE;
    iVar13 = param_2 * 4;
    if (*(int *)((u32)*(u16 *)(param_1 + 0x764) * 0x14 + DAT_02013b2c + param_2 * 4) == -1) {
        return;
    }
    iVar11 = param_1 + iVar13;
    iVar5 = param_1 + DAT_02013b30;
    iVar9 = *(int *)(iVar11 + 0x734);
    iVar7 = *(int *)(iVar11 + 0x740);
    *(int *)(iVar5 + iVar13) = *(int *)(iVar5 + iVar13) + param_3;
    *(int *)(param_1 + 0x740 + iVar13) = *(int *)(param_1 + 0x740 + iVar13) + param_4;
    iStack_60 = (iVar9 << 5) >> 0x10;
    iStack_5c = (iVar7 << 5) >> 0x10;
    iVar9 = *(int *)(param_1 + 0x774);
    iVar7 = *(int *)(iVar11 + 0x734) >> 8;
    piVar6 = (int *)(iVar5 + iVar13);
    if (iVar7 < iVar9) {
        if (iVar7 <= -iVar9) {
            *piVar6 = *piVar6 + iVar9 * 0x100;
            iStack_60 = (iStack_60 + (*(int *)(param_1 + 0x774) >> 3)) * 0x10000 >> 0x10;
        }
    }
    else {
        *piVar6 = *piVar6 + iVar9 * -0x100;
        iStack_60 = (iStack_60 - (*(int *)(param_1 + 0x774) >> 3)) * 0x10000 >> 0x10;
    }
    iVar9 = *(int *)(param_1 + 0x778);
    iVar7 = *(int *)(param_1 + param_2 * 4 + 0x740) >> 8;
    iVar13 = param_2 * 4;
    if (iVar7 < iVar9) {
        if (iVar7 <= -iVar9) {
            *(int *)(param_1 + 0x740 + iVar13) = *(int *)(param_1 + 0x740 + iVar13) + iVar9 * 0x100;
            iStack_5c = (iStack_5c + (*(int *)(param_1 + 0x778) >> 3)) * 0x10000 >> 0x10;
        }
    }
    else {
        *(int *)(param_1 + 0x740 + iVar13) = *(int *)(param_1 + 0x740 + iVar13) + iVar9 * -0x100;
        iStack_5c = (iStack_5c - (*(int *)(param_1 + 0x778) >> 3)) * 0x10000 >> 0x10;
    }
    iVar7 = param_1 + param_2 * 4;
    iVar13 = (*(int *)(iVar7 + 0x734) << 5) >> 0x10;
    iVar7 = (*(int *)(iVar7 + 0x740) << 5) >> 0x10;
    if ((iStack_60 == iVar13) && (iStack_5c == iVar7)) {
        return;
    }
    if (iStack_60 != iVar13) {
        if (iVar13 < iStack_60) {
            iVar9 = iVar13 + -1;
        }
        else {
            iVar9 = iVar13 + 0x20;
        }
        if (iVar9 < 0) {
            uVar10 = (u32)*(u16 *)(param_1 + 0x76a);
            bVar1 = TRUE;
            FUN_020464b0(iVar9, uVar10);
            FUN_020464b0(uVar10 + extraout_r1, uVar10);
            iStack_64 = extraout_r1_00;
        }
        else {
            iStack_64 = iVar9;
            if ((int)(u32)*(u16 *)(param_1 + 0x76a) <= iVar9) {
                bVar1 = TRUE;
                FUN_020464b0();
                iStack_64 = extraout_r1_01;
            }
        }
        iVar5 = iVar9 >> 0x1f;
        if (iVar9 < 0) {
            iVar5 = ((u32)(iVar9 * 0x4000000 + iVar5) >> 0x1a | iVar5 << 6) - iVar5;
            iVar9 = iVar5 + 0x40 >> 0x1f;
            iVar9 = ((u32)(iVar5 * 0x4000000 + iVar9) >> 0x1a | iVar9 << 6) - iVar9;
        }
        else if (0x3f < iVar9) {
            iVar9 = ((u32)(iVar9 * 0x4000000 + iVar5) >> 0x1a | iVar5 << 6) - iVar5;
        }
        iVar11 = iVar7 + -1;
        iVar5 = iVar9 >> 0x1f;
        iStack_54 = -1;
        iVar5 = ((u32)(iVar9 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5;
        do {
            bVar2 = FALSE;
            if (iVar11 < 0) {
                uVar10 = (u32)*(u16 *)(param_1 + 0x76c);
                FUN_020464b0(iVar11, uVar10);
                FUN_020464b0(uVar10 + extraout_r1_02, uVar10);
                iVar8 = extraout_r1_03;
                bVar2 = TRUE;
            }
            else {
                iVar8 = iVar11;
                if ((int)(u32)*(u16 *)(param_1 + 0x76c) <= iVar11) {
                    bVar2 = TRUE;
                    FUN_020464b0(iVar11, (u32)*(u16 *)(param_1 + 0x76c));
                    iVar8 = extraout_r1_04;
                }
            }
            iVar3 = iVar11 >> 0x1f;
            if (iVar11 < 0) {
                iVar3 = ((u32)(iVar11 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3;
                iVar12 = iVar3 + 0x20 >> 0x1f;
                iVar12 = ((u32)(iVar3 * 0x8000000 + iVar12) >> 0x1b | iVar12 << 5) - iVar12;
            }
            else {
                iVar12 = iVar11;
                if (0x1f < iVar11) {
                    iVar12 = ((u32)(iVar11 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3;
                }
            }
            if (((bVar1) || (bVar2)) && (((u32)*(u8 *)(param_1 + 0x786) & 1 << (param_2 & 0xff)) != 0))
            {
                uVar4 = 0;
            }
            else {
                uVar4 = *(u16 *)
                         (*(int *)(param_1 + param_2 * 8 + 0x604) +
                         (iVar8 * (u32)*(u16 *)(param_1 + 0x76a) + iStack_64) * 2);
            }
            iVar8 = iVar12 >> 0x1f;
            if (((int)(iVar9 + ((u32)(iVar9 >> 4) >> 0x1b)) >> 5 & 1U) == 0) {
                *(u16 *)
                 (*(int *)(param_1 + param_2 * 4 + 0x74) +
                 (iVar5 + (((u32)(iVar12 * 0x8000000 + iVar8) >> 0x1b | iVar8 << 5) - iVar8) * 0x20) * 2) =
                     uVar4;
            }
            else {
                *(u16 *)
                 (*(int *)(param_1 + param_2 * 4 + 0x74) +
                 (iVar5 + (((u32)(iVar12 * 0x8000000 + iVar8) >> 0x1b | iVar8 << 5) - iVar8) * 0x20 + 0x400
                 ) * 2) = uVar4;
            }
            iStack_54 = iStack_54 + 1;
            iVar11 = iVar11 + 1;
        } while (iStack_54 < 0x19);
        if (iVar13 < iStack_60) {
            *(u8 *)(param_1 + DAT_02013b34 + param_2) = *(u8 *)(param_1 + DAT_02013b34 + param_2) | 4;
        }
        else {
            *(u8 *)(param_1 + DAT_02013b34 + param_2) = *(u8 *)(param_1 + DAT_02013b34 + param_2) | 8;
        }
    }
    bVar1 = FALSE;
    if (iStack_5c == iVar7) {
        return;
    }
    if (iVar7 < iStack_5c) {
        iVar9 = iVar7 + -1;
    }
    else {
        iVar9 = iVar7 + 0x18;
    }
    if (iVar9 < 0) {
        uVar10 = (u32)*(u16 *)(param_1 + 0x76c);
        bVar1 = TRUE;
        FUN_020464b0(iVar9, uVar10);
        FUN_020464b0(uVar10 + extraout_r1_05, uVar10);
        iStack_68 = extraout_r1_06;
    }
    else {
        iStack_68 = iVar9;
        if ((int)(u32)*(u16 *)(param_1 + 0x76c) <= iVar9) {
            bVar1 = TRUE;
            FUN_020464b0();
            iStack_68 = extraout_r1_07;
        }
    }
    iVar5 = iVar9 >> 0x1f;
    if (iVar9 < 0) {
        iVar5 = ((u32)(iVar9 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5;
        iVar9 = iVar5 + 0x20 >> 0x1f;
        iVar9 = ((u32)(iVar5 * 0x8000000 + iVar9) >> 0x1b | iVar9 << 5) - iVar9;
    }
    else if (0x1f < iVar9) {
        iVar9 = ((u32)(iVar9 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5;
    }
    iVar5 = iVar9 >> 0x1f;
    iVar9 = (((u32)(iVar9 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5) * 0x20;
    iVar13 = iVar13 + -1;
    iStack_58 = -1;
    do {
        bVar2 = FALSE;
        if (iVar13 < 0) {
            bVar2 = TRUE;
            uVar10 = (u32)*(u16 *)(param_1 + 0x76a);
            FUN_020464b0(iVar13, uVar10);
            FUN_020464b0(uVar10 + extraout_r1_08, uVar10);
            iVar5 = extraout_r1_09;
        }
        else {
            uVar10 = (u32)*(u16 *)(param_1 + 0x76a);
            iVar5 = iVar13;
            if ((int)uVar10 <= iVar13) {
                bVar2 = TRUE;
                FUN_020464b0(iVar13, uVar10);
                iVar5 = extraout_r1_10;
            }
        }
        iVar11 = iVar13 >> 0x1f;
        if (iVar13 < 0) {
            iVar11 = ((u32)(iVar13 * 0x4000000 + iVar11) >> 0x1a | iVar11 << 6) - iVar11;
            iVar8 = iVar11 + 0x40 >> 0x1f;
            iVar8 = ((u32)(iVar11 * 0x4000000 + iVar8) >> 0x1a | iVar8 << 6) - iVar8;
        }
        else {
            iVar8 = iVar13;
            if (0x3f < iVar13) {
                iVar8 = ((u32)(iVar13 * 0x4000000 + iVar11) >> 0x1a | iVar11 << 6) - iVar11;
            }
        }
        if (((bVar2) || (bVar1)) && (((u32)*(u8 *)(param_1 + 0x786) & 1 << (param_2 & 0xff)) != 0)) {
            uVar4 = 0;
        }
        else {
            uVar4 = *(u16 *)
                     (*(int *)(param_1 + param_2 * 8 + 0x604) + (iStack_68 * uVar10 + iVar5) * 2);
        }
        iVar5 = iVar8 >> 0x1f;
        if (((int)(iVar8 + ((u32)(iVar8 >> 4) >> 0x1b)) >> 5 & 1U) == 0) {
            *(u16 *)
             (*(int *)(param_1 + param_2 * 4 + 0x74) +
             ((((u32)(iVar8 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5) + iVar9) * 2) = uVar4;
        }
        else {
            *(u16 *)
             (*(int *)(param_1 + param_2 * 4 + 0x74) +
             ((((u32)(iVar8 * 0x8000000 + iVar5) >> 0x1b | iVar5 << 5) - iVar5) + iVar9 + 0x400) * 2) =
                 uVar4;
        }
        iStack_58 = iStack_58 + 1;
        iVar13 = iVar13 + 1;
    } while (iStack_58 < 0x21);
    if (iStack_5c <= iVar7) {
        *(u8 *)(param_1 + DAT_02013b34 + param_2) = *(u8 *)(param_1 + DAT_02013b34 + param_2) | 2;
        return;
    }
    *(u8 *)(param_1 + DAT_02013b34 + param_2) = *(u8 *)(param_1 + DAT_02013b34 + param_2) | 1;
    return;
}

// FUN_02013b38 @ 0x02013b38 (796 bytes) — Full BG tilemap redraw with coordinate wrapping
void FUN_02013b38(int param_1, u32 param_2, int param_3, int param_4)
{
    BOOL bVar1;
    BOOL bVar2;
    int iVar3;
    int iVar4;
    short extraout_r1 = 0;
    short extraout_r1_00 = 0;
    short extraout_r1_01 = 0;
    short extraout_r1_02 = 0;
    u16 uVar5;
    int extraout_r1_03 = 0;
    int extraout_r1_04 = 0;
    int iVar6;
    int iVar7;
    u32 uVar8;
    int iVar9;
    int iVar10;
    u32 local_50;
    u32 local_48;
    u32 local_34;

    local_50 = -1;
    if (*(int *)((u32)*(u16 *)(param_1 + 0x764) * 0x14 + DAT_02013e54 + param_2 * 4) != -1) {
        local_48 = ((param_4 << 5) >> 0x10) + -1;
        local_34 = -1;
        do {
            bVar2 = FALSE;
            iVar7 = (int)(short)local_48;
            if (iVar7 < 0) {
                uVar8 = (u32)*(u16 *)(param_1 + 0x76c);
                bVar2 = TRUE;
                FUN_020464b0(iVar7, uVar8);
                FUN_020464b0(uVar8 + extraout_r1_03, uVar8);
                iVar10 = (int)extraout_r1;
            }
            else {
                iVar10 = iVar7;
                if ((int)(u32)*(u16 *)(param_1 + 0x76c) <= iVar7) {
                    bVar2 = TRUE;
                    FUN_020464b0(iVar7);
                    iVar10 = (int)extraout_r1_00;
                }
            }
            iVar3 = iVar7 >> 0x1f;
            if (iVar7 < 0) {
                iVar3 = ((u32)(iVar7 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3;
                iVar7 = iVar3 + 0x20 >> 0x1f;
                iVar7 = (int)((((u32)(iVar3 * 0x8000000 + iVar7) >> 0x1b | iVar7 << 5) - iVar7) * 0x10000)
                        >> 0x10;
            }
            else if (0x1f < iVar7) {
                iVar7 = (int)((((u32)(iVar7 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3) * 0x10000)
                        >> 0x10;
            }
            iVar3 = iVar7 >> 0x1f;
            iVar7 = (((u32)(iVar7 * 0x8000000 + iVar3) >> 0x1b | iVar3 << 5) - iVar3) * 0x20;
            iVar3 = local_34;
            do {
                bVar1 = FALSE;
                iVar6 = (iVar3 + ((param_3 << 5) >> 0x10)) * 0x10000;
                iVar9 = iVar6 >> 0x10;
                if (iVar9 < 0) {
                    bVar1 = TRUE;
                    uVar8 = (u32)*(u16 *)(param_1 + 0x76a);
                    FUN_020464b0(iVar9, uVar8);
                    FUN_020464b0(uVar8 + extraout_r1_04, uVar8);
                    iVar4 = (int)extraout_r1_01;
                }
                else {
                    uVar8 = (u32)*(u16 *)(param_1 + 0x76a);
                    iVar4 = iVar9;
                    if ((int)uVar8 <= iVar9) {
                        bVar1 = TRUE;
                        FUN_020464b0(iVar9, uVar8);
                        iVar4 = (int)extraout_r1_02;
                    }
                }
                iVar6 = iVar6 >> 0x1f;
                if (iVar9 < 0) {
                    iVar6 = ((u32)(iVar9 * 0x4000000 + iVar6) >> 0x1a | iVar6 << 6) - iVar6;
                    iVar9 = iVar6 + 0x40 >> 0x1f;
                    iVar9 = (int)((((u32)(iVar6 * 0x4000000 + iVar9) >> 0x1a | iVar9 << 6) - iVar9) * 0x10000
                                 ) >> 0x10;
                }
                else if (0x3f < iVar9) {
                    iVar9 = (int)((((u32)(iVar9 * 0x4000000 + iVar6) >> 0x1a | iVar6 << 6) - iVar6) * 0x10000
                                 ) >> 0x10;
                }
                if (((bVar1) || (bVar2)) &&
                   (((u32)*(u8 *)(param_1 + 0x786) & 1 << (param_2 & 0xff)) != 0)) {
                    uVar5 = 0;
                }
                else {
                    uVar5 = *(u16 *)
                             (*(int *)(param_1 + param_2 * 8 + 0x604) + (iVar10 * uVar8 + iVar4) * 2);
                }
                iVar6 = iVar9 >> 0x1f;
                if (((int)(iVar9 + ((u32)(iVar9 >> 4) >> 0x1b)) >> 5 & 1U) == 0) {
                    *(u16 *)
                     (*(int *)(param_1 + param_2 * 4 + 0x74) +
                     ((((u32)(iVar9 * 0x8000000 + iVar6) >> 0x1b | iVar6 << 5) - iVar6) + iVar7) * 2) = uVar5;
                }
                else {
                    *(u16 *)
                     (*(int *)(param_1 + param_2 * 4 + 0x74) +
                     ((((u32)(iVar9 * 0x8000000 + iVar6) >> 0x1b | iVar6 << 5) - iVar6) + iVar7 + 0x400) * 2)
                         = uVar5;
                }
                iVar3 = (iVar3 + 1) * 0x10000 >> 0x10;
            } while (iVar3 < 0x21);
            local_50 = (local_50 + 1) * 0x10000 >> 0x10;
            local_48 = local_48 + 1;
        } while (local_50 < 0x19);
    }
    *(u8 *)(param_1 + param_2 + 0x77c) = 0xff;
    param_1 = param_1 + param_2 * 4;
    *(int *)(param_1 + 0x734) = param_3;
    *(int *)(param_1 + 0x740) = param_4;
    return;
}

// FUN_02013f54 @ 0x02013f54 (1160 bytes) — Configure BG display control registers for main/sub screen
void FUN_02013f54(int *param_1, int param_2)
{
    u16 uVar1;
    u32 uVar2;
    u16 *puVar3;
    u16 *puVar4;
    u16 *puVar5;
    int iVar6;

    puVar4 = DAT_020143ec;
    puVar3 = DAT_020143dc;
    if (*(char *)((int)param_1 + 0x782) == '\0') {
        _DAT_04000000 = _DAT_04000000 & 0xf8ffffff;
        *DAT_020143dc =
             *DAT_020143dc & 0x43 | 0x4000 | (*(u8 *)((int)param_1 + 0x783) & 1) << 7 | 0xa00 |
             (u16)*(u8 *)((int)param_1 + 0x77f) << 2;
        puVar4 = DAT_020143e0;
        *DAT_020143e0 =
             *DAT_020143e0 & 0x43 | 0x4000 |
             (u16)(((int)(*(u8 *)((int)param_1 + 0x783) & 2) >> 1) << 7) | 0xc00 |
             (u16)*(u8 *)(param_1 + 0x1e0) << 2;
        puVar5 = DAT_020143e4;
        *DAT_020143e4 =
             *DAT_020143e4 & 0x43 | 0x4000 |
             (u16)(((int)(*(u8 *)((int)param_1 + 0x783) & 4) >> 2) << 7) | 0xe00 |
             (u16)*(u8 *)((int)param_1 + 0x781) << 2;
        if (param_2 == 1) {
            uVar2 = *(u32 *)(param_1[0x18d] + 8) & 3;
            if (uVar2 == 0) {
                *puVar3 = *puVar3 & 0xfffc | 1;
                *puVar4 = *puVar4 & 0xfffc | 2;
                *puVar5 = *puVar5 & 0xfffc | 3;
            }
            else if (uVar2 == 1) {
                *puVar3 = *puVar3 & 0xfffc | 1;
                *puVar5 = *puVar5 & 0xfffc | 2;
                *puVar4 = *puVar4 & 0xfffc | 3;
            }
            else if (uVar2 == 2) {
                *puVar5 = *puVar5 & 0xfffc | 1;
                *puVar3 = *puVar3 & 0xfffc | 2;
                *puVar4 = *puVar4 & 0xfffc | 3;
            }
        }
        else {
            *puVar3 = *puVar3 & 0xfffc | 1;
            *puVar4 = *puVar4 & 0xfffc | 2;
            *puVar5 = *puVar5 & 0xfffc | 3;
        }
        if ((param_1[0x192] != 0) && (iVar6 = param_1[0x1ca], iVar6 != 0)) {
            uVar1 = *(u16 *)(iVar6 + 4) & 0x1f;
            FUN_020369c4(DAT_020143e8, (((u32)*(u16 *)(iVar6 + 2) << 0x19) >> 0x1b) << 1,
                         (((u32)*(u16 *)(iVar6 + 2) << 0x14) >> 0x1b) << 1, uVar1, uVar1);
            param_1[0x1cb] = (u32)*(u16 *)(param_1[0x1ca] + 6) << 8;
            *(u16 *)(param_1 + 0x1cc) = 0;
        }
        puVar4 = DAT_020143e4;
        puVar3 = DAT_020143e0;
        *DAT_020143dc = *DAT_020143dc & 0xffbf;
        *puVar3 = *puVar3 & 0xffbf;
        *puVar4 = *puVar4 & 0xffbf;
    }
    else {
        *DAT_020143ec =
             *DAT_020143ec & 0x43 | 0x4000 | (*(u8 *)((int)param_1 + 0x783) & 1) << 7 | 0xa00 |
             (u16)*(u8 *)((int)param_1 + 0x77f) << 2;
        puVar3 = DAT_020143f0;
        *DAT_020143f0 =
             *DAT_020143f0 & 0x43 | 0x4000 |
             (u16)(((int)(*(u8 *)((int)param_1 + 0x783) & 2) >> 1) << 7) | 0xc00 |
             (u16)*(u8 *)(param_1 + 0x1e0) << 2;
        puVar5 = DAT_020143f4;
        *DAT_020143f4 =
             *DAT_020143f4 & 0x43 | 0x4000 |
             (u16)(((int)(*(u8 *)((int)param_1 + 0x783) & 4) >> 2) << 7) | 0xe00 |
             (u16)*(u8 *)((int)param_1 + 0x781) << 2;
        if (param_2 == 1) {
            uVar2 = *(u32 *)(param_1[0x18d] + 8) & 3;
            if (uVar2 == 0) {
                *puVar4 = *puVar4 & 0xfffc | 1;
                *puVar3 = *puVar3 & 0xfffc | 2;
                *puVar5 = *puVar5 & 0xfffc | 3;
            }
            else if (uVar2 == 1) {
                *puVar4 = *puVar4 & 0xfffc | 1;
                *puVar5 = *puVar5 & 0xfffc | 2;
                *puVar3 = *puVar3 & 0xfffc | 3;
            }
            else if (uVar2 == 2) {
                *puVar5 = *puVar5 & 0xfffc | 1;
                *puVar4 = *puVar4 & 0xfffc | 2;
                *puVar3 = *puVar3 & 0xfffc | 3;
            }
        }
        else {
            *puVar4 = *puVar4 & 0xfffc | 1;
            *puVar3 = *puVar3 & 0xfffc | 2;
            *puVar5 = *puVar5 & 0xfffc | 3;
        }
        if ((param_1[0x192] != 0) && (iVar6 = param_1[0x1ca], iVar6 != 0)) {
            uVar1 = *(u16 *)(iVar6 + 4) & 0x1f;
            FUN_020369c4(DAT_020143f8, (((u32)*(u16 *)(iVar6 + 2) << 0x19) >> 0x1b) << 1,
                         (((u32)*(u16 *)(iVar6 + 2) << 0x14) >> 0x1b) << 1, uVar1, uVar1);
            param_1[0x1cb] = (u32)*(u16 *)(param_1[0x1ca] + 6) << 8;
            *(u16 *)(param_1 + 0x1cc) = 0;
        }
        puVar4 = DAT_020143f4;
        puVar3 = DAT_020143f0;
        *DAT_020143ec = *DAT_020143ec & 0xffbf;
        *puVar3 = *puVar3 & 0xffbf;
        *puVar4 = *puVar4 & 0xffbf;
    }
    (**(code **)(*param_1 + 0x88))();
    return;
}

// FUN_02015c38 @ 0x02015c38 (356 bytes) — Construct animation manager instance with display parameters
u32 *FUN_02015c38(u32 *param_1,
                   u8 in_stack_00000000, u16 in_stack_00000004,
                   u8 in_stack_00000008, u8 in_stack_0000000c,
                   u32 in_stack_00000010, u16 in_stack_00000014,
                   u16 in_stack_00000018, u32 in_stack_0000001c,
                   u32 in_stack_00000020, u8 in_stack_00000024)
{
    int iVar1;
    int iVar2;
    u32 uVar3;

    FUN_0202a74c();
    iVar2 = DAT_02015da0;
    *param_1 = DAT_02015d9c;
    if (*(int *)(iVar2 + 0x834) == 1) {
        *(u8 *)((int)param_1 + 0x782) = 0;
    }
    else {
        *(u8 *)((int)param_1 + 0x782) = in_stack_00000000;
    }
    *(u16 *)(param_1 + 0x1d9) = in_stack_00000004;
    param_1[0xc] = in_stack_00000010;
    *(u8 *)(param_1 + 10) = 0;
    *(u8 *)(param_1 + 0x1e1) = in_stack_00000008;
    *(u8 *)((int)param_1 + 0x785) = in_stack_0000000c;
    *(u16 *)((int)param_1 + 0x766) = in_stack_00000014;
    *(u16 *)(param_1 + 0x1da) = in_stack_00000018;
    param_1[0x1cd] = 0;
    param_1[0x1ce] = 0;
    param_1[0x1cf] = 0;
    param_1[0x1d0] = 0;
    param_1[0x1d1] = 0;
    param_1[0x1d2] = 0;
    param_1[0x1d3] = 0;
    param_1[0x1d4] = 0;
    param_1[0x1d5] = 0;
    param_1[0x26] = in_stack_0000001c;
    param_1[0x25] = in_stack_00000020;
    *(u8 *)((int)param_1 + 0x787) = 0;
    *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xfc;
    *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xf3;
    *(u8 *)(param_1 + 0x1e3) = *(u8 *)(param_1 + 0x1e3) & 0xef;
    *(u8 *)((int)param_1 + 0x78b) = 0;
    *(u8 *)((int)param_1 + 0x78a) = 0;
    param_1[0x1d8] = 0x100;
    param_1[0x2b] = 0;
    param_1[0x1d] = 0;
    param_1[0x1e] = 0;
    param_1[0x1f] = 0;
    param_1[0x20] = 0;
    param_1[0x21] = 0;
    param_1[0x22] = 0;
    param_1[0x23] = 0;
    iVar2 = 0;
    do {
        iVar1 = iVar2 + 1;
        param_1[iVar2 + 0xd] = 0;
        iVar2 = iVar1;
    } while (iVar1 < 0x10);
    param_1[0x2a] = 0;
    *(u8 *)((int)param_1 + 0x78d) =
         *(u8 *)((int)param_1 + 0x78d) & 0xef | (in_stack_00000024 & 1) << 4;
    iVar2 = FUN_02029c1c(0x100, *(u8 *)(param_1 + 0x1e1), 0, 0);
    uVar3 = 0;
    if (iVar2 != 0) {
        uVar3 = FUN_02011bac(iVar2, 8, 0, param_1);
    }
    param_1[0xb] = uVar3;
    return param_1;
}
