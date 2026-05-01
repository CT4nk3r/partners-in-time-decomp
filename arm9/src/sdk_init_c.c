/**
 * SDK Init C — interrupt dispatch, timer, power management (0x02043xxx-0x02044xxx)
 * Decompiled from 0x02043464-0x0204401c region (23 functions)
 */
#include "types.h"

/* ------------------------------------------------------------------ */
/*  External function declarations                                     */
/* ------------------------------------------------------------------ */
extern void FUN_02004228(...);
extern void FUN_0200445a(...);
extern u32  FUN_02038d00(...);
extern void FUN_02038c4c(...);
extern void FUN_02038c80(...);
extern void FUN_02038cc8(...);
extern void FUN_02038dcc(...);
extern u16  FUN_02038e7c(...);
extern u32  FUN_02038f14(...);
extern int  FUN_02038fb4(...);
extern void FUN_02038fd4(...);
extern void FUN_020394a0(...);
extern void FUN_020394d4(...);
extern void FUN_02039548(...);
extern void FUN_02039fe0(...);
extern void FUN_0203a014(...);
extern void FUN_0203a030(...);
extern void FUN_0203a04c(...);
extern void FUN_0203a070(...);
extern void FUN_0203a088(...);
extern u32  FUN_0203a4d8(...);
extern u32  FUN_0203ae38(...);
extern void FUN_0203ae4c(...);
extern void FUN_0203aea8(...);
extern void FUN_0203aeb4(...);
extern void FUN_0203b2c8(...);
extern void FUN_0203b53c(...);
extern void FUN_0203b7f0(...);
extern void FUN_0203b9a8(...);
extern void FUN_0203bcf8(...);
extern int  FUN_0203d518(...);
extern int  FUN_0203d5cc(...);
extern void FUN_0203d5f4(...);
extern void thunk_FUN_0203d660(...);
extern void FUN_020417d0(...);
extern void FUN_02043bf4(...);
extern void FUN_02043f50(...);

/* ------------------------------------------------------------------ */
/*  External data declarations                                         */
/* ------------------------------------------------------------------ */

/* FUN_02043464 */
extern u32 *DAT_02043494;

/* FUN_02043498 */
extern int   DAT_0204358c;
extern u32  *DAT_02043590;
extern u32  *DAT_02043594;

/* FUN_02043598 */
extern int   DAT_020436d8;
extern u32   DAT_020436dc;
extern u32   DAT_020436e0;
extern u32  *DAT_020436e4;
extern u32   DAT_020436e8;

/* FUN_020436ec */
extern int   DAT_0204375c;

/* FUN_02043760 */
extern int   DAT_020437a8;
extern u32   DAT_020437ac;
extern int   DAT_020437b0;
extern u32  *DAT_020437b4;

/* FUN_020437b8 */
extern u32  *DAT_02043830;
extern u32  *DAT_02043834;
extern u8   *DAT_02043838;
extern u8   *DAT_0204383c;
extern u8   *DAT_02043840;
extern u8   *DAT_02043844;
extern u8   *DAT_02043848;
extern u8   *DAT_0204384c;
extern u8   *DAT_02043850;
extern u8   *DAT_02043854;

/* FUN_02043858 */
extern u32  *DAT_020438d4;

/* FUN_020438d8 */
extern int   DAT_0204396c;

/* FUN_02043b10 */
extern u16  *DAT_02043b4c;

/* FUN_02043b50 */
extern u32  *DAT_02043bb4;
extern u32  *DAT_02043bb8;

/* FUN_02043bbc */
extern u32   DAT_02043bec;
extern u32  *DAT_02043bf0;

/* FUN_02043c80 */
extern u32   DAT_02043cfc;

/* FUN_02043d00 / FUN_02043d34 */
extern u16  *DAT_02043d30;
extern u16  *DAT_02043d78;

/* FUN_02043d7c */
extern u32  *DAT_02043e88;
extern u32   DAT_02043e8c;
extern int   DAT_02043e90;
extern u32  *DAT_02043e94;
extern u8    DAT_080000b2;
extern u32   _DAT_080000be;
extern u32   _DAT_080000ac;

/* FUN_02043e98 */
extern u8   *DAT_02043ed0;
extern u8    DAT_02043ed4;

/* FUN_02043f10 */
extern int   DAT_02043f48;
extern u32   DAT_02043f4c;

/* FUN_02043f80 */
extern u32  *DAT_02043fdc;

/* FUN_02043fe0 */
extern u32  *DAT_02044018;

/* FUN_0204401c */
extern u32  *DAT_020441c0;
extern u16  *DAT_020441c4;
extern u16  *DAT_020441c8;
extern s16  *DAT_020441cc;
extern u16  *DAT_020441d0;
extern int   DAT_020441d4;
extern u32   DAT_020441d8;
extern u16  *DAT_020441dc;
extern u32   DAT_020441e0;
extern u32   DAT_020441e4;

/* ------------------------------------------------------------------ */
/*  Forward declarations (functions defined in this file)               */
/* ------------------------------------------------------------------ */
static void FUN_020437b8(u32 param_1, u32 param_2);
static void FUN_02043760(void);
static void FUN_02043858(void);
static BOOL FUN_020438d8(int param_1);
static void FUN_02043aac(u32 param_1, u32 param_2);
static void FUN_02043b10(void);
static void FUN_02043c58(u32 param_1, int *param_2);
static void FUN_02043c80(u32 param_1, u32 *param_2);
static void FUN_02043d00(int *param_1);
static void FUN_02043d34(int *param_1);

/* ------------------------------------------------------------------ */
/*  Functions                                                          */
/* ------------------------------------------------------------------ */

/* 0x02043464 — dispatch callback from card read */
void FUN_02043464(void)
{
    u32 *puVar1;
    int iVar2;

    puVar1 = DAT_02043494;
    iVar2 = FUN_020438d8((int)DAT_02043494);
    if (iVar2 != 0) {
        ((void (*)(u32 *))(*puVar1))(puVar1);
    }
    FUN_02043858();
}

/* 0x02043498 — card sector read loop */
void FUN_02043498(int param_1)
{
    int iVar1;
    u32 *puVar2;
    u32 *puVar3;
    int iVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;

    puVar3 = DAT_02043594;
    puVar2 = DAT_02043590;
    iVar1 = DAT_0204358c;
    do {
        while (TRUE) {
            uVar5 = *(u32 *)(iVar1 + 0x18) & 0xFFFFFE00;
            if (((uVar5 != *(u32 *)(iVar1 + 0x18)) ||
                 ((*(u32 *)(iVar1 + 0x1c) & 3) != 0)) ||
                (uVar7 = *(u32 *)(iVar1 + 0x1c),
                 *(u32 *)(iVar1 + 0x20) < 0x200)) {
                *(u32 *)(param_1 + 8) = uVar5;
                uVar7 = (u32)(param_1 + 0x20);
            }
            FUN_020437b8(uVar5 >> 8 | 0xB7000000, 0);
            uVar5 = 0;
            *puVar3 = *(u32 *)(param_1 + 4);
            do {
                uVar6 = *puVar3;
                if (((uVar6 & 0x800000) != 0) && (uVar5 < 0x200)) {
                    *(u32 *)(uVar7 + uVar5 * 4) = *puVar2;
                    uVar5 = uVar5 + 1;
                }
            } while ((uVar6 & 0x80000000) != 0);
            if (uVar7 != *(u32 *)(iVar1 + 0x1c))
                break;
            iVar4 = *(int *)(iVar1 + 0x20) + -0x200;
            *(int *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + 0x200;
            *(int *)(iVar1 + 0x1c) = *(int *)(iVar1 + 0x1c) + 0x200;
            *(int *)(iVar1 + 0x20) = iVar4;
            if (iVar4 == 0) {
                return;
            }
        }
        iVar4 = FUN_020438d8(param_1);
    } while (iVar4 != 0);
}

/* 0x02043598 — card read with cache flush */
int FUN_02043598(int param_1)
{
    BOOL bVar1;
    BOOL bVar2;
    int iVar3;
    u32 uVar4;
    u32 uVar5;
    BOOL bVar6;
    u32 uVar7;
    BOOL bVar8;
    u32 uVar9;
    int iVar10;
    int iVar11;

    iVar3 = DAT_020436d8;
    iVar10 = 0;
    uVar9 = *(u32 *)(DAT_020436d8 + 0x1c);
    bVar2 = FALSE;
    bVar1 = FALSE;
    bVar6 = FALSE;
    if (((uVar9 & 3) == 0) && (*(u32 *)(DAT_020436d8 + 0x24) < 4)) {
        bVar6 = TRUE;
    }
    if (bVar6) {
        iVar11 = *(int *)(DAT_020436d8 + 0x20);
        uVar4 = FUN_0203a4d8();
        uVar7 = uVar9 + (u32)iVar11;
        bVar8 = TRUE;
        bVar6 = FALSE;
        if ((DAT_020436dc < uVar7) && (uVar9 < 0x2000000)) {
            bVar6 = TRUE;
        }
        if ((!bVar6) && ((uVar7 <= uVar4 || (uVar4 + 0x4000 <= uVar9)))) {
            bVar8 = FALSE;
        }
        if (!bVar8) {
            bVar1 = TRUE;
        }
    }
    if (bVar1) {
        if (((*(u32 *)(iVar3 + 0x18) | *(u32 *)(iVar3 + 0x20)) & DAT_020436e0) == 0) {
            bVar2 = TRUE;
        }
    }
    if ((bVar2) && (*(int *)(iVar3 + 0x20) != 0)) {
        iVar10 = 1;
    }
    *(u32 *)(param_1 + 4) = *DAT_020436e4 & 0xF8FFFFFF | 0xA1000000;
    if (iVar10 != 0) {
        uVar5 = FUN_0203ae38();
        FUN_0203a030(*(u32 *)(iVar3 + 0x1c), *(u32 *)(iVar3 + 0x20));
        FUN_0203a014(*(u32 *)(iVar3 + 0x1c), *(u32 *)(iVar3 + 0x20));
        FUN_0203a088(*(u32 *)(iVar3 + 0x1c), *(u32 *)(iVar3 + 0x20));
        FUN_02038dcc(0x80000, DAT_020436e8);
        FUN_02038c4c(0x80000);
        FUN_02038cc8(0x80000);
        FUN_0203a070();
        FUN_0203ae4c(uVar5);
        FUN_02043760();
    }
    return iVar10;
}

/* 0x020436ec — advance card DMA and dispatch */
void FUN_020436ec(void)
{
    int *piVar1;
    int iVar2;
    int iVar3;

    FUN_0203b2c8(*(u32 *)(DAT_0204375c + 0x24));
    iVar2 = DAT_0204375c;
    piVar1 = (int *)(DAT_0204375c + 0x1c);
    iVar3 = *(int *)(DAT_0204375c + 0x20) + -0x200;
    *(int *)(DAT_0204375c + 0x18) = *(int *)(DAT_0204375c + 0x18) + 0x200;
    *(int *)(iVar2 + 0x1c) = *piVar1 + 0x200;
    *(int *)(iVar2 + 0x20) = iVar3;
    if (iVar3 == 0) {
        FUN_02038c80(0x80000);
        FUN_02038c4c(0x80000);
        FUN_02043858();
        return;
    }
    FUN_02043760();
}

/* 0x02043760 — start card DMA transfer */
static void FUN_02043760(void)
{
    int iVar1;

    iVar1 = DAT_020437a8;
    FUN_0203bcf8(*(u32 *)(DAT_020437a8 + 0x24), DAT_020437ac,
                 *(u32 *)(DAT_020437a8 + 0x1c), 0x200);
    FUN_020437b8(*(u32 *)(iVar1 + 0x18) >> 8 | 0xB7000000,
                 *(u32 *)(iVar1 + 0x18) << 0x18);
    *DAT_020437b4 = *(u32 *)(DAT_020437b0 + 4);
}

/* 0x020437b8 — send card command bytes */
static void FUN_020437b8(u32 param_1, u32 param_2)
{
    u8 *puVar1;
    u8 *puVar2;

    puVar1 = DAT_02043838;
    do {
    } while ((*DAT_02043830 & 0x80000000) != 0);
    *DAT_02043834 = 0xC0;
    puVar2 = DAT_0204383c;
    *puVar1 = (u8)(param_1 >> 0x18);
    puVar1 = DAT_02043840;
    *puVar2 = (u8)(param_1 >> 0x10);
    puVar2 = DAT_02043844;
    *puVar1 = (u8)(param_1 >> 8);
    puVar1 = DAT_02043848;
    *puVar2 = (u8)param_1;
    puVar2 = DAT_0204384c;
    *puVar1 = (u8)(param_2 >> 0x18);
    puVar1 = DAT_02043850;
    *puVar2 = (u8)(param_2 >> 0x10);
    puVar2 = DAT_02043854;
    *puVar1 = (u8)(param_2 >> 8);
    *puVar2 = (u8)param_2;
}

/* 0x02043858 — complete card operation and invoke callback */
static void FUN_02043858(void)
{
    u32 *puVar1;
    u32 uVar2;
    u32 uVar3;
    void (*pcVar4)(u32);

    puVar1 = DAT_020438d4;
    pcVar4 = (void (*)(u32))DAT_020438d4[10];
    uVar3 = DAT_020438d4[0xB];
    *(u32 *)*DAT_020438d4 = 0;
    uVar2 = FUN_0203ae38();
    puVar1[0xD] = puVar1[0xD] & 0xFFFFFFF3;
    FUN_020394d4(puVar1 + 0x35);
    if ((puVar1[0xD] & 0x10) != 0) {
        FUN_020394a0(puVar1 + 0xF);
    }
    FUN_0203ae4c(uVar2);
    if (pcVar4 != (void (*)(u32))0) {
        pcVar4(uVar3);
    }
}

/* 0x020438d8 — fill read buffer from card */
static BOOL FUN_020438d8(int param_1)
{
    int iVar1;
    u32 uVar2;
    int iVar3;

    iVar1 = DAT_0204396c;
    uVar2 = *(u32 *)(DAT_0204396c + 0x18) & 0xFFFFFE00;
    if (uVar2 == *(u32 *)(param_1 + 8)) {
        iVar3 = *(int *)(DAT_0204396c + 0x18) - (int)uVar2;
        uVar2 = 0x200 - (u32)iVar3;
        if (*(u32 *)(DAT_0204396c + 0x20) < uVar2) {
            uVar2 = *(u32 *)(DAT_0204396c + 0x20);
        }
        FUN_0203b9a8(param_1 + 0x20 + iVar3, *(u32 *)(DAT_0204396c + 0x1c), uVar2);
        *(u32 *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + uVar2;
        *(u32 *)(iVar1 + 0x1c) = *(int *)(iVar1 + 0x1c) + uVar2;
        *(u32 *)(iVar1 + 0x20) = *(int *)(iVar1 + 0x20) - uVar2;
    }
    return *(int *)(iVar1 + 0x20) != 0;
}

/* 0x02043970 — SPI device init/reset with retry */
BOOL FUN_02043970(u32 *param_1, int param_2, int param_3)
{
    int iVar1;
    u32 uVar2;
    u32 uVar3;

    if ((param_1[0xD] & 2) == 0) {
        param_1[0xD] = param_1[0xD] | 2;
        iVar1 = FUN_0203d5cc(0xB, 1);
        while (iVar1 == 0) {
            FUN_0203aea8(100);
            iVar1 = FUN_0203d5cc(0xB, 1);
        }
        FUN_02043970(param_1, 0, 1);
    }
    FUN_0203a04c(*param_1, 0x40);
    FUN_0203a070();
    do {
        param_1[1] = (u32)param_2;
        param_1[0xD] = param_1[0xD] | 0x20;
        do {
            iVar1 = FUN_0203d518(0xB, param_2, 1);
        } while (iVar1 < 0);
        if (param_2 == 0) {
            uVar3 = *param_1;
            do {
                iVar1 = FUN_0203d518(0xB, uVar3, 1);
            } while (iVar1 < 0);
        }
        uVar3 = FUN_0203ae38();
        uVar2 = param_1[0xD];
        while ((uVar2 & 0x20) != 0) {
            FUN_02039548(0);
            uVar2 = param_1[0xD];
        }
        FUN_0203ae4c(uVar3);
    } while ((*(int *)*param_1 == 4) && (param_3 = param_3 + -1, 0 < param_3));
    return *(int *)*param_1 == 0;
}

/* 0x02043aac — wait for SPI transfer completion */
static void FUN_02043aac(u32 param_1, u32 param_2)
{
    int iVar1;

    iVar1 = FUN_0203d518(0xE, param_1, 0);
    if (iVar1 == 0) {
        return;
    }
    do {
        FUN_02004228(param_2);
        iVar1 = FUN_0203d518(0xE, param_1, 0);
    } while (iVar1 != 0);
}

/* 0x02043b10 — power management interrupt handler */
static void FUN_02043b10(void)
{
    if ((int)(*DAT_02043b4c & 0x8000) >> 0xF != 0) {
        FUN_020417d0();
    }
    FUN_02043aac(1, 1);
    FUN_0203aeb4();
}

/* 0x02043b50 — SPI interrupt callback */
void FUN_02043b50(u32 param_1, u32 param_2)
{
    int iVar1;
    void (*pcVar2)(void);

    if ((param_2 & 0x3F) != 0x11) {
        FUN_0203aeb4();
        return;
    }
    pcVar2 = (void (*)(void))*DAT_02043bb4;
    iVar1 = 1;
    *DAT_02043bb8 = 1;
    if (pcVar2 != (void (*)(void))0) {
        iVar1 = ((int (*)(void))pcVar2)();
    }
    if (iVar1 != 0) {
        FUN_02043b10();
    }
}

/* 0x02043bbc — init SPI interrupt handler */
void FUN_02043bbc(void)
{
    thunk_FUN_0203d660();
    FUN_0203d5f4(0xE, DAT_02043bec);
    *DAT_02043bf0 = 0;
}

/* 0x02043c58 — restore SPI bus lock */
static void FUN_02043c58(u32 param_1, int *param_2)
{
    if (*param_2 == 0) {
        FUN_02038fd4();
    }
    FUN_0203ae4c((u32)param_2[1]);
}

/* 0x02043c80 — acquire SPI bus lock */
static void FUN_02043c80(u32 param_1, u32 *param_2)
{
    u32 uVar1;
    u32 uVar2;
    int iVar3;

    uVar1 = DAT_02043cfc;
    while (TRUE) {
        uVar2 = FUN_0203ae38();
        param_2[1] = uVar2;
        uVar2 = FUN_02038f14(uVar1);
        *param_2 = uVar2 & 0x40;
        if (*param_2 != 0) {
            return;
        }
        iVar3 = FUN_02038fb4(param_1);
        if (iVar3 == 0)
            break;
        FUN_0203ae4c(param_2[1]);
        FUN_02004228(1);
    }
}

/* 0x02043d00 — restore POWCNT settings */
static void FUN_02043d00(int *param_1)
{
    u16 *puVar1;

    puVar1 = DAT_02043d30;
    *DAT_02043d30 = (u16)((*DAT_02043d30 & 0xFFF3) | (u16)(*param_1 << 2));
    *puVar1 = (u16)((*puVar1 & 0xFFEF) | (u16)(param_1[1] << 4));
}

/* 0x02043d34 — save and set POWCNT settings */
static void FUN_02043d34(int *param_1)
{
    u16 *puVar1;

    puVar1 = DAT_02043d78;
    *param_1 = (int)(*DAT_02043d78 & 0xC) >> 2;
    param_1[1] = (int)(*puVar1 & 0x10) >> 4;
    *puVar1 = (u16)((*puVar1 & 0xFFF3) | 0xC);
    *puVar1 = (u16)(*puVar1 & 0xFFEF);
}

/* 0x02043d7c — validate card header */
u32 FUN_02043d7c(void)
{
    u32 uVar1;
    u8 auStack_18[8];
    u8 auStack_10[8];

    uVar1 = 1;
    if (*DAT_02043e88 == DAT_02043e8c) {
        return 0;
    }
    if (-1 < (int)((u32)(*(u8 *)((int)DAT_02043e88 + 5)) << 0x1E)) {
        FUN_02043c80(*(u16 *)(DAT_02043e90 + 2), (u32 *)auStack_18);
        FUN_02043d34((int *)auStack_10);
        if ((((DAT_080000b2 == (u8)(-0x6A)) &&
              (*DAT_02043e88 != _DAT_080000be)) ||
             ((DAT_080000b2 != (u8)(-0x6A) &&
               (*DAT_02043e88 != *DAT_02043e94)))) ||
            ((*(int *)(DAT_02043e88 + 4) != (int)_DAT_080000ac &&
              ((int)((u32)(*(u8 *)((int)DAT_02043e88 + 5)) << 0x1F) < 0)))) {
            uVar1 = 0;
            *(u8 *)((int)DAT_02043e88 + 5) =
                *(u8 *)((int)DAT_02043e88 + 5) | 2;
        }
        FUN_02043d00((int *)auStack_10);
        FUN_02043c58(*(u16 *)(DAT_02043e90 + 2), (int *)auStack_18);
        return uVar1;
    }
    return 0;
}

/* 0x02043e98 — check card presence */
u32 FUN_02043e98(u32 param_1)
{
    u32 uVar1;

    uVar1 = 0;
    if ((*DAT_02043ed0 != DAT_02043ed4) && (*DAT_02043ed0 != 0) &&
        ((~(u32)(u8)*DAT_02043ed0 & param_1) != 0)) {
        uVar1 = 1;
    }
    return uVar1;
}

/* 0x02043f10 — init SPI channel */
void FUN_02043f10(void)
{
    u32 local_8[2];

    local_8[0] = 0;
    FUN_0200445a(local_8, DAT_02043f48, DAT_02043f4c);
    *(u16 *)(DAT_02043f48 + 2) = FUN_02038e7c();
}

/* 0x02043f80 — SPI interrupt handler with callback */
void FUN_02043f80(u32 param_1, u32 param_2)
{
    int iVar1;

    if ((param_2 & 0x3F) != 0x11) {
        FUN_0203aeb4();
        return;
    }
    iVar1 = 0;
    if ((void *)*DAT_02043fdc != (void *)0) {
        iVar1 = ((int (*)(void))*DAT_02043fdc)();
    }
    if (iVar1 != 0) {
        FUN_02043f50();
    }
}

/* 0x02043fe0 — card completion interrupt handler */
void FUN_02043fe0(u32 param_1, u32 param_2)
{
    if ((param_2 & 0x3F) != 1) {
        FUN_0203aeb4();
        return;
    }
    *DAT_02044018 = 1;
}

/* 0x0204401c — power management init sequence */
void FUN_0204401c(void)
{
    u16 uVar1;
    u16 uVar2;
    s16 sVar3;
    u16 uVar4;
    u16 *puVar5;
    s16 *psVar6;
    u16 *puVar7;
    u32 uVar8;
    u32 uVar9;
    u32 uVar10;
    int iVar11;
    int iVar12;
    int iVar13;
    u8 auStack_28[8];
    u8 auStack_20[12];

    puVar5 = DAT_020441c4;
    if (*DAT_020441c0 != 0) {
        return;
    }
    *DAT_020441c0 = 1;
    if ((*puVar5 & 1) != 0) {
        uVar10 = FUN_02038d00(0x40000);
        psVar6 = DAT_020441cc;
        uVar1 = *DAT_020441c8;
        *DAT_020441c8 = 1;
        FUN_02043c80((u32)psVar6[1], (u32 *)auStack_28);
        uVar2 = *DAT_020441d0;
        FUN_02043d34((int *)auStack_20);
        iVar11 = DAT_020441d4 + 0x80;
        *DAT_020441d0 = (u16)(*DAT_020441d0 & 0x7FFF);
        FUN_0203a014(iVar11, 0x40);
        FUN_0203b53c(1, DAT_020441d8, DAT_020441d4 + 0x80, 0x40);
        *DAT_020441d0 = (u16)(*DAT_020441d0 & 0x7FFF |
                        (u16)(((int)(uVar2 & 0x8000) >> 0xF) << 0xF));
        FUN_02043d00((int *)auStack_20);
        FUN_02043c58((u32)DAT_020441cc[1], (int *)auStack_28);
        iVar11 = DAT_020441d4;
        *DAT_020441dc = *(u16 *)(DAT_020441d4 + 0xBE);
        iVar12 = 0;
        do {
            iVar13 = iVar12 + 1;
            *(u8 *)(iVar12 + 0x27FFC32) =
                *(u8 *)(iVar11 + iVar12 + 0xB5);
            uVar8 = DAT_020441e0;
            puVar7 = DAT_020441dc;
            iVar12 = iVar13;
        } while (iVar13 < 3);
        DAT_020441dc[3] = *(u16 *)(iVar11 + 0xB0);
        uVar9 = DAT_020441e4;
        *(u32 *)(puVar7 + 4) = *(u32 *)(iVar11 + 0xAC);
        FUN_0203b7f0(uVar8, uVar9, 0x9C);
        FUN_02039fe0();
        FUN_02043bf4((DAT_020441d4 + 0xFE000000U >> 5) << 6 | 1);
        psVar6 = DAT_020441cc;
        sVar3 = *DAT_020441cc;
        while (sVar3 != 1) {
            FUN_02004228(1);
            sVar3 = *psVar6;
        }
        uVar4 = *DAT_020441c8;
        *DAT_020441c8 = uVar1;
        FUN_02038d00(uVar10, uVar4);
    }
}
