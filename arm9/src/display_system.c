/**
 * Display System — screen configuration, VBlank processing, layer management
 *
 * Handles OAM affine matrix updates, layer rendering dispatch (per BG mode),
 * BG control register configuration, and tilemap fill operations.
 *
 * Decompiled from 0x02018d14-0x0201b568 region (24 functions)
 */
#include "types.h"
#ifdef HOST_PORT
#include <stdio.h>
#endif

/* Forward declarations */
extern void FUN_0203a04c(u32, u32);
extern void FUN_02038144(u32, u32, u32);
extern void FUN_02038074(u32, u32, u32);
extern void FUN_02037fa4(u32, u32, u32);
extern void FUN_02037ed4(u32, u32, u32);
extern void FUN_020381ac(u32, u32, u32);
extern void FUN_020380dc(u32, u32, u32);
extern void FUN_0203800c(u32, u32, u32);
extern void FUN_02037f3c(u32, u32, u32);
extern void FUN_02038484(u32, u32, u16);
extern void FUN_020383b4(u32, u32, u16);
extern void FUN_020382e4(u32, u32, u16);
extern void FUN_02038214(u32, u32, u16);
extern void FUN_020384ec(u32, u32, u16);
extern void FUN_0203841c(u32, u32, u16);
extern void FUN_0203834c(u32, u32, u16);
extern void FUN_0203827c(u32, u32, u16);

// FUN_0201ae04 @ 0x0201ae04 (516 bytes) — Render layer to framebuffer (dispatch by BG mode)
void FUN_0201ae04(int param_1, int param_2)
{
    FUN_0203a04c(*(u32 *)(param_2 + 0x74), (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10);
    if (*(int *)(param_2 + 8) << 0x1f < 0) {
        switch ((u32)(*(int *)(param_1 + 0x50) << 0xe) >> 0x1e) {
        case 0: FUN_02038144(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 1: FUN_02038074(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 2: FUN_02037fa4(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 3: FUN_02037ed4(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        }
    } else {
        switch ((u32)(*(int *)(param_1 + 0x50) << 0x10) >> 0x1e) {
        case 0: FUN_020381ac(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 1: FUN_020380dc(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 2: FUN_0203800c(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        case 3: FUN_02037f3c(*(u32 *)(param_2 + 0x74), ((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) << 5, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10); return;
        }
    }
    return;
}

// FUN_0201b008 @ 0x0201b008 (176 bytes) — Update OAM affine matrix for dirty entries
void FUN_0201b008(int param_1)
{
    int iVar1;
    int iVar2;

    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) & 0xfffffff7;
    iVar2 = *(int *)(param_1 + 0xf10);
    iVar1 = *(int *)(iVar2 + 4);
    while (iVar1 != -1) {
        if ((int)(*(u32 *)(iVar2 + 0x68) << 0x1f) < 0) {
            *(u32 *)(iVar2 + 0x68) = *(u32 *)(iVar2 + 0x68) & 0xfffffffe;
            FUN_0201ae04(param_1, iVar2);
        }
        iVar2 = *(int *)(iVar2 + 4);
        iVar1 = *(int *)(iVar2 + 4);
    }
    iVar2 = *(int *)(param_1 + 0xf20);
    if (*(int *)(iVar2 + 4) == -1) {
        return;
    }
    do {
        if ((int)(*(u32 *)(iVar2 + 0x68) << 0x1f) < 0) {
            *(u32 *)(iVar2 + 0x68) = *(u32 *)(iVar2 + 0x68) & 0xfffffffe;
            FUN_0201ae04(param_1, iVar2);
        }
        iVar2 = *(int *)(iVar2 + 4);
    } while (*(int *)(iVar2 + 4) != -1);
    return;
}

// FUN_0201b0b8 @ 0x0201b0b8 (168 bytes) — Clear layer rendering flag (sub screen)
void FUN_0201b0b8(int param_1)
{
    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) & 0xfffffffb;
    FUN_0203a04c(*(u32 *)(param_1 + 0x38), 0x780);
    switch ((u32)(*(int *)(param_1 + 0x50) << 0xe) >> 0x1e) {
    case 0: FUN_02038484(*(u32 *)(param_1 + 0x38), 0x80, *(u16 *)(param_1 + 0x4e)); return;
    case 1: FUN_020383b4(*(u32 *)(param_1 + 0x38), 0x80, *(u16 *)(param_1 + 0x4e)); return;
    case 2: FUN_020382e4(*(u32 *)(param_1 + 0x38), 0x80, *(u16 *)(param_1 + 0x4e)); return;
    case 3: FUN_02038214(*(u32 *)(param_1 + 0x38), 0x80, *(u16 *)(param_1 + 0x4e)); return;
    }
    return;
}

// FUN_0201b160 @ 0x0201b160 (168 bytes) — Clear affine transformation flag (main screen)
void FUN_0201b160(int param_1)
{
    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) & 0xfffffffd;
    FUN_0203a04c(*(u32 *)(param_1 + 0x34), 0x780);
    switch ((u32)(*(int *)(param_1 + 0x50) << 0x10) >> 0x1e) {
    case 0: FUN_020384ec(*(u32 *)(param_1 + 0x34), 0x80, *(u16 *)(param_1 + 0x4c)); return;
    case 1: FUN_0203841c(*(u32 *)(param_1 + 0x34), 0x80, *(u16 *)(param_1 + 0x4c)); return;
    case 2: FUN_0203834c(*(u32 *)(param_1 + 0x34), 0x80, *(u16 *)(param_1 + 0x4c)); return;
    case 3: FUN_0203827c(*(u32 *)(param_1 + 0x34), 0x80, *(u16 *)(param_1 + 0x4c)); return;
    }
    return;
}

// FUN_0201b418 @ 0x0201b418 (336 bytes) — Fill tilemap with palette index
void FUN_0201b418(int param_1, int param_2)
{
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    int iVar4;
    int iVar5;
    u32 uVar6;
    u32 uVar7;
    u32 uVar8;

    uVar1 = (*(u32 *)(param_2 + 0x10) & 0x1ff) << 0xd;
    uVar7 = ((*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17) << 0xd;
    uVar2 = uVar1 >> 0x10;
    uVar3 = uVar7 >> 0x10;
    if (*(int *)(param_2 + 8) << 0x1f < 0) {
        iVar5 = *(int *)(param_1 + 0x38);
        uVar6 = *(int *)(param_1 + 0x50) << 2;
    } else {
        iVar5 = *(int *)(param_1 + 0x34);
        uVar6 = *(int *)(param_1 + 0x50) << 6;
    }
    uVar6 = (((u32)(*(int *)(param_2 + 0x68) << 6) >> 0x11) + (uVar6 >> 0x1c) * 0x1000) & 0xffff;
    uVar7 = (u32)(s16)(uVar7 >> 0x10);
    if (uVar7 < uVar3 + ((u32)(*(int *)(param_2 + 0x10) << 1) >> 0x1a) + 1) {
        iVar4 = uVar7 << 5;
        uVar1 = (u32)(s16)(uVar1 >> 0x10);
        do {
            uVar8 = uVar1;
            if (uVar1 < uVar2 + ((u32)(*(int *)(param_2 + 0x10) << 7) >> 0x1a) + 1) {
                do {
                    *(s16 *)(iVar5 + (iVar4 + uVar8) * 2) = (s16)uVar6;
                    uVar8 = (int)((uVar8 + 1) * 0x10000) >> 0x10;
                    uVar6 = (uVar6 + 1) & 0xffff;
                } while (uVar8 < uVar2 + ((u32)(*(int *)(param_2 + 0x10) << 7) >> 0x1a) + 1);
            }
            uVar7 = (int)((uVar7 + 1) * 0x10000) >> 0x10;
            iVar4 = iVar4 + 0x20;
        } while (uVar7 < uVar3 + ((u32)(*(int *)(param_2 + 0x10) << 1) >> 0x1a) + 1);
        return;
    }
    return;
}

/* ===== Forward declarations for appended functions ===== */
void FUN_0201abf0(int param_1);

/* External function declarations */
extern void FUN_0202a068(void);
extern void thunk_FUN_02029ab8(u32 *);
extern void FUN_0201a698(int, int, int, int);
extern void FUN_0203aeb4(void);
extern void FUN_0202916c(int, u16);
extern void FUN_0201d9c4(u32, int, int, int);
extern void FUN_02023a78(int, int, void *);
extern void FUN_02026388(u8 *, u32, int, int, int, int, u32, u32, int, int, int, char, int, int);
extern void FUN_02025810(u8 *, u8 *, int, int);
extern u16 FUN_0202609c(u32, u32, int, int, int, u32, u32, int, u32, int);
extern int FUN_020464b0(int, u32);
extern void FUN_0202cc10(int, int, int);
extern void FUN_0202cd68(u32, u32, int);
extern void FUN_0202cc94(u32, u32, int);
extern void FUN_0203b808(u32, int, int);

/* External data declarations */
extern u32 DAT_020192d4;
extern int *DAT_020192d8;
extern int DAT_020192dc;
extern u32 DAT_020192e0;
extern int DAT_020192e4;
extern u32 DAT_020192e8;
extern int DAT_020192ec;
extern int DAT_020192f0;
extern int DAT_020192f4;
#ifdef HOST_PORT
/* HOST_PORT: rewire DAT_02019730..d to host-side full-width globals
 * defined in pc/src/host_display_data_init.c. The host_undefined_stubs.c
 * versions are 4-byte uint32_t which collide adjacent slots when read
 * as 8-byte pointers on a 64-bit host. Renaming via #define lets the
 * linker pick our 8-byte definitions instead. */
#include <stdint.h>
#define DAT_02019730 g_disp_DAT_02019730
#define DAT_02019734 g_disp_DAT_02019734
#define DAT_02019738 g_disp_DAT_02019738
#define DAT_0201973c g_disp_DAT_0201973c
#define DAT_02019740 g_disp_DAT_02019740
#define DAT_02019744 g_disp_DAT_02019744
#define DAT_02019748 g_disp_DAT_02019748
#define DAT_0201974c g_disp_DAT_0201974c
#define DAT_02019750 g_disp_DAT_02019750
#define DAT_02019754 g_disp_DAT_02019754
#define DAT_02019758 g_disp_DAT_02019758
#define DAT_0201975c g_disp_DAT_0201975c
#define DAT_02019760 g_disp_DAT_02019760
#define DAT_02019764 g_disp_DAT_02019764
#define DAT_02019768 g_disp_DAT_02019768
#define DAT_0201976c g_disp_DAT_0201976c
#define DAT_02019770 g_disp_DAT_02019770
#define DAT_02019774 g_disp_DAT_02019774
#define DAT_02019778 g_disp_DAT_02019778
extern int *DAT_02019730;
extern short *DAT_02019734;
extern intptr_t DAT_02019738;
extern intptr_t DAT_0201973c;
extern intptr_t DAT_02019740;
extern intptr_t DAT_02019744;
extern short *DAT_02019748;
extern intptr_t DAT_0201974c;
extern intptr_t DAT_02019750;
extern intptr_t DAT_02019754;
extern short *DAT_02019758;
extern intptr_t DAT_0201975c;
extern intptr_t DAT_02019760;
extern intptr_t DAT_02019764;
extern intptr_t DAT_02019768;
extern short *DAT_0201976c;
extern intptr_t DAT_02019770;
extern intptr_t DAT_02019774;
extern intptr_t DAT_02019778;
#else
extern int *DAT_02019730;
extern short *DAT_02019734;
extern int DAT_02019738;
extern int DAT_0201973c;
extern int DAT_02019740;
extern int DAT_02019744;
extern short *DAT_02019748;
extern int DAT_0201974c;
extern int DAT_02019750;
extern int DAT_02019754;
extern short *DAT_02019758;
extern int DAT_0201975c;
extern int DAT_02019760;
extern int DAT_02019764;
extern int DAT_02019768;
extern short *DAT_0201976c;
extern int DAT_02019770;
extern int DAT_02019774;
extern int DAT_02019778;
#endif
extern int DAT_0201977c;
extern int DAT_02019780;
extern int DAT_02019784;
extern int DAT_02019788;
extern int DAT_0201978c;
extern int DAT_02019790;
extern u32 DAT_020197bc;
extern u32 DAT_02019b40;
extern u8 *DAT_02019e98;
extern u32 DAT_0201a154;
extern u32 DAT_0201a158;
extern u32 DAT_0201a15c;
extern u32 DAT_0201a684;
extern u32 DAT_0201a688;
extern u32 DAT_0201a68c;
extern u32 DAT_0201a690;
extern u32 DAT_0201a694;
extern int *DAT_0201abe0;
extern u32 DAT_0201abe4;
extern u32 DAT_0201abe8;
extern int DAT_0201abec;
extern u32 DAT_0201addc;
extern u32 DAT_0201ade0;
extern int *DAT_0201ade4;
extern int *DAT_0201ade8;
extern int *DAT_0201adec;
extern int *DAT_0201adf0;
extern int *DAT_0201adf4;
extern int *DAT_0201adf8;
extern int *DAT_0201adfc;
extern int *DAT_0201ae00;
extern u16 *DAT_0201b300;
extern u16 *DAT_0201b304;
extern u16 *DAT_0201b308;
extern u16 *DAT_0201b30c;
extern u16 *DAT_0201b408;
extern u16 *DAT_0201b40c;
extern u16 *DAT_0201b410;
extern u16 *DAT_0201b414;
extern u32 DAT_0201c6f4;

#define CONCAT11(hi, lo) ((u16)(((u8)(hi) << 8) | (u8)(lo)))

// FUN_0201913C @ 0x0201913C (408 bytes) - Adjust stat parameter with bounds checking
int FUN_0201913c(u32 param_1,int param_2)

{
  u16 uVar1;
  int iVar2;
  int iVar3;
  u32 uVar4;
  int iVar5;
  
  if (param_1 == DAT_020192d4) {
    iVar3 = *(int *)(*DAT_020192d8 + 0x488);
    if (DAT_020192dc < iVar3 + param_2) {
      param_2 = DAT_020192dc - iVar3;
    }
    else if (iVar3 + param_2 < 0) {
      param_2 = -iVar3;
    }
    *(int *)(*DAT_020192d8 + 0x488) = iVar3 + param_2;
  }
  else if (param_1 == DAT_020192e0) {
    uVar1 = *(u16 *)(*DAT_020192d8 + 0x48c);
    uVar4 = (u32)uVar1;
    if (DAT_020192e4 < (int)(uVar4 + param_2)) {
      param_2 = DAT_020192e4 - uVar4;
    }
    else if ((int)(uVar4 + param_2) < 0) {
      param_2 = -uVar4;
    }
    *(u16 *)(*DAT_020192d8 + 0x48c) = uVar1 + (short)param_2;
  }
  else {
    uVar4 = param_1 & 0xf000;
    iVar5 = 0;
    iVar3 = 0;
    param_1 = param_1 & DAT_020192e8;
    if (uVar4 < 0x2001) {
      if (uVar4 < 0x2000) {
        if ((uVar4 == 0x1000) && ((int)param_1 < 0xb)) {
          iVar3 = 99;
          iVar5 = *DAT_020192d8 + DAT_020192ec;
        }
      }
      else if ((int)param_1 < 0xe) {
        iVar3 = 99;
        iVar5 = *DAT_020192d8 + DAT_020192f0;
      }
    }
    else if (uVar4 < 0x3001) {
      if ((uVar4 == 0x3000) && ((int)param_1 < 0x29)) {
        iVar3 = 9;
        iVar5 = *DAT_020192d8 + 0x4e0;
      }
    }
    else if ((uVar4 == 0x4000) && ((int)param_1 < 0x21)) {
      iVar3 = 9;
      iVar5 = *DAT_020192d8 + DAT_020192f4;
    }
    if (iVar5 == 0) {
      FUN_0203aeb4();
    }
    iVar2 = (int)*(char *)(iVar5 + param_1);
    if (iVar3 < iVar2 + param_2) {
      param_2 = iVar3 - iVar2;
    }
    else if (iVar2 + param_2 < 0) {
      param_2 = -iVar2;
    }
    *(char *)(iVar5 + param_1) = *(char *)(iVar5 + param_1) + (char)param_2;
  }
  return param_2;
}

// FUN_020192F8 @ 0x020192F8 (1080 bytes) - Initialize display configuration for all 4 layers
void FUN_020192f8(int param_1)

{
  int *piVar1;
  short *psVar2;
  short *psVar3;
  short *psVar4;
  short *psVar5;
  u32 uVar6;
  int iVar7;
  short *unaff_r6;
  int unaff_r7;
  int iVar8;
  int iVar9;

#ifdef HOST_PORT
  fflush(stderr); fprintf(stderr,
    "[FUN_020192f8] enter p=%d *DAT_02019730=0x%x DAT_02019744=0x%x DAT_02019768=0x%x\n",
    param_1, (unsigned)*DAT_02019730, (unsigned)DAT_02019744, (unsigned)DAT_02019768); fflush(stderr);
#endif
  FUN_0202cc10(*DAT_02019730 + 0x3f8,0,0x100);
#ifdef HOST_PORT
  fflush(stderr); fprintf(stderr, "[FUN_020192f8] after first MI_CpuFill32Fast\n"); fflush(stderr);
#endif
  iVar8 = 0;
  do {
#ifdef HOST_PORT
    fflush(stderr); fprintf(stderr, "[FUN_020192f8] loop iVar8=%d\n", iVar8); fflush(stderr);
#endif
    psVar5 = DAT_0201976c;
    psVar4 = DAT_02019758;
    psVar3 = DAT_02019748;
    psVar2 = DAT_02019734;
    switch(iVar8) {
    case 0:
      iVar9 = *DAT_02019730;
      unaff_r7 = iVar9 + 0x3f8;
      *(u16 *)(iVar9 + 0x3f8) = 0;
      *(char *)(iVar9 + 0x410) = (char)*(u16 *)(DAT_02019738 + param_1 * 2);
      FUN_0202916c(0,*(u16 *)(DAT_0201973c + param_1 * 2));
      FUN_0202916c(0,*(u16 *)(DAT_02019740 + param_1 * 2));
      FUN_0201913c(*(u16 *)(DAT_0201973c + param_1 * 2),1);
      FUN_0201913c(*(u16 *)(DAT_02019740 + param_1 * 2),1);
      unaff_r6 = psVar2;
      break;
    case 1:
      unaff_r7 = *DAT_02019730 + DAT_02019744;
      *(u16 *)(*DAT_02019730 + 0x41c) = 1;
      *(char *)(unaff_r7 + 0x18) = (char)*(u16 *)(DAT_0201974c + param_1 * 2);
      FUN_0202916c(1,*(u16 *)(DAT_02019750 + param_1 * 2));
      FUN_0202916c(1,*(u16 *)(DAT_02019754 + param_1 * 2));
      FUN_0201913c(*(u16 *)(DAT_02019750 + param_1 * 2),1);
      FUN_0201913c(*(u16 *)(DAT_02019754 + param_1 * 2),1);
      unaff_r6 = psVar3;
      break;
    case 2:
      iVar9 = *DAT_02019730;
      unaff_r7 = iVar9 + 0x440;
      *(u16 *)(iVar9 + 0x440) = 2;
      *(char *)(iVar9 + 0x458) = (char)*(u16 *)(DAT_0201975c + param_1 * 2);
      FUN_0202916c(2,*(u16 *)(DAT_02019760 + param_1 * 2));
      FUN_0202916c(2,*(u16 *)(DAT_02019764 + param_1 * 2));
      FUN_0201913c(*(u16 *)(DAT_02019760 + param_1 * 2),1);
      FUN_0201913c(*(u16 *)(DAT_02019764 + param_1 * 2),1);
      unaff_r6 = psVar4;
      break;
    case 3:
      unaff_r7 = *DAT_02019730 + DAT_02019768;
      *(u16 *)(*DAT_02019730 + 0x464) = 3;
      *(char *)(unaff_r7 + 0x18) = (char)*(u16 *)(DAT_02019770 + param_1 * 2);
      FUN_0202916c(3,*(u16 *)(DAT_02019774 + param_1 * 2));
      FUN_0202916c(3,*(u16 *)(DAT_02019778 + param_1 * 2));
      FUN_0201913c(*(u16 *)(DAT_02019774 + param_1 * 2),1);
      FUN_0201913c(*(u16 *)(DAT_02019778 + param_1 * 2),1);
      unaff_r6 = psVar5;
    }
    iVar9 = 1;
    *(u32 *)(unaff_r7 + 0x18) = *(u32 *)(unaff_r7 + 0x18) & 0xff;
    uVar6 = (u32)*(u8 *)(unaff_r7 + 0x18);
    if (1 < uVar6) {
      do {
        uVar6 = *(u32 *)(unaff_r7 + 0x18);
        iVar7 = iVar9 + -1;
        iVar9 = iVar9 + 1;
        *(u32 *)(unaff_r7 + 0x18) =
             uVar6 & 0xff | ((u32)(u16)unaff_r6[iVar7 * 6 + 4] + (uVar6 >> 8)) * 0x100;
        uVar6 = (u32)*(u8 *)(unaff_r7 + 0x18);
      } while (iVar9 < (int)uVar6);
    }
    if (uVar6 != 0) {
      unaff_r6 = unaff_r6 + (uVar6 - 1) * 6;
    }
    iVar9 = (uVar6 - 1) * 0x80;
    iVar8 = iVar8 + 1;
    *(short *)(unaff_r7 + 2) = *unaff_r6 + (short)(iVar9 + ((u32)(iVar9 >> 7) >> 0x18) >> 8);
    iVar9 = (*(u8 *)(unaff_r7 + 0x18) - 1) * 0x80;
    *(short *)(unaff_r7 + 4) = unaff_r6[1] + (short)(iVar9 + ((u32)(iVar9 >> 7) >> 0x18) >> 8);
    iVar9 = (*(u8 *)(unaff_r7 + 0x18) - 1) * 0x80;
    *(short *)(unaff_r7 + 6) = unaff_r6[2] + (short)(iVar9 + ((u32)(iVar9 >> 7) >> 0x18) >> 8);
    iVar9 = (*(u8 *)(unaff_r7 + 0x18) - 1) * 0x80;
    *(short *)(unaff_r7 + 8) = unaff_r6[3] + (short)(iVar9 + ((u32)(iVar9 >> 7) >> 0x18) >> 8);
    *(short *)(unaff_r7 + 10) = unaff_r6[5];
    *(u32 *)(unaff_r7 + 0x1c) = *(u32 *)(unaff_r7 + 0x1c) & 0xff000000 | (u32)(u16)unaff_r6[4]
    ;
    *(u16 *)(unaff_r7 + 0xc) = *(u16 *)(unaff_r7 + 2);
    *(u16 *)(unaff_r7 + 0x10) = *(u16 *)(unaff_r7 + 4);
    *(u16 *)(unaff_r7 + 0x12) = *(u16 *)(unaff_r7 + 6);
    *(u16 *)(unaff_r7 + 0x14) = *(u16 *)(unaff_r7 + 8);
    *(u16 *)(unaff_r7 + 0x16) = *(u16 *)(unaff_r7 + 10);
    *(u16 *)(unaff_r7 + 0xe) = *(u16 *)(unaff_r7 + 0xc);
    *(u8 *)(unaff_r7 + 0x21) = 0;
    *(u8 *)(unaff_r7 + 0x22) = 0;
    *(u8 *)(unaff_r7 + 0x23) = 0;
    iVar9 = DAT_0201977c;
    piVar1 = DAT_02019730;
  } while (iVar8 < 4);
  *(u32 *)(*DAT_02019730 + 0x488) = 0;
  iVar8 = DAT_02019780;
  param_1 = param_1 * 2;
  *(u16 *)(*piVar1 + 0x48c) = 0;
  iVar7 = DAT_02019784;
  *(char *)(*piVar1 + 0x48e) = (char)*(u16 *)(iVar9 + param_1);
  iVar9 = DAT_02019788;
  *(char *)(*piVar1 + 0x48f) = (char)*(u16 *)(iVar8 + param_1);
  iVar8 = DAT_0201978c;
  *(char *)(*piVar1 + 0x490) = (char)*(u16 *)(iVar7 + param_1);
  iVar7 = DAT_02019790;
  *(u8 *)(*piVar1 + 0x491) = 0;
  *(char *)(*piVar1 + 0x49e) = (char)*(u16 *)(iVar9 + param_1);
  *(char *)(*piVar1 + 0x49f) = (char)*(u16 *)(iVar8 + param_1);
  *(char *)(*piVar1 + 0x4a0) = (char)*(u16 *)(iVar7 + param_1);
  *(u8 *)(*piVar1 + 0x4a1) = 0;
  return;
}

// FUN_02019794 @ 0x02019794 (40 bytes) - Construct display object with vtable
u32 * FUN_02019794(u32 *param_1)

{
  *param_1 = DAT_020197bc;
  FUN_0202a068();
  thunk_FUN_02029ab8(param_1);
  return param_1;
}

// FUN_02019808 @ 0x02019808 (128 bytes) - VBlank handler � process dirty flags
void FUN_02019808(int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 4);
  if (-1 < piVar1[0x14] << 0x1f) {
    return;
  }
  FUN_0201abf0((int)piVar1);
  if (piVar1[0x14] << 0x1e < 0) {
    FUN_0201b160((int)piVar1);
  }
  if (piVar1[0x14] << 0x1d < 0) {
    FUN_0201b0b8((int)piVar1);
  }
  if (piVar1[0x14] << 0x1c < 0) {
    (*(void (**)(void))(*piVar1 + 0xc))();
    return;
  }
  return;
}

// FUN_02019888 @ 0x02019888 (696 bytes) - Render text string with glyph lookup
void FUN_02019888(int param_1,int param_2)

{
  u32 uVar1;
  u8 bVar2;
  u8 bVar3;
  u8 bVar4;
  int iVar5;
  u8 *pbVar6;
  u32 uVar7;
  u32 uVar8;
  u8 *pbVar9;
  u32 uVar10;
  int iVar11;
  u32 uVar12;
  int iVar13;
  int local_40;
  u8 local_28;
  
  uVar7 = 0;
  iVar11 = 0x1000;
  uVar12 = *(u32 *)(param_2 + 0x2c) >> 0x1c;
  local_40 = 0x1000;
  uVar8 = (u32)*(u8 *)(param_2 + 100);
  bVar2 = *(u8 *)(param_2 + 0x65);
  pbVar6 = (u8 *)(((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xd4 +
                    *(int *)(param_1 + 0xf04) + 8);
  while ((pbVar9 = pbVar6, bVar3 = *pbVar9, bVar3 != 0xff || (pbVar9[1] != 0xff))) {
    if (bVar3 == 0xff) {
      pbVar6 = pbVar9 + 2;
      switch(pbVar9[1]) {
      case 0:
        uVar8 = (u32)*pbVar6;
        pbVar6 = pbVar9 + 3;
        break;
      case 1:
        bVar2 = *pbVar6;
        pbVar6 = pbVar9 + 3;
        break;
      case 2:
        uVar12 = (u32)*pbVar6;
        pbVar6 = pbVar9 + 3;
        break;
      case 3:
        iVar11 = (int)*(short *)pbVar6;
        pbVar6 = pbVar9 + 4;
        break;
      case 4:
        local_40 = (int)*(short *)pbVar6;
        pbVar6 = pbVar9 + 4;
        break;
      case 5:
        uVar7 = (u32)*pbVar6;
        pbVar6 = pbVar9 + 3;
      }
    }
    else if (bVar3 == 0x20) {
      uVar8 = uVar8 + ((u32)(*(int *)(param_2 + 0x30) << 0x17) >> 0x1b) +
                      ((u32)(*(int *)(param_2 + 0x44) << 0xb) >> 0x1c) & 0xff;
      pbVar6 = pbVar9 + 1;
    }
    else {
      bVar4 = 0;
      if ((u32)(*(int *)(param_2 + 0x30) << 1) >> 0x18 == 0) {
        if ((u8)(-bVar3 - 1) < 6) {
          pbVar9 = pbVar9 + 1;
          bVar4 = -bVar3 - 1;
        }
        else {
          bVar4 = 0;
        }
      }
      uVar1 = (u32)CONCAT11(bVar4,*pbVar9);
      iVar13 = *(int *)(param_2 + 0x38);
      if (uVar1 < 0x100) {
        iVar5 = *(int *)(iVar13 + uVar7 * 8);
        uVar10 = uVar1;
      }
      else {
        iVar5 = *(int *)(iVar13 + (uVar7 * 2 + 1) * 4);
        uVar10 = uVar1 - 0x100 & 0xffff;
      }
      uVar10 = (u32)*(u16 *)(uVar10 * 2 + iVar5 + (*(u32 *)(iVar5 + 4) & 0xfffffffc));
      if (uVar10 == DAT_02019b40) {
        local_28 = 1;
      }
      else {
        if (uVar1 < 0x100) {
          iVar13 = *(int *)(iVar13 + uVar7 * 8);
        }
        else {
          iVar13 = *(int *)(iVar13 + (uVar7 * 2 + 1) * 4);
        }
        local_28 = FUN_0202609c(uVar8,bVar2,iVar11,local_40,(int)uVar10 >> 8,
                                (u32)(*(int *)(param_2 + 0x30) << 0x10) >> 0x19,
                                *(u32 *)(param_2 + 0x74),
                                iVar13 + (*(u32 *)(iVar13 + ((uVar10 & 0xff) + 2) * 4) & 0xfffffffc
                                         ),uVar12,0);
      }
      uVar8 = uVar8 + local_28 + 1 + ((u32)(*(int *)(param_2 + 0x44) << 0xb) >> 0x1c) & 0xff;
      pbVar6 = pbVar9 + 1;
    }
  }
  return;
}

// FUN_02019B44 @ 0x02019B44 (852 bytes) - Buffer text draw commands with delta encoding
void FUN_02019b44(int param_1,int param_2,int param_3,u32 param_4)

{
  u8 *puVar1;
  u8 *puVar2;
  int iVar3;
  char *pcVar4;
  u32 uVar5;
  u8 *puVar6;
  char *pcVar7;
  int iVar8;
  char local_20 [4];
  
  puVar1 = DAT_02019e98;
  iVar8 = ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xd4;
  iVar3 = 0;
  pcVar4 = (char *)(*(int *)(param_1 + 0xf04) + iVar8);
  if ((u32)*(u8 *)(*(int *)(param_1 + 0xf04) + iVar8) != (*(u32 *)(param_3 + 0x24) & 0xff)) {
    *pcVar4 = (char)*(u32 *)(param_3 + 0x24);
    *puVar1 = 0xff;
    puVar1[1] = 0;
    puVar1[2] = *pcVar4;
    iVar3 = 3;
  }
  puVar1 = DAT_02019e98;
  uVar5 = (u32)(*(int *)(param_3 + 0x24) << 0xc) >> 0x16;
  if ((u32)(u8)pcVar4[1] != (uVar5 & 0xff)) {
    pcVar4[1] = (char)uVar5;
    iVar8 = iVar3 + 2;
    puVar1[iVar3] = 0xff;
    puVar1[(iVar3 + 1) * 0x1000000 >> 0x18] = 1;
    iVar3 = (iVar3 + 3) * 0x1000000 >> 0x18;
    puVar1[iVar8 * 0x1000000 >> 0x18] = pcVar4[1];
  }
  puVar1 = DAT_02019e98;
  if (*(short *)(pcVar4 + 4) != *(short *)(param_3 + 0x10)) {
    *(short *)(pcVar4 + 4) = *(short *)(param_3 + 0x10);
    puVar1[iVar3] = 0xff;
    puVar1[(iVar3 + 1) * 0x1000000 >> 0x18] = 3;
    puVar1[(iVar3 + 2) * 0x1000000 >> 0x18] = (char)*(u16 *)(pcVar4 + 4);
    puVar1[(iVar3 + 3) * 0x1000000 >> 0x18] = (char)((u16)*(u16 *)(pcVar4 + 4) >> 8);
    iVar3 = (iVar3 + 4) * 0x1000000 >> 0x18;
  }
  puVar1 = DAT_02019e98;
  if (*(short *)(pcVar4 + 6) != *(short *)(param_3 + 0x12)) {
    *(short *)(pcVar4 + 6) = *(short *)(param_3 + 0x12);
    puVar1[iVar3] = 0xff;
    puVar1[(iVar3 + 1) * 0x1000000 >> 0x18] = 4;
    puVar1[(iVar3 + 2) * 0x1000000 >> 0x18] = (char)*(u16 *)(pcVar4 + 6);
    puVar1[(iVar3 + 3) * 0x1000000 >> 0x18] = (char)((u16)*(u16 *)(pcVar4 + 6) >> 8);
    iVar3 = (iVar3 + 4) * 0x1000000 >> 0x18;
  }
  puVar1 = DAT_02019e98;
  if ((u32)(u8)pcVar4[2] != (*(u32 *)(param_3 + 0xc) & 0xf)) {
    pcVar4[2] = (u8)((*(u32 *)(param_3 + 0xc) << 0x1c) >> 0x1c);
    iVar8 = iVar3 + 2;
    puVar1[iVar3] = 0xff;
    puVar1[(iVar3 + 1) * 0x1000000 >> 0x18] = 2;
    iVar3 = (iVar3 + 3) * 0x1000000 >> 0x18;
    puVar1[iVar8 * 0x1000000 >> 0x18] = pcVar4[2];
  }
  puVar1 = DAT_02019e98;
  if ((u16)(u8)pcVar4[3] != *(u16 *)(param_3 + 0x1c)) {
    pcVar4[3] = (char)*(u16 *)(param_3 + 0x1c);
    iVar8 = iVar3 + 2;
    puVar1[iVar3] = 0xff;
    puVar1[(iVar3 + 1) * 0x1000000 >> 0x18] = 5;
    iVar3 = (iVar3 + 3) * 0x1000000 >> 0x18;
    puVar1[iVar8 * 0x1000000 >> 0x18] = pcVar4[3];
  }
  uVar5 = param_4 & 0xff00;
  if (uVar5 != 0) {
    local_20[0] = -1 - (char)(uVar5 >> 8);
  }
  uVar5 = (u32)(uVar5 != 0);
  local_20[uVar5] = (char)param_4;
  iVar8 = ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xd4 + *(int *)(param_1 + 0xf04);
  puVar1 = *(u8 **)(iVar8 + 0xd0);
  if (puVar1 + uVar5 + (iVar3 - (iVar8 + 8)) + 4 < (u8 *)0xc8) {
    pcVar7 = local_20;
    if ((iVar3 != 0) && (puVar2 = puVar1, pcVar7 = local_20, puVar6 = DAT_02019e98, iVar3 != 0)) {
      do {
        iVar3 = (iVar3 + -1) * 0x1000000 >> 0x18;
        *puVar2 = *puVar6;
        puVar1 = puVar2 + 1;
        puVar2 = puVar2 + 1;
        pcVar7 = local_20;
        puVar6 = puVar6 + 1;
      } while (0 < iVar3);
    }
    do {
      puVar2 = puVar1;
      puVar1 = puVar2 + 1;
      *puVar2 = *pcVar7;
      uVar5 = (int)((uVar5 - 1) * 0x1000000) >> 0x18;
      pcVar7 = pcVar7 + 1;
    } while (-1 < (int)uVar5);
    *puVar1 = 0xff;
    puVar2[2] = 0xff;
    *(u8 **)
     (((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xd4 + *(int *)(param_1 + 0xf04) + 0xd0) =
         puVar1;
    *pcVar4 = *pcVar4 + (u8)((u32)(*(int *)(param_3 + 0x24) << 6) >> 0x1a) + 1 +
                        (u8)((u32)(*(int *)(param_3 + 0x20) << 0xb) >> 0x1c);
    return;
  }
  return;
}

// FUN_02019E9C @ 0x02019E9C (116 bytes) - Initialize OAM entry for sprite
void FUN_02019e9c(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xd4;
  iVar3 = *(int *)(param_1 + 0xf04);
  iVar1 = iVar3 + iVar2;
  *(int *)(iVar1 + 0xd0) = iVar1 + 8;
  *(u8 *)(iVar3 + iVar2) = *(u8 *)(param_2 + 100);
  *(u8 *)(iVar1 + 1) = *(u8 *)(param_2 + 0x65);
  *(u16 *)(iVar1 + 4) = 0x1000;
  *(u16 *)(iVar1 + 6) = 0x1000;
  *(u8 *)(iVar1 + 2) = (u8)((u32)*(u32 *)(param_2 + 0x2c) >> 0x1c);
  *(char *)(iVar1 + 3) = (char)*(u16 *)(param_2 + 0x40);
  **(u8 **)(iVar1 + 0xd0) = 0xff;
  *(u8 *)(*(int *)(iVar1 + 0xd0) + 1) = 0xff;
  return;
}

// FUN_02019F10 @ 0x02019F10 (580 bytes) - Process font string into tile coordinates
void FUN_02019f10(u32 param_1,int param_2,u8 *param_3)

{
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  int iVar5;
  u8 bVar6;
  u32 uVar7;
  int iVar8;
  u32 uVar9;
  u8 *pbVar10;
  int iVar12;
  int iVar13;
  u32 *puVar14;
  u8 local_30;
  u16 _local_2c;
  u8 *pbVar11;
  
  uVar3 = DAT_0201a15c;
  uVar2 = DAT_0201a158;
  uVar1 = DAT_0201a154;
  if (*param_3 != 0) {
    pbVar11 = param_3;
    do {
      pbVar10 = pbVar11 + 1;
      uVar9 = (u32)*pbVar11;
      if (uVar9 == 0x20) {
        local_30 = (u8)((u32)(*(int *)(param_2 + 0xc) << 0x17) >> 0x1b);
      }
      else {
        uVar7 = (u32)*(u16 *)(param_2 + 0x1c);
        iVar12 = *(int *)(param_2 + 0x14);
        if (uVar9 < 0x100) {
          iVar13 = *(int *)(iVar12 + uVar7 * 8);
          uVar4 = uVar9;
        }
        else {
          iVar13 = *(int *)(iVar12 + (uVar7 * 2 + 1) * 4);
          uVar4 = uVar9 - 0x100 & 0xffff;
        }
        uVar4 = (u32)*(u16 *)(uVar4 * 2 + iVar13 + (*(u32 *)(iVar13 + 4) & 0xfffffffc));
        if (uVar9 < 0x100) {
          iVar12 = *(int *)(iVar12 + uVar7 * 8);
        }
        else {
          iVar12 = *(int *)(iVar12 + (uVar7 * 2 + 1) * 4);
        }
        uVar9 = *(u32 *)(iVar12 + ((uVar4 & 0xff) + 2) * 4) >> 2;
        iVar13 = ((int)uVar4 >> 8 & 7U) << 2;
        iVar5 = ((int)uVar4 >> 0xb) + 1;
        uVar7 = *(u32 *)(iVar12 + uVar9 * 4);
        puVar14 = (u32 *)(iVar12 + uVar9 * 4);
        bVar6 = (u8)(puVar14[iVar5] >> iVar13) & 0xf;
        iVar8 = (int)*(short *)(param_2 + 0x12);
        iVar12 = (int)*(short *)(param_2 + 0x10);
        if (4 < (uVar7 & 0xf0) >> 4) {
          bVar6 = bVar6 | (u8)((puVar14[iVar5 + (uVar7 >> 0x19)] >> iVar13 & 0xf) << 4);
        }
        _local_2c = CONCAT11((char)((*puVar14 & 0xf) << 2) + -1,bVar6);
        if (((iVar12 != 0) && (iVar8 != 0)) && ((iVar12 != 0x1000 || (iVar8 != 0x1000)))) {
          FUN_02023a78(iVar12,iVar8,(void *)&_local_2c);
        }
        local_30 = (u8)_local_2c;
      }
      *(u32 *)(param_2 + 0x24) =
           *(u32 *)(param_2 + 0x24) & 0xfffffc00 | uVar2 & *(u32 *)(param_2 + 8) & 0x3ff;
      *(u32 *)(param_2 + 0x24) =
           *(u32 *)(param_2 + 0x24) & uVar3 |
           (uVar2 & (u32)(*(int *)(param_2 + 8) << 0xc) >> 0x16) << 10;
      if (*(u32 *)(param_2 + 0x20) >> 0x16 == uVar2) {
        *(u32 *)(param_2 + 0x20) =
             *(u32 *)(param_2 + 0x20) & uVar1 | (uVar2 & *(u32 *)(param_2 + 8) & 0x3ff) << 0x16;
      }
      *(u32 *)(param_2 + 8) =
           *(u32 *)(param_2 + 8) & 0xfffffc00 |
           local_30 + 1 + ((u32)(*(int *)(param_2 + 0x20) << 0xb) >> 0x1c) +
           (*(u32 *)(param_2 + 8) & 0x3ff) & uVar2;
      *(u32 *)(param_2 + 0x24) = *(u32 *)(param_2 + 0x24) & 0xfc0fffff | (local_30 & 0x3f) << 0x14
      ;
      pbVar11 = pbVar10;
    } while (*pbVar10 != 0);
  }
  *(u8 **)(param_3 + 8) = param_3;
  return;
}

// FUN_0201A160 @ 0x0201A160 (116 bytes) - Load sprite frame with palette (mode 1)
u32 FUN_0201a160(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  if (param_3 != 0) {
    iVar1 = (*(u32 *)(param_1 + 0x54) >> 0x1b) * 0xcc + *(int *)(param_1 + 0xf00);
    if (*(char *)(iVar1 + 0x60) == '\0') {
      FUN_0201a698(param_1,iVar1,*(char *)(*(int *)(param_2 + 4) + 2) + -1,1);
    }
    FUN_02019f10(param_1,param_2,(u8 *)(iVar1 + 0x54));
    return 0;
  }
  return 0;
}

// FUN_0201A1D4 @ 0x0201A1D4 (116 bytes) - Load sprite frame with palette (mode 0)
u32 FUN_0201a1d4(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  if (param_3 != 0) {
    iVar1 = (*(u32 *)(param_1 + 0x54) >> 0x1b) * 0xcc + *(int *)(param_1 + 0xf00);
    if (*(char *)(iVar1 + 0x60) == '\0') {
      FUN_0201a698(param_1,iVar1,*(char *)(*(int *)(param_2 + 4) + 2) + -1,0);
    }
    FUN_02019f10(param_1,param_2,(u8 *)(iVar1 + 0x54));
    return 0;
  }
  return 0;
}

// FUN_0201A248 @ 0x0201A248 (1084 bytes) - Advance text cursor and render glyph
BOOL FUN_0201a248(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  int iVar3;
  u32 uVar4;
  int iVar5;
  u32 uVar6;
  u32 unaff_r8;
  int iVar7;
  BOOL bVar8;
  u8 local_2e;
  u8 bStack_2d;
  u16 local_2c;
  
  uVar2 = DAT_0201a684;
  uVar6 = (u32)**(u8 **)(param_2 + 0x5c);
  if (uVar6 == 0x20) {
    *(u32 *)(param_2 + 0x48) =
         *(u32 *)(param_2 + 0x48) & 0xfffffc00 | DAT_0201a684 & *(u32 *)(param_2 + 0x2c) & 0x3ff;
    uVar6 = (u32)(*(int *)(param_2 + 0x30) << 0x17) >> 0x1b;
    *(u32 *)(param_2 + 0x48) =
         *(u32 *)(param_2 + 0x48) & DAT_0201a688 |
         (uVar2 & (u32)(*(int *)(param_2 + 0x2c) << 0xc) >> 0x16) << 10;
    if (*(u32 *)(param_2 + 0x44) >> 0x16 == uVar2) {
      *(u32 *)(param_2 + 0x44) =
           *(u32 *)(param_2 + 0x44) & DAT_0201a68c |
           (uVar2 & *(u32 *)(param_2 + 0x2c) & 0x3ff) << 0x16;
    }
    *(u32 *)(param_2 + 0x2c) =
         *(u32 *)(param_2 + 0x2c) & 0xfffffc00 |
         uVar6 + 1 + ((u32)(*(int *)(param_2 + 0x44) << 0xb) >> 0x1c) +
         (*(u32 *)(param_2 + 0x2c) & 0x3ff) & DAT_0201a684;
    *(u32 *)(param_2 + 0x48) = *(u32 *)(param_2 + 0x48) & 0xfc0fffff | uVar6 << 0x14;
  }
  else {
    bVar8 = *(u32 *)(param_1 + 0x50) >> 0x1e == 0;
    if (bVar8) {
      unaff_r8 = *(u32 *)(param_2 + 0x78);
    }
    if (!bVar8) {
      unaff_r8 = *(u32 *)(param_2 + 0x74);
    }
    uVar2 = (u32)*(u16 *)(param_2 + 0x40);
    iVar5 = *(int *)(param_2 + 0x38);
    if (uVar6 < 0x100) {
      uVar2 = uVar2 << 1;
    }
    if (uVar6 < 0x100) {
      iVar3 = *(int *)(iVar5 + uVar2 * 4);
      uVar4 = uVar6;
    }
    else {
      uVar2 = uVar2 * 2;
      iVar3 = *(int *)(iVar5 + (uVar2 + 1) * 4);
      uVar4 = uVar6 - 0x100 & 0xffff;
    }
    uVar4 = (u32)*(u16 *)(uVar4 * 2 + iVar3 + (*(u32 *)(iVar3 + 4) & 0xfffffffc));
    if (uVar4 == DAT_0201a690) {
      local_2c = 0x101;
    }
    else {
      if (uVar6 < 0x100) {
        iVar5 = *(int *)(iVar5 + uVar2 * 4);
      }
      else {
        iVar5 = *(int *)(iVar5 + (uVar2 + 1) * 4);
      }
      local_2c = FUN_0202609c(*(u32 *)(param_2 + 0x2c) & 0x3ff,
                              (*(u32 *)(param_2 + 0x2c) << 0xc) >> 0x16,
                              (int)*(short *)(param_2 + 0x34),(int)*(short *)(param_2 + 0x36),
                              (int)uVar4 >> 8,(*(u32 *)(param_2 + 0x30) << 0x10) >> 0x19,unaff_r8,
                              iVar5 + (*(u32 *)(iVar5 + ((uVar4 & 0xff) + 2) * 4) & 0xfffffffc),
                              *(u32 *)(param_2 + 0x30) & 0xf,0);
    }
    uVar6 = *(u32 *)(param_2 + 0x2c);
    uVar2 = (uVar6 << 0xc) >> 0x16;
    bStack_2d = (u8)((u16)local_2c >> 8);
    local_2e = (u8)local_2c;
    if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
      uVar4 = (((u32)(*(int *)(param_2 + 0x10) << 7) >> 0x1a) + 1) * 0x80000 >> 0x10;
      iVar1 = ((int)(uVar2 & 0xff) >> 3) * uVar4 + (uVar6 & 0xff & DAT_0201a694);
      iVar5 = (((int)((u32)local_2e + (uVar6 & 7)) >> 3) + 1) * 0x20;
      iVar7 = (int)(short)((int)((u32)bStack_2d + (uVar2 & 7)) >> 3);
      uVar6 = *(int *)(param_2 + 0x74) + iVar1 * 4;
      iVar3 = uVar4 * 4;
      uVar2 = *(int *)(param_2 + 0x78) + iVar1 * 4;
      do {
        if (uVar2 < uVar6) {
          FUN_0202cd68(uVar2,uVar6,iVar5);
        }
        else {
          FUN_0202cc94(uVar2,uVar6,iVar5);
        }
        uVar6 = uVar6 + iVar3;
        uVar2 = uVar2 + iVar3;
        iVar7 = (iVar7 + -1) * 0x10000 >> 0x10;
      } while (-1 < iVar7);
    }
    uVar2 = DAT_0201a684;
    iVar5 = (short)(u16)((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xcc +
            *(int *)(param_1 + 0xf00);
    *(u32 *)(iVar5 + 0x68) = *(u32 *)(iVar5 + 0x68) & 0xfffffffe | 1;
    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 8;
    *(u32 *)(param_2 + 0x48) =
         *(u32 *)(param_2 + 0x48) & 0xfffffc00 | uVar2 & *(u32 *)(param_2 + 0x2c) & 0x3ff;
    *(u32 *)(param_2 + 0x48) =
         *(u32 *)(param_2 + 0x48) & DAT_0201a688 |
         (uVar2 & (u32)(*(int *)(param_2 + 0x2c) << 0xc) >> 0x16) << 10;
    if (*(u32 *)(param_2 + 0x44) >> 0x16 == uVar2) {
      *(u32 *)(param_2 + 0x44) =
           *(u32 *)(param_2 + 0x44) & DAT_0201a68c |
           (uVar2 & *(u32 *)(param_2 + 0x2c) & 0x3ff) << 0x16;
    }
    *(u32 *)(param_2 + 0x2c) =
         *(u32 *)(param_2 + 0x2c) & 0xfffffc00 |
         local_2e + 1 + ((u32)(*(int *)(param_2 + 0x44) << 0xb) >> 0x1c) +
         (*(u32 *)(param_2 + 0x2c) & 0x3ff) & DAT_0201a684;
    *(u32 *)(param_2 + 0x48) = *(u32 *)(param_2 + 0x48) & 0xfc0fffff | (local_2e & 0x3f) << 0x14;
  }
  *(int *)(param_2 + 0x5c) = *(int *)(param_2 + 0x5c) + 1;
  return **(char **)(param_2 + 0x5c) == '\0';
}

// FUN_0201A7B0 @ 0x0201A7B0 (424 bytes) - Compute sprite bounding box for collision
void FUN_0201a7b0(u32 param_1,int param_2,int param_3,int param_4)

{
  u32 uVar1;
  u32 uVar2;
  int iVar3;
  int *piVar4;
  u32 uVar5;
  
  if ((*(u32 *)(param_2 + 0xc) & 0xf) == 0) {
    return;
  }
  piVar4 = (int *)(param_2 + 8);
  uVar5 = *(u32 *)(param_2 + 0xc);
  uVar1 = *(u32 *)(param_2 + 0x10) & 0x1ff;
  uVar2 = (*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17;
  switch(uVar5 & 0xf) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    goto LAB_0201a8a8;
  case 4:
LAB_0201a8a8:
    uVar5 = (uVar5 << 0xe) >> 0x18;
    if (uVar5 == 0) {
      iVar3 = uVar1 + (((u32)(*piVar4 << 0x16) >> 0x1c) + 1) * 8;
    }
    else {
      iVar3 = uVar1 + uVar5 + 1;
    }
    param_3 = iVar3 * 0x10000 >> 0x10;
    if ((u32)(*(int *)(param_2 + 0xc) << 0x17) >> 0x1c == 0) {
      param_4 = (int)((uVar2 + ((u32)(*piVar4 << 0x11) >> 0x1b) * 8 + -8) * 0x10000) >> 0x10;
    }
    else {
      param_4 = (int)((uVar2 + 0x10) * 0x10000) >> 0x10;
    }
    goto switchD_0201a810_default;
  case 5:
    break;
  case 6:
    break;
  default:
    goto switchD_0201a810_default;
  }
  uVar5 = (uVar5 << 0xe) >> 0x18;
  if (uVar5 == 0) {
    uVar5 = (((u32)(*piVar4 << 0x16) >> 0x1c) + 1) * 8;
  }
  param_3 = (int)((uVar1 + uVar5) * 0x10000) >> 0x10;
  if ((u32)(*(int *)(param_2 + 0xc) << 0x17) >> 0x1c == 0) {
    param_4 = (int)((uVar2 + (((u32)(*piVar4 << 0x11) >> 0x1b) + 1) * 8 + -8) * 0x10000) >> 0x10;
  }
  else {
    param_4 = (int)((uVar2 + 8) * 0x10000) >> 0x10;
  }
switchD_0201a810_default:
  FUN_0201d9c4(param_1,param_2,param_3,param_4);
  return;
}

// FUN_0201A960 @ 0x0201A960 (640 bytes) - Calculate screen position for text layout
u32 FUN_0201a960(u32 param_1,int param_2,u32 param_3,u32 param_4,int param_5,
                 u32 param_6)

{
  int iVar1;
  char cVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  int iVar7;
  u32 uVar8;
  u32 uVar9;
  int extraout_r1;
  int extraout_r1_00;
  u16 *puVar10;
  u32 uVar11;
  u32 uVar12;
  int iVar13;
  u8 bVar14;
  int iVar15;
  u8 auStack_60 [8];
  u8 auStack_58 [4];
  char *local_54;
  u32 local_38;
  u8 local_30;
  
  if (param_5 != 0) {
    cVar2 = *(char *)(*DAT_0201abe0 + 0x515);
    if (cVar2 == '\0') {
      uVar8 = 3;
      uVar5 = 2;
    }
    else {
      uVar8 = 1;
      uVar5 = 1;
    }
    FUN_02026388(auStack_58,param_6,0,param_5,0,0,uVar5,uVar8,1,6,0x40,cVar2,0xff,0);
    uVar4 = DAT_0201abe8;
    uVar3 = DAT_0201abe4;
    bVar14 = 0;
    local_38 = local_38 | 0x200000;
    while( true ) {
      while (*local_54 != '\0') {
        FUN_02025810(auStack_60,auStack_58,1,1);
      }
      if (bVar14 < local_30) {
        bVar14 = local_30;
      }
      if (CONCAT11(local_54[-2],local_54[-1]) == uVar3) break;
      if (CONCAT11(local_54[-2],local_54[-1]) == uVar4) {
        local_54 = local_54 + 1;
      }
    }
    if (((1 < bVar14) && (cVar2 != '\0')) && (param_2 == 2)) {
      param_4 = param_4 + 1 & 0xff;
    }
  }
  puVar10 = *(u16 **)(DAT_0201abec + param_2 * 4);
  uVar3 = *puVar10 & 0x7f;
  iVar15 = uVar3 * 2;
  uVar12 = **(u16 **)(puVar10 + 0xc) & 0xff;
  uVar9 = iVar15 + uVar12 & 0xff;
  uVar4 = puVar10[1] & 0x7f;
  iVar13 = uVar4 * 2;
  uVar11 = *(u16 *)(*(int *)(puVar10 + 0xe) + 2) & 0xff;
  uVar6 = iVar13 + uVar11 & 0xff;
  if (param_3 < uVar9) {
    param_3 = uVar9;
  }
  if (param_4 < uVar6) {
    param_4 = uVar6;
  }
  if (param_2 == 2) {
    uVar6 = param_3 + 1 & 0xff;
    iVar7 = (int)((uVar6 + uVar3 * -2) * 0x1000000) >> 0x18;
    iVar1 = (int)((param_4 + uVar4 * -2) * 0x1000000) >> 0x18;
    FUN_020464b0(iVar7,uVar12);
    if (extraout_r1 != 0) {
      iVar7 = FUN_020464b0(iVar7,uVar12);
      uVar6 = uVar12 * (iVar7 + 1) + iVar15 & 0xff;
    }
    FUN_020464b0(iVar1,uVar11);
    if (extraout_r1_00 != 0) {
      iVar15 = FUN_020464b0(iVar1,uVar11);
      param_4 = uVar11 * (iVar15 + 1) + iVar13 & 0xff;
    }
  }
  else {
    iVar7 = FUN_020464b0((param_3 + uVar3 * -2) * 0x100,uVar12);
    uVar6 = ((iVar7 + 0x80) * 0x100 >> 0x10) * uVar12 + iVar15 & 0xff;
    iVar15 = FUN_020464b0((param_4 + uVar4 * -2) * 0x100,uVar11);
    param_4 = ((iVar15 + 0x80) * 0x100 >> 0x10) * uVar11 + iVar13 & 0xff;
  }
  if (0x20 < uVar6) {
    uVar6 = uVar6 - uVar12 & 0xff;
  }
  if (0x18 < param_4) {
    param_4 = param_4 - uVar11 & 0xff;
  }
  return uVar6 | param_4 << 0x10;
}

// FUN_0201ABF0 @ 0x0201ABF0 (492 bytes) - Update BG scroll registers for both screens
void FUN_0201abf0(int param_1)

{
  if (-1 < *(int *)(param_1 + 0x54) << 6) {
    switch((u32)(*(int *)(param_1 + 0x50) << 0x10) >> 0x1e) {
    case 0:
      *DAT_0201ade4 =
           (int)*(short *)(param_1 + 0xf2c) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf2e) + 0x10) * 0x10000;
      break;
    case 1:
      *DAT_0201ade8 =
           (int)*(short *)(param_1 + 0xf2c) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf2e) + 0x10) * 0x10000;
      break;
    case 2:
      *DAT_0201adec =
           (int)*(short *)(param_1 + 0xf2c) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf2e) + 0x10) * 0x10000;
      break;
    case 3:
      *DAT_0201adf0 =
           (int)*(short *)(param_1 + 0xf2c) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf2e) + 0x10) * 0x10000;
    }
  }
  if (-1 < *(int *)(param_1 + 0x54) << 5) {
    switch((u32)(*(int *)(param_1 + 0x50) << 0xe) >> 0x1e) {
    case 0:
      *DAT_0201adf4 =
           (int)*(short *)(param_1 + 0xf30) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf32) + 0x10) * 0x10000;
      return;
    case 1:
      *DAT_0201adf8 =
           (int)*(short *)(param_1 + 0xf30) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf32) + 0x10) * 0x10000;
      return;
    case 2:
      *DAT_0201adfc =
           (int)*(short *)(param_1 + 0xf30) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf32) + 0x10) * 0x10000;
      return;
    case 3:
      *DAT_0201ae00 =
           (int)*(short *)(param_1 + 0xf30) & DAT_0201addc |
           DAT_0201ade0 & (*(short *)(param_1 + 0xf32) + 0x10) * 0x10000;
      return;
    default:
      return;
    }
  }
  return;
}

// FUN_0201B208 @ 0x0201B208 (248 bytes) - Configure BG control register (sub screen)
void FUN_0201b208(int param_1,int param_2,int param_3)

{
  u16 *puVar1;
  u16 *puVar2;
  u16 *puVar3;
  u16 *puVar4;
  
  puVar4 = DAT_0201b30c;
  puVar3 = DAT_0201b308;
  puVar2 = DAT_0201b304;
  puVar1 = DAT_0201b300;
  switch((u32)(*(int *)(param_1 + 0x50) << 0xe) >> 0x1e) {
  case 0:
    *DAT_0201b300 = *DAT_0201b300 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar1 = *puVar1 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 10) >> 0x1e);
    return;
  case 1:
    *DAT_0201b304 = *DAT_0201b304 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar2 = *puVar2 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 10) >> 0x1e);
    return;
  case 2:
    *DAT_0201b308 = *DAT_0201b308 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar3 = *puVar3 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 10) >> 0x1e);
    return;
  case 3:
    *DAT_0201b30c = *DAT_0201b30c & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar4 = *puVar4 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 10) >> 0x1e);
    return;
  default:
    return;
  }
}

// FUN_0201B310 @ 0x0201B310 (248 bytes) - Configure BG control register (main screen)
void FUN_0201b310(int param_1,int param_2,int param_3)

{
  u16 *puVar1;
  u16 *puVar2;
  u16 *puVar3;
  u16 *puVar4;
  
  puVar4 = DAT_0201b414;
  puVar3 = DAT_0201b410;
  puVar2 = DAT_0201b40c;
  puVar1 = DAT_0201b408;
  switch((u32)(*(int *)(param_1 + 0x50) << 0x10) >> 0x1e) {
  case 0:
    *DAT_0201b408 = *DAT_0201b408 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar1 = *puVar1 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 0xc) >> 0x1e);
    return;
  case 1:
    *DAT_0201b40c = *DAT_0201b40c & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar2 = *puVar2 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 0xc) >> 0x1e);
    return;
  case 2:
    *DAT_0201b410 = *DAT_0201b410 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar3 = *puVar3 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 0xc) >> 0x1e);
    return;
  case 3:
    *DAT_0201b414 = *DAT_0201b414 & 0x43 | (u16)(param_2 << 8) | (u16)(param_3 << 2);
    *puVar4 = *puVar4 & 0xfffc | (u16)((u32)(*(int *)(param_1 + 0x50) << 0xc) >> 0x1e);
    return;
  default:
    return;
  }
}

// FUN_0201B568 @ 0x0201B568 (4492 bytes) - Composite tiled sprite into framebuffer
void FUN_0201b568(int param_1,int param_2,u16 *param_3,u32 param_4,int param_5)

{
  u32 uVar1;
  u16 uVar2;
  u32 *puVar3;
  u32 *puVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  int *piVar8;
  u32 uVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  u32 uVar14;
  int iVar15;
  u32 uVar16;
  u32 uVar17;
  int iVar18;
  int iVar19;
  int local_150;
  int local_14c;
  u16 *local_148;
  u32 local_144;
  int local_140;
  int local_13c;
  int local_138;
  u32 local_134;
  u32 local_130;
  u32 local_12c;
  u32 local_128;
  u32 local_124;
  u32 local_120;
  int local_11c;
  u32 local_118;
  u32 *local_114;
  u32 local_110;
  u32 local_10c;
  u32 local_108;
  u32 local_104;
  u32 local_100;
  int local_fc;
  int local_f8;
  int local_f4;
  u32 local_f0;
  u32 local_ec;
  int local_e8;
  u32 local_e4;
  u32 local_e0;
  int local_dc;
  u32 local_d8;
  int local_d4;
  int local_d0;
  int local_cc;
  u32 local_c8;
  int local_c4;
  int local_c0;
  u32 local_bc;
  u32 local_b8;
  u32 local_b4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  u32 local_78;
  int local_74;
  int local_70;
  int local_6c;
  u32 local_68;
  int local_64;
  u32 local_60;
  int local_5c;
  u32 local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  u32 local_34;
  u32 local_30;
  u32 *local_2c;
  
  local_148 = param_3;
  local_128 = (u32)*param_3;
  local_150 = param_1;
  local_124 = (u32)param_3[1];
  local_14c = param_2;
  local_144 = param_4;
  uVar1 = (**(u16 **)(param_3 + 0xc) + local_128) * 0x20000 >> 0x10;
  local_78 = (*(u16 *)(*(int *)(param_3 + 0xe) + 2) + local_124) * 0x20000 >> 0x10;
  local_74 = uVar1 + 1;
  uVar5 = (uVar1 - local_128) * 0x80000;
  local_130 = (uVar1 + 1) * (local_78 - local_124) * 0x80000 >> 0x10;
  local_134 = uVar5 >> 0x10;
  local_12c = local_130 + (uVar5 >> 0x10) & 0xffff;
  if (param_5 == 0) {
    local_120 = (*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17 & 7;
    uVar5 = *(u32 *)(param_2 + 0x10) & 7;
    iVar13 = uVar5 * 4;
    local_11c = *(u32 *)(param_3 + 2) << iVar13;
    uVar5 = uVar5 * -4 + 0x20;
    local_118 = *(u32 *)(param_3 + 2) >> (uVar5 & 0xff);
    FUN_0202cc10(param_1 + 0x60,0,0xea0);
    iVar19 = local_150 + 0x60;
    local_110 = 0;
    local_138 = (int)(short)local_148[1];
    if (0 < (short)local_148[1]) {
      local_80 = local_120 + 8;
      local_7c = uVar1 + 1;
      do {
        local_13c = (int)(short)*local_148;
        while (0 < local_13c) {
          iVar10 = *(int *)(local_150 + 0x58);
          iVar11 = local_110 * 2;
          local_2c = (u32 *)(iVar10 + (u32)*(u16 *)(*(int *)(local_148 + 8) + iVar11) * 4);
          local_114 = (u32 *)(iVar10 + (u32)*(u16 *)(*(int *)(local_148 + 10) + iVar11) * 4);
          puVar3 = (u32 *)(iVar10 + (u32)*(u16 *)(*(int *)(local_148 + 4) + iVar11) * 4);
          puVar4 = (u32 *)(iVar10 + (u32)*(u16 *)(*(int *)(local_148 + 6) + iVar11) * 4);
          for (uVar16 = local_120; (int)uVar16 < (int)(local_120 + 8);
              uVar16 = (int)((uVar16 + 1) * 0x10000) >> 0x10) {
            iVar11 = (uVar16 & DAT_0201c6f4) * uVar1 + uVar16;
            uVar6 = *puVar3;
            iVar18 = iVar19 + iVar11 * 4;
            iVar15 = iVar18 + local_134 * 4;
            *(u32 *)(iVar19 + iVar11 * 4) = *(int *)(iVar19 + iVar11 * 4) + (uVar6 << iVar13);
            *(u32 *)(iVar18 + 0x20) = *(int *)(iVar18 + 0x20) + (uVar6 >> (uVar5 & 0xff));
            uVar6 = *puVar4;
            iVar10 = iVar18 + local_130 * 4;
            *(u32 *)(iVar18 + local_134 * 4) = *(int *)(iVar18 + local_134 * 4) + (uVar6 << iVar13)
            ;
            *(u32 *)(iVar15 + 0x20) = *(int *)(iVar15 + 0x20) + (uVar6 >> (uVar5 & 0xff));
            uVar6 = *local_2c;
            local_2c = local_2c + 1;
            iVar11 = iVar18 + local_12c * 4;
            *(u32 *)(iVar18 + local_130 * 4) = *(int *)(iVar18 + local_130 * 4) + (uVar6 << iVar13)
            ;
            *(u32 *)(iVar10 + 0x20) = *(int *)(iVar10 + 0x20) + (uVar6 >> (uVar5 & 0xff));
            uVar6 = *local_114;
            local_114 = local_114 + 1;
            *(u32 *)(iVar18 + local_12c * 4) = *(int *)(iVar18 + local_12c * 4) + (uVar6 << iVar13)
            ;
            *(u32 *)(iVar11 + 0x20) = *(int *)(iVar11 + 0x20) + (uVar6 >> (uVar5 & 0xff));
            puVar3 = puVar3 + 1;
            puVar4 = puVar4 + 1;
          }
          iVar19 = iVar19 + 0x20;
          local_110 = local_110 + 1 & 0xff;
          local_13c = (local_13c + -1) * 0x10000 >> 0x10;
        }
        iVar19 = iVar19 + ((uVar1 + 1) - (u32)*local_148) * 0x20;
        local_138 = (local_138 + -1) * 0x10000 >> 0x10;
      } while (0 < local_138);
    }
    local_5c = local_78 - local_124;
    local_10c = (u32)**(u16 **)(local_148 + 0xc);
    local_70 = local_74 * local_5c;
    uVar16 = (u32)(local_70 * 0x80000) >> 0x10;
    local_6c = (int)(short)local_128;
    local_140 = local_6c;
    local_84 = uVar1 - local_128;
    if (local_6c < local_84) {
      local_9c = local_6c << 3;
      local_98 = local_10c * 8;
      local_94 = (int)(short)local_124;
      local_90 = (int)(short)**(u16 **)(local_148 + 0xc);
      local_8c = local_10c * local_124;
      local_88 = ((uVar1 + 1) - local_10c) * 0x20;
      local_34 = 2;
      do {
        iVar19 = local_150 + 0x60 + local_9c * 4;
        local_108 = 2;
        local_4c = local_94;
        while (0 < local_4c) {
          local_44 = local_90;
          while (0 < local_44) {
            puVar3 = (u32 *)(*(int *)(local_150 + 0x58) +
                             (u32)*(u16 *)(*(int *)(local_148 + 0xc) + local_108 * 2) * 4);
            puVar4 = (u32 *)(*(int *)(local_150 + 0x58) +
                             (u32)*(u16 *)
                                    (local_108 * 2 +
                                    *(int *)(local_148 + 0xc) + local_10c * local_124 * 2) * 4);
            for (uVar6 = local_120; (int)uVar6 < (int)(local_120 + 8);
                uVar6 = (int)((uVar6 + 1) * 0x10000) >> 0x10) {
              iVar11 = (uVar6 & DAT_0201c6f4) * uVar1 + uVar6;
              uVar7 = *puVar3;
              iVar10 = iVar19 + iVar11 * 4;
              *(u32 *)(iVar19 + iVar11 * 4) = *(int *)(iVar19 + iVar11 * 4) + (uVar7 << iVar13);
              *(u32 *)(iVar10 + 0x20) = *(int *)(iVar10 + 0x20) + (uVar7 >> (uVar5 & 0xff));
              uVar7 = *puVar4;
              iVar11 = iVar10 + uVar16 * 4;
              *(u32 *)(iVar10 + uVar16 * 4) = *(int *)(iVar10 + uVar16 * 4) + (uVar7 << iVar13);
              *(u32 *)(iVar11 + 0x20) = *(int *)(iVar11 + 0x20) + (uVar7 >> (uVar5 & 0xff));
              puVar3 = puVar3 + 1;
              puVar4 = puVar4 + 1;
            }
            iVar19 = iVar19 + 0x20;
            local_108 = local_108 + 1 & 0xff;
            local_44 = (local_44 + -1) * 0x10000 >> 0x10;
          }
          iVar19 = iVar19 + local_88;
          local_4c = (local_4c + -1) * 0x10000 >> 0x10;
        }
        local_9c = local_9c + local_10c * 8;
        local_140 = (int)((local_140 + local_10c) * 0x10000) >> 0x10;
      } while (local_140 < local_84);
    }
    if (local_120 != 0) {
      piVar8 = (int *)(local_150 + 0x60 + (local_128 + local_70) * 0x20);
      piVar12 = (int *)(local_150 + 0x60 + (local_124 * (uVar1 + 1) + local_128) * 0x20 +
                       local_120 * 4);
      iVar19 = (int)((uVar1 + local_128 * -2) * 0x10000) >> 0x10;
      if (0 < iVar19) {
        iVar10 = 8 - local_120;
        iVar11 = iVar10;
        do {
          for (; iVar11 = iVar11 * 0x10000 >> 0x10, 0 < iVar11; iVar11 = iVar11 + -1) {
            *piVar12 = *piVar12 + local_11c;
            piVar12[8] = piVar12[8] + local_118;
            piVar12 = piVar12 + 1;
          }
          piVar12 = piVar12 + local_120;
          for (uVar16 = local_120; 0 < (int)uVar16; uVar16 = (int)((uVar16 - 1) * 0x10000) >> 0x10)
          {
            *piVar8 = *piVar8 + local_11c;
            piVar8[8] = piVar8[8] + local_118;
            piVar8 = piVar8 + 1;
          }
          piVar8 = piVar8 + iVar10;
          iVar19 = (iVar19 + -1) * 0x10000 >> 0x10;
          iVar11 = iVar10;
        } while (0 < iVar19);
      }
    }
    uVar16 = (u32)(local_84 * 0x80000) >> 0x10;
    uVar2 = *(u16 *)(*(int *)(local_148 + 0xe) + 2);
    local_104 = (u32)uVar2;
    local_38 = (int)(short)local_124;
    if (local_38 < local_5c) {
      local_b0 = local_38 * (uVar1 + 1);
      local_ac = (uVar1 + 1) * local_104;
      local_a8 = (int)(short)uVar2;
      local_a4 = local_128 * local_104;
      local_a0 = (uVar16 + 8) * 4;
      local_30 = 2;
      do {
        iVar19 = local_150 + 0x60 + local_b0 * 0x20;
        local_100 = 2;
        local_50 = local_a8;
        while (0 < local_50) {
          local_48 = local_6c;
          while (0 < local_48) {
            puVar3 = (u32 *)(*(int *)(local_150 + 0x58) +
                             (u32)*(u16 *)(*(int *)(local_148 + 0xe) + local_100 * 2) * 4);
            puVar4 = (u32 *)(*(int *)(local_150 + 0x58) +
                             (u32)*(u16 *)
                                    (local_100 * 2 +
                                    *(int *)(local_148 + 0xe) + local_128 * local_104 * 2) * 4);
            for (uVar6 = local_120; (int)uVar6 < (int)(local_120 + 8);
                uVar6 = (int)((uVar6 + 1) * 0x10000) >> 0x10) {
              iVar11 = (uVar6 & DAT_0201c6f4) * uVar1 + uVar6;
              uVar7 = *puVar3;
              iVar10 = iVar19 + iVar11 * 4;
              *(u32 *)(iVar19 + iVar11 * 4) = *(int *)(iVar19 + iVar11 * 4) + (uVar7 << iVar13);
              *(u32 *)(iVar10 + 0x20) = *(int *)(iVar10 + 0x20) + (uVar7 >> (uVar5 & 0xff));
              uVar7 = *puVar4;
              iVar11 = iVar10 + uVar16 * 4;
              *(u32 *)(iVar10 + uVar16 * 4) = *(int *)(iVar10 + uVar16 * 4) + (uVar7 << iVar13);
              *(u32 *)(iVar11 + 0x20) = *(int *)(iVar11 + 0x20) + (uVar7 >> (uVar5 & 0xff));
              puVar3 = puVar3 + 1;
              puVar4 = puVar4 + 1;
            }
            iVar19 = iVar19 + 0x20;
            local_100 = local_100 + 1 & 0xff;
            local_48 = (local_48 + -1) * 0x10000 >> 0x10;
          }
          iVar19 = iVar19 + local_a0;
          local_50 = (local_50 + -1) * 0x10000 >> 0x10;
        }
        local_b0 = local_b0 + local_ac;
        local_38 = (int)((local_38 + local_104) * 0x10000) >> 0x10;
      } while (local_38 < local_5c);
    }
    if ((*(u32 *)(local_14c + 0x10) & 7) != 0) {
      uVar5 = local_124;
      if (((*(u32 *)(local_14c + 0x10) << 0xe) >> 0x17 & 7) != 0) {
        uVar5 = local_124 + 1 & 0xffff;
      }
      piVar12 = (int *)(local_150 + 0x60 + (uVar5 * local_74 + local_128) * 0x20);
      uVar5 = (uVar1 + local_128 * -2) * 0x80000 >> 0x10;
      for (iVar13 = (int)(short)uVar2; 0 < iVar13; iVar13 = (iVar13 + -1) * 0x10000 >> 0x10) {
        iVar19 = 8;
        do {
          *piVar12 = *piVar12 + local_11c;
          iVar19 = (iVar19 + -1) * 0x10000 >> 0x10;
          piVar8 = piVar12 + 1;
          piVar12[uVar5] = piVar12[uVar5] + local_118;
          piVar12 = piVar8;
        } while (0 < iVar19);
        piVar12 = piVar8 + uVar1 * 8;
      }
    }
  }
  uVar5 = *(u32 *)(local_14c + 0x10);
  local_fc = ((uVar5 << 7) >> 0x1a) + 1;
  local_f8 = ((uVar5 << 1) >> 0x1a) + 1;
  if ((uVar5 & 7) != 0) {
    local_128 = local_128 + 1 & 0xffff;
  }
  if (((u32)(*(int *)(local_14c + 0x10) << 0xe) >> 0x17 & 7) != 0) {
    local_124 = local_124 + 1 & 0xffff;
  }
  uVar5 = (local_fc - local_128) * 0x80000;
  local_60 = (local_fc - local_128) * 8;
  iVar13 = local_fc * (local_f8 - local_124);
  local_68 = iVar13 * 8;
  uVar16 = (u32)(iVar13 * 0x80000) >> 0x10;
  uVar6 = local_150 + 0x60;
  local_58 = local_144;
  iVar13 = local_128 << 5;
  local_64 = (int)(short)local_124;
  local_54 = local_64;
  if (0 < local_64) {
    local_bc = local_144 + (uVar5 >> 0x10) * 4;
    local_b8 = uVar6 + local_134 * 4;
    local_b4 = local_144 + uVar16 * 4;
    uVar7 = uVar6 + local_130 * 4;
    uVar16 = local_144 + (uVar16 + (uVar5 >> 0x10) & 0xffff) * 4;
    uVar5 = uVar6 + local_12c * 4;
    iVar19 = local_fc * 0x20;
    iVar11 = (uVar1 + 1) * 0x20;
    do {
      if (uVar6 < local_58) {
        FUN_0202cd68(uVar6,local_58,iVar13);
      }
      else {
        FUN_0202cc94(uVar6,local_58,iVar13);
      }
      if (local_b8 < local_bc) {
        FUN_0202cd68(local_b8,local_bc,iVar13);
      }
      else {
        FUN_0202cc94(local_b8,local_bc,iVar13);
      }
      if (uVar7 < local_b4) {
        FUN_0202cd68(uVar7,local_b4,iVar13);
      }
      else {
        FUN_0202cc94(uVar7,local_b4,iVar13);
      }
      if (uVar5 < uVar16) {
        FUN_0202cd68(uVar5,uVar16,iVar13);
      }
      else {
        FUN_0202cc94(uVar5,uVar16,iVar13);
      }
      uVar7 = uVar7 + iVar11;
      local_b8 = local_b8 + iVar11;
      uVar5 = uVar5 + iVar11;
      local_bc = local_bc + iVar19;
      uVar6 = uVar6 + iVar11;
      local_b4 = local_b4 + iVar19;
      uVar16 = uVar16 + iVar19;
      local_58 = local_58 + iVar19;
      local_54 = (local_54 + -1) * 0x10000 >> 0x10;
    } while (0 < local_54);
  }
  local_f4 = local_fc + (u32)*local_148 * -2;
  if ((*(u32 *)(local_14c + 0x10) & 7) != 0) {
    local_f4 = local_f4 + (u32)**(u16 **)(local_148 + 0xc) * -2;
  }
  uVar5 = local_68 & 0xffff;
  local_ec = uVar5;
  local_3c = 0;
  iVar19 = (u32)**(u16 **)(local_148 + 0xc) * 0x20;
  uVar16 = local_74 * (local_78 - (*(u16 **)(local_148 + 0xc))[1]) * 0x80000 >> 0x10;
  local_f0 = uVar16;
  if (0 < local_f4) {
    local_c8 = local_150 + 0x60 + local_128 * 0x20;
    local_c4 = (uVar1 + 1) * 0x20;
    local_c0 = local_fc << 5;
    do {
      uVar6 = local_144 + (local_128 + local_3c) * 0x20;
      uVar7 = local_c8;
      for (iVar11 = local_64; 0 < iVar11; iVar11 = (iVar11 + -1) * 0x10000 >> 0x10) {
        if (uVar7 < uVar6) {
          FUN_0202cd68(uVar7,uVar6,iVar19);
        }
        else {
          FUN_0202cc94(uVar7,uVar6,iVar19);
        }
        uVar14 = uVar6 + uVar5 * 4;
        uVar9 = uVar7 + uVar16 * 4;
        if (uVar9 < uVar14) {
          FUN_0202cd68(uVar9,uVar14,iVar19);
        }
        else {
          FUN_0202cc94(uVar9,uVar14,iVar19);
        }
        uVar7 = uVar7 + local_c4;
        uVar6 = uVar6 + local_c0;
      }
      local_3c = (int)((local_3c + (u32)**(u16 **)(local_148 + 0xc)) * 0x10000) >> 0x10;
    } while (local_3c < local_f4);
  }
  if (((*(u32 *)(local_14c + 0x10) & 7) != 0) && (iVar19 = iVar19 + -0x20, iVar19 != 0)) {
    uVar16 = local_144 + (local_128 + local_3c) * 0x20;
    uVar5 = local_150 + 0x60 + local_128 * 0x20;
    if (0 < local_64) {
      uVar7 = uVar16 + local_ec * 4;
      uVar6 = uVar5 + local_f0 * 4;
      iVar11 = local_fc * 0x20;
      local_d0 = local_74 << 5;
      local_cc = (uVar1 + 1) * 0x20;
      do {
        if (uVar5 < uVar16) {
          FUN_0202cd68(uVar5,uVar16,iVar19);
        }
        else {
          FUN_0202cc94(uVar5,uVar16,iVar19);
        }
        if (uVar6 < uVar7) {
          FUN_0202cd68(uVar6,uVar7,iVar19);
        }
        else {
          FUN_0202cc94(uVar6,uVar7,iVar19);
        }
        uVar7 = uVar7 + iVar11;
        uVar6 = uVar6 + local_d0;
        uVar16 = uVar16 + iVar11;
        uVar5 = uVar5 + local_cc;
        local_64 = (local_64 + -1) * 0x10000 >> 0x10;
      } while (0 < local_64);
    }
  }
  local_e8 = local_f8 + (u32)local_148[1] * -2;
  if (((u32)(*(int *)(local_14c + 0x10) << 0xe) >> 0x17 & 7) != 0) {
    local_e8 = local_e8 + (u32)*(u16 *)(*(int *)(local_148 + 0xe) + 2) * -2;
  }
  if ((*(u32 *)(local_14c + 0x10) & 7) == 0) {
    iVar19 = uVar1 - local_128;
  }
  else {
    iVar19 = local_74 - local_128;
  }
  uVar5 = iVar19 * 8 & 0xffff;
  local_e4 = uVar5;
  uVar16 = local_60 & 0xffff;
  local_e0 = uVar16;
  local_40 = 0;
  if (0 < local_e8) {
    local_d8 = local_150 + 0x60 + local_124 * (uVar1 + 1) * 0x20;
    local_d4 = local_fc << 5;
    do {
      uVar6 = local_144 + local_fc * (local_124 + local_40) * 0x20;
      uVar7 = local_d8;
      for (iVar19 = (int)*(short *)(*(int *)(local_148 + 0xe) + 2); 0 < iVar19;
          iVar19 = (iVar19 + -1) * 0x10000 >> 0x10) {
        if (uVar7 < uVar6) {
          FUN_0202cd68(uVar7,uVar6,iVar13);
        }
        else {
          FUN_0202cc94(uVar7,uVar6,iVar13);
        }
        uVar14 = uVar6 + uVar16 * 4;
        uVar9 = uVar7 + uVar5 * 4;
        if (uVar9 < uVar14) {
          FUN_0202cd68(uVar9,uVar14,iVar13);
        }
        else {
          FUN_0202cc94(uVar9,uVar14,iVar13);
        }
        uVar7 = uVar7 + (uVar1 + 1) * 0x20;
        uVar6 = uVar6 + local_d4;
      }
      local_40 = (int)((local_40 + (u32)*(u16 *)(*(int *)(local_148 + 0xe) + 2)) * 0x10000) >>
                 0x10;
    } while (local_40 < local_e8);
  }
  if ((((u32)(*(int *)(local_14c + 0x10) << 0xe) >> 0x17 & 7) != 0) &&
     (iVar19 = *(u16 *)(*(int *)(local_148 + 0xe) + 2) - 1, 0 < iVar19)) {
    uVar16 = local_144 + local_fc * (local_124 + local_40) * 0x20;
    iVar19 = iVar19 * 0x10000 >> 0x10;
    uVar5 = local_150 + 0x60 + local_124 * local_74 * 0x20;
    if (0 < iVar19) {
      uVar6 = uVar16 + local_e0 * 4;
      uVar7 = uVar5 + local_e4 * 4;
      local_dc = local_74 << 5;
      iVar11 = local_fc * 0x20;
      do {
        if (uVar5 < uVar16) {
          FUN_0202cd68(uVar5,uVar16,iVar13);
        }
        else {
          FUN_0202cc94(uVar5,uVar16,iVar13);
        }
        if (uVar7 < uVar6) {
          FUN_0202cd68(uVar7,uVar6,iVar13);
        }
        else {
          FUN_0202cc94(uVar7,uVar6,iVar13);
        }
        uVar5 = uVar5 + (uVar1 + 1) * 0x20;
        uVar7 = uVar7 + local_dc;
        iVar19 = (iVar19 + -1) * 0x10000 >> 0x10;
        uVar6 = uVar6 + iVar11;
        uVar16 = uVar16 + iVar11;
      } while (0 < iVar19);
    }
  }
  iVar13 = (int)((local_f8 + local_124 * -2) * 0x10000) >> 0x10;
  iVar19 = local_fc + local_128 * -2;
  iVar11 = local_144 + (local_fc * local_124 + local_128) * 0x20;
  piVar12 = &local_150;
  if (iVar13 < 1) {
    piVar12 = (int *)0;
  }
  uVar17 = *(u32 *)(local_148 + 2);
  if (0 < iVar13) {
    uVar5 = local_fc << 0x13;
    do {
      *(u32 *)((int)piVar12 + 0x128) = uVar17;
      FUN_0203b808(*(u32 *)((int)piVar12 + 0x128),iVar11,iVar19 * 0x20);
      iVar13 = (iVar13 + -1) * 0x10000 >> 0x10;
      iVar11 = iVar11 + (uVar5 >> 0xe);
    } while (0 < iVar13);
    return;
  }
  return;
}

