/**
 * SDK Math -- fixed-point math, trigonometry, matrix ops (0x0202exxx-0x0202fxxx)
 * Decompiled from 0x0202ec4c-0x0202fff8 region (48 functions)
 */
#include "types.h"

typedef u32 (*code)(u32, u32, u32, u32, u32, u32);

/* External function declarations */
extern u32 FUN_0202e3d4(u32 a0, u32 a1);
extern u32 FUN_0202e3ec(u32 a0, u32 a1);
extern u32 FUN_0202e458(u32 a0, u32 a1, u32 a2);
extern u32 FUN_0202e53c(u32 a0, u32 a1);
extern u32 FUN_0202e5d0(u32 a0, u32 a1);
extern u32 FUN_0202eba8(u32 a0);
extern u32 FUN_0202ebc8(void);
extern u32 FUN_020311a8(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4);
extern u32 FUN_02031218(u32 a0);
extern u32 FUN_020312d8(void);
extern u32 FUN_020312f4(u32 a0, u32 a1);
extern u32 FUN_02034150(u32 a0);
extern u32 FUN_02034168(u32 a0);
extern u32 FUN_02034180(u32 a0);
extern u32 FUN_020341bc(u32 a0, u32 a1, u32 a2);
extern u32 FUN_020341ec(u32 a0);
extern u32 FUN_02039bf0(u32 a0, u32 a1, u32 a2);
extern u32 FUN_02039c9c(u32 a0, u32 a1, u32 a2);
extern u32 FUN_0203a014(u32 a0, u32 a1);
extern u32 FUN_0203ae38(void);
extern u32 FUN_0203ae4c(u32 a0);
extern u32 FUN_0203bf4c(u32 a0, u32 a1, u32 a2, u32 a3);
extern u32 FUN_0203bfa4(u32 a0, u32 a1);
extern u32 FUN_0203bfd0(u32 a0, u32 a1, u32 a2);
extern u32 FUN_0203c004(u32 a0, u32 a1);
extern u32 FUN_0203c030(u32 a0, u32 a1);
extern u32 FUN_0203c05c(u32 a0, u32 a1, u32 a2, u32 a3);
extern u32 FUN_0203c0ec(u32 a0, u32 a1, u32 a2, u32 a3);
extern u32 FUN_0203c15c(u32 a0, u32 a1, u32 a2, u32 a3);
extern u32 FUN_0203c190(u32 a0, u32 a1);
extern u32 FUN_0203c1c4(u32 a0, u32 a1, u32 a2);
extern u32 FUN_0203c1ec(void);
extern u32 FUN_0203c218(u32 a0);
extern u32 FUN_0203c230(u32 a0, u32 a1);
extern u32 FUN_0203c248(u32 a0);
extern u32 FUN_0203c274(u32 a0);
extern u32 FUN_0203c2a8(u32 a0);
extern u32 FUN_0203c558(u32 a0);
extern u32 FUN_0203c5ac(void);
extern u32 FUN_0203c5ec(u32 a0);
extern u32 FUN_0203c67c(u32 a0);
extern u32 FUN_0203cd00(void);
extern u32 FUN_0203cd34(u32 a0);
extern u32 FUN_020412ac(u32 a0);
extern u32 FUN_020412c4(u32 a0);
extern u32 FUN_020412dc(u32 a0);
extern u32 FUN_020412f4(u32 a0);
extern u32 FUN_020466bc(u32 a0, u32 a1);

/* External data declarations */
extern u32 *DAT_0202ec80;
extern u32 *DAT_0202ecd4;
extern u32 DAT_0202ed38;
extern u32 DAT_0202ed94;
extern u32 DAT_0202ee20;
extern u32 DAT_0202ee24;
extern u32 DAT_0202ee9c;
extern u32 DAT_0202eea0;
extern u32 DAT_0202eedc;
extern u32 DAT_0202ef3c;
extern u32 DAT_0202f040;
extern u32 DAT_0202f0fc;
extern u32 DAT_0202f1c4;
extern u32 DAT_0202f330;
extern u32 DAT_0202f334;
extern u32 DAT_0202f338;
extern u32 DAT_0202f3e4;
extern u32 DAT_0202f3e8;
extern u32 DAT_0202f3ec;
extern u32 DAT_0202f3f0;
extern u32 DAT_0202f490;
extern u32 DAT_0202f58c;
extern u32 DAT_0202f620;
extern u32 DAT_0202f688;
extern void (*DAT_0202f698)(u32);
extern u32 DAT_0202f734;
extern u32 DAT_0202f738;
extern u32 DAT_0202f770;
extern u32 DAT_0202f8f0;
extern u32 DAT_0202f8f4;
extern u32 DAT_0202f938;
extern u32 DAT_0202fa70;
extern u32 DAT_0202fcc4;
extern u32 DAT_0202fcc8;
extern u32 DAT_0202fccc;
extern u32 *DAT_0202fdd8;
extern u32 DAT_0202fddc;
extern u32 DAT_0202fde0;
extern u32 DAT_0202fde4;
extern u32 DAT_0202fe50;
extern u32 *DAT_0202ff48;
extern int *DAT_0202ff4c;
extern u32 DAT_0202ff50;
extern u32 DAT_0202ff54;
extern u32 *DAT_02030054;

/* Forward declarations */
void FUN_0202ec4c(u32 param_1);
u32 FUN_0202ec84(u32 param_1);
void FUN_0202ecd8(int param_1, u8 param_2);
void FUN_0202ed3c(int param_1);
void FUN_0202ed98(int param_1);
int FUN_0202ee28(int param_1);
void FUN_0202eea4(int param_1);
void FUN_0202eee0(int param_1);
void FUN_0202ef40(u32 param_1, int param_2);
void FUN_0202ef9c(int param_1);
u32 FUN_0202efe8(int param_1, int param_2);
void FUN_0202f044(int param_1, int param_2);
void FUN_0202f0a4(int param_1);
u32 *FUN_0202f10c(int *param_1, int param_2, int param_3);
void FUN_0202f1c8(void);
void FUN_0202f33c(void);
u32 FUN_0202f3f4(void);
void FUN_0202f410(int *param_1, u16 param_2, u16 param_3);
void FUN_0202f438(int *param_1, u16 param_2);
void FUN_0202f454(int *param_1, u32 param_2, int param_3);
void FUN_0202f494(int *param_1);
void FUN_0202f4c4(int *param_1, int param_2);
void FUN_0202f50c(int *param_1, u8 param_2);
int FUN_0202f51c(u32 param_1, u32 param_2);
void FUN_0202f590(u32 *param_1);
void FUN_0202f5a8(u32 *param_1);
void FUN_0202f5b4(u32 param_1, u32 param_2, u32 param_3);
void FUN_0202f624(u32 param_1, u32 param_2);
void FUN_0202f68c(u32 *param_1);
u32 FUN_0202f69c(int param_1, u32 param_2, int param_3);
void FUN_0202f754(int param_1, u32 param_2);
void FUN_0202f774(int param_1);
void FUN_0202f7ec(int param_1);
void FUN_0202f840(int param_1, u32 param_2);
void FUN_0202f908(int param_1);
void FUN_0202f93c(int param_1);
void FUN_0202f9bc(int param_1, int param_2, u32 param_3);
void FUN_0202fa00(int param_1, u32 param_2);
void FUN_0202fa74(int param_1);
void FUN_0202faa4(int param_1);
u32 FUN_0202fb00(int param_1, int param_2, int param_3, u32 param_4, int param_5, int param_6, u32 param_7, u32 param_8);
void FUN_0202fcd0(int param_1);
BOOL FUN_0202fd00(int param_1, int param_2, int param_3);
void FUN_0202fd64(int param_1);
void FUN_0202fde8(void);
void FUN_0202fe54(int param_1);
void FUN_0202fff8(u32 param_1, u32 param_2, int param_3);

// FUN_0202ec4c @ 0x0202ec4c (52 bytes)
void FUN_0202ec4c(u32 param_1)

{
  if (param_1 != 0) {
    FUN_0203c004(param_1,0);
    *DAT_0202ec80 = *DAT_0202ec80 & ~param_1;
    return;
  }
  return;
}

// FUN_0202ec84 @ 0x0202ec84 (80 bytes)
u32 FUN_0202ec84(u32 param_1)

{
  if (param_1 == 0) {
    return 1;
  }
  if ((param_1 & *DAT_0202ecd4) == 0) {
    FUN_0203c030(param_1,0);
    *DAT_0202ecd4 = *DAT_0202ecd4 | param_1;
    return 1;
  }
  return 0;
}

// FUN_0202ecd8 @ 0x0202ecd8 (96 bytes)
void FUN_0202ecd8(int param_1,u8 param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 4);
  if (iVar1 != 0) {
    FUN_0202e3ec(iVar1,param_1);
    *(u32 *)(param_1 + 4) = 0;
  }
  FUN_0202e3ec(DAT_0202ed38,param_1);
  *(u8 *)(param_1 + 0x3d) = param_2;
  if (iVar1 != 0) {
    FUN_0202ef40(iVar1,param_1);
  }
  FUN_0202eee0(param_1);
  return;
}

// FUN_0202ed3c @ 0x0202ed3c (88 bytes)
void FUN_0202ed3c(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return;
  }
  FUN_020312d8();
  if (*(int *)(param_1 + 0xc) == 0) {
    FUN_0202e3ec(*(int *)(param_1 + 0x10) * 0x24 + DAT_0202ed94 + 0xc,param_1);
    return;
  }
  *(u32 *)(*(int *)(param_1 + 0xc) + 8) = 0;
  return;
}

// FUN_0202ed98 @ 0x0202ed98 (136 bytes)
void FUN_0202ed98(int param_1)

{
  int iVar1;
  
  if (*(u32 *)param_1 != 0) {
    *(u32 *)*(u32 *)param_1 = 0;
    *(u32 *)param_1 = 0;
  }
  iVar1 = *(u32 *)(param_1 + 4);
  FUN_0202e3ec(iVar1,param_1);
  *(u32 *)(param_1 + 4) = 0;
  if (*(u32 *)(param_1 + 8) != 0) {
    FUN_0202e53c(iVar1 + 0xc, 0);
    *(u32 *)(*(u32 *)(param_1 + 8) + 0xc) = 0;
    *(u32 *)(param_1 + 8) = 0;
  }
  FUN_0202e3ec(DAT_0202ee20,param_1);
  FUN_0202e53c(DAT_0202ee24,param_1);
  *(u8 *)(param_1 + 0x2c) = 0;
  return;
}

// FUN_0202ee28 @ 0x0202ee28 (116 bytes)
int FUN_0202ee28(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0202e3d4(DAT_0202ee9c,0);
  if (iVar1 == 0) {
    iVar1 = FUN_0202e3d4(DAT_0202eea0,0);
    if (param_1 < (int)(u32)*(u8 *)(iVar1 + 0x3d)) {
      return 0;
    }
    FUN_0202eea4(iVar1);
  }
  FUN_0202e3ec(DAT_0202ee9c,iVar1);
  *(char *)(iVar1 + 0x3d) = (char)param_1;
  FUN_0202eee0(iVar1);
  return iVar1;
}

// FUN_0202eea4 @ 0x0202eea4 (56 bytes)
void FUN_0202eea4(int param_1)

{
  if (*(char *)(param_1 + 0x2c) == '\x02') {
    FUN_0203c230(*(u8 *)(param_1 + 0x3c),DAT_0202eedc);
  }
  FUN_0203c2a8(*(u8 *)(param_1 + 0x3c));
  FUN_0202ed98(param_1);
  return;
}

// FUN_0202eee0 @ 0x0202eee0 (92 bytes)
void FUN_0202eee0(int param_1)

{
  u32 uVar1;
  int iVar2;
  
  iVar2 = FUN_0202e3d4(DAT_0202ef3c,0);
  uVar1 = DAT_0202ef3c;
  while ((iVar2 != 0 && (*(u8 *)(iVar2 + 0x3d) <= *(u8 *)(param_1 + 0x3d)))) {
    iVar2 = FUN_0202e3d4(uVar1, 0);
  }
  FUN_0202e458(DAT_0202ef3c,iVar2,param_1);
  return;
}

// FUN_0202ef40 @ 0x0202ef40 (92 bytes)
void FUN_0202ef40(u32 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_0202e3d4(param_1,0);
  while ((iVar1 != 0 && (*(u8 *)(iVar1 + 0x3d) <= *(u8 *)(param_2 + 0x3d)))) {
    iVar1 = FUN_0202e3d4(param_1, 0);
  }
  FUN_0202e458(param_1,iVar1,param_2);
  *(u32 *)(param_2 + 4) = param_1;
  return;
}

// FUN_0202ef9c @ 0x0202ef9c (76 bytes)
void FUN_0202ef9c(int param_1)

{
  *(u8 *)(param_1 + 0x2e) = 0;
  *(u8 *)(param_1 + 0x2d) = 0;
  *(u8 *)(param_1 + 0x2f) = 0;
  *(u16 *)(param_1 + 0x34) = 0;
  *(u16 *)(param_1 + 0x3e) = 0;
  *(u8 *)(param_1 + 0x40) = 0x7f;
  *(u8 *)(param_1 + 0x41) = 0x7f;
  FUN_020341ec(param_1 + 0x1c);
  FUN_020341bc(param_1 + 0x1c,0x7f00,1);
  return;
}

// FUN_0202efe8 @ 0x0202efe8 (88 bytes)
u32 FUN_0202efe8(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x24 + DAT_0202f040;
  iVar1 = FUN_0202e3d4(iVar2 + 0xc,0);
  if (iVar1 != 0) {
    FUN_0202e3ec(iVar2 + 0xc,iVar1);
    *(int *)(iVar1 + 0xc) = param_2;
    *(int *)(param_2 + 8) = iVar1;
    FUN_02031218(*(u32 *)(iVar1 + 8));
    return *(u32 *)(iVar1 + 8);
  }
  return 0;
}

// FUN_0202f044 @ 0x0202f044 (96 bytes)
void FUN_0202f044(int param_1,int param_2)

{
  if (param_1 == 0) {
    return;
  }
  if (*(char *)(param_1 + 0x2c) != '\0') {
    if (param_2 != 0) {
      FUN_020341bc(param_1 + 0x1c,0, 0);
      FUN_0202ecd8(param_1,0);
      *(u8 *)(param_1 + 0x2c) = 2;
      return;
    }
    FUN_0202eea4(param_1);
    return;
  }
  return;
}

// FUN_0202f0a4 @ 0x0202f0a4 (88 bytes)
void FUN_0202f0a4(int param_1)

{
  u32 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 4);
  FUN_0203c274(*(u8 *)(param_1 + 0x3c));
  if (*(int *)(iVar2 + 0x1c) != 0) {
    FUN_0203c190(*(u8 *)(param_1 + 0x3c),DAT_0202f0fc);
  }
  FUN_0202ef9c(param_1);
  uVar1 = FUN_0203c5ac();
  *(u32 *)(param_1 + 0x30) = uVar1;
  *(u8 *)(param_1 + 0x2f) = 1;
  *(u8 *)(param_1 + 0x2c) = 1;
  return;
}

// FUN_0202f10c @ 0x0202f10c (184 bytes)
u32 * FUN_0202f10c(int *param_1,int param_2,int param_3)

{
  int iVar1;
  u32 *puVar2;
  int iVar3;
  
  iVar3 = param_2 * 0x24 + DAT_0202f1c4;
  if (*param_1 != 0) {
    FUN_0202f590((u32 *)param_1);
  }
  if (*(u32 *)(iVar3 + 0x18) <= (u32)*(u16 *)(iVar3 + 8)) {
    iVar1 = FUN_0202e3d4(iVar3,0);
    if (iVar1 == 0) {
      return (u32 *)0x0;
    }
    if (param_3 < (int)(u32)*(u8 *)(iVar1 + 0x3d)) {
      return (u32 *)0x0;
    }
    FUN_0202eea4(iVar1);
  }
  puVar2 = (u32 *)FUN_0202ee28(param_3);
  if (puVar2 != (u32 *)0x0) {
    FUN_0202ef40(iVar3,(int)puVar2);
    *puVar2 = (u32)param_1;
    *param_1 = (int)puVar2;
    return puVar2;
  }
  return (u32 *)0;
}

// FUN_0202f1c8 @ 0x0202f1c8 (360 bytes)
void FUN_0202f1c8(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  u32 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  uVar5 = FUN_0203cd00();
  iVar6 = FUN_0202e3d4(DAT_0202f330,0);
  iVar4 = DAT_0202f334;
  if (iVar6 == 0) {
    return;
  }
  do {
    iVar7 = FUN_0202e3d4(DAT_0202f330,iVar6);
    if ((*(char *)(iVar6 + 0x2d) == '\0') &&
       (iVar8 = FUN_0203c558(*(u32 *)(iVar6 + 0x30)), iVar8 != 0)) {
      *(u8 *)(iVar6 + 0x2d) = 1;
    }
    if ((*(char *)(iVar6 + 0x2d) == '\0') || ((uVar5 & 1 << *(s8 *)(iVar6 + 0x3c)) != 0)) {
      FUN_02034168(iVar6 + 0x1c);
      sVar1 = *(short *)(iVar4 + (u32)*(u8 *)(iVar6 + 0x41) * 2);
      sVar2 = *(short *)(iVar4 + (u32)*(u8 *)(iVar6 + 0x40) * 2);
      sVar3 = *(short *)(iVar4 + (u32)*(u8 *)(*(int *)(iVar6 + 4) + 0x20) * 2);
      iVar8 = FUN_02034180(iVar6 + 0x1c);
      iVar9 = (int)*(short *)(iVar4 + (iVar8 >> 8) * 2) + (int)sVar3 + (int)sVar2 + (int)sVar1;
      iVar8 = DAT_0202f338;
      if ((DAT_0202f338 <= iVar9) && (iVar8 = iVar9, 0 < iVar9)) {
        iVar8 = 0;
      }
      if (iVar8 != *(short *)(iVar6 + 0x3e)) {
        FUN_0203c230(*(u8 *)(iVar6 + 0x3c),iVar8);
        *(short *)(iVar6 + 0x3e) = (short)iVar8;
      }
      if ((*(char *)(iVar6 + 0x2c) == '\x02') && (iVar8 = FUN_02034150(iVar6 + 0x1c), iVar8 != 0)) {
        FUN_0202eea4(iVar6);
      }
      if (*(char *)(iVar6 + 0x2f) != '\0') {
        FUN_0203c248(*(u8 *)(iVar6 + 0x3c));
        *(u8 *)(iVar6 + 0x2f) = 0;
      }
    }
    else {
      FUN_0202ed98(iVar6);
    }
    iVar6 = iVar7;
  } while (iVar7 != 0);
  return;
}

// FUN_0202f33c @ 0x0202f33c (168 bytes)
void FUN_0202f33c(void)

{
  u32 uVar1;
  int iVar2;
  int iVar3;
  
  FUN_0202e5d0(DAT_0202f3e4,0x14);
  FUN_0202e5d0(DAT_0202f3e8,0x14);
  uVar1 = DAT_0202f3e8;
  iVar3 = 0;
  iVar2 = DAT_0202f3ec;
  do {
    *(u8 *)(iVar2 + 0x2c) = 0;
    *(char *)(iVar2 + 0x3c) = (char)iVar3;
    FUN_0202e53c(uVar1,iVar2);
    iVar3 = iVar3 + 1;
    iVar2 = iVar2 + 0x44;
  } while (iVar3 < 0x10);
  iVar3 = 0;
  iVar2 = DAT_0202f3f0;
  do {
    FUN_0202e5d0(iVar2,0xc);
    FUN_0202e5d0(iVar2 + 0xc,0);
    *(u8 *)(iVar2 + 0x20) = 0x7f;
    *(u32 *)(iVar2 + 0x18) = 1;
    iVar3 = iVar3 + 1;
    *(u32 *)(iVar2 + 0x1c) = 0;
    iVar2 = iVar2 + 0x24;
  } while (iVar3 < 0x20);
  return;
}

// FUN_0202f3f4 @ 0x0202f3f4 (28 bytes)
u32 FUN_0202f3f4(void)

{
  FUN_0203c1ec();
  return 1;
}

// FUN_0202f410 @ 0x0202f410 (40 bytes)
void FUN_0202f410(int *param_1,u16 param_2,u16 param_3)

{
  if (*param_1 == 0) {
    return;
  }
  *(u16 *)(*param_1 + 0x34) = 2;
  *(u16 *)(*param_1 + 0x38) = param_2;
  *(u16 *)(*param_1 + 0x3a) = param_3;
  return;
}

// FUN_0202f438 @ 0x0202f438 (28 bytes)
void FUN_0202f438(int *param_1,u16 param_2)

{
  if (*param_1 != 0) {
    *(u16 *)(*param_1 + 0x34) = 1;
    *(u16 *)(*param_1 + 0x38) = param_2;
  }
  return;
}

// FUN_0202f454 @ 0x0202f454 (60 bytes)
void FUN_0202f454(int *param_1,u32 param_2,int param_3)

{
  if (*param_1 != 0) {
    FUN_0203c1c4(*(u8 *)(*param_1 + 0x3c),param_2,
                 (int)*(short *)(DAT_0202f490 + param_3 * 2));
    return;
  }
  return;
}

// FUN_0202f494 @ 0x0202f494 (48 bytes)
void FUN_0202f494(int *param_1)

{
  if (*param_1 != 0) {
    FUN_0203c218(*(u8 *)(*param_1 + 0x3c));
    return;
  }
  return;
}

// FUN_0202f4c4 @ 0x0202f4c4 (72 bytes)
void FUN_0202f4c4(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 == 0) {
    return;
  }
  if (*(char *)(iVar1 + 0x2c) != '\x02') {
    FUN_020341bc(iVar1 + 0x1c,param_2 << 8, 0);
    return;
  }
  return;
}

// FUN_0202f50c @ 0x0202f50c (16 bytes)
void FUN_0202f50c(int *param_1,u8 param_2)

{
  if (*param_1 != 0) {
    *(u8 *)(*param_1 + 0x40) = param_2;
  }
  return;
}

// FUN_0202f51c @ 0x0202f51c (112 bytes)
int FUN_0202f51c(u32 param_1,u32 param_2)

{
  u32 uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  iVar2 = FUN_0202e3d4(DAT_0202f58c,0);
  uVar1 = DAT_0202f58c;
  while (iVar2 != 0) {
    if (((*(short *)(iVar2 + 0x34) == 2) && (*(u16 *)(iVar2 + 0x38) == param_1)) &&
       (*(u16 *)(iVar2 + 0x3a) == param_2)) {
      iVar3 = iVar3 + 1;
    }
    iVar2 = FUN_0202e3d4(uVar1, 0);
  }
  return iVar3;
}

// FUN_0202f590 @ 0x0202f590 (24 bytes)
void FUN_0202f590(u32 *param_1)

{
  if ((u32 *)*param_1 != (u32 *)0x0) {
    *(u32 *)*param_1 = 0;
    *param_1 = 0;
  }
  return;
}

// FUN_0202f5a8 @ 0x0202f5a8 (12 bytes)
void FUN_0202f5a8(u32 *param_1)

{
  *param_1 = 0;
  return;
}

// FUN_0202f5b4 @ 0x0202f5b4 (108 bytes)
void FUN_0202f5b4(u32 param_1,u32 param_2,u32 param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = DAT_0202f620;
  do {
    if ((((*(char *)(iVar1 + 0x2c) != '\0') && (*(short *)(iVar1 + 0x34) == 2)) &&
        (*(u16 *)(iVar1 + 0x38) == param_1)) && (*(u16 *)(iVar1 + 0x3a) == param_2)) {
      FUN_0202f044(iVar1,param_3);
    }
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 0x44;
  } while (iVar2 < 0x10);
  return;
}

// FUN_0202f624 @ 0x0202f624 (100 bytes)
void FUN_0202f624(u32 param_1,u32 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = DAT_0202f688;
  do {
    if (((*(char *)(iVar1 + 0x2c) != '\0') && (*(short *)(iVar1 + 0x34) == 2)) &&
       (*(u16 *)(iVar1 + 0x38) == param_1)) {
      FUN_0202f044(iVar1,param_2);
    }
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 0x44;
  } while (iVar2 < 0x10);
  return;
}

// FUN_0202f68c @ 0x0202f68c (12 bytes)
void FUN_0202f68c(u32 *param_1)

{
  (*DAT_0202f698)(*param_1);
  return;
}

// FUN_0202f69c @ 0x0202f69c (152 bytes)
u32 FUN_0202f69c(int param_1,u32 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  u32 uVar3;
  int iVar4;
  
  iVar1 = FUN_020311a8(param_2,param_3 + 0x14,DAT_0202f734,0,0);
  if (iVar1 == 0) {
    return 0;
  }
  *(u32 *)(iVar1 + 0xc) = 0;
  *(int *)(iVar1 + 0x10) = param_1;
  *(u32 *)(iVar1 + 8) = 0;
  iVar2 = FUN_020312f4(iVar1 + 0x14,param_3);
  if (iVar2 != 0) {
    iVar4 = param_1 * 0x24 + DAT_0202f738;
    *(int *)(iVar1 + 8) = iVar2;
    FUN_0202e53c(iVar4 + 0xc,iVar1);
    return 1;
  }
  return 0;
}

// FUN_0202f754 @ 0x0202f754 (28 bytes)
void FUN_0202f754(int param_1,u32 param_2)

{
  *(u32 *)(DAT_0202f770 + param_1 * 0x24) = param_2 & 0xffff;
  return;
}

// FUN_0202f774 @ 0x0202f774 (120 bytes)
void FUN_0202f774(int param_1)

{
  int iVar1;
  u32 uVar2;
  
  if (*(int *)(param_1 + 0x24) << 0x1e < 0) {
    iVar1 = *(int *)(param_1 + 0x38);
    while (iVar1 != 0) {
      uVar2 = FUN_0203ae38();
      FUN_0202f840(param_1,1);
      FUN_0203ae4c(uVar2);
      iVar1 = *(int *)(param_1 + 0x38);
    }
    FUN_0203c15c(*(u32 *)(param_1 + 0x44),0,1 << (*(u32 *)(param_1 + 0x40) & 0xff),0);
    return;
  }
  return;
}

// FUN_0202f7ec @ 0x0202f7ec (84 bytes)
void FUN_0202f7ec(int param_1)

{
  u32 uVar1;
  
  if (*(int *)(param_1 + 0x24) << 0x1e < 0) {
    FUN_0203c0ec(*(u32 *)(param_1 + 0x44),0,1 << (*(u32 *)(param_1 + 0x40) & 0xff),0);
    uVar1 = FUN_0203c5ac();
    FUN_0203c67c(1);
    FUN_0203c5ec(uVar1);
    return;
  }
  return;
}

// FUN_0202f840 @ 0x0202f840 (176 bytes)
void FUN_0202f840(int param_1,u32 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar3 = FUN_020466bc(*(u32 *)(param_1 + 0x28),*(u32 *)(param_1 + 0x2c));
  iVar2 = DAT_0202f8f4;
  iVar1 = DAT_0202f8f0;
  iVar4 = *(int *)(param_1 + 0x48);
  iVar5 = *(int *)(param_1 + 0x38);
  iVar6 = 0;
  if (0 < iVar4) {
    do {
      *(int *)(iVar2 + iVar6 * 4) =
           *(int *)(iVar1 + (u32)*(u8 *)(param_1 + iVar6 + 0x4c) * 8) + iVar3 * iVar5;
      iVar4 = *(int *)(param_1 + 0x48);
      iVar6 = iVar6 + 1;
    } while (iVar6 < iVar4);
  }
  (**(code **)(param_1 + 0x30))
            (param_2,iVar4,DAT_0202f8f4,iVar3,*(u32 *)(param_1 + 0x20),
             *(u32 *)(param_1 + 0x34));
  *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + 1;
  if (*(int *)(param_1 + 0x2c) <= *(int *)(param_1 + 0x38)) {
    *(u32 *)(param_1 + 0x38) = 0;
  }
  return;
}

// FUN_0202f908 @ 0x0202f908 (48 bytes)
void FUN_0202f908(int param_1)

{
  FUN_0202eba8(*(u32 *)(param_1 + 0x40));
  FUN_0202e3ec(DAT_0202f938,param_1);
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) & 0xfffffffe;
  return;
}

// FUN_0202f93c @ 0x0202f93c (128 bytes)
void FUN_0202f93c(int param_1)

{
  u32 uVar1;
  
  if (*(int *)(param_1 + 0x24) << 0x1e < 0) {
    FUN_0203c0ec(*(u32 *)(param_1 + 0x44),0,1 << (*(u32 *)(param_1 + 0x40) & 0xff),0);
    FUN_020412c4(param_1 + 8);
    FUN_020412ac(param_1 + 0x14);
    *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) & 0xfffffffd;
    uVar1 = FUN_0203c5ac();
    FUN_0203c67c(1);
    FUN_0203c5ec(uVar1);
  }
  FUN_0202f908(param_1);
  return;
}

// FUN_0202f9bc @ 0x0202f9bc (68 bytes)
void FUN_0202f9bc(int param_1,int param_2,u32 param_3)

{
  if (param_2 <= *(int *)(param_1 + 0x48) + -1) {
    FUN_0203bfa4(1 << *(s8 *)(param_1 + param_2 + 0x4c),param_3);
    return;
  }
  return;
}

// FUN_0202fa00 @ 0x0202fa00 (112 bytes)
void FUN_0202fa00(int param_1,u32 param_2)

{
  int iVar1;
  u32 uVar2;
  int iVar3;
  u32 uVar4;
  
  *(u32 *)(param_1 + 0x3c) = param_2;
  iVar1 = DAT_0202fa70;
  iVar3 = 0;
  if (*(int *)(param_1 + 0x48) < 1) {
    return;
  }
  do {
    uVar4 = (u32)*(u8 *)(param_1 + iVar3 + 0x4c);
    uVar2 = FUN_0203cd34(*(int *)(param_1 + 0x3c) + *(int *)(iVar1 + uVar4 * 8 + 4));
    FUN_0203bfd0(1 << uVar4,uVar2 & 0xff,(int)uVar2 >> 8);
    iVar3 = iVar3 + 1;
  } while (iVar3 < *(int *)(param_1 + 0x48));
  return;
}

// FUN_0202fa74 @ 0x0202fa74 (48 bytes)
void FUN_0202fa74(int param_1)

{
  if (*(int *)(param_1 + 0x24) << 0x1f < 0) {
    FUN_0202f93c(param_1);
    return;
  }
  return;
}

// FUN_0202faa4 @ 0x0202faa4 (92 bytes)
void FUN_0202faa4(int param_1)

{
  FUN_0203c15c(*(u32 *)(param_1 + 0x44),0,1 << (*(u32 *)(param_1 + 0x40) & 0xff),0);
  if (-1 < *(int *)(param_1 + 0x24) << 0x1e) {
    FUN_020412f4(param_1 + 8);
    FUN_020412dc(param_1 + 0x14);
    *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) | 2;
    return;
  }
  return;
}

// FUN_0202fb00 @ 0x0202fb00 (452 bytes)
u32
FUN_0202fb00(int param_1,int param_2,int param_3,u32 param_4,int param_5,int param_6,
            u32 param_7,u32 param_8)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  int iVar5;
  
  if (*(int *)(param_1 + 0x24) << 0x1f < 0) {
    FUN_0202fa74(param_1);
  }
  iVar1 = FUN_020466bc(param_4,*(int *)(param_1 + 0x48) * param_6 * 0x20);
  *(int *)(param_1 + 0x28) = iVar1 * param_6 * 0x20;
  uVar2 = *(u32 *)(param_1 + 0x28);
  if (param_2 == 1) {
    uVar2 = uVar2 >> 1;
  }
  uVar3 = FUN_020466bc(param_5 * uVar2,param_6);
  uVar4 = FUN_0202ebc8();
  *(u32 *)(param_1 + 0x40) = uVar4;
  iVar1 = DAT_0202fcc4;
  if (*(int *)(param_1 + 0x40) < 0) {
    return 0;
  }
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x48)) {
    do {
      uVar2 = (u32)*(u8 *)(param_1 + iVar5 + 0x4c);
      *(int *)(iVar1 + uVar2 * 8) = *(int *)(param_1 + 0x28) * iVar5 + param_3;
      *(u32 *)(iVar1 + uVar2 * 8 + 4) = 0;
      FUN_0203bf4c(uVar2,param_2,*(u32 *)(iVar1 + uVar2 * 8),1);
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)(param_1 + 0x48));
  }
  FUN_0203c05c(*(u32 *)(param_1 + 0x40),uVar3,uVar3,DAT_0202fcc8);
  FUN_0202e53c(DAT_0202fccc,param_1);
  *(int *)(param_1 + 0x20) = param_2;
  *(int *)(param_1 + 0x2c) = param_6;
  *(u32 *)(param_1 + 0x30) = param_7;
  *(u32 *)(param_1 + 0x34) = param_8;
  *(u32 *)(param_1 + 0x38) = 0;
  *(u32 *)(param_1 + 0x3c) = 0;
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) & 0xfffffffe | 1;
  uVar3 = FUN_0203ae38();
  *(u32 *)(param_1 + 0x2c) = 1;
  FUN_0202f840(param_1,0);
  *(int *)(param_1 + 0x2c) = param_6;
  FUN_0203ae4c(uVar3);
  return 1;
}

// FUN_0202fcd0 @ 0x0202fcd0 (48 bytes)
void FUN_0202fcd0(int param_1)

{
  if (*(int *)(param_1 + 0x44) != 0) {
    FUN_0202ec4c(*(u32 *)(param_1 + 0x44));
    *(u32 *)(param_1 + 0x44) = 0;
    *(u32 *)(param_1 + 0x48) = 0;
    return;
  }
  return;
}

// FUN_0202fd00 @ 0x0202fd00 (100 bytes)
BOOL FUN_0202fd00(int param_1,int param_2,int param_3)

{
  s8 *psVar1;
  u32 uVar2;
  int iVar3;
  
  uVar2 = 0;
  iVar3 = 0;
  if (0 < param_2) {
    do {
      *(u8 *)(param_1 + iVar3 + 0x4c) = *(u8 *)(param_3 + iVar3);
      psVar1 = (s8 *)(param_3 + iVar3);
      iVar3 = iVar3 + 1;
      uVar2 = uVar2 | 1 << *psVar1;
    } while (iVar3 < param_2);
  }
  iVar3 = FUN_0202ec84(uVar2);
  if (iVar3 != 0) {
    *(int *)(param_1 + 0x48) = param_2;
    *(u32 *)(param_1 + 0x44) = uVar2;
  }
  return iVar3 != 0;
}

// FUN_0202fd64 @ 0x0202fd64 (116 bytes)
void FUN_0202fd64(int param_1)

{
  u32 uVar1;
  
  if (*DAT_0202fdd8 == 0) {
    FUN_0202e5d0(DAT_0202fddc,0);
    *DAT_0202fdd8 = 1;
  }
  uVar1 = DAT_0202fde4;
  *(u32 *)(param_1 + 8) = DAT_0202fde0;
  *(int *)(param_1 + 0xc) = param_1;
  *(u32 *)(param_1 + 0x14) = uVar1;
  *(int *)(param_1 + 0x18) = param_1;
  *(u32 *)(param_1 + 0x44) = 0;
  *(u32 *)(param_1 + 0x48) = 0;
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) & 0xfffffffe;
  *(u32 *)(param_1 + 0x24) = *(u32 *)(param_1 + 0x24) & 0xfffffffd;
  return;
}

// FUN_0202fde8 @ 0x0202fde8 (104 bytes)
void FUN_0202fde8(void)

{
  u32 uVar1;
  int *piVar2;
  int iVar3;
  int *local_1c [2];
  
  uVar1 = DAT_0202fe50;
  do {
    FUN_02039bf0(uVar1,(u32)local_1c,1);
    piVar2 = local_1c[0];
    FUN_0203a014(local_1c[0][3],local_1c[0][1]);
    FUN_0203a014(piVar2[4],piVar2[1]);
    iVar3 = *piVar2;
    (**(code **)(iVar3 + 0x34))
              (piVar2[3],piVar2[4],piVar2[1],*(u32 *)(iVar3 + 8),
               *(u32 *)(iVar3 + 0x38), 0);
  } while( true );
}

// FUN_0202fe54 @ 0x0202fe54 (244 bytes)
void FUN_0202fe54(int param_1)

{
  int *piVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar4 = DAT_0202ff50;
  iVar6 = *(int *)(param_1 + 0x18);
  iVar3 = iVar6 * *(int *)(param_1 + 0x1c);
  iVar8 = *(int *)(param_1 + 0xc) + iVar3;
  iVar7 = *(int *)(param_1 + 0x10) + iVar3;
  if (*DAT_0202ff48 == 0) {
    FUN_0203a014(iVar8,iVar6);
    FUN_0203a014(iVar7,iVar6);
    (**(code **)(param_1 + 0x34))
              (iVar8,iVar7,iVar6,*(u32 *)(param_1 + 8),*(u32 *)(param_1 + 0x38), 0);
  }
  else {
    iVar5 = *DAT_0202ff4c;
    *(int *)(DAT_0202ff50 + iVar5 * 0x14) = param_1;
    iVar4 = iVar4 + iVar5 * 0x14;
    *(int *)(iVar4 + 4) = iVar6;
    *(int *)(iVar4 + 8) = iVar3;
    *(int *)(iVar4 + 0xc) = iVar8;
    uVar2 = DAT_0202ff54;
    *(int *)(iVar4 + 0x10) = iVar7;
    FUN_02039c9c(uVar2,iVar4,0);
    piVar1 = DAT_0202ff4c;
    iVar4 = *DAT_0202ff4c;
    *DAT_0202ff4c = iVar4 + 1;
    if (7 < iVar4 + 1) {
      *piVar1 = 0;
    }
  }
  *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
  if (*(int *)(param_1 + 0x30) <= *(int *)(param_1 + 0x1c)) {
    *(u32 *)(param_1 + 0x1c) = 0;
  }
  return;
}

// FUN_0202fff8 @ 0x0202fff8 (92 bytes)
void FUN_0202fff8(u32 param_1,u32 param_2,int param_3)

{
  u32 uVar1;
  u32 uVar2;
  
  if (*DAT_02030054 != 0) {
    uVar2 = DAT_02030054[0xb];
    if (-1 < (int)uVar2) {
      param_3 = 1 << (uVar2 & 0xff);
    }
    if (-1 >= (int)uVar2) {
      param_3 = 0;
    }
    FUN_0203c0ec(DAT_02030054[9],DAT_02030054[10],param_3,0);
    uVar1 = FUN_0203c5ac();
    FUN_0203c67c(1);
    FUN_0203c5ec(uVar1);
    return;
  }
  return;
}
