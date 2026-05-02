/**
 * Miscellaneous Functions
 *
 * Various utility, DMA, sound, palette, and cleanup routines
 * that don't fit into other translation units.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/* Extern data references */
extern u32 DAT_020079a4;
extern u32 *DAT_020079a8;
extern u32 DAT_020079ac;
extern u32 DAT_020079b0;
extern char *DAT_020073bc;
extern u32 *DAT_020073c0;
extern char *DAT_020073c4;
extern u16 *DAT_020073c8;
extern u16 *DAT_020073cc;
extern char *DAT_02007478;
extern u32 *DAT_0200747c;
extern u32 *DAT_02007480;
extern char *DAT_02007524;
extern u32 *DAT_02007528;
extern u32 *DAT_0200752c;
extern u16 *DAT_0200758c;
extern char *DAT_02007584;
extern u32 *DAT_02007588;
extern u32 *DAT_02007618;
extern u32 *DAT_0200761c;
extern u8 *DAT_02007620;
extern char *DAT_02007624;
extern u32 DAT_02007628;
extern u32 *DAT_020077b4;
extern u32 *DAT_0200780c;
extern u32 DAT_02059e04;
extern u32 *DAT_02059e08;
extern u32 *DAT_02059e0c;
extern u32 DAT_02059e10;
extern u32 *DAT_02059e14;
extern u32 DAT_02059e18;
extern u32 *DAT_02059f2c;
extern u32 DAT_02059f30;
extern u32 *DAT_02059f34;
extern u8 *DAT_02059f38;
extern u32 *DAT_02059f3c;
extern u8 *DAT_02059f40;
extern u8 *DAT_02059f44;
extern u8 *DAT_02059f48;
extern u8 *DAT_02059f4c;
extern u8 *DAT_02059f50;
extern u8 *DAT_02059f54;
extern u8 *DAT_02059f58;
extern u8 *DAT_02059f5c;
extern u32 *DAT_0205a020;
extern u32 DAT_0205a024;
extern u32 *DAT_0205a028;
extern u32 *DAT_0205a02c;
extern u32 *DAT_0205a030;
extern u32 *DAT_0205a034;
extern u32 *DAT_0205a038;
extern u32 *DAT_0205a03c;
extern u32 *DAT_0205a078;
extern u32 *DAT_0205a07c;
extern u32 DAT_0205a114;
extern u32 *DAT_0205a0d8;
extern u32 DAT_0205a0dc;
extern u32 DAT_0205a1d8;
extern u32 DAT_0205a184;
extern u32 DAT_0205a188;
extern u32 DAT_0205a280;
extern u32 DAT_0205a284;
extern u32 DAT_0205a27c;
extern u32 _DAT_04000208;
extern u32 DAT_040000b0;
extern void (*DAT_02007640)(...);
extern u32 DAT_02007644;

/* Extern function declarations */
extern void FUN_0202e0d8(...);
extern int FUN_0202e078(...);
extern void FUN_0202e088(...);
extern void FUN_0202e010(...);
extern int FUN_0202e274(...);
extern void FUN_0203a88c(...);
extern void FUN_0203acc0(...);
extern void FUN_0202cc94(...);
extern void FUN_0202cd68(...);
extern void FUN_02029ab8(...);
extern u32 func_0x0209c9f8(...);
extern void func_0x0209ca0c(...);
extern void func_0x0209cd64(...);
extern void func_0x0209bb74(...);
extern void func_0x0209bb68(...);
extern void func_0x0209bc3c(...);
extern void func_0x0209bc30(...);

/* Forward declarations for functions defined in this file */
void FUN_02059e30(u32 param_1, int param_2, int param_3);
static u32 FUN_02059e1c(u32 param_1, u32 *param_2, int param_3);
void FUN_02059d6c(void);
void FUN_02059f60(void);
void FUN_0205a18c(int param_1, u32 param_2, u32 param_3, u32 param_4);
void FUN_02007530(u32 param_1, u16 param_2);
void FUN_0200762c(void);
void FUN_02007810(int param_1, int param_2);

// FUN_0202f100 @ 0x0202F100 (8 bytes) — Thunk to FUN_02029ab8 (free)
void FUN_0202f100(u32 param_1)
{
    FUN_02029ab8(param_1);
}

// FUN_02038e70 @ 0x02038E70 (12 bytes) — Thunk stub
void FUN_02038e70(void)
{
    return;
}

// FUN_020072d4 @ 0x020072D4 (232 bytes) — Periodic save check / autosave timer
void FUN_020072d4(void)
{
#ifdef HOST_PORT
    /* HOST_PORT: dereferences several DAT_02007xxx pointers that arm9.bin
     * statically initialises to NDS RAM addresses pointing into a sound
     * system / save manager struct. That struct is populated by sound init
     * code we have not yet wired up on the host, so the pointer at
     * *DAT_020073c0 leads to uninitialised bytes — `testb $0x40, 0x514(%eax)`
     * faults at a non-canonical address. Skip the autosave tick entirely
     * until the sound subsystem is online. */
    return;
#else
    char cVar1;
    u16 uVar2;
    char *pcVar3;
    u16 *puVar4;
    int iVar5;

    if ((*DAT_020073bc == '\0') ||
       ((*DAT_020073c0 != 0 && (-1 < (int)((u32)*(u8 *)(*DAT_020073c0 + 0x514) << 0x19))))) {
        *DAT_020073c4 = '\0';
        return;
    }
    cVar1 = *DAT_020073c4;
    iVar5 = FUN_0202e078();
    pcVar3 = DAT_020073c4;
    *DAT_020073c4 = iVar5 != 0;
    if (*pcVar3 == '\0') {
        return;
    }
    if (cVar1 == '\0') {
        *DAT_020073c8 = 0;
    }
    puVar4 = DAT_020073c8;
    uVar2 = *DAT_020073cc;
    *DAT_020073c8 = *DAT_020073c8 + 1;
    if (((uVar2 == 0) || (*puVar4 < uVar2)) && (*DAT_020073c8 < 600)) {
        return;
    }
    if (*DAT_020073bc == '\0') {
        return;
    }
    FUN_0202e088();
    return;
#endif /* HOST_PORT */
}

// FUN_020073d0 @ 0x020073D0 (168 bytes) — Play sound from table with volume param
void FUN_020073d0(int param_1, u16 param_2)
{
    u32 *puVar1;
    u32 uStack_48;
    u32 uStack_44;
    u32 uStack_40;
    u32 uStack_3c;
    u32 uStack_38;
    u32 uStack_34;
    u32 uStack_30;
    u32 uStack_2c;
    u32 uStack_28;
    u32 uStack_24;
    u32 uStack_20;
    u32 uStack_1c;
    u32 uStack_18;
    u32 uStack_14;
    u32 uStack_10;

    if (*DAT_02007478 == '\0') {
        return;
    }
    if ((*DAT_0200747c != 0) && (-1 < (int)((u32)*(u8 *)(*DAT_0200747c + 0x514) << 0x19))) {
        return;
    }
    puVar1 = (u32 *)(param_1 * 0x3c + *DAT_02007480);
    uStack_48 = *puVar1;
    uStack_44 = puVar1[1];
    uStack_40 = puVar1[2];
    uStack_3c = puVar1[3];
    uStack_38 = puVar1[4];
    uStack_34 = puVar1[5];
    uStack_30 = puVar1[6];
    uStack_2c = puVar1[7];
    uStack_28 = puVar1[8];
    uStack_24 = puVar1[9];
    uStack_20 = puVar1[10];
    uStack_1c = puVar1[0xb];
    uStack_18 = puVar1[0xc];
    uStack_14 = puVar1[0xd];
    uStack_10 = 0;
    FUN_02007530(&uStack_48, param_2);
    return;
}

// FUN_02007484 @ 0x02007484 (160 bytes) — Play sound from table with optional duration
void FUN_02007484(int param_1, int param_2)
{
    u32 *puVar1;
    u32 uStack_48;
    u32 uStack_44;
    u32 uStack_40;
    u32 uStack_3c;
    u32 uStack_38;
    u32 uStack_34;
    u32 uStack_30;
    u32 uStack_2c;
    u32 uStack_28;
    u32 uStack_24;
    u32 uStack_20;
    u32 uStack_1c;
    u32 uStack_18;
    u32 uStack_14;
    int iStack_10;

    if (*DAT_02007524 == '\0') {
        return;
    }
    if ((*DAT_02007528 != 0) && (-1 < (int)((u32)*(u8 *)(*DAT_02007528 + 0x514) << 0x19))) {
        return;
    }
    puVar1 = (u32 *)(param_1 * 0x3c + *DAT_0200752c);
    uStack_48 = *puVar1;
    uStack_44 = puVar1[1];
    uStack_40 = puVar1[2];
    uStack_3c = puVar1[3];
    uStack_38 = puVar1[4];
    uStack_34 = puVar1[5];
    uStack_30 = puVar1[6];
    uStack_2c = puVar1[7];
    uStack_28 = puVar1[8];
    uStack_24 = puVar1[9];
    uStack_20 = puVar1[10];
    uStack_1c = puVar1[0xb];
    uStack_18 = puVar1[0xc];
    uStack_14 = puVar1[0xd];
    iStack_10 = puVar1[0xe];
    if (param_2 != 0) {
        iStack_10 = param_2;
    }
    FUN_02007530(&uStack_48, 0);
    return;
}

// FUN_02007530 @ 0x02007530 (84 bytes) — Submit sound request
void FUN_02007530(u32 param_1, u16 param_2)
{
    if (*DAT_02007584 == '\0') {
        return;
    }
    if ((*DAT_02007588 != 0) && (-1 < (int)((u32)*(u8 *)(*DAT_02007588 + 0x514) << 0x19))) {
        return;
    }
    FUN_0202e0d8();
    *DAT_0200758c = param_2;
    return;
}

// FUN_020075ac @ 0x020075AC (108 bytes) — Initialize sound system state
void FUN_020075ac(void)
{
    u32 *puVar1;
    u8 *puVar2;
    char *pcVar3;
    int iVar4;

    puVar2 = DAT_02007620;
    puVar1 = DAT_0200761c;
    *DAT_02007618 = 0;
    *puVar1 = 0;
    *puVar2 = 0;
    FUN_0203a88c();
    FUN_0203acc0();
    iVar4 = FUN_0202e274();
    pcVar3 = DAT_02007624;
    *DAT_02007624 = iVar4 != 0;
    if (*pcVar3 == '\0') {
        return;
    }
    FUN_0202e010(DAT_02007628);
    return;
}

// FUN_0200762c @ 0x0200762C (20 bytes) — Clear sound state via indirect call
void FUN_0200762c(void)
{
    DAT_02007640(DAT_02007644, 0, 0x38);
    return;
}

// FUN_02007648 @ 0x02007648 (364 bytes) — Cleanup / free game resources
void FUN_02007648(void)
{
    u8 bVar1;
    int *piVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iStack_28;

    if (*DAT_020077b4 << 0x1f < 0) {
        FUN_02029ab8(DAT_020077b4[1]);
        if (DAT_020077b4[2] != 0) {
            FUN_02029ab8(*(u32 *)(DAT_020077b4[2] + 0x4104));
            FUN_02029ab8(*(u32 *)(DAT_020077b4[2] + 0x4100));
            FUN_02029ab8(DAT_020077b4[2]);
        }
        FUN_02029ab8(DAT_020077b4[3]);
        piVar2 = (int *)DAT_020077b4;
        iVar6 = 0;
        iStack_28 = 0;
        do {
            bVar1 = *(u8 *)(piVar2[iVar6 + 10] + 0x2b22);
            iVar5 = iStack_28;
            if (bVar1 != 0) {
                do {
                    iVar4 = *(int *)(piVar2[iVar6 + 10] + iVar5 * 4 + 0x29d8);
                    if (((u32)*(u16 *)(iVar4 + 10) << 0x1a) >> 0x1c < 2) {
                        iVar3 = 0;
                        do {
                            FUN_02029ab8(*(u32 *)(iVar4 + iVar3 * 4 + 0x578));
                            iVar3 = iVar3 + 1;
                        } while (iVar3 < 6);
                    }
                    FUN_02029ab8(iVar4);
                    iVar5 = iVar5 + 1;
                } while (iVar5 < (int)(u32)bVar1);
            }
            if (iVar6 == 1) {
                FUN_02029ab8(*(u32 *)(piVar2[0xb] + 0x24f8));
                FUN_02029ab8(*(u32 *)(piVar2[0xb] + 0x24e0));
                FUN_02029ab8(*(u32 *)(piVar2[0xb] + 0x24e4));
            }
            FUN_02029ab8(piVar2[iVar6 + 10]);
            FUN_02029ab8(piVar2[iVar6 + 0xc]);
            iVar6 = iVar6 + 1;
        } while (iVar6 < 2);
        FUN_0200762c();
        return;
    }
    return;
}

// FUN_020077b8 @ 0x020077B8 (84 bytes) — Adjust palette brightness
void FUN_020077b8(u32 param_1)
{
    u32 uVar1;

    uVar1 = *DAT_0200780c;
    if (uVar1 == 0) {
        return;
    }
    if (param_1 <= uVar1) {
        FUN_0202cc94(uVar1, param_1, 0x324);
        return;
    }
    FUN_0202cd68(uVar1, param_1, 0x324);
    return;
}

// FUN_02007810 @ 0x02007810 (108 bytes) — Copy palette block (16 entries x 16 rows)
void FUN_02007810(int param_1, int param_2)
{
    u16 uVar1;
    u16 uVar2;
    u16 uVar3;
    u16 uVar4;
    u16 uVar5;
    u16 uVar6;
    u16 uVar7;
    u16 *puVar8;
    u16 *puVar9;
    int iVar10;

    iVar10 = 0;
    puVar8 = (u16 *)(param_1 + 6);
    puVar9 = (u16 *)(param_2 + 6);
    do {
        uVar1 = puVar8[4];
        uVar2 = puVar8[8];
        uVar3 = puVar8[0xc];
        uVar4 = puVar8[0x10];
        uVar5 = puVar8[0x14];
        uVar6 = puVar8[0x18];
        uVar7 = puVar8[0x1c];
        *puVar9 = *puVar8;
        puVar9[4] = uVar1;
        puVar9[8] = uVar2;
        puVar9[0xc] = uVar3;
        puVar9[0x10] = uVar4;
        puVar9[0x14] = uVar5;
        puVar9[0x18] = uVar6;
        puVar9[0x1c] = uVar7;
        puVar8 = puVar8 + 0x20;
        puVar9 = puVar9 + 0x20;
        iVar10 = iVar10 + 1;
    } while (iVar10 < 0x10);
    return;
}

// FUN_0200787c @ 0x0200787C (296 bytes) — Apply palette transform with overlay
void FUN_0200787c(int param_1, u32 param_2, u32 param_3)
{
    u8 bVar1;
    u32 uVar2;
    u32 uVar3;
    int iVar4;
    int iVar5;
    u32 uVar6;
    u32 uVar7;

    uVar6 = (u32)*(u8 *)(DAT_020079a4 + param_1);
    uVar7 = *DAT_020079a8;
    if (uVar6 != 0) {
        if (param_2 < uVar7) {
            FUN_0202cd68(param_2, uVar7, uVar6 << 3);
        }
        else {
            FUN_0202cc94(param_2, uVar7, uVar6 << 3);
        }
    }
    bVar1 = *(u8 *)(DAT_020079ac + param_1);
    iVar5 = DAT_020079b0 + param_1 * 0x400;
    iVar4 = 0;
    if (bVar1 != 0) {
        do {
            uVar3 = uVar7 + uVar6 * 8;
            uVar2 = param_2 + (u32)*(u8 *)(iVar5 + 4) * 8;
            if (uVar2 < uVar3) {
                FUN_0202cd68(uVar2, uVar3, (u32)*(u8 *)(iVar5 + 5) << 3);
            }
            else {
                FUN_0202cc94(uVar2, uVar3, (u32)*(u8 *)(iVar5 + 5) << 3);
            }
            iVar4 = iVar4 + 1;
            uVar6 = uVar6 + *(u8 *)(iVar5 + 5);
            iVar5 = iVar5 + 8;
        } while (iVar4 < (int)(u32)bVar1);
    }
    if (uVar6 != 0x80) {
        uVar3 = uVar7 + uVar6 * 8;
        uVar2 = param_2 + uVar6 * 8;
        if (uVar2 < uVar3) {
            FUN_0202cd68(uVar2, uVar3, (0x80 - uVar6) * 8);
        }
        else {
            FUN_0202cc94(uVar2, uVar3, (0x80 - uVar6) * 8);
        }
    }
    FUN_02007810(param_2, uVar7);
    if (uVar7 < param_3) {
        FUN_0202cd68(uVar7, param_3, 0x400);
        return;
    }
    FUN_0202cc94(uVar7, param_3, 0x400);
    return;
}

// FUN_02059d6c @ 0x02059D6C (152 bytes) — GX command submission / display list flush
void FUN_02059d6c(void)
{
    u32 uVar1;
    u32 *puVar2;
    u32 uVar3;

    _DAT_04000208 = 0x4000000;
    *(u32 *)(DAT_02059e04 + 0x3ffc) = 0;
    do {
    } while ((*DAT_02059e08 & 0xf) != 1);
    *DAT_02059e08 = 0x100;
    uVar3 = *DAT_02059e0c;
    puVar2 = DAT_02059e0c;
    uVar1 = (u32)FUN_02059e1c(0, DAT_02059e10, 0x80);
    *puVar2 = uVar3;
    FUN_02059e1c(uVar1, DAT_02059e14, 0x80);
    do {
    } while ((*DAT_02059e08 & 0xf) == 1);
    *DAT_02059e08 = 0;
    (*(void (**)(...))((u8 *)DAT_02059e18 + 0x24))(*DAT_02059e14, DAT_02059e14[1], DAT_02059e14[2], DAT_02059e14[3]);
    return;
}

// FUN_02059e1c @ 0x02059E1C (20 bytes) — Fill memory with u32 value
static u32 FUN_02059e1c(u32 param_1, u32 *param_2, int param_3)
{
    int end;
    end = (int)param_2 + param_3;
    for (; (int)param_2 < end; param_2 = param_2 + 1) {
        *param_2 = param_1;
    }
    return (u32)param_2;
}

// FUN_02059e30 @ 0x02059E30 (252 bytes) — SPI flash read into buffer
void FUN_02059e30(u32 param_1, int param_2, int param_3)
{
    u32 *puVar1;
    u32 *puVar2;
    u8 *puVar3;
    u8 *puVar4;
    u32 uVar5;
    u32 uVar6;
    u32 uVar7;
    int iVar8;

    uVar5 = *DAT_02059f2c;
    iVar8 = -(param_1 & DAT_02059f30);
    do {
    } while ((*DAT_02059f34 & 0x80000000) != 0);
    *DAT_02059f38 = 0x80;
    puVar4 = DAT_02059f44;
    puVar3 = DAT_02059f40;
    puVar2 = DAT_02059f3c;
    puVar1 = DAT_02059f34;
    param_1 = param_1 + iVar8;
    if (iVar8 < param_3) {
        uVar6 = param_1 >> 8;
        do {
            *puVar3 = 0xb7;
            *puVar4 = (u8)(param_1 >> 0x18);
            *DAT_02059f48 = (u8)(param_1 >> 0x10);
            *DAT_02059f4c = (u8)uVar6;
            *DAT_02059f50 = (u8)param_1;
            *DAT_02059f54 = 0;
            *DAT_02059f58 = 0;
            *DAT_02059f5c = 0;
            *puVar1 = uVar5 & 0xf8ffffff | 0xa1000000;
            do {
                uVar7 = *puVar1;
                if ((uVar7 & 0x800000) != 0) {
                    if ((-1 < iVar8) && (iVar8 < param_3)) {
                        *(u32 *)(param_2 + iVar8) = *puVar2;
                    }
                    iVar8 = iVar8 + 4;
                }
            } while ((uVar7 & 0x80000000) != 0);
            uVar6 = uVar6 + 2;
            param_1 = param_1 + 0x200;
        } while (iVar8 < param_3);
        return;
    }
    return;
}

// FUN_02059f60 @ 0x02059F60 (192 bytes) — VRAM bank setup and transfer
void FUN_02059f60(void)
{
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    u32 uVar8;
    u32 uVar9;

    uVar9 = *DAT_0205a020;
    if (0x7fff < uVar9) {
        FUN_02059e30(uVar9, DAT_0205a024, 0x160);
    }
    iVar7 = *DAT_0205a028;
    iVar6 = *DAT_0205a02c;
    iVar5 = *DAT_0205a030;
    iVar4 = *DAT_0205a034;
    uVar3 = *DAT_0205a038;
    uVar2 = *DAT_0205a03c;
    uVar1 = (u32)func_0x0209c9f8();
    func_0x0209bb74();
    func_0x0209bb68();
    func_0x0209ca0c(uVar1);
    func_0x0209bc3c();
    func_0x0209bc30();
    uVar8 = iVar7 + uVar9;
    if (uVar8 < 0x8000) {
        iVar6 = iVar6 + (0x8000 - uVar8);
        iVar5 = iVar5 - (0x8000 - uVar8);
        uVar8 = 0x8000;
    }
    FUN_02059e30(uVar8, iVar6, iVar5);
    FUN_02059e30(iVar4 + uVar9, uVar3, uVar2);
    return;
}

// FUN_0205a040 @ 0x0205A040 (56 bytes) — Wait for VBLANK then flush GX
void FUN_0205a040(void)
{
    do {
    } while (*DAT_0205a078 == 0);
    *DAT_0205a07c = 0;
    FUN_02059f60();
    FUN_02059d6c();
    return;
}

// FUN_0205a080 @ 0x0205A080 (88 bytes) — Set DMA channel params (with channel 0 special case)
u32 FUN_0205a080(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 *puVar1;
    u32 *puVar2;

    puVar2 = DAT_0205a0d8;
    *(u32 *)((u8 *)&DAT_040000b0 + param_1 * 0xc) = param_2;
    puVar2 = puVar2 + param_1 * 3;
    puVar2[1] = param_3;
    puVar2[2] = param_4;
    puVar1 = DAT_0205a0d8;
    if (param_1 == 0) {
        *puVar2 = 0;
        puVar2[1] = 0;
        puVar1 = DAT_0205a0d8;
        puVar2[2] = DAT_0205a0dc;
    }
    return *puVar1;
}

// FUN_0205a0e0 @ 0x0205A0E0 (52 bytes) — Set DMA channel registers
void FUN_0205a0e0(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    int iVar1;

    iVar1 = DAT_0205a114;
    *(u32 *)((u8 *)&DAT_040000b0 + param_1 * 0xc) = param_2;
    iVar1 = param_1 * 0xc + iVar1;
    *(u32 *)(iVar1 + 4) = param_3;
    *(u32 *)(iVar1 + 8) = param_4;
    return;
}

// FUN_0205a118 @ 0x0205A118 (108 bytes) — Set DMA channel with IRQ disable (channel 0 special)
void FUN_0205a118(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    int iVar1;
    u32 uVar2;
    u32 *puVar3;

    func_0x0209c9f8();
    iVar1 = DAT_0205a184;
    *(u32 *)((u8 *)&DAT_040000b0 + param_1 * 0xc) = param_2;
    puVar3 = (u32 *)(param_1 * 0xc + iVar1);
    puVar3[1] = param_3;
    puVar3[2] = param_4;
    if (param_1 == 0) {
        *puVar3 = 0;
        uVar2 = DAT_0205a188;
        puVar3[1] = 0;
        puVar3[2] = uVar2;
    }
    func_0x0209ca0c();
    return;
}

// FUN_0205a18c @ 0x0205A18C (76 bytes) — Set DMA channel registers with IRQ disable
void FUN_0205a18c(int param_1, u32 param_2, u32 param_3, u32 param_4)
{
    int iVar1;

    func_0x0209c9f8();
    iVar1 = DAT_0205a1d8;
    *(u32 *)((u8 *)&DAT_040000b0 + param_1 * 0xc) = param_2;
    iVar1 = param_1 * 0xc + iVar1;
    *(u32 *)(iVar1 + 4) = param_3;
    *(u32 *)(iVar1 + 8) = param_4;
    func_0x0209ca0c();
    return;
}

// FUN_0205a1dc @ 0x0205A1DC (160 bytes) — Chunked DMA transfer with size limit
void FUN_0205a1dc(int param_1, int param_2, u32 param_3)
{
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 *puVar4;

    if (param_3 == 0) {
        return;
    }
    func_0x0209cd64();
    uVar2 = DAT_0205a284;
    uVar1 = DAT_0205a280;
    puVar4 = (u32 *)(DAT_0205a27c + (param_1 * 3 + 2) * 4);
    do {
    } while ((*puVar4 & 0x80000000) != 0);
    for (; param_3 != 0; param_3 = param_3 - uVar3) {
        uVar3 = param_3;
        if (0x1d8 < param_3) {
            uVar3 = 0x1d8;
        }
        FUN_0205a18c(param_1, param_2, uVar1, uVar2 | uVar3 >> 2);
        param_2 = param_2 + uVar3;
    }
    do {
    } while ((*puVar4 & 0x80000000) != 0);
    return;
}
