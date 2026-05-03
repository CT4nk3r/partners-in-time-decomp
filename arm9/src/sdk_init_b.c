/**
 * SDK Init B — card access, ROM tables, hardware init (0x02041xxx-0x02043xxx)
 * Decompiled from 0x02041abc-0x02043330 region (23 functions)
 * Note: FUN_02042c58 has no Ghidra pseudocode available (22 implemented)
 */
#include "types.h"

/* ------------------------------------------------------------------ */
/*  External function declarations                                     */
/* ------------------------------------------------------------------ */
extern void FUN_02041400(...);
extern void FUN_02041728(...);
extern void FUN_02041de8(...);
extern u32  FUN_020392d8(...);
extern void FUN_020392e0(...);
extern u32  FUN_0203ae38(...);
extern void FUN_0203ae4c(...);
extern void FUN_0203aeb4(...);
extern void FUN_02039548(...);
extern void FUN_020394a0(...);
extern void FUN_020394d4(...);
extern void FUN_0203960c(...);
extern int  FUN_020425a0(...);
extern int  FUN_020425b0(...);
extern void FUN_020426ac(...);
extern int  FUN_020426e8(...);
extern void FUN_0203a014(...);
extern void FUN_0203a04c(...);
extern void FUN_0203a070(...);
extern u32  FUN_0203a51c(...);
extern void FUN_0203a514(...);
extern void FUN_0203b808(...);
extern void FUN_0203b914(...);
extern void FUN_0203b9a8(...);
extern void FUN_0203b2c8(...);
extern int  FUN_0203d5cc(...);
extern void FUN_0203d5f4(...);
extern void thunk_FUN_0203d660(...);
extern int  FUN_02043598(...);
extern int  FUN_02043970(...);
extern void FUN_02043464(...);

/* ------------------------------------------------------------------ */
/*  External data references (DAT_ labels from Ghidra)                 */
/* ------------------------------------------------------------------ */
extern vu32 *DAT_02041b5c;
extern u32   DAT_02041b60;
extern u32  *DAT_02041b64;
extern u32   DAT_02041b68;
extern u32   DAT_02041b6c;
extern u32   DAT_02041b70;

extern u32  *DAT_02041c54;
extern u32  *DAT_02041c58;
extern int   DAT_02041c5c;
extern int   DAT_02041c4c;
extern int   DAT_02041c50;

extern int  *DAT_02041d74;

extern u32  *DAT_02041de4;

extern int  *DAT_02042414;
extern u32  *DAT_02042418;
extern u32  *DAT_0204241c;
extern u16  *DAT_02042420;

extern u32   DAT_02042460;
extern int   DAT_02042464;

extern int  *DAT_020424e0;

extern int  *DAT_02042560;
extern u32  *DAT_02042564;
extern u32   DAT_02042568;

extern u32  *DAT_020426a8;

extern u32  *DAT_020427e8;
extern u32   DAT_020427ec;
extern int  *DAT_020427f0;
extern u32   DAT_020427f4;
extern u32   DAT_020427f8;
extern u32   DAT_020427fc;
extern u32   DAT_02042800;
extern u32   DAT_02042804;
extern u32   DAT_02042808;

extern int   DAT_020429d4;
extern int   DAT_020429d8;

extern int   DAT_02042a2c;
extern int   DAT_02042a7c;

extern int  *DAT_02042c48;
extern u32   DAT_02042c4c;
extern u32   DAT_02042c50;
extern u32   DAT_02042c54;

extern int  *DAT_02042d44;
extern int   DAT_02042d48;

extern void (*DAT_02043338)(void);

/* ------------------------------------------------------------------ */
/*  Forward declarations for functions in this file                    */
/* ------------------------------------------------------------------ */
u32  FUN_02041abc(u32 param_1, u32 param_2);
void FUN_02041b74(u32 param_1, u32 param_2, int param_3);
void FUN_02041d24(u32 param_1);
u32  FUN_02041d98(void);
int  FUN_02041e10(u32 param_1);
void FUN_02041e94(u32 param_1, u32 param_2, int param_3);
u32  FUN_02042424(u32 param_1);
u32  FUN_02042468(int param_1, int param_2, int param_3);
void FUN_020424e4(void);
BOOL FUN_02042650(void);
void FUN_020426bc(void);
void FUN_020426f8(void);
void FUN_0204294c(u32 param_1);
void FUN_020429dc(void);
void FUN_02042a30(int param_1, u32 param_2, int param_3);
void FUN_02042a80(u32 param_1);
BOOL FUN_02042c64(u32 param_1);
void FUN_02042fbc(int *param_1);
void FUN_0204309c(int *param_1);
void FUN_0204317c(int *param_1);
void FUN_02043238(int param_1);
void thunk_FUN_02042650(void);

/* ================================================================== */
/*  Function implementations (address order)                           */
/* ================================================================== */

// FUN_02041abc @ 0x02041abc (160 bytes)
u32 FUN_02041abc(u32 param_1, u32 param_2)
{
    u32 uVar1;
    u32 *puVar2;
    int iVar3;

    iVar3 = FUN_02041d98();
    uVar1 = DAT_02041b60;
    if (iVar3 != 0) {
        *DAT_02041b5c = 0;
        FUN_02041728(uVar1);
        puVar2 = DAT_02041b64;
        do {
        } while (*DAT_02041b5c == 0);
        *DAT_02041b5c = 0;
        *puVar2 = 0;
        FUN_02041400(0, 2, 0, 0);
        FUN_02041728(param_1 & 0xff | DAT_02041b68);
        FUN_02041728(param_2 & DAT_02041b6c | DAT_02041b70);
        return 0;
    }
    return 1;
}

// FUN_02041b74 @ 0x02041b74 (216 bytes)
void FUN_02041b74(u32 param_1, u32 param_2, int param_3)
{
    u32 uVar1;
    u32 uVar2;
    u16 *puVar3;

    if (param_3 == 0) {
        uVar2 = param_2 & 0xff;
        uVar1 = ((param_2 & 0x7f00) << 8) >> 0x10;
        if ((uVar1 < 0x70) || (0x73 < uVar1)) {
            if (uVar1 == 0x60) {
                *DAT_02041c54 = 1;
            }
            else if (uVar1 == 0x62) {
                *DAT_02041c58 = 1;
            }
            else if (uVar1 == 0x67) {
                if (*(u32 **)(DAT_02041c5c + 0xc) != (u32 *)0x0) {
                    **(u32 **)(DAT_02041c5c + 0xc) = uVar2;
                }
                uVar2 = 0;
            }
        }
        else {
            puVar3 = *(u16 **)(DAT_02041c4c + (uVar1 - 0x70) * 8);
            if (puVar3 != (u16 *)0x0) {
                *puVar3 = (u16)uVar2;
            }
            *(u16 *)(DAT_02041c50 + (uVar1 - 0x70) * 8) = 1;
            uVar2 = 0;
        }
        FUN_02041d24(uVar2);
        return;
    }
    FUN_02041d24(2);
    return;
}

// FUN_02041d24 @ 0x02041d24 (80 bytes)
void FUN_02041d24(u32 param_1)
{
    int *piVar1;
    int iVar2;
    void (*pcVar3)(u32, int);

    piVar1 = DAT_02041d74;
    pcVar3 = (void (*)(u32, int))DAT_02041d74[1];
    if (*DAT_02041d74 != 0) {
        *DAT_02041d74 = 0;
    }
    iVar2 = piVar1[2];
    if (pcVar3 != (void (*)(u32, int))0x0) {
        DAT_02041d74[1] = 0;
        pcVar3(param_1, iVar2);
        return;
    }
    return;
}

// FUN_02041d98 @ 0x02041d98 (76 bytes)
u32 FUN_02041d98(void)
{
    FUN_0203ae38();
    if (*DAT_02041de4 != 0) {
        FUN_0203ae4c();
        return 0;
    }
    *DAT_02041de4 = 1;
    FUN_0203ae4c();
    return 1;
}

// FUN_02041e10 @ 0x02041e10 (132 bytes) — BCD to integer conversion
int FUN_02041e10(u32 param_1)
{
    u32 uVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar3 = 0;
    iVar2 = 0;
    uVar1 = 0;
    do {
        if (9 < (param_1 >> (uVar1 & 0xff) & 0xf)) {
            return 0;
        }
        iVar2 = iVar2 + 1;
        uVar1 = uVar1 + 4;
    } while (iVar2 < 8);
    iVar4 = 0;
    uVar1 = 0;
    iVar2 = 1;
    do {
        iVar3 = iVar2 * (param_1 >> (uVar1 & 0xff) & 0xf) + iVar3;
        iVar4 = iVar4 + 1;
        uVar1 = uVar1 + 4;
        iVar2 = iVar2 * 10;
    } while (iVar4 < 8);
    return iVar3;
}

// FUN_02041e94 @ 0x02041e94 (1404 bytes) — RTC response handler
void FUN_02041e94(u32 param_1, u32 param_2, int param_3)
{
    int *piVar1;
    u32 *puVar2;
    u16 *puVar3;
    u32 uVar4;
    u32 uVar5;
    int iVar6;
    void (*pcVar7)(u32, int);
    u32 *puVar8;
    u32 *puVar9;
    u32 uVar10;

    puVar3 = DAT_02042420;
    puVar2 = DAT_0204241c;
    piVar1 = DAT_02042414;
    if (param_3 != 0) {
        pcVar7 = (void (*)(u32, int))DAT_02042414[1];
        if (DAT_02042414[6] != 0) {
            DAT_02042414[6] = 0;
        }
        if (*DAT_02042414 != 0) {
            *DAT_02042414 = 0;
        }
        if (pcVar7 == (void (*)(u32, int))0x0) {
            return;
        }
        piVar1[1] = 0;
        pcVar7(6, piVar1[4]);
        return;
    }
    if ((param_2 & 0x7f00) == 0x3000) {
        if ((void (*)(void))DAT_02042414[7] == (void (*)(void))0x0) {
            return;
        }
        ((void (*)(void))DAT_02042414[7])();
        return;
    }
    if ((param_2 & 0xff) == 0) {
        uVar10 = 0;
        switch (DAT_02042414[5]) {
        case 0:
            puVar8 = (u32 *)DAT_02042414[2];
            uVar4 = FUN_02041e10((u8)*DAT_02042418);
            puVar2 = DAT_02042418;
            *puVar8 = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 0x13) >> 0x1b);
            puVar2 = DAT_02042418;
            puVar8[1] = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 10) >> 0x1a);
            puVar2 = DAT_02042418;
            puVar8[2] = uVar4;
            puVar8[3] = (*puVar2 << 5) >> 0x1d;
            break;
        case 1:
            puVar8 = (u32 *)DAT_02042414[2];
            uVar4 = FUN_02041e10(*DAT_0204241c & 0x3f);
            puVar2 = DAT_0204241c;
            *puVar8 = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 0x11) >> 0x19);
            puVar2 = DAT_0204241c;
            puVar8[1] = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 9) >> 0x19);
            puVar8[2] = uVar4;
            break;
        case 2:
            puVar8 = (u32 *)DAT_02042414[2];
            uVar4 = FUN_02041e10(*DAT_02042418 & 0xff);
            puVar2 = DAT_02042418;
            *puVar8 = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 0x13) >> 0x1b);
            puVar2 = DAT_02042418;
            puVar8[1] = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 10) >> 0x1a);
            puVar2 = DAT_02042418;
            puVar8[2] = uVar4;
            puVar9 = DAT_0204241c;
            puVar8[3] = (*puVar2 << 5) >> 0x1d;
            puVar8 = (u32 *)DAT_02042414[3];
            uVar4 = FUN_02041e10(*puVar9 & 0x3f);
            puVar2 = DAT_0204241c;
            *puVar8 = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 0x11) >> 0x19);
            puVar2 = DAT_0204241c;
            puVar8[1] = uVar4;
            uVar4 = FUN_02041e10((*puVar2 << 9) >> 0x19);
            puVar8[2] = uVar4;
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            if ((*DAT_02042420 & 0xf) == 4) {
                *(u32 *)DAT_02042414[2] = 1;
            }
            else {
                *(u32 *)DAT_02042414[2] = 0;
            }
            break;
        case 7:
            if ((int)((u32)*DAT_02042420 << 0x19) < 0) {
                *(u32 *)DAT_02042414[2] = 1;
            }
            else {
                *(u32 *)DAT_02042414[2] = 0;
            }
            break;
        case 8:
            puVar9 = (u32 *)DAT_02042414[2];
            *puVar9 = *DAT_0204241c & 7;
            uVar5 = FUN_02041e10((*puVar2 << 0x12) >> 0x1a);
            puVar2 = DAT_0204241c;
            puVar9[1] = uVar5;
            uVar5 = FUN_02041e10((*puVar2 << 9) >> 0x19);
            puVar9[2] = uVar5;
            puVar2 = DAT_0204241c;
            puVar9[3] = 0;
            if ((int)(*puVar2 << 0x18) < 0) {
                puVar9[3] = puVar9[3] + 1;
            }
            if ((int)(*DAT_0204241c << 0x10) < 0) {
                puVar9[3] = puVar9[3] + 2;
            }
            if ((int)(*DAT_0204241c << 8) < 0) {
                puVar9[3] = puVar9[3] + 4;
            }
            break;
        case 9:
            if (DAT_02042414[6] == 0) {
                if (*(int *)DAT_02042414[2] == 1) {
                    if ((*DAT_02042420 & 0xf) != 4) {
                        DAT_02042414[6] = 1;
                        *puVar3 = *puVar3 & 0xfff0 | 4;
                        iVar6 = FUN_020425a0();
                        if (iVar6 == 0) {
                            uVar10 = 3;
                            DAT_02042414[6] = 0;
                        }
                    }
                }
                else if ((*DAT_02042420 & 0xf) != 0) {
                    DAT_02042414[6] = 1;
                    *puVar3 = *puVar3 & 0xfff0;
                    iVar6 = FUN_020425a0();
                    if (iVar6 == 0) {
                        uVar10 = 3;
                        DAT_02042414[6] = 0;
                    }
                }
            }
            else {
                DAT_02042414[6] = 0;
            }
            break;
        case 10:
            if (DAT_02042414[6] == 0) {
                if (*(int *)DAT_02042414[2] == 1) {
                    if (-1 < (int)((u32)*DAT_02042420 << 0x19)) {
                        DAT_02042414[6] = 1;
                        *puVar3 = *puVar3 | 0x40;
                        iVar6 = FUN_020425a0();
                        if (iVar6 == 0) {
                            uVar10 = 3;
                            DAT_02042414[6] = 0;
                        }
                    }
                }
                else if ((int)((u32)*DAT_02042420 << 0x19) < 0) {
                    DAT_02042414[6] = 1;
                    *puVar3 = *puVar3 & 0xffbf;
                    iVar6 = FUN_020425a0();
                    if (iVar6 == 0) {
                        uVar10 = 3;
                        DAT_02042414[6] = 0;
                    }
                }
            }
            else {
                DAT_02042414[6] = 0;
            }
            break;
        case 0xb:
            break;
        case 0xc:
            break;
        case 0xd:
            break;
        case 0xe:
            break;
        case 0xf:
            break;
        default:
            DAT_02042414[6] = 0;
            uVar10 = 4;
        }
    }
    else {
        DAT_02042414[6] = 0;
        switch (param_2 & 0xff) {
        case 1:
            uVar10 = 4;
            break;
        case 2:
            uVar10 = 5;
            break;
        case 3:
            uVar10 = 1;
            break;
        case 4:
        default:
            uVar10 = 6;
        }
    }
    piVar1 = DAT_02042414;
    if (DAT_02042414[6] == 0) {
        pcVar7 = (void (*)(u32, int))DAT_02042414[1];
        if (*DAT_02042414 != 0) {
            *DAT_02042414 = 0;
        }
        if (pcVar7 == (void (*)(u32, int))0x0) {
            return;
        }
        piVar1[1] = 0;
        pcVar7(uVar10, piVar1[4]);
        return;
    }
    return;
}

// FUN_02042424 @ 0x02042424 (60 bytes)
u32 FUN_02042424(u32 param_1)
{
    int iVar1;

    iVar1 = FUN_02042468(param_1, DAT_02042460, 0);
    *(int *)(DAT_02042464 + 0x20) = iVar1;
    if (iVar1 == 0) {
        FUN_02041de8();
    }
    return *(u32 *)(DAT_02042464 + 0x20);
}

// FUN_02042468 @ 0x02042468 (120 bytes)
u32 FUN_02042468(int param_1, int param_2, int param_3)
{
    int *piVar1;
    int iVar2;
    u32 uVar3;

    FUN_0203ae38();
    if (*DAT_020424e0 == 0) {
        *DAT_020424e0 = 1;
        FUN_0203ae4c();
        piVar1 = DAT_020424e0;
        DAT_020424e0[5] = 1;
        piVar1[6] = 0;
        piVar1[2] = param_1;
        piVar1[1] = param_2;
        piVar1[4] = param_3;
        iVar2 = FUN_020425b0();
        if (iVar2 == 0) {
            uVar3 = 3;
        }
        else {
            uVar3 = 0;
        }
        return uVar3;
    }
    FUN_0203ae4c();
    return 1;
}

// FUN_020424e4 @ 0x020424e4 (124 bytes)
void FUN_020424e4(void)
{
    u32 *puVar1;
    int iVar2;

    puVar1 = DAT_02042564;
    if (*DAT_02042560 == 0) {
        *DAT_02042560 = 1;
        *puVar1 = 0;
        puVar1[1] = 0;
        puVar1[7] = 0;
        puVar1[2] = 0;
        puVar1[3] = 0;
        thunk_FUN_0203d660();
        do {
            iVar2 = FUN_0203d5cc(5, 1);
        } while (iVar2 == 0);
        FUN_0203d5f4(5, DAT_02042568);
        return;
    }
    return;
}

// FUN_02042650 @ 0x02042650 (88 bytes)
BOOL FUN_02042650(void)
{
    u32 *puVar1;
    u32 uVar2;

    puVar1 = DAT_020426a8;
    uVar2 = FUN_0203ae38();
    if ((puVar1[0xd] & 4) != 0) {
        do {
            FUN_02039548(puVar1 + 0x35);
        } while ((puVar1[0xd] & 4) != 0);
    }
    FUN_0203ae4c(uVar2);
    return *(int *)*puVar1 == 0;
}

// FUN_020426bc @ 0x020426bc (44 bytes)
void FUN_020426bc(void)
{
    int iVar1;

    iVar1 = FUN_020426e8();
    if (iVar1 == 0) {
        FUN_0203aeb4();
        return;
    }
    return;
}

// FUN_020426f8 @ 0x020426f8 (240 bytes) — card ROM init
void FUN_020426f8(void)
{
    u32 *puVar1;
    u32 uVar2;
    u32 uVar3;

    puVar1 = DAT_020427e8;
    DAT_020427e8[2] = 0xfffffffd;
    puVar1[3] = 0;
    uVar3 = DAT_020427ec;
    puVar1[5] = 0;
    *puVar1 = uVar3;
    FUN_0203b808(0, uVar3, 0x40);
    FUN_0203a04c(DAT_020427ec, 0x40);
    uVar2 = FUN_0203a51c();
    FUN_0203a514(0);
    uVar3 = DAT_020427f8;
    if (*DAT_020427f0 != 2) {
        uVar3 = DAT_020427fc;
    }
    FUN_0203b9a8(uVar3, DAT_020427f4, 0x160);
    FUN_0203a514(uVar2);
    *(u16 *)(puVar1 + 4) = 0;
    *(u16 *)(puVar1 + 0x35) = 0;
    puVar1[0xe] = 4;
    FUN_0203960c(puVar1 + 0xf, DAT_02042800, 0, DAT_02042804, 0x400, puVar1[0xe]);
    FUN_020394a0(puVar1 + 0xf);
    FUN_0203d5f4(0xb, DAT_02042808);
    if (*DAT_020427f0 == 2) {
        return;
    }
    FUN_020426ac(1);
    return;
}

// FUN_0204294c @ 0x0204294c (136 bytes)
void FUN_0204294c(u32 param_1)
{
    int iVar1;
    u32 uVar2;

    iVar1 = DAT_020429d4;
    uVar2 = *(u32 *)(DAT_020429d4 + 0x38);
    if (uVar2 == 0x20) {
        uVar2 = FUN_020392d8(*(u32 *)(DAT_020429d8 + 8));
        if (uVar2 != 0) {
            uVar2 = uVar2 - 1;
        }
    }
    else if ((uVar2 == 0x21) &&
            (uVar2 = FUN_020392d8(*(u32 *)(DAT_020429d8 + 8)), uVar2 < 0x1f)) {
        uVar2 = uVar2 + 1;
    }
    FUN_020392e0(iVar1 + 0x3c, uVar2);
    *(int *)(iVar1 + 0xd0) = iVar1 + 0x3c;
    *(u32 *)(iVar1 + 0x30) = param_1;
    *(u32 *)(iVar1 + 0x34) = *(u32 *)(iVar1 + 0x34) | 8;
    FUN_020394a0();
    return;
}

// FUN_020429dc @ 0x020429dc (80 bytes) — worker thread loop
void FUN_020429dc(void)
{
    int iVar1;
    u32 uVar2;
    int iVar3;

    iVar1 = DAT_02042a2c;
    iVar3 = DAT_02042a2c + 0x3c;
    do {
        uVar2 = FUN_0203ae38();
        while ((*(u32 *)(iVar1 + 0x34) & 8) == 0) {
            *(int *)(iVar1 + 0xd0) = iVar3;
            FUN_02039548(0);
        }
        FUN_0203ae4c(uVar2);
        (*(void (**)(int))(iVar1 + 0x30))(iVar1);
    } while (TRUE);
}

// FUN_02042a30 @ 0x02042a30 (76 bytes)
void FUN_02042a30(int param_1, u32 param_2, int param_3)
{
    int iVar1;

    iVar1 = DAT_02042a7c;
    if (param_1 != 0xb) {
        return;
    }
    if (param_3 != 0) {
        *(u32 *)(DAT_02042a7c + 0x34) = *(u32 *)(DAT_02042a7c + 0x34) & 0xffffffdf;
        FUN_020394a0(*(u32 *)(iVar1 + 0xd0));
        return;
    }
    return;
}

// FUN_02042a80 @ 0x02042a80 (456 bytes) — configure card parameters
void FUN_02042a80(u32 param_1)
{
    int iVar1;
    u32 uVar2;
    u32 uVar3;
    int iVar4;

    iVar4 = *DAT_02042c48;
    FUN_0203b914(iVar4 + 0x18, 0, 0x28);
    *(u32 *)(iVar4 + 4) = param_1;
    if (param_1 == 0) {
        return;
    }
    iVar1 = 1 << ((int)param_1 >> 8 & 0xffU);
    param_1 = param_1 & 0xff;
    *(int *)(iVar4 + 0x18) = iVar1;
    uVar2 = DAT_02042c50;
    if (param_1 == 1) {
        if (iVar1 == 0x200) {
            *(u32 *)(iVar4 + 0x20) = 0x10;
            *(u32 *)(iVar4 + 0x24) = 1;
            *(u32 *)(iVar4 + 0x28) = 0x4f;
            return;
        }
        if (iVar1 == 0x2000) {
            *(u32 *)(iVar4 + 0x20) = 0x20;
            *(u32 *)(iVar4 + 0x24) = 2;
            *(u32 *)(iVar4 + 0x28) = 0x4f;
            return;
        }
        if (iVar1 == 0x10000) {
            *(u32 *)(iVar4 + 0x20) = 0x80;
            *(u32 *)(iVar4 + 0x24) = 2;
            *(u32 *)(iVar4 + 0x28) = 0x9e;
            return;
        }
    }
    else {
        if (param_1 == 2) {
            if (iVar1 == 0x40000) {
                *(u32 *)(iVar4 + 0x2c) = DAT_02042c4c;
                *(u32 *)(iVar4 + 0x30) = uVar2;
                uVar3 = DAT_02042c54;
                *(u32 *)(iVar4 + 0x3c) = uVar2;
                *(u32 *)(iVar4 + 0x38) = uVar3;
                *(u32 *)(iVar4 + 0x34) = uVar3;
            }
            else {
                if ((iVar1 != 0x80000) && (iVar1 != 0x100000)) goto LAB_02042c20;
                *(u32 *)(iVar4 + 0x2c) = DAT_02042c4c;
                uVar2 = DAT_02042c50;
                *(u32 *)(iVar4 + 0x30) = 0;
                uVar3 = DAT_02042c54;
                *(u32 *)(iVar4 + 0x3c) = uVar2;
                *(u32 *)(iVar4 + 0x38) = uVar3;
            }
            *(u32 *)(iVar4 + 0x1c) = 0x10000;
            *(u32 *)(iVar4 + 0x20) = 0x100;
            *(u32 *)(iVar4 + 0x24) = 3;
            *(u32 *)(iVar4 + 0x28) = 0x4f;
            return;
        }
        if ((param_1 == 3) && ((iVar1 == 0x2000 || (iVar1 == 0x8000)))) {
            *(int *)(iVar4 + 0x20) = iVar1;
            *(u32 *)(iVar4 + 0x24) = 2;
            return;
        }
    }
LAB_02042c20:
    *(u32 *)(iVar4 + 4) = 0;
    *(u32 *)(iVar4 + 0x18) = 0;
    *(u32 *)*DAT_02042c48 = 3;
    return;
}

// FUN_02042c58 @ 0x02042c58 — no Ghidra pseudocode available

// FUN_02042c64 @ 0x02042c64 (224 bytes)
BOOL FUN_02042c64(u32 param_1)
{
    int *piVar1;
    u32 uVar2;

    piVar1 = DAT_02042d44;
    FUN_020426bc();
    uVar2 = FUN_0203ae38();
    if ((piVar1[0xd] & 4U) != 0) {
        do {
            FUN_02039548(piVar1 + 0x35);
        } while ((piVar1[0xd] & 4U) != 0);
    }
    piVar1[0xd] = piVar1[0xd] | 4;
    FUN_0203ae4c(uVar2);
    FUN_02042a80(param_1);
    piVar1[10] = 0;
    piVar1[0xb] = 0;
    DAT_02042d44[0x34] = *(int *)(DAT_02042d48 + 8);
    FUN_02043970(piVar1, 2, 1);
    *(u32 *)(*piVar1 + 0xc) = 0;
    *(int **)(*piVar1 + 0x10) = piVar1 + 0x38;
    *(u32 *)(*piVar1 + 0x14) = 1;
    FUN_02043970(piVar1, 6);
    FUN_02043238((int)piVar1);
    return *(int *)*piVar1 == 0;
}

// FUN_02042fbc @ 0x02042fbc (224 bytes) — card read transfer
void FUN_02042fbc(int *param_1)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;

    piVar2 = param_1 + 0x38;
    do {
        uVar3 = param_1[8];
        if (0x100 < uVar3) {
            uVar3 = 0x100;
        }
        FUN_0203b9a8(param_1[6], piVar2, uVar3);
        FUN_0203a04c(piVar2, uVar3);
        FUN_0203a070();
        *(int **)(*param_1 + 0xc) = piVar2;
        *(int *)(*param_1 + 0x10) = param_1[7];
        *(u32 *)(*param_1 + 0x14) = uVar3;
        iVar1 = FUN_02043970(param_1, 7, 10);
        if ((iVar1 == 0) || (iVar1 = FUN_02043970(param_1, 9, 1), iVar1 == 0)) break;
        param_1[6] = param_1[6] + uVar3;
        param_1[7] = param_1[7] + uVar3;
        param_1[8] = param_1[8] - uVar3;
    } while (param_1[8] != 0);
    FUN_02043238((int)param_1);
    return;
}

// FUN_0204309c @ 0x0204309c (224 bytes) — card write transfer
void FUN_0204309c(int *param_1)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;

    piVar2 = param_1 + 0x38;
    do {
        uVar3 = param_1[8];
        if (0x100 < uVar3) {
            uVar3 = 0x100;
        }
        FUN_0203b9a8(param_1[6], piVar2, uVar3);
        FUN_0203a04c(piVar2, uVar3);
        FUN_0203a070();
        *(int **)(*param_1 + 0xc) = piVar2;
        *(int *)(*param_1 + 0x10) = param_1[7];
        *(u32 *)(*param_1 + 0x14) = uVar3;
        iVar1 = FUN_02043970(param_1, 8, 10);
        if ((iVar1 == 0) || (iVar1 = FUN_02043970(param_1, 9, 1), iVar1 == 0)) break;
        param_1[6] = param_1[6] + uVar3;
        param_1[7] = param_1[7] + uVar3;
        param_1[8] = param_1[8] - uVar3;
    } while (param_1[8] != 0);
    FUN_02043238((int)param_1);
    return;
}

// FUN_0204317c @ 0x0204317c (188 bytes) — card page read
void FUN_0204317c(int *param_1)
{
    int iVar1;
    int *piVar2;
    u32 uVar3;

    piVar2 = param_1 + 0x38;
    do {
        uVar3 = param_1[8];
        if (0x100 < uVar3) {
            uVar3 = 0x100;
        }
        FUN_0203a014(piVar2, 0x100);
        *(int *)(*param_1 + 0xc) = param_1[6];
        *(int **)(*param_1 + 0x10) = piVar2;
        *(u32 *)(*param_1 + 0x14) = uVar3;
        iVar1 = FUN_02043970(param_1, 6, 1);
        if (iVar1 == 0) break;
        FUN_0203b9a8(piVar2, param_1[7], uVar3);
        param_1[6] = param_1[6] + uVar3;
        param_1[7] = param_1[7] + uVar3;
        param_1[8] = param_1[8] - uVar3;
    } while (param_1[8] != 0);
    FUN_02043238((int)param_1);
    return;
}

// FUN_02043238 @ 0x02043238 (112 bytes) — card operation completion
void FUN_02043238(int param_1)
{
    u32 uVar1;
    u32 uVar2;
    void (*pcVar3)(u32);

    pcVar3 = *(void (**)(u32))(param_1 + 0x28);
    uVar2 = *(u32 *)(param_1 + 0x2c);
    uVar1 = FUN_0203ae38();
    *(u32 *)(param_1 + 0x34) = *(u32 *)(param_1 + 0x34) & 0xfffffff3;
    FUN_020394d4(param_1 + 0xd4);
    if ((*(u32 *)(param_1 + 0x34) & 0x10) != 0) {
        FUN_020394a0(param_1 + 0x3c);
    }
    FUN_0203ae4c(uVar1);
    if (pcVar3 != (void (*)(u32))0x0) {
        pcVar3(uVar2);
        return;
    }
    return;
}

// thunk_FUN_02042650 @ 0x02043330 (8 bytes) — indirect jump to FUN_02042650
void thunk_FUN_02042650(void)
{
    DAT_02043338();
}
