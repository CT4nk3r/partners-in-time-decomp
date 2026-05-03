/**
 * Hardware GX — 2D/3D graphics engine, VRAM bank control (0x02036xxx-0x02039xxx)
 * Decompiled from 0x02036538-0x02039894 region (58 functions)
 */
#include "types.h"

// ============================================================
// External variable declarations
// ============================================================

extern vu32 _DAT_04000000;

extern u8  *DAT_02036594;
extern u8  *DAT_02036634;
extern u8  *DAT_02036654;
extern u8  *DAT_020366a8;
extern u16 *DAT_0203675c;
extern u8  *DAT_02036758;
extern u16 *DAT_020367e8;
extern u8  *DAT_02036864;
extern u16 *DAT_02036868;
extern u16 *DAT_020368f4;
extern u8  *DAT_02036914;  /* actually u16* used as u8 read shifted */
extern u8  *DAT_02036968;
extern u8  *DAT_02036cd8;
extern void (*DAT_02036cdc)(...);
extern u8  *DAT_02036d10;
extern void (*DAT_02036d14)(...);

extern void (*DAT_02036bf0)(...);
extern void (*DAT_02036c1c)(...);

extern u32 *DAT_020374f4;
extern u32 *DAT_020374f8;

extern u32 *DAT_020377e8;
extern u32 *DAT_020377ec;
extern u32 *DAT_020377f0; /* actually undefined4[2] */
extern s32 *DAT_020377f4; /* actually int[2] */

extern u32 *DAT_020379d8;
extern s32 *DAT_020379dc;
extern u32 *DAT_020379e0;
extern s32 *DAT_020379e4;

extern s32 *DAT_02037a2c;
extern u32 *DAT_02037a30;
extern s32 *DAT_02037aa0;
extern u32  DAT_02037aa4;
extern u32 *DAT_02037ac8;

extern s32 *DAT_02037bfc;
extern u32 *DAT_02037c00;
extern u32 *DAT_02037c04;
extern s32 *DAT_02037c78;
extern u32 *DAT_02037c74;
extern u32 *DAT_02037cdc;
extern u32 *DAT_02037ce4;
extern u32  DAT_02037ce8;
extern u32  DAT_02037ce0;

extern s32 *DAT_020385b0;
extern s32 *DAT_02038610;

extern s32 *DAT_02038724;
extern u32 *DAT_02038728;
extern u32 *DAT_0203872c;

extern s32 *DAT_020387a0;
extern u32 *DAT_0203879c;
extern u32 *DAT_020387e0;
extern u32  DAT_020387e4;
extern s32 *DAT_020387e8;

extern s32 *DAT_02038848;
extern u32 *DAT_0203884c;
extern u32 *DAT_02038850;
extern u32 *DAT_02038854;
extern u32 *DAT_02038858;

extern s32 *DAT_020389b0;
extern u32 *DAT_020389b4;
extern u32 *DAT_020389b8;
extern s32 *DAT_020389bc;

extern u32 *DAT_02038a30;
extern u32  DAT_02038a24;
extern u32  DAT_02038a28;
extern u32  DAT_02038a2c;
extern s32 *DAT_02038a34;
extern s32 *DAT_02038a38;
extern s32 *DAT_02038a3c;

extern u32  DAT_02038e58;
extern u32  DAT_02038e54;

extern u32 *DAT_02038e6c;

extern void (*DAT_02038f10)(...);
extern void (*DAT_02038fc8)(...);
extern u32  DAT_02038fcc;
extern u32  DAT_02038fd0;

extern void (*DAT_020390a0)(...);
extern void (*DAT_02039140)(...);

extern u32 *DAT_02039274;
extern u32 *DAT_02039278;
extern u32 *DAT_0203927c;
extern u32 *DAT_02039280;
extern u32  DAT_02039284;
extern u16 *DAT_02039288;

extern u32 *DAT_0203938c;
extern u32  DAT_02039390;

extern u16 *DAT_02039468;
extern u32 *DAT_02039464;
extern u32 *DAT_0203946c;
extern u32 *DAT_02039470;

extern u32  DAT_02039544;
extern u32 *DAT_020395ac;
extern u32 *DAT_02039604;
extern u32  DAT_02039608;

extern u32  DAT_02039704;
extern u32  DAT_02039708;
extern u32  DAT_0203970c;
extern u32  DAT_02039710;

extern u32 *DAT_02039858;
extern u16 *DAT_0203985c;
extern u32  DAT_02039860;
extern u32  DAT_02039864;
extern u32 *DAT_0203986c;
extern u32  DAT_02039868;
extern u32  DAT_02039870;
extern u32  DAT_02039874;
extern u32  DAT_02039878;
extern u32  DAT_0203987c;
extern u32  DAT_02039880;
extern u32 *DAT_02039884;
extern u32  DAT_02039888;
extern u32  DAT_0203988c;
extern u32  DAT_02039890;

extern u32  DAT_020398d8;

// ============================================================
// Forward declarations (functions defined in this file)
// ============================================================

void FUN_02036ad4(u32 *param_1, s32 param_2);
void FUN_02036bc8(u32 *param_1, u32 param_2);
void FUN_02036bf4(u32 *param_1, u32 param_2);
void FUN_02036c84(u32 *param_1, u8 param_2, u32 param_3);
void FUN_02039094(void);
void FUN_02039134(void);
void FUN_02039394(void);
void FUN_020394d4(u16 *param_1);
void FUN_02039894(s32 param_1);
void FUN_0203960c(s32 param_1, u32 param_2, u32 param_3, s32 param_4, s32 param_5, u32 param_6);

// ============================================================
// External function declarations
// ============================================================

extern void FUN_02034a50(...);
extern s32  FUN_02034b90(...);
extern s32  FUN_02034c2c(...);
extern s32  FUN_020348b4(...);
extern s32  FUN_02034a34(...);
extern s32  FUN_020348f0(...);
extern u64  FUN_0203492c(...);
extern s64  FUN_02046300(...);

extern u32  FUN_020357f0(...);
extern void FUN_020358e8(...);
extern s32  FUN_02035840(...);
extern s32  FUN_02035854(...);
extern s32  FUN_02035868(...);
extern void FUN_02035b48(...);
extern void FUN_02035c3c(...);
extern void FUN_02035e40(...);

extern void FUN_0203ae24(...);
extern u32  FUN_0203ae38(...);
extern void FUN_0203ae4c(...);
extern u32  FUN_0203ae64(...);
extern void FUN_0203ae78(...);
extern s32  FUN_0203ae90(...);
extern void FUN_0203ae9c(...);
extern void FUN_0203aeb4(...);

extern void FUN_0203b34c(...);
extern void FUN_0203b3bc(...);
extern void FUN_0203b5b4(...);
extern void FUN_0203b7dc(...);
extern void FUN_0203b7f0(...);

extern s32  FUN_0203bad8(...);

extern void FUN_020392a4(...);
extern s32  FUN_02039474(...);
extern s32  FUN_02039944(...);
extern void FUN_02039974(...);
extern void FUN_020398dc(...);
extern s32  FUN_020399d8(...);
extern void FUN_02039a24(...);
extern void FUN_02039e48(...);

extern void FUN_02036c60(...);
extern void FUN_02036ca0(...);

// ============================================================
// Functions
// ============================================================

// --- FUN_02036538 @ 0x02036538 ---
u32 FUN_02036538(void) {
    u32 mode = _DAT_04000000 & 7;
    if (mode > 2 && (mode > 5 || (*DAT_02036594 & 0x80) != 0)) {
        return 0;
    }
    return ((_DAT_04000000 & 0x7000000) >> 24) * 0x10000 + 0x6000000 +
           ((*DAT_02036594 & 0x3C) >> 2) * 0x4000;
}

// --- FUN_020365e0 @ 0x020365e0 ---
u32 FUN_020365e0(void) {
    u32 mode = _DAT_04000000 & 7;
    if (mode > 4 && (*DAT_02036634 & 0x80) != 0) {
        return 0;
    }
    return ((_DAT_04000000 & 0x7000000) >> 24) * 0x10000 + 0x6000000 +
           ((*DAT_02036634 & 0x3C) >> 2) * 0x4000;
}

// --- FUN_02036638 @ 0x02036638 ---
u32 FUN_02036638(void) {
    return ((*DAT_02036654 & 0x3C) >> 2) * 0x4000 + 0x6200000;
}

// --- FUN_0203668c @ 0x0203668c ---
u32 FUN_0203668c(void) {
    return ((*DAT_020366a8 & 0x3C) >> 2) * 0x4000 + 0x6200000;
}

// --- FUN_020366e0 @ 0x020366e0 ---
u32 FUN_020366e0(void) {
    u32 charBase;
    u32 mode;

    charBase = (*DAT_0203675c & 0x1F00) >> 8;
    mode = *DAT_02036758 & 7;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return charBase * 0x800 + 0x6200000;
    case 3:
    case 4:
    case 5:
        if ((*DAT_0203675c & 0x80) == 0) {
            return charBase * 0x800 + 0x6200000;
        } else {
            return charBase * 0x4000 + 0x6200000;
        }
    case 6:
    default:
        return 0;
    }
}

// --- FUN_02036760 @ 0x02036760 ---
u32 FUN_02036760(void) {
    u32 charBase;
    u32 vramOffset;
    u32 mode;

    vramOffset = ((_DAT_04000000 & 0x38000000) >> 27) * 0x10000;
    charBase = (*DAT_020367e8 & 0x1F00) >> 8;
    mode = _DAT_04000000 & 7;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return vramOffset + 0x6000000 + charBase * 0x800;
    case 3:
    case 4:
    case 5:
        if ((*DAT_020367e8 & 0x80) == 0) {
            return vramOffset + 0x6000000 + charBase * 0x800;
        } else {
            return charBase * 0x4000 + 0x6000000;
        }
    case 6:
    default:
        return 0;
    }
}

// --- FUN_020367ec @ 0x020367ec ---
u32 FUN_020367ec(void) {
    u32 charBase;
    u32 mode;

    charBase = (*DAT_02036868 & 0x1F00) >> 8;
    mode = *DAT_02036864 & 7;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return charBase * 0x800 + 0x6200000;
    case 5:
        if ((*DAT_02036868 & 0x80) == 0) {
            return charBase * 0x800 + 0x6200000;
        } else {
            return charBase * 0x4000 + 0x6200000;
        }
    case 6:
    default:
        return 0;
    }
}

// --- FUN_0203686c @ 0x0203686c ---
u32 FUN_0203686c(void) {
    u32 charBase;
    u32 vramOffset;
    u32 mode;

    vramOffset = ((_DAT_04000000 & 0x38000000) >> 27) * 0x10000;
    charBase = (*DAT_020368f4 & 0x1F00) >> 8;
    mode = _DAT_04000000 & 7;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return vramOffset + 0x6000000 + charBase * 0x800;
    case 5:
        if ((*DAT_020368f4 & 0x80) == 0) {
            return vramOffset + 0x6000000 + charBase * 0x800;
        } else {
            return charBase * 0x4000 + 0x6000000;
        }
    case 6:
        return 0x6000000;
    default:
        return 0;
    }
}

// --- FUN_020368f8 @ 0x020368f8 ---
u32 FUN_020368f8(void) {
    return ((*DAT_02036914 & 0x1F00) >> 8) * 0x800 + 0x6200000;
}

// --- FUN_0203694c @ 0x0203694c ---
u32 FUN_0203694c(void) {
    return ((*DAT_02036968 & 0x1F00) >> 8) * 0x800 + 0x6200000;
}

// --- FUN_020369e0 @ 0x020369e0 ---
void FUN_020369e0(u32 *param_1, s32 *param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6) {
    s32 iVar1;
    s32 iVar2;

    *param_1 = (u32)(((param_2[0] << 12) >> 16) & 0xFFFF) | (u32)(((param_2[1] << 12) >> 16) << 16);
    param_1[1] = (u32)(((param_2[2] << 12) >> 16) & 0xFFFF) | (u32)(((param_2[3] << 12) >> 16) << 16);
    iVar1 = param_2[3];
    iVar2 = param_2[2];
    param_1[2] = (u32)(param_2[0] * (param_5 - param_3) + param_2[1] * (param_6 - param_4) + param_3 * 0x1000) >> 4;
    param_1[3] = (u32)(iVar2 * (param_5 - param_3) + iVar1 * (param_6 - param_4) + param_4 * 0x1000) >> 4;
}

// --- FUN_02036a94 @ 0x02036a94 ---
void FUN_02036a94(u32 *param_1, u32 param_2) {
    FUN_02036bc8(param_1, param_2);
    FUN_02036ad4(param_1, 9);
}

// --- FUN_02036ab4 @ 0x02036ab4 ---
void FUN_02036ab4(u32 *param_1, u32 param_2) {
    FUN_02036bf4(param_1, param_2);
    FUN_02036ad4(param_1, 0x10);
}

// --- FUN_02036ad4 @ 0x02036ad4 ---
void FUN_02036ad4(u32 *param_1, s32 param_2) {
    u32 *puVar1;
    u32 uVar2;

    param_1[1] = param_1[1] + param_2 * 4;
    *param_1 = *param_1 + 1;
    if ((*param_1 & 3) != 0) {
        return;
    }
    if (param_1[4] != 0) {
        puVar1 = (u32 *)param_1[1];
        param_1[1] = param_1[1] + 4;
        *puVar1 = 0;
        param_1[4] = 0;
    }
    uVar2 = param_1[1];
    param_1[1] = uVar2 + 4;
    *param_1 = uVar2;
}

// --- FUN_02036b68 @ 0x02036b68 ---
void FUN_02036b68(u32 *param_1, u32 param_2) {
    FUN_02036c60(param_1, param_2);
    FUN_02036ad4(param_1, 2);
}

// --- FUN_02036b88 @ 0x02036b88 ---
void FUN_02036b88(u32 *param_1, u8 param_2, u32 param_3) {
    FUN_02036c84(param_1, param_2, param_3);
    FUN_02036ad4(param_1, 1);
}

// --- FUN_02036ba8 @ 0x02036ba8 ---
void FUN_02036ba8(u32 *param_1, u32 param_2) {
    FUN_02036ca0(param_1, param_2);
    FUN_02036ad4(param_1, 0);
}

// --- FUN_02036bc8 @ 0x02036bc8 ---
void FUN_02036bc8(u32 *param_1, u32 param_2) {
    param_1[4] = 0;
    *(u8 *)*param_1 = 0x1A;
    ((void (*)(...))*DAT_02036bf0)(param_2, param_1[1]);
}

// --- FUN_02036bf4 @ 0x02036bf4 ---
void FUN_02036bf4(u32 *param_1, u32 param_2) {
    param_1[4] = 0;
    *(u8 *)*param_1 = 0x16;
    ((void (*)(...))*DAT_02036c1c)(param_2, param_1[1]);
}

// --- FUN_02036c84 @ 0x02036c84 ---
void FUN_02036c84(u32 *param_1, u8 param_2, u32 param_3) {
    param_1[4] = 0;
    *(u8 *)*param_1 = param_2;
    *(u32 *)param_1[1] = param_3;
}

// --- FUN_02036cc4 @ 0x02036cc4 ---
void FUN_02036cc4(void) {
#ifdef HOST_PORT
    /* GX list-end flush: writes 0x1A to *DAT_02036cd8 then tail-calls
     * *DAT_02036cdc.  In the original ROM both are pointers to live
     * runtime structures set up by the GX init path; in the host port
     * they're zero-init globals and dereferencing them faults.  Nothing
     * in the host needs the flush — every GXFIFO/MMIO write is already
     * observed by host_gxfifo_observer / nds_hw_stubs at the IO site.
     * Skip the body. */
    return;
#else
    *DAT_02036cd8 = 0x1A;
    DAT_02036cdc();
#endif
}

// --- FUN_02036cfc @ 0x02036cfc ---
void FUN_02036cfc(void) {
#ifdef HOST_PORT
    /* GX list-begin flush — see FUN_02036cc4 for rationale. */
    return;
#else
    *DAT_02036d10 = 0x16;
    DAT_02036d14();
#endif
}

// --- FUN_02037364 @ 0x02037364 ---
void FUN_02037364(s32 *param_1, u32 param_2, s32 *param_3, s32 param_4, s32 *param_5) {
    s32 local_40, local_3c, local_38;
    s32 local_34, local_30, local_2c;
    s32 local_28, local_24, local_20;
    s32 iVar1, iVar2, iVar3;

    local_40 = param_1[0] - param_3[0];
    local_3c = param_1[1] - param_3[1];
    local_38 = param_1[2] - param_3[2];
    FUN_02034a50(&local_40, &local_40);
    FUN_02034b90(param_2, &local_40, &local_34);
    FUN_02034a50(&local_34, &local_34);
    FUN_02034b90(&local_40, &local_34, &local_28);
    if (param_4 != 0) {
        *DAT_020374f4 = 2;
        *DAT_020374f8 = (u32)local_34;
        *(u32 *)DAT_020374f8 = (u32)local_28;
        *(u32 *)DAT_020374f8 = (u32)local_40;
        *(u32 *)DAT_020374f8 = (u32)local_30;
        *(u32 *)DAT_020374f8 = (u32)local_24;
        *(u32 *)DAT_020374f8 = (u32)local_3c;
        *(u32 *)DAT_020374f8 = (u32)local_2c;
        *(u32 *)DAT_020374f8 = (u32)local_20;
        *(u32 *)DAT_020374f8 = (u32)local_38;
    }
    iVar1 = FUN_02034c2c(param_1, &local_34);
    iVar2 = FUN_02034c2c(param_1, &local_28);
    iVar3 = FUN_02034c2c(param_1, &local_40);
    if (param_4 != 0) {
        *(s32 *)DAT_020374f8 = -iVar1;
    }
    if (param_4 != 0) {
        *(s32 *)DAT_020374f8 = -iVar2;
        *(s32 *)DAT_020374f8 = -iVar3;
    }
    if (param_5 != (s32 *)0) {
        param_5[0] = local_34;
        param_5[1] = local_28;
        param_5[2] = local_40;
        param_5[3] = local_30;
        param_5[4] = local_24;
        param_5[5] = local_3c;
        param_5[6] = local_2c;
        param_5[7] = local_20;
        param_5[8] = local_38;
        param_5[9] = -iVar1;
        param_5[10] = -iVar2;
        param_5[11] = -iVar3;
    }
}

// --- FUN_020374fc @ 0x020374fc ---
void FUN_020374fc(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6,
                  u32 param_7, s32 param_8, u32 *param_9) {
    s32 iVar1;
    s64 lVar2;
    u32 *puVar3;
    u32 *puVar4;
    s32 *piVar5;
    u32 uVar6, uVar7, uVar8, uVar9;
    u64 uVar10, uVar11, uVar12;
    s32 local_44, local_2c;

    FUN_020348b4(param_4 - param_3);
    puVar3 = DAT_020377ec;
    if (param_8 != 0) {
        *DAT_020377e8 = 0;
    }
    if (param_9 != (u32 *)0) {
        param_9[1] = 0;
        param_9[2] = 0;
        param_9[3] = 0;
        param_9[4] = 0;
        param_9[6] = 0;
        param_9[7] = 0;
        param_9[8] = 0;
        param_9[9] = 0;
        param_9[11] = 0;
        param_9[15] = param_7;
    }
    uVar10 = FUN_0203492c();
    puVar4 = DAT_020377f0;
    *DAT_020377f0 = 0;
    puVar4[1] = 0x1000;
    piVar5 = DAT_020377f4;
    *piVar5 = param_1 - param_2;
    piVar5[1] = 0;
    iVar1 = (s32)param_7 >> 31;
    if (param_7 != 0x1000) {
        lVar2 = (s64)(u64)param_7 * (s64)(u32)uVar10;
        uVar10 = (u64)FUN_02046300((s32)lVar2,
                       (s32)(uVar10 >> 32) * (s32)param_7 +
                       (s32)uVar10 * iVar1 + (s32)((u64)lVar2 >> 32));
    }
    local_44 = (s32)(uVar10 >> 32);
    uVar6 = (u32)((uVar10 << 13) >> 32) + (u32)(0x7FFFFFFF < (u32)(uVar10 << 13));
    if (param_8 != 0) {
        *puVar3 = uVar6;
    }
    if (param_9 != (u32 *)0) {
        *param_9 = uVar6;
    }
    uVar11 = FUN_0203492c();
    puVar4 = DAT_020377f0;
    *DAT_020377f0 = 0;
    puVar4[1] = 0x1000;
    piVar5 = DAT_020377f4;
    *DAT_020377f4 = param_5 - param_6;
    piVar5[1] = 0;
    if (param_7 != 0x1000) {
        lVar2 = (s64)(u64)param_7 * (s64)(u32)uVar11;
        uVar11 = (u64)FUN_02046300((s32)lVar2,
                       (s32)(uVar11 >> 32) * (s32)param_7 +
                       (s32)uVar11 * iVar1 + (s32)((u64)lVar2 >> 32));
    }
    local_2c = (s32)(uVar11 >> 32);
    uVar6 = (u32)((uVar11 << 13) >> 32) + (u32)(0x7FFFFFFF < (u32)(uVar11 << 13));
    if (param_9 != (u32 *)0) {
        param_9[5] = uVar6;
    }
    uVar12 = FUN_0203492c();
    if (param_7 != 0x1000) {
        lVar2 = (s64)(u64)param_7 * (s64)(u32)uVar12;
        uVar12 = (u64)FUN_02046300((s32)lVar2,
                       (s32)(uVar12 >> 32) * (s32)param_7 +
                       (s32)uVar12 * iVar1 + (s32)((u64)lVar2 >> 32),
                       0x1000, 0);
    }
    uVar6 = (u32)((uVar12 << 13) >> 32) + (u32)(0x7FFFFFFF < (u32)(uVar12 << 13));
    if (param_9 != (u32 *)0) {
        param_9[10] = uVar6;
    }
    uVar6 = -(param_4 + param_3);
    uVar8 = -(param_1 + param_2);
    uVar7 = param_6 + param_5;
    lVar2 = (s64)(u64)uVar6 * (s64)(u32)uVar10;
    uVar9 = local_44 * uVar6 + (s32)uVar10 * ((s32)uVar6 >> 31) + (u32)((u64)lVar2 >> 32) +
            (u32)(0x7FFFFFFF < (u32)lVar2);
    lVar2 = (s64)(u64)uVar8 * (s64)(u32)uVar11;
    uVar8 = local_2c * uVar8 + (s32)uVar11 * ((s32)uVar8 >> 31) + (u32)((u64)lVar2 >> 32) +
            (u32)(0x7FFFFFFF < (u32)lVar2);
    lVar2 = (s64)(u64)uVar7 * (s64)(u32)uVar12;
    uVar6 = (s32)(uVar12 >> 32) * uVar7 +
            (s32)uVar12 * ((s32)uVar7 >> 31) + (u32)((u64)lVar2 >> 32) +
            (u32)(0x7FFFFFFF < (u32)lVar2);
    if (param_9 != (u32 *)0) {
        param_9[12] = uVar9;
        param_9[13] = uVar8;
        param_9[14] = uVar6;
    }
}

// --- FUN_020377f8 @ 0x020377f8 ---
void FUN_020377f8(u32 param_1, u32 param_2, s32 param_3, s32 param_4, s32 param_5,
                  u32 param_6, s32 param_7, s32 *param_8) {
    s64 lVar1;
    s32 *piVar2;
    s32 iVar3;
    u32 *puVar4;
    s32 iVar5;
    u32 uVar6;
    u64 uVar7;

    iVar3 = FUN_02034a34(param_2, param_1);
    puVar4 = DAT_020379d8;
    if (param_6 != 0x1000) {
        iVar3 = (s32)(iVar3 * (s32)param_6 + ((u32)((s32)(iVar3 * (s32)param_6) >> 11) >> 20)) >> 12;
    }
    *DAT_020379d8 = 0;
    piVar2 = DAT_020379dc;
    puVar4[1] = (u32)iVar3;
    *piVar2 = param_3;
    puVar4 = (u32 *)0;
    if (param_7 != 0) {
        puVar4 = DAT_020379e0;
    }
    piVar2[1] = 0;
    piVar2 = DAT_020379e4;
    if (param_7 != 0) {
        *puVar4 = 0;
    }
    if (param_8 != (s32 *)0) {
        param_8[1] = 0;
        param_8[2] = 0;
        param_8[3] = 0;
        param_8[4] = 0;
        param_8[6] = 0;
        param_8[7] = 0;
        param_8[8] = 0;
        param_8[9] = 0;
        param_8[11] = -(s32)param_6;
        param_8[12] = 0;
        param_8[13] = 0;
        param_8[15] = 0;
    }
    iVar5 = FUN_020348f0();
    puVar4 = DAT_020379d8;
    *DAT_020379d8 = 0;
    piVar2 = DAT_020379dc;
    puVar4[1] = 0x1000;
    *piVar2 = param_4 - param_5;
    piVar2[1] = 0;
    if (param_8 != (s32 *)0) {
        *param_8 = iVar5;
        param_8[5] = iVar3;
    }
    uVar7 = FUN_0203492c();
    if (param_6 != 0x1000) {
        lVar1 = (s64)(u64)param_6 * (s64)(u32)uVar7;
        uVar7 = (u64)FUN_02046300((s32)lVar1,
                     (s32)(uVar7 >> 32) * (s32)param_6 +
                     (s32)uVar7 * ((s32)param_6 >> 31) + (s32)((u64)lVar1 >> 32),
                     0x1000, 0);
    }
    iVar3 = (s32)(uVar7 >> 32);
    uVar6 = (u32)(param_5 + param_4);
    lVar1 = (s64)(u64)uVar6 * (s64)(u32)uVar7;
    iVar5 = iVar3 * (s32)uVar6 + (s32)uVar7 * ((s32)uVar6 >> 31) + (s32)((u64)lVar1 >> 32) +
            (u32)(0x7FFFFFFF < (u32)lVar1);
    lVar1 = (s64)(param_4 << 1) * (s64)param_5 + 0x800;
    uVar6 = (u32)lVar1 >> 12 | (s32)((u64)lVar1 >> 32) * 0x100000;
    lVar1 = (s64)(u64)uVar6 * (s64)(u32)uVar7;
    iVar3 = iVar3 * (s32)uVar6 + (s32)uVar7 * ((s32)uVar6 >> 31) + (s32)((u64)lVar1 >> 32) +
            (u32)(0x7FFFFFFF < (u32)lVar1);
    if (param_8 != (s32 *)0) {
        param_8[10] = iVar5;
        param_8[14] = iVar3;
    }
}

// --- FUN_020379e8 @ 0x020379e8 ---
void FUN_020379e8(void) {
    if (*DAT_02037a2c != -1) {
        FUN_0203b34c();
    }
    FUN_020358e8(*DAT_02037a30);
    *DAT_02037a30 = 0;
}

// --- FUN_02037a34 @ 0x02037a34 ---
void FUN_02037a34(u32 param_1, s32 param_2, u32 param_3) {
    if (*DAT_02037aa0 != -1) {
        FUN_0203b3bc(*DAT_02037aa0, param_1, param_2 + (s32)DAT_02037aa4, param_3, 0, 0);
        return;
    }
    FUN_0203b7f0(param_1, param_2 + (s32)DAT_02037aa4, param_3, param_3);
}

// --- FUN_02037aa8 @ 0x02037aa8 ---
void FUN_02037aa8(void) {
    u32 uVar1;
    uVar1 = FUN_020357f0();
    *DAT_02037ac8 = uVar1;
}

// --- FUN_02037bb0 @ 0x02037bb0 ---
void FUN_02037bb0(void) {
    if (*DAT_02037bfc != -1) {
        FUN_0203b34c();
    }
    FUN_02035e40(*DAT_02037c00);
    *DAT_02037c00 = 0;
    *DAT_02037c04 = 0;
}

// --- FUN_02037c08 @ 0x02037c08 ---
void FUN_02037c08(u32 param_1, s32 param_2, u32 param_3) {
    if (*DAT_02037c78 != -1) {
        FUN_0203b3bc(*DAT_02037c78, param_1, *DAT_02037c74 + param_2, param_3, 0, 0);
        return;
    }
    FUN_0203b7f0(param_1, *DAT_02037c74 + param_2, param_3, param_3);
}

// --- FUN_02037c7c @ 0x02037c7c ---
void FUN_02037c7c(void) {
    s32 iVar1;

    iVar1 = FUN_02035868();
    *DAT_02037cdc = (u32)iVar1;
    if (iVar1 == 0) {
        return;
    }
    if (iVar1 == 0x20) {
        *DAT_02037ce4 = DAT_02037ce8;
        return;
    }
    if (iVar1 == 0x40) {
        *DAT_02037ce4 = DAT_02037ce0;
    }
}

// --- FUN_02038554 @ 0x02038554 ---
void FUN_02038554(u32 param_1, s32 param_2, u32 param_3) {
    if (*DAT_020385b0 != -1 && param_3 > 0x30) {
        FUN_0203b5b4(*DAT_020385b0, param_1, param_2 + 0x6600000);
        return;
    }
    FUN_0203b7f0(param_1, param_2 + 0x6600000, param_3);
}

// --- FUN_020385b4 @ 0x020385b4 ---
void FUN_020385b4(u32 param_1, s32 param_2, u32 param_3) {
    if (*DAT_02038610 != -1 && param_3 > 0x30) {
        FUN_0203b5b4(*DAT_02038610, param_1, param_2 + 0x6400000);
        return;
    }
    FUN_0203b7f0(param_1, param_2 + 0x6400000, param_3);
}

// --- FUN_020386d8 @ 0x020386d8 ---
void FUN_020386d8(void) {
    if (*DAT_02038724 != -1) {
        FUN_0203b34c();
    }
    FUN_02035b48(*DAT_02038728);
    *DAT_02038728 = 0;
    *DAT_0203872c = 0;
}

// --- FUN_02038730 @ 0x02038730 ---
void FUN_02038730(u32 param_1, s32 param_2, u32 param_3) {
    if (*DAT_020387a0 != -1) {
        FUN_0203b3bc(*DAT_020387a0, param_1, *DAT_0203879c + param_2, param_3, 0, 0);
        return;
    }
    FUN_0203b7f0(param_1, *DAT_0203879c + param_2, param_3, param_3);
}

// --- FUN_020387a4 @ 0x020387a4 ---
void FUN_020387a4(void) {
    u16 uVar1;
    s32 iVar3;

    iVar3 = FUN_02035840();
    uVar1 = *(u16 *)(DAT_020387e4 + (iVar3 >> 4) * 2);
    *DAT_020387e0 = (u32)iVar3;
    *DAT_020387e8 = (s32)((u32)uVar1 << 12);
}

// --- FUN_020387ec @ 0x020387ec ---
void FUN_020387ec(void) {
    if (*DAT_02038848 != -1) {
        FUN_0203b34c();
    }
    FUN_02035c3c(*DAT_0203884c);
    *DAT_02038850 = 0;
    *DAT_02038854 = 0;
    *DAT_02038858 = 0;
    *DAT_0203884c = 0;
}

// --- FUN_0203885c @ 0x0203885c ---
void FUN_0203885c(s32 param_1, u32 param_2, s32 param_3) {
    u32 uVar1;
    s32 iVar2;

    iVar2 = *DAT_020389b0;
    if (iVar2 == 0) {
        iVar2 = *DAT_020389b4 + param_2;
    } else {
        uVar1 = *DAT_020389b8;
        if (param_2 + param_3 < uVar1) {
            iVar2 = *DAT_020389b4 + param_2;
        } else {
            if (param_2 < uVar1) {
                uVar1 = uVar1 - param_2;
                if (*DAT_020389bc == -1 || uVar1 < 0x31) {
                    FUN_0203b7f0(param_1, *DAT_020389b4 + param_2, uVar1);
                } else {
                    FUN_0203b5b4(*DAT_020389bc, param_1, *DAT_020389b4 + param_2, uVar1);
                }
                if (*DAT_020389bc == -1) {
                    FUN_0203b7f0(param_1 + uVar1, iVar2, param_3 - uVar1);
                    return;
                }
                FUN_0203b3bc(*DAT_020389bc, param_1 + uVar1, iVar2, param_3 - uVar1, 0, 0);
                return;
            }
            iVar2 = (iVar2 + param_2) - uVar1;
        }
    }
    if (*DAT_020389bc == -1) {
        FUN_0203b7f0(param_1, iVar2, param_3);
        return;
    }
    FUN_0203b3bc(*DAT_020389bc, param_1, iVar2, param_3, 0, 0);
}

// --- FUN_020389c0 @ 0x020389c0 ---
void FUN_020389c0(void) {
    u16 uVar1, uVar2, uVar3;
    s32 iVar7, iVar8;

    iVar7 = FUN_02035854();
    iVar8 = iVar7 * 6;
    uVar1 = *(u16 *)(DAT_02038a24 + iVar8);
    uVar2 = *(u16 *)(DAT_02038a28 + iVar8);
    uVar3 = *(u16 *)(DAT_02038a2c + iVar8);
    *DAT_02038a30 = (u32)iVar7;
    *DAT_02038a34 = (s32)((u32)uVar1 << 12);
    *DAT_02038a38 = (s32)((u32)uVar2 << 12);
    *DAT_02038a3c = (s32)((u32)uVar3 << 12);
}

// --- FUN_02038dcc @ 0x02038dcc ---
void FUN_02038dcc(u32 param_1, u32 param_2) {
    s32 iVar1;
    s32 iVar2;
    u32 *puVar3;
    s32 iVar4;

    iVar2 = (s32)DAT_02038e58;
    iVar1 = (s32)DAT_02038e54;
    iVar4 = 0;
    do {
        if ((param_1 & 1) != 0) {
            puVar3 = (u32 *)0;
            if ((iVar4 < 8) || (iVar4 > 11)) {
                if ((iVar4 < 3) || (iVar4 > 6)) {
                    *(u32 *)(iVar1 + iVar4 * 4) = param_2;
                } else {
                    puVar3 = (u32 *)((iVar4 + 1) * 12 + iVar2);
                }
            } else {
                puVar3 = (u32 *)((iVar4 - 8) * 12 + iVar2);
            }
            if (puVar3 != (u32 *)0) {
                *puVar3 = param_2;
                puVar3[2] = 0;
                puVar3[1] = 1;
            }
        }
        iVar4 = iVar4 + 1;
        param_1 = param_1 >> 1;
    } while (iVar4 < 22);
}

// --- FUN_02038e5c @ 0x02038e5c ---
void FUN_02038e5c(void) {
    *DAT_02038e6c = 0;
}

// --- FUN_02038f04 @ 0x02038f04 ---
void FUN_02038f04(void) {
    DAT_02038f10(0x1000);
}

// --- FUN_02038fb4 @ 0x02038fb4 ---
void FUN_02038fb4(u32 param_1) {
    DAT_02038fc8(param_1, DAT_02038fcc, DAT_02038fd0, 1);
}

// --- FUN_02039014 @ 0x02039014 ---
s32 FUN_02039014(u32 param_1, s32 param_2, void (*param_3)(...), s32 param_4) {
    u32 uVar1;
    s32 iVar2;

    if (param_4 == 0) {
        uVar1 = FUN_0203ae38();
    } else {
        uVar1 = FUN_0203ae64();
    }
    iVar2 = FUN_0203bad8(param_1, param_2);
    if (iVar2 == 0) {
        if (param_3 != (void (*)(...))(0)) {
            param_3();
        }
        *(s16 *)(param_2 + 4) = (s16)param_1;
    }
    if (param_4 == 0) {
        FUN_0203ae4c(uVar1);
    } else {
        FUN_0203ae78(uVar1);
    }
    return iVar2;
}

// --- FUN_02039094 @ 0x02039094 ---
void FUN_02039094(void) {
    DAT_020390a0();
}

// --- FUN_02039134 @ 0x02039134 ---
void FUN_02039134(void) {
    DAT_02039140();
}

// --- FUN_0203919c @ 0x0203919c ---
void FUN_0203919c(void) {
    s16 sVar1;
    u32 *puVar2;
    u32 *puVar3;

    puVar2 = DAT_02039278;
    if (*DAT_02039274 != 0) {
        return;
    }
    *DAT_02039274 = 1;
    *puVar2 = 0;
    DAT_02039140(0x7E);
    puVar3 = DAT_02039278;
    sVar1 = *(s16 *)((u32)DAT_02039278 + 6);
    puVar2 = DAT_02039280;
    while (sVar1 != 0) {
        DAT_02039280 = puVar2;
        FUN_02038f04();
        puVar2 = DAT_02039280;
        sVar1 = *(s16 *)((u32)puVar3 + 6);
    }
    *DAT_0203927c = 0xFFFFFFFF;
    *puVar2 = 0xFFFF0000;
    FUN_0203b7dc(0, DAT_02039284, 0x28);
    *DAT_02039288 = *DAT_02039288 | 0x800;
    *(u16 *)DAT_02039288 = *(u16 *)DAT_02039288 | 0x80;
    DAT_020390a0(0x7E, puVar2, 0);
    DAT_02039140(0x7F, DAT_02039278, 0);
}

// --- FUN_02039290 @ 0x02039290 ---
void FUN_02039290(void) {
    FUN_0203ae24();
    do {
        FUN_0203ae9c();
    } while (TRUE);
}

// --- FUN_020392e0 @ 0x020392e0 ---
u32 FUN_020392e0(s32 param_1, s32 param_2) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;

    uVar1 = FUN_0203ae38();
    iVar2 = 0;
    for (iVar3 = *(s32 *)(DAT_0203938c + 0xC); iVar3 != 0 && iVar3 != param_1;
         iVar3 = *(s32 *)(iVar3 + 0x68)) {
        iVar2 = iVar3;
    }
    if (iVar3 != 0 && iVar3 != (s32)DAT_02039390) {
        if (*(s32 *)(iVar3 + 0x70) != param_2) {
            if (iVar2 == 0) {
                *(u32 *)(DAT_0203938c + 0xC) = *(u32 *)(param_1 + 0x68);
            } else {
                *(u32 *)(iVar2 + 0x68) = *(u32 *)(param_1 + 0x68);
            }
            *(s32 *)(param_1 + 0x70) = param_2;
            FUN_020398dc(param_1);
            FUN_02039394();
        }
        FUN_0203ae4c(uVar1);
        return 1;
    }
    FUN_0203ae4c(uVar1);
    return 0;
}

// --- FUN_02039394 @ 0x02039394 ---
void FUN_02039394(void) {
    u16 *puVar1;
    s32 iVar2;
    s32 iVar3;
    void (*pcVar4)(...);
    s32 iVar5;

    puVar1 = DAT_02039468;
    if (*DAT_02039464 != 0) {
        return;
    }
    if ((DAT_02039468[2] != 0) || (iVar2 = FUN_0203ae90(), iVar2 == 0x12)) {
        *puVar1 = 1;
        return;
    }
    iVar5 = *(s32 *)*DAT_0203946c;
    iVar2 = FUN_02039474();
    if (iVar5 == iVar2) {
        return;
    }
    if (iVar2 != 0) {
        iVar3 = FUN_020399d8(iVar5);
        if (iVar3 == 0) {
            if ((void (*)(...))*DAT_02039470 != (void (*)(...))(0)) {
                (*(void (*)(...))*DAT_02039470)(iVar5, iVar2);
            }
            pcVar4 = *(void (*)(...))(puVar1 + 8);
            if (pcVar4 != (void (*)(...))(0)) {
                pcVar4(iVar5, iVar2);
            }
            *(s32 *)(DAT_02039468 + 4) = iVar2;
            FUN_02039a24(iVar2);
            return;
        }
        return;
    }
}

// --- FUN_020394a0 @ 0x020394a0 ---
void FUN_020394a0(s32 param_1) {
    u32 uVar1;

    uVar1 = FUN_0203ae38();
    *(u32 *)(param_1 + 100) = 1;
    FUN_02039394();
    FUN_0203ae4c(uVar1);
}

// --- FUN_020394d4 @ 0x020394d4 ---
void FUN_020394d4(u16 *param_1) {
    u16 uVar1;
    u32 uVar2;
    s32 iVar3;

    uVar2 = FUN_0203ae38();
    uVar1 = *param_1;
    if (uVar1 != 0) {
        for (iVar3 = *(s32 *)(DAT_02039544 + 0xC); iVar3 != 0; iVar3 = *(s32 *)(iVar3 + 0x68)) {
            if (((u32)uVar1 & (1 << (*(u32 *)(iVar3 + 0x6C) & 0xFF))) != 0) {
                *(u32 *)(iVar3 + 100) = 1;
            }
        }
        *param_1 = 0;
        FUN_02039394();
    }
    FUN_0203ae4c(uVar2);
}

// --- FUN_02039548 @ 0x02039548 ---
void FUN_02039548(u16 *param_1) {
    u32 uVar1;
    s32 iVar2;

    uVar1 = FUN_0203ae38();
    iVar2 = *(s32 *)*DAT_020395ac;
    if (param_1 != (u16 *)0) {
        *param_1 = *param_1 | (u16)(1 << (*(u32 *)(iVar2 + 0x6C) & 0xFF));
    }
    *(u32 *)(iVar2 + 100) = 0;
    FUN_02039394();
    FUN_0203ae4c(uVar1);
}

// --- FUN_020395b0 @ 0x020395b0 ---
void FUN_020395b0(void) {
    s32 iVar1;

    FUN_0203ae38();
    iVar1 = *(s32 *)*DAT_02039604;
    FUN_02039e48(iVar1);
    FUN_02039894(iVar1);
    *(u32 *)(DAT_02039608 + *(s32 *)(iVar1 + 0x6C) * 4) = 0;
    *(u32 *)(iVar1 + 100) = 2;
    FUN_020394d4((u16 *)(iVar1 + 0x90));
    FUN_02039394();
    FUN_0203aeb4();
}

// --- FUN_0203960c @ 0x0203960c ---
void FUN_0203960c(s32 param_1, u32 param_2, u32 param_3, s32 param_4, s32 param_5,
                  u32 param_6) {
    s32 iVar1;
    u32 uVar2;
    u32 uVar3;
    s32 iVar4;

    uVar3 = FUN_0203ae38();
    iVar4 = FUN_02039944();
    if (iVar4 < 0) {
        FUN_0203ae4c(uVar3);
        return;
    }
    *(u32 *)(param_1 + 0x70) = param_6;
    *(s32 *)(param_1 + 0x6C) = iVar4;
    *(u32 *)(param_1 + 100) = 0;
    iVar1 = (s32)DAT_02039704;
    *(u32 *)(param_1 + 0x74) = 0;
    *(s32 *)(iVar1 + iVar4 * 4) = param_1;
    FUN_020398dc(param_1);
    *(s32 *)(param_1 + 0x88) = param_4;
    *(s32 *)(param_1 + 0x84) = param_4 - param_5;
    *(u32 *)(param_1 + 0x8C) = 0;
    uVar2 = DAT_0203970c;
    *(u32 *)(*(s32 *)(param_1 + 0x88) + -4) = DAT_02039708;
    *(u32 *)(*(s32 *)(param_1 + 0x84)) = uVar2;
    *(u16 *)(param_1 + 0x90) = 0;
    FUN_02039974(param_1, param_2, param_4 + -4);
    uVar2 = DAT_02039710;
    *(u32 *)(param_1 + 4) = param_3;
    *(u32 *)(param_1 + 0x3C) = uVar2;
    FUN_0203b7dc(0, (param_4 - param_5) + 4, param_5 + -8);
    *(u32 *)(param_1 + 0x78) = 0;
    *(u32 *)(param_1 + 0x7C) = 0;
    *(u32 *)(param_1 + 0x80) = 0;
    FUN_0203ae4c(uVar3);
}

// --- FUN_02039714 @ 0x02039714 ---
void FUN_02039714(void) {
    u16 *puVar1;
    u32 uVar2;
    u32 *puVar3;
    s32 iVar4;
    s32 iVar5;
    s32 iVar6;
    s32 iVar7;

    if (*DAT_02039858 != 0) {
        return;
    }
    *DAT_02039858 = 1;
    puVar1 = DAT_0203985c;
    iVar4 = 0;
    do {
        iVar5 = iVar4 + 1;
        *(u32 *)(puVar1 + iVar4 * 2 + 10) = 0;
        iVar6 = (s32)DAT_02039864;
        iVar7 = (s32)DAT_02039860;
        iVar4 = iVar5;
    } while (iVar5 < 16);
    *(u32 *)(iVar7 + 0x70) = 0x10;
    puVar3 = DAT_0203986c;
    uVar2 = DAT_02039868;
    *(u32 *)(iVar7 + 0x6C) = 0;
    *(u32 *)(iVar7 + 100) = 1;
    *(u32 *)(iVar7 + 0x68) = 0;
    *(u32 *)(iVar7 + 0x74) = 0;
    *(s32 *)(puVar1 + 10) = iVar7;
    *(s32 *)(puVar1 + 6) = iVar7;
    *(s32 *)(puVar1 + 4) = iVar7;
    if (iVar6 == 0) {
        iVar7 = (s32)DAT_02039870;
    }
    *puVar3 = uVar2;
    uVar2 = DAT_0203987c;
    iVar4 = (s32)DAT_02039860;
    if (iVar6 != 0) {
        iVar7 = (s32)(((s32)DAT_02039874 + 0x3F80) - (s32)DAT_02039878) - iVar6;
    }
    iVar6 = (s32)(((s32)DAT_02039874 + 0x3F80) - (s32)DAT_02039878);
    *(s32 *)(iVar4 + 0x88) = iVar6;
    *(s32 *)(iVar4 + 0x84) = iVar7;
    *(u32 *)(iVar4 + 0x8C) = 0;
    *(u32 *)(iVar6 + -4) = uVar2;
    puVar1 = DAT_0203985c;
    *(u32 *)(*(s32 *)(iVar4 + 0x84)) = DAT_02039880;
    puVar3 = DAT_02039884;
    *(u16 *)(iVar4 + 0x90) = 0;
    puVar1[1] = 0x10;
    *puVar1 = 0;
    puVar1[2] = 0;
    *puVar3 = (u32)puVar1;
    FUN_020392a4();
    FUN_0203960c(DAT_02039888, DAT_0203988c, 0, DAT_02039890, 0, 0);
    iVar4 = (s32)DAT_02039888;
    *(u32 *)(iVar4 + 0x70) = 0x20;
    *(u32 *)(iVar4 + 100) = 1;
}

// --- FUN_02039894 @ 0x02039894 ---
void FUN_02039894(s32 param_1) {
    s32 iVar1;
    s32 iVar2;
    s32 iVar3;

    iVar1 = *(s32 *)(DAT_020398d8 + 0xC);
    iVar3 = 0;
    while (iVar2 = iVar1, iVar2 != 0 && iVar2 != param_1) {
        iVar3 = iVar2;
        iVar1 = *(s32 *)(iVar2 + 0x68);
    }
    if (iVar3 == 0) {
        *(u32 *)(DAT_020398d8 + 0xC) = *(u32 *)(param_1 + 0x68);
    } else {
        *(u32 *)(iVar3 + 0x68) = *(u32 *)(param_1 + 0x68);
    }
}
