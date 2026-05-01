/**
 * SDK Init D — OS heap, arena, filesystem core (0x02044xxx-0x02047xxx)
 * Decompiled from 0x02044284-0x020470d8 region (23 functions)
 */
#include "types.h"

/* Helper macros for Ghidra intrinsics */
#define CONCAT44(hi, lo)  ((u64)(u32)(hi) << 32 | (u32)(lo))
#define CARRY4(a, b)      ((u32)(a) > (u32)(0xFFFFFFFFu - (u32)(b)))
#define LZCOUNT(x)        ((u32)__builtin_clz(x))

/* External function declarations */
extern int  FUN_02039dd4(...);
extern void FUN_02039e98(...);
extern void FUN_02039f08(...);
extern void FUN_020485c0(...);
extern void FUN_02044644(...);

/* External data declarations */
extern u32  DAT_02044348;
extern u32  DAT_02044464;
extern u32  DAT_02044468;
extern u32 *DAT_0204446c;
extern int *DAT_02044470;
extern int *DAT_02044474;
extern u32  DAT_02044478;
extern u32 *DAT_0204447c;
extern u32 *DAT_020444cc;
extern u32 *DAT_020444d0;
extern u32 *DAT_02044500;
extern u32  DAT_02044570;
extern u32  DAT_02044884;
extern u32  DAT_02044888;
extern u32  DAT_0204488c;
extern u32  DAT_02044890;
extern u32  DAT_02044894;
extern u32  DAT_02044bac;
extern u32  DAT_020453a4;
extern u32  DAT_02044fd8;
extern u32  DAT_02046ee8;
extern u32  DAT_02046eec;

/* Forward declarations */
u32  FUN_02044284(u16 *param_1, u32 param_2);
void FUN_0204434c(void);
void FUN_02044480(u32 param_1);
void FUN_020444d4(void);
u32  FUN_02044504(void);
int  FUN_02044574(u32 *param_1, u32 *param_2);
void FUN_02044610(int param_1);
u32  FUN_02044648(int param_1);
u32  FUN_02044748(int param_1);
u64  FUN_02044898(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u64  FUN_02044c78(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_020466c4(u32 param_1, u32 param_2);
void FUN_02046e1c(u32 param_1, u32 param_2, void (*callback)(void));
void FUN_02046e98(u8 *param_1);
u32  FUN_02046ed0(int param_1);
void FUN_02046ef0(int param_1, int param_2);
u32  FUN_02046f98(int param_1);
u64  FUN_02046ffc(u32 param_1);
u64  FUN_02047010(void);
u64  FUN_02047024(void);
u32  FUN_0204703c(u32 param_1, int param_2, u32 *param_3);
u32  FUN_02047074(u32 param_1, int param_2, u32 *param_3);
u8  *FUN_020470d8(u8 *param_1, u32 *param_2);

// ============================================================
// FUN_02044284 @ 0x02044284 (196 bytes) — Internet checksum
// ============================================================
u32 FUN_02044284(u16 *param_1, u32 param_2)
{
    u16 *ptr;
    u32 sum;
    u32 blocks;
    u32 words;
    int count;
    BOOL odd_start;

    sum = 0;
    odd_start = ((u32)param_1 & 1) != 0;
    ptr = param_1;

    if (odd_start) {
        ptr = (u16 *)((u8 *)param_1 + 1);
        param_2 = param_2 - 1;
        sum = (u32)(*(u8 *)param_1) * 0x100;
    }

    blocks = param_2 >> 17;
    while (blocks != 0) {
        count = 0x10000;
        blocks = blocks - 1;
        param_2 = param_2 - 0x20000;
        do {
            sum = sum + *ptr;
            ptr = ptr + 1;
            count = count - 1;
        } while (count != 0);
        sum = (sum & DAT_02044348) + (sum >> 16);
        sum = (sum + (sum >> 16)) & 0xFFFF;
    }

    for (words = param_2 >> 1; words != 0; words = words - 1) {
        sum = sum + *ptr;
        ptr = ptr + 1;
    }

    if ((param_2 & 1) != 0) {
        sum = sum + (u8)*ptr;
    }

    sum = (sum & DAT_02044348) + (sum >> 16);
    sum = (sum & DAT_02044348) + (sum >> 16);

    if (odd_start) {
        sum = (sum * 0x1000000 | sum * 0x100) >> 16;
    }

    return ~sum & 0xFFFF;
}

// ============================================================
// FUN_0204434c @ 0x0204434C (280 bytes) — VBlank interrupt handler
// ============================================================
void FUN_0204434c(void)
{
    int *piVar1;
    int iVar2;
    int iVar3;
    void (*pcVar4)(void);

    iVar2 = FUN_02039dd4(DAT_02044464);
    piVar1 = DAT_02044470;

    if (iVar2 == 0) {
        *DAT_0204446c = *(int *)(*(int *)(DAT_02044468 + 8) + 0x6C);
        *piVar1 = 1;
    }
    else if (*DAT_0204446c == *(int *)(*(int *)(DAT_02044468 + 8) + 0x6C)) {
        *DAT_02044470 = *DAT_02044470 + 1;
    }
    else {
        FUN_02039f08(DAT_02044464);
        piVar1 = DAT_02044470;
        *DAT_0204446c = *(int *)(*(int *)(DAT_02044468 + 8) + 0x6C);
        *piVar1 = 1;
    }

    iVar2 = DAT_02044478;
    piVar1 = DAT_02044474;
    iVar3 = *DAT_02044474;

    while (0 < iVar3) {
        pcVar4 = *(void (**)(void))(iVar2 + (*piVar1 + -1) * 4);
        *piVar1 = *piVar1 + -1;
        pcVar4();
        iVar3 = *piVar1;
    }

    iVar2 = *DAT_02044470;
    *DAT_02044470 = iVar2 + -1;

    if (iVar2 + -1 == 0) {
        FUN_02039e98(DAT_02044464);
    }

    if ((void (*)(void))*DAT_0204447c != (void (*)(void))0) {
        (*(void (**)(void))DAT_0204447c)();
        *DAT_0204447c = 0;
    }

    FUN_02044648(0);
    FUN_02047024();
}

// ============================================================
// FUN_02044480 @ 0x02044480 (76 bytes) — Conditional VBlank handler
// ============================================================
void FUN_02044480(u32 param_1)
{
    if (*DAT_020444cc == 0) {
        FUN_020485c0();
        if ((void (*)(void))*DAT_020444d0 != (void (*)(void))0) {
            (*(void (**)(void))DAT_020444d0)();
            *DAT_020444d0 = 0;
        }
    }
    FUN_0204434c();
}

// ============================================================
// FUN_020444d4 @ 0x020444D4 (44 bytes) — Init with power-on
// ============================================================
void FUN_020444d4(void)
{
    FUN_02044748(1);
    *DAT_02044500 = 1;
    FUN_02044480(0);
}

// ============================================================
// FUN_02044504 @ 0x02044504 (108 bytes) — Flush all streams
// ============================================================
u32 FUN_02044504(void)
{
    int iVar1;
    int iVar2;
    int iVar3;
    u32 result;

    iVar1 = DAT_02044570;
    result = 0;
    iVar3 = 1;
    iVar2 = DAT_02044570;

    do {
        if (((u32)(*(int *)(iVar2 + 4) << 0x16) >> 0x1D != 0) &&
            (iVar2 = FUN_02044648(iVar2), iVar2 != 0))
        {
            result = 0xFFFFFFFF;
        }
        if (iVar3 < 3) {
            iVar2 = iVar1 + iVar3 * 0x4C;
            iVar3 = iVar3 + 1;
        }
        else {
            iVar2 = 0;
        }
    } while (iVar2 != 0);

    return result;
}

// ============================================================
// FUN_02044574 @ 0x02044574 (156 bytes) — Stream write/flush
// ============================================================
int FUN_02044574(u32 *param_1, u32 *param_2)
{
    int iVar1;

    if (param_1[9] - param_1[7] != 0) {
        param_1[10] = param_1[9] - param_1[7];

        if (-1 < (int)(param_1[1] << 0x13)) {
            FUN_02044644(param_1[7], &param_1[10]);
        }

        iVar1 = (*(int (*)(u32, u32, u32 *, u32))param_1[0x10])(
                    *param_1, param_1[7], &param_1[10], param_1[0x12]);

        if (param_2 != (u32 *)0) {
            *param_2 = param_1[10];
        }

        if (iVar1 != 0) {
            return iVar1;
        }

        param_1[6] = param_1[6] + param_1[10];
    }

    FUN_02044610((int)param_1);
    return 0;
}

// ============================================================
// FUN_02044610 @ 0x02044610 (52 bytes) — Reset stream buffer pointers
// ============================================================
void FUN_02044610(int param_1)
{
    *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x1C);
    *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x20);
    *(u32 *)(param_1 + 0x28) =
        *(int *)(param_1 + 0x28) - (*(u32 *)(param_1 + 0x18) & *(u32 *)(param_1 + 0x2C));
    *(u32 *)(param_1 + 0x34) = *(u32 *)(param_1 + 0x18);
}

// ============================================================
// FUN_02044648 @ 0x02044648 (256 bytes) — Flush stream
// ============================================================
u32 FUN_02044648(int param_1)
{
    u32 uVar1;
    int iVar2;

    if (param_1 == 0) {
        uVar1 = FUN_02044504();
        return uVar1;
    }

    if ((*(char *)(param_1 + 0xD) != '\0') ||
        ((u32)(*(int *)(param_1 + 4) << 0x16) >> 0x1D == 0))
    {
        return 0xFFFFFFFF;
    }

    if ((u32)(*(int *)(param_1 + 4) << 0x1B) >> 0x1D == 1) {
        return 0;
    }

    if (2 < (*(u32 *)(param_1 + 8) & 7)) {
        *(u32 *)(param_1 + 8) = (*(u32 *)(param_1 + 8) & 0xFFFFFFF8) | 2;
    }

    if ((*(u32 *)(param_1 + 8) & 7) == 2) {
        *(u32 *)(param_1 + 0x28) = 0;
    }

    if ((*(u32 *)(param_1 + 8) & 7) == 1) {
        iVar2 = FUN_02044574((u32 *)param_1, 0);
        if (iVar2 == 0) {
            *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 8) & 0xFFFFFFF8;
            *(u32 *)(param_1 + 0x18) = 0;
            *(u32 *)(param_1 + 0x28) = 0;
            return 0;
        }
        *(u8 *)(param_1 + 0xD) = 1;
        *(u32 *)(param_1 + 0x28) = 0;
        return 0xFFFFFFFF;
    }

    *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 8) & 0xFFFFFFF8;
    return 0;
}

// ============================================================
// FUN_02044748 @ 0x02044748 (316 bytes) — Signal handler dispatch
// ============================================================
u32 FUN_02044748(int param_1)
{
    int iVar1;
    int iVar2;
    void (*pcVar3)(int);

    if ((param_1 < 1) || (7 < param_1)) {
        return 0xFFFFFFFF;
    }

    iVar1 = FUN_02039dd4(DAT_02044884);
    iVar2 = DAT_02044890;

    if (iVar1 == 0) {
        *(u32 *)(DAT_0204488c + 0x1C) = *(u32 *)(*(int *)(DAT_02044888 + 8) + 0x6C);
        *(u32 *)(iVar2 + 0x1C) = 1;
    }
    else if (*(int *)(DAT_0204488c + 0x1C) == *(int *)(*(int *)(DAT_02044888 + 8) + 0x6C)) {
        *(int *)(DAT_02044890 + 0x1C) = *(int *)(DAT_02044890 + 0x1C) + 1;
    }
    else {
        FUN_02039f08(DAT_02044884);
        iVar2 = DAT_02044890;
        *(u32 *)(DAT_0204488c + 0x1C) = *(u32 *)(*(int *)(DAT_02044888 + 8) + 0x6C);
        *(u32 *)(iVar2 + 0x1C) = 1;
    }

    pcVar3 = *(void (**)(int))(DAT_02044894 + (param_1 + -1) * 4);

    if (pcVar3 != (void (*)(int))1) {
        *(u32 *)(DAT_02044894 + (param_1 + -1) * 4) = 0;
    }

    iVar2 = *(int *)(DAT_02044890 + 0x1C) + -1;
    *(int *)(DAT_02044890 + 0x1C) = iVar2;

    if (iVar2 == 0) {
        FUN_02039e98(DAT_02044884);
    }

    if ((pcVar3 != (void (*)(int))1) &&
        ((pcVar3 != (void (*)(int))0) || (param_1 != 1)))
    {
        if (pcVar3 == (void (*)(int))0) {
            FUN_02044480(0);
        }
        pcVar3(param_1);
        return 0;
    }

    return 0;
}

// ============================================================
// FUN_02044898 @ 0x02044898 (1700 bytes) — double-precision addition
// Compiler runtime soft-float __adddf3
// ============================================================
u64 FUN_02044898(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;
    u32 uVar8;
    int iVar9;
    BOOL bVar10;

    if ((int)(param_2 ^ param_4) < 0) {
        param_4 = param_4 ^ 0x80000000;
        uVar7 = param_1 - param_3;
        if (param_2 <= param_4 && (u32)(param_3 <= param_1) <= param_2 - param_4) {
            uVar8 = param_2 - (param_4 + (param_3 > param_1)) ^ 0x80000000;
            bVar10 = CARRY4(param_3, uVar7);
            param_3 = param_3 + uVar7;
            param_4 = param_4 + uVar8 + (u32)bVar10;
            bVar10 = param_1 < uVar7;
            param_1 = param_1 - uVar7;
            param_2 = param_2 - (uVar8 + bVar10);
        }
        uVar8 = param_2 >> 0x14;
        param_2 = param_2 << 0xB;
        uVar6 = param_1 >> 0x15;
        uVar3 = param_2 | 0x80000000 | uVar6;
        uVar7 = param_1 * 0x800;
        if (uVar8 * 0x200000 == 0 || uVar8 * 0x200000 == (u32)-0x200000) {
            uVar2 = 0x80000000;
            if (uVar8 < 0x800) {
                uVar2 = 0;
            }
            if ((uVar8 & 0xFFFFF7FF) != 0) {
                if (uVar7 != 0 || ((param_2 & 0x7FFFFFFF) != 0 || uVar6 != 0)) {
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
                iVar9 = (param_4 >> 0x14) * 0x200000;
                if ((iVar9 != 0) && (iVar9 == (int)-0x200000)) {
                    if ((param_3 & 0x1FFFFF) != 0 || ((param_4 & 0xFFFFF) != 0 || param_3 >> 0x15 != 0)) {
                        return 0x7FFFFFFFFFFFFFFFULL;
                    }
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
                goto LAB_0204535c;
            }
            if (uVar7 == 0 && ((param_2 & 0x7FFFFFFF) == 0 && uVar6 == 0)) {
                iVar9 = (param_4 >> 0x14) * 0x200000;
                if (iVar9 == 0) {
                    if (param_3 << 0xB == 0 && ((param_4 << 0xB & 0x7FFFFFFF) == 0 && param_3 >> 0x15 == 0)) {
                        return 0;
                    }
                }
                else if (iVar9 == (int)-0x200000) {
                    if (param_3 << 0xB != 0 || ((param_4 << 0xB & 0x7FFFFFFF) != 0 || param_3 >> 0x15 != 0)) {
                        return 0x7FFFFFFFFFFFFFFFULL;
                    }
                    goto LAB_0204535c;
                }
                return CONCAT44(param_4, param_3);
            }
            uVar8 = 1;
            uVar3 = param_2 & 0x7FFFFFFF | uVar6;
            uVar4 = param_4 << 0xB | param_3 >> 0x15;
            iVar9 = (param_4 >> 0x14) * 0x200000;
            uVar6 = param_4 >> 0x14 & 0x7FF | uVar2 >> 0x14;
            if (iVar9 == 0 || iVar9 == (int)-0x200000) goto LAB_02045270;
            uVar4 = uVar4 | 0x80000000;
            uVar8 = uVar2 >> 0x14 | 1;
        }
        else {
            uVar6 = param_4 >> 0x14;
            uVar4 = param_4 << 0xB | 0x80000000 | param_3 >> 0x15;
            if ((uVar6 & 0x7FF) == 0) {
LAB_02045270:
                if (uVar6 < 0x800) {
                    uVar2 = 0;
                }
                else {
                    uVar2 = 0x80000000;
                }
                uVar8 = uVar8 & 0xFFFFF7FF;
                if ((uVar6 & 0xFFFFF7FF) != 0) {
                    if (param_3 << 0xB != 0 || (uVar4 & 0x7FFFFFFF) != 0) {
                        return 0x7FFFFFFFFFFFFFFFULL;
                    }
LAB_0204535c:
                    return (u64)(uVar2 | DAT_020453a4) << 32;
                }
                if (param_3 << 0xB == 0 && (uVar4 & 0x7FFFFFFF) == 0) {
                    if (-1 < (int)uVar3) {
                        uVar8 = uVar8 - 1;
                    }
                    return CONCAT44(uVar2 | uVar3 * 2 >> 0xC | uVar8 << 0x14,
                                    param_1 & 0x1FFFFF | uVar3 << 0x15);
                }
                uVar4 = uVar4 & 0x7FFFFFFF;
                uVar8 = uVar8 | uVar2 >> 0x14;
                uVar6 = uVar2 >> 0x14 | 1;
            }
        }
        uVar2 = param_3 * 0x800;
        uVar6 = uVar8 - uVar6;
        if (uVar6 == 0) {
            uVar6 = uVar7 + param_3 * (u32)-0x800;
            uVar3 = uVar3 - (uVar4 + (uVar7 < uVar2));
            if (uVar3 == 0 && uVar6 == 0) {
                return 0;
            }
            uVar7 = (uVar8 & 0x800) << 0x14;
            uVar8 = uVar8 & 0xFFFFF7FF;
            if ((int)uVar3 < 0) {
                if (-1 < (int)uVar3) {
                    uVar8 = uVar8 - 1;
                }
                goto LAB_02045194;
            }
            uVar2 = uVar6;
            if (uVar3 == 0) {
                uVar8 = uVar8 - 0x20;
                uVar2 = 0;
                uVar3 = uVar6;
                if (-1 < (int)uVar6) goto LAB_0204513c;
            }
            else {
LAB_0204513c:
                uVar6 = 0x20 - LZCOUNT(uVar3);
                uVar3 = uVar3 << LZCOUNT(uVar3) | uVar2 >> (uVar6 & 0xFF);
                uVar6 = 0x20 - uVar6;
                uVar2 = uVar2 << (uVar6 & 0xFF);
                uVar8 = uVar8 - uVar6;
            }
            uVar6 = uVar2;
            if ((int)uVar8 < 1) {
                uVar4 = (u32)(-(int)uVar8) + 0xC;
                uVar6 = uVar3;
                if (0x1F < (int)uVar4) {
                    uVar6 = 0;
                    uVar4 = (u32)(-(int)uVar8) - 0x14;
                    uVar2 = uVar3;
                }
                return CONCAT44(uVar7 | uVar6 >> (uVar4 & 0xFF),
                                uVar2 >> (uVar4 & 0xFF) | uVar6 << (0x20 - uVar4 & 0xFF));
            }
LAB_02045194:
            return CONCAT44(uVar7 | uVar3 * 2 >> 0xC | uVar8 << 0x14, uVar6 >> 0xB | uVar3 << 0x15);
        }
        if ((int)uVar6 < 0x21) {
            uVar5 = uVar2 >> (uVar6 & 0xFF) | uVar4 << (0x20 - uVar6 & 0xFF);
            if (uVar2 << (0x20 - uVar6 & 0xFF) != 0) {
                uVar5 = uVar5 | 1;
            }
            uVar6 = (uVar4 >> (uVar6 & 0xFF)) + (u32)(uVar7 < uVar5);
        }
        else {
            if (0x37 < (int)uVar6) {
                uVar6 = 0x3F;
            }
            uVar5 = uVar4 >> (uVar6 - 0x20 & 0xFF);
            if (uVar2 != 0 || uVar4 << (0x20 - (uVar6 - 0x20) & 0xFF) != 0) {
                uVar5 = uVar5 | 1;
            }
            uVar6 = (u32)(uVar7 < uVar5);
        }
        uVar3 = uVar3 - uVar6;
        uVar7 = uVar7 - uVar5;
        if ((int)uVar3 < 0) goto LAB_020450c0;
        uVar2 = uVar8 & 0xFFFFF7FF;
        uVar6 = uVar7;
        if (uVar3 == 0) {
            uVar2 = uVar2 - 0x20;
            uVar6 = 0;
            uVar3 = uVar7;
            if (-1 < (int)uVar7) goto LAB_020451d4;
        }
        else {
LAB_020451d4:
            uVar7 = 0x20 - LZCOUNT(uVar3);
            uVar3 = uVar3 << LZCOUNT(uVar3) | uVar6 >> (uVar7 & 0xFF);
            uVar7 = 0x20 - uVar7;
            uVar2 = uVar2 - uVar7;
            uVar6 = uVar6 << (uVar7 & 0xFF);
        }
        uVar7 = uVar6;
        if ((int)uVar2 < 1) {
            uVar4 = (u32)(-(int)uVar2) + 0xC;
            uVar6 = uVar3;
            if (0x1F < (int)uVar4) {
                uVar6 = 0;
                uVar4 = (u32)(-(int)uVar2) - 0x14;
                uVar7 = uVar3;
            }
            return CONCAT44((uVar8 & 0x800) << 0x14 | uVar6 >> (uVar4 & 0xFF),
                            uVar7 >> (uVar4 & 0xFF) | uVar6 << (0x20 - uVar4 & 0xFF));
        }
        uVar8 = uVar2 | uVar8 & 0x800;
LAB_020450c0:
        uVar6 = uVar7 >> 0xB | uVar3 << 0x15;
        uVar8 = uVar3 * 2 >> 0xC | uVar8 << 0x14;
        if ((uVar7 & 0x400) == 0) {
            return CONCAT44(uVar8, uVar6);
        }
        if ((uVar7 & 0x3FF) != 0 || (uVar7 >> 0xB & 1) != 0) {
            return CONCAT44(uVar8 + (0xFFFFFFFE < uVar6), uVar6 + 1);
        }
        return CONCAT44(uVar8, uVar6);
    }

    /* Same-sign addition path */
    uVar7 = param_1 - param_3;
    iVar9 = param_2 - (param_4 + (param_3 > param_1));
    if (param_2 <= param_4 && (u32)(param_3 <= param_1) <= param_2 - param_4) {
        bVar10 = CARRY4(param_3, uVar7);
        param_3 = param_3 + uVar7;
        param_4 = param_4 + iVar9 + (u32)bVar10;
        bVar10 = param_1 < uVar7;
        param_1 = param_1 - uVar7;
        param_2 = param_2 - (iVar9 + (u32)bVar10);
    }
    uVar8 = param_2 >> 0x14;
    param_2 = param_2 << 0xB;
    uVar6 = param_1 >> 0x15;
    uVar3 = param_2 | 0x80000000 | uVar6;
    uVar7 = param_1 * 0x800;
    if (uVar8 * 0x200000 == 0 || uVar8 * 0x200000 == (u32)-0x200000) {
        uVar2 = 0x80000000;
        if (uVar8 < 0x800) {
            uVar2 = 0;
        }
        if ((uVar8 & 0xFFFFF7FF) != 0) {
            if (uVar7 != 0 || ((param_2 & 0x7FFFFFFF) != 0 || uVar6 != 0)) {
                return 0x7FFFFFFFFFFFFFFFULL;
            }
            iVar9 = (param_4 >> 0x14) * 0x200000;
            if (((iVar9 != 0) && (iVar9 == (int)-0x200000)) &&
                ((param_3 & 0x1FFFFF) != 0 || ((param_4 & 0xFFFFF) != 0 || param_3 >> 0x15 != 0)))
            {
                return 0x7FFFFFFFFFFFFFFFULL;
            }
            goto LAB_02044b54;
        }
        if (uVar7 == 0 && ((param_2 & 0x7FFFFFFF) == 0 && uVar6 == 0)) {
            iVar9 = (param_4 >> 0x14) * 0x200000;
            if ((iVar9 != 0) && (iVar9 == (int)-0x200000)) {
                if ((param_3 & 0x1FFFFF) != 0 || ((param_4 & 0xFFFFF) != 0 || param_3 >> 0x15 != 0)) {
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
                goto LAB_02044b54;
            }
            goto LAB_02044b20;
        }
        uVar8 = 1;
        uVar3 = param_2 & 0x7FFFFFFF | uVar6;
        uVar6 = param_4 >> 0x14;
        param_4 = param_4 << 0xB | param_3 >> 0x15;
        iVar9 = uVar6 * 0x200000;
        uVar6 = uVar6 & 0x7FF | uVar2 >> 0x14;
        if (iVar9 == 0 || iVar9 == (int)-0x200000) goto LAB_02044a08;
        param_4 = param_4 | 0x80000000;
        uVar8 = uVar2 >> 0x14 | 1;
    }
    else {
        uVar6 = param_4 >> 0x14;
        param_4 = param_4 << 0xB | 0x80000000 | param_3 >> 0x15;
        if ((uVar6 & 0x7FF) == 0) {
            u32 uVar4_local;
LAB_02044a08:
            uVar4_local = param_3 * 0x800;
            if (uVar6 < 0x800) {
                uVar2 = 0;
            }
            else {
                uVar2 = 0x80000000;
            }
            uVar8 = uVar8 & 0xFFFFF7FF;
            if ((uVar6 & 0xFFFFF7FF) != 0) {
                if (uVar4_local != 0 || (param_4 & 0x7FFFFFFF) != 0) {
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
LAB_02044b54:
                return (u64)(uVar2 | DAT_02044bac) << 32;
            }
            if (uVar4_local == 0 && (param_4 & 0x7FFFFFFF) == 0) {
                if (-1 < (int)uVar3) {
                    uVar8 = uVar8 - 1;
                }
                return CONCAT44(uVar2 | uVar3 * 2 >> 0xC | uVar8 << 0x14,
                                param_1 & 0x1FFFFF | uVar3 << 0x15);
            }
            param_4 = param_4 & 0x7FFFFFFF;
            if (-1 < (int)uVar3) {
                u8 bVar1;
                uVar6 = uVar7 + uVar4_local;
                bVar10 = CARRY4(uVar3 + param_4, (u32)CARRY4(uVar7, uVar4_local));
                uVar7 = uVar3 + param_4 + (u32)CARRY4(uVar7, uVar4_local);
                if (CARRY4(uVar3, param_4) || bVar10) {
                    uVar8 = uVar8 + 1;
                    bVar1 = (u8)uVar7;
                    uVar7 = (u32)(CARRY4(uVar3, param_4) || bVar10) << 0x1F | uVar7 >> 1;
                    uVar6 = (u32)(bVar1 & 1) << 0x1F | uVar6 >> 1;
                }
                if (-1 < (int)uVar7) {
                    uVar8 = uVar8 - 1;
                }
                uVar3 = uVar6 >> 0xB | uVar7 << 0x15;
                uVar7 = uVar2 | uVar7 * 2 >> 0xC | uVar8 << 0x14;
                if (uVar6 << 0x15 == 0) {
                    return CONCAT44(uVar7, uVar3);
                }
                if (uVar6 << 0x15 == 0) {
                    return CONCAT44(uVar7, uVar3);
                }
                param_3 = uVar6 >> 0xB & 1;
                if (param_3 == 0) {
                    return CONCAT44(uVar7, uVar3);
                }
LAB_02044b20:
                return CONCAT44(param_4, param_3);
            }
            uVar8 = uVar8 | uVar2 >> 0x14;
            uVar6 = uVar2 >> 0x14 | 1;
        }
    }
    param_3 = param_3 << 0xB;
    uVar6 = uVar8 - uVar6;
    if (uVar6 == 0) {
LAB_02044954:
        uVar2 = uVar7 + param_3;
        uVar6 = uVar3 + param_4 + (u32)CARRY4(uVar7, param_3);
        if (!CARRY4(uVar3, param_4) && !CARRY4(uVar3 + param_4, (u32)CARRY4(uVar7, param_3)))
            goto LAB_0204497c;
    }
    else {
        if ((int)uVar6 < 0x21) {
            iVar9 = param_3 << (0x20 - uVar6 & 0xFF);
            param_3 = param_3 >> (uVar6 & 0xFF) | param_4 << (0x20 - uVar6 & 0xFF);
            param_4 = param_4 >> (uVar6 & 0xFF);
            if (iVar9 != 0) {
                param_3 = param_3 | 1;
            }
            goto LAB_02044954;
        }
        if (0x37 < (int)uVar6) {
            uVar6 = 0x3F;
        }
        uVar4 = param_4 >> (uVar6 - 0x20 & 0xFF);
        if (param_3 != 0 || param_4 << (0x20 - (uVar6 - 0x20) & 0xFF) != 0) {
            uVar4 = uVar4 | 1;
        }
        uVar2 = uVar7 + uVar4;
        uVar6 = uVar3 + CARRY4(uVar7, uVar4);
        if (!CARRY4(uVar3, (u32)CARRY4(uVar7, uVar4))) goto LAB_0204497c;
    }
    {
        u8 bVar1;
        uVar8 = uVar8 + 1;
        bVar1 = (u8)uVar6;
        uVar6 = uVar6 >> 1;
        uVar2 = (uVar2 & 1) | (u32)(bVar1 & 1) << 0x1F | uVar2 >> 1;
        if ((uVar8 & 0x7FF) == 0x7FF) {
            if (uVar8 < 0x800) {
                uVar7 = 0;
            }
            else {
                uVar7 = 0x80000000;
            }
            return (u64)(uVar7 | DAT_02044bac) << 32;
        }
    }
LAB_0204497c:
    uVar7 = uVar2 >> 0xB | uVar6 << 0x15;
    uVar8 = uVar6 * 2 >> 0xC | uVar8 << 0x14;
    if ((uVar2 & 0x400) == 0) {
        return CONCAT44(uVar8, uVar7);
    }
    if ((uVar2 & 0x3FF) != 0 || (uVar2 >> 0xB & 1) != 0) {
        return CONCAT44(uVar8 + (0xFFFFFFFE < uVar7), uVar7 + 1);
    }
    return CONCAT44(uVar8, uVar7);
}

// ============================================================
// FUN_02044c78 @ 0x02044C78 (864 bytes) — double-precision multiply
// Compiler runtime soft-float __muldf3
// ============================================================
u64 FUN_02044c78(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    s64 lVar1;
    s64 lVar2;
    s64 lVar3;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;
    u32 uVar8;
    u32 uVar9;
    int iVar10;
    int iVar11;
    u32 uVar12;
    u32 uVar13;
    BOOL bVar14;

    uVar12 = param_2 ^ param_4;
    uVar13 = uVar12 & 0x80000000;
    uVar9 = param_2 >> 0x14;
    param_2 = param_2 << 0xB;
    uVar4 = param_1 >> 0x15;
    uVar5 = param_2 | uVar4;
    param_1 = param_1 << 0xB;
    uVar6 = param_1;

    if (uVar9 * 0x200000 == 0 || uVar9 * 0x200000 == (u32)-0x200000) {
        if ((uVar9 & 0xFFFFF7FF) != 0) {
            if (param_1 != 0 || ((param_2 & 0x7FFFFFFF) != 0 || uVar4 != 0)) {
                return 0x7FFFFFFFFFFFFFFFULL;
            }
            iVar10 = (param_4 >> 0x14) * 0x200000;
            if (iVar10 == 0) {
                if ((param_4 << 0xB == 0 && param_3 >> 0x15 == 0) && param_3 << 0xB == 0) {
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
            }
            else if ((iVar10 == (int)-0x200000) &&
                     (param_3 << 0xB != 0 || ((param_4 << 0xB & 0x7FFFFFFF) != 0 || param_3 >> 0x15 != 0)))
            {
                return 0x7FFFFFFFFFFFFFFFULL;
            }
            goto LAB_02044f54;
        }
        if (param_1 == 0 && ((param_2 & 0x7FFFFFFF) == 0 && uVar4 == 0)) {
            iVar10 = (param_4 >> 0x14) * 0x200000;
            if ((iVar10 != 0) && (iVar10 == (int)-0x200000)) {
                if ((param_3 & 0x1FFFFF) != 0 || ((param_4 & 0xFFFFF) != 0 || param_3 >> 0x15 != 0)) {
                    return 0x7FFFFFFFFFFFFFFFULL;
                }
                return 0x7FFFFFFFFFFFFFFFULL;
            }
            goto LAB_02044fc8;
        }
        uVar9 = 1;
        if (uVar5 == 0) {
            uVar9 = 0xFFFFFFE1;
            uVar6 = 0;
            uVar5 = param_1;
            if (-1 < (int)param_1) goto LAB_02044e00;
        }
        else {
LAB_02044e00:
            uVar4 = 0x20 - LZCOUNT(uVar5);
            param_1 = uVar5 << LZCOUNT(uVar5) | uVar6 >> (uVar4 & 0xFF);
            uVar4 = 0x20 - uVar4;
            uVar6 = uVar6 << (uVar4 & 0xFF);
            uVar9 = uVar9 - uVar4;
        }
        uVar7 = param_4 << 0xB | param_3 >> 0x15;
        uVar4 = param_3 << 0xB;
        iVar10 = (param_4 >> 0x14) * 0x200000;
        if (iVar10 != 0 && iVar10 != (int)-0x200000) {
            param_3 = uVar7 | 0x80000000;
            uVar8 = param_4 >> 0x14 & 0xFFFFF7FF;
            goto LAB_02044ccc;
        }
    }
    else {
        uVar9 = uVar9 & 0xFFFFF7FF;
        uVar7 = param_4 << 0xB | param_3 >> 0x15;
        uVar4 = param_3 << 0xB;
        iVar10 = (param_4 >> 0x14) * 0x200000;
        param_1 = uVar5 | 0x80000000;
        if (iVar10 != 0 && iVar10 != (int)-0x200000) {
            param_3 = uVar7 | 0x80000000;
            uVar8 = param_4 >> 0x14 & 0xFFFFF7FF;
            goto LAB_02044ccc;
        }
    }

    param_3 = param_3 << 0xB;
    if ((param_4 >> 0x14 & 0xFFFFF7FF) != 0) {
        if (param_3 != 0 || (uVar7 & 0x7FFFFFFF) != 0) {
            return 0x7FFFFFFFFFFFFFFFULL;
        }
LAB_02044f54:
        return (u64)(uVar13 | DAT_02044fd8) << 32;
    }
    if (param_3 == 0 && (uVar7 & 0x7FFFFFFF) == 0) {
LAB_02044fc8:
        return ((u64)uVar12 & 0x80000000) << 32;
    }
    uVar8 = 1;
    uVar4 = param_3;
    if (uVar7 == 0) {
        uVar8 = 0xFFFFFFE1;
        uVar4 = 0;
        uVar7 = param_3;
        if ((int)param_3 < 0) goto LAB_02044ccc;
    }
    uVar5 = 0x20 - LZCOUNT(uVar7);
    param_3 = uVar7 << LZCOUNT(uVar7) | uVar4 >> (uVar5 & 0xFF);
    uVar5 = 0x20 - uVar5;
    uVar4 = uVar4 << (uVar5 & 0xFF);
    uVar8 = uVar8 - uVar5;

LAB_02044ccc:
    iVar10 = uVar8 + uVar9;
    lVar2 = (u64)param_3 * (u64)uVar6 + ((u64)uVar4 * (u64)uVar6 >> 32);
    uVar5 = (u32)((u64)lVar2 >> 32);
    lVar1 = (u64)uVar4 * (u64)param_1;
    uVar9 = (u32)((u64)lVar1 >> 32);

    {
        u32 carry_val = (u32)(CARRY4(uVar9, uVar5) ||
                     CARRY4(uVar9 + uVar5, (u32)CARRY4((u32)lVar1, (u32)lVar2)));
        u32 sum_hi = (u32)((u64)(lVar2 + lVar1) >> 32);
        u32 combined = (carry_val << 31) | (sum_hi >> 1);
        /* Simplified: use full 64-bit product chain */
        lVar3 = (u64)param_3 * (u64)param_1 +
                (u64)((carry_val) | sum_hi);
    }

    uVar9 = (u32)lVar3;
    uVar5 = (u32)((u64)lVar3 >> 32);
    if ((int)(lVar2 + lVar1) != 0 || (int)((u64)uVar4 * (u64)uVar6) != 0) {
        uVar9 = uVar9 | 1;
    }
    if (-1 < lVar3) {
        iVar10 = iVar10 + -1;
        bVar14 = CARRY4(uVar9, uVar9);
        uVar9 = uVar9 * 2;
        uVar5 = uVar5 * 2 + (u32)bVar14;
    }
    iVar11 = iVar10 + (int)-0x3FE;
    if ((iVar11 < 0) || (iVar11 == 0)) {
        if (iVar11 == -0x34) {
            return CONCAT44(uVar12, (u32)(uVar9 != 0 || (uVar5 & 0x7FFFFFFF) != 0)) & 0x80000000FFFFFFFFULL;
        }
        if (iVar10 + (int)-0x3CA < 0) {
            return ((u64)uVar12 & 0x80000000) << 32;
        }
        uVar12 = iVar10 - 0x3CA;
        uVar4 = uVar9;
        uVar6 = uVar5;
        if (0x1F < (int)uVar12) {
            uVar4 = 0;
            uVar12 = iVar10 - 0x3EA;
            uVar6 = uVar9;
        }
        uVar6 = uVar6 << (uVar12 & 0xFF) | uVar4 >> (0x20 - uVar12 & 0xFF);
        if (uVar4 << (uVar12 & 0xFF) != 0) {
            uVar6 = uVar6 | 1;
        }
        uVar12 = (u32)(-iVar11) + 0xC;
        uVar4 = uVar5;
        if (0x1F < (int)uVar12) {
            uVar4 = 0;
            uVar12 = (u32)(-iVar11) - 0x14;
            uVar9 = uVar5;
        }
        uVar9 = uVar9 >> (uVar12 & 0xFF) | uVar4 << (0x20 - uVar12 & 0xFF);
        uVar13 = uVar13 | uVar4 >> (uVar12 & 0xFF);
        if (uVar6 == 0) {
            return CONCAT44(uVar13, uVar9);
        }
        if ((uVar6 & 0x80000000) == 0) {
            return CONCAT44(uVar13, uVar9);
        }
        if ((uVar6 & 0x7FFFFFFF) == 0 && (uVar9 & 1) == 0) {
            return CONCAT44(uVar13, uVar9);
        }
        return CONCAT44(uVar13 + (0xFFFFFFFE < uVar9), uVar9 + 1);
    }
    if (iVar11 * 0x100000 + 0x100000 < 0) {
        return (u64)(uVar13 | DAT_02044fd8) << 32;
    }
    uVar4 = uVar9 >> 0xB | uVar5 << 0x15;
    uVar6 = uVar13 | uVar5 * 2 >> 0xC | iVar11 * 0x100000;
    if (uVar9 << 0x15 == 0) {
        return CONCAT44(uVar6, uVar4);
    }
    if ((uVar9 << 0x15 & 0x80000000) == 0) {
        return CONCAT44(uVar6, uVar4);
    }
    if ((uVar9 & 0x3FF) == 0 && (uVar9 >> 0xB & 1) == 0) {
        return CONCAT44(uVar6, uVar4);
    }
    return CONCAT44(uVar6 + (0xFFFFFFFE < uVar4), uVar4 + 1);
}

// ============================================================
// FUN_020466c4 @ 0x020466C4 (80 bytes) — Division lookup dispatcher
// ============================================================
void FUN_020466c4(u32 param_1, u32 param_2)
{
    u32 uVar1;
    u32 uVar2;

    if (param_2 <= param_1) {
        uVar1 = 0x1C;
        uVar2 = param_1 >> 4;
        if ((int)param_2 <= (int)(param_1 >> 0x10)) {
            uVar1 = 0xC;
            uVar2 = param_1 >> 0x14;
        }
        if ((int)param_2 <= (int)(uVar2 >> 4)) {
            uVar1 = uVar1 - 8;
            uVar2 = uVar2 >> 8;
        }
        if ((int)param_2 <= (int)uVar2) {
            uVar1 = uVar1 - 4;
            uVar2 = uVar2 >> 4;
        }
        /* jump table dispatch — not fully recoverable */
        (void)uVar1;
        (void)uVar2;
    }
}

// ============================================================
// FUN_02046e1c @ 0x02046E1C (48 bytes) — Indirect jump dispatch
// ============================================================
void FUN_02046e1c(u32 param_1, u32 param_2, void (*callback)(void))
{
    /* indirect jump — not fully recoverable */
    callback();
}

// ============================================================
// FUN_02046e98 @ 0x02046E98 (56 bytes) — Decode variable-length pair
// ============================================================
void FUN_02046e98(u8 *param_1)
{
    u8 bVar1;
    u32 uVar2;
    u32 stack_buf[2];

    bVar1 = *param_1;
    uVar2 = (u32)(unsigned long)FUN_020470d8(param_1 + 2, stack_buf);
    if ((bVar1 & 0x40) != 0) {
        FUN_020470d8((u8 *)(unsigned long)uVar2, stack_buf);
    }
}

// ============================================================
// FUN_02046ed0 @ 0x02046ED0 (24 bytes) — Init pair from data
// ============================================================
u32 FUN_02046ed0(int param_1)
{
    u32 uVar1;

    uVar1 = DAT_02046eec;
    *(u32 *)(param_1 + 0xC) = DAT_02046ee8;
    *(u32 *)(param_1 + 0x10) = uVar1;
    return 1;
}

// ============================================================
// FUN_02046ef0 @ 0x02046EF0 (168 bytes) — Parse section header
// ============================================================
void FUN_02046ef0(int param_1, int param_2)
{
    u8 bVar1;
    u8 *pbVar3;

    pbVar3 = *(u8 **)(param_2 + 4);
    bVar1 = *pbVar3;
    *(BOOL *)(param_1 + 0x6B) = (bVar1 & 0x40) != 0;
    *(BOOL *)(param_1 + 0x6A) = (bVar1 & 0x20) != 0;
    *(BOOL *)(param_1 + 0x6C) = (bVar1 & 0x80) != 0;
    *(u16 *)(param_1 + 0x68) = (u16)pbVar3[1] << 4;
    *(u16 *)(param_1 + 0x68) = *(u16 *)(param_1 + 0x68) | 0x4000;

    {
        u8 *next = FUN_020470d8(pbVar3 + 2, (u32 *)(param_1 + 0x60));
        if ((bVar1 & 0x40) != 0) {
            FUN_020470d8(next, (u32 *)(param_1 + 0x64));
        }
    }

    if ((bVar1 & 0x40) == 0) {
        *(u32 *)(param_1 + 0x18) = *(u32 *)(param_1 + 0x14);
        return;
    }
    if ((bVar1 & 0x80) == 0) {
        *(u32 *)(param_1 + 0x18) = *(u32 *)(param_1 + 0x48);
    }
    else {
        *(u32 *)(param_1 + 0x18) = *(u32 *)(param_1 + 0x38);
    }
}

// ============================================================
// FUN_02046f98 @ 0x02046F98 (100 bytes) — Restore registers from frame
// ============================================================
u32 FUN_02046f98(int param_1)
{
    int iVar1;
    int iVar2;
    u32 uVar3;
    u32 *puVar4;

    iVar2 = *(int *)(param_1 + 0x60);
    uVar3 = 0xF;

    if (*(char *)(param_1 + 0x6A) == '\0') {
        iVar1 = 0;
    }
    else {
        iVar1 = 0x10;
    }

    puVar4 = (u32 *)((*(int *)(param_1 + 0x18) + iVar2) - iVar1);
    do {
        if (((u32)*(u16 *)(param_1 + 0x68) & (1 << (uVar3 & 0xFF))) != 0) {
            puVar4 = puVar4 - 1;
            *(u32 *)(param_1 + uVar3 * 4 + 0x1C) = *puVar4;
        }
        uVar3 = uVar3 - 1;
    } while (-1 < (int)uVar3);

    *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x18) + iVar2;
    return *(u32 *)(param_1 + 0x54);
}

// ============================================================
// FUN_02046ffc @ 0x02046FFC (20 bytes) — SWI write character
// ============================================================
u64 FUN_02046ffc(u32 param_1)
{
    (void)param_1;
    /* software_interrupt(0x123456); */
    return CONCAT44(param_1, 3);
}

// ============================================================
// FUN_02047010 @ 0x02047010 (20 bytes) — SWI read character
// ============================================================
u64 FUN_02047010(void)
{
    /* software_interrupt(0x123456); */
    return 7;
}

// ============================================================
// FUN_02047024 @ 0x02047024 (16 bytes) — SWI flush
// ============================================================
u64 FUN_02047024(void)
{
    /* software_interrupt(0x123456); */
    return 0x18;
}

// ============================================================
// FUN_02046e10 @ 0x02046e10 (8 bytes) — Thunk to FUN_02047024
// ============================================================
u64 FUN_02046e10(void)
{
    return FUN_02047024();
}

// ============================================================
// FUN_0204703c @ 0x0204703C (56 bytes) — Write buffer loop
// ============================================================
u32 FUN_0204703c(u32 param_1, int param_2, u32 *param_3)
{
    u32 uVar1;
    u32 uVar2;

    (void)param_1;
    uVar1 = *param_3;
    uVar2 = 0;
    if (uVar1 != 0) {
        do {
            FUN_02046ffc(param_2 + uVar2);
            uVar2 = uVar2 + 1;
        } while (uVar2 < uVar1);
    }
    return 0;
}

// ============================================================
// FUN_02047074 @ 0x02047074 (100 bytes) — Read buffer loop
// ============================================================
u32 FUN_02047074(u32 param_1, int param_2, u32 *param_3)
{
    u8 uVar1;
    u32 uVar2;
    u32 uVar3;

    (void)param_1;
    uVar2 = *param_3;
    uVar3 = 0;
    if (uVar2 != 0) {
        do {
            uVar1 = (u8)FUN_02047010();
            *(u8 *)(param_2 + uVar3) = uVar1;
            if ((*(char *)(param_2 + uVar3) == '\r') || (*(char *)(param_2 + uVar3) == '\n')) {
                *param_3 = uVar3 + 1;
                return 0;
            }
            uVar3 = uVar3 + 1;
        } while (uVar3 < uVar2);
    }
    return 0;
}

// ============================================================
// FUN_020470d8 @ 0x020470D8 (168 bytes) — Decode variable-length integer
// ============================================================
u8 *FUN_020470d8(u8 *param_1, u32 *param_2)
{
    u8 bVar1;
    u8 bVar2;

    bVar1 = *param_1;
    if ((bVar1 & 1) == 0) {
        *param_2 = (u32)(bVar1 >> 1);
        return param_1 + 1;
    }

    bVar2 = param_1[1];
    if ((bVar1 & 2) != 0) {
        if ((bVar1 & 4) != 0) {
            *param_2 = (u32)param_1[3] |
                       (u32)(bVar1 >> 3) << 0x18 | (u32)bVar2 << 0x10 | (u32)param_1[2] << 8;
            return param_1 + 4;
        }
        *param_2 = (u32)param_1[2] | (u32)(bVar1 >> 3) << 0x10 | (u32)bVar2 << 8;
        return param_1 + 3;
    }

    *param_2 = (u32)((u16)(bVar1 >> 2) << 8 | (u16)bVar2);
    return param_1 + 2;
}
