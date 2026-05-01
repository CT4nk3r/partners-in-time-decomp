/**
 * Game Engine A -- core engine functions (0x02020xxx-0x02026xxx)
 * Decompiled from 0x02020008-0x020268c8 region (49 functions)
 */
#include "types.h"

/* Suppress Ghidra type-mismatch artifacts (int/pointer casts) */
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/* Ghidra sub-word access helpers */
#define LBYTE(x)     ((u8)(x))
#define LWORD(x)     ((u16)(x))
#define HWORD(x)     ((u16)((x) >> 16))
#define UBYTES123(x) ((u32)((x) >> 8))

/* Ghidra CONCAT helpers: combine sub-word values into larger types */
#define CONCAT22(hi16, lo16)  (((u32)(hi16) << 16) | ((u16)(lo16)))
#define CONCAT31(hi24, lo8)   (((u32)(hi24) << 8) | ((u8)(lo8)))

typedef int (*FuncPtr)(...);

/* ============================================================
 * External function declarations
 * ============================================================ */
extern void FUN_020084a0(...);
extern void FUN_02008534(...);
extern void FUN_0201a7b0(...);
extern void FUN_0201ae04(...);
extern void FUN_0201b418(...);
extern void FUN_0201ccf4(...);
extern void FUN_0201d0cc(...);
extern int FUN_0201d3bc(...);
extern void FUN_0201f360(...);
extern void FUN_02022048(...);
extern void FUN_0202277c(...);
extern void FUN_02025810(...);
extern void FUN_02027798(...);
extern void FUN_020278fc(...);
extern void FUN_02027a68(...);
extern void FUN_02027cf4(...);
extern u32 FUN_02029bf8(...);
extern void FUN_0202a034(...);
extern void FUN_0202cc10(...);
extern void FUN_0202cc94(...);
extern void FUN_0202cd68(...);
extern void FUN_02030790(...);
extern void FUN_0203a04c(...);
extern void FUN_0203b7c0(...);
extern void FUN_0203b7f0(...);
extern void FUN_0203b808(...);
extern void FUN_0203b854(...);
extern void FUN_0203b914(...);
extern void FUN_0203bec8(...);
extern u32 FUN_0203cda8(...);
extern void FUN_0203cdf0(...);
extern void thunk_FUN_02029ab8(...);

/* ============================================================
 * External data references (DAT_)
 * ============================================================ */
extern u32 DAT_02020290;
extern u32 DAT_02020294;
extern u32 DAT_020204b0;
extern u32 DAT_020204b4;
extern u32 DAT_020204b8;
extern u32 DAT_0202055c;
extern u32 DAT_02020858;
extern u32 DAT_0202085c;
extern u32 *DAT_02020aac;
extern u32 DAT_02020ab0;
extern u32 DAT_02020ab4;
extern u32 DAT_02020da0;
extern u32 DAT_02020da4;
extern u32 DAT_02020da8;
extern u32 DAT_02020dac;
extern u32 DAT_02020db0;
extern u32 DAT_02020db4;
extern u32 DAT_02020f1c;
extern u32 DAT_02020f54;
extern u32 DAT_02020fe4;
extern u32 DAT_02021124;
extern u32 DAT_02021128;
extern u32 *DAT_02021578;
extern u32 DAT_0202157c;
extern u32 DAT_02021580;
extern u32 DAT_02021584;
extern u32 DAT_020216e4;
extern u32 DAT_020216e8;
extern u32 DAT_020216ec;
extern u32 DAT_02021acc;
extern u32 DAT_02021ad0;
extern u32 DAT_02021ad4;
extern u32 DAT_02021ad8;
extern u32 DAT_02021c2c;
extern u32 DAT_02021c30;
extern u32 DAT_02021f54;
extern u32 DAT_02021f58;
extern u32 DAT_02021f5c;
extern u32 DAT_02021f60;
extern u32 DAT_02021f64;
extern u32 DAT_02021f68;
extern u32 DAT_02021f6c;
extern u32 DAT_02021f70;
extern u32 DAT_02021f74;
extern u32 DAT_02021f78;
extern u32 DAT_02024a98;
extern u32 DAT_02024a9c;
extern u32 DAT_02024aa0;
extern u32 DAT_02024aa4;
extern u32 DAT_02024aa8;
extern u32 DAT_02025184;
extern u32 DAT_02025188;
extern u32 DAT_0202518c;
extern u32 DAT_02025190;
extern u32 DAT_020257fc;
extern u32 DAT_02025800;
extern u32 DAT_02025804;
extern u32 DAT_02025808;
extern u32 DAT_0202580c;
extern u32 DAT_020265a8;
extern u16 *DAT_02026794;
extern u32 DAT_02026798;
extern u32 DAT_0202679c;
extern u32 DAT_020267a0;
extern u32 DAT_020267a4;
extern u16 *DAT_020267a8;
extern u32 DAT_020267ac;
extern u32 DAT_020267b0;
extern u16 *DAT_020267b4;
extern u32 DAT_020267b8;
extern u32 DAT_020267bc;
extern u32 DAT_020267c0;
extern u32 DAT_020267c4;
extern u16 *DAT_020267c8;
extern u32 DAT_020267cc;
extern u32 DAT_020267d0;
extern u32 DAT_020267d4;
extern u16 *DAT_020267d8;
extern u32 DAT_020267dc;
extern u32 DAT_020267e0;
extern u16 *DAT_020267e4;
extern u32 DAT_020267e8;
extern u32 DAT_020267ec;
extern u16 *DAT_020267f0;
extern u32 DAT_020267f4;
extern u32 DAT_020267f8;
extern u16 *DAT_020267fc;
extern u32 DAT_02026800;
extern u16 *DAT_02026804;
extern u32 DAT_02026808;
extern u16 *DAT_0202680c;
extern u32 DAT_02026810;
extern u16 *DAT_02026814;
extern u32 DAT_02026818;
extern u16 *DAT_0202681c;
extern u32 DAT_02026820;
extern u16 *DAT_02026824;
extern u32 DAT_02026828;
extern u16 *DAT_0202682c;
extern u32 DAT_02026860;
extern u32 DAT_02026864;

extern int FUN_0201d170(...);
extern void FUN_0201fa50(...);

/* ============================================================
 * Forward declarations
 * ============================================================ */
void FUN_02020008(int param_1,int param_2);
void FUN_02020094(int param_1,short param_2);
void FUN_0202016c(int param_1,u32 param_2,int param_3,int param_4);
void FUN_02020298(int param_1,short param_2);
int FUN_020202ec(int param_1,u32 param_2,int param_3);
void FUN_020203a0(u32 param_1,u32 *param_2);
void FUN_020204bc(int param_1,short param_2);
void FUN_02020560(int param_1,int param_2);
void FUN_02020860(int param_1);
void FUN_02020990(int param_1,int param_2);
u32 FUN_02020ab8(int param_1,int param_2);
int FUN_02020db8(int param_1);
u32 * FUN_02020ee0(u32 *param_1);
u32 * FUN_02020f20(u32 *param_1);
u32 * FUN_02020f58(u32 *param_1);
void FUN_02020fe8(int param_1);
void FUN_02021058(int param_1,u32 param_2,int param_3);
void FUN_0202112c(int param_1,u32 param_2);
int FUN_020211b8(int param_1,int *param_2,int param_3);
void FUN_02021298(int param_1,int param_2,u32 param_3,u16 *param_4);
void FUN_02021588(u32 param_1,int param_2,int *param_3,int param_4,u32 param_5, u8 *param_6,u16 *param_7);
void FUN_020216f0(int param_1,int param_2);
void FUN_02021adc(int param_1,int param_2);
u32 * FUN_02021df0(u32 *param_1);
void FUN_02021f7c(int param_1);
void FUN_02021fd4(int param_1);
void FUN_0202202c(u32 param_1);
void FUN_02022770(u16 *param_1,u16 *param_2);
void FUN_0202296c(int param_1,int param_2);
void FUN_02022c18(int param_1,int param_2);
void FUN_02022e4c(int param_1,int param_2);
void FUN_02022e78(int param_1,int param_2);
void FUN_02022ea4(int param_1,int param_2);
void FUN_02022ed0(int param_1,int param_2);
void FUN_0202309c(int param_1,int param_2);
void FUN_020231e8(u32 *param_1,char *param_2);
void FUN_02023480(u32 *param_1,char *param_2);
void FUN_02023698(u32 *param_1,char *param_2);
void FUN_02023854(u32 *param_1,char *param_2);
void FUN_02023990(u32 *param_1,char *param_2);
void FUN_02023afc(u32 param_1,int param_2,u32 param_3,int param_4,int param_5,u16 param_6, u8 param_7,u32 param_8,int param_9);
void FUN_02024aac(u32 param_1,int param_2,int param_3,int param_4,int param_5,u16 param_6, u8 param_7);
int FUN_02025194(int param_1,u16 *param_2,int param_3);
u32 FUN_02025e74(u8 *param_1);
void FUN_02025fb4(u8 *param_1,u8 *param_2);
void FUN_02026530(u32 param_1);
void FUN_020265ac(u32 param_1,u32 param_2);
void FUN_02026830(int param_1,u32 param_2);
void FUN_020268c8(int param_1,int param_2,int param_3,u32 param_4);

/* ============================================================
 * Function implementations
 * ============================================================ */

// FUN_02020008 @ 0x02020008 (140 bytes)
void FUN_02020008(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (short)param_2 * 0xcc + *(int *)(param_1 + 0xf00);
  *(u32 *)(iVar1 + 100) = *(u32 *)(iVar1 + 100) & 0x7fffffff | 0x80000000;
  FUN_0202277c(*(u32 *)(param_1 + 0x2c),(u32)(param_2 << 0x18) >> 0x10,0);
  FUN_0202277c(*(u32 *)(param_1 + 0x2c),param_2 * 0x100 + 0xffU & 0xffff,0);
  if (*(char *)(iVar1 + 0xc0) != '\0') {
    FUN_0202277c(*(u32 *)(param_1 + 0x2c),
                 param_2 * 0x100 + *(u8 *)(iVar1 + 0xc1) + 1 & 0xffff,0);
    return;
  }
  return;
}

// FUN_02020094 @ 0x02020094 (64 bytes)
void FUN_02020094(int param_1,short param_2)

{
  int iVar1;
  
  FUN_0201ccf4(param_1,*(int *)(param_1 + 0xf00) + param_2 * 0xcc);
  iVar1 = *(int *)(param_1 + 0xf00) + param_2 * 0xcc;
  *(u32 *)(iVar1 + 0x68) = *(u32 *)(iVar1 + 0x68) | 8;
  return;
}

// FUN_0202016c @ 0x0202016c (292 bytes)
void FUN_0202016c(int param_1,u32 param_2,int param_3,int param_4)

{
  int iVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  
  if (param_3 != 0) {
    if (param_4 == 0) {
      iVar1 = *(int *)(param_1 + 0x44) + *(int *)(param_1 + 0x48);
    }
    else {
      iVar1 = param_3 * 0xd4;
    }
    iVar1 = param_3 * 0xcc + iVar1;
    uVar2 = FUN_02029bf8(iVar1,param_2,DAT_02020290,0);
    *(u32 *)(param_1 + 0xf00) = uVar2;
    FUN_0202cc10(*(u32 *)(param_1 + 0xf00),0,iVar1);
    if (*(int *)(param_1 + 0x44) + *(int *)(param_1 + 0x48) != 0) {
      *(int *)(param_1 + 0xf04) = *(int *)(param_1 + 0xf00) + param_3 * 0xcc;
    }
    uVar2 = DAT_02020294;
    iVar1 = (param_3 + -1) * 0x10000;
    iVar4 = iVar1 >> 0x10;
    if (-1 < iVar4) {
      iVar1 = (short)((u32)iVar1 >> 0x10) * 0xcc;
      do {
        iVar3 = *(int *)(param_1 + 0xf00) + iVar1;
        FUN_0203b914(iVar3 + 0x7c,0,0x18);
        *(short *)(iVar3 + 0x88) = (short)uVar2;
        iVar3 = *(int *)(param_1 + 0xf00) + iVar1;
        FUN_0203b914(iVar3 + 0x94,0,0x18);
        *(short *)(iVar3 + 0xa0) = (short)uVar2;
        iVar1 = iVar1 + -0xcc;
        iVar4 = (iVar4 + -1) * 0x10000 >> 0x10;
      } while (-1 < iVar4);
    }
    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) & 0x3fffffff | param_4 << 0x1e;
    *(short *)(param_1 + 0xf08) = (short)param_3;
    return;
  }
  return;
}

// FUN_02020298 @ 0x02020298 (84 bytes)
void FUN_02020298(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = FUN_0201d170();
  if (iVar1 != 0) {
    FUN_0201d0cc(param_1,*(u32 *)(param_2 * 0xcc + *(int *)(param_1 + 0xf00) + 8) & 1);
    return;
  }
  return;
}

// FUN_020202ec @ 0x020202ec (180 bytes)
int FUN_020202ec(int param_1,u32 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_0201d3bc();
  if (iVar2 != -1) {
    iVar1 = (short)iVar2 * 0xcc;
    iVar3 = *(int *)(param_1 + 0xf00) + iVar1;
    *(u32 *)(iVar3 + 8) = *(u32 *)(iVar3 + 8) & 0xfffc7fff;
    FUN_0201b418(param_1,*(int *)(param_1 + 0xf00) + iVar1);
    if (*(int *)(*(int *)(param_1 + 0xf00) + iVar1 + 8) << 0x1f < 0) {
      *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 4;
    }
    else {
      *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 2;
    }
    FUN_0201a7b0(param_1,*(int *)(param_1 + 0xf00) + iVar1);
    if (param_3 != -1) {
      iVar2 = param_3;
    }
    return iVar2;
  }
  return -1;
}

// FUN_020203a0 @ 0x020203a0 (272 bytes)
void FUN_020203a0(u32 param_1,u32 *param_2)

{
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  
  *param_2 = *param_2 & 0xfffffffe;
  uVar1 = DAT_020204b0;
  *param_2 = *param_2 & 0xffffffe1 | 2;
  uVar2 = DAT_020204b4;
  *param_2 = *param_2 & 0xfffffc1f;
  *param_2 = *param_2 & 0xffff83ff;
  uVar3 = DAT_020204b8;
  param_2[2] = param_2[2] & 0xfe07ffff;
  param_2[2] = param_2[2] & 0x81ffffff;
  *param_2 = *param_2 & 0xfffc7fff;
  *param_2 = *param_2 & uVar1;
  param_2[1] = param_2[1] & 0xfffffff0;
  param_2[1] = param_2[1] & 0xffffffef;
  param_2[1] = param_2[1] & 0xfffffe1f;
  param_2[1] = param_2[1] & 0xfffffdff;
  param_2[1] = param_2[1] & 0xfffc03ff;
  param_2[1] = param_2[1] & uVar2;
  param_2[1] = param_2[1] & 0xfffffff;
  param_2[2] = param_2[2] & 0xfffffe00;
  param_2[2] = param_2[2] & uVar3;
  param_2[2] = param_2[2] & 0xfffbffff;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[6] = 0;
  return;
}

// FUN_020204bc @ 0x020204bc (160 bytes)
void FUN_020204bc(int param_1,short param_2)

{
  u32 uVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = param_2 * 0xcc + *(int *)(param_1 + 0xf00);
  *(u8 *)((u32)*(u8 *)(iVar2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44) + 0x18) = 0;
  *(u32 *)(iVar2 + 8) = *(u32 *)(iVar2 + 8) & 0xfffc7fff;
  uVar1 = DAT_0202055c;
  piVar3 = (int *)((u32)*(u8 *)(iVar2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44));
  if (piVar3[2] << 5 < 0) {
    piVar3[2] = piVar3[2] & 0xfbffffff;
    *(short *)(piVar3 + 3) = (short)uVar1;
    *(int *)(*piVar3 + 4) = piVar3[1];
    *(int *)piVar3[1] = *piVar3;
  }
  FUN_020084a0((u32)*(u8 *)(iVar2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44) + 0x1c);
  return;
}

// FUN_02020560 @ 0x02020560 (760 bytes)
void FUN_02020560(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  int iVar4;
  u32 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  u32 *local_68;
  int local_64;
  int iStack_60;
  int local_5c;
  int iStack_58;
  int iStack_4c;
  u32 uStack_3c;
  
  iVar6 = (u32)*(u8 *)(param_2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44);
  piVar9 = (int *)(iVar6 + 0x1c);
  iVar11 = ((u32)(*(int *)(param_2 + 8) << 0x16) >> 0x1b) + 1;
  iVar7 = ((u32)(*(int *)(param_2 + 8) << 0x11) >> 0x1b) + 1;
  local_68 = (u32 *)(*(int *)(param_1 + 0xf48) + (u32)*(u8 *)(param_2 + 0xc4) * 0x60);
  if ((*(u8 *)(iVar6 + 0x30) & 3) == 0) {
    iVar6 = *piVar9 + 0x6400000;
  }
  else {
    iVar6 = *piVar9 + 0x6600000;
  }
  local_64 = *(int *)(param_2 + 0x74);
  local_5c = 0;
  if (iVar7 == 0) {
    return;
  }
  uStack_3c = 8;
  do {
    iStack_60 = 0;
    if (iVar11 != 0) {
      iStack_4c = 0;
      do {
        iVar12 = local_64 + iStack_4c;
        iStack_58 = (int)(short)((short)iVar7 - (short)local_5c);
        if (8 < iStack_58) {
          iStack_58 = 8;
        }
        uVar3 = iVar11 - iStack_60 & 0xffff;
        if (8 < uVar3) {
          uVar3 = uStack_3c;
        }
        uVar2 = (uVar3 << 0x15) >> 0x10;
        iVar4 = ((*local_68 << 0x10) >> 0x1e) * 4 + (*local_68 >> 0x1e);
        uVar5 = (u32)*(u16 *)(DAT_02020858 + iVar4 * 2);
        iVar1 = uVar5 * 4;
        iVar13 = iVar1 - ((uVar3 << 0x15) >> 0x10);
        iVar8 = iStack_58;
        if (iVar13 < 1) {
          for (; 0 < iVar8; iVar8 = (iVar8 + -1) * 0x10000 >> 0x10) {
            FUN_0203b854(iVar12,iVar6,uVar2);
            iVar12 = iVar12 + iVar11 * 0x20;
            iVar6 = iVar6 + iVar1;
          }
        }
        else if (0 < iStack_58) {
          iVar10 = iVar6 + ((int)uVar2 >> 2) * 4;
          do {
            FUN_0203b854(iVar12,iVar6,uVar2);
            FUN_0203b808(0,iVar10,iVar13);
            iVar8 = (iVar8 + -1) * 0x10000 >> 0x10;
            iVar12 = iVar12 + iVar11 * 0x20;
            iVar10 = iVar10 + iVar1;
            iVar6 = iVar6 + iVar1;
          } while (0 < iVar8);
        }
        iVar12 = (((int)(u32)*(u16 *)(DAT_0202085c + iVar4 * 2) >> 3) - iStack_58) * 0x10000 >>
                 0x10;
        if (0 < iVar12) {
          do {
            FUN_0203b808(0,iVar6,iVar1);
            iVar12 = (iVar12 + -1) * 0x10000 >> 0x10;
            iVar6 = iVar6 + uVar5 * 4;
          } while (0 < iVar12);
        }
        local_68 = local_68 + 2;
        iStack_4c = iStack_4c + 0x100;
        iStack_60 = (iStack_60 + 8) * 0x10000 >> 0x10;
      } while (iStack_60 < iVar11);
    }
    local_64 = local_64 + iVar11 * 0x100;
    local_5c = (local_5c + 8) * 0x10000 >> 0x10;
  } while (local_5c < iVar7);
  return;
}

// FUN_02020860 @ 0x02020860 (304 bytes)
void FUN_02020860(int param_1)

{
  u32 uVar1;
  int iVar2;
  int iVar3;
  
  *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) & 0xfffffff7;
  iVar3 = *(int *)(param_1 + 0xf10);
  iVar2 = *(int *)(iVar3 + 4);
  while (iVar2 != -1) {
    if ((int)(*(u32 *)(iVar3 + 0x68) << 0x1f) < 0) {
      *(u32 *)(iVar3 + 0x68) = *(u32 *)(iVar3 + 0x68) & 0xfffffffe;
      uVar1 = (u32)(*(int *)(iVar3 + 8) << 0xe) >> 0x1d;
      if (uVar1 == 0) {
        FUN_0203a04c(*(u32 *)(iVar3 + 0x74),(u32)(*(int *)(iVar3 + 0x6c) << 0xf) >> 0x10);
        FUN_0201ae04(param_1,iVar3);
      }
      else if (uVar1 == 1) {
        FUN_02020560(param_1,iVar3);
      }
    }
    iVar3 = *(int *)(iVar3 + 4);
    iVar2 = *(int *)(iVar3 + 4);
  }
  iVar3 = *(int *)(param_1 + 0xf20);
  if (*(int *)(iVar3 + 4) != -1) {
    do {
      if ((int)(*(u32 *)(iVar3 + 0x68) << 0x1f) < 0) {
        *(u32 *)(iVar3 + 0x68) = *(u32 *)(iVar3 + 0x68) & 0xfffffffe;
        uVar1 = (u32)(*(int *)(iVar3 + 8) << 0xe) >> 0x1d;
        if (uVar1 == 0) {
          FUN_0203a04c(*(u32 *)(iVar3 + 0x74),(u32)(*(int *)(iVar3 + 0x6c) << 0xf) >> 0x10);
          FUN_0201ae04(param_1,iVar3);
        }
        else if (uVar1 == 1) {
          FUN_02020560(param_1,iVar3);
        }
      }
      iVar3 = *(int *)(iVar3 + 4);
    } while (*(int *)(iVar3 + 4) != -1);
    return;
  }
  return;
}

// FUN_02020990 @ 0x02020990 (284 bytes)
void FUN_02020990(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  u32 uVar5;
  int iVar6;
  u32 uVar7;
  BOOL bVar8;
  
  uVar2 = DAT_02020ab4;
  iVar4 = (u32)*(u8 *)(param_2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44);
  iVar3 = *(int *)(iVar4 + 0x10);
  uVar5 = *(u8 *)(iVar4 + 0x30) & 3;
  bVar8 = (*(u8 *)(iVar4 + 0x30) & 3) == 0;
  if (bVar8) {
    uVar5 = *(u32 *)(iVar4 + 0x1c);
  }
  iVar1 = *(int *)(param_2 + 8) << 0x1f;
  if (bVar8) {
    iVar6 = uVar5 + 0x6400000;
  }
  else {
    iVar6 = *(int *)(iVar4 + 0x1c) + 0x6600000;
  }
  if (iVar1 < 0) {
    uVar5 = DAT_02020ab0;
    if (iVar1 < 0) {
      uVar5 = iVar6 + 0xf9a00000U >> ((*(u32 *)DAT_02020aac[1] & 0x300000) >> 0x14) + 5 & 0xffff;
    }
  }
  else {
    uVar5 = iVar6 + 0xf9c00000U >> ((*(u32 *)*DAT_02020aac & 0x300000) >> 0x14) + 5 & 0xffff;
  }
  uVar7 = (u32)*(u8 *)(iVar4 + 0x14);
  if (uVar7 == 0) {
    return;
  }
  do {
    uVar7 = (int)((uVar7 - 1) * 0x10000) >> 0x10;
    *(u32 *)(iVar3 + 4) =
         *(u32 *)(iVar3 + 4) & 0xfffffc00 | uVar5 + (*(u32 *)(iVar3 + 4) & 0x3ff) & uVar2;
    iVar3 = iVar3 + 8;
  } while (0 < (int)uVar7);
  return;
}

// FUN_02020ab8 @ 0x02020ab8 (744 bytes)
u32 FUN_02020ab8(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  u8 bVar3;
  char cVar4;
  int iVar5;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  u32 uVar9;
  u32 uVar10;
  u32 uVar11;
  int iVar12;
  u32 *puVar13;
  int iVar14;
  u32 *puVar15;
  u32 uVar16;
  u32 uVar17;
  int local_44;
  
  iVar5 = *(int *)(param_2 + 8);
  iVar2 = (iVar5 << 0x1f) >> 0x1f;
  iVar1 = -iVar2;
  puVar15 = *(u32 **)(DAT_02020da0 + iVar2 * -4);
  iVar14 = (u32)*(u8 *)(param_2 + 0xc4) * 0x34 + *(int *)(param_1 + 0xf44);
  uVar11 = 0;
  puVar13 = *(u32 **)(iVar14 + 0x10);
  iVar2 = (int)((((u32)(iVar5 << 0x11) >> 0x1b) + 1) * 0x10000) >> 0x10;
  iVar12 = ((*puVar15 & 0x300000) >> 0x14) + 5;
  uVar6 = 0x20 << ((*puVar15 & 0x300000) >> 0x14) & 0xff;
  local_44 = 0;
  *(u8 *)(iVar14 + 0x14) = 0;
  iVar5 = (int)((((u32)(iVar5 << 0x16) >> 0x1b) + 1) * 0x10000) >> 0x10;
  if (iVar2 != 0) {
    cVar4 = '\0';
    do {
      uVar16 = 0;
      if (iVar5 != 0) {
        uVar17 = uVar16;
        do {
          uVar9 = iVar5 - uVar17 & 0xff;
          uVar7 = iVar2 - local_44 & 0xff;
          if (7 < uVar9) {
            uVar9 = 0;
          }
          if (7 < uVar7) {
            uVar7 = 0;
          }
          bVar3 = *(u8 *)(DAT_02020da4 + uVar9 + uVar7 * 8);
          uVar10 = bVar3 & 3;
          uVar7 = (u32)(bVar3 >> 4);
          *puVar13 = *puVar13 & 0xffff3fff | (uVar7 & 3) << 0xe;
          uVar8 = DAT_02020da8 & uVar11 >> iVar12;
          *puVar13 = *puVar13 & 0x3fffffff | uVar10 << 0x1e;
          uVar9 = DAT_02020dac;
          puVar13[1] = puVar13[1] & 0xfffffc00 | uVar8;
          uVar9 = uVar16 & uVar9;
          uVar16 = uVar16 + 0x40;
          *puVar13 = *puVar13 & DAT_02020db0 | uVar9 << 0x10;
          *(char *)puVar13 = cVar4;
          puVar13 = puVar13 + 2;
          uVar11 = ~(uVar6 - 1) &
                   (uVar11 + (u32)*(u16 *)(DAT_02020db4 + (uVar10 + uVar7 * 4) * 2) * 0x20 +
                   uVar6) - 1;
          *(char *)(iVar14 + 0x14) = *(char *)(iVar14 + 0x14) + '\x01';
          uVar17 = (int)((uVar17 + 8) * 0x10000) >> 0x10;
        } while ((int)uVar17 < iVar5);
      }
      cVar4 = cVar4 + '@';
      local_44 = (local_44 + 8) * 0x10000 >> 0x10;
    } while (local_44 < iVar2);
  }
  if (iVar1 == 0) {
    uVar16 = *(u32 *)(param_1 + 0xf4c) & 0xf;
    uVar6 = *(int *)(param_1 + 0x50) << 0xc;
  }
  else {
    uVar16 = (u32)(*(int *)(param_1 + 0xf4c) << 0x18) >> 0x1c;
    uVar6 = *(int *)(param_1 + 0x50) << 10;
  }
  FUN_02022048(*(u32 *)(param_1 + 0x28),iVar1,0xffffffff,*(u32 *)(param_2 + 0x10) & 0x1ff,
               (u32)(*(int *)(param_2 + 0x10) << 0xe) >> 0x17,0,uVar6 >> 0x1e,uVar16,0,iVar14);
  return uVar11 >> iVar12 & 0xffff;
}

// FUN_02020db8 @ 0x02020db8 (292 bytes)
int FUN_02020db8(int param_1)

{
  int iVar1;
  BOOL bVar2;
  u32 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  bVar2 = false;
  iVar4 = (*(u8 *)(param_1 + 0xf4d) - 1) * 0x10000;
  iVar6 = iVar4 >> 0x10;
  if (-1 < iVar6) {
    iVar4 = (short)((u32)iVar4 >> 0x10) * 0x34 + *(int *)(param_1 + 0xf44);
    do {
      if (*(char *)(iVar4 + 0x18) == '\0') {
        bVar2 = true;
        break;
      }
      iVar4 = iVar4 + -0x34;
      iVar6 = (iVar6 + -1) * 0x10000 >> 0x10;
    } while (-1 < iVar6);
  }
  if (!bVar2) {
    return -1;
  }
  iVar4 = FUN_0201d3bc(param_1);
  if (iVar4 != -1) {
    iVar1 = (short)iVar6 * 0x34;
    *(u8 *)(*(int *)(param_1 + 0xf44) + iVar1 + 0x18) = 1;
    *(char *)(*(int *)(param_1 + 0xf44) + iVar1 + 0x19) = (char)iVar4;
    iVar5 = (short)iVar4 * 0xcc + *(int *)(param_1 + 0xf00);
    *(char *)(iVar5 + 0xc4) = (char)iVar6;
    uVar3 = FUN_02020ab8(param_1,iVar5);
    FUN_02008534(*(int *)(param_1 + 0xf44) + iVar1 + 0x1c,*(u32 *)(iVar5 + 8) & 1,1,uVar3);
    FUN_02020990(param_1,iVar5);
    return iVar4;
  }
  return -1;
}

// FUN_02020ee0 @ 0x02020ee0 (60 bytes)
u32 * FUN_02020ee0(u32 *param_1)

{
  *param_1 = DAT_02020f1c;
  FUN_0202a034(param_1[0x3d0]);
  FUN_02020fe8(param_1);
  FUN_0201f360(param_1);
  thunk_FUN_02029ab8(param_1);
  return param_1;
}

// FUN_02020f20 @ 0x02020f20 (52 bytes)
u32 * FUN_02020f20(u32 *param_1)

{
  *param_1 = DAT_02020f54;
  FUN_0202a034(param_1[0x3d0]);
  FUN_02020fe8(param_1);
  FUN_0201f360(param_1);
  return param_1;
}

// FUN_02020f58 @ 0x02020f58 (140 bytes)
u32 * FUN_02020f58(u32 *param_1)

{
  FUN_0201fa50();
  *param_1 = DAT_02020fe4;
  param_1[0x3d1] = 0;
  *(u8 *)((int)param_1 + 0xf4d) = 0;
  param_1[0x3d3] = param_1[0x3d3] & 0xfffffff0 | 0xe;
  param_1[0x3d3] = param_1[0x3d3] & 0xffffff0f | 0xe0;
  return param_1;
}

// FUN_02020fe8 @ 0x02020fe8 (112 bytes)
void FUN_02020fe8(int param_1)

{
  u32 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0xf44);
  if (iVar2 != 0) {
    uVar1 = (u32)*(u8 *)(param_1 + 0xf4d);
    if (uVar1 != 0) {
      do {
        if (*(char *)(iVar2 + 0x18) != '\0') {
          FUN_020204bc(param_1,*(u8 *)(iVar2 + 0x19));
        }
        uVar1 = (int)((uVar1 - 1) * 0x10000) >> 0x10;
        iVar2 = iVar2 + 0x34;
      } while (0 < (int)uVar1);
    }
    thunk_FUN_02029ab8(*(u32 *)(param_1 + 0xf44));
    *(u32 *)(param_1 + 0xf44) = 0;
    *(u8 *)(param_1 + 0xf4d) = 0;
    return;
  }
  return;
}

// FUN_02021058 @ 0x02021058 (204 bytes)
void FUN_02021058(int param_1,u32 param_2,int param_3)

{
  u32 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  uVar1 = FUN_02029bf8(param_3 * 0x94,param_2,DAT_02021124,0);
  *(u32 *)(param_1 + 0xf44) = uVar1;
  FUN_0202cc10(*(u32 *)(param_1 + 0xf44),0,param_3 * 0x94);
  *(int *)(param_1 + 0xf48) = *(int *)(param_1 + 0xf44) + param_3 * 0x34;
  iVar5 = 0;
  if (0 < param_3) {
    iVar3 = 0;
    iVar2 = 0;
    do {
      iVar4 = *(int *)(param_1 + 0xf44) + iVar3;
      FUN_0203b914(iVar4,0,0x18);
      *(short *)(iVar4 + 0xc) = (short)DAT_02021128;
      iVar5 = (iVar5 + 1) * 0x10000 >> 0x10;
      *(int *)(*(int *)(param_1 + 0xf44) + iVar3 + 0x10) = *(int *)(param_1 + 0xf48) + iVar2 * 8;
      iVar3 = iVar3 + 0x34;
      iVar2 = iVar2 + 0xc;
    } while (iVar5 < param_3);
  }
  *(char *)(param_1 + 0xf4d) = (char)param_3;
  return;
}

// FUN_0202112c @ 0x0202112c (140 bytes)
void FUN_0202112c(int param_1,u32 param_2)

{
  u32 uVar1;
  int iVar2;
  u32 uVar3;
  
  iVar2 = FUN_0201d170();
  if (iVar2 == 0) {
    return;
  }
  uVar3 = *(u32 *)((short)param_2 * 0xcc + *(int *)(param_1 + 0xf00) + 8);
  uVar1 = (uVar3 << 0xe) >> 0x1d;
  if (uVar1 != 0) {
    if (uVar1 == 1) {
      FUN_020204bc(param_1,param_2);
      return;
    }
    return;
  }
  FUN_0201d0cc(param_1,uVar3 & 1);
  return;
}

// FUN_020211b8 @ 0x020211b8 (224 bytes)
int FUN_020211b8(int param_1,int *param_2,int param_3)

{
  u32 uVar1;
  int iVar2;
  int unaff_r4;
  
  uVar1 = (u32)(*param_2 << 0xe) >> 0x1d;
  if (uVar1 == 0) {
    unaff_r4 = FUN_0201d3bc();
    if (unaff_r4 == -1) {
      return -1;
    }
    iVar2 = (short)unaff_r4 * 0xcc;
    FUN_0201b418(param_1,*(int *)(param_1 + 0xf00) + iVar2);
    if (*(int *)(*(int *)(param_1 + 0xf00) + iVar2 + 8) << 0x1f < 0) {
      *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 4;
    }
    else {
      *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 2;
    }
  }
  else if ((uVar1 == 1) && (unaff_r4 = ((int (*)(void))FUN_02020db8)(), unaff_r4 == -1)) {
    return -1;
  }
  FUN_0201a7b0(param_1,(short)unaff_r4 * 0xcc + *(int *)(param_1 + 0xf00));
  if (param_3 != -1) {
    unaff_r4 = param_3;
  }
  return unaff_r4;
}

// FUN_02021298 @ 0x02021298 (736 bytes)
void FUN_02021298(int param_1,int param_2,u32 param_3,u16 *param_4)

{
  u32 uVar1;
  u32 uVar2;
  int iVar3;
  u16 *puVar4;
  u32 uVar5;
  u16 *puVar6;
  u32 uVar7;
  u32 *puVar8;
  u8 *pbVar9;
  int iVar10;
  u32 uVar11;
  u32 uVar12;
  u32 uVar13;
  u32 uVar14;
  
  uVar5 = DAT_02021580;
  if (param_2 != 0) {
    param_4 = *(u16 **)(param_1 + 100);
  }
  pbVar9 = *(u8 **)(param_1 + 0x54);
  if (param_2 != 0) {
    if ((*(u8 *)(param_1 + 0x44) & 3) == 0) {
      iVar10 = *(int *)(param_1 + 0x30) + 0x6400000;
    }
    else {
      iVar10 = *(int *)(param_1 + 0x30) + 0x6600000;
    }
    uVar2 = iVar10 + 0xf9a00000U >> ((*(u32 *)DAT_02021578[1] & 0x300000) >> 0x14) + 5;
  }
  else {
    param_4 = *(u16 **)(param_1 + 0x60);
    if ((*(u8 *)(param_1 + 0x2c) & 3) == 0) {
      iVar10 = *(int *)(param_1 + 0x18) + 0x6400000;
    }
    else {
      iVar10 = *(int *)(param_1 + 0x18) + 0x6600000;
    }
    uVar2 = iVar10 + 0xf9c00000U >> ((*(u32 *)*DAT_02021578 & 0x300000) >> 0x14) + 5;
  }
  puVar8 = (u32 *)DAT_02021578[param_2];
  uVar7 = (u32)*(u16 *)(DAT_0202157c + (((u32)*pbVar9 << 0x1a) >> 0x1e) * 2);
  if (uVar7 == (0x20 << ((*puVar8 & 0x300000) >> 0x14) & 0xffffU)) {
    iVar10 = (int)*(short *)(pbVar9 + 0x12);
    if (iVar10 < 1) {
      return;
    }
    do {
      iVar10 = (iVar10 + -1) * 0x10000 >> 0x10;
      param_4[3] = param_4[3] & 0xfc00 |
                   (u16)uVar5 &
                   (u16)(((uVar2 & 0xffff) + (param_4[2] & 0x3ff)) * 0x10000 >> 0x10);
      param_4 = param_4 + 6;
    } while (0 < iVar10);
    return;
  }
  uVar11 = *puVar8;
  uVar12 = 0;
  uVar14 = (0x20 << ((*puVar8 & 0x300000) >> 0x14) & 0xffffU) - 1 & 0xff;
  iVar10 = 0;
  uVar13 = 0xffffffff;
  if (*(short *)(pbVar9 + 0x12) == 0) {
    return;
  }
  do {
    uVar1 = param_4[2] & 0x3ff;
    puVar6 = param_4;
    iVar3 = iVar10;
    if ((int)uVar13 < (int)uVar1) {
      param_4[3] = param_4[3] & 0xfc00 |
                   (u16)DAT_02021580 &
                   (u16)(((uVar2 & 0xffff) + (uVar12 >> ((uVar11 & 0x300000) >> 0x14) + 5)) *
                            0x10000 >> 0x10);
      uVar12 = ~uVar14 & (uVar7 & 0xff) *
                         (u32)*(u16 *)
                                (DAT_02021584 +
                                ((((u32)*param_4 << 0x10) >> 0x1e) << 2 |
                                ((u32)param_4[1] << 0x10) >> 0x1e) * 2) + uVar12 + uVar14;
      uVar13 = uVar1;
    }
    else {
      do {
        puVar4 = puVar6;
        iVar3 = (iVar3 + -1) * 0x10000 >> 0x10;
        if (iVar3 < 0) goto LAB_02021550;
        puVar6 = puVar4 + -6;
      } while ((puVar4[-4] & 0x3ff) != uVar1);
      param_4[3] = param_4[3] & 0xfc00 |
                   (u16)DAT_02021580 & (u16)(((u32)puVar4[-3] << 0x16) >> 0x16);
    }
LAB_02021550:
    iVar10 = (iVar10 + 1) * 0x10000 >> 0x10;
    param_4 = param_4 + 6;
    if ((int)(u32)*(u16 *)(pbVar9 + 0x12) <= iVar10) {
      return;
    }
  } while( true );
}

// FUN_02021588 @ 0x02021588 (348 bytes)
void FUN_02021588(u32 param_1,int param_2,int *param_3,int param_4,u32 param_5,
                 u8 *param_6,u16 *param_7)

{
  u16 uVar1;
  u32 uVar2;
  u32 uVar3;
  int iVar4;
  u32 uVar5;
  int iVar6;
  
  if ((*(u8 *)(param_3 + 5) & 3) == 0) {
    iVar6 = *param_3 + 0x6400000;
  }
  else {
    iVar6 = *param_3 + 0x6600000;
  }
  uVar2 = 0x20 << ((**(u32 **)(DAT_020216e4 + param_2 * 4) & 0x300000) >> 0x14) & 0xff;
  if (uVar2 == *(u16 *)(DAT_020216e8 + (((u32)*param_6 << 0x1a) >> 0x1e) * 2)) {
    FUN_0203b854(param_4,iVar6,param_5);
    return;
  }
  uVar5 = (u32)*(u16 *)(param_6 + 0x12);
  uVar3 = 0xffffffff;
  uVar1 = *(u16 *)(DAT_020216e8 + ((int)((u32)param_6[1] << 0x1b) >> 0x1f) * -2);
  if (uVar5 == 0) {
    return;
  }
  uVar2 = uVar2 - 1 & 0xff;
  do {
    if ((int)uVar3 < (int)(param_7[2] & 0x3ff)) {
      iVar4 = (u32)uVar1 *
              (u32)*(u16 *)
                     (DAT_020216ec +
                     ((((u32)*param_7 << 0x10) >> 0x1e) << 2 | ((u32)param_7[1] << 0x10) >> 0x1e)
                     * 2);
      FUN_0203b854(param_4,iVar6,iVar4);
      param_4 = param_4 + iVar4;
      iVar6 = iVar6 + (~uVar2 & iVar4 + uVar2);
    }
    uVar5 = uVar5 - 1;
    uVar3 = param_7[2] & 0x3ff;
    param_7 = param_7 + 6;
  } while (0 < (int)uVar5);
  return;
}

// FUN_020216f0 @ 0x020216f0 (988 bytes)
void FUN_020216f0(int param_1,int param_2)

{
  u16 uVar1;
  u16 uVar2;
  char cVar3;
  u16 uVar4;
  u32 uVar5;
  u32 uVar6;
  u32 *puVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  u32 *puVar11;
  u8 *pbVar12;
  int iVar13;
  u32 uStack_30;
  u16 uStack_2a;
  
  if (-1 < *(int *)(param_2 + 8) << 1) {
    iVar9 = *(int *)(param_1 + 0x68) + (u32)*(u8 *)(param_2 + 0x13) * 8;
    uVar6 = (u32)*(u16 *)(*(int *)(param_1 + 0x68) + (u32)*(u8 *)(param_2 + 0x13) * 8);
    if (*(int *)(param_2 + 8) << 0x1f < 0) {
      iVar13 = *(int *)(param_1 + 0x54);
      iVar8 = iVar13 + 0x14 + (u32)*(u16 *)(iVar13 + 0xc) * 4 +
              (u32)*(u16 *)(iVar13 + 0xe) * 8;
      puVar7 = (u32 *)((u32)*(u16 *)(iVar8 + uVar6 * 4) * 0xc + *(int *)(param_1 + 100));
      iVar10 = *(int *)(param_1 + 0x4c);
      uStack_30 = *puVar7;
      uStack_2a = (u16)(puVar7[1] >> 0x10);
      pbVar12 = (u8 *)(param_1 + 0x15);
    }
    else {
      iVar13 = *(int *)(param_1 + 0x54);
      iVar8 = iVar13 + 0x14 + (u32)*(u16 *)(iVar13 + 0xc) * 4 +
              (u32)*(u16 *)(iVar13 + 0xe) * 8;
      puVar7 = (u32 *)((u32)*(u16 *)(iVar8 + uVar6 * 4) * 0xc + *(int *)(param_1 + 0x60));
      iVar10 = *(int *)(param_1 + 0x48);
      uStack_30 = *puVar7;
      uStack_2a = (u16)(puVar7[1] >> 0x10);
      pbVar12 = (u8 *)(param_1 + 0x14);
    }
    if (*(int *)(param_1 + 0x50) == 0) {
      uVar6 = (u32)*pbVar12;
    }
    else {
      uVar6 = 0x7f - *pbVar12;
    }
    puVar11 = (u32 *)(iVar10 + uVar6 * 8);
    uVar6 = (u32)(*(int *)(param_2 + 8) << 6) >> 0x1e;
    uVar2 = (u16)DAT_02021acc;
    if ((uVar6 & 1) != 0) {
      if ((uStack_30 & 0x1000000) != 0) {
        uStack_30 = CONCAT22(HWORD(uStack_30) & 0xfe00 |
                             uVar2 & (u16)(((HWORD(uStack_30) & 0x1ff ^ DAT_02021acc) + 1) *
                                              0x10000 >> 0x10),(u16)uStack_30);
      }
      uVar1 = HWORD(uStack_30) & 0xfe00 |
              uVar2 & (HWORD(uStack_30) & 0x1ff) -
                      *(short *)(DAT_02021ad0 +
                                (((uStack_30 << 0x10) >> 0x1e) << 2 |
                                ((u32)HWORD(uStack_30) << 0x10) >> 0x1e) * 2);
      uStack_30 = CONCAT22(uVar1 & 0xc1ff | (u16)((((u32)uVar1 << 0x12) >> 0x1b ^ 8) << 9),
                           (u16)uStack_30);
    }
    if ((uVar6 & 2) != 0) {
      if ((uStack_30 & 0x80) != 0) {
        uStack_30 = CONCAT31(UBYTES123(uStack_30),((u8)uStack_30 ^ 0xff) + 1);
      }
      uVar6 = uStack_30;
      uStack_30 = CONCAT31(UBYTES123(uStack_30),
                           (u8)uStack_30 -
                           (char)*(u16 *)
                                  (DAT_02021ad4 +
                                  (((uStack_30 << 0x10) >> 0x1e) << 2 | uStack_30 >> 0x1e) * 2));
      uVar5 = uStack_30;
      uStack_30 = (uStack_30 & 0xFFFFu) | ((u32)((u16)((uVar6) >> ((2) * 8))) << 16);
      uStack_30 = (uStack_30 & 0xFFFF0000u) | ((u16)uVar5);
      uStack_30 = CONCAT22(HWORD(uStack_30) & 0xc1ff |
                           (u16)((((u32)HWORD(uStack_30) << 0x12) >> 0x1b ^ 0x10) << 9),
                           (u16)uStack_30);
    }
    uVar4 = (u16)uStack_30;
    cVar3 = (u8)uStack_30;
    if ((int)((u32)*(u16 *)(iVar9 + 2) << 0x16) < 0) {
      iVar9 = (u32)*(u16 *)(iVar9 + 4) * 0xc +
              (u32)*(u16 *)(iVar13 + 0x12) * 0xc + iVar8 + (u32)*(u16 *)(iVar13 + 0x10) * 4;
      uStack_30 = CONCAT22(HWORD(uStack_30) & 0xfe00 |
                           uVar2 & (u16)(((int)*(short *)(iVar9 + 6) +
                                             ((u32)(*(int *)(param_2 + 8) << 0x15) >> 0x17) +
                                            (HWORD(uStack_30) & 0x1ff)) * 0x10000 >> 0x10),uVar4);
      uStack_30 = CONCAT31(UBYTES123(uStack_30),
                           cVar3 + (char)*(u16 *)(iVar9 + 8) +
                                   (char)((u32)(*(int *)(param_2 + 8) << 0xc) >> 0x17));
    }
    else {
      uStack_30 = CONCAT22(HWORD(uStack_30) & 0xfe00 |
                           uVar2 & (u16)(((HWORD(uStack_30) & 0x1ff) +
                                            ((u32)(*(int *)(param_2 + 8) << 0x15) >> 0x17)) *
                                            0x10000 >> 0x10),uVar4);
      uStack_30 = CONCAT31(UBYTES123(uStack_30),
                           cVar3 + (char)((u32)(*(int *)(param_2 + 8) << 0xc) >> 0x17));
    }
    uVar2 = (u16)DAT_02021ad8;
    iVar9 = *(int *)(param_2 + 8);
    iVar8 = *(int *)(param_2 + 8);
    uStack_30 = CONCAT22(HWORD(uStack_30),
                         (u16)uStack_30 & 0xdfff |
                         (u16)(((u32)(*(int *)(param_2 + 8) << 2) >> 0x1f) << 0xd));
    *puVar11 = uStack_30;
    *(u16 *)(puVar11 + 1) =
         uVar2 & (u16)(((u32)uStack_2a << 0x16) >> 0x16) |
         (u16)(((u32)(iVar9 << 3) >> 0x1e) << 10) |
         (u16)(((u32)(iVar8 << 8) >> 0x1c) << 0xc);
    *pbVar12 = *pbVar12 + 1;
    return;
  }
  return;
}

// FUN_02021adc @ 0x02021adc (336 bytes)
void FUN_02021adc(int param_1,int param_2)

{
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  int unaff_r4;
  u32 *puVar4;
  u8 *unaff_r5;
  u32 uVar5;
  u32 *puVar6;
  BOOL bVar7;
  u32 uStack_28;
  u16 uStack_24;
  
  uVar2 = DAT_02021c30;
  uVar1 = DAT_02021c2c;
  if (*(int *)(param_2 + 8) << 1 < 0) {
    return;
  }
  bVar7 = -1 < *(int *)(param_2 + 8) << 0x1f;
  if (bVar7) {
    unaff_r5 = (u8 *)(param_1 + 0x14);
    unaff_r4 = *(int *)(param_1 + 0x48);
  }
  if (!bVar7) {
    unaff_r5 = (u8 *)(param_1 + 0x15);
    unaff_r4 = *(int *)(param_1 + 0x4c);
  }
  bVar7 = *(int *)(param_1 + 0x50) != 0;
  uVar3 = 0;
  if (bVar7) {
    uVar3 = (u32)*unaff_r5;
  }
  uVar5 = (u32)*(u8 *)(param_2 + 0x14);
  puVar6 = *(u32 **)(param_2 + 0x10);
  if (bVar7) {
    uVar3 = 0x7f - uVar3;
  }
  else {
    uVar3 = (u32)*unaff_r5;
  }
  puVar4 = (u32 *)(unaff_r4 + uVar3 * 8);
  if (uVar5 == 0) {
    return;
  }
  do {
    uVar3 = *puVar6 & uVar1;
    uStack_28 = (uStack_28 & 0xFFFFFF00u) | ((u8)(char)uVar3);
    uStack_28 = CONCAT31((u32)(uVar3 >> 8) |
                         (u32)(((((*puVar6 << 7) >> 0x17) +
                                   ((u32)(*(int *)(param_2 + 8) << 0x15) >> 0x17) & uVar2) << 0x10)
                                >> 8),
                         (char)uStack_28 + (char)((u32)(*(int *)(param_2 + 8) << 0xc) >> 0x17));
    uStack_24 = (u16)puVar6[1] & 0x3ff |
                (u16)(((u32)(*(int *)(param_2 + 8) << 3) >> 0x1e) << 10) |
                (u16)(((u32)(*(int *)(param_2 + 8) << 8) >> 0x1c) << 0xc);
    *puVar4 = uStack_28;
    *(u16 *)(puVar4 + 1) = uStack_24;
    bVar7 = *(int *)(param_1 + 0x50) != 0;
    if (bVar7) {
      puVar4 = puVar4 + -2;
    }
    uVar5 = (int)((uVar5 - 1) * 0x10000) >> 0x10;
    if (!bVar7) {
      puVar4 = puVar4 + 2;
    }
    *unaff_r5 = *unaff_r5 + 1;
    puVar6 = puVar6 + 2;
  } while (0 < (int)uVar5);
  return;
}

// FUN_02021df0 @ 0x02021df0 (356 bytes)
u32 * FUN_02021df0(u32 *param_1)

{
  u32 uVar1;
  int *piVar2;
  u32 *puVar3;
  u32 uVar4;
  int iVar5;
  int iVar6;
  
  *param_1 = DAT_02021f54;
  uVar4 = DAT_02021f58;
  param_1[5] = param_1[5] & 0xfffeffff;
  uVar1 = DAT_02021f5c;
  param_1[5] = param_1[5] & 0xfffdffff;
  param_1[5] = param_1[5] & 0xfffbffff;
  param_1[5] = param_1[5] & 0xfff7ffff;
  param_1[0x12] = uVar4;
  param_1[0x13] = uVar1;
  param_1[0x14] = 1;
  FUN_0202cc10(param_1 + 6,0,0x18);
  FUN_0202cc10(param_1 + 0xc,0,0x18);
  param_1[1] = DAT_02021f60;
  param_1[2] = param_1 + 3;
  param_1[3] = param_1 + 1;
  piVar2 = DAT_02021f64;
  param_1[4] = 0xffffffff;
  puVar3 = DAT_02021f6c;
  iVar6 = *piVar2;
  param_1[0x16] = DAT_02021f68;
  uVar4 = DAT_02021f70;
  param_1[0x17] = *puVar3;
  uVar4 = FUN_02029bf8(iVar6,0,uVar4,0);
  param_1[0x15] = uVar4;
  FUN_0203b7f0(DAT_02021f74,param_1[0x15],iVar6);
  iVar5 = param_1[0x15];
  iVar6 = iVar6 - ((u32)*(u16 *)(iVar5 + 0xc) * 4 + 0x14 + (u32)*(u16 *)(iVar5 + 0xe) * 8 +
                  (u32)*(u16 *)(iVar5 + 0x10) * 4);
  param_1[0x18] =
       iVar5 + 0x14 + (u32)*(u16 *)(iVar5 + 0xc) * 4 + (u32)*(u16 *)(iVar5 + 0xe) * 8 +
       (u32)*(u16 *)(iVar5 + 0x10) * 4;
  uVar4 = FUN_02029bf8(iVar6,0,DAT_02021f78,0);
  param_1[0x19] = uVar4;
  FUN_0203b7f0(param_1[0x18],param_1[0x19],iVar6);
  param_1[0x1a] = param_1[0x15] + 0x14 + (u32)*(u16 *)(param_1[0x15] + 0xc) * 4;
  return param_1;
}

// FUN_02021f7c @ 0x02021f7c (88 bytes)
void FUN_02021f7c(int param_1)

{
  FUN_02021588(param_1,1,param_1 + 0x30,*(u32 *)(param_1 + 0x58),
               *(u32 *)(param_1 + 0x5c),*(u32 *)(param_1 + 0x54),
               *(u32 *)(param_1 + 100));
  ((void (*)(int, int))FUN_02021298)(param_1,1);
  *(u32 *)(param_1 + 0x14) = *(u32 *)(param_1 + 0x14) | 0x80000;
  return;
}

// FUN_02021fd4 @ 0x02021fd4 (88 bytes)
void FUN_02021fd4(int param_1)

{
  FUN_02021588(param_1,0,param_1 + 0x18,*(u32 *)(param_1 + 0x58),
               *(u32 *)(param_1 + 0x5c),*(u32 *)(param_1 + 0x54),
               *(u32 *)(param_1 + 0x60));
  ((void (*)(int, int))FUN_02021298)(param_1,0);
  *(u32 *)(param_1 + 0x14) = *(u32 *)(param_1 + 0x14) | 0x40000;
  return;
}

// FUN_0202202c @ 0x0202202c (28 bytes)
void FUN_0202202c(u32 param_1)

{
  ((void (*)(void))FUN_02021fd4)();
  FUN_02021f7c(param_1);
  return;
}

// FUN_02022770 @ 0x02022770 (12 bytes)
void FUN_02022770(u16 *param_1,u16 *param_2)

{
  *param_1 = *param_2;
  return;
}

// FUN_0202296c @ 0x0202296c (684 bytes)
void FUN_0202296c(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  char *pcVar3;
  char *pcVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  
  ((void (*)(void))FUN_020231e8)();
  FUN_020231e8(param_1 + 0x30,param_2 + 8);
  uVar7 = *(u32 *)(param_1 + 0x60);
  uVar6 = *(u32 *)(param_1 + 100);
  pcVar3 = (char *)(param_2 + 0x10);
  uVar5 = uVar7 >> 0xf;
  uVar2 = uVar6 >> 0xf;
  iVar1 = 4;
  pcVar4 = pcVar3;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    *pcVar4 = ((u8)uVar7 & 1) + ((u8)uVar5 & 2);
    pcVar4[0x14] = (char)((uVar7 & 2) + (uVar5 & 4) >> 1);
    pcVar4[0x28] = (char)((uVar7 & 4) + (uVar5 & 8) >> 2);
    pcVar4[0x3c] = (char)((uVar7 & 8) + (uVar5 & 0x10) >> 3);
    pcVar4[0x50] = ((u8)uVar6 & 1) + ((u8)uVar2 & 2);
    pcVar4[100] = (char)((uVar6 & 2) + (uVar2 & 4) >> 1);
    pcVar4[0x78] = (char)((uVar6 & 4) + (uVar2 & 8) >> 2);
    pcVar4[0x8c] = (char)((uVar6 & 8) + (uVar2 & 0x10) >> 3);
    uVar7 = uVar7 >> 4;
    uVar5 = uVar5 >> 4;
    uVar6 = uVar6 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar4 = pcVar4 + 1;
  } while (0 < iVar1);
  uVar7 = *(u32 *)(param_1 + 0x68);
  uVar6 = *(u32 *)(param_1 + 0x6c);
  uVar5 = uVar7 >> 0xf;
  uVar2 = uVar6 >> 0xf;
  iVar1 = 4;
  pcVar4 = pcVar3;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    pcVar4[0xa0] = ((u8)uVar7 & 1) + ((u8)uVar5 & 2);
    pcVar4[0xb4] = (char)((uVar7 & 2) + (uVar5 & 4) >> 1);
    pcVar4[200] = (char)((uVar7 & 4) + (uVar5 & 8) >> 2);
    pcVar4[0xdc] = (char)((uVar7 & 8) + (uVar5 & 0x10) >> 3);
    pcVar4[0xf0] = ((u8)uVar6 & 1) + ((u8)uVar2 & 2);
    pcVar4[0x104] = (char)((uVar6 & 2) + (uVar2 & 4) >> 1);
    pcVar4[0x118] = (char)((uVar6 & 4) + (uVar2 & 8) >> 2);
    pcVar4[300] = (char)((uVar6 & 8) + (uVar2 & 0x10) >> 3);
    uVar7 = uVar7 >> 4;
    uVar5 = uVar5 >> 4;
    uVar6 = uVar6 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar4 = pcVar4 + 1;
  } while (0 < iVar1);
  uVar7 = *(u32 *)(param_1 + 0x70);
  uVar6 = *(u32 *)(param_1 + 0x74);
  uVar5 = uVar7 >> 0xf;
  uVar2 = uVar6 >> 0xf;
  iVar1 = 4;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    pcVar3[0x140] = ((u8)uVar7 & 1) + ((u8)uVar5 & 2);
    pcVar3[0x154] = (char)((uVar7 & 2) + (uVar5 & 4) >> 1);
    pcVar3[0x168] = (char)((uVar7 & 4) + (uVar5 & 8) >> 2);
    pcVar3[0x17c] = (char)((uVar7 & 8) + (uVar5 & 0x10) >> 3);
    pcVar3[400] = ((u8)uVar6 & 1) + ((u8)uVar2 & 2);
    pcVar3[0x1a4] = (char)((uVar6 & 2) + (uVar2 & 4) >> 1);
    pcVar3[0x1b8] = (char)((uVar6 & 4) + (uVar2 & 8) >> 2);
    pcVar3[0x1cc] = (char)((uVar6 & 8) + (uVar2 & 0x10) >> 3);
    uVar7 = uVar7 >> 4;
    uVar5 = uVar5 >> 4;
    uVar6 = uVar6 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar3 = pcVar3 + 1;
  } while (0 < iVar1);
  return;
}

// FUN_02022c18 @ 0x02022c18 (564 bytes)
void FUN_02022c18(int param_1,int param_2)

{
  u32 uVar1;
  u32 uVar2;
  int iVar3;
  u32 uVar4;
  char *pcVar5;
  char *pcVar6;
  u32 uVar7;
  u32 uVar8;
  u32 uVar9;
  
  ((void (*)(void))FUN_02023480)();
  FUN_02023480(param_1 + 0x28,param_2 + 8);
  uVar9 = *(u32 *)(param_1 + 0x50);
  uVar8 = *(u32 *)(param_1 + 0x54);
  pcVar5 = (char *)(param_2 + 0x10);
  uVar4 = uVar9 >> 0xf;
  uVar1 = uVar8 >> 0xf;
  iVar3 = 4;
  pcVar6 = pcVar5;
  do {
    iVar3 = (iVar3 + -1) * 0x10000 >> 0x10;
    *pcVar6 = ((u8)uVar9 & 1) + ((u8)uVar4 & 2);
    pcVar6[0x14] = (char)((uVar9 & 2) + (uVar4 & 4) >> 1);
    pcVar6[0x28] = (char)((uVar9 & 4) + (uVar4 & 8) >> 2);
    pcVar6[0x3c] = (char)((uVar9 & 8) + (uVar4 & 0x10) >> 3);
    pcVar6[0x50] = ((u8)uVar8 & 1) + ((u8)uVar1 & 2);
    pcVar6[100] = (char)((uVar8 & 2) + (uVar1 & 4) >> 1);
    pcVar6[0x78] = (char)((uVar8 & 4) + (uVar1 & 8) >> 2);
    pcVar6[0x8c] = (char)((uVar8 & 8) + (uVar1 & 0x10) >> 3);
    uVar9 = uVar9 >> 4;
    uVar4 = uVar4 >> 4;
    uVar8 = uVar8 >> 4;
    uVar1 = uVar1 >> 4;
    pcVar6 = pcVar6 + 1;
  } while (0 < iVar3);
  uVar8 = *(u32 *)(param_1 + 0x58);
  uVar2 = *(u32 *)(param_1 + 0x5c);
  uVar7 = *(u32 *)(param_1 + 0x60);
  uVar4 = uVar8 >> 0xf;
  uVar1 = uVar2 >> 0xf;
  uVar9 = uVar7 >> 0xf;
  iVar3 = 4;
  do {
    iVar3 = (iVar3 + -1) * 0x10000 >> 0x10;
    pcVar5[0xa0] = ((u8)uVar8 & 1) + ((u8)uVar4 & 2);
    pcVar5[0xb4] = (char)((uVar8 & 2) + (uVar4 & 4) >> 1);
    pcVar5[200] = (char)((uVar8 & 4) + (uVar4 & 8) >> 2);
    pcVar5[0xdc] = (char)((uVar8 & 8) + (uVar4 & 0x10) >> 3);
    pcVar5[0xf0] = ((u8)uVar2 & 1) + ((u8)uVar1 & 2);
    pcVar5[0x104] = (char)((uVar2 & 2) + (uVar1 & 4) >> 1);
    pcVar5[0x118] = (char)((uVar2 & 4) + (uVar1 & 8) >> 2);
    pcVar5[300] = (char)((uVar2 & 8) + (uVar1 & 0x10) >> 3);
    pcVar5[0x140] = ((u8)uVar7 & 1) + ((u8)uVar9 & 2);
    pcVar5[0x154] = (char)((uVar7 & 2) + (uVar9 & 4) >> 1);
    pcVar5[0x168] = (char)((uVar7 & 4) + (uVar9 & 8) >> 2);
    pcVar5[0x17c] = (char)((uVar7 & 8) + (uVar9 & 0x10) >> 3);
    uVar8 = uVar8 >> 4;
    uVar4 = uVar4 >> 4;
    uVar2 = uVar2 >> 4;
    uVar1 = uVar1 >> 4;
    uVar7 = uVar7 >> 4;
    uVar9 = uVar9 >> 4;
    pcVar5 = pcVar5 + 1;
  } while (0 < iVar3);
  return;
}

// FUN_02022e4c @ 0x02022e4c (44 bytes)
void FUN_02022e4c(int param_1,int param_2)

{
  ((void (*)(void))FUN_020231e8)();
  FUN_020231e8(param_1 + 0x30,param_2 + 8);
  return;
}

// FUN_02022e78 @ 0x02022e78 (44 bytes)
void FUN_02022e78(int param_1,int param_2)

{
  ((void (*)(void))FUN_02023480)();
  FUN_02023480(param_1 + 0x28,param_2 + 8);
  return;
}

// FUN_02022ea4 @ 0x02022ea4 (44 bytes)
void FUN_02022ea4(int param_1,int param_2)

{
  ((void (*)(void))FUN_02023698)();
  FUN_02023698(param_1 + 0x20,param_2 + 8);
  return;
}

// FUN_02022ed0 @ 0x02022ed0 (460 bytes)
void FUN_02022ed0(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  char *pcVar3;
  char *pcVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  
  ((void (*)(void))FUN_02023698)();
  uVar6 = *(u32 *)(param_1 + 0x20);
  uVar7 = *(u32 *)(param_1 + 0x24);
  pcVar3 = (char *)(param_2 + 8);
  uVar5 = uVar6 >> 0xf;
  uVar2 = uVar7 >> 0xf;
  iVar1 = 4;
  pcVar4 = pcVar3;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    *pcVar4 = ((u8)uVar6 & 1) + ((u8)uVar5 & 2);
    pcVar4[0x14] = (char)((uVar6 & 2) + (uVar5 & 4) >> 1);
    pcVar4[0x28] = (char)((uVar6 & 4) + (uVar5 & 8) >> 2);
    pcVar4[0x3c] = (char)((uVar6 & 8) + (uVar5 & 0x10) >> 3);
    pcVar4[0x50] = ((u8)uVar7 & 1) + ((u8)uVar2 & 2);
    pcVar4[100] = (char)((uVar7 & 2) + (uVar2 & 4) >> 1);
    pcVar4[0x78] = (char)((uVar7 & 4) + (uVar2 & 8) >> 2);
    pcVar4[0x8c] = (char)((uVar7 & 8) + (uVar2 & 0x10) >> 3);
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    uVar7 = uVar7 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar4 = pcVar4 + 1;
  } while (0 < iVar1);
  uVar6 = *(u32 *)(param_1 + 0x28);
  uVar7 = *(u32 *)(param_1 + 0x2c);
  uVar5 = uVar6 >> 0xf;
  uVar2 = uVar7 >> 0xf;
  iVar1 = 4;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    pcVar3[0xa0] = ((u8)uVar6 & 1) + ((u8)uVar5 & 2);
    pcVar3[0xb4] = (char)((uVar6 & 2) + (uVar5 & 4) >> 1);
    pcVar3[200] = (char)((uVar6 & 4) + (uVar5 & 8) >> 2);
    pcVar3[0xdc] = (char)((uVar6 & 8) + (uVar5 & 0x10) >> 3);
    pcVar3[0xf0] = ((u8)uVar7 & 1) + ((u8)uVar2 & 2);
    pcVar3[0x104] = (char)((uVar7 & 2) + (uVar2 & 4) >> 1);
    pcVar3[0x118] = (char)((uVar7 & 4) + (uVar2 & 8) >> 2);
    pcVar3[300] = (char)((uVar7 & 8) + (uVar2 & 0x10) >> 3);
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    uVar7 = uVar7 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar3 = pcVar3 + 1;
  } while (0 < iVar1);
  return;
}

// FUN_0202309c @ 0x0202309c (332 bytes)
void FUN_0202309c(int param_1,int param_2)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  char *pcVar5;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  
  ((void (*)(void))FUN_02023854)();
  uVar6 = *(u32 *)(param_1 + 0x18);
  uVar7 = *(u32 *)(param_1 + 0x1c);
  uVar3 = *(u32 *)(param_1 + 0x20);
  pcVar5 = (char *)(param_2 + 8);
  uVar8 = uVar6 >> 0xf;
  uVar4 = uVar7 >> 0xf;
  uVar2 = uVar3 >> 0xf;
  iVar1 = 4;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    *pcVar5 = ((u8)uVar6 & 1) + ((u8)uVar8 & 2);
    pcVar5[0x14] = (char)((uVar6 & 2) + (uVar8 & 4) >> 1);
    pcVar5[0x28] = (char)((uVar6 & 4) + (uVar8 & 8) >> 2);
    pcVar5[0x3c] = (char)((uVar6 & 8) + (uVar8 & 0x10) >> 3);
    pcVar5[0x50] = ((u8)uVar7 & 1) + ((u8)uVar4 & 2);
    pcVar5[100] = (char)((uVar7 & 2) + (uVar4 & 4) >> 1);
    pcVar5[0x78] = (char)((uVar7 & 4) + (uVar4 & 8) >> 2);
    pcVar5[0x8c] = (char)((uVar7 & 8) + (uVar4 & 0x10) >> 3);
    pcVar5[0xa0] = ((u8)uVar3 & 1) + ((u8)uVar2 & 2);
    pcVar5[0xb4] = (char)((uVar3 & 2) + (uVar2 & 4) >> 1);
    pcVar5[200] = (char)((uVar3 & 4) + (uVar2 & 8) >> 2);
    pcVar5[0xdc] = (char)((uVar3 & 8) + (uVar2 & 0x10) >> 3);
    uVar6 = uVar6 >> 4;
    uVar8 = uVar8 >> 4;
    uVar7 = uVar7 >> 4;
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    uVar2 = uVar2 >> 4;
    pcVar5 = pcVar5 + 1;
  } while (0 < iVar1);
  return;
}

// FUN_020231e8 @ 0x020231e8 (664 bytes)
void FUN_020231e8(u32 *param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  
  uVar6 = *param_1;
  uVar5 = param_1[1];
  uVar4 = param_1[2];
  uVar3 = param_1[3];
  iVar2 = 8;
  pcVar1 = param_2;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    *pcVar1 = ((u8)uVar6 & 1) + ((u8)uVar5 & 1) * '\x02';
    pcVar1[0x14] = (char)((uVar6 & 2) + (uVar5 & 2) * 2 >> 1);
    pcVar1[0x28] = (char)((uVar6 & 4) + (uVar5 & 4) * 2 >> 2);
    pcVar1[0x3c] = (char)((uVar6 & 8) + (uVar5 & 8) * 2 >> 3);
    pcVar1[0x50] = ((u8)uVar4 & 1) + ((u8)uVar3 & 1) * '\x02';
    pcVar1[100] = (char)((uVar4 & 2) + (uVar3 & 2) * 2 >> 1);
    pcVar1[0x78] = (char)((uVar4 & 4) + (uVar3 & 4) * 2 >> 2);
    pcVar1[0x8c] = (char)((uVar4 & 8) + (uVar3 & 8) * 2 >> 3);
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    pcVar1 = pcVar1 + 1;
  } while (0 < iVar2);
  uVar6 = param_1[4];
  uVar5 = param_1[5];
  uVar4 = param_1[6];
  uVar3 = param_1[7];
  iVar2 = 8;
  pcVar1 = param_2;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    pcVar1[0xa0] = ((u8)uVar6 & 1) + ((u8)uVar5 & 1) * '\x02';
    pcVar1[0xb4] = (char)((uVar6 & 2) + (uVar5 & 2) * 2 >> 1);
    pcVar1[200] = (char)((uVar6 & 4) + (uVar5 & 4) * 2 >> 2);
    pcVar1[0xdc] = (char)((uVar6 & 8) + (uVar5 & 8) * 2 >> 3);
    pcVar1[0xf0] = ((u8)uVar4 & 1) + ((u8)uVar3 & 1) * '\x02';
    pcVar1[0x104] = (char)((uVar4 & 2) + (uVar3 & 2) * 2 >> 1);
    pcVar1[0x118] = (char)((uVar4 & 4) + (uVar3 & 4) * 2 >> 2);
    pcVar1[300] = (char)((uVar4 & 8) + (uVar3 & 8) * 2 >> 3);
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    pcVar1 = pcVar1 + 1;
  } while (0 < iVar2);
  uVar6 = param_1[8];
  uVar5 = param_1[9];
  uVar4 = param_1[10];
  uVar3 = param_1[0xb];
  iVar2 = 8;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    param_2[0x140] = ((u8)uVar6 & 1) + ((u8)uVar5 & 1) * '\x02';
    param_2[0x154] = (char)((uVar6 & 2) + (uVar5 & 2) * 2 >> 1);
    param_2[0x168] = (char)((uVar6 & 4) + (uVar5 & 4) * 2 >> 2);
    param_2[0x17c] = (char)((uVar6 & 8) + (uVar5 & 8) * 2 >> 3);
    param_2[400] = ((u8)uVar4 & 1) + ((u8)uVar3 & 1) * '\x02';
    param_2[0x1a4] = (char)((uVar4 & 2) + (uVar3 & 2) * 2 >> 1);
    param_2[0x1b8] = (char)((uVar4 & 4) + (uVar3 & 4) * 2 >> 2);
    param_2[0x1cc] = (char)((uVar4 & 8) + (uVar3 & 8) * 2 >> 3);
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    param_2 = param_2 + 1;
  } while (0 < iVar2);
  return;
}

// FUN_02023480 @ 0x02023480 (536 bytes)
void FUN_02023480(u32 *param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  
  uVar8 = *param_1;
  uVar7 = param_1[1];
  uVar6 = param_1[2];
  uVar4 = param_1[3];
  iVar2 = 8;
  pcVar1 = param_2;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    *pcVar1 = ((u8)uVar8 & 1) + ((u8)uVar7 & 1) * '\x02';
    pcVar1[0x14] = (char)((uVar8 & 2) + (uVar7 & 2) * 2 >> 1);
    pcVar1[0x28] = (char)((uVar8 & 4) + (uVar7 & 4) * 2 >> 2);
    pcVar1[0x3c] = (char)((uVar8 & 8) + (uVar7 & 8) * 2 >> 3);
    pcVar1[0x50] = ((u8)uVar6 & 1) + ((u8)uVar4 & 1) * '\x02';
    pcVar1[100] = (char)((uVar6 & 2) + (uVar4 & 2) * 2 >> 1);
    pcVar1[0x78] = (char)((uVar6 & 4) + (uVar4 & 4) * 2 >> 2);
    pcVar1[0x8c] = (char)((uVar6 & 8) + (uVar4 & 8) * 2 >> 3);
    uVar8 = uVar8 >> 4;
    uVar7 = uVar7 >> 4;
    uVar6 = uVar6 >> 4;
    uVar4 = uVar4 >> 4;
    pcVar1 = pcVar1 + 1;
  } while (0 < iVar2);
  uVar8 = param_1[4];
  uVar7 = param_1[5];
  uVar6 = param_1[6];
  uVar4 = param_1[7];
  uVar5 = param_1[8];
  uVar3 = param_1[9];
  iVar2 = 8;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    param_2[0xa0] = ((u8)uVar8 & 1) + ((u8)uVar7 & 1) * '\x02';
    param_2[0xb4] = (char)((uVar8 & 2) + (uVar7 & 2) * 2 >> 1);
    param_2[200] = (char)((uVar8 & 4) + (uVar7 & 4) * 2 >> 2);
    param_2[0xdc] = (char)((uVar8 & 8) + (uVar7 & 8) * 2 >> 3);
    param_2[0xf0] = ((u8)uVar6 & 1) + ((u8)uVar4 & 1) * '\x02';
    param_2[0x104] = (char)((uVar6 & 2) + (uVar4 & 2) * 2 >> 1);
    param_2[0x118] = (char)((uVar6 & 4) + (uVar4 & 4) * 2 >> 2);
    param_2[300] = (char)((uVar6 & 8) + (uVar4 & 8) * 2 >> 3);
    param_2[0x140] = ((u8)uVar5 & 1) + ((u8)uVar3 & 1) * '\x02';
    param_2[0x154] = (char)((uVar5 & 2) + (uVar3 & 2) * 2 >> 1);
    param_2[0x168] = (char)((uVar5 & 4) + (uVar3 & 4) * 2 >> 2);
    param_2[0x17c] = (char)((uVar5 & 8) + (uVar3 & 8) * 2 >> 3);
    uVar8 = uVar8 >> 4;
    uVar7 = uVar7 >> 4;
    uVar6 = uVar6 >> 4;
    uVar4 = uVar4 >> 4;
    uVar5 = uVar5 >> 4;
    uVar3 = uVar3 >> 4;
    param_2 = param_2 + 1;
  } while (0 < iVar2);
  return;
}

// FUN_02023698 @ 0x02023698 (444 bytes)
void FUN_02023698(u32 *param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  
  uVar4 = *param_1;
  uVar3 = param_1[1];
  uVar6 = param_1[2];
  uVar5 = param_1[3];
  iVar2 = 8;
  pcVar1 = param_2;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    *pcVar1 = ((u8)uVar4 & 1) + ((u8)uVar3 & 1) * '\x02';
    pcVar1[0x14] = (char)((uVar4 & 2) + (uVar3 & 2) * 2 >> 1);
    pcVar1[0x28] = (char)((uVar4 & 4) + (uVar3 & 4) * 2 >> 2);
    pcVar1[0x3c] = (char)((uVar4 & 8) + (uVar3 & 8) * 2 >> 3);
    pcVar1[0x50] = ((u8)uVar6 & 1) + ((u8)uVar5 & 1) * '\x02';
    pcVar1[100] = (char)((uVar6 & 2) + (uVar5 & 2) * 2 >> 1);
    pcVar1[0x78] = (char)((uVar6 & 4) + (uVar5 & 4) * 2 >> 2);
    pcVar1[0x8c] = (char)((uVar6 & 8) + (uVar5 & 8) * 2 >> 3);
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    pcVar1 = pcVar1 + 1;
  } while (0 < iVar2);
  uVar4 = param_1[4];
  uVar3 = param_1[5];
  uVar6 = param_1[6];
  uVar5 = param_1[7];
  iVar2 = 8;
  do {
    iVar2 = (iVar2 + -1) * 0x10000 >> 0x10;
    param_2[0xa0] = ((u8)uVar4 & 1) + ((u8)uVar3 & 1) * '\x02';
    param_2[0xb4] = (char)((uVar4 & 2) + (uVar3 & 2) * 2 >> 1);
    param_2[200] = (char)((uVar4 & 4) + (uVar3 & 4) * 2 >> 2);
    param_2[0xdc] = (char)((uVar4 & 8) + (uVar3 & 8) * 2 >> 3);
    param_2[0xf0] = ((u8)uVar6 & 1) + ((u8)uVar5 & 1) * '\x02';
    param_2[0x104] = (char)((uVar6 & 2) + (uVar5 & 2) * 2 >> 1);
    param_2[0x118] = (char)((uVar6 & 4) + (uVar5 & 4) * 2 >> 2);
    param_2[300] = (char)((uVar6 & 8) + (uVar5 & 8) * 2 >> 3);
    uVar4 = uVar4 >> 4;
    uVar3 = uVar3 >> 4;
    uVar6 = uVar6 >> 4;
    uVar5 = uVar5 >> 4;
    param_2 = param_2 + 1;
  } while (0 < iVar2);
  return;
}

// FUN_02023854 @ 0x02023854 (316 bytes)
void FUN_02023854(u32 *param_1,char *param_2)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  
  uVar5 = *param_1;
  uVar4 = param_1[1];
  uVar7 = param_1[2];
  uVar6 = param_1[3];
  uVar3 = param_1[4];
  uVar2 = param_1[5];
  iVar1 = 8;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    *param_2 = ((u8)uVar5 & 1) + ((u8)uVar4 & 1) * '\x02';
    param_2[0x14] = (char)((uVar5 & 2) + (uVar4 & 2) * 2 >> 1);
    param_2[0x28] = (char)((uVar5 & 4) + (uVar4 & 4) * 2 >> 2);
    param_2[0x3c] = (char)((uVar5 & 8) + (uVar4 & 8) * 2 >> 3);
    param_2[0x50] = ((u8)uVar7 & 1) + ((u8)uVar6 & 1) * '\x02';
    param_2[100] = (char)((uVar7 & 2) + (uVar6 & 2) * 2 >> 1);
    param_2[0x78] = (char)((uVar7 & 4) + (uVar6 & 4) * 2 >> 2);
    param_2[0x8c] = (char)((uVar7 & 8) + (uVar6 & 8) * 2 >> 3);
    param_2[0xa0] = ((u8)uVar3 & 1) + ((u8)uVar2 & 1) * '\x02';
    param_2[0xb4] = (char)((uVar3 & 2) + (uVar2 & 2) * 2 >> 1);
    param_2[200] = (char)((uVar3 & 4) + (uVar2 & 4) * 2 >> 2);
    param_2[0xdc] = (char)((uVar3 & 8) + (uVar2 & 8) * 2 >> 3);
    uVar5 = uVar5 >> 4;
    uVar4 = uVar4 >> 4;
    uVar7 = uVar7 >> 4;
    uVar6 = uVar6 >> 4;
    uVar3 = uVar3 >> 4;
    uVar2 = uVar2 >> 4;
    param_2 = param_2 + 1;
  } while (0 < iVar1);
  return;
}

// FUN_02023990 @ 0x02023990 (232 bytes)
void FUN_02023990(u32 *param_1,char *param_2)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  
  uVar3 = *param_1;
  uVar2 = param_1[1];
  uVar5 = param_1[2];
  uVar4 = param_1[3];
  iVar1 = 8;
  do {
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
    *param_2 = ((u8)uVar3 & 1) + ((u8)uVar2 & 1) * '\x02';
    param_2[0x14] = (char)((uVar3 & 2) + (uVar2 & 2) * 2 >> 1);
    param_2[0x28] = (char)((uVar3 & 4) + (uVar2 & 4) * 2 >> 2);
    param_2[0x3c] = (char)((uVar3 & 8) + (uVar2 & 8) * 2 >> 3);
    param_2[0x50] = ((u8)uVar5 & 1) + ((u8)uVar4 & 1) * '\x02';
    param_2[100] = (char)((uVar5 & 2) + (uVar4 & 2) * 2 >> 1);
    param_2[0x78] = (char)((uVar5 & 4) + (uVar4 & 4) * 2 >> 2);
    param_2[0x8c] = (char)((uVar5 & 8) + (uVar4 & 8) * 2 >> 3);
    uVar3 = uVar3 >> 4;
    uVar2 = uVar2 >> 4;
    uVar5 = uVar5 >> 4;
    uVar4 = uVar4 >> 4;
    param_2 = param_2 + 1;
  } while (0 < iVar1);
  return;
}

// FUN_02023afc @ 0x02023afc (3984 bytes)
void FUN_02023afc(u32 param_1,int param_2,u32 param_3,int param_4,int param_5,u16 param_6,
                 u8 param_7,u32 param_8,int param_9)

{
  short sVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  u32 uVar8;
  int iVar9;
  u32 uVar10;
  u32 uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  u8 *pbVar20;
  u32 uVar21;
  u32 *puVar22;
  u32 uVar23;
  u32 uVar24;
  u32 uVar25;
  u32 local_58;
  u32 uStack_54;
  u32 local_50;
  
  iVar9 = DAT_02024aa8;
  uVar8 = DAT_02024aa4;
  uVar23 = DAT_02024aa0;
  iVar12 = DAT_02024a9c;
  uVar21 = param_1 & 7;
  uVar2 = ((u32)param_6 << 0x13) >> 0x10;
  iVar3 = (int)(uVar21 << 0x12) >> 0x10;
  param_5 = param_5 + ((param_2 >> 3) * uVar2 + (param_1 & DAT_02024a98)) * 4;
  uVar11 = (u32)(short)((u16)param_2 & 7);
  uVar10 = param_4 + uVar11 & 0xff;
  local_58 = 0;
  iVar6 = (int)param_8 >> 0xc;
  if ((int)(param_3 + uVar21) < 8) {
    if (uVar10 < uVar11) {
      return;
    }
    do {
      puVar22 = (u32 *)(param_5 + ((int)uVar11 >> 3) * uVar2 * 4 + (uVar11 & 7) * 4);
      pbVar20 = (u8 *)(((int)local_58 >> 0xc) * 0x14 + iVar12);
      local_58 = local_58 + param_9;
      uVar21 = 0;
      switch((int)((param_3 + 1) * 0x10000) >> 0x10) {
      case 0:
        break;
      case 1:
        uVar21 = (u32)*pbVar20;
        break;
      case 2:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10;
        break;
      case 3:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100;
        break;
      case 4:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                 (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000;
        break;
      case 5:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                 (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                 (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000;
        break;
      case 6:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                 (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                 (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                 (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000;
        break;
      case 7:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                 (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                 (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                 (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000 +
                 (u32)pbVar20[(int)(param_8 * 6) >> 0xc] * 0x1000000;
        break;
      case 8:
        uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                 (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                 (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                 (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                 (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000 +
                 (u32)pbVar20[(int)(param_8 * 6) >> 0xc] * 0x1000000 +
                 (u32)pbVar20[(int)(param_8 * 7) >> 0xc] * 0x10000000;
      }
      uVar24 = uVar23 & uVar21 << iVar3;
      *puVar22 = *puVar22 & (uVar24 * 0xf ^ 0xffffffff);
      uVar11 = (int)((uVar11 + 1) * 0x10000) >> 0x10;
      *puVar22 = *puVar22 + uVar24 * param_7 + ((uVar8 & uVar21 << iVar3) >> 1);
    } while ((int)uVar11 <= (int)uVar10);
    return;
  }
  iVar12 = (8 - uVar21) * 0x10000;
  iVar4 = iVar12 >> 0x10;
  iVar12 = (int)(short)((short)param_3 - (short)((u32)iVar12 >> 0x10) & (u16)DAT_02024a98);
  iVar5 = (int)((((param_3 + 1) - iVar4) - iVar12) * 0x10000) >> 0x10;
  local_50 = 0xffffffff;
  sVar1 = (short)(iVar12 >> 3);
  if (param_8 != 0x1000) {
    if (uVar10 < uVar11) {
      return;
    }
    uVar23 = (u32)param_7;
    do {
      puVar22 = (u32 *)(param_5 + ((int)uVar11 >> 3) * uVar2 * 4 + (uVar11 & 7) * 4);
      pbVar20 = (u8 *)(((int)local_58 >> 0xc) * 0x14 + DAT_02024a9c);
      uVar21 = 0;
      local_58 = local_58 + param_9;
      if ((local_58 & 0xfffff000) == local_50) {
        uVar21 = uStack_54 & DAT_02024aa0;
        *puVar22 = *puVar22 & (uVar21 * 0xf ^ 0xffffffff);
        *puVar22 = *puVar22 + uVar21 * uVar23 + ((uStack_54 & uVar8) >> 1);
        for (iVar12 = (int)sVar1; puVar22 = puVar22 + 8, 0 < iVar12;
            iVar12 = (iVar12 + -1) * 0x10000 >> 0x10) {
          uVar21 = *(u32 *)(iVar9 + iVar12 * 4);
          uVar24 = uVar21 & DAT_02024aa0;
          *puVar22 = *puVar22 & (uVar24 * 0xf ^ 0xffffffff);
          *puVar22 = *puVar22 + uVar24 * uVar23 + ((uVar21 & uVar8) >> 1);
        }
        uVar21 = param_3 & DAT_02024aa0;
        *puVar22 = *puVar22 & (uVar21 * 0xf ^ 0xffffffff);
        *puVar22 = *puVar22 + uVar21 * uVar23 + ((param_3 & uVar8) >> 1);
      }
      else {
        uVar24 = uVar21;
        switch(iVar4) {
        case 0:
          break;
        case 1:
          uVar21 = (u32)*pbVar20;
          uVar24 = param_8;
          break;
        case 2:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10;
          uVar24 = param_8 * 2;
          break;
        case 3:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100;
          uVar24 = param_8 * 3;
          break;
        case 4:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                   (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000;
          uVar24 = param_8 * 4;
          break;
        case 5:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                   (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                   (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000;
          uVar24 = param_8 * 5;
          break;
        case 6:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                   (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                   (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                   (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000;
          uVar24 = param_8 * 6;
          break;
        case 7:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                   (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                   (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                   (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000 +
                   (u32)pbVar20[(int)(param_8 * 6) >> 0xc] * 0x1000000;
          uVar24 = param_8 * 7;
          break;
        case 8:
          uVar21 = (u32)*pbVar20 + (u32)pbVar20[iVar6] * 0x10 +
                   (u32)pbVar20[(int)(param_8 * 2) >> 0xc] * 0x100 +
                   (u32)pbVar20[(int)(param_8 * 3) >> 0xc] * 0x1000 +
                   (u32)pbVar20[(int)(param_8 * 4) >> 0xc] * 0x10000 +
                   (u32)pbVar20[(int)(param_8 * 5) >> 0xc] * 0x100000 +
                   (u32)pbVar20[(int)(param_8 * 6) >> 0xc] * 0x1000000 +
                   (u32)pbVar20[(int)(param_8 * 7) >> 0xc] * 0x10000000;
          uVar24 = param_8 * 8;
        }
        uStack_54 = uVar21 << iVar3;
        uVar25 = DAT_02024aa0 & uVar21 << iVar3;
        *puVar22 = *puVar22 & (uVar25 * 0xf ^ 0xffffffff);
        iVar12 = (int)sVar1;
        *puVar22 = *puVar22 + uVar25 * uVar23 + ((uVar8 & uVar21 << iVar3) >> 1);
        while( true ) {
          puVar22 = puVar22 + 8;
          iVar7 = (int)uVar24 >> 0xc;
          if (iVar12 < 1) break;
          iVar13 = uVar24 + param_8;
          iVar14 = iVar13 + param_8;
          iVar15 = iVar14 + param_8;
          iVar16 = iVar15 + param_8;
          iVar17 = iVar16 + param_8;
          iVar18 = iVar17 + param_8;
          iVar19 = iVar18 + param_8;
          uVar24 = iVar19 + param_8;
          *(u32 *)(iVar9 + iVar12 * 4) =
               (u32)pbVar20[iVar7] + (u32)pbVar20[iVar13 >> 0xc] * 0x10 +
               (u32)pbVar20[iVar14 >> 0xc] * 0x100 + (u32)pbVar20[iVar15 >> 0xc] * 0x1000 +
               (u32)pbVar20[iVar16 >> 0xc] * 0x10000 + (u32)pbVar20[iVar17 >> 0xc] * 0x100000 +
               (u32)pbVar20[iVar18 >> 0xc] * 0x1000000 + (u32)pbVar20[iVar19 >> 0xc] * 0x10000000;
          uVar21 = *(u32 *)(iVar9 + iVar12 * 4);
          uVar25 = uVar21 & DAT_02024aa0;
          *puVar22 = *puVar22 & (uVar25 * 0xf ^ 0xffffffff);
          iVar12 = (iVar12 + -1) * 0x10000 >> 0x10;
          *puVar22 = *puVar22 + uVar25 * uVar23 + ((uVar21 & uVar8) >> 1);
        }
        param_3 = 0;
        switch(iVar5) {
        case 0:
          break;
        case 1:
          param_3 = (u32)pbVar20[iVar7];
          break;
        case 2:
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10;
          break;
        case 3:
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[(int)(uVar24 + param_8 + param_8) >> 0xc] * 0x100;
          break;
        case 4:
          iVar12 = uVar24 + param_8 + param_8;
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[iVar12 >> 0xc] * 0x100 +
                    (u32)pbVar20[(int)(iVar12 + param_8) >> 0xc] * 0x1000;
          break;
        case 5:
          iVar13 = uVar24 + param_8 + param_8;
          iVar12 = iVar13 + param_8;
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[iVar13 >> 0xc] * 0x100 + (u32)pbVar20[iVar12 >> 0xc] * 0x1000 +
                    (u32)pbVar20[(int)(iVar12 + param_8) >> 0xc] * 0x10000;
          break;
        case 6:
          iVar14 = uVar24 + param_8 + param_8;
          iVar12 = iVar14 + param_8;
          iVar13 = iVar12 + param_8;
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[iVar14 >> 0xc] * 0x100 + (u32)pbVar20[iVar12 >> 0xc] * 0x1000 +
                    (u32)pbVar20[iVar13 >> 0xc] * 0x10000 +
                    (u32)pbVar20[(int)(iVar13 + param_8) >> 0xc] * 0x100000;
          break;
        case 7:
          iVar15 = uVar24 + param_8 + param_8;
          iVar12 = iVar15 + param_8;
          iVar13 = iVar12 + param_8;
          iVar14 = iVar13 + param_8;
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[iVar15 >> 0xc] * 0x100 + (u32)pbVar20[iVar12 >> 0xc] * 0x1000 +
                    (u32)pbVar20[iVar13 >> 0xc] * 0x10000 + (u32)pbVar20[iVar14 >> 0xc] * 0x100000
                    + (u32)pbVar20[(int)(iVar14 + param_8) >> 0xc] * 0x1000000;
          break;
        case 8:
          iVar16 = uVar24 + param_8 + param_8;
          iVar12 = iVar16 + param_8;
          iVar13 = iVar12 + param_8;
          iVar14 = iVar13 + param_8;
          iVar15 = iVar14 + param_8;
          param_3 = (u32)pbVar20[iVar7] + (u32)pbVar20[(int)(uVar24 + param_8) >> 0xc] * 0x10 +
                    (u32)pbVar20[iVar16 >> 0xc] * 0x100 + (u32)pbVar20[iVar12 >> 0xc] * 0x1000 +
                    (u32)pbVar20[iVar13 >> 0xc] * 0x10000 + (u32)pbVar20[iVar14 >> 0xc] * 0x100000
                    + (u32)pbVar20[iVar15 >> 0xc] * 0x1000000 +
                    (u32)pbVar20[(int)(iVar15 + param_8) >> 0xc] * 0x10000000;
        }
        uVar21 = param_3 & DAT_02024aa0;
        *puVar22 = *puVar22 & (uVar21 * 0xf ^ 0xffffffff);
        *puVar22 = *puVar22 + uVar21 * uVar23 + ((param_3 & uVar8) >> 1);
        local_50 = local_58 & 0xfffff000;
      }
      uVar11 = (int)((uVar11 + 1) * 0x10000) >> 0x10;
    } while ((int)uVar11 <= (int)uVar10);
    return;
  }
  if (uVar10 < uVar11) {
    return;
  }
  uVar21 = (u32)param_7;
  do {
    puVar22 = (u32 *)(param_5 + ((int)uVar11 >> 3) * uVar2 * 4 + (uVar11 & 7) * 4);
    pbVar20 = (u8 *)(((int)local_58 >> 0xc) * 0x14 + DAT_02024a9c);
    local_58 = local_58 + param_9;
    if ((local_58 & 0xfffff000) == local_50) {
      *puVar22 = *puVar22 & ((uStack_54 & uVar23) * 0xf ^ 0xffffffff);
      *puVar22 = *puVar22 + (uStack_54 & uVar23) * uVar21 + ((uStack_54 & uVar8) >> 1);
      for (iVar6 = (int)sVar1; puVar22 = puVar22 + 8, 0 < iVar6;
          iVar6 = (iVar6 + -1) * 0x10000 >> 0x10) {
        uVar24 = *(u32 *)(iVar9 + iVar6 * 4);
        uVar25 = uVar24 & uVar23;
        *puVar22 = *puVar22 & (uVar25 * 0xf ^ 0xffffffff);
        *puVar22 = *puVar22 + uVar25 * uVar21 + ((uVar24 & uVar8) >> 1);
      }
      *puVar22 = *puVar22 & ((param_3 & uVar23) * 0xf ^ 0xffffffff);
      *puVar22 = *puVar22 + (param_3 & uVar23) * uVar21 + ((param_3 & uVar8) >> 1);
    }
    else {
      uVar24 = 0;
      switch(iVar4) {
      case 0:
        break;
      case 1:
        uVar24 = (u32)*pbVar20;
        break;
      case 2:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10;
        break;
      case 3:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100;
        break;
      case 4:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                 (u32)pbVar20[3] * 0x1000;
        break;
      case 5:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                 (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000;
        break;
      case 6:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                 (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                 (u32)pbVar20[5] * 0x100000;
        break;
      case 7:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                 (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                 (u32)pbVar20[5] * 0x100000 + (u32)pbVar20[6] * 0x1000000;
        break;
      case 8:
        uVar24 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                 (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                 (u32)pbVar20[5] * 0x100000 + (u32)pbVar20[6] * 0x1000000 +
                 (u32)pbVar20[7] * 0x10000000;
      }
      uStack_54 = uVar24 << iVar3;
      uVar25 = uVar23 & uVar24 << iVar3;
      *puVar22 = *puVar22 & (uVar25 * 0xf ^ 0xffffffff);
      *puVar22 = *puVar22 + uVar25 * uVar21 + ((uVar8 & uVar24 << iVar3) >> 1);
      pbVar20 = pbVar20 + iVar4;
      for (iVar6 = (int)sVar1; puVar22 = puVar22 + 8, 0 < iVar6;
          iVar6 = (iVar6 + -1) * 0x10000 >> 0x10) {
        *(u32 *)(iVar9 + iVar6 * 4) =
             (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
             (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 + (u32)pbVar20[5] * 0x100000 +
             (u32)pbVar20[6] * 0x1000000 + (u32)pbVar20[7] * 0x10000000;
        uVar25 = *(u32 *)(iVar9 + iVar6 * 4);
        uVar24 = uVar25 & uVar23;
        *puVar22 = *puVar22 & (uVar24 * 0xf ^ 0xffffffff);
        pbVar20 = pbVar20 + 8;
        *puVar22 = *puVar22 + uVar24 * uVar21 + ((uVar25 & uVar8) >> 1);
      }
      param_3 = 0;
      switch(iVar5) {
      case 0:
        break;
      case 1:
        param_3 = (u32)*pbVar20;
        break;
      case 2:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10;
        break;
      case 3:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100;
        break;
      case 4:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                  (u32)pbVar20[3] * 0x1000;
        break;
      case 5:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                  (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000;
        break;
      case 6:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                  (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                  (u32)pbVar20[5] * 0x100000;
        break;
      case 7:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                  (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                  (u32)pbVar20[5] * 0x100000 + (u32)pbVar20[6] * 0x1000000;
        break;
      case 8:
        param_3 = (u32)*pbVar20 + (u32)pbVar20[1] * 0x10 + (u32)pbVar20[2] * 0x100 +
                  (u32)pbVar20[3] * 0x1000 + (u32)pbVar20[4] * 0x10000 +
                  (u32)pbVar20[5] * 0x100000 + (u32)pbVar20[6] * 0x1000000 +
                  (u32)pbVar20[7] * 0x10000000;
      }
      *puVar22 = *puVar22 & ((param_3 & uVar23) * 0xf ^ 0xffffffff);
      *puVar22 = *puVar22 + (param_3 & uVar23) * uVar21 + ((param_3 & uVar8) >> 1);
      local_50 = local_58 & 0xfffff000;
    }
    uVar11 = (int)((uVar11 + 1) * 0x10000) >> 0x10;
  } while ((int)uVar11 <= (int)uVar10);
  return;
}

// FUN_02024aac @ 0x02024aac (1748 bytes)
void FUN_02024aac(u32 param_1,int param_2,int param_3,int param_4,int param_5,u16 param_6,
                 u8 param_7)

{
  int iVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  u32 uVar5;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  u8 *pbVar9;
  u32 uVar10;
  u32 uVar11;
  u32 uVar12;
  u32 uVar13;
  u32 *puVar14;
  u32 uVar15;
  
  uVar6 = DAT_02025190;
  uVar5 = DAT_0202518c;
  uVar10 = param_1 & 7;
  uVar2 = ((u32)param_6 << 0x13) >> 0x10;
  uVar8 = (u32)(short)((u16)param_2 & 7);
  iVar3 = (int)(uVar10 << 0x12) >> 0x10;
  param_5 = param_5 + ((param_2 >> 3) * uVar2 + (param_1 & DAT_02025184)) * 4;
  uVar15 = param_4 + uVar8 & 0xff;
  if ((int)(param_3 + uVar10) < 8) {
    if (uVar8 <= uVar15) {
      pbVar9 = DAT_02025188;
      do {
        uVar10 = 0;
        puVar14 = (u32 *)(param_5 + ((int)uVar8 >> 3) * uVar2 * 4 + (uVar8 & 7) * 4);
        switch((param_3 + 1) * 0x10000 >> 0x10) {
        case 0:
          break;
        case 1:
          uVar10 = (u32)*pbVar9;
          break;
        case 2:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10;
          break;
        case 3:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100;
          break;
        case 4:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                   (u32)pbVar9[3] * 0x1000;
          break;
        case 5:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                   (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000;
          break;
        case 6:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                   (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000
          ;
          break;
        case 7:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                   (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000
                   + (u32)pbVar9[6] * 0x1000000;
          break;
        case 8:
          uVar10 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                   (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000
                   + (u32)pbVar9[6] * 0x1000000 + (u32)pbVar9[7] * 0x10000000;
        }
        uVar11 = uVar5 & uVar10 << iVar3;
        *puVar14 = *puVar14 & (uVar11 * 0xf ^ 0xffffffff);
        uVar8 = (int)((uVar8 + 1) * 0x10000) >> 0x10;
        pbVar9 = pbVar9 + 0x14;
        *puVar14 = *puVar14 + uVar11 * param_7 + ((uVar6 & uVar10 << iVar3) >> 1);
      } while ((int)uVar8 <= (int)uVar15);
      return;
    }
    return;
  }
  iVar1 = (8 - uVar10) * 0x10000;
  uVar11 = iVar1 >> 0x10;
  iVar1 = (int)(short)((short)param_3 - (short)((u32)iVar1 >> 0x10) & (u16)DAT_02025184);
  if (uVar8 <= uVar15) {
    uVar13 = (u32)param_7;
    pbVar9 = DAT_02025188;
    do {
      puVar14 = (u32 *)(param_5 + ((int)uVar8 >> 3) * uVar2 * 4 + (uVar8 & 7) * 4);
      uVar7 = 0;
      if (uVar11 < 9) {
        switch(uVar10) {
        case 0:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                  (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000
                  + (u32)pbVar9[6] * 0x1000000 + (u32)pbVar9[7] * 0x10000000;
          break;
        case 1:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                  (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000
                  + (u32)pbVar9[6] * 0x1000000;
          break;
        case 2:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                  (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000;
          break;
        case 3:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                  (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000;
          break;
        case 4:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                  (u32)pbVar9[3] * 0x1000;
          break;
        case 5:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100;
          break;
        case 6:
          uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10;
          break;
        case 7:
          uVar7 = (u32)*pbVar9;
        }
      }
      pbVar9 = pbVar9 + uVar11;
      uVar12 = uVar5 & uVar7 << iVar3;
      *puVar14 = *puVar14 & (uVar12 * 0xf ^ 0xffffffff);
      *puVar14 = *puVar14 + uVar12 * uVar13 + ((uVar6 & uVar7 << iVar3) >> 1);
      for (iVar4 = iVar1; puVar14 = puVar14 + 8, 0 < iVar4; iVar4 = (iVar4 + -8) * 0x10000 >> 0x10)
      {
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000 +
                (u32)pbVar9[6] * 0x1000000 + (u32)pbVar9[7] * 0x10000000;
        uVar12 = uVar7 & uVar5;
        *puVar14 = *puVar14 & (uVar12 * 0xf ^ 0xffffffff);
        pbVar9 = pbVar9 + 8;
        *puVar14 = *puVar14 + uVar12 * uVar13 + ((uVar7 & uVar6) >> 1);
      }
      uVar7 = 0;
      switch((int)((((param_3 + 1) - uVar11) - iVar1) * 0x10000) >> 0x10) {
      case 0:
        break;
      case 1:
        uVar7 = (u32)*pbVar9;
        break;
      case 2:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10;
        break;
      case 3:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100;
        break;
      case 4:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000;
        break;
      case 5:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000;
        break;
      case 6:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000;
        break;
      case 7:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000 +
                (u32)pbVar9[6] * 0x1000000;
        break;
      case 8:
        uVar7 = (u32)*pbVar9 + (u32)pbVar9[1] * 0x10 + (u32)pbVar9[2] * 0x100 +
                (u32)pbVar9[3] * 0x1000 + (u32)pbVar9[4] * 0x10000 + (u32)pbVar9[5] * 0x100000 +
                (u32)pbVar9[6] * 0x1000000 + (u32)pbVar9[7] * 0x10000000;
      }
      *puVar14 = *puVar14 & ((uVar7 & uVar5) * 0xf ^ 0xffffffff);
      *puVar14 = *puVar14 + (uVar7 & uVar5) * uVar13 + ((uVar7 & uVar6) >> 1);
      uVar8 = (int)((uVar8 + 1) * 0x10000) >> 0x10;
      pbVar9 = pbVar9 + (0x14 - (uVar11 + iVar1) & 0xffff);
    } while ((int)uVar8 <= (int)uVar15);
    return;
  }
  return;
}

// FUN_02025194 @ 0x02025194 (1640 bytes)
int FUN_02025194(int param_1,u16 *param_2,int param_3)

{
  u8 bVar1;
  short sVar2;
  int iVar3;
  u32 uVar4;
  u32 uVar5;
  FuncPtr pcVar6;
  int iVar7;
  char *pcVar8;
  
  bVar1 = *(u8 *)(*(int *)(param_1 + 4) + 1);
  uVar4 = (u32)bVar1;
  iVar7 = 0;
  if (uVar4 == 0) {
    *(char *)(param_1 + 0x28) = *(char *)(param_1 + 0x28) + '\x01';
    uVar4 = DAT_02025800;
    uVar5 = *(u32 *)(param_1 + 8);
    *(u32 *)(param_1 + 8) =
         uVar5 & DAT_020257fc |
         (((u32)(*(int *)(param_1 + 0x20) << 0xf) >> 0x1a) + ((uVar5 << 4) >> 0x1c) +
          ((uVar5 << 0xc) >> 0x16) & DAT_02025800) << 10;
    *(u32 *)(param_1 + 8) =
         *(u32 *)(param_1 + 8) & 0xfffffc00 | uVar4 & *(u32 *)(param_1 + 0x1c) >> 0x16;
    uVar4 = DAT_02025804;
    *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x20) & 0xfffe07ff;
    *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x20) & uVar4 | 0xffc00000;
    if (-1 < *(int *)(param_1 + 0x20) << 10) {
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 2;
      uVar4 = (u32)(*(int *)(param_1 + 0x20) << 0x16) >> 0x1e;
      if (uVar4 == 1) {
        iVar3 = ((int (*)(void))FUN_02025e74)();
        uVar4 = DAT_02025800;
        *(u32 *)(param_1 + 8) =
             *(u32 *)(param_1 + 8) & 0xfffffc00 |
             (((u32)(*(int *)(param_1 + 0xc) << 9) >> 0x19) + 1) * 4 - (iVar3 >> 1) & DAT_02025800;
        *(u32 *)(param_1 + 8) =
             *(u32 *)(param_1 + 8) & 0xfffffc00 |
             (u32)*(u8 *)(param_1 + 0x18) + (*(u32 *)(param_1 + 8) & 0x3ff) & uVar4;
      }
      else if (uVar4 == 2) {
        iVar3 = ((int (*)(void))FUN_02025e74)();
        uVar4 = DAT_02025800;
        *(u32 *)(param_1 + 8) =
             *(u32 *)(param_1 + 8) & 0xfffffc00 |
             (((u32)(*(int *)(param_1 + 0xc) << 9) >> 0x19) + 1) * 8 - iVar3 & DAT_02025800;
        *(u32 *)(param_1 + 8) =
             *(u32 *)(param_1 + 8) & 0xfffffc00 |
             (*(u32 *)(param_1 + 8) & 0x3ff) -
             ((*(u32 *)(param_1 + 0x1c) >> 0x16) - (u32)*(u8 *)(param_1 + 0x18)) & uVar4;
        *(u32 *)(param_1 + 8) =
             *(u32 *)(param_1 + 8) & 0xfffffc00 |
             (u32)*(u8 *)(param_1 + 0x18) + (*(u32 *)(param_1 + 8) & 0x3ff) & uVar4;
      }
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + -2;
    }
    *param_2 = 0xff00;
  }
  else if (uVar4 < 0x20) {
    pcVar8 = (char *)((uVar4 & 0x1f) * 0xc + DAT_02025808);
    pcVar6 = *(FuncPtr *)(pcVar8 + 4);
    if (pcVar6 == (FuncPtr )0x0) {
      iVar7 = (int)*pcVar8;
    }
    else if (*pcVar8 < '\0') {
      iVar7 = (*pcVar6)(*(u32 *)(pcVar8 + 8),param_1);
    }
    else {
      (*pcVar6)(*(u32 *)(pcVar8 + 8),param_1);
      iVar7 = (int)*pcVar8;
    }
    if (param_3 == 0) {
      *param_2 = (u16)(uVar4 & 0x1f) | 0xff00;
      *(int *)(param_2 + 2) = *(int *)(param_1 + 4) + 2;
    }
  }
  else if ((uVar4 < 0x30) && (param_3 == 0)) {
    *(u32 *)(param_1 + 0xc) =
         *(u32 *)(param_1 + 0xc) & 0xfffffff0 |
         (uVar4 & 0xf) + (*(u32 *)(param_1 + 8) >> 0x1c) & 0xf;
    if (0xe < (*(u32 *)(param_1 + 0xc) & 0xf)) {
      *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) & 0xfffffff0 | 0xe;
    }
  }
  else if (uVar4 < 0x40) {
    uVar5 = uVar4 & 0xf;
    if (uVar5 < 4) {
      if ((bVar1 & 3) == 0) {
        *(u16 *)(param_1 + 0x10) = 0x1000;
        *(u16 *)(param_1 + 0x12) = 0x1000;
      }
      else {
        if ((bVar1 & 1) != 0) {
          *(u16 *)(param_1 + 0x12) = 0x800;
        }
        if ((bVar1 & 2) != 0) {
          *(u16 *)(param_1 + 0x10) = 0x800;
        }
      }
    }
    else if (uVar5 < 7) {
      if (-1 < *(int *)(param_1 + 0x20) << 10) {
        if ((*(u32 *)(param_1 + 0x1c) << 0xd) >> 0x1e != (uVar5 - 4 & 0xff)) {
          uVar4 = uVar5 - 4 & 3;
          *(u32 *)(param_1 + 0x1c) = *(u32 *)(param_1 + 0x1c) & 0xfff9ffff | uVar4 << 0x11;
          *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x20) & 0xfffffcff | uVar4 << 8;
          uVar4 = (u32)(*(int *)(param_1 + 0x20) << 0x16) >> 0x1e;
          if (uVar4 == 1) {
            iVar3 = FUN_02025e74(param_1);
            uVar4 = DAT_02025800;
            *(u32 *)(param_1 + 8) =
                 *(u32 *)(param_1 + 8) & 0xfffffc00 |
                 (((u32)(*(int *)(param_1 + 0xc) << 9) >> 0x19) + 1) * 4 - (iVar3 >> 1) &
                 DAT_02025800;
            *(u32 *)(param_1 + 8) =
                 *(u32 *)(param_1 + 8) & 0xfffffc00 |
                 (u32)*(u8 *)(param_1 + 0x18) + (*(u32 *)(param_1 + 8) & 0x3ff) & uVar4;
          }
          else if (uVar4 == 2) {
            iVar3 = FUN_02025e74(param_1);
            uVar4 = DAT_02025800;
            *(u32 *)(param_1 + 8) =
                 *(u32 *)(param_1 + 8) & 0xfffffc00 |
                 (((u32)(*(int *)(param_1 + 0xc) << 9) >> 0x19) + 1) * 8 - iVar3 & DAT_02025800;
            *(u32 *)(param_1 + 8) =
                 *(u32 *)(param_1 + 8) & 0xfffffc00 |
                 (*(u32 *)(param_1 + 8) & 0x3ff) -
                 ((*(u32 *)(param_1 + 0x1c) >> 0x16) - (u32)*(u8 *)(param_1 + 0x18)) & uVar4;
            *(u32 *)(param_1 + 8) =
                 *(u32 *)(param_1 + 8) & 0xfffffc00 |
                 (u32)*(u8 *)(param_1 + 0x18) + (*(u32 *)(param_1 + 8) & 0x3ff) & uVar4;
          }
          sVar2 = (short)DAT_0202580c;
          *param_2 = sVar2 + (u16)((u32)(*(int *)(param_1 + 0x20) << 0x16) >> 0x1e);
          param_2[1] = sVar2 + (u16)((u32)(*(int *)(param_1 + 0x20) << 0x16) >> 0x1e);
        }
      }
    }
    else if (uVar5 < 9) {
      *(u32 *)(param_1 + 0x20) =
           *(u32 *)(param_1 + 0x20) & 0xfffffbff | ((int)(uVar4 & 8) >> 3) << 10;
    }
  }
  else if (uVar4 < 0x50) {
    *(u16 *)(param_1 + 0x1c) = bVar1 & 0xf;
  }
  else if (uVar4 < 0x60) {
    *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) & 0xfffffe0f | (uVar4 & 0xf) << 4;
  }
  else if (uVar4 < 0x80) {
    *(u32 *)(param_1 + 8) =
         *(u32 *)(param_1 + 8) & 0xfffffc00 |
         (uVar4 - 0x5f) + (*(u32 *)(param_1 + 8) & 0x3ff) & DAT_02025800;
  }
  else if (uVar4 < 0xa0) {
    *(u32 *)(param_1 + 8) =
         *(u32 *)(param_1 + 8) & 0xfffffc00 |
         (*(u32 *)(param_1 + 8) & 0x3ff) - (uVar4 - 0x7f) & DAT_02025800;
  }
  else if (uVar4 < 0xc0) {
    *(u32 *)(param_1 + 8) =
         *(u32 *)(param_1 + 8) & DAT_020257fc |
         ((uVar4 - 0x9f) + ((*(u32 *)(param_1 + 8) << 0xc) >> 0x16) & DAT_02025800) << 10;
  }
  else if (uVar4 < 0xe0) {
    *(u32 *)(param_1 + 8) =
         *(u32 *)(param_1 + 8) & DAT_020257fc |
         (((*(u32 *)(param_1 + 8) << 0xc) >> 0x16) - (uVar4 - 0xbf) & DAT_02025800) << 10;
  }
  else if (uVar4 < 0xe8) {
    uVar4 = uVar4 - 0xe0 & 0xf;
    *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 8) & 0xff0fffff | uVar4 << 0x14;
    *(u32 *)(param_1 + 0x20) = *(u32 *)(param_1 + 0x20) & 0xffe1ffff | uVar4 << 0x11;
  }
  else if (uVar4 < 0xf0) {
    if ((uVar4 - 0xe8 & 0xff) == 7) {
      *(u32 *)(param_1 + 0x20) =
           *(u32 *)(param_1 + 0x20) & 0xffe1ffff |
           ((u32)(*(int *)(param_1 + 8) << 8) >> 0x1c) << 0x11;
    }
    else {
      *(u32 *)(param_1 + 0x20) =
           *(u32 *)(param_1 + 0x20) & 0xffe1ffff | (uVar4 - 0xe8 & 0xf) << 0x11;
    }
  }
  return iVar7;
}

// FUN_02025e74 @ 0x02025e74 (172 bytes)
u32 FUN_02025e74(u8 *param_1)

{
  u8 auStack_50 [8];
  u8 auStack_48 [4];
  char *local_44;
  u32 local_40;
  u32 local_28;
  
  if (param_1 < auStack_48) {
    FUN_0202cd68(param_1,auStack_48,0x30);
  }
  else {
    FUN_0202cc94(param_1,auStack_48,0x30);
  }
  local_28 = local_28 | 0x200000;
  while( true ) {
    if (((*local_44 == -1) && (local_44[1] == '\0')) || (*local_44 == '\0')) break;
    FUN_02025810(auStack_50,auStack_48,1,1);
  }
  return (local_40 & 0x3ff) - (*(u32 *)(param_1 + 8) & 0x3ff) & 0xffff;
}

// FUN_02025fb4 @ 0x02025fb4 (168 bytes)
void FUN_02025fb4(u8 *param_1,u8 *param_2)

{
  u8 auStack_50 [8];
  u8 auStack_48 [4];
  char *local_44;
  u8 auStack_20 [12];
  
  if (param_1 < auStack_48) {
    FUN_0202cd68(param_1,auStack_48,0x30);
  }
  else {
    FUN_0202cc94(param_1,auStack_48,0x30);
  }
  if (*local_44 != '\0') {
    do {
      FUN_02025810(auStack_50,auStack_48,1,1);
    } while (*local_44 != '\0');
  }
  if (auStack_20 < param_2) {
    FUN_0202cd68(auStack_20,param_2,8);
    return;
  }
  FUN_0202cc94(auStack_20,param_2,8);
  return;
}

// FUN_02026530 @ 0x02026530 (120 bytes)
void FUN_02026530(u32 param_1)

{
  if ((param_1 & 0x4000) != 0) {
    FUN_02027a68((int)(short)((u16)param_1 & 0xbfff),0xffffffff);
    return;
  }
  if ((param_1 & 0x8000) != 0) {
    FUN_02027798(param_1 & 0xffff7fff & DAT_020265a8);
    return;
  }
  FUN_02027cf4((int)(short)((u16)param_1 & 0xbfff),0,0xffffffff);
  return;
}

// FUN_020265ac @ 0x020265ac (488 bytes)
void FUN_020265ac(u32 param_1,u32 param_2)

{
  u16 *puVar1;
  int iVar2;
  u32 uVar3;
  u16 uVar4;
  
  uVar4 = *DAT_02026794;
  FUN_0203b7c0(DAT_02026798,DAT_0202679c,(u32)uVar4 << 1);
  iVar2 = 0;
  param_1 = param_1 & DAT_020267a0;
  uVar3 = 0;
  if (param_1 < 0x5001) {
    if (param_1 < 0x5000) {
      if (DAT_020267a4 < param_1) {
        if (DAT_020267b0 < param_1) {
          if ((param_1 <= DAT_020267bc) && (DAT_020267c0 <= param_1)) {
            if (param_1 == DAT_020267c0) {
              uVar3 = (u32)*DAT_0202681c;
              iVar2 = DAT_02026820;
            }
            else if (param_1 == DAT_020267c4) {
              uVar3 = (u32)*DAT_02026824;
              iVar2 = DAT_02026828;
            }
            else if (param_1 == DAT_020267bc) {
              uVar3 = (u32)*DAT_020267c8;
              iVar2 = DAT_020267cc;
            }
          }
        }
        else if (param_1 == DAT_020267b0) {
          uVar3 = (u32)*DAT_020267b4;
          iVar2 = DAT_020267b8;
        }
      }
      else if (param_1 < DAT_020267a4) {
        if (param_1 == 0x2000) {
          uVar3 = (u32)*DAT_020267a8;
          iVar2 = DAT_020267ac;
        }
      }
      else {
        uVar3 = (u32)*DAT_02026814;
        iVar2 = DAT_02026818;
      }
    }
    else {
      uVar3 = (u32)*DAT_02026804;
      iVar2 = DAT_02026808;
    }
  }
  else if (DAT_020267d0 < param_1) {
    if (DAT_020267ec < param_1) {
      if (param_1 == DAT_020267f8) {
        uVar3 = (u32)*DAT_020267fc;
        iVar2 = DAT_02026800;
      }
    }
    else if (param_1 == DAT_020267ec) {
      uVar3 = (u32)*DAT_020267f0;
      iVar2 = DAT_020267f4;
    }
  }
  else if (param_1 < DAT_020267d0) {
    if (DAT_020267d4 < param_1) {
      if (param_1 == DAT_020267e0) {
        uVar3 = (u32)*DAT_020267e4;
        iVar2 = DAT_020267e8;
      }
    }
    else if (param_1 == DAT_020267d4) {
      uVar3 = (u32)*DAT_020267d8;
      iVar2 = DAT_020267dc;
    }
  }
  else {
    uVar3 = (u32)*DAT_0202680c;
    iVar2 = DAT_02026810;
  }
  if (iVar2 != 0) {
    FUN_0203b7c0(iVar2,DAT_0202679c + (u32)uVar4 * 2,uVar3 << 1);
    uVar4 = uVar4 + (short)uVar3;
  }
  puVar1 = DAT_0202682c;
  *DAT_0202682c = uVar4;
  FUN_020278fc(puVar1,1,param_2);
  return;
}

// FUN_02026830 @ 0x02026830 (48 bytes)
void FUN_02026830(int param_1,u32 param_2)

{
  FUN_02030790(DAT_02026860,(param_1 + 1) * 0x100,0,DAT_02026864,param_2);
  return;
}

// FUN_020268c8 @ 0x020268c8 (96 bytes)
void FUN_020268c8(int param_1,int param_2,int param_3,u32 param_4)

{
  int iVar1;
  
  if ((*(short *)(param_3 + 0xc) != 0) && (iVar1 = FUN_0203cda8(param_3,param_4), param_1 == iVar1))
  {
    FUN_0203cdf0(param_3,param_4,0);
  }
  FUN_0203bec8(param_1,param_1 + param_2);
  return;
}

