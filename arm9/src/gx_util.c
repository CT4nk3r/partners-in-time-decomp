/**
 * GX Utility — graphics helper functions, VRAM management (0x0202axxx-0x0202cxxx)
 * Decompiled from 0x0202a33c-0x0202cae4 region (49 functions)
 */
#include "types.h"

// ============================================================
// Forward declarations (functions defined in this file)
// ============================================================

extern void FUN_0202a74c(void);
int  FUN_0202acdc(char *param_1);
u32  FUN_0202b208(int param_1);
u32  FUN_0202b2cc(int param_1);
u32  FUN_0202b4f0(int param_1);
int  FUN_0202b608(int param_1, u32 param_2);
void FUN_0202bea8(u32 *param_1, int *param_2, u32 *param_3);
void FUN_0202bf4c(u32 *param_1);
void FUN_0202bff8(u32 *param_1);
void FUN_0202c0d0(u32 param_1, int param_2);
u32  FUN_0202c10c(int param_1);
void FUN_0202c37c(int param_1, int param_2);
void FUN_0202c860(int param_1);

char * FUN_0202a980(char *param_1, int param_2, u32 param_3, char *param_4);
void   FUN_0202a9f4(u8 *param_1, int param_2, u32 param_3, u32 param_4);
void   FUN_0202aab0(char *param_1, int param_2, u32 param_3, u32 param_4);
void   FUN_0202ac30(char *param_1, int param_2, u32 param_3, u32 param_4);

// ============================================================
// External function declarations
// ============================================================

extern void FUN_0202a5a8(u32 *param_1);
extern void thunk_FUN_02029ab8(u32 *param_1);
extern int  FUN_0202b9b8(int param_1, int param_2, int param_3);

extern void FUN_020381ac(u32 param_1, int param_2, int param_3);
extern void FUN_020380dc(u32 param_1, int param_2, int param_3);
extern void FUN_0203800c(u32 param_1, int param_2, int param_3);
extern void FUN_02037f3c(u32 param_1, int param_2, int param_3);
extern void FUN_0203867c(int param_1, int param_2, int param_3);
extern void FUN_02038144(u32 param_1, int param_2, int param_3);
extern void FUN_02038074(u32 param_1, int param_2, int param_3);
extern void FUN_02037fa4(u32 param_1, int param_2, int param_3);
extern void FUN_02037ed4(u32 param_1, int param_2, int param_3);
extern void FUN_02038614(int param_1, int param_2, int param_3);
extern void FUN_020384ec(int param_1, int param_2, int param_3);
extern void FUN_0203841c(int param_1, int param_2, int param_3);
extern void FUN_0203834c(int param_1, int param_2, int param_3);
extern void FUN_0203827c(int param_1, int param_2, int param_3);
extern void FUN_02038484(int param_1, int param_2, int param_3);
extern void FUN_020383b4(int param_1, int param_2, int param_3);
extern void FUN_020382e4(int param_1, int param_2, int param_3);
extern void FUN_02038214(int param_1, int param_2, int param_3);
extern void FUN_0203a04c(int param_1, int param_2);
extern void FUN_0203ed60(int param_1, int param_2, int param_3);
extern void FUN_0203edcc(int param_1, u32 param_2, u32 param_3);
extern void FUN_0203ef08(int param_1);
extern void FUN_0203ef50(int param_1, u32 param_2);
extern int  FUN_0203efa0(int param_1, u32 param_2, u32 param_3);
extern void FUN_0203f2a0(u32 *param_1);
extern void FUN_0203f2c8(void);
extern u32  FUN_0203f378(int param_1, u32 param_2);
extern void FUN_0203f398(void);
extern void FUN_0203f864(u32 param_1);
extern int  FUN_0203faa8(u32 param_1, u32 param_2);
extern int  FUN_0203fb40(int param_1, u32 param_2, u32 param_3);
extern void FUN_0203fd64(u32 *param_1, int param_2);
extern void FUN_0203fd8c(int param_1);

// ============================================================
// External data references (DAT_ symbols)
// ============================================================

extern int   *DAT_0202a510;
extern int   *DAT_0202a514;
extern int   *DAT_0202a518;
extern int   *DAT_0202a568;
extern u32    DAT_0202a618;
extern u32   *DAT_0202a61c;
extern u32   *DAT_0202a620;
extern u32   *DAT_0202a624;
extern u32   *DAT_0202a628;
extern u32    DAT_0202a6b4;
extern u32   *DAT_0202a6b8;
extern u32   *DAT_0202a6bc;
extern u32   *DAT_0202a6c0;
extern u32   *DAT_0202a6c4;
extern char  *DAT_0202a9f0;
extern u32   *DAT_0202aaac;
extern u32   *DAT_0202ac2c;
extern u32    DAT_0202af10;
extern u16   *DAT_0202b1dc;
extern u32    DAT_0202b1e0;
extern u16   *DAT_0202b1e4;
extern u16   *DAT_0202b1e8;
extern u16   *DAT_0202b1ec;
extern int    DAT_0202b1f0;
extern u32    DAT_0202b1f4;
extern u16   *DAT_0202b1f8;
extern u16   *DAT_0202b1fc;
extern u16   *DAT_0202b200;
extern u16   *DAT_0202b204;
extern int   *DAT_0202b6ac;
extern void (*DAT_0202b6c0)(int, int);
extern int   *DAT_0202b71c;
extern u32    DAT_0202b898;
extern u32    DAT_0202b89c;
extern u32    DAT_0202b920;
extern int   *DAT_0202b924;
extern u32    DAT_0202b928;
extern void (*DAT_0202b940)(u32, u32);
extern u32    DAT_0202bcb0;
extern u8    *DAT_0202bcb4;
extern u32    DAT_0202bcfc;
extern u32    DAT_0202bd4c;
extern u32    DAT_0202bd94;
extern u32    DAT_0202bea0;
extern int    DAT_0202bea4;
extern u32    DAT_0202bff4;
extern u32    DAT_0202c0cc;
extern u32    DAT_0202c100;
extern u32    DAT_0202c104;
extern u32    DAT_0202c108;
extern u32    DAT_0202c77c;
extern u8    *DAT_0202c780;
extern u32    DAT_0202c7d0;
extern u32    DAT_0202c828;
extern u32    DAT_0202c85c;
extern u8    *DAT_0202c904;
extern void (*DAT_0202c9bc)(void);
extern u32    DAT_0202cadc;
extern u8    *DAT_0202cae0;
extern u32    DAT_0202cb0c;

extern u8     DAT_05000000;

// ============================================================
// Functions
// ============================================================

// FUN_0202a33c @ 0x0202a33c (468 bytes) — process animation/callback queue
void FUN_0202a33c(void)
{
    u32 uVar1;
    BOOL bVar2;
    int *piVar3;
    int *piVar4;
    int *piVar5;
    u32 uVar6;
    int iVar7;
    int *piVar8;

    piVar5 = DAT_0202a518;
    piVar4 = DAT_0202a514;
    piVar3 = DAT_0202a510;
    if (*DAT_0202a510 != 0) {
        do {
            bVar2 = TRUE;
            piVar8 = (int *)*piVar3;
            while (piVar8 != (int *)0x0) {
                *piVar5 = piVar8[3];
                *piVar4 = (int)piVar8;
                uVar6 = (u32)*(u16 *)((int)piVar8 + 0x12);
                if (((int)(uVar6 << 0x1f) < 0) && (-1 < (int)(uVar6 << 0x1e))) {
                    uVar1 = ((int)*(s16 *)((int)piVar8 + 0x12) << 0x1a) >> 0x1c;
                    if ((uVar1 & 8) == 0) {
                        (**(void (**)())(*piVar8 + 8))();
                        if (*piVar4 != 0) {
                            piVar8[5] = piVar8[5] + 1;
                            *(u16 *)((int)piVar8 + 0x12) =
                                *(u16 *)((int)piVar8 + 0x12) & 0xfc3f |
                                (u16)(((((u32)*(u16 *)((int)piVar8 + 0x12) << 0x16) >> 0x1c) + 1 & 0xf) <<
                                        6);
                            if ((int)(((u32)*(u16 *)((int)piVar8 + 0x12) << 0x16) >> 0x1c) <
                                ((int)*(s16 *)((int)piVar8 + 0x12) << 0x1a) >> 0x1c) {
                                bVar2 = FALSE;
                            }
                            else {
                                *(u16 *)((int)piVar8 + 0x12) = *(u16 *)((int)piVar8 + 0x12) | 2;
                            }
                        }
                    }
                    else {
                        *(u16 *)((int)piVar8 + 0x12) =
                            *(u16 *)((int)piVar8 + 0x12) & 0xfc3f |
                            (u16)((((uVar6 << 0x16) >> 0x1c) + 1 & 0xf) << 6);
                        if ((int)-uVar1 <= (int)(((u32)*(u16 *)((int)piVar8 + 0x12) << 0x16) >> 0x1c)) {
                            (**(void (**)())(*piVar8 + 8))();
                            *(u16 *)((int)piVar8 + 0x12) = *(u16 *)((int)piVar8 + 0x12) & 0xfc3f;
                        }
                        if (*piVar4 != 0) {
                            piVar8[5] = piVar8[5] + 1;
                            *(u16 *)((int)piVar8 + 0x12) = *(u16 *)((int)piVar8 + 0x12) | 2;
                        }
                    }
                }
                piVar8 = (int *)*piVar5;
            }
        } while (!bVar2);
    }
    iVar7 = *DAT_0202a510;
    if (iVar7 != 0) {
        do {
            *(u16 *)(iVar7 + 0x12) = *(u16 *)(iVar7 + 0x12) & 0xfffd;
            if ((((int)*(s16 *)(iVar7 + 0x12) << 0x1a) >> 0x1c & 8U) == 0) {
                *(u16 *)(iVar7 + 0x12) = *(u16 *)(iVar7 + 0x12) & 0xfc3f;
            }
            iVar7 = *(int *)(iVar7 + 0xc);
        } while (iVar7 != 0);
        return;
    }
    return;
}

// FUN_0202a51c @ 0x0202a51c (76 bytes) — mark active entries for update
void FUN_0202a51c(void)
{
    int iVar1;

    iVar1 = *DAT_0202a568;
    if (iVar1 == 0) {
        return;
    }
    do {
        if ((*(char *)(iVar1 + 0x11) != '\0') && (*(char *)(iVar1 + 0x11) != -1)) {
            *(u16 *)(iVar1 + 0x12) = *(u16 *)(iVar1 + 0x12) & 0xfffe;
            *(u16 *)(iVar1 + 0x12) = *(u16 *)(iVar1 + 0x12) | 2;
        }
        iVar1 = *(int *)(iVar1 + 0xc);
    } while (iVar1 != 0);
    return;
}

// FUN_0202a58c @ 0x0202a58c (28 bytes) — clear and set update flag
void FUN_0202a58c(int param_1)
{
    *(u16 *)(param_1 + 0x12) = *(u16 *)(param_1 + 0x12) & 0xfffe;
    *(u16 *)(param_1 + 0x12) = *(u16 *)(param_1 + 0x12) | 2;
    return;
}

// FUN_0202a62c @ 0x0202a62c (136 bytes) — unlink and destroy entry
u32 * FUN_0202a62c(u32 *param_1)
{
    u32 *puVar1;
    u32 *puVar2;

    *param_1 = DAT_0202a6b4;
    if (param_1[3] != 0) {
        *(u32 *)(param_1[3] + 8) = param_1[2];
    }
    if (param_1[2] == 0) {
        *DAT_0202a6b8 = param_1[3];
    }
    else {
        *(u32 *)(param_1[2] + 0xc) = param_1[3];
    }
    puVar1 = DAT_0202a6c0;
    puVar2 = (u32 *)*DAT_0202a6c0;
    *DAT_0202a6bc = *DAT_0202a6bc + -1;
    if (puVar2 == param_1) {
        *puVar1 = 0;
    }
    if ((u32 *)*DAT_0202a6c4 == param_1) {
        *DAT_0202a6c4 = param_1[3];
    }
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_0202a850 @ 0x0202a850 (256 bytes) — formatted string printer (sprintf-like)
void FUN_0202a850(u8 *param_1, u8 *param_2, u32 *param_3)
{
    int iVar1;
    u8 *pbVar2;
    u8 *pbVar3;
    u32 uVar4;
    char cVar5;

LAB_0202a85c:
    do {
        uVar4 = (u32)*param_2;
        pbVar2 = param_2 + 1;
        if (uVar4 == 0x25) {
            pbVar2 = param_2 + 2;
            uVar4 = (u32)param_2[1];
            if (uVar4 != 0x25) {
                cVar5 = uVar4 == 0x30;
                pbVar3 = pbVar2;
                if ((BOOL)cVar5) {
                    pbVar3 = param_2 + 3;
                    uVar4 = (u32)*pbVar2;
                }
                iVar1 = 0;
                pbVar2 = pbVar3;
                while (0x2f < uVar4) {
                    if (0x39 < uVar4) {
                        param_2 = pbVar2;
                        if (uVar4 == 0x42 || uVar4 == 0x62) {
                            FUN_0202ac30((char *)param_1, iVar1, (u32)cVar5, *param_3);
                            param_3 = param_3 + 1;
                            goto LAB_0202a85c;
                        }
                        if (uVar4 == 0x44 || uVar4 == 100) {
                            FUN_0202aab0((char *)param_1, iVar1, (u32)cVar5, *param_3);
                            param_3 = param_3 + 1;
                            goto LAB_0202a85c;
                        }
                        if (uVar4 == 0x58) {
                            cVar5 = cVar5 + '\x02';
                        }
                        if (uVar4 == 0x58 || uVar4 == 0x78) {
                            FUN_0202a9f4(param_1, iVar1, (u32)cVar5, *param_3);
                            param_3 = param_3 + 1;
                            goto LAB_0202a85c;
                        }
                        if (uVar4 == 0x53 || uVar4 == 0x73) {
                            param_1 = (u8 *)FUN_0202a980((char *)param_1, iVar1, (u32)cVar5, (char *)*param_3);
                            param_3 = param_3 + 1;
                            goto LAB_0202a85c;
                        }
                        if (uVar4 == 0x43 || uVar4 == 99) {
                            *param_1 = (char)*param_3;
                            param_1 = param_1 + 1;
                            param_3 = param_3 + 1;
                            goto LAB_0202a85c;
                        }
                        break;
                    }
                    iVar1 = iVar1 * 10 + (uVar4 - 0x30);
                    uVar4 = (u32)*pbVar2;
                    pbVar2 = pbVar2 + 1;
                }
            }
        }
        *param_1 = (char)uVar4;
        param_1 = param_1 + 1;
        param_2 = pbVar2;
        if (uVar4 == 0) {
            return;
        }
    } while (TRUE);
}

// FUN_0202a980 @ 0x0202a980 (112 bytes) — format string copy with padding
char * FUN_0202a980(char *param_1, int param_2, u32 param_3, char *param_4)
{
    int iVar1;
    char *pcVar2;

    if (param_4 == (char *)0x0) {
        param_4 = DAT_0202a9f0;
    }
    iVar1 = FUN_0202acdc(param_4);
    for (; iVar1 < param_2; param_2 = param_2 + -1) {
        *param_1 = ' ';
        param_1 = param_1 + 1;
    }
    do {
        pcVar2 = param_1 + 1;
        *param_1 = *param_4;
        param_4 = param_4 + 1;
        if (*param_4 == '\0') break;
        param_1 = pcVar2;
    } while ((param_2 < 1) || (param_2 = param_2 + -1, param_2 != 0));
    *pcVar2 = '\0';
    return pcVar2;
}

// FUN_0202a9f4 @ 0x0202a9f4 (184 bytes) — format hex number to string
void FUN_0202a9f4(u8 *param_1, int param_2, u32 param_3, u32 param_4)
{
    u8 *puVar1;
    u8 uVar2;
    u8 *puVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    u8 local_18[12];

    iVar4 = *(int *)(DAT_0202aaac + ((int)param_3 >> 1) * 4);
    if ((param_3 & 1) == 0) {
        uVar2 = 0x20;
    }
    else {
        uVar2 = 0x30;
    }
    puVar3 = local_18;
    iVar6 = 0;
    if (8 < param_2) {
        param_2 = 8;
        puVar3 = local_18;
    }
    do {
        iVar5 = iVar6;
        iVar6 = iVar5 + 1;
        *puVar3 = *(u8 *)(iVar4 + (param_4 & 0xf));
        param_4 = param_4 >> 4;
        if ((param_2 != 0) && (param_2 <= iVar6)) break;
        puVar3 = puVar3 + 1;
    } while (param_4 != 0);
    if ((0 < param_2) && (iVar6 < param_2)) {
        puVar3 = local_18 + iVar5 + 1;
        do {
            iVar6 = iVar6 + 1;
            *puVar3 = uVar2;
            puVar3 = puVar3 + 1;
        } while (iVar6 < param_2);
    }
    puVar3 = local_18 + iVar6;
    do {
        puVar3 = puVar3 + -1;
        iVar6 = iVar6 + -1;
        puVar1 = param_1 + 1;
        *param_1 = *puVar3;
        param_1 = puVar1;
    } while (0 < iVar6);
    *puVar1 = 0;
    return;
}

// FUN_0202aab0 @ 0x0202aab0 (380 bytes) — format decimal number to string
void FUN_0202aab0(char *param_1, int param_2, u32 param_3, u32 param_4)
{
    BOOL bVar1;
    char *pcVar2;
    int iVar3;
    char cVar4;
    char *pcVar5;
    u32 uVar6;
    u32 uVar7;
    int iVar8;
    BOOL bVar9;
    char acStack_31[12];
    char acStack_25[5];

    if ((param_3 & 1) == 0) {
        cVar4 = ' ';
    }
    else {
        cVar4 = '0';
    }
    bVar1 = FALSE;
    if (0xb < param_2) {
        param_2 = 0xb;
    }
    bVar9 = (int)param_4 < 0;
    if (bVar9) {
        param_4 = -param_4;
    }
    uVar6 = 0;
    do {
        uVar7 = *(u32 *)(DAT_0202ac2c + uVar6 * 4);
        if (uVar6 == 9) {
            bVar1 = TRUE;
        }
        iVar8 = 0;
        for (; uVar7 <= param_4; param_4 = param_4 - uVar7) {
            iVar8 = iVar8 + 1;
        }
        if ((iVar8 != 0) || (bVar1)) {
            bVar1 = TRUE;
            acStack_31[uVar6 + 2] = (char)iVar8 + '0';
        }
        else {
            acStack_31[uVar6 + 2] = cVar4;
        }
        uVar6 = uVar6 + 1;
    } while (uVar6 < 10);
    if (param_2 == 0) {
        iVar3 = 1;
        if (acStack_31[2] == cVar4) {
            pcVar5 = acStack_31 + 2;
            do {
                pcVar5 = pcVar5 + 1;
                iVar3 = iVar3 + 1;
            } while (*pcVar5 == cVar4);
        }
        iVar8 = iVar3;
        if (bVar9) {
            iVar8 = iVar3 + -1;
            acStack_31[iVar3] = '-';
        }
    }
    else {
        param_2 = -param_2;
        iVar8 = param_2 + 0xb;
        if (bVar9) {
            if (cVar4 == ' ') {
                pcVar5 = acStack_25 + param_2;
                cVar4 = acStack_25[param_2];
                iVar3 = iVar8;
                while (cVar4 == ' ') {
                    pcVar5 = pcVar5 + 1;
                    iVar3 = iVar3 + 1;
                    cVar4 = *pcVar5;
                }
                acStack_31[iVar3] = '-';
            }
            else {
                acStack_25[param_2] = '-';
            }
        }
    }
    pcVar5 = acStack_31 + iVar8 + 1;
    do {
        iVar8 = iVar8 + 1;
        pcVar2 = param_1 + 1;
        *param_1 = *pcVar5;
        pcVar5 = pcVar5 + 1;
        param_1 = pcVar2;
    } while (iVar8 < 0xb);
    *pcVar2 = '\0';
    return;
}

// FUN_0202ac30 @ 0x0202ac30 (172 bytes) — format binary number to string
void FUN_0202ac30(char *param_1, int param_2, u32 param_3, u32 param_4)
{
    char *pcVar1;
    char cVar2;
    char *pcVar3;
    int iVar4;
    int iVar5;
    char local_28[32];

    if ((param_3 & 1) == 0) {
        cVar2 = ' ';
    }
    else {
        cVar2 = '0';
    }
    pcVar3 = local_28;
    iVar5 = 0;
    if (0x20 < param_2) {
        param_2 = 0x20;
        pcVar3 = local_28;
    }
    do {
        iVar4 = iVar5;
        *pcVar3 = ((u8)param_4 & 1) + 0x30;
        iVar5 = iVar4 + 1;
        param_4 = param_4 >> 1;
        if ((param_2 != 0) && (param_2 <= iVar5)) break;
        pcVar3 = pcVar3 + 1;
    } while (param_4 != 0);
    if ((0 < param_2) && (iVar5 < param_2)) {
        pcVar3 = local_28 + iVar4 + 1;
        do {
            iVar5 = iVar5 + 1;
            *pcVar3 = cVar2;
            pcVar3 = pcVar3 + 1;
        } while (iVar5 < param_2);
    }
    pcVar3 = local_28 + iVar5;
    do {
        pcVar3 = pcVar3 + -1;
        iVar5 = iVar5 + -1;
        pcVar1 = param_1 + 1;
        *param_1 = *pcVar3;
        param_1 = pcVar1;
    } while (0 < iVar5);
    *pcVar1 = '\0';
    return;
}

// FUN_0202ad04 @ 0x0202ad04 (136 bytes) — draw text to tilemap buffer
void FUN_0202ad04(int param_1, u8 *param_2)
{
    u8 bVar1;
    u16 *puVar2;
    int iVar3;
    u8 bVar4;
    u32 uVar5;
    u16 *puVar6;
    u8 bVar7;
    u32 uVar8;

    bVar4 = *(u8 *)(param_1 + 0x642);
    uVar5 = (u32)bVar4;
    bVar7 = *(u8 *)(param_1 + 0x641);
    uVar8 = (u32)bVar7;
    iVar3 = uVar5 << 5;
    bVar1 = *param_2;
    puVar2 = (u16 *)(param_1 + (uVar8 + uVar5 * 0x20) * 2);
    while (bVar1 != 0) {
        if (*param_2 == 10) {
            uVar8 = (u32)*(u8 *)(param_1 + 0x641);
            iVar3 = iVar3 + 0x20;
            uVar5 = uVar5 + 1;
            puVar6 = (u16 *)(param_1 + (uVar8 + iVar3) * 2);
        }
        else {
            uVar8 = uVar8 + 1;
            puVar6 = puVar2 + 1;
            *puVar2 = *param_2 - 0x20 | (u16)*(u8 *)(param_1 + 0x643) << 0xc;
        }
        bVar7 = (u8)uVar8;
        bVar4 = (u8)uVar5;
        bVar1 = param_2[1];
        puVar2 = puVar6;
        param_2 = param_2 + 1;
    }
    *(u8 *)(param_1 + 0x642) = bVar4;
    *(u8 *)(param_1 + 0x641) = bVar7;
    return;
}

// FUN_0202add4 @ 0x0202add4 (316 bytes) — copy tilemap to VRAM (main/sub BG select)
void FUN_0202add4(int param_1)
{
    u32 uVar1;

    FUN_0203a04c(param_1, 0x600);
    uVar1 = (u32)*(u8 *)(param_1 + 0x640);
    if ((int)(uVar1 << 0x1f) < 0) {
        if (*(int *)(DAT_0202af10 + 0x834) == 1) {
            return;
        }
        switch ((uVar1 << 0x1d) >> 0x1e) {
        case 0:
            FUN_02038484(param_1, 0, 0x600);
            return;
        case 1:
            FUN_020383b4(param_1, 0, 0x600);
            return;
        case 2:
            FUN_020382e4(param_1, 0, 0x600);
            return;
        case 3:
            FUN_02038214(param_1, 0, 0x600);
        }
    }
    else {
        switch ((uVar1 << 0x1d) >> 0x1e) {
        case 0:
            FUN_020384ec(param_1, 0, 0x600);
            return;
        case 1:
            FUN_0203841c(param_1, 0, 0x600);
            return;
        case 2:
            FUN_0203834c(param_1, 0, 0x600);
            return;
        case 3:
            FUN_0203827c(param_1, 0, 0x600);
            return;
        }
    }
    return;
}

// FUN_0202af40 @ 0x0202af40 (668 bytes) — configure BG layer and load tilemap
int FUN_0202af40(int param_1, int param_2, u32 param_3, int param_4, int param_5, int param_6,
                int param_7)
{
    u16 *puVar1;
    u32 uVar2;
    u16 *puVar3;
    u16 *puVar4;
    u16 *puVar5;
    u16 *puVar6;
    u16 *puVar7;
    u16 *puVar8;
    u16 *puVar9;

    *(u8 *)(param_1 + 0x640) = *(u8 *)(param_1 + 0x640) & 0xfe | (u8)param_2 & 1;
    *(u8 *)(param_1 + 0x640) = *(u8 *)(param_1 + 0x640) & 0xf9 | (u8)((param_3 & 3) << 1);
    *(char *)(param_1 + 0x643) = (char)param_6;
    puVar9 = DAT_0202b204;
    puVar8 = DAT_0202b200;
    puVar7 = DAT_0202b1fc;
    puVar6 = DAT_0202b1f8;
    puVar5 = DAT_0202b1ec;
    puVar4 = DAT_0202b1e8;
    puVar3 = DAT_0202b1e4;
    uVar2 = DAT_0202b1e0;
    puVar1 = DAT_0202b1dc;
    if (param_2 == 0) {
        switch (param_3) {
        case 0:
            *DAT_0202b1dc = *DAT_0202b1dc & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar1 = *puVar1 & 0xfffc;
            FUN_020381ac(uVar2, 0, 0x1800);
            break;
        case 1:
            *DAT_0202b1e4 = *DAT_0202b1e4 & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar3 = *puVar3 & 0xfffc;
            FUN_020380dc(uVar2, 0, 0x1800);
            break;
        case 2:
            *DAT_0202b1e8 = *DAT_0202b1e8 & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar4 = *puVar4 & 0xfffc;
            FUN_0203800c(uVar2, 0, 0x1800);
            break;
        case 3:
            *DAT_0202b1ec = *DAT_0202b1ec & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar5 = *puVar5 & 0xfffc;
            FUN_02037f3c(uVar2, 0, 0x1800);
        }
        FUN_0203867c(DAT_0202b1f0 + param_7 * 4, param_6 << 5, 4);
    }
    else if (*(int *)(DAT_0202b1f4 + 0x834) != 1) {
        switch (param_3) {
        case 0:
            *DAT_0202b1f8 = *DAT_0202b1f8 & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar6 = *puVar6 & 0xfffc;
            FUN_02038144(uVar2, 0, 0x1800);
            break;
        case 1:
            *DAT_0202b1fc = *DAT_0202b1fc & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar7 = *puVar7 & 0xfffc;
            FUN_02038074(uVar2, 0, 0x1800);
            break;
        case 2:
            *DAT_0202b200 = *DAT_0202b200 & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar8 = *puVar8 & 0xfffc;
            FUN_02037fa4(uVar2, 0, 0x1800);
            break;
        case 3:
            *DAT_0202b204 = *DAT_0202b204 & 0x43 | (u16)(param_5 << 8) | (u16)(param_4 << 2);
            *puVar9 = *puVar9 & 0xfffc;
            FUN_02037ed4(uVar2, 0, 0x1800);
        }
        FUN_02038614(DAT_0202b1f0 + param_7 * 4, param_6 << 5, 4);
    }
    return param_1;
}

// FUN_0202b208 @ 0x0202b208 (196 bytes) — DMA transfer chunk for simple stream
u32 FUN_0202b208(int param_1)
{
    if ((*(u32 *)(param_1 + 0x3c) & 1) != 0) {
        return 0;
    }
    if ((*(u16 *)(param_1 + 0xac) & 3) == 2) {
        if (*(int *)(param_1 + 0x44) == 0) {
            if (*(u32 *)(param_1 + 0xa4) < 0x201) {
                *(u16 *)(param_1 + 0xac) = 0;
                return 1;
            }
            *(int *)(param_1 + 0xa0) = *(int *)(param_1 + 0xa0) + 0x200;
            *(int *)(param_1 + 0xa4) = *(int *)(param_1 + 0xa4) + -0x200;
            if (*(u32 *)(param_1 + 0xa4) < 0x200) {
                *(u32 *)(param_1 + 0xa8) = *(u32 *)(param_1 + 0xa4);
            }
        }
    }
    else {
        *(u16 *)(param_1 + 0xac) = 2;
    }
    FUN_0203edcc(param_1 + 0x30, *(u32 *)(param_1 + 0xa0), *(u32 *)(param_1 + 0xa8));
    return 1;
}

// FUN_0202b2cc @ 0x0202b2cc (348 bytes) — DMA transfer chunk for linked-list stream
u32 FUN_0202b2cc(int param_1)
{
    u16 uVar1;
    int iVar2;
    u32 *puVar3;

    if ((*(u32 *)(param_1 + 0x3c) & 1) != 0) {
        return 0;
    }
    puVar3 = *(u32 **)(param_1 + 0x28);
    uVar1 = *(u16 *)((int)puVar3 + 0x26);
    if ((uVar1 & 3) == 2) {
        if (*(int *)(param_1 + 0x44) == 0) {
            if (((uVar1 & 0x8000) == 0) || ((u32)puVar3[6] < 0x201)) {
                *(u16 *)((int)puVar3 + 0x26) = 0;
                *(u32 *)(param_1 + 0x28) = *puVar3;
                *puVar3 = 0;
                puVar3 = *(u32 **)(param_1 + 0x28);
                if (puVar3 == (u32 *)0x0) {
                    *(u32 *)(param_1 + 0x2c) = 0;
                    return 1;
                }
                *(u16 *)((int)puVar3 + 0x26) = *(u16 *)((int)puVar3 + 0x26) & 0x8000 | 2;
            }
            else {
                puVar3[2] = puVar3[2] + 0x200;
                puVar3[8] = puVar3[8] + 0x200;
                puVar3[6] = puVar3[6] + -0x200;
                if ((u32)puVar3[6] < 0x200) {
                    puVar3[7] = puVar3[6];
                }
            }
        }
    }
    else {
        *(u16 *)((int)puVar3 + 0x26) = uVar1 & 0x8000 | 2;
    }
    if ((puVar3[4] != 0) && (iVar2 = (int)FUN_0202b608(param_1, puVar3[4]), iVar2 == 0)) {
        return 0;
    }
    FUN_0203ed60(param_1 + 0x30, puVar3[8] + *(int *)(puVar3[1] + (u32)*(u16 *)(puVar3 + 9) * 4), 0);
    FUN_0203edcc(param_1 + 0x30, puVar3[2], puVar3[7]);
    return 1;
}

// FUN_0202b4f0 @ 0x0202b4f0 (280 bytes) — DMA transfer from ring buffer queue
u32 FUN_0202b4f0(int param_1)
{
    int iVar1;
    int *piVar2;

    if ((*(u32 *)(param_1 + 0x3c) & 1) != 0) {
        return 0;
    }
    piVar2 = (int *)(param_1 + 0xbc + (u32)*(u16 *)(param_1 + 0x13c) * 0x10);
    if (((s16)piVar2[3] == 2) && (*(int *)(param_1 + 0x44) == 0)) {
        if (*(u16 *)(param_1 + 0x13c) == 7) {
            *(u16 *)(param_1 + 0x13c) = 0;
        }
        else {
            *(s16 *)(param_1 + 0x13c) = *(s16 *)(param_1 + 0x13c) + 1;
        }
        *(u16 *)(piVar2 + 3) = 0;
        if ((u32)*(u16 *)(param_1 + 0x13c) == (u32)*(u16 *)(param_1 + 0x13e)) {
            return 1;
        }
        piVar2 = (int *)(param_1 + 0xbc + (u32)*(u16 *)(param_1 + 0x13c) * 0x10);
    }
    if (*piVar2 == 0) {
        FUN_0203ed60(param_1 + 0x30, 0, 0);
    }
    else {
        iVar1 = (int)FUN_0202b608(param_1, (u32)*piVar2);
        if (iVar1 == 0) {
            return 0;
        }
    }
    FUN_0203edcc(param_1 + 0x30, piVar2[1], piVar2[2]);
    *(u16 *)(piVar2 + 3) = 2;
    return 1;
}

// FUN_0202b608 @ 0x0202b608 (72 bytes) — set source file for DMA stream
int FUN_0202b608(int param_1, u32 param_2)
{
    if ((*(u32 *)(param_1 + 0x3c) & 0x10) != 0) {
        FUN_0203ef08(param_1 + 0x30);
    }
    FUN_0203ef50(param_1 + 0x30, param_2);
    return 1;
}

// FUN_0202b650 @ 0x0202b650 (92 bytes) — try to acquire audio stream lock
BOOL FUN_0202b650(int param_1, u32 param_2)
{
    u32 uVar1;

    if (*DAT_0202b6ac == 0) {
        uVar1 = FUN_0203f378(param_1, param_2);
        if (uVar1 <= param_2) {
            *DAT_0202b6ac = param_1;
        }
        return uVar1 <= param_2;
    }
    return FALSE;
}

// FUN_0202b6b0 @ 0x0202b6b0 (16 bytes) — indirect call dispatch
void FUN_0202b6b0(void)
{
    DAT_0202b6c0(0, 0);
    return;
}

// FUN_0202b6c4 @ 0x0202b6c4 (88 bytes) — initialize stream with resource
void FUN_0202b6c4(u32 param_1, int param_2, u32 param_3)
{
    u32 uVar1;

    FUN_0203f2c8();
    if ((param_2 != 0) && (uVar1 = FUN_0203f378(param_2, param_3), uVar1 <= param_3)) {
        *DAT_0202b71c = param_2;
    }
    FUN_0203f378(0, 0);
    return;
}

// FUN_0202b770 @ 0x0202b770 (216 bytes) — open file and begin DMA streaming
u32 FUN_0202b770(int param_1, u32 param_2, u32 param_3)
{
    int iVar1;
    u32 uVar2;
    u32 uVar3;
    int iVar4;
    void (*UNRECOVERED_JUMPTABLE)(int);
    u32 uStack_18;
    u32 uStack_14;

    iVar4 = param_1 + 0x74;
    iVar1 = FUN_0203fb40(iVar4, param_3, param_2);
    if (iVar1 == 0) {
        return 0;
    }
    FUN_0203fd8c(iVar4);
    if ((*(u32 *)(param_1 + 0x3c) & 0x10) != 0) {
        FUN_0203ef08(param_1 + 0x30);
    }
    FUN_0203fd64(&uStack_18, iVar4);
    iVar1 = FUN_0203efa0(param_1 + 0x30, uStack_18, uStack_14);
    if (iVar1 != 0) {
        FUN_0203fd8c(iVar4);
        *(u32 *)(param_1 + 0xa0) = *(u32 *)(param_1 + 0x78);
        *(u32 *)(param_1 + 0xa4) = *(u32 *)(param_1 + 0x7c);
        uVar3 = *(u32 *)(param_1 + 0xa4);
        if (0x200 < uVar3) {
            uVar3 = 0x200;
        }
        *(u32 *)(param_1 + 0xa8) = uVar3;
        *(u16 *)(param_1 + 0xac) = 1;
        FUN_0202b208(param_1);
        return 1;
    }
    uVar2 = ((u32 (*)(int))UNRECOVERED_JUMPTABLE)(0);
    return uVar2;
}

// FUN_0202b848 @ 0x0202b848 (80 bytes) — check and flush stream status
u32 FUN_0202b848(void)
{
    if ((*(u32 *)(DAT_0202b898 + 0xc) & 1) == 0) {
        FUN_0203ef08(DAT_0202b898);
        FUN_0203f864(DAT_0202b89c);
        return 0;
    }
    return 2;
}

// FUN_0202b8a0 @ 0x0202b8a0 (128 bytes) — open and bind archive resource
BOOL FUN_0202b8a0(u32 param_1, u32 param_2, u32 param_3)
{
    int iVar1;

    iVar1 = FUN_0203fb40(DAT_0202b920, param_3, param_1);
    if (iVar1 != 0) {
        FUN_0203fd8c(DAT_0202b920);
        if (*DAT_0202b924 == 0) {
            FUN_0203f2a0((u32 *)DAT_0202b928);
            *DAT_0202b924 = 1;
        }
        iVar1 = FUN_0203faa8(DAT_0202b920, DAT_0202b928);
        return iVar1 != 0;
    }
    return FALSE;
}

// FUN_0202b92c @ 0x0202b92c (20 bytes) — indirect swap call
void FUN_0202b92c(u32 param_1, u32 param_2)
{
    DAT_0202b940(param_2, param_1);
    return;
}

// FUN_0202b9cc @ 0x0202b9cc (92 bytes) — search ring buffer for matching entry
u16 FUN_0202b9cc(int param_1, int param_2)
{
    u32 uVar1;
    int iVar2;

    uVar1 = (u32)*(u16 *)(param_1 + 0x13c);
    while (TRUE) {
        if (uVar1 == *(u16 *)(param_1 + 0x13e)) {
            return 0;
        }
        iVar2 = param_1 + uVar1 * 0x10;
        if (*(int *)(iVar2 + 0xbc) == param_2) break;
        if (uVar1 == 7) {
            uVar1 = 0;
        }
        else {
            uVar1 = uVar1 + 1 & 0xffff;
        }
    }
    return *(u16 *)(iVar2 + 200);
}

// FUN_0202bb98 @ 0x0202bb98 (280 bytes) — process DMA stream queue (variant A)
void FUN_0202bb98(int param_1)
{
    if ((*(s16 *)(DAT_0202bcb0 + 0x1e) == 0) &&
       (((u32)*DAT_0202bcb4 < *(u32 *)(param_1 + 0xb0) || (0xbf < *DAT_0202bcb4)))) {
        *(u16 *)(param_1 + 0xb8) = 0;
    }
    else if (*(u16 *)(param_1 + 0xb8) < *(u16 *)(param_1 + 0xb6)) {
        *(u16 *)(param_1 + 0xb8) = *(u16 *)(param_1 + 0xb8) + 1;
        if ((u32)*(u16 *)(param_1 + 0xb8) != *(u16 *)(param_1 + 0xb4) + 1) {
            return;
        }
    }
    else {
        *(u16 *)(param_1 + 0xb8) = *(u16 *)(param_1 + 0xb4);
    }
    while (TRUE) {
        if (*(int *)(param_1 + 0x28) == 0) {
            if (*(s16 *)(param_1 + 0x13c) == *(s16 *)(param_1 + 0x13e)) {
                if (*(s16 *)(param_1 + 0xac) == 0) {
                    *(u16 *)(param_1 + 0xb8) = 0;
                    return;
                }
                FUN_0202b208(param_1);
            }
            else {
                FUN_0202b4f0(param_1);
            }
        }
        else {
            FUN_0202b2cc(param_1);
        }
        if (*(s16 *)(DAT_0202bcb0 + 0x1e) != 0) break;
        if ((*(u32 *)(param_1 + 0xb0) <= (u32)*DAT_0202bcb4) && (*DAT_0202bcb4 < 0xc0)) {
            return;
        }
    }
    return;
}

// FUN_0202bcb8 @ 0x0202bcb8 (68 bytes) — destructor: release stream resources
u32 * FUN_0202bcb8(u32 *param_1)
{
    *param_1 = DAT_0202bcfc;
    if ((param_1[0xf] & 0x10) != 0) {
        FUN_0203ef08((int)(param_1 + 0xc));
    }
    FUN_0202a5a8(param_1);
    return param_1;
}

// FUN_0202bd00 @ 0x0202bd00 (76 bytes) — destructor: release stream and free
u32 * FUN_0202bd00(u32 *param_1)
{
    *param_1 = DAT_0202bd4c;
    if ((param_1[0xf] & 0x10) != 0) {
        FUN_0203ef08((int)(param_1 + 0xc));
    }
    FUN_0202a5a8(param_1);
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_0202bd50 @ 0x0202bd50 (68 bytes) — destructor: release stream (variant B)
u32 * FUN_0202bd50(u32 *param_1)
{
    *param_1 = DAT_0202bd94;
    if ((param_1[0xf] & 0x10) != 0) {
        FUN_0203ef08((int)(param_1 + 0xc));
    }
    FUN_0202a5a8(param_1);
    return param_1;
}

// FUN_0202be20 @ 0x0202be20 (128 bytes) — constructor: initialize stream object
u32 * FUN_0202be20(u32 *param_1)
{
    int iVar1;
    int in_stack_00000000;

    FUN_0202a74c();
    *param_1 = DAT_0202bea0;
    param_1[0xb] = 0;
    param_1[10] = param_1[0xb];
    param_1[0x2c] = 0xa8;
    *(u16 *)(param_1 + 0x2d) = 5;
    *(u16 *)((int)param_1 + 0xb6) = 2;
    *(u16 *)(param_1 + 0x2e) = 0;
    *(u16 *)((int)param_1 + 0x13e) = *(u16 *)(param_1 + 0x2e);
    iVar1 = DAT_0202bea4;
    *(u16 *)(param_1 + 0x4f) = *(u16 *)((int)param_1 + 0x13e);
    *(u16 *)(param_1 + 0x2b) = 0;
    if (in_stack_00000000 != iVar1) {
        FUN_0203f398();
    }
    FUN_0203f2a0(param_1 + 0xc);
    return param_1;
}

// FUN_0202bea8 @ 0x0202bea8 (164 bytes) — decode variable-length encoded data
void FUN_0202bea8(u32 *param_1, int *param_2, u32 *param_3)
{
    u8 bVar1;
    u8 bVar2;
    u8 *pbVar3;
    u8 *pbVar4;
    u32 uVar5;

    pbVar3 = (u8 *)*param_1;
    pbVar4 = pbVar3 + 1;
    bVar1 = *pbVar3;
    if (param_3 == (u32 *)0x0) {
        bVar1 = bVar1 >> 6;
    }
    if (param_3 == (u32 *)0x0) {
        pbVar4 = pbVar4 + bVar1;
    }
    else {
        bVar2 = bVar1 >> 6;
        uVar5 = bVar1 & 0x3f;
        if (bVar2 != 0) {
            uVar5 = uVar5 | (u32)*pbVar4 << 6;
            pbVar4 = pbVar3 + 2;
            if (bVar2 != 1) {
                uVar5 = uVar5 | (u32)pbVar3[2] << 0xc;
                pbVar4 = pbVar3 + 3;
                if (bVar2 != 2) {
                    pbVar4 = pbVar3 + 4;
                    uVar5 = uVar5 | (u32)pbVar3[3] << 0x12;
                }
            }
        }
        *param_3 = uVar5;
    }
    bVar1 = *pbVar4 >> 6;
    uVar5 = *pbVar4 & 0x3f;
    pbVar3 = pbVar4 + 1;
    if (bVar1 != 0) {
        uVar5 = uVar5 | (u32)pbVar4[1] << 6;
        pbVar3 = pbVar4 + 2;
        if (bVar1 != 1) {
            uVar5 = uVar5 | (u32)pbVar4[2] << 0xc;
            pbVar3 = pbVar4 + 3;
            if (bVar1 != 2) {
                pbVar3 = pbVar4 + 4;
                uVar5 = uVar5 | (u32)pbVar4[3] << 0x12;
            }
        }
    }
    *param_2 = uVar5 + 1;
    *param_1 = (u32)pbVar3;
    return;
}

// FUN_0202bf4c @ 0x0202bf4c (48 bytes) — dispatch decompression via jump table
void FUN_0202bf4c(u32 *param_1)
{
    (*(void (**)())(DAT_0202bff4 + (*(u8 *)*param_1 & 3) * 4))();
    return;
}

// FUN_0202bff8 @ 0x0202bff8 (52 bytes) — dispatch VRAM copy via jump table
void FUN_0202bff8(u32 *param_1)
{
    (*(void (**)())(DAT_0202c0cc + (*(u8 *)*param_1 & 3) * 4))();
    return;
}

// FUN_0202c0d0 @ 0x0202c0d0 (48 bytes) — select DMA transfer mode based on address
void FUN_0202c0d0(u32 param_1, int param_2)
{
    BOOL bVar1;
    u32 uVar2;

    bVar1 = FALSE;
    if ((0x4ffffff < *(u32 *)(param_2 + 0x30)) && (*(u32 *)(param_2 + 0x30) < DAT_0202c100)) {
        bVar1 = TRUE;
    }
    uVar2 = DAT_0202c104;
    if (!bVar1) {
        uVar2 = DAT_0202c108;
    }
    *(u32 *)(param_2 + 0x34) = uVar2;
    return;
}

// FUN_0202c10c @ 0x0202c10c (624 bytes) — DMA transfer with decompression support
u32 FUN_0202c10c(int param_1)
{
    u16 uVar1;
    int iVar2;
    u32 *puVar3;

    if ((*(u32 *)(param_1 + 0x3c) & 1) != 0) {
        return 0;
    }
    puVar3 = *(u32 **)(param_1 + 0x28);
    uVar1 = *(u16 *)((int)puVar3 + 0x26);
    if ((uVar1 & 3) == 2) {
        if (*(int *)(param_1 + 0x44) == 0) {
            if (*(s16 *)(puVar3 + 10) == 0) {
                if (((uVar1 & 0x8000) == 0) || ((u32)puVar3[6] < 0x201)) {
                    *(u16 *)((int)puVar3 + 0x26) = 0;
                    *(u32 *)(param_1 + 0x28) = *puVar3;
                    *puVar3 = 0;
                    if (*(int *)(param_1 + 0x28) == 0) {
                        *(u32 *)(param_1 + 0x2c) = 0;
                        return 1;
                    }
                    if (*(s16 *)(*(int *)(param_1 + 0x28) + 0x28) != 0) {
                        FUN_0202c0d0(param_1, *(int *)(param_1 + 0x28));
                    }
                    puVar3 = *(u32 **)(param_1 + 0x28);
                    *(u16 *)((int)puVar3 + 0x26) = *(u16 *)((int)puVar3 + 0x26) & 0x8000 | 2;
                }
                else {
                    puVar3[2] = puVar3[2] + 0x200;
                    puVar3[8] = puVar3[8] + 0x200;
                    puVar3[6] = puVar3[6] + -0x200;
                    if ((u32)puVar3[6] < 0x200) {
                        puVar3[7] = puVar3[6];
                    }
                }
            }
            else {
                puVar3[2] = param_1 + 0x140;
                (*(void (*)(u32 *, u32 *))puVar3[0xd])(puVar3 + 2, puVar3 + 0xc);
                *(s16 *)(puVar3 + 0xf) = *(s16 *)(puVar3 + 0xf) + -1;
                if (*(s16 *)(puVar3 + 0xf) == 0) {
                    *(u16 *)((int)puVar3 + 0x26) = 0;
                    *(u16 *)(puVar3 + 10) = 0;
                    *(u32 *)(param_1 + 0x28) = *puVar3;
                    *puVar3 = 0;
                    if (*(int *)(param_1 + 0x28) == 0) {
                        *(u32 *)(param_1 + 0x2c) = 0;
                        return 1;
                    }
                    if (*(s16 *)(*(int *)(param_1 + 0x28) + 0x28) != 0) {
                        FUN_0202c0d0(param_1, *(int *)(param_1 + 0x28));
                        *(int *)(*(int *)(param_1 + 0x28) + 8) = param_1 + 0x140;
                    }
                    puVar3 = *(u32 **)(param_1 + 0x28);
                    *(u16 *)((int)puVar3 + 0x26) = *(u16 *)((int)puVar3 + 0x26) & 0x8000 | 2;
                }
                else {
                    puVar3[8] = puVar3[8] + puVar3[7];
                    puVar3[2] = (u32)(puVar3 + 7);
                    puVar3[7] = 2;
                    FUN_0202c37c(param_1, (int)puVar3);
                    puVar3[8] = puVar3[8] + 2;
                    puVar3[2] = param_1 + 0x140;
                }
            }
        }
    }
    else {
        *(u16 *)((int)puVar3 + 0x26) = uVar1 & 0x8000 | 2;
    }
    if ((puVar3[4] != 0) && (iVar2 = (int)FUN_0202b608(param_1, puVar3[4]), iVar2 == 0)) {
        return 0;
    }
    FUN_0203ed60(param_1 + 0x30, puVar3[8] + *(int *)(puVar3[1] + (u32)*(u16 *)(puVar3 + 9) * 4), 0);
    FUN_0203edcc(param_1 + 0x30, puVar3[2], puVar3[7]);
    if (*(s16 *)(puVar3 + 10) == 2) {
        *(u16 *)(puVar3 + 10) = 3;
    }
    return 1;
}

// FUN_0202c37c @ 0x0202c37c (144 bytes) — wait for DMA and transfer data
void FUN_0202c37c(int param_1, int param_2)
{
    int iVar1;

    do {
    } while ((*(u32 *)(param_1 + 0x3c) & 1) != 0);
    if ((*(int *)(param_2 + 0x10) != 0) && (iVar1 = (int)FUN_0202b608(param_1, *(u32 *)(param_2 + 0x10)), iVar1 == 0)) {
        return;
    }
    FUN_0203ed60(param_1 + 0x30,
                 *(int *)(param_2 + 0x20) +
                 *(int *)(*(int *)(param_2 + 4) + (u32)*(u16 *)(param_2 + 0x24) * 4), 0);
    FUN_0203edcc(param_1 + 0x30, *(u32 *)(param_2 + 8), *(u32 *)(param_2 + 0x1c));
    return;
}

// FUN_0202c40c @ 0x0202c40c (204 bytes) — enqueue DMA transfer request
void FUN_0202c40c(int param_1, u32 *param_2, int param_3, int param_4, u32 param_5,
                 u16 param_6)
{
    BOOL bVar1;
    int iVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;

    if (param_4 != 0) {
        param_2[1] = param_4;
    }
    *(u16 *)((int)param_2 + 0x26) = param_6 | 1;
    *(u16 *)(param_2 + 10) = 0;
    param_2[8] = param_3;
    param_2[4] = param_5;
    param_2[2] = param_2[3];
    *param_2 = 0;
    iVar2 = (int)FUN_0202b9b8(param_1, param_2[1], *(u16 *)(param_2 + 9));
    uVar4 = param_2[5];
    bVar1 = TRUE;
    if ((uVar4 != 0) && (uVar4 <= (u32)(iVar2 - param_3))) {
        bVar1 = FALSE;
    }
    uVar5 = iVar2 - param_3;
    if (!bVar1) {
        uVar5 = uVar4;
    }
    param_2[6] = uVar5;
    if ((param_6 == 0) || ((u32)param_2[6] < 0x201)) {
        uVar3 = param_2[6];
    }
    else {
        uVar3 = 0x200;
    }
    param_2[7] = uVar3;
    if (*(u32 **)(param_1 + 0x2c) == (u32 *)0x0) {
        *(u32 **)(param_1 + 0x2c) = param_2;
        *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x2c);
    }
    else {
        **(u32 **)(param_1 + 0x2c) = (u32)param_2;
        *(u32 **)(param_1 + 0x2c) = param_2;
    }
    FUN_0202c10c(param_1);
    return;
}

// FUN_0202c4d8 @ 0x0202c4d8 (116 bytes) — flush pending DMA for specific entry
void FUN_0202c4d8(int param_1, int param_2)
{
    int *piVar1;

    for (piVar1 = *(int **)(param_1 + 0x28); (piVar1 != (int *)0x0 && (piVar1 != (int *)param_2));
        piVar1 = (int *)*piVar1) {
    }
    if (piVar1 != (int *)0x0) {
        if (*(s16 *)(param_2 + 0x26) == 0) {
            return;
        }
        do {
            FUN_0202c10c(param_1);
        } while (*(s16 *)(param_2 + 0x26) != 0);
        return;
    }
    return;
}

// FUN_0202c54c @ 0x0202c54c (152 bytes) — enqueue compressed DMA transfer
void FUN_0202c54c(int param_1, u32 *param_2, u32 param_3)
{
    param_2[0xb] = param_3;
    param_2[0xc] = param_2[0xb];
    *(u16 *)((int)param_2 + 0x26) = 1;
    *(u16 *)(param_2 + 10) = 2;
    *param_2 = 0;
    param_2[2] = (u32)(param_2 + 7);
    param_2[7] = 2;
    FUN_0202c37c(param_1, (int)param_2);
    param_2[8] = param_2[8] + 2;
    param_2[2] = param_1 + 0x140;
    if (*(u32 **)(param_1 + 0x2c) == (u32 *)0x0) {
        *(u32 **)(param_1 + 0x2c) = param_2;
        *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x2c);
        FUN_0202c0d0(param_1, (int)param_2);
    }
    else {
        **(u32 **)(param_1 + 0x2c) = (u32)param_2;
        *(u32 **)(param_1 + 0x2c) = param_2;
    }
    FUN_0202c10c(param_1);
    return;
}

// FUN_0202c5e4 @ 0x0202c5e4 (128 bytes) — initialize and read compressed header
u32 FUN_0202c5e4(u32 param_1, int param_2, int param_3, u32 param_4)
{
    u8 auStack_18[8];
    u16 local_10[4];

    if (param_3 != 0) {
        *(int *)(param_2 + 4) = param_3;
    }
    *(u8 **)(param_2 + 8) = auStack_18;
    *(u32 *)(param_2 + 0x10) = param_4;
    *(u32 *)(param_2 + 0x1c) = 8;
    *(u32 *)(param_2 + 0x20) = 0;
    FUN_0202c37c(param_1, param_2);
    *(u8 **)(param_2 + 8) = auStack_18;
    FUN_0202bea8((u32 *)(param_2 + 8), (int *)local_10, (u32 *)(param_2 + 0x38));
    *(int *)(param_2 + 0x20) = *(int *)(param_2 + 8) - (int)auStack_18;
    *(u16 *)(param_2 + 0x28) = 1;
    *(u16 *)(param_2 + 0x3c) = local_10[0];
    return *(u32 *)(param_2 + 0x38);
}

// FUN_0202c664 @ 0x0202c664 (280 bytes) — process DMA stream queue (variant B)
void FUN_0202c664(int param_1)
{
    if ((*(s16 *)(DAT_0202c77c + 0x1e) == 0) &&
       (((u32)*DAT_0202c780 < *(u32 *)(param_1 + 0xb0) || (0xbf < *DAT_0202c780)))) {
        *(u16 *)(param_1 + 0xb8) = 0;
    }
    else if (*(u16 *)(param_1 + 0xb8) < *(u16 *)(param_1 + 0xb6)) {
        *(u16 *)(param_1 + 0xb8) = *(u16 *)(param_1 + 0xb8) + 1;
        if ((u32)*(u16 *)(param_1 + 0xb8) != *(u16 *)(param_1 + 0xb4) + 1) {
            return;
        }
    }
    else {
        *(u16 *)(param_1 + 0xb8) = *(u16 *)(param_1 + 0xb4);
    }
    while (TRUE) {
        if (*(int *)(param_1 + 0x28) == 0) {
            if (*(s16 *)(param_1 + 0x13c) == *(s16 *)(param_1 + 0x13e)) {
                if (*(s16 *)(param_1 + 0xac) == 0) {
                    *(u16 *)(param_1 + 0xb8) = 0;
                    return;
                }
                FUN_0202b208(param_1);
            }
            else {
                FUN_0202b4f0(param_1);
            }
        }
        else {
            FUN_0202c10c(param_1);
        }
        if (*(s16 *)(DAT_0202c77c + 0x1e) != 0) break;
        if ((*(u32 *)(param_1 + 0xb0) <= (u32)*DAT_0202c780) && (*DAT_0202c780 < 0xc0)) {
            return;
        }
    }
    return;
}

// FUN_0202c7a8 @ 0x0202c7a8 (40 bytes) — destructor: destroy and free (variant C)
u32 * FUN_0202c7a8(u32 *param_1)
{
    *param_1 = DAT_0202c7d0;
    FUN_0202bcb8(param_1);
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_0202c7f8 @ 0x0202c7f8 (48 bytes) — constructor: init stream object (variant A)
u32 * FUN_0202c7f8(u32 *param_1)
{
    FUN_0202be20(param_1);
    *param_1 = DAT_0202c828;
    return param_1;
}

// FUN_0202c82c @ 0x0202c82c (48 bytes) — constructor: init stream object (variant B)
u32 * FUN_0202c82c(u32 *param_1)
{
    FUN_0202be20(param_1);
    *param_1 = DAT_0202c85c;
    return param_1;
}

// FUN_0202c860 @ 0x0202c860 (164 bytes) — process decompressed DMA transfer
void FUN_0202c860(int param_1)
{
    u32 *puVar1;

    puVar1 = *(u32 **)(param_1 + 0x28);
    puVar1[2] = puVar1[2] + 2;
    if (((u8 *)puVar1[0xc] < &DAT_05000000) || (DAT_0202c904 <= (u8 *)puVar1[0xc])) {
        FUN_0202bf4c(puVar1 + 2);
    }
    else {
        FUN_0202bff8(puVar1 + 2);
    }
    *(u16 *)(puVar1 + 10) = 3;
    *(s16 *)(puVar1 + 0xf) = *(s16 *)(puVar1 + 0xf) + -1;
    if (*(s16 *)(puVar1 + 0xf) == 0) {
        *(u16 *)(puVar1 + 10) = 0;
        *(u32 *)(param_1 + 0x28) = *puVar1;
        *puVar1 = 0;
        if (*(int *)(param_1 + 0x28) == 0) {
            *(u32 *)(param_1 + 0x2c) = 0;
        }
        return;
    }
    return;
}

// FUN_0202c908 @ 0x0202c908 (116 bytes) — flush pending decompressed DMA for entry
void FUN_0202c908(int param_1, int param_2)
{
    int *piVar1;

    for (piVar1 = *(int **)(param_1 + 0x28); (piVar1 != (int *)0x0 && (piVar1 != (int *)param_2));
        piVar1 = (int *)*piVar1) {
    }
    if (piVar1 != (int *)0x0) {
        if (*(s16 *)(param_2 + 0x28) == 0) {
            return;
        }
        do {
            FUN_0202c860(param_1);
        } while (*(s16 *)(param_2 + 0x28) != 0);
        return;
    }
    return;
}

// FUN_0202c97c @ 0x0202c97c (64 bytes) — enqueue decompressed DMA transfer
void FUN_0202c97c(int param_1, u32 *param_2, u32 param_3)
{
    void (*UNRECOVERED_JUMPTABLE)(void);

    param_2[0xb] = param_3;
    param_2[0xc] = param_2[0xb];
    *(u16 *)(param_2 + 10) = 2;
    *param_2 = 0;
    UNRECOVERED_JUMPTABLE = DAT_0202c9bc;
    if (*(u32 **)(param_1 + 0x2c) == (u32 *)0x0) {
        *(u32 **)(param_1 + 0x2c) = param_2;
        *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x2c);
    }
    else {
        **(u32 **)(param_1 + 0x2c) = (u32)param_2;
        *(u32 **)(param_1 + 0x2c) = param_2;
    }
    (*UNRECOVERED_JUMPTABLE)();
    return;
}

// FUN_0202c9c0 @ 0x0202c9c0 (68 bytes) — read compressed header and store result
u32 FUN_0202c9c0(u32 param_1, int param_2)
{
    u16 local_10[4];

    *(u32 *)(param_2 + 8) = *(u32 *)(param_2 + 0xc);
    FUN_0202bea8((u32 *)(param_2 + 8), (int *)local_10, (u32 *)(param_2 + 0x38));
    *(u16 *)(param_2 + 0x3c) = local_10[0];
    *(u16 *)(param_2 + 0x28) = 1;
    return *(u32 *)(param_2 + 0x38);
}

// FUN_0202ca04 @ 0x0202ca04 (216 bytes) — process decompressed DMA stream queue
void FUN_0202ca04(int param_1)
{
    if ((*(s16 *)(DAT_0202cadc + 0x1e) == 0) &&
       (((u32)*DAT_0202cae0 < *(u32 *)(param_1 + 0x30) || (0xbf < *DAT_0202cae0)))) {
        *(u16 *)(param_1 + 0x38) = 0;
    }
    else if (*(u16 *)(param_1 + 0x38) < *(u16 *)(param_1 + 0x36)) {
        *(u16 *)(param_1 + 0x38) = *(u16 *)(param_1 + 0x38) + 1;
        if ((u32)*(u16 *)(param_1 + 0x38) != *(u16 *)(param_1 + 0x34) + 1) {
            return;
        }
    }
    else {
        *(u16 *)(param_1 + 0x38) = *(u16 *)(param_1 + 0x34);
    }
    while (TRUE) {
        if (*(int *)(param_1 + 0x28) == 0) {
            *(u16 *)(param_1 + 0x38) = 0;
            return;
        }
        FUN_0202c860(param_1);
        if (*(s16 *)(DAT_0202cadc + 0x1e) != 0) break;
        if ((*(u32 *)(param_1 + 0x30) <= (u32)*DAT_0202cae0) && (*DAT_0202cae0 < 0xc0)) {
            return;
        }
    }
    return;
}

// FUN_0202cae4 @ 0x0202cae4 (40 bytes) — destructor: destroy stream (variant D)
u32 * FUN_0202cae4(u32 *param_1)
{
    *param_1 = DAT_0202cb0c;
    FUN_0202a5a8(param_1);
    thunk_FUN_02029ab8(param_1);
    return param_1;
}
