/**
 * SDK I/O — memory-mapped I/O, DMA helpers, data copy (0x0202cxxx-0x0202exxx)
 * Decompiled from 0x0202cb34-0x0202ec28 region (49 functions)
 * Note: FUN_0202e86c has no Ghidra pseudocode available (48 implemented)
 */
#include "types.h"

/* ------------------------------------------------------------------ */
/*  External function declarations                                     */
/* ------------------------------------------------------------------ */
extern void FUN_0202a74c(void);
extern u32  FUN_0202cba8(void);
extern void FUN_020464b0(u32, u32);
extern void FUN_0203b7c0(u32, void *, u32);
extern void FUN_0202cd68(u32, u32, u32);
extern void FUN_0202cc94(u32, u32, u32);
extern void FUN_0203aa50(u32);
extern void FUN_0203aae8(u32, u32, u32, u32);
extern int  FUN_0203acb0(void);
extern void FUN_0203aca0(u32);
extern void FUN_0203b808(u32, u32, u32);
extern void FUN_02043f70(u32);
extern void FUN_02038ff4(u32);
extern void FUN_02038fd4(u32);
extern void FUN_020441e8(int);
extern int  FUN_02038e7c(void);
extern void FUN_02043ed8(int);
extern void FUN_0203b7dc(u32, u32, u32);
extern void FUN_0202fff8(void);
extern void FUN_0203c0ec(u32, u32, u32, u32);
extern u32  FUN_0203c5ac(void);
extern void FUN_0203c67c(u32);
extern void FUN_0203c5ec(u32);
extern int  FUN_0203c920(u32);
extern void FUN_0202f1c8(void);
extern void FUN_020304bc(void);
extern void FUN_02033708(void);
extern void FUN_0203c2fc(void);
extern void FUN_020412f4(void);
extern void FUN_020412dc(u32 *);
extern void FUN_0203056c(void);
extern void FUN_0202f33c(void);

/* ------------------------------------------------------------------ */
/*  External data references (DAT_ labels from Ghidra)                 */
/* ------------------------------------------------------------------ */
extern u32  DAT_0202cb80;
extern u32  DAT_0202d158;
extern u32  DAT_0202dec0;
extern u32 *DAT_0202dffc;
extern u32  DAT_0202e000;
extern u32  DAT_0202e004;
extern u32  DAT_0202e008;
extern u32  DAT_0202e00c;
extern u32  DAT_0202e01c;
extern u32 *DAT_0202e070;
extern u32  DAT_0202e074;
extern u32  DAT_0202e084;
extern u32  DAT_0202e0d0;
extern u32  DAT_0202e0d4;
extern u32  DAT_0202e25c;
extern u32  DAT_0202e260;
extern u32 *DAT_0202e264;
extern u32  DAT_0202e268;
extern u32  DAT_0202e26c;
extern u32  DAT_0202e270;
extern u32 *DAT_0202e304;
extern u32  DAT_0202e308;
extern u32  DAT_0202e30c;
extern u32  DAT_0202e310;
extern int (*DAT_0202e320)(int);
extern u16 *DAT_0202e354;
extern u16 *DAT_0202e358;
extern char *DAT_0202e3b8;
extern s16 *DAT_0202e3bc;
extern u32 *DAT_0202e68c;
extern u32 *DAT_0202e690;
extern u32  DAT_0202e6bc;
extern u32  DAT_0202ea54;
extern u32  DAT_0202dce0;
extern u32 *DAT_0202eb60;
extern u32  DAT_0202eb64;
extern u32 *DAT_0202eb68;
extern u32 *DAT_0202eb6c;
extern u32  DAT_0202eb70;
extern u32 *DAT_0202eb74;
extern u32 *DAT_0202eb78;
extern u32 *DAT_0202eb9c;
extern u32 *DAT_0202eba0;
extern u32 *DAT_0202eba4;
extern u32 *DAT_0202ebc4;
extern u32 *DAT_0202ec24;
extern u32 *DAT_0202ec48;

/* ------------------------------------------------------------------ */
/*  Forward declarations for functions in this file                    */
/* ------------------------------------------------------------------ */
u32 *FUN_0202cb34(u32 *param_1);
u32  FUN_0202cb84(u32 param_1);
u32  FUN_0202d100(u32 param_1, u32 param_2, int param_3);
s16 *FUN_0202d6e4(s16 *param_1, int *param_2, int *param_3, int *param_4, int *param_5);
void FUN_0202d7a0(s16 *param_1, int param_2, u16 *param_3, int param_4, int param_5);
s16 *FUN_0202d8c4(s16 *param_1, int param_2, u16 *param_3, int param_4, int param_5);
s16 *FUN_0202da0c(s16 *param_1, int param_2);
void FUN_0202db38(s16 *param_1, int *param_2, u32 param_3, int param_4, u32 param_5, int param_6);
u32  FUN_0202de90(void);
void FUN_0202dec4(void);
void FUN_0202e010(u32 param_1);
void FUN_0202e020(void);
u32  FUN_0202e078(void);
void FUN_0202e088(void);
void FUN_0202e0d8(u32 *param_1);
u32  FUN_0202e274(void);
int  FUN_0202e314(void);
void FUN_0202e324(u16 param_1);
int  FUN_0202e35c(void);
u32  FUN_0202e3c0(int param_1, int param_2);
u32  FUN_0202e3d4(u32 *param_1, int param_2);
void FUN_0202e3ec(u32 *param_1, int param_2);
void FUN_0202e458(int *param_1, int param_2, int param_3);
void FUN_0202e4d8(int *param_1, int param_2);
void FUN_0202e53c(int *param_1, int param_2);
void FUN_0202e5a4(int *param_1, int param_2);
void FUN_0202e5d0(u32 *param_1, u16 param_2);
void FUN_0202e5e8(u32 param_1);
void FUN_0202e604(u32 *param_1, u32 param_2, u32 param_3, u32 param_4, u16 param_5);
int  FUN_0202e694(u32 param_1);
int  FUN_0202e6c0(u32 param_1, u32 param_2);
u32  FUN_0202e730(int param_1, int param_2);
u32  FUN_0202e78c(int param_1, u32 param_2);
void FUN_0202e7e4(u32 param_1, u32 param_2);
void FUN_0202e824(int param_1, int param_2, int param_3);
u32  FUN_0202e878(int param_1, int param_2);
void FUN_0202e8c4(int param_1);
void FUN_0202e8f4(int param_1);
u32  FUN_0202e90c(u32 *param_1, int param_2, int param_3);
u32  FUN_0202e988(u32 *param_1, int param_2, int param_3);
int  FUN_0202ea08(int param_1, u32 param_2, u32 param_3);
void FUN_0202ea64(void);
void FUN_0202eaa0(void);
void FUN_0202ead4(void);
void FUN_0202eb7c(void);
void FUN_0202eba8(u32 param_1);
void FUN_0202ec0c(u32 param_1);
BOOL FUN_0202ec28(u32 param_1);

/* ================================================================== */
/*  Function implementations (address order)                           */
/* ================================================================== */

// FUN_0202cb34 @ 0x0202cb34 (76 bytes) — initialize struct with default values
u32 *FUN_0202cb34(u32 *param_1)
{
  FUN_0202a74c();
  *param_1 = DAT_0202cb80;
  param_1[0xb] = 0;
  param_1[10] = param_1[0xb];
  param_1[0xc] = 0xa8;
  *(u16 *)(param_1 + 0xd) = 5;
  *(u16 *)((int)param_1 + 0x36) = 2;
  *(u16 *)(param_1 + 0xe) = 0;
  return param_1;
}

// FUN_0202cb84 @ 0x0202cb84 (36 bytes) — division/modulo helper
u32 FUN_0202cb84(u32 param_1)
{
  u32 uVar1;
  u32 extraout_r1 = 0; /* secondary return via r1 register */

  uVar1 = FUN_0202cba8();
  FUN_020464b0(uVar1, param_1);
  return extraout_r1 & 0xffff;
}

// FUN_0202d100 @ 0x0202d100 (88 bytes) — color blend/interpolation
u32 FUN_0202d100(u32 param_1, u32 param_2, int param_3)
{
  u32 uVar1;
  u32 uVar2;

  uVar1 = (param_1 | param_1 << 0xf) & DAT_0202d158;
  uVar2 = uVar1 + (uVar1 >> 0x14);
  uVar2 = uVar2 + (uVar2 >> 10) >> 1;
  if ((uVar2 & 0x20) != 0) {
    uVar2 = uVar2 | 0x1f;
  }
  uVar2 = (DAT_0202d158 & (0x20 - param_3) * uVar1 >> 5) +
          (DAT_0202d158 &
          param_3 * (DAT_0202d158 &
                    (uVar2 & 0x1f) * ((param_2 | param_2 << 0xf) & DAT_0202d158) >> 5) >> 5);
  return (uVar2 | uVar2 * 0x8000) >> 0xf;
}

// FUN_0202d6e4 @ 0x0202d6e4 (176 bytes) — iterate entries, extract params
s16 *FUN_0202d6e4(s16 *param_1, int *param_2, int *param_3, int *param_4, int *param_5)
{
  int iVar1;
  s16 *psVar2;

  iVar1 = *param_2;
  psVar2 = param_1 + *param_1 * 3 + -2;
  param_1 = param_1 + 1;
  while (TRUE) {
    if (iVar1 < param_1[1] * 0x100) {
      *param_2 = iVar1;
      *param_3 = (int)param_1[1] << 8;
      *param_4 = (int)*param_1;
      *param_5 = (int)param_1[3];
      return psVar2;
    }
    iVar1 = iVar1 + param_1[2] * -0x100;
    if ((iVar1 < 0) || (param_1 == psVar2)) break;
    param_1 = param_1 + 3;
  }
  *param_2 = 1;
  *param_3 = 1;
  *param_4 = (int)*param_1;
  *param_5 = (int)param_1[3];
  return psVar2;
}

// FUN_0202d7a0 @ 0x0202d7a0 (280 bytes) — waveform sample copy with ping-pong loop
void FUN_0202d7a0(s16 *param_1, int param_2, u16 *param_3, int param_4, int param_5)
{
  int iVar1;
  u16 *puVar2;
  int iVar3;
  int iVar4;
  u16 *puVar5;
  BOOL bVar6;
  u16 auStack_220[258];

  iVar4 = (int)param_1[1];
  iVar1 = iVar4 * 2;
  FUN_0203b7c0(param_5 + *param_1 * 2, auStack_220, iVar1);
  for (iVar3 = 0; iVar3 != iVar1; iVar3 = iVar3 + 1) {
    param_2 = param_2 + param_1[iVar3 + 2] * -0x100;
    if (param_2 < 0) goto LAB_0202d800;
  }
  iVar3 = 0;
LAB_0202d800:
  if (iVar4 + -1 < iVar3) {
    iVar3 = (iVar1 + -1) - iVar3;
    puVar5 = param_3;
    do {
      param_4 = param_4 + -1;
      if (param_4 < 0) {
        return;
      }
      puVar2 = auStack_220 + iVar3;
      bVar6 = iVar3 != 0;
      if (bVar6) {
        iVar3 = iVar3 + -1;
      }
      *puVar5 = *puVar2;
      param_3 = puVar5 + 1;
      puVar5 = puVar5 + 1;
    } while (bVar6);
  }
  do {
    while (TRUE) {
      param_4 = param_4 + -1;
      if (param_4 < 0) {
        return;
      }
      *param_3 = auStack_220[iVar3];
      puVar5 = param_3 + 1;
      if (iVar3 == iVar4 + -1) break;
      iVar3 = iVar3 + 1;
      param_3 = param_3 + 1;
    }
    while (TRUE) {
      param_4 = param_4 + -1;
      if (param_4 < 0) {
        return;
      }
      param_3 = puVar5 + 1;
      *puVar5 = auStack_220[iVar3];
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      puVar5 = param_3;
    }
  } while (TRUE);
}

// FUN_0202d8c4 @ 0x0202d8c4 (148 bytes) — looping sample copy
s16 *FUN_0202d8c4(s16 *param_1, int param_2, u16 *param_3, int param_4, int param_5)
{
  u16 *puVar1;
  int iVar2;
  int iVar3;
  u16 *puVar4;
  u16 auStack_218[256];

  iVar3 = (int)param_1[1];
  FUN_0203b7c0(param_5 + *param_1 * 2, auStack_218, iVar3 << 1);
  for (iVar2 = 0; iVar2 != iVar3; iVar2 = iVar2 + 1) {
    param_2 = param_2 + param_1[iVar2 + 2] * -0x100;
    if (param_2 < 0) goto LAB_0202d920;
  }
  iVar2 = 0;
LAB_0202d920:
  while (TRUE) {
    param_4 = param_4 + -1;
    if (param_4 < 0) break;
    puVar1 = auStack_218 + iVar2;
    iVar2 = iVar2 + 1;
    puVar4 = param_3 + 1;
    *param_3 = *puVar1;
    param_3 = puVar4;
    if (iVar2 == iVar3) {
      iVar2 = 0;
    }
  }
  return param_1 + iVar3 + 2;
}

// FUN_0202da0c @ 0x0202da0c (60 bytes) — lookup entry in table by index
s16 *FUN_0202da0c(s16 *param_1, int param_2)
{
  int iVar1;

  param_2 = param_2 + 3;
  if (1 < param_2) {
    if (*param_1 <= param_2) {
      param_2 = 2;
    }
    iVar1 = (int)param_1[param_2];
    if (iVar1 == 0) {
      iVar1 = (int)param_1[2];
    }
    if (iVar1 == 0) {
      param_1 = (s16 *)0x0;
    }
    else {
      param_1 = param_1 + iVar1;
    }
    return param_1;
  }
  return (s16 *)0x0;
}

// FUN_0202db38 @ 0x0202db38 (412 bytes) — process animation/sequence entries
void FUN_0202db38(s16 *param_1, int *param_2, u32 param_3, int param_4, u32 param_5, int param_6)
{
  s16 sVar1;
  s16 sVar2;
  s16 *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int extraout_r1 = 0;    /* secondary return via r1 register */
  int extraout_r1_00 = 0; /* secondary return via r1 register */
  s16 *psVar8;
  int iVar9;
  int *piVar10;
  s16 *psStack_2c;

  if (((param_3 != 0) && (param_5 != 0)) && (param_5 != param_3)) {
    if (param_5 < param_3) {
      FUN_0202cd68(param_5, param_3, param_4 << 1);
    }
    else {
      FUN_0202cc94(param_5, param_3, param_4 << 1);
    }
  }
  if (param_1 == (s16 *)0x0) {
    return;
  }
  iVar9 = 0;
  psStack_2c = param_1;
LAB_0202dbbc:
  do {
    psVar3 = psStack_2c + 1;
    if (*psStack_2c == -1) {
      /* UNRECOVERED_JUMPTABLE: indirect jump, return with iVar9 */
      return;
    }
    psVar8 = param_1 + *psStack_2c;
    iVar4 = (int)psVar8[1];
    if (iVar4 < 0) {
      iVar4 = -iVar4;
    }
    if (iVar9 < iVar4) {
      iVar9 = iVar4;
    }
    psStack_2c = psVar3;
  } while (param_3 == 0);
  sVar1 = psVar8[2];
  sVar2 = *psVar8;
  iVar4 = psVar8[1] * 0x100;
  iVar7 = (int)psVar8[4] << 8;
  iVar5 = psVar8[3] * 0x100;
  piVar10 = param_2 + 1;
  iVar6 = *param_2;
  param_2 = piVar10;
  if (param_6 == 0) goto code_r0x0202dc3c;
  if (iVar4 < 0) {
    iVar7 = iVar6;
    if (psVar8[1] * -0x100 < iVar6) {
      iVar7 = iVar5;
    }
  }
  else {
    FUN_020464b0(iVar6, iVar4);
    iVar6 = extraout_r1;
    if (extraout_r1 < 0) {
      iVar6 = extraout_r1 + iVar4;
    }
    iVar6 = iVar6 + sVar1 * -0x100;
    if (iVar5 <= iVar6) goto LAB_0202dc9c;
    if (iVar6 < 0) goto LAB_0202dbbc;
    FUN_020464b0(iVar6, iVar7);
    iVar7 = extraout_r1_00;
  }
  goto LAB_0202dc9c;
code_r0x0202dc3c:
  iVar7 = iVar6 + sVar1 * -0x100;
  if (iVar7 < 0) goto LAB_0202dbbc;
  if (iVar5 < iVar7) {
    iVar7 = iVar5;
  }
LAB_0202dc9c:
  iVar5 = param_4 - psVar8[5];
  iVar4 = (int)psVar8[6];
  if (iVar5 < psVar8[6]) {
    iVar4 = iVar5;
  }
  (*(void (**)(s16 *, int, u32, int))(DAT_0202dce0 + sVar2 * 4))(psVar8 + 7, iVar7, param_3 + psVar8[5] * 2, iVar4);
  goto LAB_0202dbbc;
}

// FUN_0202de90 @ 0x0202de90 (48 bytes) — stop I/O and invoke callback
u32 FUN_0202de90(void)
{
  FUN_0202e088();
  if (*(void (**)(void))(DAT_0202dec0 + 0x48) != (void (*)(void))0x0) {
    (*(void (**)(void))(DAT_0202dec0 + 0x48))();
  }
  return 0;
}

// FUN_0202dec4 @ 0x0202dec4 (312 bytes) — state machine tick for DMA transfer
void FUN_0202dec4(void)
{
  u32 *puVar1;
  u32 *puVar2;
  u32 uVar3;

  puVar2 = DAT_0202dffc;
  if (*DAT_0202dffc == 0) {
    uVar3 = DAT_0202dffc[0x11];
    puVar1 = DAT_0202dffc + 0x10;
    DAT_0202dffc[0x11] = uVar3 + 1;
    if ((*puVar1 != 0) && (*puVar1 <= uVar3 + 1)) {
      puVar2[0xf] = 0;
      return;
    }
  }
  if (*DAT_0202dffc == DAT_0202dffc[1]) {
    FUN_0202e324(0);
    FUN_0203aae8(DAT_0202e000, DAT_0202dffc[2], 0, DAT_0202e004);
    *DAT_0202dffc = 0;
    return;
  }
  if ((*DAT_0202dffc & 1) == 0) {
    FUN_0202e324(2);
    FUN_0203aae8(DAT_0202e000, *(u32 *)(DAT_0202e00c + (*DAT_0202dffc >> 1) * 4), 0,
                 DAT_0202e004);
    *DAT_0202dffc = *DAT_0202dffc + 1;
    return;
  }
  FUN_0202e324(0);
  FUN_0203aae8(DAT_0202e000, *(u32 *)(DAT_0202e008 + (*DAT_0202dffc >> 1) * 4), 0, DAT_0202e004);
  *DAT_0202dffc = *DAT_0202dffc + 1;
  return;
}

// FUN_0202e010 @ 0x0202e010 (12 bytes) — set callback pointer
void FUN_0202e010(u32 param_1)
{
  *(u32 *)(DAT_0202e01c + 0x48) = param_1;
  return;
}

// FUN_0202e020 @ 0x0202e020 (80 bytes) — poll and advance DMA state
void FUN_0202e020(void)
{
  if (DAT_0202e070[0xf] == 0) {
    return;
  }
  if ((*DAT_0202e070 & 1) != 0) {
    FUN_0203aa50(DAT_0202e074);
    FUN_0202dec4();
    return;
  }
  return;
}

// FUN_0202e078 @ 0x0202e078 (12 bytes) — read status word
u32 FUN_0202e078(void)
{
  return *(u32 *)(DAT_0202e084 + 0x3c);
}

// FUN_0202e088 @ 0x0202e088 (72 bytes) — stop active transfer
void FUN_0202e088(void)
{
  if (*(int *)(DAT_0202e0d0 + 0x3c) != 0) {
    FUN_0203aa50(DAT_0202e0d4);
    FUN_0202e324(0);
    *(u32 *)(DAT_0202e0d0 + 0x3c) = 0;
    return;
  }
  return;
}

// FUN_0202e0d8 @ 0x0202e0d8 (388 bytes) — configure and start multi-step DMA
void FUN_0202e0d8(u32 *param_1)
{
  u32 *puVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;

  iVar2 = FUN_0202e314();
  if (iVar2 == 0) {
    return;
  }
  uVar4 = *param_1;
  uVar5 = 0;
  if (uVar4 != 0) {
    do {
      if (0xf < param_1[uVar5 + 2]) {
        return;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < uVar4);
  }
  uVar5 = 0;
  if (uVar4 != 1) {
    do {
      if (param_1[uVar5 + 8] < param_1[uVar5 + 2]) {
        return;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < uVar4 - 1);
  }
  if (0xe < param_1[1]) {
    FUN_0202e088();
    puVar1 = DAT_0202e264;
    uVar4 = DAT_0202e260;
    iVar2 = DAT_0202e25c;
    DAT_0202e264[2] =
         (u32)((u64)DAT_0202e260 * (u64)(param_1[1] * DAT_0202e25c >> 6) >> 0x23) -
         0x13;
    puVar1[0x10] = param_1[0xe];
    puVar1[0x11] = 0;
    *puVar1 = 1;
    uVar5 = 0;
    do {
      puVar1[uVar5 + 3] =
           (u32)((u64)uVar4 * (u64)(param_1[uVar5 + 2] * iVar2 >> 6) >> 0x23) - 0x13;
      uVar3 = uVar5 + 1;
      puVar1[uVar5 + 9] =
           (u32)((u64)uVar4 * (u64)(param_1[uVar5 + 8] * iVar2 >> 6) >> 0x23) - 0x13;
      uVar5 = uVar3;
    } while (uVar3 < 6);
    puVar1[1] = *param_1 * 2 - 1;
    FUN_0202e324(2);
    FUN_0203aae8(DAT_0202e26c, *(u32 *)(DAT_0202e268 + (*DAT_0202e264 >> 1) * 4), 0,
                 DAT_0202e270);
    DAT_0202e264[0xf] = 1;
    return;
  }
  return;
}

// FUN_0202e274 @ 0x0202e274 (144 bytes) — initialize I/O subsystem
u32 FUN_0202e274(void)
{
  u32 uVar1;
  int iVar2;

  if (*DAT_0202e304 != 0) {
    uVar1 = FUN_0202e314();
    return uVar1;
  }
  *DAT_0202e304 = 1;
  iVar2 = FUN_0203acb0();
  if ((iVar2 != 0) && (iVar2 = FUN_0202e35c(), iVar2 != 0)) {
    FUN_0203aca0(DAT_0202e308);
    FUN_0203b808(0, DAT_0202e30c, 0x4c);
    FUN_02043f70(DAT_0202e310);
    return 1;
  }
  return 0;
}

// FUN_0202e314 @ 0x0202e314 (12 bytes) — indirect call via function pointer
int FUN_0202e314(void)
{
  return (*DAT_0202e320)(2);
}

// FUN_0202e324 @ 0x0202e324 (48 bytes) — set transfer mode register
void FUN_0202e324(u16 param_1)
{
  u16 *puVar1;

  FUN_02038ff4((u32)*DAT_0202e354);
  puVar1 = DAT_0202e354;
  *DAT_0202e358 = param_1;
  FUN_02038fd4((u32)*puVar1);
  return;
}

// FUN_0202e35c @ 0x0202e35c (92 bytes) — scan config string and init device
int FUN_0202e35c(void)
{
  char cVar1;
  int iVar2;
  char *pcVar3;
  u32 uVar4;

  uVar4 = 0;
  pcVar3 = DAT_0202e3b8;
  do {
    uVar4 = uVar4 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (uVar4 < 0x13);
  FUN_020441e8((int)cVar1);
  iVar2 = FUN_02038e7c();
  if (iVar2 != -3) {
    *DAT_0202e3bc = (s16)iVar2;
    FUN_02043ed8(2);
    return 1;
  }
  return 0;
}

// FUN_0202e3c0 @ 0x0202e3c0 (20 bytes) — get node value from linked structure
u32 FUN_0202e3c0(int param_1, int param_2)
{
  u32 uVar1;

  if (param_2 == 0) {
    uVar1 = *(u32 *)(param_1 + 4);
  }
  else {
    uVar1 = *(u32 *)(param_2 + (u32)*(u16 *)(param_1 + 10));
  }
  return uVar1;
}

// FUN_0202e3d4 @ 0x0202e3d4 (24 bytes) — get next node in linked list
u32 FUN_0202e3d4(u32 *param_1, int param_2)
{
  u32 uVar1;

  if (param_2 == 0) {
    uVar1 = *param_1;
  }
  else {
    uVar1 = *(u32 *)(param_2 + (u32)*(u16 *)((int)param_1 + 10) + 4);
  }
  return uVar1;
}

// FUN_0202e3ec @ 0x0202e3ec (108 bytes) — unlink node from doubly-linked list
void FUN_0202e3ec(u32 *param_1, int param_2)
{
  u32 uVar1;
  u32 *puVar2;

  uVar1 = (u32)*(u16 *)((int)param_1 + 10);
  puVar2 = (u32 *)(param_2 + uVar1);
  if (*(int *)(param_2 + uVar1) == 0) {
    *param_1 = puVar2[1];
  }
  else {
    *(u32 *)(*(int *)(param_2 + uVar1) + uVar1 + 4) = puVar2[1];
  }
  if (puVar2[1] == 0) {
    param_1[1] = *puVar2;
  }
  else {
    *(u32 *)(puVar2[1] + (u32)*(u16 *)((int)param_1 + 10)) = *puVar2;
  }
  *puVar2 = 0;
  puVar2[1] = 0;
  *(s16 *)(param_1 + 2) = *(s16 *)(param_1 + 2) + -1;
  return;
}

// FUN_0202e458 @ 0x0202e458 (128 bytes) — insert node into doubly-linked list
void FUN_0202e458(int *param_1, int param_2, int param_3)
{
  int iVar1;
  u32 uVar2;

  if (param_2 == 0) {
    FUN_0202e53c(param_1, param_3);
    return;
  }
  if (param_2 == *param_1) {
    FUN_0202e4d8(param_1, param_3);
    return;
  }
  uVar2 = (u32)*(u16 *)((int)param_1 + 10);
  iVar1 = *(int *)(param_2 + uVar2);
  *(int *)(param_3 + uVar2) = iVar1;
  *(int *)(param_3 + uVar2 + 4) = param_2;
  *(int *)(iVar1 + uVar2 + 4) = param_3;
  *(int *)(param_2 + (u32)*(u16 *)((int)param_1 + 10)) = param_3;
  *(s16 *)(param_1 + 2) = (s16)param_1[2] + 1;
  return;
}

// FUN_0202e4d8 @ 0x0202e4d8 (100 bytes) — prepend node to head of list
void FUN_0202e4d8(int *param_1, int param_2)
{
  u16 uVar1;

  if (*param_1 == 0) {
    FUN_0202e5a4(param_1, param_2);
    return;
  }
  uVar1 = *(u16 *)((int)param_1 + 10);
  *(u32 *)(param_2 + (u32)uVar1) = 0;
  *(int *)(param_2 + (u32)uVar1 + 4) = *param_1;
  *(int *)(*param_1 + (u32)*(u16 *)((int)param_1 + 10)) = param_2;
  *param_1 = param_2;
  *(s16 *)(param_1 + 2) = (s16)param_1[2] + 1;
  return;
}

// FUN_0202e53c @ 0x0202e53c (104 bytes) — append node to tail of list
void FUN_0202e53c(int *param_1, int param_2)
{
  u16 uVar1;

  if (*param_1 == 0) {
    FUN_0202e5a4(param_1, param_2);
    return;
  }
  uVar1 = *(u16 *)((int)param_1 + 10);
  *(int *)(param_2 + (u32)uVar1) = param_1[1];
  *(u32 *)(param_2 + (u32)uVar1 + 4) = 0;
  *(int *)(param_1[1] + (u32)*(u16 *)((int)param_1 + 10) + 4) = param_2;
  param_1[1] = param_2;
  *(s16 *)(param_1 + 2) = (s16)param_1[2] + 1;
  return;
}

// FUN_0202e5a4 @ 0x0202e5a4 (44 bytes) — initialize list with single node
void FUN_0202e5a4(int *param_1, int param_2)
{
  u16 uVar1;

  uVar1 = *(u16 *)((int)param_1 + 10);
  *(u32 *)(param_2 + (u32)uVar1 + 4) = 0;
  *(u32 *)(param_2 + (u32)uVar1) = 0;
  *param_1 = param_2;
  param_1[1] = param_2;
  *(s16 *)(param_1 + 2) = (s16)param_1[2] + 1;
  return;
}

// FUN_0202e5d0 @ 0x0202e5d0 (24 bytes) — reset linked list header
void FUN_0202e5d0(u32 *param_1, u16 param_2)
{
  *param_1 = 0;
  param_1[1] = 0;
  *(u16 *)(param_1 + 2) = 0;
  *(u16 *)((int)param_1 + 10) = param_2;
  return;
}

// FUN_0202e5e8 @ 0x0202e5e8 (28 bytes) — remove node from its owner list
void FUN_0202e5e8(u32 param_1)
{
  int uVar1;

  uVar1 = FUN_0202e694(param_1);
  FUN_0202e3ec((u32 *)uVar1, param_1);
  return;
}

// FUN_0202e604 @ 0x0202e604 (136 bytes) — register a memory region
void FUN_0202e604(u32 *param_1, u32 param_2, u32 param_3, u32 param_4, u16 param_5)
{
  int uVar1;

  *param_1 = param_2;
  param_1[6] = param_3;
  param_1[7] = param_4;
  param_1[8] = 0;
  param_1[8] = param_1[8] & 0xffffff00;
  param_1[8] = param_1[8] | param_5 & 0xff;
  FUN_0202e5d0(param_1 + 3, 4);
  if (*DAT_0202e68c == 0) {
    FUN_0202e5d0(DAT_0202e690, 4);
    *DAT_0202e68c = 1;
  }
  uVar1 = FUN_0202e694((u32)param_1);
  FUN_0202e53c((int *)uVar1, (int)param_1);
  return;
}

// FUN_0202e694 @ 0x0202e694 (40 bytes) — find owning region for address
int FUN_0202e694(u32 param_1)
{
  int iVar1;
  int iVar2;

  iVar2 = DAT_0202e6bc;
  iVar1 = FUN_0202e6c0(DAT_0202e6bc, param_1);
  if (iVar1 != 0) {
    iVar2 = iVar1 + 0xc;
  }
  return iVar2;
}

// FUN_0202e6c0 @ 0x0202e6c0 (112 bytes) — recursive region lookup
int FUN_0202e6c0(u32 param_1, u32 param_2)
{
  int iVar1;
  int iVar2;

  iVar1 = FUN_0202e3d4((u32 *)param_1, 0);
  while (TRUE) {
    if (iVar1 == 0) {
      return 0;
    }
    if ((*(u32 *)(iVar1 + 0x18) <= param_2) && (param_2 < *(u32 *)(iVar1 + 0x1c))) break;
    iVar1 = FUN_0202e3d4((u32 *)param_1, iVar1);
  }
  iVar2 = FUN_0202e6c0(iVar1 + 0xc, param_2);
  if (iVar2 == 0) {
    iVar2 = iVar1;
  }
  return iVar2;
}

// FUN_0202e730 @ 0x0202e730 (92 bytes) — walk allocation chain
u32 FUN_0202e730(int param_1, int param_2)
{
  int *piVar1;

  piVar1 = *(int **)(param_1 + 0x2c);
  if (param_2 != 0) {
    for (; (piVar1 != (int *)0x0 && (*piVar1 != param_2)); piVar1 = (int *)piVar1[3]) {
    }
  }
  if (piVar1 != (int *)0x0) {
    *(int *)(param_1 + 0x24) = piVar1[1];
    *(int *)(param_1 + 0x28) = piVar1[2];
    *(int *)(param_1 + 0x2c) = piVar1[3];
    return 1;
  }
  return 0;
}

// FUN_0202e78c @ 0x0202e78c (88 bytes) — push allocation record
u32 FUN_0202e78c(int param_1, u32 param_2)
{
  u32 *puVar1;
  u32 uVar2;

  uVar2 = *(u32 *)(param_1 + 0x24);
  puVar1 = (u32 *)FUN_0202e988((u32 *)(param_1 + 0x24), 0x10, 4);
  if (puVar1 != (u32 *)0x0) {
    *puVar1 = param_2;
    puVar1[1] = uVar2;
    puVar1[2] = *(u32 *)(param_1 + 0x28);
    puVar1[3] = *(u32 *)(param_1 + 0x2c);
    *(u32 **)(param_1 + 0x2c) = puVar1;
    return 1;
  }
  return 0;
}

// FUN_0202e7e4 @ 0x0202e7e4 (64 bytes) — reset allocator state
void FUN_0202e7e4(u32 param_1, u32 param_2)
{
  if ((param_2 & 1) != 0) {
    FUN_0202e8f4(param_1);
  }
  if ((param_2 & 2) != 0) {
    FUN_0202e8c4(param_1);
    return;
  }
  return;
}

// FUN_0202e824 @ 0x0202e824 (72 bytes) — resize heap allocation
void FUN_0202e824(int param_1, int param_2, int param_3)
{
  if (param_2 == 0) {
    param_2 = 1;
  }
  if (-1 < param_3) {
    FUN_0202e988((u32 *)(param_1 + 0x24), param_2 + 3U & 0xfffffffc, param_3);
    return;
  }
  FUN_0202e90c((u32 *)(param_1 + 0x24), param_2 + 3U & 0xfffffffc, -param_3);
  return;
}

// FUN_0202e878 @ 0x0202e878 (76 bytes) — create heap from aligned range
u32 FUN_0202e878(int param_1, int param_2)
{
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;

  uVar3 = (u32)(param_2 + param_1) & 0xfffffffc;
  uVar1 = (u32)(param_1 + 3U) & 0xfffffffc;
  if ((uVar1 <= uVar3) && (0x2f < uVar3 - uVar1)) {
    uVar2 = FUN_0202ea08(uVar1, uVar3 - uVar1, 0);
    return uVar2;
  }
  return 0;
}

// FUN_0202e8c4 @ 0x0202e8c4 (48 bytes) — propagate end pointer to all blocks
void FUN_0202e8c4(int param_1)
{
  int iVar1;

  for (iVar1 = *(int *)(param_1 + 0x2c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0xc)) {
    *(u32 *)(iVar1 + 8) = *(u32 *)(param_1 + 0x1c);
  }
  *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x1c);
  return;
}

// FUN_0202e8f4 @ 0x0202e8f4 (24 bytes) — reset allocation pointer to base
void FUN_0202e8f4(int param_1)
{
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x18);
  *(u32 *)(param_1 + 0x2c) = 0;
  return;
}

// FUN_0202e90c @ 0x0202e90c (124 bytes) — shrink heap from end
u32 FUN_0202e90c(u32 *param_1, int param_2, int param_3)
{
  u32 uVar1;

  uVar1 = ~(param_3 - 1U) & param_1[1] - param_2;
  if (*param_1 <= uVar1) {
    if ((param_1[-1] & 1) != 0) {
      FUN_0203b7dc(0, uVar1, param_1[1] - uVar1);
    }
    param_1[1] = uVar1;
    return uVar1;
  }
  return 0;
}

// FUN_0202e988 @ 0x0202e988 (128 bytes) — grow heap from front
u32 FUN_0202e988(u32 *param_1, int param_2, int param_3)
{
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;

  uVar1 = *param_1;
  uVar3 = ~(param_3 - 1U) & (param_3 - 1U) + uVar1;
  uVar2 = param_2 + uVar3;
  if (uVar2 <= param_1[1]) {
    if ((param_1[-1] & 1) != 0) {
      FUN_0203b7dc(0, uVar1, uVar2 - uVar1);
    }
    *param_1 = uVar2;
    return uVar3;
  }
  return 0;
}

// FUN_0202ea08 @ 0x0202ea08 (76 bytes) — create and init heap structure
int FUN_0202ea08(int param_1, u32 param_2, u32 param_3)
{
  FUN_0202e604((u32 *)param_1, DAT_0202ea54, param_1 + 0x30, param_2, (u16)param_3);
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x18);
  *(u32 *)(param_1 + 0x28) = *(u32 *)(param_1 + 0x1c);
  *(u32 *)(param_1 + 0x2c) = 0;
  return param_1;
}

// FUN_0202ea64 @ 0x0202ea64 (60 bytes) — system init sequence
void FUN_0202ea64(void)
{
  u32 uVar1;

  FUN_0202fff8();
  FUN_0203c0ec(0, 0, 0, 0);
  uVar1 = FUN_0203c5ac();
  FUN_0203c67c(1);
  FUN_0203c5ec(uVar1);
  return;
}

// FUN_0202eaa0 @ 0x0202eaa0 (52 bytes) — shutdown sequence
void FUN_0202eaa0(void)
{
  int iVar1;

  do {
    iVar1 = FUN_0203c920(0);
  } while (iVar1 != 0);
  FUN_0202f1c8();
  FUN_020304bc();
  FUN_02033708();
  FUN_0203c67c(0);
  return;
}

// FUN_0202ead4 @ 0x0202ead4 (140 bytes) — one-time system bootstrap
void FUN_0202ead4(void)
{
  u32 *puVar1;
  u32 *puVar2;
  u32 uVar3;

  if (*DAT_0202eb60 == 0) {
    *DAT_0202eb60 = 1;
    FUN_0203c2fc();
    uVar3 = DAT_0202eb70;
    puVar2 = DAT_0202eb6c;
    puVar1 = DAT_0202eb68;
    *DAT_0202eb68 = DAT_0202eb64;
    puVar1[1] = 0;
    *puVar2 = uVar3;
    puVar2[1] = 0;
    FUN_020412f4();
    FUN_020412dc(DAT_0202eb6c);
    FUN_0202eb7c();
    FUN_0203056c();
    FUN_0202f33c();
    puVar1 = DAT_0202eb78;
    *DAT_0202eb74 = 0xff;
    *puVar1 = 1;
    return;
  }
  return;
}

// FUN_0202eb7c @ 0x0202eb7c (32 bytes) — clear three globals
void FUN_0202eb7c(void)
{
  u32 *puVar1;
  u32 *puVar2;

  puVar2 = DAT_0202eba4;
  puVar1 = DAT_0202eba0;
  *DAT_0202eb9c = 0;
  *puVar1 = 0;
  *puVar2 = 0;
  return;
}

// FUN_0202eba8 @ 0x0202eba8 (28 bytes) — clear single bit in bitmask
void FUN_0202eba8(u32 param_1)
{
  *DAT_0202ebc4 = *DAT_0202ebc4 & ~(1 << (param_1 & 0xff));
  return;
}

// FUN_0202ec0c @ 0x0202ec0c (24 bytes) — clear bits in mask
void FUN_0202ec0c(u32 param_1)
{
  *DAT_0202ec24 = *DAT_0202ec24 & ~param_1;
  return;
}

// FUN_0202ec28 @ 0x0202ec28 (32 bytes) — test-and-set bits in mask
BOOL FUN_0202ec28(u32 param_1)
{
  BOOL bVar1;

  bVar1 = (param_1 & *DAT_0202ec48) == 0;
  if (bVar1) {
    *DAT_0202ec48 = *DAT_0202ec48 | param_1;
  }
  return bVar1;
}
