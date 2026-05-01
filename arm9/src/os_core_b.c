/**
 * OS Core B — memory management, heap, cache operations (0x02033xxx-0x02036xxx)
 * Decompiled from 0x020332e8-0x02036430 region (58 functions)
 */
#include "types.h"

typedef void (*FuncPtr)(...);

/* ======== Extern data references (sorted by address) ======== */

extern u32 DAT_02033618;
extern u32 DAT_0203361c;
extern int DAT_02033704;
extern int DAT_020339f4;
extern u32 DAT_020339f8;
extern u32 *DAT_02033ae0;
extern u32 DAT_02033ae4;
extern int DAT_02033ae8;
extern u32 DAT_02033aec;
extern u32 DAT_02033af0;
extern u32 *DAT_02033af4;
extern int DAT_02033af8;
extern u32 DAT_02033afc;
extern int DAT_02033d48;
extern int DAT_02033eec;
extern int DAT_02034e2c;
extern int DAT_02034e30;
extern int DAT_02034e34;
extern int DAT_02034e38;
extern int DAT_02034e3c;
extern int DAT_02034e40;
extern int DAT_02034e44;
extern u32 DAT_02034e48;
extern u32 DAT_0203501c;
extern u32 DAT_02035108;
extern u32 DAT_020355c0;
extern u32 DAT_020355d0;
extern u32 DAT_02035e2c;

/* Hardware-mapped pointer externs */
extern u16 *DAT_020347f0;
extern u32 *DAT_020347f4;
extern u32 *DAT_020347f8;
extern u32 *DAT_020347fc;
extern u32 *DAT_0203482c;
extern u32 *DAT_02034830;
extern u32 *DAT_02034834;
extern u16 *DAT_0203485c;
extern u32 *DAT_02034860;
extern u32 *DAT_020348ac;
extern u32 *DAT_020348b0;
extern u32 *DAT_020348e4;
extern u32 *DAT_020348e8;
extern u32 *DAT_020348ec;
extern u16 *DAT_02034924;
extern u32 *DAT_02034928;
extern u16 *DAT_0203494c;
extern u64 *DAT_02034950;
extern u16 *DAT_020349c0;
extern u32 *DAT_020349c4;
extern u32 *DAT_02034a10;
extern u32 *DAT_02034a14;
extern vu16 *DAT_02034b74;
extern u32 *DAT_02034b78;
extern s64 *DAT_02034b7c;
extern u16 *DAT_02034b80;
extern s64 *DAT_02034b84;
extern u32 *DAT_02034b88;
extern u32 *DAT_02034b8c;

/* Display/sound register pointers */
extern u16 *DAT_02035210;
extern u32 *DAT_02035214;
extern u32 *DAT_02035258;
extern u16 *DAT_0203525c;
extern u32 *DAT_0203580c;
extern u32 DAT_02035810;
extern u32 DAT_02035814;
extern u32 *DAT_02035834;
extern u32 DAT_02035838;
extern u32 DAT_0203583c;
extern u32 DAT_0203584c;
extern u32 DAT_02035850;
extern u32 DAT_02035860;
extern u32 DAT_02035864;
extern u32 DAT_02035884;
extern u32 DAT_02035888;
extern u32 DAT_020358a8;
extern u32 DAT_020358ac;

/* Sound channel register pointers */
extern u16 *DAT_02035968;
extern u32 *DAT_0203596c;
extern u8 *DAT_02035970;
extern u16 *DAT_020359f4;
extern u32 *DAT_020359f8;
extern u8 *DAT_020359fc;
extern u16 *DAT_02035a70;
extern u8 *DAT_02035a74;
extern u8 *DAT_02035a78;
extern u16 *DAT_02035b18;
extern u8 *DAT_02035b1c;
extern u8 *DAT_02035b20;
extern u8 *DAT_02035b24;
extern u16 *DAT_02035c2c;
extern u8 *DAT_02035c30;
extern u8 *DAT_02035c34;
extern u8 *DAT_02035c38;
extern u16 *DAT_02035e24;
extern u16 *DAT_02035e28;
extern u8 *DAT_02035e30;
extern u8 *DAT_02035e34;
extern u8 *DAT_02035e38;
extern u8 *DAT_02035e3c;
extern u16 *DAT_02035eec;
extern u8 *DAT_02035ef0;
extern u8 *DAT_02035ef4;
extern u16 *DAT_02035ffc;
extern u8 *DAT_02036000;
extern u8 *DAT_02036004;
extern u8 *DAT_02036008;
extern u16 *DAT_02036160;
extern u8 *DAT_02036164;
extern u8 *DAT_02036168;
extern u8 *DAT_0203616c;
extern u8 *DAT_02036170;
extern u8 *DAT_02036174;
extern u16 *DAT_02036410;
extern u8 *DAT_02036414;
extern u8 *DAT_02036418;
extern u8 *DAT_0203641c;
extern u8 *DAT_02036420;
extern u8 *DAT_02036424;
extern u8 *DAT_02036428;
extern u8 *DAT_0203642c;
extern u8 *DAT_020364c4;
extern u8 *DAT_020364c8;
extern u8 *DAT_020364cc;
extern u8 *DAT_020364d0;
extern u8 *DAT_020364d4;
extern u8 *DAT_020364d8;
extern u8 *DAT_020364dc;
extern u8 *DAT_020364e0;
extern u8 *DAT_020364e4;

/* Memory-mapped I/O */
extern vu32 _DAT_04000000;

/* ======== Extern function declarations ======== */

extern void FUN_02033150(...);
extern int FUN_02033198(...);
extern void FUN_02033248(...);
extern void FUN_020308b4(...);
extern int FUN_020308d4(...);
extern void FUN_020330d8(...);
extern u32 FUN_02030980(...);
extern int FUN_02030a0c(...);
extern int FUN_02030ad4(...);
extern int FUN_020311a8(...);
extern void FUN_020341bc(...);
extern void FUN_020341ec(...);
extern int FUN_0203efa0(...);
extern void FUN_0203ef08(...);
extern u32 FUN_020466bc(...);
extern int FUN_0202fb00(...);
extern void FUN_0202f9bc(...);
extern void FUN_0202e53c(...);
extern void FUN_0202e5d0(...);
extern void FUN_0202fd64(...);
extern void FUN_02039f94(...);
extern void FUN_0203a04c(...);
extern void FUN_0203b854(...);
extern void FUN_0203b8c4(...);
extern void FUN_0203f2a0(...);

/* ======== Forward declarations ======== */

void FUN_020332e8(int param_1, int param_2);
u32 FUN_0203334c(u32 param_1, u32 *param_2, u32 param_3, u32 param_4, u32 param_5, int param_6, u32 param_7, u32 param_8, u32 param_9, u32 param_10);
void FUN_02033624(int param_1);
int FUN_02033660(int *param_1, int param_2, int param_3);
void FUN_02033820(int *param_1);
void FUN_02033838(u32 *param_1);
void FUN_02033844(int *param_1);
u32 FUN_02033870(u32 param_1);
void FUN_020338a0(int *param_1);
u32 FUN_020338b8(u32 param_1, u32 param_2);
u32 FUN_02033920(int param_1);
void FUN_020339fc(u32 param_1, u32 param_2);
void FUN_02033b00(int param_1, int param_2, u32 param_3);
void FUN_02033b4c(int param_1, int param_2, u32 param_3, int param_4);
void FUN_02033d4c(int param_1, int param_2, u32 param_3, int param_4);
void FUN_02033ef4(u32 param_1, u32 param_2, u32 param_3, u32 param_4, int param_5);
int FUN_02034110(int param_1, u32 param_2);
void FUN_0203420c(u32 *param_1, int param_2, u32 param_3);
void FUN_02034598(u32 *param_1, u32 *param_2, int param_3, int param_4, int param_5);
void FUN_020346c4(int *param_1, int *param_2, int param_3, int param_4, int param_5);
u32 FUN_020347a8(u32 param_1, u32 param_2);
void FUN_02034800(u32 param_1, u32 param_2);
u32 FUN_02034838(void);
void FUN_02034864(int param_1);
void FUN_020348b4(u32 param_1);
u32 FUN_020348f0(void);
u64 FUN_0203492c(void);
int FUN_02034954(int param_1);
void FUN_020349c8(int param_1);
void FUN_02034a18(void);
void FUN_02034a34(void);
void FUN_02034a50(u32 *param_1, int *param_2);
void FUN_02034b90(int *param_1, int *param_2, u32 *param_3);
u32 FUN_02034c2c(int *param_1, int *param_2);
int FUN_02034c70(int param_1, int param_2);
s16 FUN_02034e4c(int param_1, int param_2);
s16 FUN_02035020(int param_1);
void FUN_020351cc(void);
void FUN_02035218(void);
u16 FUN_020355b4(void);
u16 FUN_020355c4(void);
void FUN_020357f0(void);
void FUN_02035818(void);
void FUN_02035840(void);
void FUN_02035854(void);
void FUN_02035868(void);
void FUN_0203588c(void);
void FUN_020358e8(int param_1);
void FUN_02035974(int param_1);
void FUN_02035a00(int param_1);
void FUN_02035a7c(int param_1);
void FUN_02035b48(int param_1);
void FUN_02035c3c(int param_1);
void FUN_02035e40(int param_1);
void FUN_02035ef8(int param_1);
void FUN_0203600c(int param_1);
void FUN_02036178(int param_1);
void FUN_02036430(u32 param_1);

/* ======== Function implementations ======== */

/* 0x020332e8 */
void FUN_020332e8(int param_1, int param_2)
{
    if (-1 < *(int *)(param_1 + 0x10c) << 0x1e) {
        FUN_02033248();
        return;
    }
    if (param_2 == 0) {
        FUN_02033248();
        return;
    }
    FUN_020341bc(param_1 + 0xe4, 0);
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) | 8;
    *(u32 *)(param_1 + 0x14c) = 0;
    return;
}

/* 0x0203334c */
u32 FUN_0203334c(u32 param_1, u32 *param_2, u32 param_3, u32 param_4,
                 u32 param_5, int param_6, u32 param_7, u32 param_8,
                 u32 param_9, u32 param_10)
{
    char cVar1;
    int iVar2;
    int iVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;
    u32 unaff_r5;
    u32 uStack_18;
    u32 uStack_14;

    iVar2 = FUN_02033660((int *)param_1, param_3, param_4);
    if (iVar2 == 0) { return 0; }
    iVar3 = FUN_020308d4(*param_2, iVar2 + 0xa4, 0x40, 0);
    if (iVar3 != 0x40) { FUN_02033624(iVar2); return 0; }
    FUN_020308b4(&uStack_18);
    iVar3 = FUN_0203efa0(iVar2 + 0x5c, uStack_18, uStack_14);
    if (iVar3 == 0) { FUN_02033624(iVar2); return 0; }
    uVar4 = FUN_02030980(*param_2);
    *(u32 *)(iVar2 + 0xa0) = uVar4;
    *(u32 *)(iVar2 + 0x158) = (u32)((u64)DAT_02033618 * (u64)((u32)*(u16 *)(iVar2 + 0xc0) * param_6) >> 0x26);
    if ((*(int *)(iVar2 + 0x158) == 0) || (*(char *)(iVar2 + 0xbc) != '\x02')) {
        *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xffffffef;
    } else {
        *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) | 0x10;
    }
    *(u32 *)(iVar2 + 0x110) = 4;
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xffffffdf;
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xfffffffd;
    *(u32 *)(iVar2 + 0x114) = 0;
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xfffffffb;
    uVar5 = *(u32 *)(iVar2 + 0x10c) & 0xfffffff7;
    *(u32 *)(iVar2 + 0x10c) = uVar5;
    *(u32 *)(iVar2 + 0x118) = 0;
    *(u32 *)(iVar2 + 0x130) = param_7;
    *(u32 *)(iVar2 + 0x134) = param_8;
    *(u32 *)(iVar2 + 0x138) = param_9;
    *(u32 *)(iVar2 + 0x13c) = param_10;
    *(u32 *)(iVar2 + 0x140) = param_5;
    *(u32 *)(iVar2 + 0x154) = 0;
    *(u32 *)(iVar2 + 0x150) = (u32)*(u8 *)(param_2 + 1);
    FUN_020341ec(iVar2 + 0xe4);
    FUN_020341bc(iVar2 + 0xe4, 0x7f00, 1);
    cVar1 = *(char *)(iVar2 + 0xbc);
    if (cVar1 == '\0') { unaff_r5 = 0; }
    else if ((cVar1 == '\x01') || (cVar1 == '\x02')) { unaff_r5 = 1; }
    uVar6 = (u32)*(u8 *)(iVar2 + 0xbe);
    if ((*(u8 *)((int)param_2 + 7) & 1) != 0) { uVar6 = 2; }
    iVar3 = iVar2 + DAT_0203361c;
    if (*(u8 *)(iVar2 + 0x120) < uVar6) { uVar6 = (u32)*(u8 *)(iVar2 + 0x120); }
    if (uVar6 == 1) { uVar5 = 1; }
    if (uVar6 != 1) { uVar5 = 0; }
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xffffffbf | (uVar5 & 1) << 6;
    iVar3 = FUN_02033198(iVar2, uVar6, iVar3);
    if (iVar3 == 0) { FUN_0203ef08(iVar2 + 0x5c); FUN_02033624(iVar2); return 0; }
    uVar4 = FUN_020466bc(*(int *)(iVar2 + 300) * uVar6, *(u8 *)(iVar2 + 0x120));
    iVar3 = FUN_0202fb00(iVar2, unaff_r5, *(u32 *)(iVar2 + 0x128), uVar4);
    if (iVar3 == 0) { FUN_02033150(iVar2); FUN_0203ef08(iVar2 + 0x5c); FUN_02033624(iVar2); return 0; }
    if (uVar6 == 2) { FUN_0202f9bc(iVar2, 0, 0); FUN_0202f9bc(iVar2, 1, 0x7f); }
    return 1;
}

/* 0x02033624 */
void FUN_02033624(int param_1)
{
    if (*(u32 **)(param_1 + 0x148) != (u32 *)0x0) {
        **(u32 **)(param_1 + 0x148) = 0;
        *(u32 *)(param_1 + 0x148) = 0;
    }
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) & 0xfffffffe;
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) & 0xfffffffb;
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) & 0xfffffffd;
    return;
}

/* 0x02033660 */
int FUN_02033660(int *param_1, int param_2, int param_3)
{
    int iVar1;
    if (*param_1 != 0) { FUN_02033820(param_1); }
    iVar1 = param_2 * 0x15c + DAT_02033704;
    if (*(int *)(iVar1 + 0x128) != 0) {
        if (*(int *)(iVar1 + 0x10c) << 0x1f < 0) {
            if (param_3 < *(int *)(iVar1 + 0x14c)) { return 0; }
            FUN_02033248(iVar1);
        }
        *(int *)(iVar1 + 0x14c) = param_3;
        *(u32 *)(iVar1 + 0x10c) = *(u32 *)(iVar1 + 0x10c) & 0xfffffffe | 1;
        *(int **)(iVar1 + 0x148) = param_1;
        *param_1 = iVar1;
        return iVar1;
    }
    return 0;
}

/* 0x02033820 */
void FUN_02033820(int *param_1)
{
    if (*param_1 != 0) {
        *(u32 *)(*param_1 + 0x148) = 0;
        *param_1 = 0;
    }
    return;
}

/* 0x02033838 */
void FUN_02033838(u32 *param_1)
{
    *param_1 = 0;
    return;
}

/* 0x02033844 */
void FUN_02033844(int *param_1)
{
    if (*param_1 != 0) { FUN_020332e8(*param_1, 0); return; }
    return;
}

/* 0x02033870 */
u32 FUN_02033870(u32 param_1)
{
    int iVar1;
    iVar1 = FUN_020338b8(param_1, 0);
    if (iVar1 != 0) { FUN_020338a0((int *)param_1); return 1; }
    return 0;
}

/* 0x020338a0 */
void FUN_020338a0(int *param_1)
{
    int iVar1;
    iVar1 = *param_1;
    if (iVar1 != 0) { *(u32 *)(iVar1 + 0x10c) = *(u32 *)(iVar1 + 0x10c) | 4; }
    return;
}

/* 0x020338b8 */
u32 FUN_020338b8(u32 param_1, u32 param_2)
{
    int iVar1;
    u32 uVar2;
    iVar1 = FUN_02030ad4(param_2);
    if (iVar1 != 0) {
        uVar2 = FUN_0203334c(param_1, (u32 *)iVar1, *(u8 *)(iVar1 + 6), *(u8 *)(iVar1 + 5), 0, 0, 0, 0, 0, 0);
        return uVar2;
    }
    return 0;
}

/* 0x02033920 */
u32 FUN_02033920(int param_1)
{
    u32 uVar1;
    u8 *pbVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    uVar1 = DAT_020339f8;
    iVar7 = 0;
    iVar6 = DAT_020339f4;
    do {
        pbVar2 = (u8 *)FUN_02030a0c(iVar7);
        if (pbVar2 != (u8 *)0x0) {
            *(u8 *)(iVar6 + 0x120) = *pbVar2;
            iVar4 = 0;
            if (*pbVar2 != 0) {
                do {
                    iVar3 = iVar4 + 1;
                    iVar5 = iVar6 + iVar4;
                    iVar4 = iVar4 + 1;
                    *(u8 *)(iVar5 + 0x122) = pbVar2[iVar3];
                } while (iVar4 < (int)(u32)*pbVar2);
            }
            if (param_1 != 0) {
                iVar4 = (u32)*(u8 *)(iVar6 + 0x120) << 0xb;
                iVar3 = FUN_020311a8(param_1, iVar4, uVar1, iVar6);
                if (iVar3 == 0) { return 0; }
                FUN_02033248(iVar6);
                *(int *)(iVar6 + 0x128) = iVar3;
                *(int *)(iVar6 + 300) = iVar4;
            }
        }
        iVar7 = iVar7 + 1;
        iVar6 = iVar6 + 0x15c;
    } while (iVar7 < 4);
    return 1;
}

/* 0x020339fc */
void FUN_020339fc(u32 param_1, u32 param_2)
{
    u32 uVar1;
    int iVar2;
    int iVar3;
    uVar1 = DAT_02033ae4;
    if (*DAT_02033ae0 != 0) { FUN_02033920(param_2); return; }
    *DAT_02033ae0 = 1;
    FUN_0202e5d0(uVar1, 0);
    uVar1 = DAT_02033ae4;
    iVar3 = 0;
    iVar2 = DAT_02033ae8;
    do {
        FUN_0202e53c(uVar1, iVar2);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + 0x30;
    } while (iVar3 < 8);
    FUN_02039f94(DAT_02033aec);
    iVar2 = DAT_02033af8;
    *DAT_02033af4 = DAT_02033af0;
    iVar3 = 0;
    do {
        *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x10c) & 0xfffffffe;
        FUN_0203f2a0(iVar2 + 0x5c);
        FUN_0202fd64(iVar2);
        *(int *)(iVar2 + 0x144) = iVar3;
        *(u8 *)(iVar2 + 0x120) = 0;
        *(u32 *)(iVar2 + 0x128) = 0;
        *(u32 *)(iVar2 + 300) = 0;
        iVar3 = iVar3 + 1;
        *(u32 *)(iVar2 + 0x11c) = 0;
        iVar2 = iVar2 + 0x15c;
    } while (iVar3 < 4);
    FUN_02033920(param_2);
    FUN_020330d8(DAT_02033afc, param_1);
    return;
}

/* 0x02033b00 */
void FUN_02033b00(int param_1, int param_2, u32 param_3)
{
    int iVar1;
    u32 uVar2;
    uVar2 = 0;
    if (param_3 >> 1 != 0) {
        do {
            iVar1 = uVar2 * 2;
            uVar2 = uVar2 + 1;
            *(s16 *)(param_1 + iVar1) = (s16)((int)*(s16 *)(param_1 + iVar1) + (int)*(s16 *)(param_2 + iVar1) + 1 >> 1);
        } while (uVar2 < param_3 >> 1);
    }
    FUN_0203b854();
    return;
}

/* 0x02033b4c */
void FUN_02033b4c(int param_1, int param_2, u32 param_3, int param_4)
{
    int iVar1;
    u32 uVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    iVar4 = DAT_02033d48;
    param_3 = param_3 >> 1;
    iVar8 = 0;
    iVar5 = iVar8;
    if (0x18 < param_3) {
        do {
            iVar8 = 0;
            do {
                iVar6 = iVar8 + iVar5;
                iVar7 = param_4 + iVar8 * 8;
                iVar3 = (int)*(s16 *)(param_1 + iVar6 * 2) + *(int *)(iVar7 + 0x18);
                iVar7 = (int)*(s16 *)(param_2 + iVar6 * 2) + *(int *)(iVar7 + 0x1c);
                if (iVar3 < -0x8000) { iVar1 = -0x8000; }
                else { iVar1 = iVar3; if (iVar4 < iVar3) { iVar1 = iVar4; } }
                *(s16 *)(param_1 + iVar6 * 2) = (s16)iVar1;
                if (iVar7 < -0x8000) { iVar1 = -0x8000; }
                else { iVar1 = iVar7; if (iVar4 < iVar7) { iVar1 = iVar4; } }
                iVar9 = param_4 + iVar8 * 8;
                *(s16 *)(param_2 + iVar6 * 2) = (s16)iVar1;
                iVar8 = iVar8 + 1;
                *(int *)(iVar9 + 0x18) = iVar7 + 1 >> 2;
                *(int *)(iVar9 + 0x1c) = iVar3 + 1 >> 2;
            } while (iVar8 < 0x18);
            iVar8 = iVar5 + 0x18;
            uVar2 = iVar5 + 0x30;
            iVar5 = iVar8;
        } while (uVar2 < param_3);
    }
    iVar4 = DAT_02033d48;
    param_3 = param_3 - iVar8;
    uVar2 = 0;
    if (param_3 != 0) {
        do {
            iVar6 = uVar2 + iVar8;
            iVar5 = param_4 + uVar2 * 8;
            iVar7 = (int)*(s16 *)(param_1 + iVar6 * 2) + *(int *)(iVar5 + 0x18);
            iVar3 = (int)*(s16 *)(param_2 + iVar6 * 2) + *(int *)(iVar5 + 0x1c);
            iVar5 = -0x8000;
            if ((-0x8001 < iVar7) && (iVar5 = iVar7, iVar4 < iVar7)) { iVar5 = iVar4; }
            *(s16 *)(param_1 + iVar6 * 2) = (s16)iVar5;
            iVar5 = -0x8000;
            if ((-0x8001 < iVar3) && (iVar5 = iVar3, iVar4 < iVar3)) { iVar5 = iVar4; }
            uVar2 = uVar2 + 1;
            *(s16 *)(param_2 + iVar6 * 2) = (s16)iVar5;
        } while (uVar2 < param_3);
    }
    uVar2 = 0;
    if (-param_3 != -0x18) {
        do {
            iVar5 = param_4 + param_3 * 8 + uVar2 * 8;
            iVar4 = param_4 + uVar2 * 8;
            *(u32 *)(iVar4 + 0x18) = *(u32 *)(iVar5 + 0x18);
            uVar2 = uVar2 + 1;
            *(u32 *)(iVar4 + 0x1c) = *(u32 *)(iVar5 + 0x1c);
        } while (uVar2 < -param_3 + 0x18);
    }
    uVar2 = 0;
    if (param_3 != 0) {
        do {
            iVar4 = (uVar2 + iVar8) * 2;
            iVar5 = param_4 + ((uVar2 + 0x18) - param_3) * 8;
            *(int *)(iVar5 + 0x18) = *(s16 *)(param_2 + iVar4) + 1 >> 2;
            uVar2 = uVar2 + 1;
            *(int *)(iVar5 + 0x1c) = *(s16 *)(param_1 + iVar4) + 1 >> 2;
        } while (uVar2 < param_3);
        return;
    }
    return;
}

/* 0x02033d4c */
void FUN_02033d4c(int param_1, int param_2, u32 param_3, int param_4)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    s16 *psVar5;
    s16 *psVar6;
    s16 local_28[4];
    iVar1 = DAT_02033eec;
    param_3 = param_3 >> 1;
    iVar2 = 0;
    do {
        iVar4 = (int)*(s16 *)(param_1 + param_3 * 2 + iVar2 * 2 + -4) - (int)*(s16 *)(param_2 + param_3 * 2 + iVar2 * 2 + -4);
        iVar3 = -0x8000;
        if ((-0x8001 < iVar4) && (iVar3 = iVar4, DAT_02033eec < iVar4)) { iVar3 = DAT_02033eec; }
        iVar4 = iVar2 + 1;
        local_28[iVar2] = (s16)iVar3;
        iVar2 = iVar4;
    } while (iVar4 < 2);
    psVar5 = (s16 *)(param_2 + (param_3 - 1) * 2);
    iVar2 = DAT_02033eec;
    for (psVar6 = (s16 *)(param_1 + (param_3 - 1) * 2); DAT_02033eec = iVar2,
        (s16 *)(param_1 + 4) <= psVar6; psVar6 = psVar6 + -1) {
        iVar3 = (int)psVar6[-2] - (int)psVar5[-2];
        iVar2 = *psVar6 + iVar3;
        iVar4 = *psVar5 - iVar3;
        if (iVar3 < 0) {
            if (iVar2 < -0x7fff) { *psVar6 = -0x8000; }
            else { *psVar6 = (s16)iVar2; }
            if (iVar4 < iVar1) { *psVar5 = (s16)iVar4; }
            else { *psVar5 = (s16)iVar1; }
        } else {
            if (iVar2 < iVar1) { *psVar6 = (s16)iVar2; }
            else { *psVar6 = (s16)iVar1; }
            if (iVar4 < -0x7fff) { *psVar5 = -0x8000; }
            else { *psVar5 = (s16)iVar4; }
        }
        psVar5 = psVar5 + -1;
        iVar2 = DAT_02033eec;
    }
    iVar1 = 1;
    do {
        iVar4 = (int)*(s16 *)(param_1 + iVar1 * 2) + *(int *)(param_4 + iVar1 * 4 + 0x18);
        iVar3 = -0x8000;
        if ((-0x8001 < iVar4) && (iVar3 = iVar4, iVar2 < iVar4)) { iVar3 = iVar2; }
        *(s16 *)(param_1 + iVar1 * 2) = (s16)iVar3;
        iVar4 = (int)*(s16 *)(param_2 + iVar1 * 2) - *(int *)(param_4 + iVar1 * 4 + 0x18);
        iVar3 = -0x8000;
        if ((-0x8001 < iVar4) && (iVar3 = iVar4, iVar2 < iVar4)) { iVar3 = iVar2; }
        *(s16 *)(param_2 + iVar1 * 2) = (s16)iVar3;
        iVar1 = iVar1 + -1;
    } while (-1 < iVar1);
    iVar1 = 0;
    do {
        psVar6 = local_28 + iVar1;
        iVar2 = iVar1 * 4;
        iVar1 = iVar1 + 1;
        *(int *)(param_4 + iVar2 + 0x18) = (int)*psVar6;
    } while (iVar1 < 2);
    return;
}

/* 0x02033ef4 */
void FUN_02033ef4(u32 param_1, u32 param_2, u32 param_3, u32 param_4, int param_5)
{
    if (*(FuncPtr *)(param_5 + 8) != (FuncPtr)0) { (*(FuncPtr *)(param_5 + 8))(); }
    (*(FuncPtr *)(param_5 + 4))(param_1, param_2, param_3, param_5);
    if (*(FuncPtr *)(param_5 + 0x10) != (FuncPtr)0) {
        (*(FuncPtr *)(param_5 + 0x10))(param_1, param_2, param_3, param_4, *(u32 *)(param_5 + 0x14));
    }
    FUN_0203a04c(param_1, param_3);
    FUN_0203a04c(param_2, param_3);
    return;
}

/* 0x02034110 */
int FUN_02034110(int param_1, u32 param_2)
{
    int iVar1;
    if ((int)param_2 < 0) { return 0; }
    if (param_2 < *(u32 *)(param_1 + 0x1c)) {
        iVar1 = param_1 + 0x20 + param_2 * 0xc;
        if (*(int *)(param_1 + 0x20 + param_2 * 0xc) == -1) { iVar1 = 0; }
        return iVar1;
    }
    return 0;
}

/* 0x0203420c */
void FUN_0203420c(u32 *param_1, int param_2, u32 param_3)
{
    *param_1 = param_3;
    param_1[1] = param_2;
    param_1[2] = 0;
    param_1[3] = -param_2;
    param_1[4] = param_3;
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0x1000;
    return;
}

/* 0x02034598 */
void FUN_02034598(u32 *param_1, u32 *param_2, int param_3, int param_4, int param_5)
{
    u32 uVar1;
    *param_2 = (u32)((s64)param_3 * (s64)(int)*param_1) >> 0xc | (int)((u64)((s64)param_3 * (s64)(int)*param_1) >> 0x20) << 0x14;
    param_2[1] = (u32)((s64)param_3 * (s64)(int)param_1[1]) >> 0xc | (int)((u64)((s64)param_3 * (s64)(int)param_1[1]) >> 0x20) << 0x14;
    param_2[2] = (u32)((s64)param_3 * (s64)(int)param_1[2]) >> 0xc | (int)((u64)((s64)param_3 * (s64)(int)param_1[2]) >> 0x20) << 0x14;
    param_2[3] = (u32)((s64)param_3 * (s64)(int)param_1[3]) >> 0xc | (int)((u64)((s64)param_3 * (s64)(int)param_1[3]) >> 0x20) << 0x14;
    param_2[4] = (u32)((s64)param_4 * (s64)(int)param_1[4]) >> 0xc | (int)((u64)((s64)param_4 * (s64)(int)param_1[4]) >> 0x20) << 0x14;
    param_2[5] = (u32)((s64)param_4 * (s64)(int)param_1[5]) >> 0xc | (int)((u64)((s64)param_4 * (s64)(int)param_1[5]) >> 0x20) << 0x14;
    param_2[6] = (u32)((s64)param_4 * (s64)(int)param_1[6]) >> 0xc | (int)((u64)((s64)param_4 * (s64)(int)param_1[6]) >> 0x20) << 0x14;
    param_2[7] = (u32)((s64)param_4 * (s64)(int)param_1[7]) >> 0xc | (int)((u64)((s64)param_4 * (s64)(int)param_1[7]) >> 0x20) << 0x14;
    param_2[8] = (u32)((s64)param_5 * (s64)(int)param_1[8]) >> 0xc | (int)((u64)((s64)param_5 * (s64)(int)param_1[8]) >> 0x20) << 0x14;
    param_2[9] = (u32)((s64)param_5 * (s64)(int)param_1[9]) >> 0xc | (int)((u64)((s64)param_5 * (s64)(int)param_1[9]) >> 0x20) << 0x14;
    param_2[10] = (u32)((s64)param_5 * (s64)(int)param_1[10]) >> 0xc | (int)((u64)((s64)param_5 * (s64)(int)param_1[10]) >> 0x20) << 0x14;
    param_2[11] = (u32)((s64)param_5 * (s64)(int)param_1[11]) >> 0xc | (int)((u64)((s64)param_5 * (s64)(int)param_1[11]) >> 0x20) << 0x14;
    if (param_1 != param_2) {
        uVar1 = param_1[0xd];
        param_2[0xc] = param_1[0xc];
        param_2[0xd] = uVar1;
        uVar1 = param_1[0xf];
        param_2[0xe] = param_1[0xe];
        param_2[0xf] = uVar1;
        return;
    }
    return;
}

/* 0x020346c4 */
void FUN_020346c4(int *param_1, int *param_2, int param_3, int param_4, int param_5)
{
    s64 lVar1;
    if (param_1 != param_2) { FUN_0203b8c4(param_1, param_2); }
    lVar1 = (s64)param_1[8] * (s64)param_5 + (s64)*param_1 * (s64)param_3 + (s64)param_4 * (s64)param_1[4];
    param_2[0xc] = param_1[0xc] + ((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14);
    lVar1 = (s64)param_1[9] * (s64)param_5 + (s64)param_1[1] * (s64)param_3 + (s64)param_4 * (s64)param_1[5];
    param_2[0xd] = param_1[0xd] + ((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14);
    lVar1 = (s64)param_1[10] * (s64)param_5 + (s64)param_1[2] * (s64)param_3 + (s64)param_4 * (s64)param_1[6];
    param_2[0xe] = param_1[0xe] + ((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14);
    lVar1 = (s64)param_1[0xb] * (s64)param_5 + (s64)param_1[3] * (s64)param_3 + (s64)param_4 * (s64)param_1[7];
    param_2[0xf] = param_1[0xf] + ((u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) << 0x14);
    return;
}

/* 0x020347a8 */
u32 FUN_020347a8(u32 param_1, u32 param_2)
{
    u16 *puVar1;
    u32 *puVar2;
    u32 *puVar3;
    puVar2 = DAT_020347f4;
    puVar1 = DAT_020347f0;
    *DAT_020347f0 = 0;
    puVar3 = DAT_020347f8;
    *puVar2 = param_1;
    *puVar3 = param_2;
    puVar3[1] = 0;
    do { } while ((*puVar1 & 0x8000) != 0);
    return *DAT_020347fc;
}

/* 0x02034800 */
void FUN_02034800(u32 param_1, u32 param_2)
{
    u32 *puVar1;
    u32 *puVar2;
    puVar1 = DAT_02034830;
    *DAT_0203482c = 1;
    *puVar1 = 0;
    puVar2 = DAT_02034834;
    puVar1[1] = param_1;
    *puVar2 = param_2;
    puVar2[1] = 0;
    return;
}

/* 0x02034838 */
u32 FUN_02034838(void)
{
    do { } while ((*DAT_0203485c & 0x8000) != 0);
    return *DAT_02034860 + 0x200U >> 10;
}

/* 0x02034864 */
void FUN_02034864(int param_1)
{
    u32 *puVar1;
    puVar1 = DAT_020348b0;
    if (0 < param_1) {
        *DAT_020348ac = 1;
        puVar1 = DAT_020348b0;
        *DAT_020348b0 = 0;
        puVar1[1] = param_1;
        return;
    }
    *DAT_020348ac = 1;
    *puVar1 = 0;
    puVar1[1] = 0;
    return;
}

/* 0x020348b4 */
void FUN_020348b4(u32 param_1)
{
    u32 *puVar1;
    u32 *puVar2;
    puVar1 = DAT_020348e8;
    *DAT_020348e4 = 1;
    *puVar1 = 0;
    puVar2 = DAT_020348ec;
    puVar1[1] = 0x1000;
    *puVar2 = param_1;
    puVar2[1] = 0;
    return;
}

/* 0x020348f0 */
u32 FUN_020348f0(void)
{
    do { } while ((*DAT_02034924 & 0x8000) != 0);
    return *DAT_02034928 + 0x80000 >> 0x14 | (DAT_02034928[1] + (u32)(0xfff7ffff < *DAT_02034928)) * 0x1000;
}

/* 0x0203492c */
u64 FUN_0203492c(void)
{
    do { } while ((*DAT_0203494c & 0x8000) != 0);
    return *DAT_02034950;
}

/* 0x02034954 */
int FUN_02034954(int param_1)
{
    u32 uVar1;
    u64 uVar2;
    if (0 < param_1) {
        FUN_020348b4(param_1);
        FUN_02034864(param_1);
        uVar2 = FUN_0203492c();
        do { } while ((*DAT_020349c0 & 0x8000) != 0);
        uVar1 = *DAT_020349c4;
        return (int)((int)(uVar2 >> 0x20) * uVar1 + (int)uVar2 * ((int)uVar1 >> 0x1f) + (int)((u64)uVar1 * (uVar2 & 0xffffffff) >> 0x20) + 0x200) >> 10;
    }
    return 0;
}

/* 0x020349c8 */
void FUN_020349c8(int param_1)
{
    u32 *puVar1;
    if (0 < param_1) {
        *DAT_02034a10 = 1;
        puVar1 = DAT_02034a14;
        *DAT_02034a14 = 0;
        puVar1[1] = param_1;
        FUN_02034838();
        return;
    }
    return;
}

/* 0x02034a18 */
void FUN_02034a18(void)
{
    FUN_020348b4(0);
    FUN_020348f0();
    return;
}

/* 0x02034a34 */
void FUN_02034a34(void)
{
    FUN_02034800(0, 0);
    FUN_020348f0();
    return;
}

/* 0x02034a50 */
void FUN_02034a50(u32 *param_1, int *param_2)
{
    s64 lVar1;
    u64 uVar2;
    u32 *puVar3;
    s64 *plVar4;
    u16 *puVar5;
    int iVar6;
    int iVar7;
    u32 uVar8;
    u32 uVar9;
    puVar3 = DAT_02034b78;
    lVar1 = (s64)(int)param_1[2] * (s64)(int)param_1[2] + (s64)(int)*param_1 * (s64)(int)*param_1 + (s64)(int)param_1[1] * (s64)(int)param_1[1];
    *DAT_02034b74 = 2;
    *puVar3 = 0;
    plVar4 = DAT_02034b7c;
    puVar3[1] = 0x1000000;
    puVar5 = DAT_02034b80;
    *plVar4 = lVar1;
    plVar4 = DAT_02034b84;
    *puVar5 = 1;
    *plVar4 = lVar1 * 4;
    do { } while ((*puVar5 & 0x8000) != 0);
    uVar9 = *DAT_02034b88;
    do { } while ((*DAT_02034b74 & 0x8000) != 0);
    uVar8 = *param_1;
    uVar2 = (u64)uVar9 * (u64)*DAT_02034b8c;
    iVar7 = (int)uVar2;
    iVar6 = DAT_02034b8c[1] * uVar9 + *DAT_02034b8c * ((int)uVar9 >> 0x1f) + (int)(uVar2 >> 0x20);
    *param_2 = (int)(iVar6 * uVar8 + iVar7 * ((int)uVar8 >> 0x1f) + (int)((u64)uVar8 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >> 0xd;
    uVar9 = param_1[1];
    param_2[1] = (int)(iVar6 * uVar9 + iVar7 * ((int)uVar9 >> 0x1f) + (int)((u64)uVar9 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >> 0xd;
    uVar9 = param_1[2];
    param_2[2] = (int)(iVar6 * uVar9 + iVar7 * ((int)uVar9 >> 0x1f) + (int)((u64)uVar9 * (uVar2 & 0xffffffff) >> 0x20) + 0x1000) >> 0xd;
    return;
}

/* 0x02034b90 */
void FUN_02034b90(int *param_1, int *param_2, u32 *param_3)
{
    s64 lVar1; s64 lVar2; s64 lVar3; s64 lVar4; s64 lVar5; s64 lVar6;
    u32 uVar7; u32 uVar8; u32 uVar9; u32 uVar10; u32 uVar11; u32 uVar12; u32 uVar13; u32 uVar14; u32 uVar15;
    lVar1 = (s64)param_1[1] * (s64)param_2[2];
    uVar10 = (u32)lVar1;
    lVar2 = (s64)param_1[2] * (s64)param_2[1];
    uVar7 = (u32)lVar2;
    uVar12 = uVar10 - uVar7;
    lVar3 = (s64)param_1[2] * (s64)*param_2;
    uVar11 = (u32)lVar3;
    lVar4 = (s64)*param_1 * (s64)param_2[2];
    uVar15 = (u32)lVar4;
    lVar5 = (s64)*param_1 * (s64)param_2[1];
    uVar8 = (u32)lVar5;
    lVar6 = (s64)param_1[1] * (s64)*param_2;
    uVar14 = (u32)lVar6;
    uVar13 = uVar11 - uVar15;
    uVar9 = uVar8 - uVar14;
    *param_3 = uVar12 + 0x800 >> 0xc | (((int)((u64)lVar1 >> 0x20) - ((int)((u64)lVar2 >> 0x20) + (u32)(uVar10 < uVar7))) + (u32)(0xfffff7ff < uVar12)) * 0x100000;
    param_3[1] = uVar13 + 0x800 >> 0xc | (((int)((u64)lVar3 >> 0x20) - ((int)((u64)lVar4 >> 0x20) + (u32)(uVar11 < uVar15))) + (u32)(0xfffff7ff < uVar13)) * 0x100000;
    param_3[2] = uVar9 + 0x800 >> 0xc | (((int)((u64)lVar5 >> 0x20) - ((int)((u64)lVar6 >> 0x20) + (u32)(uVar8 < uVar14))) + (u32)(0xfffff7ff < uVar9)) * 0x100000;
    return;
}

/* 0x02034c2c */
u32 FUN_02034c2c(int *param_1, int *param_2)
{
    s64 lVar1;
    lVar1 = (s64)param_2[2] * (s64)param_1[2] + (s64)*param_2 * (s64)*param_1 + (s64)param_1[1] * (s64)param_2[1] + 0x800;
    return (u32)lVar1 >> 0xc | (int)((u64)lVar1 >> 0x20) * 0x100000;
}

/* 0x02034c70 */
int FUN_02034c70(int param_1, int param_2)
{
    int iVar1; int iVar2; int iVar3; int iVar4;
    if (param_1 < 1) {
        if (-1 < param_1) {
            iVar2 = DAT_02034e34;
            if (-1 < param_2) { iVar2 = 0; }
            return iVar2;
        }
        iVar2 = -param_1;
        iVar3 = iVar2;
        if (param_2 < 0) {
            iVar4 = -param_2;
            if (param_1 == param_2 || iVar4 < iVar2) {
                if (iVar2 <= iVar4) { return DAT_02034e44; }
                iVar1 = 0; iVar2 = iVar4; iVar4 = DAT_02034e40;
            } else { iVar1 = 1; iVar3 = iVar4; iVar4 = DAT_02034e3c; }
        } else {
            if (param_2 < 1) { return DAT_02034e40; }
            if (param_2 < iVar2) { iVar1 = 1; iVar2 = param_2; iVar4 = DAT_02034e40; }
            else { if (param_2 <= iVar2) { return -0xc91; } iVar1 = 0; iVar3 = param_2; iVar4 = iVar1; }
        }
    } else {
        iVar3 = param_1;
        iVar4 = DAT_02034e2c;
        if (param_2 < 1) {
            if (-1 < param_2) { return DAT_02034e2c; }
            iVar2 = -param_2;
            if (iVar2 < param_1) { iVar1 = 1; }
            else { if (iVar2 <= param_1) { return DAT_02034e38; } iVar1 = 0; iVar3 = iVar2; iVar2 = param_1; iVar4 = DAT_02034e34; }
        } else if (param_1 < param_2) { iVar1 = 1; iVar3 = param_2; iVar2 = param_1; iVar4 = 0; }
        else { if (param_1 <= param_2) { return DAT_02034e30; } iVar1 = 0; iVar2 = param_2; }
    }
    if (iVar3 == 0) { return 0; }
    if (iVar1 != 0) {
        FUN_02034a34();
        iVar2 = FUN_020347a8(iVar2, iVar3);
        return (iVar4 + *(s16 *)(DAT_02034e48 + (iVar2 >> 5) * 2)) * 0x10000 >> 0x10;
    }
    FUN_02034a34();
    iVar2 = FUN_020347a8(iVar2, iVar3);
    return (iVar4 - *(s16 *)(DAT_02034e48 + (iVar2 >> 5) * 2)) * 0x10000 >> 0x10;
}

/* 0x02034e4c */
s16 FUN_02034e4c(int param_1, int param_2)
{
    int iVar1; s16 sVar2; s16 sVar3; int iVar4;
    if (param_1 < 1) {
        if (-1 < param_1) {
            if (param_2 < 0) { sVar2 = -0x8000; } else { sVar2 = 0; }
            return sVar2;
        }
        iVar4 = -param_1;
        if (param_2 < 0) {
            iVar1 = -param_2;
            if (param_1 == param_2 || iVar1 < iVar4) {
                if (iVar4 <= iVar1) { return -0x6000; }
                sVar2 = -0x4000; param_1 = iVar4; iVar4 = iVar1; sVar3 = 0;
            } else { sVar2 = (s16)-0x8000; param_1 = iVar1; sVar3 = 1; }
        } else {
            if (param_2 < 1) { return -0x4000; }
            if (param_2 < iVar4) { sVar2 = -0x4000; param_1 = iVar4; iVar4 = param_2; sVar3 = 1; }
            else { if (param_2 <= iVar4) { return -0x2000; } sVar2 = 0; param_1 = param_2; sVar3 = sVar2; }
        }
    } else if (param_2 < 1) {
        if (-1 < param_2) { return 0x4000; }
        iVar4 = -param_2;
        if (iVar4 < param_1) { sVar2 = 0x4000; sVar3 = 1; }
        else { if (iVar4 <= param_1) { return 0x6000; } sVar2 = (s16)-0x8000; param_1 = iVar4; iVar4 = param_1; sVar3 = 0; }
    } else if (param_1 < param_2) {
        sVar2 = 0; param_1 = param_2; iVar4 = param_1; sVar3 = 1;
    } else {
        if (param_1 <= param_2) { return 0x2000; }
        sVar2 = 0x4000; iVar4 = param_2; sVar3 = 0;
    }
    if (param_1 != 0) {
        if (sVar3 == 0) {
            FUN_02034a34();
            iVar4 = FUN_020347a8(iVar4, param_1);
            return sVar2 - *(s16 *)(DAT_0203501c + (iVar4 >> 5) * 2);
        }
        FUN_02034a34();
        iVar4 = FUN_020347a8(iVar4, param_1);
        return sVar2 + *(s16 *)(DAT_0203501c + (iVar4 >> 5) * 2);
    }
    return 0;
}

/* 0x02035020 */
s16 FUN_02035020(int param_1)
{
    s16 sVar1; int iVar2;
    if (-1 < param_1) {
        if (param_1 < 0x1001) {
            if (param_1 < 0x1000) { sVar1 = *(s16 *)(DAT_02035108 + (param_1 >> 5) * 2); }
            else { sVar1 = 0x2000; }
            return sVar1;
        }
        FUN_02034a18();
        iVar2 = FUN_020347a8(param_1, 0x1000);
        return 0x4000 - *(s16 *)(DAT_02035108 + (iVar2 >> 5) * 2);
    }
    if (-0x1001 < param_1) {
        if (-0x1000 < param_1) { return -*(s16 *)(DAT_02035108 + (-param_1 >> 5) * 2); }
        return -0x2000;
    }
    FUN_02034a18();
    iVar2 = FUN_020347a8(-param_1, 0x1000);
    return *(s16 *)(DAT_02035108 + (iVar2 >> 5) * 2) + -0x4000;
}

/* 0x020351cc */
void FUN_020351cc(void)
{
    u16 uVar1;
    uVar1 = *DAT_02035210;
    *DAT_02035214 = 1;
    if (uVar1 != 0) {
        _DAT_04000000 = _DAT_04000000 & 0xfffcffff | (u32)uVar1 << 0x10;
        return;
    }
    _DAT_04000000 = _DAT_04000000 | 0x10000;
    return;
}

/* 0x02035218 */
void FUN_02035218(void)
{
    u16 *puVar1;
    puVar1 = DAT_0203525c;
    *DAT_02035258 = 0;
    *puVar1 = (u16)(_DAT_04000000 >> 0x10) & 3;
    _DAT_04000000 = _DAT_04000000 & 0xfffcffff;
    return;
}

/* 0x020355b4 */
u16 FUN_020355b4(void)
{
    return *(u16 *)(DAT_020355c0 + 0x18);
}

/* 0x020355c4 */
u16 FUN_020355c4(void)
{
    return *(u16 *)(DAT_020355d0 + 0x10);
}

/* 0x020357f0 */
void FUN_020357f0(void)
{
    u32 uVar1;
    FuncPtr func;
    uVar1 = DAT_02035814;
    func = (FuncPtr)DAT_02035810;
    *DAT_0203580c = *DAT_0203580c & 0x7fffffff;
    func(uVar1);
    return;
}

/* 0x02035818 */
void FUN_02035818(void)
{
    u32 uVar1;
    FuncPtr func;
    uVar1 = DAT_0203583c;
    func = (FuncPtr)DAT_02035838;
    *DAT_02035834 = *DAT_02035834 & 0xbfffffff;
    func(uVar1);
    return;
}

/* 0x02035840 */
void FUN_02035840(void)
{
    ((FuncPtr)DAT_0203584c)(DAT_02035850);
    return;
}

/* 0x02035854 */
void FUN_02035854(void)
{
    ((FuncPtr)DAT_02035860)(DAT_02035864);
    return;
}

/* 0x02035868 */
void FUN_02035868(void)
{
    _DAT_04000000 = _DAT_04000000 & 0x7fffffff;
    ((FuncPtr)DAT_02035884)(DAT_02035888);
    return;
}

/* 0x0203588c */
void FUN_0203588c(void)
{
    _DAT_04000000 = _DAT_04000000 & 0xbfffffff;
    ((FuncPtr)DAT_020358a8)(DAT_020358ac);
    return;
}

/* 0x020358e8 */
void FUN_020358e8(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    u8 *puVar4;
    puVar3 = DAT_02035968;
    uVar1 = DAT_02035968[0xc];
    uVar2 = *DAT_02035968;
    DAT_02035968[0xc] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    puVar4 = DAT_02035970;
    if (param_1 == 0) {
        *DAT_0203596c = *DAT_0203596c & 0x7fffffff;
    } else if (param_1 == 0x100) {
        *DAT_0203596c = *DAT_0203596c | 0x80000000;
        *puVar4 = 0x83;
    }
    FUN_02036430(*DAT_02035968);
    return;
}

/* 0x02035974 */
void FUN_02035974(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    u8 *puVar4;
    puVar3 = DAT_020359f4;
    uVar1 = DAT_020359f4[0xb];
    uVar2 = *DAT_020359f4;
    DAT_020359f4[0xb] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    puVar4 = DAT_020359fc;
    if (param_1 == 0) {
        *DAT_020359f8 = *DAT_020359f8 & 0xbfffffff;
    } else if (param_1 == 0x80) {
        *DAT_020359f8 = *DAT_020359f8 | 0x40000000;
        *puVar4 = 0x82;
    }
    FUN_02036430(*DAT_020359f4);
    return;
}

/* 0x02035a00 */
void FUN_02035a00(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    puVar3 = DAT_02035a70;
    uVar1 = DAT_02035a70[10];
    uVar2 = *DAT_02035a70;
    DAT_02035a70[10] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 != 0) {
        if (param_1 == 8) { *DAT_02035a78 = 0x84; }
        else if (param_1 == 0x100) { *DAT_02035a74 = 0x82; }
    }
    FUN_02036430(*DAT_02035a70);
    return;
}

/* 0x02035a7c */
void FUN_02035a7c(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    puVar3 = DAT_02035b18;
    uVar1 = DAT_02035b18[9];
    uVar2 = *DAT_02035b18;
    DAT_02035b18[9] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 < 0x81) {
        if (param_1 < 0x80) {
            if ((((param_1 < 5) && (-1 < param_1)) && (param_1 != 0)) && (param_1 == 4)) {
                *DAT_02035b1c = 0x84;
            }
            goto LAB_02035b00;
        }
    } else {
        if (param_1 != 0x180) goto LAB_02035b00;
        *DAT_02035b20 = 0x81;
    }
    *DAT_02035b24 = 0x81;
LAB_02035b00:
    FUN_02036430(*DAT_02035b18);
    return;
}

/* 0x02035b48 */
void FUN_02035b48(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    puVar3 = DAT_02035c2c;
    uVar1 = DAT_02035c2c[5];
    uVar2 = *DAT_02035c2c;
    DAT_02035c2c[5] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 < 0x31) {
        if (0x2f < param_1) goto LAB_02035bfc;
        if (0x10 < param_1) { if (param_1 != 0x20) goto LAB_02035c14; goto LAB_02035be0; }
        if (param_1 < 0x10) goto LAB_02035c14;
    } else {
        if (param_1 < 0x61) {
            if (param_1 < 0x60) { if (param_1 == 0x40) { *DAT_02035c30 = 0x83; } goto LAB_02035c14; }
            *DAT_02035c30 = 0x8b;
LAB_02035be0:
            *DAT_02035c34 = 0x83;
            goto LAB_02035c14;
        }
        if (param_1 != 0x70) goto LAB_02035c14;
        *DAT_02035c30 = 0x9b;
LAB_02035bfc:
        *DAT_02035c34 = 0x93;
    }
    *DAT_02035c38 = 0x83;
LAB_02035c14:
    FUN_02036430(*DAT_02035c2c);
    return;
}

/* 0x02035c3c */
void FUN_02035c3c(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    u8 *puVar4; u8 *puVar5; u8 *puVar6;
    puVar3 = DAT_02035e24;
    uVar1 = DAT_02035e24[4];
    uVar2 = *DAT_02035e24;
    DAT_02035e24[4] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 == 0) {
        *DAT_02035e28 = *DAT_02035e28 & (u16)DAT_02035e2c;
        goto switchD_02035c9c_default;
    }
    *DAT_02035e28 = *DAT_02035e28 & 0xcfff | 1;
    puVar6 = DAT_02035e3c;
    puVar5 = DAT_02035e38;
    puVar4 = DAT_02035e34;
    switch(param_1) {
    case 1: goto LAB_02035e00;
    case 2: goto LAB_02035dcc;
    case 3: goto LAB_02035df4;
    case 4: goto LAB_02035da4;
    case 5: *DAT_02035e30 = 0x83; *puVar4 = 0x8b; break;
    case 6: goto LAB_02035dc0;
    case 7: goto LAB_02035de8;
    case 8: *DAT_02035e38 = 0x83; break;
    case 9: *DAT_02035e30 = 0x83; *puVar5 = 0x8b; break;
    case 10: *DAT_02035e3c = 0x83; *puVar5 = 0x8b; break;
    case 0xb: *DAT_02035e30 = 0x83; puVar4 = DAT_02035e38; *puVar6 = 0x8b; *puVar4 = 0x93; break;
    case 0xc: *DAT_02035e38 = 0x8b;
    LAB_02035da4: *DAT_02035e34 = 0x83; break;
    case 0xd: *DAT_02035e30 = 0x83; puVar5 = DAT_02035e38; *puVar4 = 0x8b; *puVar5 = 0x93; break;
    case 0xe: *DAT_02035e38 = 0x93;
    LAB_02035dc0: *DAT_02035e34 = 0x8b;
    LAB_02035dcc: *DAT_02035e3c = 0x83; break;
    case 0xf: *DAT_02035e38 = 0x9b;
    LAB_02035de8: *DAT_02035e34 = 0x93;
    LAB_02035df4: *DAT_02035e3c = 0x8b;
    LAB_02035e00: *DAT_02035e30 = 0x83;
    }
switchD_02035c9c_default:
    FUN_02036430(*DAT_02035e24);
    return;
}

/* 0x02035e40 */
void FUN_02035e40(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    puVar3 = DAT_02035eec;
    uVar1 = DAT_02035eec[8];
    uVar2 = *DAT_02035eec;
    DAT_02035eec[8] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 == 0) {
        _DAT_04000000 = _DAT_04000000 & 0x7fffffff;
    } else if (param_1 == 0x20) {
        _DAT_04000000 = _DAT_04000000 | 0x80000000;
        *DAT_02035ef0 = 0x85;
    } else if (param_1 == 0x40) {
        _DAT_04000000 = _DAT_04000000 | 0x80000000;
        *DAT_02035ef4 = 0x85;
    }
    FUN_02036430(*DAT_02035eec);
    return;
}

/* 0x02035ef8 */
void FUN_02035ef8(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    puVar3 = DAT_02035ffc;
    uVar1 = DAT_02035ffc[7];
    uVar2 = *DAT_02035ffc;
    DAT_02035ffc[7] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    if (param_1 < 0x21) {
        if (param_1 < 0x20) {
            if (param_1 < 1) {
                if (param_1 == 0) { _DAT_04000000 = _DAT_04000000 & 0xbfffffff; }
            } else if (param_1 == 0x10) {
                _DAT_04000000 = _DAT_04000000 | 0x40000000;
                *DAT_02036000 = 0x84;
            }
            goto LAB_02035fe4;
        }
    } else {
        if (param_1 < 0x41) {
            if (param_1 == 0x40) { _DAT_04000000 = _DAT_04000000 | 0x40000000; *DAT_02036004 = 0x8c; }
            goto LAB_02035fe4;
        }
        if (param_1 != 0x60) goto LAB_02035fe4;
        *DAT_02036004 = 0x8c;
    }
    *DAT_02036008 = 0x84;
    _DAT_04000000 = _DAT_04000000 | 0x40000000;
LAB_02035fe4:
    FUN_02036430(*DAT_02035ffc);
    return;
}

/* 0x0203600c */
void FUN_0203600c(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    u8 *puVar4;
    puVar3 = DAT_02036160;
    uVar1 = DAT_02036160[2];
    uVar2 = *DAT_02036160;
    DAT_02036160[2] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    puVar4 = DAT_02036174;
    if (0x30 < param_1) {
        if (param_1 < 0x51) {
            if (param_1 < 0x50) {
                if (param_1 == 0x40) { *DAT_02036164 = 0x82; }
            } else { *DAT_02036164 = 0x92; *puVar4 = 0x82; }
            goto switchD_02036058_default;
        }
        if (param_1 < 0x61) {
            if (param_1 != 0x60) goto switchD_02036058_default;
            *DAT_02036164 = 0x8a;
            goto LAB_0203613c;
        }
        if (param_1 != 0x70) goto switchD_02036058_default;
        *DAT_02036164 = 0x9a;
LAB_020360f8:
        *DAT_02036170 = 0x92;
LAB_02036104:
        *DAT_02036174 = 0x82;
        goto switchD_02036058_default;
    }
    if (0x2f < param_1) goto LAB_020360f8;
    if (0x10 < param_1) { if (param_1 != 0x20) goto switchD_02036058_default;
LAB_0203613c:
        *DAT_02036170 = 0x82;
        goto switchD_02036058_default;
    }
    if (0xf < param_1) goto LAB_02036104;
    switch(param_1) {
    case 0: break;
    case 1: goto LAB_020360cc;
    case 2: *DAT_02036168 = 0x82; break;
    case 3: *DAT_02036168 = 0x8a;
    LAB_020360cc: *DAT_0203616c = 0x82;
    }
switchD_02036058_default:
    FUN_02036430(*DAT_02036160);
    return;
}

/* 0x02036178 */
void FUN_02036178(int param_1)
{
    u16 uVar1; u16 uVar2;
    u16 *puVar3;
    u8 *puVar4; u8 *puVar5; u8 *puVar6;
    puVar3 = DAT_02036410;
    uVar1 = DAT_02036410[1];
    uVar2 = *DAT_02036410;
    DAT_02036410[1] = (u16)param_1;
    *puVar3 = ~(u16)param_1 & (uVar2 | uVar1);
    puVar6 = DAT_0203642c;
    puVar5 = DAT_0203641c;
    puVar4 = DAT_02036414;
    if (0x40 < param_1) {
        if (param_1 < 0x61) {
            if (param_1 < 0x60) {
                if (param_1 == 0x50) { *DAT_02036424 = 0x91; *puVar6 = 0x81; }
                goto switchD_020361bc_default;
            }
            *DAT_02036424 = 0x89;
LAB_020363dc:
            *DAT_02036428 = 0x81;
            goto switchD_020361bc_default;
        }
        if (param_1 != 0x70) goto switchD_020361bc_default;
        *DAT_02036424 = 0x99;
LAB_02036398:
        *DAT_02036428 = 0x91;
LAB_020363a4:
        *DAT_0203642c = 0x81;
        goto switchD_020361bc_default;
    }
    if (0x3f < param_1) { *DAT_02036424 = 0x81; goto switchD_020361bc_default; }
    if (0x20 < param_1) { if (param_1 != 0x30) goto switchD_020361bc_default; goto LAB_02036398; }
    switch(param_1) {
    case 0: break;
    case 1: goto LAB_020362f4;
    case 2: goto LAB_020362c0;
    case 3: goto LAB_020362e8;
    case 4: goto LAB_02036298;
    case 5: goto LAB_02036338;
    case 6: goto LAB_020362b4;
    case 7: goto LAB_020362dc;
    case 8: *DAT_02036414 = 0x81; break;
    case 9: *DAT_02036420 = 0x81; *puVar4 = 0x89; break;
    case 10: *DAT_0203641c = 0x81; *puVar4 = 0x89; break;
    case 0xb: *DAT_02036420 = 0x81; puVar4 = DAT_02036414; *puVar5 = 0x89; *puVar4 = 0x91; break;
    case 0xc: *DAT_02036414 = 0x89;
    LAB_02036298: *DAT_02036418 = 0x81; break;
    case 0xd: *DAT_02036414 = 0x91;
    LAB_02036338: puVar4 = DAT_02036418; *DAT_02036420 = 0x81; *puVar4 = 0x89; break;
    case 0xe: *DAT_02036414 = 0x91;
    LAB_020362b4: *DAT_02036418 = 0x89;
    LAB_020362c0: *DAT_0203641c = 0x81; break;
    case 0xf: *DAT_02036414 = 0x99;
    LAB_020362dc: *DAT_02036418 = 0x91;
    LAB_020362e8: *DAT_0203641c = 0x89;
    LAB_020362f4: *DAT_02036420 = 0x81; break;
    case 0x10: goto LAB_020363a4;
    case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: break;
    case 0x20: goto LAB_020363dc;
    }
switchD_020361bc_default:
    FUN_02036430(*DAT_02036410);
    return;
}

/* 0x02036430 */
void FUN_02036430(u32 param_1)
{
    if ((param_1 & 1) != 0) { *DAT_020364c4 = 0x80; }
    if ((param_1 & 2) != 0) { *DAT_020364c8 = 0x80; }
    if ((param_1 & 4) != 0) { *DAT_020364cc = 0x80; }
    if ((param_1 & 8) != 0) { *DAT_020364d0 = 0x80; }
    if ((param_1 & 0x10) != 0) { *DAT_020364d4 = 0x80; }
    if ((param_1 & 0x20) != 0) { *DAT_020364d8 = 0x80; }
    if ((param_1 & 0x40) != 0) { *DAT_020364dc = 0x80; }
    if ((param_1 & 0x80) != 0) { *DAT_020364e0 = 0x80; }
    if ((param_1 & 0x100) != 0) { *DAT_020364e4 = 0x80; }
    return;
}
