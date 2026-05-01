/**
 * OS Core A - OS services, threading, interrupts (0x02030xxx-0x02033xxx)
 * Decompiled from 0x02030058-0x02033248 region (58 functions)
 *
 * Compile: arm-none-eabi-gcc -c -mcpu=arm946e-s -mthumb-interwork -O2
 *          -std=gnu89 -I arm9/include -o build/os_core_a.o arm9/src/os_core_a.c
 */
#include "types.h"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/* Ghidra unrecovered jump table stubs */
typedef u32 (*JumpTableFn)(u32);
static JumpTableFn UNRECOVERED_JUMPTABLE_00 = (JumpTableFn)0;
static JumpTableFn UNRECOVERED_JUMPTABLE = (JumpTableFn)0;

/* ------------------------------------------------------------------ */
/*  External data references                                          */
/* ------------------------------------------------------------------ */
extern u32 *DAT_0203014c;
extern u32 DAT_02030150;
extern u32 *DAT_020304b0;
extern u32 DAT_020304b4;
extern u32 DAT_020304b8;
extern u32 *DAT_0203060c;
extern u32 *DAT_02030610;
extern u32 DAT_02030614;
extern u32 DAT_02030618;
extern u32 DAT_0203061c;
extern u32 DAT_02030620;
extern u32 DAT_02030624;
extern u32 *DAT_0203066c;
extern u32 *DAT_0203072c;
extern u32 *DAT_020308d0;
extern u32 *DAT_020309a4;
extern u32 *DAT_02030a08;
extern u32 *DAT_02030a6c;
extern u32 *DAT_02030ad0;
extern u32 *DAT_02030b34;
extern u32 *DAT_02030c60;
extern u32 *DAT_02030cd4;
extern u32 *DAT_02030cec;
extern u32 DAT_02030f00;
extern u32 DAT_02030f04;
extern u32 DAT_02030f08;
extern u32 *DAT_02030f90;
extern u32 (*DAT_02031054)();
extern u32 DAT_02031550;
extern u32 DAT_02031810;
extern u32 DAT_02031814;
extern u32 DAT_02031890;
extern u32 DAT_0203190c;
extern u32 DAT_02031988;
extern u32 DAT_02031a04;
extern u32 DAT_02031cc0;
extern u32 DAT_02032380;
extern u32 DAT_02032384;
extern u32 DAT_02032388;
extern u32 DAT_02032c2c;
extern u32 DAT_02032c30;
extern u32 *DAT_02032c34;
extern u32 DAT_02032c38;
extern u32 DAT_02032c3c;
extern u32 DAT_02032d88;
extern u32 DAT_02032ecc;
extern u32 DAT_02032ed0;
extern u32 *DAT_02032ed4;
extern u32 DAT_02032ed8;
extern u32 DAT_02032edc;
extern u32 DAT_02032f80;
extern u32 *DAT_02032f84;
extern u32 DAT_02032f88;
extern u32 DAT_02032fc0;
extern u32 DAT_0203300c;
extern u32 DAT_02033140;
extern u32 DAT_02033144;
extern u32 DAT_02033148;
extern u32 DAT_0203314c;
extern u32 DAT_0203323c;
extern u32 *DAT_02033240;
extern u32 DAT_02033244;
extern u32 DAT_020332dc;
extern u32 *DAT_020332e0;
extern u32 DAT_020332e4;

/* ------------------------------------------------------------------ */
/*  External function declarations                                    */
/* ------------------------------------------------------------------ */
extern u32 FUN_0202e3d4(...);
extern u32 FUN_0202e3ec(...);
extern u32 FUN_0202e53c(...);
extern u32 FUN_0202e5d0(...);
extern u32 FUN_0202e730(...);
extern u32 FUN_0202e78c(...);
extern u32 FUN_0202e824(...);
extern u32 FUN_0202eba8(...);
extern u32 FUN_0202ebc8(...);
extern u32 FUN_0202ec0c(...);
extern u32 FUN_0202ec28(...);
extern u32 FUN_0202ec4c(...);
extern u32 FUN_0202ec84(...);
extern u32 FUN_0202efe8(...);
extern u32 FUN_0202f0a4(...);
extern u32 FUN_0202f10c(...);
extern u32 FUN_0202f410(...);
extern u32 FUN_0202f438(...);
extern u32 FUN_0202f494(...);
extern u32 FUN_0202f50c(...);
extern u32 FUN_0202f69c(...);
extern u32 FUN_0202f73c(...);
extern u32 FUN_0202f754(...);
extern u32 FUN_0202fa74(...);
extern u32 FUN_0202fcd0(...);
extern u32 FUN_0202fd00(...);
extern u32 FUN_02030870(...);
extern u32 FUN_0203088c(...);
extern u32 FUN_020308d4(...);
extern u32 FUN_02030958(...);
extern u32 FUN_02030b38(...);
extern u32 FUN_02030b9c(...);
extern u32 FUN_02030c64(...);
extern u32 FUN_020311a8(...);
extern u32 FUN_02033624(...);
extern u32 FUN_020341bc(...);
extern u32 FUN_020341ec(...);
extern u32 FUN_020394a0(...);
extern u32 FUN_020394d4(...);
extern u32 FUN_02039548(...);
extern u32 FUN_0203960c(...);
extern u32 FUN_02039bf0(...);
extern u32 FUN_02039d44(...);
extern u32 FUN_02039e98(...);
extern u32 FUN_02039f08(...);
extern u32 FUN_02039f94(...);
extern u32 FUN_0203a030(...);
extern u32 FUN_0203a04c(...);
extern u32 FUN_0203ae38(...);
extern u32 FUN_0203ae4c(...);
extern u32 FUN_0203b7dc(...);
extern u32 FUN_0203b914(...);
extern u32 FUN_0203b9a8(...);
extern u32 FUN_0203be94(...);
extern u32 FUN_0203bec8(...);
extern u32 FUN_0203bef4(...);
extern u32 FUN_0203bf20(...);
extern u32 FUN_0203bf4c(...);
extern u32 FUN_0203c05c(...);
extern u32 FUN_0203c0a0(...);
extern u32 FUN_0203c0ec(...);
extern u32 FUN_0203c15c(...);
extern u32 FUN_0203c5ac(...);
extern u32 FUN_0203c5ec(...);
extern u32 FUN_0203c67c(...);
extern u32 FUN_0203cda8(...);
extern u32 FUN_0203cdf0(...);
extern u32 FUN_0203ce28(...);
extern u32 FUN_0203ce30(...);
extern u32 FUN_0203d008(...);
extern u32 FUN_0203d220(...);
extern u32 FUN_0203d26c(...);
extern u32 FUN_0203d30c(...);
extern u32 FUN_0203ed60(...);
extern u32 FUN_0203edcc(...);
extern u32 FUN_0203edec(...);
extern u32 FUN_0203ef08(...);
extern u32 FUN_0203efa0(...);
extern u32 FUN_0203f074(...);
extern u32 FUN_0203f2a0(...);
extern u32 FUN_020464b0(...);
extern u32 FUN_020466bc(...);
extern u32 thunk_FUN_0202ed98(...);

/* ------------------------------------------------------------------ */
/*  Forward declarations (C89 unspecified params for compat)           */
/* ------------------------------------------------------------------ */
void FUN_02030058(...);
u32 FUN_02030154(...);
void FUN_0203058c(...);
void FUN_02030628(...);
u32 FUN_02030670(...);
void FUN_020308b4(...);
u32 FUN_02030980(...);
int FUN_020309a8(...);
int FUN_02030a0c(...);
int FUN_02030a70(...);
int FUN_02030ad4(...);
int FUN_02030c00(...);
u32 FUN_02030cc8(...);
u32 FUN_02030cd8(...);
u32 FUN_02030cf0(...);
void FUN_02030f0c(...);
void FUN_02030f94(...);
u32 FUN_02030fb8(...);
BOOL FUN_02031008(...);
void FUN_02031048(...);
u32 FUN_02031150(...);
u32 FUN_02031390(...);
u32 FUN_02031460(...);
void FUN_02031554(...);
void FUN_020315a8(...);
void FUN_020315cc(...);
void FUN_02031608(...);
void FUN_02031644(...);
void FUN_02031678(...);
int FUN_020316e4(...);
int FUN_02031818(...);
int FUN_02031894(...);
int FUN_02031910(...);
int FUN_0203198c(...);
int FUN_02031a08(...);
u32 FUN_02031abc(...);
u32 FUN_02031b64(...);
u32 FUN_02031cc4(...);
u32 FUN_02031d34(...);
int FUN_02031de0(...);
BOOL FUN_02031f88(...);
u32 FUN_02031fac(...);
u32 FUN_0203207c(...);
u32 FUN_02032274(...);
void FUN_0203231c(...);
void FUN_0203238c(...);
void FUN_02032c40(...);
void FUN_02032d8c(...);
void FUN_02032ee0(...);
void FUN_02032f8c(...);
int FUN_02032fc4(...);
int FUN_02033010(...);
void FUN_02033064(...);
void FUN_020330d8(...);
void FUN_02033150(...);
u32 FUN_02033198(...);
void FUN_020331d8(...);
void FUN_02033248(...);

/* ================================================================== */
/*  Function implementations                                          */
/* ================================================================== */

// FUN_02030058 @ 0x02030058 (244 bytes)
void FUN_02030058(param_1, param_2, param_3)
    u32 param_1;
    u32 param_2;
    int param_3;
{
  int *piVar1;
  u32 uVar2;
  int iVar3;
  BOOL bVar4;
  
  piVar1 = DAT_0203014c;
  if (*DAT_0203014c != 0) {
    bVar4 = -1 < DAT_0203014c[0xb];
    if (bVar4) {
      param_3 = 1 << (DAT_0203014c[0xb] & 0xffU);
    }
    if (!bVar4) {
      param_3 = 0;
    }
    FUN_0203c0ec(DAT_0203014c[9],DAT_0203014c[10],param_3,0);
    if (bVar4) {
      uVar2 = FUN_0203c5ac();
      FUN_0203c67c(1);
      FUN_0203c5ec(uVar2);
      uVar2 = DAT_02030150;
      do {
        iVar3 = FUN_02039bf0(uVar2,0,0);
      } while (iVar3 != 0);
    }
    if (piVar1[10] != 0) {
      FUN_0202ec0c();
    }
    if (piVar1[8] != 0) {
      FUN_0202ec4c();
    }
    if (bVar4) {
      FUN_0202eba8(piVar1[0xb]);
    }
    if (piVar1[1] == 1) {
      FUN_0203be94(0,0,0,0);
    }
    *piVar1 = 0;
    return;
  }
  return;
}

// FUN_02030154 @ 0x02030154 (860 bytes)
u32 FUN_02030154(param_1, param_2, param_3, param_4, param_5)
    int param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int param_5;
{
  u32 *puVar1;
  int iVar2;
  u32 uVar3;
  int iVar4;
  u32 uVar5;
  int unaff_r5;
  int iVar6;
  int iVar7;
  BOOL bVar8;
  u32 in_stack_00000010;
  int in_stack_00000014;
  u32 in_stack_00000020;
  int in_stack_00000024;
  u32 in_stack_00000028;
  u32 local_34;
  int local_30;
  
  puVar1 = DAT_020304b0;
  local_34 = 0;
  iVar7 = -1;
  FUN_0203a04c(param_2,param_4);
  FUN_0203a04c(param_3,param_4);
  bVar8 = param_5 == 1;
  iVar2 = FUN_020464b0(DAT_020304b4,in_stack_00000010);
  if (in_stack_00000024 != 0) {
    iVar2 = iVar2 + 0x10 >> 5;
    uVar3 = param_4;
    if (!bVar8) {
      uVar3 = param_4 >> 1;
    }
    iVar4 = FUN_020466bc(uVar3,in_stack_00000020);
    iVar6 = 0x20;
    local_30 = iVar2 * iVar4;
    if (!bVar8) {
      iVar6 = 0x10;
    }
    unaff_r5 = iVar6 * iVar2;
  }
  if (param_1 != 2) {
    local_34 = 10;
  }
  if ((in_stack_00000024 != 0) && (iVar7 = FUN_0202ebc8(), iVar7 < 0)) {
    return 0;
  }
  iVar2 = FUN_0202ec28(3);
  if (iVar2 == 0) {
    if (-1 < iVar7) {
      FUN_0202eba8(iVar7);
    }
    return 0;
  }
  iVar2 = FUN_0202ec84(10);
  if (iVar2 != 0) {
    FUN_0203bf4c(1,!bVar8,param_2);
    FUN_0203c0a0(0,bVar8,param_2,param_4 >> 2);
    FUN_0203bf4c(3,!bVar8,param_3);
    FUN_0203c0a0(1,bVar8,param_3,param_4 >> 2);
    if (-1 < iVar7) {
      FUN_0203c05c(iVar7,local_30 + unaff_r5,local_30,DAT_020304b8);
    }
    if (param_1 == 1) {
      FUN_0203be94(1,2,1,1);
    }
    FUN_0203c15c(local_34,3);
    *puVar1 = 1;
    puVar1[1] = param_1;
    puVar1[8] = 10;
    puVar1[9] = local_34;
    puVar1[10] = 3;
    puVar1[0xb] = iVar7;
    puVar1[2] = param_5;
    puVar1[3] = param_2;
    puVar1[4] = param_3;
    puVar1[5] = param_4;
    uVar5 = FUN_020466bc(param_4,in_stack_00000020);
    puVar1[6] = uVar5;
    puVar1[7] = 0;
    puVar1[0xc] = in_stack_00000020;
    puVar1[0xd] = in_stack_00000024;
    puVar1[0xe] = in_stack_00000028;
    puVar1[0x14] = in_stack_00000014;
    FUN_020341ec(puVar1 + 0xf);
    FUN_020341bc(puVar1 + 0xf,in_stack_00000014 << 8,1);
    puVar1[0x13] = 0;
    return 1;
  }
  if (-1 < iVar7) {
    FUN_0202eba8(iVar7);
  }
  FUN_0202ec0c(3);
  return 0;
}

// FUN_0203058c @ 0x0203058c (128 bytes)
void FUN_0203058c(...)
{
  u32 uVar1;
  u32 uVar2;
  
  uVar2 = DAT_02030618;
  uVar1 = DAT_02030614;
  if (*DAT_0203060c == 0) {
    *DAT_02030610 = 0;
    FUN_02039d44(uVar1,uVar2,8);
    FUN_0203960c(DAT_0203061c,DAT_02030620,0,DAT_02030624);
    uVar1 = DAT_0203061c;
    *DAT_0203060c = 1;
    FUN_020394a0(uVar1);
    return;
  }
  return;
}

// FUN_02030628 @ 0x02030628 (68 bytes)
void FUN_02030628(...)
{
  if (*DAT_0203066c == 0) {
    return;
  }
  if (DAT_0203066c[1] == 1) {
    FUN_02030058();
    return;
  }
  return;
}

// FUN_02030670 @ 0x02030670 (188 bytes)
u32 FUN_02030670(param_1, param_2)
    int param_1;
    u32 param_2;
{
  u32 uVar1;
  
  FUN_02030628();
  if (*DAT_0203072c == 0) {
    FUN_0203b7dc(0,param_1,param_2);
    FUN_0203a04c(param_1,param_2);
    uVar1 = FUN_02030154(1,param_1,param_1 + (param_2 >> 1),param_2 >> 1);
    return uVar1;
  }
  return 0;
}

// FUN_020308b4 @ 0x020308b4 (28 bytes)
void FUN_020308b4(param_1)
    u32 * param_1;
{
  u32 uVar1;
  
  uVar1 = *(u32 *)(*DAT_020308d0 + 0x7c);
  *param_1 = *(u32 *)(*DAT_020308d0 + 0x78);
  param_1[1] = uVar1;
  return;
}

// FUN_02030980 @ 0x02030980 (36 bytes)
u32 FUN_02030980(param_1)
    u32 param_1;
{
  u32 uVar1;
  
  if (param_1 < *(u32 *)(*(int *)(*DAT_020309a4 + 0x80) + 8)) {
    uVar1 = *(u32 *)(*(int *)(*DAT_020309a4 + 0x80) + param_1 * 0x10 + 0xc);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

// FUN_020309a8 @ 0x020309a8 (96 bytes)
int FUN_020309a8(param_1)
    u32 param_1;
{
  int iVar1;
  int iVar2;
  u32 *puVar3;
  
  iVar2 = *(int *)(*DAT_02030a08 + 0x88);
  iVar1 = *(int *)(iVar2 + 0x1c);
  if (iVar1 == 0) {
    puVar3 = (u32 *)0x0;
  }
  else {
    puVar3 = (u32 *)(iVar2 + iVar1);
  }
  if (puVar3 == (u32 *)0x0) {
    return 0;
  }
  if ((int)param_1 < 0) {
    return 0;
  }
  if (param_1 < *puVar3) {
    if (puVar3[param_1 + 1] == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(*DAT_02030a08 + 0x88) + puVar3[param_1 + 1];
    }
    return iVar1;
  }
  return 0;
}

// FUN_02030a0c @ 0x02030a0c (96 bytes)
int FUN_02030a0c(param_1)
    u32 param_1;
{
  int iVar1;
  int iVar2;
  u32 *puVar3;
  
  iVar2 = *(int *)(*DAT_02030a6c + 0x88);
  iVar1 = *(int *)(iVar2 + 0x20);
  if (iVar1 == 0) {
    puVar3 = (u32 *)0x0;
  }
  else {
    puVar3 = (u32 *)(iVar2 + iVar1);
  }
  if (puVar3 == (u32 *)0x0) {
    return 0;
  }
  if ((int)param_1 < 0) {
    return 0;
  }
  if (param_1 < *puVar3) {
    if (puVar3[param_1 + 1] == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(*DAT_02030a6c + 0x88) + puVar3[param_1 + 1];
    }
    return iVar1;
  }
  return 0;
}

// FUN_02030a70 @ 0x02030a70 (96 bytes)
int FUN_02030a70(param_1)
    u32 param_1;
{
  int iVar1;
  int iVar2;
  u32 *puVar3;
  
  iVar2 = *(int *)(*DAT_02030ad0 + 0x88);
  iVar1 = *(int *)(iVar2 + 0x18);
  if (iVar1 == 0) {
    puVar3 = (u32 *)0x0;
  }
  else {
    puVar3 = (u32 *)(iVar2 + iVar1);
  }
  if (puVar3 == (u32 *)0x0) {
    return 0;
  }
  if ((int)param_1 < 0) {
    return 0;
  }
  if (param_1 < *puVar3) {
    if (puVar3[param_1 + 1] == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(*DAT_02030ad0 + 0x88) + puVar3[param_1 + 1];
    }
    return iVar1;
  }
  return 0;
}

// FUN_02030ad4 @ 0x02030ad4 (96 bytes)
int FUN_02030ad4(param_1)
    u32 param_1;
{
  int iVar1;
  int iVar2;
  u32 *puVar3;
  
  iVar2 = *(int *)(*DAT_02030b34 + 0x88);
  iVar1 = *(int *)(iVar2 + 0x24);
  if (iVar1 == 0) {
    puVar3 = (u32 *)0x0;
  }
  else {
    puVar3 = (u32 *)(iVar2 + iVar1);
  }
  if (puVar3 == (u32 *)0x0) {
    return 0;
  }
  if ((int)param_1 < 0) {
    return 0;
  }
  if (param_1 < *puVar3) {
    if (puVar3[param_1 + 1] == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(*DAT_02030b34 + 0x88) + puVar3[param_1 + 1];
    }
    return iVar1;
  }
  return 0;
}

// FUN_02030c00 @ 0x02030c00 (96 bytes)
int FUN_02030c00(param_1)
    u32 param_1;
{
  int iVar1;
  int iVar2;
  u32 *puVar3;
  
  iVar2 = *(int *)(*DAT_02030c60 + 0x88);
  iVar1 = *(int *)(iVar2 + 0xc);
  if (iVar1 == 0) {
    puVar3 = (u32 *)0x0;
  }
  else {
    puVar3 = (u32 *)(iVar2 + iVar1);
  }
  if (puVar3 == (u32 *)0x0) {
    return 0;
  }
  if ((int)param_1 < 0) {
    return 0;
  }
  if (param_1 < *puVar3) {
    if (puVar3[param_1 + 1] == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(*DAT_02030c60 + 0x88) + puVar3[param_1 + 1];
    }
    return iVar1;
  }
  return 0;
}

// FUN_02030cc8 @ 0x02030cc8 (12 bytes)
u32 FUN_02030cc8(...)
{
  return *DAT_02030cd4;
}

// FUN_02030cd8 @ 0x02030cd8 (20 bytes)
u32 FUN_02030cd8(param_1)
    u32 param_1;
{
  u32 uVar1;
  
  uVar1 = *DAT_02030cec;
  *DAT_02030cec = param_1;
  return uVar1;
}

// FUN_02030cf0 @ 0x02030cf0 (528 bytes)
u32 FUN_02030cf0(param_1, param_2, param_3)
    int param_1;
    int param_2;
    int param_3;
{
  int iVar1;
  u32 uVar2;
  u32 (*UNRECOVERED_JUMPTABLE_00)();
  
  iVar1 = FUN_0203ed60(param_1 + 0x34,0,0);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = FUN_0203edcc(param_1 + 0x34,param_1,0x30);
  if (iVar1 != 0x30) {
                    /* WARNING: Could not recover jumptable at 0x02030d48. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
    return uVar2;
  }
  if (param_2 != 0) {
    uVar2 = FUN_020311a8(param_2,*(u32 *)(param_1 + 0x1c),DAT_02030f00,param_1);
    *(u32 *)(param_1 + 0x88) = uVar2;
    if (*(int *)(param_1 + 0x88) == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030d88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    iVar1 = FUN_0203ed60(param_1 + 0x34,*(u32 *)(param_1 + 0x18),0);
    if (iVar1 == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030dac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    iVar1 = FUN_0203edcc(param_1 + 0x34,*(u32 *)(param_1 + 0x88),
                         *(u32 *)(param_1 + 0x1c));
    if (iVar1 != *(int *)(param_1 + 0x1c)) {
                    /* WARNING: Could not recover jumptable at 0x02030dd4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    uVar2 = FUN_020311a8(param_2,*(u32 *)(param_1 + 0x24),DAT_02030f04,param_1);
    *(u32 *)(param_1 + 0x80) = uVar2;
    if (*(int *)(param_1 + 0x80) == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030e0c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    iVar1 = FUN_0203ed60(param_1 + 0x34,*(u32 *)(param_1 + 0x20),0);
    if (iVar1 == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030e30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    iVar1 = FUN_0203edcc(param_1 + 0x34,*(u32 *)(param_1 + 0x80),
                         *(u32 *)(param_1 + 0x24));
    if (iVar1 != *(int *)(param_1 + 0x24)) {
                    /* WARNING: Could not recover jumptable at 0x02030e58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
      return uVar2;
    }
    if ((param_3 != 0) && (*(int *)(param_1 + 0x14) != 0)) {
      uVar2 = FUN_020311a8(param_2,*(int *)(param_1 + 0x14),DAT_02030f08,param_1);
      *(u32 *)(param_1 + 0x84) = uVar2;
      if (*(int *)(param_1 + 0x84) == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030ea0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
        return uVar2;
      }
      iVar1 = FUN_0203ed60(param_1 + 0x34,*(u32 *)(param_1 + 0x10),0);
      if (iVar1 == 0) {
                    /* WARNING: Could not recover jumptable at 0x02030ec4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
        return uVar2;
      }
      iVar1 = FUN_0203edcc(param_1 + 0x34,*(u32 *)(param_1 + 0x84),
                           *(u32 *)(param_1 + 0x14));
      if (iVar1 != *(int *)(param_1 + 0x14)) {
                    /* WARNING: Could not recover jumptable at 0x02030eec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar2 = (*UNRECOVERED_JUMPTABLE_00)(0);
        return uVar2;
      }
    }
  }
  return 1;
}

// FUN_02030f0c @ 0x02030f0c (132 bytes)
void FUN_02030f0c(param_1, param_2, param_3, param_4)
    int param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
{
  int iVar1;
  u32 (*UNRECOVERED_JUMPTABLE)();
  
  *(u32 *)(param_1 + 0x88) = 0;
  *(u32 *)(param_1 + 0x80) = 0;
  *(u32 *)(param_1 + 0x84) = 0;
  iVar1 = FUN_0203f074(param_1 + 0x78);
  if (iVar1 == 0) {
    return;
  }
  FUN_0203f2a0(param_1 + 0x34);
  iVar1 = FUN_0203efa0(param_1 + 0x34,*(u32 *)(param_1 + 0x78),
                       *(u32 *)(param_1 + 0x7c));
  if (iVar1 != 0) {
    *(u32 *)(param_1 + 0x30) = 1;
    iVar1 = FUN_02030cf0(param_1,param_3,param_4);
    if (iVar1 != 0) {
      *DAT_02030f90 = param_1;
    }
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x02030f60. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}

// FUN_02030f94 @ 0x02030f94 (36 bytes)
void FUN_02030f94(...)
{
  u32 uVar1;
  
  uVar1 = FUN_0203c5ac();
  FUN_0203c67c(1);
  FUN_0203c5ec(uVar1);
  return;
}

// FUN_02030fb8 @ 0x02030fb8 (80 bytes)
u32 FUN_02030fb8(param_1)
    u32 * param_1;
{
  int iVar1;
  
  iVar1 = FUN_0202e824(*param_1,0x14,4);
  if (iVar1 != 0) {
    FUN_02031048(iVar1);
    FUN_0202e53c(param_1 + 1,iVar1);
    return 1;
  }
  return 0;
}

// FUN_02031008 @ 0x02031008 (64 bytes)
BOOL FUN_02031008(param_1, param_2)
    u32 * param_1;
    u32 param_2;
{
  int iVar1;
  
  FUN_0202e5d0(param_1 + 1,0xc);
  *param_1 = param_2;
  iVar1 = FUN_02030fb8(param_1);
  return iVar1 != 0;
}

// FUN_02031048 @ 0x02031048 (12 bytes)
void FUN_02031048(param_1)
    u32 param_1;
{
                    /* WARNING: Could not recover jumptable at 0x02031050. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_02031054)(param_1,0);
  return;
}

// FUN_02031150 @ 0x02031150 (88 bytes)
u32 FUN_02031150(param_1)
    u32 * param_1;
{
  int iVar1;
  u32 uVar2;
  u32 (*UNRECOVERED_JUMPTABLE)();
  
  iVar1 = FUN_0202e78c(*param_1,*(u16 *)(param_1 + 3));
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  iVar1 = FUN_02030fb8(param_1);
  if (iVar1 == 0) {
    FUN_0202e730(*param_1,0);
    return 0xffffffff;
  }
                    /* WARNING: Could not recover jumptable at 0x0203118c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar2 = (*UNRECOVERED_JUMPTABLE)(*(u16 *)(param_1 + 3) - 1);
  return uVar2;
}

// FUN_02031390 @ 0x02031390 (208 bytes)
u32 FUN_02031390(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    int param_2;
    u32 param_3;
    u32 param_4;
    u32 param_5;
{
  int iVar1;
  u32 uVar2;
  u32 (*UNRECOVERED_JUMPTABLE)();
  u32 local_40;
  u32 local_3c;
  u32 local_38;
  u32 local_34;
  char acStack_30 [2];
  u16 uStack_2e;
  u16 uStack_2c;
  
  FUN_0203d008(&local_38);
  local_40 = local_38;
  local_3c = local_34;
  if (param_2 == 0) {
    return 0;
  }
  iVar1 = FUN_0203ce30(param_2,acStack_30,&local_40);
  if (iVar1 != 0) {
    do {
      if (((acStack_30[0] == '\x01') && (param_3 == uStack_2c)) &&
         (iVar1 = FUN_02031460(param_1,uStack_2e,param_4,param_5), iVar1 == 0)) {
                    /* WARNING: Could not recover jumptable at 0x02031434. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar2 = (*UNRECOVERED_JUMPTABLE)(0);
        return uVar2;
      }
      iVar1 = FUN_0203ce30(param_2,acStack_30,&local_40);
    } while (iVar1 != 0);
  }
  return 1;
}

// FUN_02031460 @ 0x02031460 (240 bytes)
u32 FUN_02031460(param_1, param_2, param_3, param_4)
    int param_1;
    u32 param_2;
    u32 param_3;
    int param_4;
{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  int iVar4;
  int iVar5;
  u32 (*UNRECOVERED_JUMPTABLE)();
  BOOL bVar6;
  
  iVar1 = FUN_0203cda8();
  if (iVar1 != 0) {
    return 1;
  }
  iVar1 = FUN_0203ce28(param_1);
  uVar2 = iVar1 - 1;
  iVar1 = param_1 + (*(int *)(param_1 + 0x38) + param_2) * 4;
  bVar6 = param_2 < uVar2;
  if (bVar6) {
    uVar2 = *(u32 *)(iVar1 + 0x40);
  }
  iVar1 = *(int *)(iVar1 + 0x3c);
  if (!bVar6) {
    uVar2 = *(u32 *)(param_1 + 8);
  }
  iVar5 = uVar2 - iVar1;
  if (param_4 == 0) {
                    /* WARNING: Could not recover jumptable at 0x020314cc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (*UNRECOVERED_JUMPTABLE)(0);
    return uVar3;
  }
  iVar4 = FUN_020311a8(param_4,iVar5 + 0x20,DAT_02031550,param_1);
  if (iVar4 == 0) {
                    /* WARNING: Could not recover jumptable at 0x020314f8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (*UNRECOVERED_JUMPTABLE)(0);
    return uVar3;
  }
  iVar1 = FUN_020308d4(param_3,iVar4,iVar5,iVar1);
  if (iVar5 == iVar1) {
    FUN_0203a030(iVar4,iVar5);
    FUN_0203cdf0(param_1,param_2,iVar4);
    return 1;
  }
                    /* WARNING: Could not recover jumptable at 0x02031520. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*UNRECOVERED_JUMPTABLE)(0);
  return uVar3;
}

// FUN_02031554 @ 0x02031554 (84 bytes)
void FUN_02031554(param_1, param_2, param_3, param_4)
    int param_1;
    int param_2;
    u32 param_3;
    u32 param_4;
{
  int iVar1;
  
  iVar1 = FUN_0203cda8(param_3,param_4);
  if (param_1 == iVar1) {
    FUN_0203cdf0(param_3,param_4,0);
  }
  FUN_0203bec8(param_1,param_1 + param_2);
  return;
}

// FUN_020315a8 @ 0x020315a8 (36 bytes)
void FUN_020315a8(param_1, param_2, param_3, param_4)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
{
  FUN_02031678(param_1,param_3,param_4);
  FUN_0203d220(param_1);
  return;
}

// FUN_020315cc @ 0x020315cc (60 bytes)
void FUN_020315cc(param_1, param_2, param_3, param_4)
    int param_1;
    int param_2;
    u32 param_3;
    u32 param_4;
{
  FUN_02031678(param_1,param_3,param_4);
  FUN_0203bec8(param_1,param_1 + param_2);
  FUN_0203d220(param_1);
  return;
}

// FUN_02031608 @ 0x02031608 (60 bytes)
void FUN_02031608(param_1, param_2, param_3, param_4)
    int param_1;
    int param_2;
    u32 param_3;
    u32 param_4;
{
  FUN_02031678(param_1,param_3,param_4);
  FUN_0203bef4(param_1,param_1 + param_2);
  FUN_0203d26c(param_1);
  return;
}

// FUN_02031644 @ 0x02031644 (52 bytes)
void FUN_02031644(param_1, param_2, param_3, param_4)
    int param_1;
    int param_2;
    u32 param_3;
    u32 param_4;
{
  FUN_02031678(param_1,param_3,param_4);
  FUN_0203bf20(param_1,param_1 + param_2);
  return;
}

// FUN_02031678 @ 0x02031678 (108 bytes)
void FUN_02031678(param_1, param_2, param_3)
    int param_1;
    int param_2;
    u32 param_3;
{
  u32 uVar1;
  u32 uVar2;
  int iVar3;
  
  if (param_2 != 0) {
    uVar1 = FUN_0203ae38();
    uVar2 = FUN_02030cd8(param_2);
    iVar3 = FUN_0203088c(param_3);
    if (param_1 == iVar3) {
      FUN_02030870(param_3,0);
    }
    FUN_02030cd8(uVar2);
    FUN_0203ae4c(uVar1);
    return;
  }
  return;
}

// FUN_020316e4 @ 0x020316e4 (300 bytes)
int FUN_020316e4(param_1, param_2, param_3)
    u32 param_1;
    int param_2;
    int param_3;
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  u32 *puVar5;
  u8 auStack_20 [4];
  
  puVar5 = (u32 *)auStack_20;
  iVar3 = FUN_0203088c();
  if (iVar3 == 0) {
    iVar3 = FUN_020308d4(param_1,DAT_02031810,0x3c,0);
    if (iVar3 != 0x3c) {
      return 0;
    }
    if (param_2 == 0) {
      puVar5 = (u32 *)((void*)0);
    }
    iVar1 = *(int *)(DAT_02031810 + 0x38) * 4;
    iVar2 = *(int *)(DAT_02031810 + 0x38) * 8;
    if (param_2 == 0) {
      return 0;
    }
    if (param_3 != 0) {
      FUN_02030cc8(iVar2);
    }
    *puVar5 = param_1;
    iVar3 = FUN_020311a8(param_2,iVar2 + 0x5c,DAT_02031814);
    if (iVar3 == 0) {
                    /* WARNING: Could not recover jumptable at 0x0203178c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      iVar3 = (*(u32 (*)())puVar5[7])(0);
      return iVar3;
    }
    iVar4 = FUN_020308d4(param_1,iVar3,iVar1 + 0x3c,0);
    if (iVar4 != iVar1 + 0x3c) {
                    /* WARNING: Could not recover jumptable at 0x020317b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      iVar3 = (*(u32 (*)())puVar5[7])(0);
      return iVar3;
    }
    FUN_0203b9a8(iVar3 + 0x3c,iVar3 + 0x3c + *(int *)(iVar3 + 0x38) * 4,iVar1);
    FUN_0203b914(iVar3 + 0x3c,0,iVar1);
    FUN_0203a030(iVar3,iVar2 + 0x3c);
    if (param_3 != 0) {
      FUN_02030870(param_1,iVar3);
    }
  }
  return iVar3;
}

// FUN_02031818 @ 0x02031818 (120 bytes)
int FUN_02031818(param_1, param_2, param_3)
    u32 param_1;
    u32 param_2;
    int param_3;
{
  int iVar1;
  
  iVar1 = FUN_0203088c();
  if (iVar1 == 0) {
    if (param_3 != 0) {
      FUN_02030cc8();
    }
    iVar1 = FUN_02031a08(param_1,DAT_02031890);
    if ((param_3 != 0) && (iVar1 != 0)) {
      FUN_02030870(param_1,iVar1);
    }
  }
  return iVar1;
}

// FUN_02031894 @ 0x02031894 (120 bytes)
int FUN_02031894(param_1, param_2, param_3)
    u32 param_1;
    u32 param_2;
    int param_3;
{
  int iVar1;
  
  iVar1 = FUN_0203088c();
  if (iVar1 == 0) {
    if (param_3 != 0) {
      FUN_02030cc8();
    }
    iVar1 = FUN_02031a08(param_1,DAT_0203190c);
    if ((param_3 != 0) && (iVar1 != 0)) {
      FUN_02030870(param_1,iVar1);
    }
  }
  return iVar1;
}

// FUN_02031910 @ 0x02031910 (120 bytes)
int FUN_02031910(param_1, param_2, param_3)
    u32 param_1;
    u32 param_2;
    int param_3;
{
  int iVar1;
  
  iVar1 = FUN_0203088c();
  if (iVar1 == 0) {
    if (param_3 != 0) {
      FUN_02030cc8();
    }
    iVar1 = FUN_02031a08(param_1,DAT_02031988);
    if ((param_3 != 0) && (iVar1 != 0)) {
      FUN_02030870(param_1,iVar1);
    }
  }
  return iVar1;
}

// FUN_0203198c @ 0x0203198c (120 bytes)
int FUN_0203198c(param_1, param_2, param_3)
    u32 param_1;
    u32 param_2;
    int param_3;
{
  int iVar1;
  
  iVar1 = FUN_0203088c();
  if (iVar1 == 0) {
    if (param_3 != 0) {
      FUN_02030cc8();
    }
    iVar1 = FUN_02031a08(param_1,DAT_02031a04);
    if ((param_3 != 0) && (iVar1 != 0)) {
      FUN_02030870(param_1,iVar1);
    }
  }
  return iVar1;
}

// FUN_02031a08 @ 0x02031a08 (180 bytes)
int FUN_02031a08(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int param_5;
{
  int iVar1;
  int iVar2;
  int iVar3;
  u32 (*UNRECOVERED_JUMPTABLE_00)();
  
  iVar1 = FUN_02030958();
  if (iVar1 == 0) {
    return 0;
  }
  if (param_5 == 0) {
    return 0;
  }
  iVar2 = FUN_020311a8(param_5,iVar1 + 0x20,param_2,param_3);
  if (iVar2 == 0) {
                    /* WARNING: Could not recover jumptable at 0x02031a74. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = (*UNRECOVERED_JUMPTABLE_00)(0);
    return iVar1;
  }
  iVar3 = FUN_020308d4(param_1,iVar2,iVar1,0);
  if (iVar1 == iVar3) {
    FUN_0203a030(iVar2,iVar1);
    return iVar2;
  }
                    /* WARNING: Could not recover jumptable at 0x02031a9c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = (*UNRECOVERED_JUMPTABLE_00)(0);
  return iVar1;
}

// FUN_02031abc @ 0x02031abc (168 bytes)
u32 FUN_02031abc(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int * param_5;
{
  u32 *puVar1;
  int iVar2;
  
  puVar1 = (u32 *)FUN_02030b38();
  if (puVar1 == (u32 *)0x0) {
    return 5;
  }
  if ((param_2 & 4) == 0) {
    iVar2 = FUN_0203088c(*puVar1 & 0xffffff);
  }
  else {
    if ((*puVar1 & 0x1000000) == 0) {
      iVar2 = FUN_02031818(*puVar1 & 0xffffff,param_3,param_4);
    }
    else {
      iVar2 = FUN_020316e4(*puVar1 & 0xffffff,param_3,param_4);
    }
    if (iVar2 == 0) {
      return 9;
    }
  }
  if (param_5 != (int *)0x0) {
    *param_5 = iVar2;
  }
  return 0;
}

// FUN_02031b64 @ 0x02031b64 (348 bytes)
u32 FUN_02031b64(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int * param_5;
{
  u32 *puVar1;
  int iVar2;
  u32 *puVar3;
  u32 uVar4;
  int iVar5;
  int iVar6;
  u32 (*UNRECOVERED_JUMPTABLE)();
  int iStack_2c;
  
  puVar1 = (u32 *)FUN_02030b9c();
  if (puVar1 == (u32 *)0x0) {
    return 4;
  }
  if ((param_2 & 2) == 0) {
    iVar2 = FUN_0203088c(*puVar1);
  }
  else {
    iVar2 = FUN_02031894(*puVar1,param_3,param_4);
    if (iVar2 == 0) {
      return 8;
    }
  }
  iVar6 = 0;
  do {
    if (*(u16 *)((int)puVar1 + iVar6 * 2 + 4) != DAT_02031cc0) {
      puVar3 = (u32 *)FUN_02030b38();
      if (puVar3 == (u32 *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x02031bfc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar4 = (*UNRECOVERED_JUMPTABLE)(5);
        return uVar4;
      }
      iVar5 = FUN_02031abc(*(u16 *)((int)puVar1 + iVar6 * 2 + 4),param_2,param_3,param_4);
      if (iVar5 != 0) {
                    /* WARNING: Could not recover jumptable at 0x02031c2c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar4 = (*UNRECOVERED_JUMPTABLE)();
        return uVar4;
      }
      if (((*puVar3 & 0x1000000) != 0) && ((param_2 & 4) != 0)) {
        iVar5 = FUN_02031390(iStack_2c,iVar2,iVar6,*puVar3 & 0xffffff);
        if (iVar5 == 0) {
                    /* WARNING: Could not recover jumptable at 0x02031c74. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          uVar4 = (*UNRECOVERED_JUMPTABLE)(9);
          return uVar4;
        }
      }
      if ((iVar2 != 0) && (iStack_2c != 0)) {
        FUN_0203d30c(iVar2,iVar6);
      }
    }
    iVar6 = iVar6 + 1;
    if (3 < iVar6) {
      if (param_5 != (int *)0x0) {
        *param_5 = iVar2;
      }
      return 0;
    }
  } while( true );
}

// FUN_02031cc4 @ 0x02031cc4 (112 bytes)
u32 FUN_02031cc4(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int * param_5;
{
  u32 *puVar1;
  int iVar2;
  
  puVar1 = (u32 *)FUN_02030c00();
  if (puVar1 == (u32 *)0x0) {
    return 3;
  }
  if ((param_2 & 8) == 0) {
    iVar2 = FUN_0203088c(*puVar1);
  }
  else {
    iVar2 = FUN_02031910(*puVar1,param_3,param_4);
    if (iVar2 == 0) {
      return 7;
    }
  }
  if (param_5 != (int *)0x0) {
    *param_5 = iVar2;
  }
  return 0;
}

// FUN_02031d34 @ 0x02031d34 (172 bytes)
u32 FUN_02031d34(param_1, param_2, param_3, param_4, param_5)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int * param_5;
{
  u32 *puVar1;
  int iVar2;
  u32 uVar3;
  u32 (*UNRECOVERED_JUMPTABLE)();
  
  puVar1 = (u32 *)FUN_02030c64();
  if (puVar1 == (u32 *)0x0) {
    return 2;
  }
  iVar2 = FUN_02031b64(*(u16 *)(puVar1 + 1),param_2,param_3,param_4);
  if (iVar2 == 0) {
    if ((param_2 & 1) == 0) {
      iVar2 = FUN_0203088c(*puVar1);
    }
    else {
      iVar2 = FUN_0203198c(*puVar1,param_3,param_4);
      if (iVar2 == 0) {
        return 6;
      }
    }
    if (param_5 != (int *)0x0) {
      *param_5 = iVar2;
    }
    return 0;
  }
                    /* WARNING: Could not recover jumptable at 0x02031d88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*UNRECOVERED_JUMPTABLE)();
  return uVar3;
}

// FUN_02031de0 @ 0x02031de0 (320 bytes)
int FUN_02031de0(param_1, param_2)
    u32 param_1;
    u32 param_2;
{
  u32 *puVar1;
  int iVar2;
  u32 *puVar3;
  u32 uVar4;
  u32 (*UNRECOVERED_JUMPTABLE)();
  
  puVar1 = (u32 *)FUN_020309a8();
  if (puVar1 == (u32 *)0x0) {
    return 1;
  }
  uVar4 = 0;
  if (*puVar1 != 0) {
    puVar3 = puVar1 + 1;
    do {
      switch((char)*puVar3) {
      case '\0':
        iVar2 = FUN_02031d34(puVar3[1],*(u8 *)((int)puVar3 + 1),param_2,1);
        if (iVar2 != 0) {
          return iVar2;
        }
        break;
      case '\x01':
        iVar2 = FUN_02031b64(puVar3[1],*(u8 *)((int)puVar3 + 1),param_2,1);
        if (iVar2 != 0) {
          return iVar2;
        }
        break;
      case '\x02':
        iVar2 = FUN_02031abc(puVar3[1],*(u8 *)((int)puVar3 + 1),param_2,1);
        if (iVar2 != 0) {
                    /* WARNING: Could not recover jumptable at 0x02031ef8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          iVar2 = (*UNRECOVERED_JUMPTABLE)();
          return iVar2;
        }
        break;
      case '\x03':
        iVar2 = FUN_02031cc4(puVar3[1],*(u8 *)((int)puVar3 + 1),param_2,1);
        if (iVar2 != 0) {
          return iVar2;
        }
      }
      uVar4 = uVar4 + 1;
      puVar3 = puVar3 + 2;
    } while (uVar4 < *puVar1);
  }
  return 0;
}

// FUN_02031f88 @ 0x02031f88 (36 bytes)
BOOL FUN_02031f88(...)
{
  int iVar1;
  
  iVar1 = FUN_02031de0();
  return iVar1 == 0;
}

// FUN_02031fac @ 0x02031fac (208 bytes)
u32 FUN_02031fac(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    u32 * param_5;
    int param_6;
    u32 param_7;
    u32 param_8;
{
  int iVar1;
  u32 uVar2;
  int iVar3;
  u32 uStack_1c;
  
  iVar1 = FUN_0202f10c(param_1,param_2,param_4);
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = FUN_0202efe8(param_2,iVar1);
  iVar3 = FUN_02031b64(param_3,6,uVar2,0);
  if (iVar3 == 0) {
    FUN_0202f0a4(iVar1,param_6 + *(int *)(param_6 + 0x18),*param_5,uStack_1c);
    FUN_0202f50c(param_1,*(u8 *)((int)param_5 + 6));
    FUN_0202f494(param_1,*(u8 *)((int)param_5 + 7));
    FUN_0202f410(param_1,param_7,param_8);
    return 1;
  }
  thunk_FUN_0202ed98(iVar1);
  return 0;
}

// FUN_0203207c @ 0x0203207c (268 bytes)
u32 FUN_0203207c(param_1, param_2, param_3, param_4, param_5, param_6)
    u32 param_1;
    u32 param_2;
    u32 param_3;
    u32 param_4;
    int param_5;
    u32 param_6;
{
  int iVar1;
  u32 uVar2;
  int iVar3;
  u32 uStack_24;
  u32 uStack_20;
  
  iVar1 = FUN_0202f10c(param_1,param_2,param_4);
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = FUN_0202efe8(param_2,iVar1);
  iVar3 = FUN_02031b64(param_3,6,uVar2,0);
  if (iVar3 == 0) {
    iVar3 = FUN_02031d34(param_6,1,uVar2,0);
    if (iVar3 == 0) {
      FUN_0202f0a4(iVar1,uStack_24 + *(int *)(uStack_24 + 0x18),0,uStack_20);
      FUN_0202f50c(param_1,*(u8 *)(param_5 + 6));
      FUN_0202f494(param_1,*(u8 *)(param_5 + 7));
      FUN_0202f438(param_1,param_6);
      return 1;
    }
    thunk_FUN_0202ed98(iVar1);
    return 0;
  }
  thunk_FUN_0202ed98(iVar1);
  return 0;
}

// FUN_02032274 @ 0x02032274 (168 bytes)
u32 FUN_02032274(param_1)
    int param_1;
{
  u8 *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  FUN_02030cc8();
  iVar3 = 0;
  do {
    pbVar1 = (u8 *)FUN_02030a70(iVar3);
    if (pbVar1 != (u8 *)0x0) {
      FUN_0202f754(iVar3,*pbVar1);
      FUN_0202f73c(iVar3,*(u16 *)(pbVar1 + 2));
      if (((*(int *)(pbVar1 + 4) != 0) && (param_1 != 0)) && (iVar4 = 0, *pbVar1 != 0)) {
        do {
          iVar2 = FUN_0202f69c(iVar3,param_1,*(u32 *)(pbVar1 + 4));
          if (iVar2 == 0) {
            return 0;
          }
          iVar4 = iVar4 + 1;
        } while (iVar4 < (int)(u32)*pbVar1);
      }
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x20);
  return 1;
}

// FUN_0203231c @ 0x0203231c (100 bytes)
void FUN_0203231c(param_1)
    int param_1;
{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 + DAT_02032380;
  iVar2 = param_1 + DAT_02032384;
  param_1 = param_1 + DAT_02032388;
  do {
    FUN_02039548(param_1);
    while( true ) {
      FUN_02039f08(iVar3);
      iVar1 = FUN_02033010(iVar2);
      if (iVar1 == 0) break;
      FUN_0203238c();
      FUN_02032f8c(iVar1);
      FUN_02039e98(iVar3);
    }
    FUN_02039e98(iVar3);
  } while( true );
}

// FUN_0203238c @ 0x0203238c (2208 bytes)
void FUN_0203238c(param_1)
    int param_1;
{
  char cVar1;
  u8 bVar2;
  u8 bVar3;
  short sVar4;
  int iVar5;
  u32 uVar6;
  u32 uVar7;
  u8 *pbVar8;
  u32 uVar9;
  u8 *pbVar10;
  u32 uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  short *psVar18;
  u32 uVar19;
  short *psVar20;
  short *psVar21;
  int iVar22;
  BOOL bVar23;
  int local_90;
  u32 local_8c;
  u32 local_88;
  u32 local_84;
  u32 local_7c;
  u32 local_78;
  u32 local_74;
  int local_68;
  int local_4c;
  int local_40;
  int local_34;
  int local_30;
  
  uVar6 = *(u32 *)(param_1 + 8);
  if ((*(int *)(uVar6 + 0x10c) << 0x1a < 0) && (0 < *(int *)(uVar6 + 0x110))) {
    *(int *)(uVar6 + 0x110) = *(int *)(uVar6 + 0x110) + -1;
  }
  iVar16 = DAT_02032c2c;
  local_8c = *(u32 *)(param_1 + 0x2c);
  local_90 = 0;
  if (local_8c != 0) {
    local_40 = 0;
    local_34 = 0;
    local_4c = 0;
    local_30 = 0;
    iVar5 = -0x8000;
    do {
      if (*(int *)(uVar6 + 0x10c) << 0x1a < 0) {
        iVar16 = 0;
        if (0 < *(int *)(param_1 + 0x10)) {
          do {
            FUN_0203b914(*(int *)(param_1 + iVar16 * 4 + 0x14) + local_90,0,local_8c);
            iVar16 = iVar16 + 1;
          } while (iVar16 < *(int *)(param_1 + 0x10));
        }
        break;
      }
      uVar19 = *(u32 *)(uVar6 + 0xd8);
      iVar17 = *(int *)(uVar6 + 0x158);
      uVar7 = FUN_020466bc(iVar17,uVar19);
      uVar11 = iVar17 - uVar7 * uVar19;
      bVar23 = uVar7 < *(int *)(uVar6 + 0xd0) - 1U;
      uVar9 = uVar6;
      if (bVar23) {
        local_88 = *(u32 *)(uVar6 + 0xd4);
        uVar9 = uVar19;
      }
      local_7c = local_8c;
      if (!bVar23) {
        uVar9 = *(u32 *)(uVar9 + 0xdc);
      }
      uVar19 = uVar9;
      if (!bVar23) {
        uVar19 = *(u32 *)(uVar6 + 0xe0);
        local_88 = uVar9;
      }
      if (*(char *)(uVar6 + 0xbc) != '\0') {
        local_7c = local_8c >> 1;
      }
      local_84 = uVar11;
      if ((int)(*(u32 *)(uVar6 + 0x10c) << 0x1b) < 0) {
        if (uVar11 == 0) {
          *(u32 *)(uVar6 + 0x10c) = *(u32 *)(uVar6 + 0x10c) & 0xffffffef;
        }
        else {
          local_84 = 0;
          local_7c = uVar11;
        }
      }
      bVar23 = false;
      if ((uVar19 <= local_84 + local_7c) &&
         (local_7c = uVar19 - local_84, *(int *)(uVar6 + 0xd0) - 1U <= uVar7)) {
        if (*(char *)(uVar6 + 0xbd) == '\0') {
          *(u32 *)(uVar6 + 0x10c) = *(u32 *)(uVar6 + 0x10c) | 0x20;
        }
        else {
          bVar23 = true;
        }
      }
      local_78 = local_7c;
      cVar1 = *(char *)(uVar6 + 0xbc);
      uVar9 = local_84;
      if (cVar1 == '\0') {
        local_74 = local_7c;
      }
      else if (cVar1 == '\x01') {
        local_78 = local_7c << 1;
        uVar9 = local_84 << 1;
        local_74 = local_78;
      }
      else if (cVar1 == '\x02') {
        local_74 = (local_84 + local_7c + 1 >> 1) - (local_84 >> 1);
        if (local_84 == 0) {
          local_74 = local_74 + 4;
          uVar9 = 0;
        }
        else {
          uVar9 = (local_84 >> 1) + 4;
        }
        local_78 = local_7c << 1;
      }
      iVar14 = *(int *)(uVar6 + 0xd4);
      bVar2 = *(u8 *)(uVar6 + 0xbe);
      iVar12 = *(int *)(uVar6 + 0xcc);
      iVar17 = *(int *)(uVar6 + 0xa0);
      local_68 = 0;
      if (0 < *(int *)(param_1 + 0x10)) {
        psVar18 = (short *)(uVar6 + 0xf4);
        do {
          psVar20 = (short *)(*(int *)(param_1 + local_68 * 4 + 0x14) + local_90);
          if (local_68 < (int)(u32)*(u8 *)(uVar6 + 0xbe)) {
            psVar21 = psVar20;
            if (*(char *)(uVar6 + 0xbc) == '\x02') {
              FUN_02039f08(DAT_02032c30);
              psVar21 = (short *)*DAT_02032c34;
            }
            FUN_0203ed60(uVar6 + 0x5c,
                         local_68 * local_88 +
                         (u32)bVar2 * uVar7 * iVar14 + uVar9 + iVar12 + iVar17,0);
            uVar11 = FUN_0203edcc(uVar6 + 0x5c,psVar21);
            if (uVar11 != local_74) {
              local_78 = 0;
              local_7c = 0;
              bVar23 = false;
              *(u32 *)(uVar6 + 0x10c) = *(u32 *)(uVar6 + 0x10c) | 0x20;
              if (*(char *)(uVar6 + 0xbc) == '\x02') {
                FUN_02039e98(DAT_02032c30);
              }
              break;
            }
            if (*(char *)(uVar6 + 0xbc) == '\x02') {
              pbVar8 = (u8 *)*DAT_02032c34;
              pbVar10 = pbVar8;
              if (local_84 == 0) {
                pbVar10 = pbVar8 + 4;
                sVar4 = *(short *)(pbVar8 + 2);
                *psVar18 = *(short *)pbVar8;
                psVar18[1] = sVar4;
              }
              uVar11 = local_84;
              psVar21 = psVar20;
              if ((local_84 & 1) != 0) {
                uVar11 = (int)(u32)*pbVar10 >> 4;
                iVar22 = (int)*(short *)(DAT_02032c38 + (u32)*(u8 *)(psVar18 + 1) * 2);
                iVar13 = iVar22 >> 3;
                if ((uVar11 & 4) != 0) {
                  iVar13 = iVar13 + iVar22;
                }
                if ((uVar11 & 2) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 1);
                }
                if ((uVar11 & 1) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 2);
                }
                if ((uVar11 & 8) == 0) {
                  iVar13 = *psVar18 + iVar13;
                  if (iVar16 < iVar13) {
                    iVar13 = iVar16;
                  }
                }
                else {
                  iVar13 = *psVar18 - iVar13;
                  if (iVar13 < -0x8000) {
                    iVar13 = iVar5;
                  }
                }
                iVar15 = (u32)*(u8 *)(psVar18 + 1) + (int)*(char *)(DAT_02032c3c + uVar11);
                iVar22 = local_4c;
                if ((-1 < iVar15) && (iVar22 = iVar15, 0x58 < iVar15)) {
                  iVar22 = 0x58;
                }
                *psVar18 = (short)iVar13;
                *(char *)(psVar18 + 1) = (char)iVar22;
                psVar21 = psVar20 + 1;
                *psVar20 = (short)iVar13;
                uVar11 = local_84 + 1;
                pbVar10 = pbVar10 + 1;
              }
              for (; uVar11 < (local_84 + local_7c & 0xfffffffe); uVar11 = uVar11 + 2) {
                bVar3 = *pbVar10;
                iVar22 = (int)*(short *)(DAT_02032c38 + (u32)*(u8 *)(psVar18 + 1) * 2);
                iVar13 = iVar22 >> 3;
                if ((bVar3 & 4) != 0) {
                  iVar13 = iVar13 + iVar22;
                }
                if ((bVar3 & 2) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 1);
                }
                if ((bVar3 & 1) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 2);
                }
                if ((bVar3 & 8) == 0) {
                  iVar13 = *psVar18 + iVar13;
                  if (iVar16 < iVar13) {
                    iVar13 = iVar16;
                  }
                }
                else {
                  iVar13 = *psVar18 - iVar13;
                  if (iVar13 < -0x8000) {
                    iVar13 = iVar5;
                  }
                }
                iVar15 = (u32)*(u8 *)(psVar18 + 1) + (int)*(char *)(DAT_02032c3c + (bVar3 & 0xf))
                ;
                iVar22 = local_40;
                if ((-1 < iVar15) && (iVar22 = iVar15, 0x58 < iVar15)) {
                  iVar22 = 0x58;
                }
                *psVar18 = (short)iVar13;
                *(char *)(psVar18 + 1) = (char)iVar22;
                *psVar21 = (short)iVar13;
                uVar19 = (int)(u32)*pbVar10 >> 4;
                iVar22 = (int)*(short *)(DAT_02032c38 + (u32)*(u8 *)(psVar18 + 1) * 2);
                iVar13 = iVar22 >> 3;
                if ((uVar19 & 4) != 0) {
                  iVar13 = iVar13 + iVar22;
                }
                if ((uVar19 & 2) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 1);
                }
                if ((uVar19 & 1) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 2);
                }
                if ((uVar19 & 8) == 0) {
                  iVar13 = *psVar18 + iVar13;
                  if (iVar16 < iVar13) {
                    iVar13 = iVar16;
                  }
                }
                else {
                  iVar13 = *psVar18 - iVar13;
                  if (iVar13 < -0x8000) {
                    iVar13 = iVar5;
                  }
                }
                iVar15 = (u32)*(u8 *)(psVar18 + 1) + (int)*(char *)(DAT_02032c3c + uVar19);
                iVar22 = local_34;
                if ((-1 < iVar15) && (iVar22 = iVar15, 0x58 < iVar15)) {
                  iVar22 = 0x58;
                }
                *psVar18 = (short)iVar13;
                *(char *)(psVar18 + 1) = (char)iVar22;
                psVar21[1] = (short)iVar13;
                psVar21 = psVar21 + 2;
                pbVar10 = pbVar10 + 1;
              }
              if (uVar11 < local_84 + local_7c) {
                bVar3 = *pbVar10;
                iVar22 = (int)*(short *)(DAT_02032c38 + (u32)*(u8 *)(psVar18 + 1) * 2);
                iVar13 = iVar22 >> 3;
                if ((bVar3 & 4) != 0) {
                  iVar13 = iVar13 + iVar22;
                }
                if ((bVar3 & 2) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 1);
                }
                if ((bVar3 & 1) != 0) {
                  iVar13 = iVar13 + (iVar22 >> 2);
                }
                if ((bVar3 & 8) == 0) {
                  iVar13 = *psVar18 + iVar13;
                  if (iVar16 < iVar13) {
                    iVar13 = iVar16;
                  }
                }
                else {
                  iVar13 = *psVar18 - iVar13;
                  if (iVar13 < -0x8000) {
                    iVar13 = iVar5;
                  }
                }
                iVar15 = (u32)*(u8 *)(psVar18 + 1) + (int)*(char *)(DAT_02032c3c + (bVar3 & 0xf))
                ;
                iVar22 = local_30;
                if ((-1 < iVar15) && (iVar22 = iVar15, 0x58 < iVar15)) {
                  iVar22 = 0x58;
                }
                *psVar18 = (short)iVar13;
                *(char *)(psVar18 + 1) = (char)iVar22;
                *psVar21 = (short)iVar13;
              }
              FUN_02039e98(DAT_02032c30);
            }
          }
          else if (*(int *)(uVar6 + 0x10c) << 0x19 < 0) {
            FUN_0203b914(psVar20,0,local_78);
          }
          else {
            FUN_0203b9a8(*(int *)(param_1 + 0x14) + local_90,psVar20,local_78);
          }
          psVar18 = psVar18 + 2;
          local_68 = local_68 + 1;
        } while (local_68 < *(int *)(param_1 + 0x10));
      }
      if ((int)(*(u32 *)(uVar6 + 0x10c) << 0x1b) < 0) {
        *(u32 *)(uVar6 + 0x10c) = *(u32 *)(uVar6 + 0x10c) & 0xffffffef;
      }
      else {
        if (bVar23) {
          *(u32 *)(uVar6 + 0x158) = *(u32 *)(uVar6 + 0xc4);
        }
        else {
          *(u32 *)(uVar6 + 0x158) = *(int *)(uVar6 + 0x158) + local_7c;
        }
        local_90 = local_90 + local_78;
        local_8c = local_8c - local_78;
        if ((*(int *)(uVar6 + 0x10c) << 0x1a < 0) && (*(int *)(uVar6 + 0x138) != 0)) {
          FUN_02032c40(uVar6);
        }
      }
    } while (local_8c != 0);
  }
  if (*(u32 (**)())(uVar6 + 0x130) != (u32 (*)())0x0) {
    (**(u32 (**)())(uVar6 + 0x130))
              (*(u32 *)(param_1 + 0xc),*(u32 *)(param_1 + 0x10),param_1 + 0x14,
               *(u32 *)(param_1 + 0x2c),*(char *)(uVar6 + 0xbc) != '\0',
               *(u32 *)(uVar6 + 0x134));
  }
  iVar16 = 0;
  if (0 < *(int *)(param_1 + 0x10)) {
    do {
      FUN_0203a04c(*(u32 *)(param_1 + iVar16 * 4 + 0x14),*(u32 *)(param_1 + 0x2c));
      iVar16 = iVar16 + 1;
    } while (iVar16 < *(int *)(param_1 + 0x10));
  }
  if (*(int *)(param_1 + 0xc) == 0) {
    *(u32 *)(uVar6 + 0x114) = 1;
  }
  return;
}

// FUN_02032c40 @ 0x02032c40 (328 bytes)
void FUN_02032c40(param_1)
    int param_1;
{
  char cVar1;
  short sVar2;
  u32 uVar3;
  int iVar4;
  u32 *puVar5;
  u32 uVar6;
  u32 (*UNRECOVERED_JUMPTABLE_00)();
  u32 local_28;
  u32 local_24;
  u32 local_20;
  int local_1c;
  
  local_28 = *(u32 *)(param_1 + 0x144);
  local_24 = *(u32 *)(param_1 + 0x140);
  local_20 = *(u32 *)(param_1 + 0x140);
  local_1c = 0;
  iVar4 = (**(u32 (**)())(param_1 + 0x138))(0,&local_28,&local_20,*(u32 *)(param_1 + 0x13c));
  if (iVar4 == 0) {
    return;
  }
  puVar5 = (u32 *)FUN_02030ad4(local_20);
  if (puVar5 == (u32 *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x02032ca4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE_00)();
    return;
  }
  cVar1 = *(char *)(param_1 + 0xbc);
  sVar2 = *(short *)(param_1 + 0xc0);
  uVar6 = FUN_02030980(*puVar5);
  *(u32 *)(param_1 + 0xa0) = uVar6;
  FUN_020308d4(*puVar5,param_1 + 0xa4,0x40,0);
  uVar3 = DAT_02032d88;
  if (sVar2 != *(short *)(param_1 + 0xc0)) {
                    /* WARNING: Could not recover jumptable at 0x02032ce0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE_00)();
    return;
  }
  if ((cVar1 == '\0') && (*(char *)(param_1 + 0xbc) != '\0')) {
    return;
  }
  if ((cVar1 != '\0') && (*(char *)(param_1 + 0xbc) == '\0')) {
    return;
  }
  *(u32 *)(param_1 + 0x140) = local_20;
  *(u32 *)(param_1 + 0x158) =
       (u32)((unsigned long long)uVar3 * (unsigned long long)((u32)*(u16 *)(param_1 + 0xc0) * local_1c) >> 0x26);
  if ((*(int *)(param_1 + 0x158) == 0) || (*(char *)(param_1 + 0xbc) != '\x02')) {
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) & 0xffffffef;
  }
  else {
    *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) | 0x10;
  }
  *(u32 *)(param_1 + 0x10c) = *(u32 *)(param_1 + 0x10c) & 0xffffffdf;
  return;
}

// FUN_02032d8c @ 0x02032d8c (320 bytes)
void FUN_02032d8c(param_1, param_2, param_3, param_4, param_5, param_6)
    int param_1;
    int param_2;
    int param_3;
    u32 param_4;
    u32 param_5;
    int param_6;
{
  int iVar1;
  int iVar2;
  u32 uVar3;
  int iVar4;
  int iVar5;
  
  if (1 < *(int *)(param_6 + 0x118)) {
    iVar4 = FUN_0202e3d4(DAT_02032ecc,0);
    uVar3 = DAT_02032ecc;
    for (; (iVar4 != 0 && (*(int *)(iVar4 + 8) != param_6)); iVar4 = FUN_0202e3d4(uVar3,iVar4)) {
    }
    iVar5 = 0;
    if (0 < *(int *)(iVar4 + 0x10)) {
      do {
        FUN_0203b914(*(u32 *)(iVar4 + iVar5 * 4 + 0x14),0,*(u32 *)(iVar4 + 0x2c));
        iVar5 = iVar5 + 1;
      } while (iVar5 < *(int *)(iVar4 + 0x10));
    }
    FUN_0202e3ec(DAT_02032ecc,iVar4);
    *(int *)(param_6 + 0x118) = *(int *)(param_6 + 0x118) + -1;
    FUN_02032f8c(iVar4);
  }
  iVar4 = FUN_02032fc4();
  *(int *)(iVar4 + 8) = param_6;
  *(int *)(iVar4 + 0xc) = param_1;
  *(int *)(iVar4 + 0x10) = param_2;
  iVar5 = 0;
  if (0 < param_2) {
    do {
      iVar2 = iVar5 * 4;
      iVar1 = iVar5 * 4;
      iVar5 = iVar5 + 1;
      *(u32 *)(iVar4 + iVar1 + 0x14) = *(u32 *)(param_3 + iVar2);
    } while (iVar5 < param_2);
  }
  *(u32 *)(iVar4 + 0x2c) = param_4;
  iVar4 = DAT_02032ed0;
  if ((param_1 == 0) && (*DAT_02032ed4 != 0)) {
    iVar4 = *DAT_02032ed4;
  }
  iVar5 = iVar4 + DAT_02032ed8;
  *(int *)(param_6 + 0x118) = *(int *)(param_6 + 0x118) + 1;
  FUN_0202e53c(iVar5);
  FUN_020394d4(iVar4 + DAT_02032edc);
  return;
}

// FUN_02032ee0 @ 0x02032ee0 (160 bytes)
void FUN_02032ee0(param_1, param_2, param_3)
    int param_1;
    u32 param_2;
    int param_3;
{
  if (param_1 != *(int *)(param_3 + 0x128)) {
    return;
  }
  FUN_02039f08(DAT_02032f80);
  if (*DAT_02032f84 != 0) {
    FUN_02039f08(*DAT_02032f84 + DAT_02032f88);
  }
  FUN_02033248(param_3);
  *(u32 *)(param_3 + 0x128) = 0;
  *(u32 *)(param_3 + 300) = 0;
  *(u8 *)(param_3 + 0x120) = 0;
  if (0 < *(int *)(param_3 + 0x11c)) {
    FUN_0202fcd0(param_3);
    *(u32 *)(param_3 + 0x11c) = 0;
  }
  FUN_02039e98(DAT_02032f80);
  if (*DAT_02032f84 != 0) {
    FUN_02039e98(*DAT_02032f84 + DAT_02032f88);
    return;
  }
  return;
}

// FUN_02032f8c @ 0x02032f8c (52 bytes)
void FUN_02032f8c(param_1)
    u32 param_1;
{
  u32 uVar1;
  
  uVar1 = FUN_0203ae38();
  FUN_0202e53c(DAT_02032fc0,param_1);
  FUN_0203ae4c(uVar1);
  return;
}

// FUN_02032fc4 @ 0x02032fc4 (72 bytes)
int FUN_02032fc4(...)
{
  u32 uVar1;
  int iVar2;
  
  uVar1 = FUN_0203ae38();
  iVar2 = FUN_0202e3d4(DAT_0203300c,0);
  if (iVar2 != 0) {
    FUN_0202e3ec(DAT_0203300c,iVar2);
  }
  FUN_0203ae4c(uVar1);
  return iVar2;
}

// FUN_02033010 @ 0x02033010 (84 bytes)
int FUN_02033010(param_1)
    u32 param_1;
{
  u32 uVar1;
  int iVar2;
  
  uVar1 = FUN_0203ae38();
  iVar2 = FUN_0202e3d4(param_1,0);
  if (iVar2 != 0) {
    FUN_0202e3ec(param_1,iVar2);
    *(int *)(*(int *)(iVar2 + 8) + 0x118) = *(int *)(*(int *)(iVar2 + 8) + 0x118) + -1;
  }
  FUN_0203ae4c(uVar1);
  return iVar2;
}

// FUN_02033064 @ 0x02033064 (116 bytes)
void FUN_02033064(param_1, param_2)
    u32 param_1;
    int param_2;
{
  int iVar1;
  u32 uVar2;
  int iVar3;
  
  uVar2 = FUN_0203ae38();
  iVar3 = FUN_0202e3d4(param_1,0);
  while (iVar1 = iVar3, iVar1 != 0) {
    iVar3 = FUN_0202e3d4(param_1,iVar1);
    if (*(int *)(iVar1 + 8) == param_2) {
      FUN_0202e3ec(param_1,iVar1);
      FUN_02032f8c(iVar1);
    }
  }
  FUN_0203ae4c(uVar2);
  return;
}

// FUN_020330d8 @ 0x020330d8 (104 bytes)
void FUN_020330d8(param_1, param_2)
    int param_1;
    u32 param_2;
{
  FUN_0203960c(param_1,DAT_02033144,param_1,param_1 + DAT_02033140,0x400,param_2);
  FUN_0202e5d0(param_1 + DAT_02033148,0);
  FUN_02039f94(param_1 + DAT_0203314c);
  *(u16 *)(param_1 + 0x494) = 0;
  FUN_020394a0(param_1);
  return;
}

// FUN_02033150 @ 0x02033150 (72 bytes)
void FUN_02033150(param_1)
    int param_1;
{
  if (*(int *)(param_1 + 0x11c) == 0) {
    return;
  }
  *(int *)(param_1 + 0x11c) = *(int *)(param_1 + 0x11c) + -1;
  if (*(int *)(param_1 + 0x11c) == 0) {
    FUN_0202fcd0();
    return;
  }
  return;
}

// FUN_02033198 @ 0x02033198 (64 bytes)
u32 FUN_02033198(param_1)
    int param_1;
{
  int iVar1;
  
  if ((*(int *)(param_1 + 0x11c) == 0) && (iVar1 = FUN_0202fd00(), iVar1 == 0)) {
    return 0;
  }
  *(int *)(param_1 + 0x11c) = *(int *)(param_1 + 0x11c) + 1;
  return 1;
}

// FUN_020331d8 @ 0x020331d8 (100 bytes)
void FUN_020331d8(param_1)
    int param_1;
{
  if (*(int *)(param_1 + 0x10c) << 0x1f < 0) {
    FUN_02033150();
    FUN_0203ef08(param_1 + 0x5c);
    FUN_02033064(DAT_0203323c,param_1);
    if (*DAT_02033240 != 0) {
      FUN_02033064(*DAT_02033240 + DAT_02033244,param_1);
    }
    FUN_02033624(param_1);
    return;
  }
  return;
}

// FUN_02033248 @ 0x02033248 (148 bytes)
void FUN_02033248(param_1)
    int param_1;
{
  FUN_02039f08(DAT_020332dc);
  if (*DAT_020332e0 != 0) {
    FUN_02039f08(*DAT_020332e0 + DAT_020332e4);
  }
  if (*(int *)(param_1 + 0x10c) << 0x1e < 0) {
    FUN_0202fa74(param_1);
  }
  if (*(int *)(param_1 + 0x10c) << 0x1f < 0) {
    FUN_0203edec(param_1 + 0x5c);
  }
  FUN_020331d8(param_1);
  FUN_02039e98(DAT_020332dc);
  if (*DAT_020332e0 != 0) {
    FUN_02039e98(*DAT_020332e0 + DAT_020332e4);
    return;
  }
  return;
}
