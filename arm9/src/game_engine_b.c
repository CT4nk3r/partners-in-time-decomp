/**
 * Game Engine B — rendering, collision, GX wrappers (0x02026xxx-0x0202axxx)
 * Decompiled from 0x020269f8-0x0202a0ec region (49 functions)
 */
#include "types.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wreturn-mismatch"
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

void thunk_FUN_02029ab8(u32 *param);

/* ----------------------------------------------------------------
 * External data references
 * ---------------------------------------------------------------- */
extern u32 DAT_02026b10;
extern u32 DAT_02026b14;
extern u32 DAT_02026be8;
extern u32 DAT_02026c68;
extern u32 DAT_02026c6c;
extern u32 DAT_02026ca8;
extern u32 DAT_02026cc8;
extern u32 DAT_02026ccc;
extern u32 DAT_02026e10;
extern u32 DAT_02027014;
extern u32 DAT_02027018;
extern u32 DAT_0202701c;
extern u32 DAT_020270c4;
extern u32 DAT_020270c8;
extern u32 DAT_020270cc;
extern u32 DAT_020270d0;
extern u32 DAT_0202728c;
extern u32 DAT_02027290;
extern u32 DAT_02027294;
extern u32 DAT_02027298;
extern u32 DAT_0202729c;
extern u32 DAT_02027368;
extern u32 DAT_0202736c;
extern u32 DAT_02027370;
extern u32 DAT_02027374;
extern u32 DAT_02027378;
extern u32 DAT_0202737c;
extern u32 DAT_020273f8;
extern u32 DAT_020273fc;
extern u32 DAT_02027400;
extern u32 DAT_020274fc;
extern u32 DAT_02027500;
extern u32 DAT_02027504;
extern u32 DAT_02027508;
extern u32 DAT_0202750c;
extern u32 DAT_02027510;
extern u32 DAT_020275d4;
extern u32 DAT_020275d8;
extern u32 DAT_020275dc;
extern u32 DAT_020275e0;
extern u32 DAT_0202764c;
extern u32 DAT_02027650;
extern u32 DAT_0202769c;
extern u32 DAT_0202770c;
extern u32 DAT_02027844;
extern u32 DAT_02027848;
extern u32 DAT_0202784c;
extern u32 DAT_02027850;
extern u32 DAT_020278f4;
extern u32 DAT_020278f8;
extern u32 DAT_02027a58;
extern u32 DAT_02027a5c;
extern u32 DAT_02027a60;
extern u32 DAT_02027a64;
extern u32 DAT_02027b28;
extern u32 DAT_02027b2c;
extern u32 DAT_02027b30;
extern u32 DAT_02027b34;
extern u32 DAT_02027c70;
extern u32 DAT_02027c74;
extern u32 DAT_02027cec;
extern u32 DAT_02027cf0;
extern u32 DAT_02027edc;
extern u32 DAT_02027fe8;
extern u32 DAT_02027fec;
extern u32 DAT_02027ff0;
extern u32 DAT_02028694;
extern u32 DAT_02028698;
extern u32 DAT_0202869c;
extern u32 DAT_020286a0;
extern u32 DAT_020286a4;
extern u32 DAT_020286a8;
extern u32 DAT_020286ac;
extern u32 DAT_020289e4;
extern u32 DAT_020289e8;
extern u32 DAT_020289ec;
extern u32 DAT_020289f0;
extern u32 DAT_02028aac;
extern u32 DAT_02028ab0;
extern u32 DAT_02028b00;
extern u32 DAT_02028b48;
extern u32 DAT_02028b4c;
extern u32 DAT_02028bcc;
extern u32 DAT_02028c1c;
extern u32 DAT_02028cd4;
extern u32 DAT_02028cd8;
extern u32 DAT_02028e98;
extern u32 DAT_02028e9c;
extern u32 DAT_02028ea0;
extern u32 DAT_02028ea4;
extern u32 DAT_020293b0;
extern u32 DAT_020293b4;
extern u32 DAT_020293b8;
extern u32 DAT_020293bc;
extern u32 DAT_020293c0;
extern u32 DAT_020293c4;
extern u32 DAT_02029478;
extern u32 DAT_0202947c;
extern u32 DAT_02029480;
extern u32 DAT_02029484;
extern u32 DAT_02029720;
extern u32 DAT_02029758;
extern u32 DAT_0202975c;
extern u32 DAT_02029760;
extern u32 DAT_02029764;
extern u32 DAT_02029768;
extern u32 DAT_02029838;
extern u32 DAT_0202983c;
extern u32 DAT_02029840;
extern u32 DAT_02029958;
extern u32 DAT_0202995c;
extern u32 DAT_02029960;
extern u32 DAT_02029df0;
extern u32 DAT_02029df4;
extern u32 DAT_02029df8;
extern u32 DAT_02029dfc;
extern u32 DAT_02029e00;
extern u32 DAT_02029e04;
extern u32 DAT_02029e08;
extern u32 DAT_02029e0c;
extern u32 DAT_02029e10;
extern u32 DAT_02029e14;
extern u32 DAT_02029e18;
extern u32 DAT_02029e1c;
extern u32 DAT_02029f84;
extern u32 DAT_02029f88;
extern u32 DAT_02029f8c;
extern u32 DAT_02029ff8;
extern u32 DAT_0202a174;
extern u32 DAT_0202a178;
extern u32 DAT_0202a17c;
extern u32 DAT_0202a180;
extern u32 DAT_0202a184;

/* ================================================================
 * Function implementations
 * ================================================================ */

// FUN_020269f8 @ 0x020269f8 (280 bytes)
u32 FUN_020269f8(void)

{
  int *piVar1;
  u32 *puVar2;
  int iVar3;
  u32 uVar4;
  int *piVar5;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  
  puVar2 = DAT_02026b14;
  piVar1 = DAT_02026b10;
  uVar7 = 0;
  while( TRUE ) {
    iVar6 = piVar1[9];
    uVar8 = *puVar2 - uVar7;
    if ((u32)(piVar1[3] - iVar6) < uVar8) {
      uVar8 = piVar1[3] - iVar6;
    }
    iVar3 = piVar1[1];
    FUN_020308d4(piVar1[2],iVar3 + iVar6,uVar8,iVar6 + piVar1[4]);
    FUN_0203a030(iVar3 + iVar6,uVar8);
    iVar6 = piVar1[9];
    piVar1[9] = iVar6 + uVar8;
    if (iVar6 + uVar8 < (u32)piVar1[3]) {
      return 0;
    }
    piVar1[9] = 0;
    if ((void *)piVar1[5] != (void *)0x0) {
      ((void (*)(int,...))piVar1[5])(piVar1);
    }
    piVar5 = (int *)piVar1[7];
    *piVar1 = *piVar5;
    piVar1[1] = piVar5[1];
    piVar1[2] = piVar5[2];
    piVar1[3] = piVar5[3];
    piVar1[4] = piVar5[4];
    piVar1[5] = piVar5[5];
    piVar1[6] = piVar5[6];
    piVar1[7] = *piVar1;
    if (*piVar1 == -1) break;
    uVar4 = *puVar2;
    if (uVar4 <= uVar8) {
      return 1;
    }
    uVar7 = uVar7 + uVar8;
    if (uVar4 <= uVar7) {
      return 1;
    }
  }
  piVar1[8] = 0;
  return 1;
}


// FUN_02026b18 @ 0x02026b18 (208 bytes)
void FUN_02026b18(void)

{
  u8 uVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  
  iVar2 = FUN_020269f8();
  if (iVar2 == 0) {
    return;
  }
  if (*(int *)(DAT_02026be8 + 0x20) != 0) {
    return;
  }
  iVar2 = *(int *)(DAT_02026be8 + 0x30);
  if (iVar2 == 0) {
    FUN_0203b914(DAT_02026be8,0,0x38);
    FUN_02026cac();
    return;
  }
  if (iVar2 == 1) {
    FUN_020272a0(*(u32 *)(DAT_02026be8 + 0x28),*(u8 *)(DAT_02026be8 + 0x2c));
    FUN_0203b914(DAT_02026be8,0,0x38);
    FUN_02026cac();
    return;
  }
  if (iVar2 == 2) {
    uVar1 = *(u8 *)(DAT_02026be8 + 0x2c);
    uVar4 = *(u32 *)(DAT_02026be8 + 0x28);
    uVar3 = *(u32 *)(DAT_02026be8 + 0x34);
    FUN_0203b914(DAT_02026be8,0,0x38);
    FUN_02026cac();
    FUN_02027024(uVar4,uVar1);
    FUN_02026bec(uVar3 & 0xff,uVar4,uVar1,0);
    return;
  }
  return;
}


// FUN_02026bec @ 0x02026bec (124 bytes)
void FUN_02026bec(u32 param_1,u32 param_2,u32 param_3,u32 param_4)

{
  u32 *puVar1;
  u32 uVar2;
  u32 *puVar3;
  
  puVar1 = DAT_02026c6c;
  puVar3 = (u32 *)(*(volatile u32 *)DAT_02026c68);
  uVar2 = *puVar3;
  (*(volatile u32 *)DAT_02026c6c) = uVar2;
  puVar1[1] = puVar3[1];
  puVar1[2] = puVar3[2];
  puVar1[3] = puVar3[3];
  puVar1[4] = puVar3[4];
  puVar1[5] = puVar3[5];
  puVar1[6] = puVar3[6];
  puVar1[7] = uVar2;
  puVar1[0xc] = param_1;
  puVar1[10] = param_2;
  *(u8 *)(puVar1 + 0xb) = param_3;
  puVar1[9] = 0;
  puVar1[8] = 1;
  puVar1[0xd] = param_4;
  return;
}


// FUN_02026c70 @ 0x02026c70 (56 bytes)
void FUN_02026c70(int *param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  
  param_1[2] = param_3;
  param_1[1] = (int)param_1;
  param_1[3] = param_2;
  param_1[4] = param_4;
  param_1[5] = param_5;
  iVar1 = DAT_02026ca8;
  param_1[6] = param_6;
  **(u32 **)(iVar1 + 4) = param_1;
  *param_1 = iVar1;
  *(int **)(iVar1 + 4) = param_1;
  return;
}


// FUN_02026cac @ 0x02026cac (28 bytes)
void FUN_02026cac(void)

{
  u32 *puVar1;
  u32 *puVar2;
  
  puVar2 = DAT_02026ccc;
  puVar1 = DAT_02026cc8;
  (*(volatile u32 *)DAT_02026ccc) = DAT_02026cc8;
  puVar1[1] = puVar2;
  *puVar1 = 0xffffffff;
  return;
}


// FUN_02026cd0 @ 0x02026cd0 (44 bytes)
u32 FUN_02026cd0(u32 param_1,u32 param_2,u32 param_3,u32 param_4,
                 u32 param_5)

{
  if (param_2 < 0x1c) {
    param_2 = 0x1c;
  }
  FUN_020311a8(param_1,param_2 + 0x20,param_3,param_4,param_5);
  return;
  return 0;
}


// FUN_02026cfc @ 0x02026cfc (68 bytes)
int FUN_02026cfc(int param_1,u32 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_0203ce28();
  iVar2 = param_1 + (*(int *)(param_1 + 0x38) + param_2) * 4;
  if (param_2 < iVar1 - 1U) {
    iVar1 = *(int *)(iVar2 + 0x40);
  }
  else {
    iVar1 = *(int *)(param_1 + 8);
  }
  return iVar1 - *(int *)(iVar2 + 0x3c);
}


// FUN_02026d40 @ 0x02026d40 (208 bytes)
u32 FUN_02026d40(int param_1,int param_2,u32 param_3,u32 param_4,int param_5)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  
  iVar1 = FUN_0203cda8();
  if (iVar1 == 0) {
    uVar2 = FUN_02026cfc(param_1,param_2);
    uVar3 = FUN_02026cd0(param_4,uVar2,DAT_02026e10,param_1,0);
    if (param_5 == 0) {
      FUN_020308d4(param_3,uVar3,uVar2,
                   *(u32 *)(param_1 + (*(int *)(param_1 + 0x38) + param_2) * 4 + 0x3c));
      FUN_0203a030(uVar3,uVar2);
    }
    else {
      FUN_02026c70(uVar3,uVar2,param_3,0,0,0);
    }
    FUN_0203cdf0(param_1,param_2,uVar3);
    return 1;
  }
  return 0;
}


// FUN_02026e14 @ 0x02026e14 (180 bytes)
void FUN_02026e14(u32 param_1,u32 param_2,u32 param_3,u32 param_4,
                 u32 param_5)

{
  int iVar1;
  u32 local_44;
  u32 local_40;
  u32 local_3c;
  u32 local_38;
  char acStack_34 [2];
  u16 uStack_32;
  u16 uStack_30;
  
  FUN_0203d008(&local_3c);
  local_44 = local_3c;
  local_40 = local_38;
  iVar1 = FUN_0203ce30(param_2,acStack_34,&local_44);
  if (iVar1 != 0) {
    do {
      if ((acStack_34[0] == '\x01') && (param_3 == uStack_30)) {
        FUN_02026d40(param_1,uStack_32,param_4,param_5,0);
      }
      iVar1 = FUN_0203ce30(param_2,acStack_34,&local_44);
    } while (iVar1 != 0);
    return;
  }
  return;
}


// FUN_02026ec8 @ 0x02026ec8 (64 bytes)
void FUN_02026ec8(int param_1)

{
  u32 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 4);
  uVar1 = *(u32 *)(param_1 + 0x18);
  FUN_0203b9a8(iVar2 + 0x3c,iVar2 + 0x3c + *(int *)(iVar2 + 0x38) * 4,uVar1);
  FUN_0203b914(iVar2 + 0x3c,0,uVar1);
  return;
}


// FUN_02026f08 @ 0x02026f08 (268 bytes)
int FUN_02026f08(u32 param_1,u32 param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_0203088c();
  if ((iVar2 != 0) && (param_3 == 0)) {
    return iVar2;
  }
  FUN_020308d4(param_1,DAT_02027014,0x3c,0);
  iVar2 = *(int *)(DAT_02027014 + 0x38) * 4;
  iVar1 = *(int *)(DAT_02027014 + 0x38) * 8;
  iVar3 = FUN_020311a8(param_2,iVar1 + 0x5c,DAT_02027018,DAT_0202701c);
  if (param_5 == 0) {
    FUN_020308d4(param_1,iVar3,iVar2 + 0x3c,0);
    FUN_0203b9a8(iVar3 + 0x3c,iVar3 + 0x3c + *(int *)(iVar3 + 0x38) * 4,iVar2);
    FUN_0203b914(iVar3 + 0x3c,0,iVar2);
    FUN_0203a030(iVar3,iVar1 + 0x3c);
  }
  else {
    FUN_02026c70(iVar3,iVar2 + 0x3c,param_1,0,0,0);
  }
  if (param_4 != 0) {
    FUN_02030870(param_1,iVar3);
  }
  return iVar3;
}


// FUN_02027024 @ 0x02027024 (160 bytes)
void FUN_02027024(u32 param_1,int param_2)

{
  u32 uVar1;
  int iVar2;
  u32 *puVar3;
  u32 *puVar4;
  int iVar5;
  int iVar6;
  u32 unaff_r7;
  
  iVar6 = param_2 * 0x18 + DAT_020270c4;
  iVar2 = FUN_02030c64();
  iVar2 = FUN_02030b9c(*(u16 *)(iVar2 + 4));
  uVar1 = DAT_020270cc;
  puVar3 = DAT_020270d0;
  if (param_2 == 0) {
    unaff_r7 = (*(volatile u32 *)DAT_020270c8);
    puVar3 = DAT_020270c8;
  }
  iVar5 = 0;
  if (param_2 != 0) {
    unaff_r7 = *puVar3;
  }
  do {
    if (*(u16 *)(iVar2 + iVar5 * 2 + 4) != uVar1) {
      puVar4 = (u32 *)FUN_02030b38();
      if ((*puVar4 & 0x1000000) != 0) {
        FUN_02026e14(*(u32 *)(iVar6 + iVar5 * 4 + 8),*(u32 *)(iVar6 + 4),iVar5,
                     *puVar4 & 0xffff,unaff_r7);
      }
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 4);
  return;
}


// FUN_020270d4 @ 0x020270d4 (440 bytes)
u32 FUN_020270d4(u32 param_1,u32 param_2,u32 *param_3)

{
  u32 uVar1;
  u32 *puVar2;
  u32 uVar3;
  u32 uVar4;
  u32 *puVar5;
  int iVar6;
  int iVar7;
  u32 uVar8;
  
  uVar8 = 0;
  *param_3 = 0;
  param_3[1] = 0;
  iVar7 = 0;
  do {
    iVar6 = iVar7 + 1;
    param_3[iVar7 + 2] = 0;
    iVar7 = iVar6;
  } while (iVar6 < 4);
  puVar2 = (u32 *)FUN_02030c64(param_2);
  uVar1 = *puVar2 & 0xffff;
  uVar3 = FUN_02030958(uVar1);
  uVar4 = FUN_02026cd0(param_1,uVar3,DAT_0202728c,DAT_02027290,uVar1);
  FUN_02026c70(uVar4,uVar3,uVar1,0,0,0);
  *param_3 = uVar4;
  puVar2 = (u32 *)FUN_02030b9c((short)puVar2[1]);
  uVar1 = *puVar2 & 0xffff;
  uVar3 = FUN_02030958(uVar1);
  uVar4 = FUN_02026cd0(param_1,uVar3,DAT_02027294,DAT_02027290,uVar1);
  FUN_02026c70(uVar4,uVar3,uVar1,0,0,0);
  iVar7 = 0;
  param_3[1] = uVar4;
  do {
    if (*(u16 *)((int)puVar2 + iVar7 * 2 + 4) != DAT_02027298) {
      puVar5 = (u32 *)FUN_02030b38();
      uVar1 = *puVar5 & 0xffff;
      if ((*puVar5 & 0x1000000) == 0) {
        uVar3 = FUN_02030958(uVar1);
        uVar4 = FUN_02026cd0(param_1,uVar3,DAT_0202729c,DAT_02027290,uVar1);
        FUN_02026c70(uVar4,uVar3,uVar1,0,0,0);
        param_3[iVar7 + 2] = uVar4;
      }
      else {
        uVar3 = FUN_02026f08(uVar1,param_1,1,0,1);
        uVar8 = 1;
        param_3[iVar7 + 2] = uVar3;
      }
    }
    iVar7 = iVar7 + 1;
  } while (iVar7 < 4);
  return uVar8;
}


// FUN_020272a0 @ 0x020272a0 (200 bytes)
void FUN_020272a0(u32 param_1,int param_2)

{
  u32 *puVar1;
  u32 *puVar2;
  u32 *puVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = DAT_02027368;
  iVar5 = DAT_02027368 + param_2 * 0x18;
  puVar1 = (u32 *)FUN_02030c64();
  puVar2 = (u32 *)FUN_02030b9c(*(u16 *)(puVar1 + 1));
  FUN_02030870(*puVar1,*(u32 *)(iVar4 + param_2 * 0x18));
  FUN_02030870(*puVar2,*(u32 *)(iVar5 + 4));
  iVar4 = 0;
  do {
    if (*(int *)(iVar5 + iVar4 * 4 + 8) != 0) {
      puVar3 = (u32 *)FUN_02030b38(*(u16 *)((int)puVar2 + iVar4 * 2 + 4));
      FUN_02030870(*puVar3 & 0xffffff,*(u32 *)(iVar5 + iVar4 * 4 + 8));
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4);
  FUN_02032220(DAT_0202736c,param_1);
  FUN_0202f454(DAT_0202736c,DAT_02027378,DAT_02027374 & *(int *)(DAT_02027370 + 4) >> 0xc);
  puVar1 = DAT_0202737c;
  (*(volatile u32 *)DAT_0202737c) = param_1;
  *(char *)(puVar1 + 1) = (char)param_2;
  return;
}


// FUN_02027380 @ 0x02027380 (120 bytes)
void FUN_02027380(void)

{
  int iVar1;
  u32 *puVar2;
  
  FUN_02031f50((*(volatile u32 *)DAT_020273f8),(*(volatile u32 *)DAT_020273fc));
  FUN_02031f20((*(volatile u32 *)DAT_02027400),2,(*(volatile u32 *)DAT_020273fc));
  iVar1 = FUN_02030b9c((*(volatile u32 *)DAT_02027400));
  puVar2 = (u32 *)FUN_02030b38(*(u16 *)(iVar1 + 4));
  FUN_02026f08(*puVar2 & 0xffffff,(*(volatile u32 *)DAT_020273fc),0,1,0);
  return;
}


// FUN_02027404 @ 0x02027404 (248 bytes)
void FUN_02027404(int param_1,int param_2,int param_3,u32 param_4)

{
  u32 *puVar1;
  u32 uVar2;
  int iVar3;
  u32 unaff_r8;
  
  if (param_2 == *(short *)(DAT_020274fc + param_1 * 2)) {
    if (param_3 == 0) {
      return;
    }
    FUN_020272a0(param_2,param_1);
    return;
  }
  puVar1 = DAT_02027504;
  if (param_1 != 0) {
    unaff_r8 = (*(volatile u32 *)DAT_02027500);
    puVar1 = DAT_02027500;
  }
  if (param_1 == 0) {
    unaff_r8 = *puVar1;
  }
  *(short *)(DAT_020274fc + param_1 * 2) = (short)param_2;
  uVar2 = FUN_02031058(unaff_r8);
  FUN_02031064(unaff_r8,uVar2);
  FUN_0203b914(DAT_02027508,0,0x38);
  FUN_02026cac();
  iVar3 = FUN_020270d4(unaff_r8,param_2,param_1 * 0x18 + DAT_0202750c);
  (*(volatile u32 *)DAT_02027510) = param_4;
  if (iVar3 == 0) {
    FUN_02026bec(param_3 != 0,param_2,param_1,0);
    return;
  }
  FUN_02026bec(2,param_2,param_1,0);
  return;
}


// FUN_02027514 @ 0x02027514 (192 bytes)
void FUN_02027514(void)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_020275d4;
  if (((volatile u32 *)DAT_020275d4)[3] == 0) {
    return;
  }
  iVar2 = ((volatile u32 *)DAT_020275d4)[3];
  if (iVar2 == 3) {
    return;
  }
  if (iVar2 == 1) {
    ((volatile u32 *)DAT_020275d4)[1] = ((volatile u32 *)DAT_020275d4)[1] + ((volatile u32 *)DAT_020275d4)[2];
    if (*piVar1 < piVar1[1]) {
      piVar1[1] = *piVar1;
      piVar1[3] = 0;
    }
  }
  else if ((iVar2 == 2) && (((volatile u32 *)DAT_020275d4)[1] = ((volatile u32 *)DAT_020275d4)[1] - ((volatile u32 *)DAT_020275d4)[2], piVar1[1] < 0)) {
    piVar1[1] = 0;
    piVar1[3] = 3;
  }
  FUN_0202f454(DAT_020275dc,DAT_020275e0,DAT_020275d8 & piVar1[1] >> 0xc);
  return;
}


// FUN_020275e4 @ 0x020275e4 (104 bytes)
void FUN_020275e4(u32 param_1,int param_2)

{
  FUN_0202f5b4((*(volatile u32 *)DAT_0202764c),param_1,0);
  FUN_02032188(DAT_02027650,(*(volatile u32 *)DAT_0202764c),param_1);
  if (param_2 != -1) {
    FUN_0202f4c4(DAT_02027650,param_2,0);
    return;
  }
  return;
}


// FUN_02027654 @ 0x02027654 (72 bytes)
void FUN_02027654(short param_1,u32 param_2,short param_3)

{
  int iVar1;
  short *psVar2;
  
  iVar1 = 3;
  psVar2 = DAT_0202769c;
  do {
    if (*psVar2 == 0) {
      *psVar2 = param_1;
      psVar2[1] = param_3;
      *(u32 *)(psVar2 + 2) = param_2;
      return;
    }
    psVar2 = psVar2 + 4;
    iVar1 = iVar1 + -1;
  } while (-1 < iVar1);
  return;
}


// FUN_020276a0 @ 0x020276a0 (108 bytes)
void FUN_020276a0(void)

{
  int iVar1;
  short *psVar2;
  
  iVar1 = 3;
  psVar2 = DAT_0202770c;
  do {
    if ((*psVar2 != 0) &&
       (*(int *)(psVar2 + 2) = *(int *)(psVar2 + 2) + -1, *(int *)(psVar2 + 2) < 0)) {
      FUN_020275e4((int)*psVar2,(int)psVar2[1]);
      *psVar2 = 0;
      psVar2[1] = -1;
      psVar2[2] = 0;
      psVar2[3] = 0;
    }
    psVar2 = psVar2 + 4;
    iVar1 = iVar1 + -1;
  } while (-1 < iVar1);
  return;
}


// FUN_020277b4 @ 0x020277b4 (144 bytes)
void FUN_020277b4(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_02027844 + param_1 * 4);
  if (iVar1 == -1) {
    (*(volatile u32 *)DAT_02027848) = 0;
    FUN_02034084();
    FUN_02026830((*(volatile u32 *)DAT_02027850) & 0xff,((volatile u32 *)DAT_02027850)[1] & 0xff);
    return;
  }
  if ((*(volatile u32 *)DAT_02027848) != 0) {
    FUN_02033f8c(iVar1);
    return;
  }
  (*(volatile u32 *)DAT_02027848) = 1;
  FUN_02030730(0);
  FUN_020340b8(DAT_0202784c,0x400,iVar1);
  return;
}


// FUN_02027854 @ 0x02027854 (160 bytes)
void FUN_02027854(int param_1)

{
  int iVar1;
  
  if ((((*(volatile u32 *)DAT_020278f4) == param_1) || (param_1 < 0)) &&
     (iVar1 = FUN_0202f51c((*(volatile u32 *)DAT_020278f8),0), iVar1 != 0)) {
    FUN_0202f5b4((*(volatile u32 *)DAT_020278f8),0,4);
  }
  if ((((volatile u32 *)DAT_020278f4)[1] != param_1) && (-1 < param_1)) {
    return;
  }
  iVar1 = FUN_0202f51c((*(volatile u32 *)DAT_020278f8),1);
  if (iVar1 != 0) {
    FUN_0202f5b4((*(volatile u32 *)DAT_020278f8),1,4);
    return;
  }
  0;
  return;
}


// FUN_020278fc @ 0x020278fc (348 bytes)
void FUN_020278fc(s16 *param_1,int param_2,u32 param_3)

{
  short sVar1;
  u32 uVar2;
  u32 *puVar3;
  u32 *puVar4;
  u32 uVar5;
  int iVar6;
  u8 auStack_30 [2];
  u16 uStack_2e;
  
  if (*(int *)(DAT_02027a58 + 0x20) != 0) {
    return;
  }
  uVar2 = FUN_02031058((*(volatile u32 *)DAT_02027a5c));
  FUN_02031064((*(volatile u32 *)DAT_02027a5c),uVar2);
  sVar1 = 0;
  puVar3 = (u32 *)FUN_02030b9c((*(volatile u32 *)DAT_02027a60));
  puVar4 = (u32 *)FUN_02030b38(*(u16 *)(puVar3 + 1));
  uVar2 = FUN_0203088c(*puVar4 & 0xffffff);
  uVar5 = FUN_0203088c(*puVar3);
  for (iVar6 = (int)*param_1; 0 < iVar6; iVar6 = (iVar6 + -1) * 0x10000 >> 0x10) {
    param_1 = param_1 + 1;
    if (*param_1 != 0) {
      FUN_0203d028(uVar5,*param_1,0,auStack_30);
      FUN_02026d40(uVar2,uStack_2e,*puVar4 & 0xffffff,(*(volatile u32 *)DAT_02027a5c),0);
      sVar1 = sVar1 + 1;
    }
  }
  if (sVar1 != 0) {
    FUN_0203d30c(uVar5,0,uVar2);
    if (param_2 != 0) {
      (*(volatile u32 *)DAT_02027a64) = param_3;
      FUN_02026bec(0,0,0xff,0);
      return;
    }
    0;
    return;
  }
  return;
}


// FUN_02027a68 @ 0x02027a68 (192 bytes)
void FUN_02027a68(int param_1,int param_2)

{
  u8 bVar1;
  BOOL bVar2;
  u8 *pbVar3;
  u32 uVar4;
  
  if ((*(volatile u32 *)DAT_02027b28) < 0) {
    return;
  }
  if (param_1 != 0) {
    FUN_02027854(0);
    pbVar3 = DAT_02027b2c;
    (*(volatile u32 *)DAT_02027b2c) = (*(volatile u32 *)DAT_02027b2c) ^ 1;
    bVar1 = *pbVar3;
    if (bVar1 != 0) {
      (*(volatile u32 *)DAT_02027b30) = (short)param_1;
      uVar4 = 0xf;
    }
    else {
      ((volatile u32 *)DAT_02027b30)[1] = (short)param_1;
      uVar4 = 0xe;
    }
    bVar2 = bVar1 == 0;
    FUN_0202f3f4(uVar4,param_1);
    FUN_0202f5b4((*(volatile u32 *)DAT_02027b28),bVar2,0);
    FUN_02032188(DAT_02027b34,(*(volatile u32 *)DAT_02027b28),bVar2);
    if (param_2 != -1) {
      FUN_0202f4c4(DAT_02027b34,param_2,0);
      return;
    }
    0;
    return;
  }
  return;
}


// FUN_02027bd8 @ 0x02027bd8 (152 bytes)
int FUN_02027bd8(u32 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 3;
  do {
    if (*(int *)(DAT_02027c70 + iVar1 * 4) == 0) break;
    iVar1 = (iVar1 + -1) * 0x10000 >> 0x10;
  } while (0 < iVar1);
  if (0 < iVar1) {
    iVar2 = DAT_02027c70 + iVar1 * 4;
    FUN_02032188(iVar2,(*(volatile u32 *)DAT_02027c74),param_1);
    if (param_2 != -1) {
      FUN_0202f4c4(iVar2,param_2,0);
    }
    return iVar1;
  }
  return -1;
}


// FUN_02027c78 @ 0x02027c78 (116 bytes)
void FUN_02027c78(u32 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_0202f51c((*(volatile u32 *)DAT_02027cec),param_1);
  if (iVar1 != 0) {
    return;
  }
  FUN_02032188(DAT_02027cf0,(*(volatile u32 *)DAT_02027cec),param_1);
  if (param_2 != -1) {
    FUN_0202f4c4(DAT_02027cf0,param_2,0);
    return;
  }
  return;
}


// FUN_02027e78 @ 0x02027e78 (100 bytes)
void FUN_02027e78(int param_1)

{
  if (param_1 == (*(volatile u32 *)DAT_02027edc)) {
    FUN_020272a0(param_1,0);
    return;
  }
  if (param_1 == ((volatile u32 *)DAT_02027edc)[1]) {
    FUN_020272a0(param_1,1);
    return;
  }
  FUN_02027f64(param_1,0,0xc00);
  return;
}


// FUN_02027f64 @ 0x02027f64 (132 bytes)
void FUN_02027f64(int param_1,u32 param_2,u32 param_3)

{
  if (param_1 == 0) {
    FUN_02027e5c(0);
    return;
  }
  if (*(int *)(DAT_02027fe8 + 0x20) == 0) {
    if ((param_2 == *(u8 *)(DAT_02027fec + 4)) &&
       (param_1 != *(short *)(DAT_02027ff0 + param_2 * 2))) {
      FUN_02027e5c(0);
    }
    FUN_02027404(param_2,(int)(short)param_1,1,param_3);
    return;
  }
  return;
}


// FUN_020285c8 @ 0x020285c8 (204 bytes)
u32 FUN_020285c8(u32 param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int *piVar2;
  u16 uVar3;
  int iVar4;
  
  iVar4 = FUN_02028710();
  piVar2 = DAT_020286a0;
  piVar1 = DAT_0202869c;
  if (iVar4 != 0) {
    (*(volatile u32 *)DAT_02028698) = param_1;
    *piVar1 = param_2;
    *piVar2 = param_3;
    if (param_4 != 0) {
      uVar3 = ((u16 (*)(int,...))DAT_020286a4)(param_2,param_3 + -2);
      *(u16 *)((*(volatile u32 *)DAT_0202869c) + (*(volatile u32 *)DAT_020286a0) + -2) = uVar3;
    }
    ((void (*)(int,...))DAT_020286a8)((*(volatile u32 *)DAT_02028698),(*(volatile u32 *)DAT_0202869c),(*(volatile u32 *)DAT_020286a0),DAT_020286ac);
    (*(volatile u32 *)DAT_02028694) = 1;
    return 1;
  }
  (*(volatile u32 *)DAT_02028694) = 0;
  return 0;
}


// FUN_02028948 @ 0x02028948 (156 bytes)
u32 FUN_02028948(int param_1,u32 param_2,u32 param_3)

{
  int iVar1;
  u32 uVar2;
  
  if ((*(volatile u32 *)DAT_020289e4) == '\0') {
    return 0;
  }
  if (param_1 == 0) {
    param_3 = 0x10;
    param_1 = 0x1fc0;
    param_2 = DAT_020289e8;
  }
  else if (param_1 == 1) {
    param_3 = 0x20;
    param_1 = DAT_020289f0;
    param_2 = DAT_020289ec;
  }
  else if (param_1 == 2) {
    param_3 = 0x40;
    param_1 = 0x1fc0;
    param_2 = DAT_020289e8;
  }
  iVar1 = FUN_020285c8(param_1,param_2,param_3,0);
  if (iVar1 != 0) {
    iVar1 = FUN_02028440(iVar1);
    return (u32)(iVar1 != 0);
  }
  uVar2 = 0;
  return uVar2;
}


// FUN_02028a9c @ 0x02028a9c (16 bytes)
int FUN_02028a9c(int param_1)

{
  return DAT_02028ab0 * param_1 + DAT_02028aac;
}


// FUN_02028ab4 @ 0x02028ab4 (76 bytes)
BOOL FUN_02028ab4()

{
  int iVar1;
  
  iVar1 = FUN_020285c8(0xc,DAT_02028b00,0x14,1);
  if (iVar1 != 0) {
    iVar1 = FUN_02028440(iVar1);
    return iVar1 != 0;
  }
  return FALSE;
}


// FUN_02028b34 @ 0x02028b34 (20 bytes)
void FUN_02028b34(void)

{
  ((void (*)(int,...))DAT_02028b48)(DAT_02028b4c,0,0x14);
  return;
}


// FUN_02028b80 @ 0x02028b80 (76 bytes)
BOOL FUN_02028b80()

{
  int iVar1;
  
  iVar1 = FUN_020285c8(8,DAT_02028bcc,4,1);
  if (iVar1 != 0) {
    iVar1 = FUN_02028440(iVar1);
    return iVar1 != 0;
  }
  return FALSE;
}


// FUN_02028bd0 @ 0x02028bd0 (76 bytes)
BOOL FUN_02028bd0(void)

{
  int iVar1;
  
  iVar1 = FUN_020285c8(0,DAT_02028c1c,8,0);
  if (iVar1 != 0) {
    iVar1 = FUN_02028440(iVar1);
    return iVar1 != 0;
  }
  return FALSE;
}


// FUN_02028c74 @ 0x02028c74 (96 bytes)
BOOL FUN_02028c74(void)

{
  int iVar1;
  
  if ((*(volatile u32 *)DAT_02028cd4) != '\0') {
    FUN_0203b914(DAT_02028cd8,0xff,0x2000);
    iVar1 = FUN_020285c8(0,DAT_02028cd8,0x2000,0);
    return iVar1 != 0;
  }
  return FALSE;
}


// FUN_02028d24 @ 0x02028d24 (372 bytes)
u32 FUN_02028d24(void)

{
  u8 uVar1;
  u8 uVar2;
  u8 uVar3;
  u8 uVar4;
  u8 uVar5;
  u8 uVar6;
  u8 uVar7;
  u8 *puVar8;
  int iVar9;
  int iVar10;
  u32 uVar11;
  u32 uVar12;
  
  iVar9 = FUN_02028c20();
  iVar10 = iVar9;
  if (iVar9 < 1) {
    if (iVar9 < 0) {
      FUN_0203b914(DAT_02028e98,0,0x2000);
    }
    else {
      FUN_0203b914(DAT_02028e98,0,0x1fc0);
    }
    puVar8 = DAT_02028e98;
    uVar1 = ((volatile u32 *)DAT_02028e9c)[1];
    uVar2 = ((volatile u32 *)DAT_02028e9c)[2];
    uVar3 = ((volatile u32 *)DAT_02028e9c)[3];
    uVar4 = ((volatile u32 *)DAT_02028e9c)[4];
    uVar5 = ((volatile u32 *)DAT_02028e9c)[5];
    uVar6 = ((volatile u32 *)DAT_02028e9c)[6];
    uVar7 = ((volatile u32 *)DAT_02028e9c)[7];
    (*(volatile u32 *)DAT_02028e98) = (*(volatile u32 *)DAT_02028e9c);
    puVar8[1] = uVar1;
    puVar8[2] = uVar2;
    puVar8[3] = uVar3;
    puVar8[4] = uVar4;
    puVar8[5] = uVar5;
    puVar8[6] = uVar6;
    puVar8[7] = uVar7;
    FUN_02028b34();
    iVar10 = 0;
    if ((*(volatile u32 *)DAT_02028ea0) != '\0') {
      iVar10 = FUN_02028bd0();
      if (iVar10 == 0) {
        return 0;
      }
      iVar10 = FUN_02028b80();
      if (iVar10 == 0) {
        uVar11 = 0;
        return uVar11;
      }
      iVar10 = FUN_02028ab4();
      if (iVar10 == 0) {
        return 0;
      }
      uVar12 = 0;
      do {
        iVar10 = FUN_02028a30(uVar12 & 0xff,uVar12 & 0xff,0);
        if (iVar10 == 0) {
          uVar11 = 0;
          return uVar11;
        }
        iVar10 = FUN_02028440(iVar10);
        if (iVar10 == 0) {
          uVar11 = 0;
          return uVar11;
        }
        uVar12 = uVar12 + 1;
      } while ((int)uVar12 < 4);
      if (iVar9 < 0) {
        iVar10 = FUN_02028948(2,0,0);
      }
    }
  }
  iVar10 = FUN_02028b50(iVar10);
  if (iVar10 == 0) {
    FUN_0203b914(DAT_02028ea4,0,8);
  }
  return 1;
}


// FUN_02029340 @ 0x02029340 (112 bytes)
void FUN_02029340(u32 param_1,u16 *param_2,u16 *param_3,u16 *param_4,
                 u16 *param_5,u16 *param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = DAT_020293c0;
  iVar2 = DAT_020293bc;
  iVar1 = DAT_020293b8;
  iVar5 = (param_1 & DAT_020293b0 & 0xffff) * 0x1c;
  *param_2 = *(u16 *)(DAT_020293b4 + iVar5);
  iVar4 = DAT_020293c4;
  *param_3 = *(u16 *)(iVar1 + iVar5);
  *param_4 = *(u16 *)(iVar2 + iVar5);
  *param_5 = *(u16 *)(iVar3 + iVar5);
  *param_6 = *(u16 *)(iVar4 + iVar5);
  return;
}


// FUN_020293c8 @ 0x020293c8 (176 bytes)
void FUN_020293c8(void)

{
  short *psVar1;
  u16 *puVar2;
  short sVar3;
  u16 *puVar4;
  int iVar5;
  int iVar6;
  u16 uVar7;
  
  *(u16 *)(DAT_0202947c + 0x20) = (*(volatile u32 *)DAT_02029478);
  FUN_02005380();
  iVar5 = DAT_0202947c;
  puVar4 = DAT_02029478;
  sVar3 = *(short *)(DAT_0202947c + 0x20);
  *(short *)(DAT_0202947c + 0x1e) = *(short *)(DAT_0202947c + 0x1e) + 1;
  uVar7 = *puVar4;
  *(u16 *)(iVar5 + 0x22) = uVar7;
  iVar5 = DAT_0202947c;
  if ((int)(u32)uVar7 < (int)sVar3) {
    uVar7 = uVar7 + (short)DAT_02029480;
  }
  psVar1 = (short *)(DAT_0202947c + 0x2e);
  *(u16 *)(DAT_0202947c + 0x24) = uVar7 - sVar3;
  iVar6 = DAT_0202947c;
  if ((*psVar1 == 0) || (*(short *)(iVar5 + 0x2c) < *(short *)(iVar5 + 0x24))) {
    puVar2 = (u16 *)(DAT_0202947c + 0x24);
    *(u16 *)(DAT_0202947c + 0x2e) = *(u16 *)(DAT_0202947c + 0x30);
    *(u16 *)(iVar6 + 0x2c) = *puVar2;
  }
  else {
    *(short *)(iVar5 + 0x2e) = *psVar1 + -1;
  }
  *(u32 *)(DAT_02029484 + 0x3ff8) = *(u32 *)(DAT_02029484 + 0x3ff8) | 1;
  return;
}


// FUN_02029664 @ 0x02029664 (188 bytes)
void FUN_02029664(u16 *param_1,u32 param_2)

{
  u16 uVar1;
  int iVar2;
  u16 uVar3;
  u16 uVar4;
  u32 uVar5;
  
  uVar4 = 0;
  param_1[1] = ~*param_1 & param_2;
  uVar5 = 0;
  param_1[2] = *param_1 & ~param_2;
  iVar2 = DAT_02029720;
  uVar1 = *param_1;
  do {
    uVar3 = param_2 & *(u16 *)(iVar2 + uVar5 * 2);
    if (uVar3 == 0) {
      *(u8 *)((int)param_1 + uVar5 + 10) = 0;
    }
    else if ((uVar1 & uVar3) == 0) {
      uVar4 = uVar4 | uVar3;
      *(char *)((int)param_1 + uVar5 + 10) = (char)param_1[4];
    }
    else {
      *(char *)((int)param_1 + uVar5 + 10) = *(char *)((int)param_1 + uVar5 + 10) + -1;
      if (*(char *)((int)param_1 + uVar5 + 10) == '\0') {
        uVar4 = uVar4 | uVar3;
        *(u8 *)((int)param_1 + uVar5 + 10) = *(u8 *)((int)param_1 + 9);
      }
    }
    uVar5 = uVar5 + 1;
  } while (uVar5 < 10);
  param_1[3] = uVar4;
  *param_1 = param_2;
  return;
}


// FUN_02029724 @ 0x02029724 (52 bytes)
void FUN_02029724(void)

{
  ((void (*)(int,...))DAT_02029764)(DAT_02029768,
                  (((*(volatile u32 *)DAT_02029758) | (*(volatile u32 *)DAT_0202975c)) ^ DAT_02029760) & DAT_02029760 & 0xffff);
  return;
}


// FUN_02029788 @ 0x02029788 (176 bytes)
void FUN_02029788(u32 param_1)

{
  int *piVar1;
  u32 uVar2;
  int iVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  u32 *puVar7;
  u32 *puVar8;
  
  piVar1 = DAT_02029840;
  if (param_1 == 0) {
    param_1 = DAT_02029838 + DAT_0202983c & 0xfffffe00;
  }
  puVar8 = (u32 *)(*(volatile u32 *)DAT_02029840);
  uVar2 = *puVar8;
  uVar4 = puVar8[1];
  uVar5 = puVar8[2];
  uVar6 = puVar8[3];
  puVar7 = (u32 *)(param_1 + DAT_0202983c & 0xfffffe00);
  iVar3 = ((volatile u32 *)DAT_02029840)[2];
  (*(volatile u32 *)DAT_02029840) = (int)puVar7;
  piVar1[1] = (int)puVar7;
  piVar1[2] = (int)puVar8 + (iVar3 - (int)puVar7);
  *puVar7 = uVar2;
  puVar7[1] = uVar4;
  puVar7[2] = uVar5;
  puVar7[3] = uVar6;
  *(int *)(*piVar1 + 8) = (int)puVar8 + (*(int *)(*piVar1 + 8) - (int)puVar7);
  *(int *)piVar1[5] = *piVar1;
  return;
}


// FUN_02029888 @ 0x02029888 (208 bytes)
u32 FUN_02029888(u32 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  u32 uVar5;
  u32 *puVar6;
  
  iVar4 = FUN_02029964(param_1,param_2 + 0x10,0);
  iVar3 = DAT_0202995c;
  iVar2 = DAT_02029958;
  if (iVar4 == 0) {
    return 0xffffffff;
  }
  uVar5 = 5;
  do {
    iVar1 = uVar5 * 0x10;
    if (*(int *)(DAT_02029958 + uVar5 * 0x10) == 0) {
      *(int *)(DAT_02029958 + iVar1) = iVar4;
      *(int *)(iVar3 + iVar1) = iVar4;
      iVar3 = DAT_02029960;
      iVar4 = iVar2 + iVar1;
      *(int *)(DAT_02029960 + iVar1) = param_2;
      *(u32 *)(iVar4 + 0xc) = *(u32 *)(iVar4 + 0xc) & 0xffffffe0 | uVar5 & 0x1f;
      puVar6 = *(u32 **)(iVar2 + iVar1);
      puVar6[1] = 0;
      *puVar6 = puVar6[1];
      puVar6[2] = *(u32 *)(iVar3 + iVar1) | 1;
      puVar6[3] = puVar6[3] & 0xffffffe0 | uVar5 & 0x1f;
      return uVar5;
    }
    uVar5 = uVar5 + 1;
  } while ((int)uVar5 < 0x20);
  FUN_02029ab8();
  return 0xffffffff;
}


// FUN_02029be0 @ 0x02029be0 � stub (missing decompilation)

// FUN_02029bec @ 0x02029bec � stub (missing decompilation)

// FUN_02029c40 @ 0x02029c40 (432 bytes)
void FUN_02029c40(void)

{
  u32 *puVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  u32 *puVar9;
  u32 uVar10;
  
  FUN_0203b7dc(0,DAT_02029df0,0x200);
  uVar5 = DAT_02029e04;
  uVar4 = DAT_02029e00;
  uVar3 = DAT_02029df8;
  uVar2 = DAT_02029df4;
  puVar1 = DAT_02029df0;
  puVar9 = puVar1 + 3;
  uVar8 = puVar1[7];
  puVar1[10] = DAT_02029dfc - DAT_02029df4;
  puVar1[3] = *puVar9 & 0xffffffe0;
  puVar1[7] = uVar8 & 0xffffffe0 | 1;
  uVar8 = 0;
  puVar1[0x13] = puVar1[0x13] & 0xffffffe0 | 4;
  iVar6 = DAT_02029e08;
  puVar1[0x12] = 0xbf0;
  uVar7 = DAT_02029e10 + DAT_02029e14 & 0xfffffe00;
  uVar10 = DAT_02029e1c - uVar7;
  puVar1[0xe] = (((iVar6 - DAT_02029e0c) - DAT_02029e18) - uVar3) + 0x3f70;
  *puVar1 = uVar7;
  puVar1[1] = uVar7;
  puVar1[4] = uVar4;
  puVar1[8] = uVar2;
  puVar1[0xc] = uVar3;
  puVar1[0x10] = uVar5;
  puVar1[5] = uVar4;
  puVar1[9] = uVar2;
  puVar1[0xd] = uVar3;
  puVar1[0x11] = uVar5;
  puVar1[2] = uVar10;
  puVar1[6] = 0;
  puVar1[0xb] = puVar1[0xb] & 0xffffffe0 | 2;
  puVar1[0xf] = puVar1[0xf] & 0xffffffe0 | 3;
  do {
    puVar9 = (u32 *)puVar1[uVar8 * 4];
    switch(uVar8) {
    case 0:
      *puVar9 = 0;
      puVar9[1] = puVar1[4];
      break;
    case 1:
      *puVar9 = *puVar1;
      puVar9[1] = 0;
      break;
    case 2:
      goto LAB_02029d9c;
    case 3:
      goto LAB_02029d9c;
    case 4:
LAB_02029d9c:
      puVar9[1] = 0;
      *puVar9 = puVar9[1];
    }
    puVar9[2] = puVar1[uVar8 * 4 + 2];
    puVar9[3] = puVar9[3] & 0xffffffe0 | uVar8 & 0x1f;
    if (uVar8 != 1) {
      puVar9[2] = puVar9[2] | 1;
    }
    uVar8 = uVar8 + 1;
    if (4 < (int)uVar8) {
      return;
    }
  } while( TRUE );
}


// FUN_02029ec4 @ 0x02029ec4 (192 bytes)
void FUN_02029ec4(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  
  piVar2 = DAT_02029f8c;
  piVar1 = DAT_02029f88;
  piVar4 = (int *)(*(volatile u32 *)DAT_02029f84);
  if (piVar4 != (int *)0x0) {
    while (piVar4 != (int *)0x0) {
      *piVar2 = piVar4[3];
      *piVar1 = (int)piVar4;
      if ((((int)((u32)*(u8 *)((int)piVar4 + 0x12) << 0x1f) < 0) &&
          (-1 < (int)((u32)*(u8 *)((int)piVar4 + 0x12) << 0x1e))) &&
         ((*(void (**)())(*(int *)piVar4 + 8))(), *piVar1 != 0)) {
        piVar4[5] = piVar4[5] + 1;
        *(u8 *)((int)piVar4 + 0x12) = *(u8 *)((int)piVar4 + 0x12) | 2;
      }
      piVar4 = (int *)*piVar2;
    }
  }
  iVar3 = (*(volatile u32 *)DAT_02029f84);
  if (iVar3 == 0) {
    return;
  }
  do {
    *(u8 *)(iVar3 + 0x12) = *(u8 *)(iVar3 + 0x12) & 0xfd;
    iVar3 = *(int *)(iVar3 + 0xc);
  } while (iVar3 != 0);
  return;
}


// FUN_02029f90 @ 0x02029f90 (104 bytes)
void FUN_02029f90(void)

{
  int iVar1;
  
  FUN_02038c80(1);
  for (iVar1 = (*(volatile u32 *)DAT_02029ff8); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0xc)) {
    if ((*(char *)(iVar1 + 0x11) != '\0') && (*(char *)(iVar1 + 0x11) != -1)) {
      *(u8 *)(iVar1 + 0x12) = *(u8 *)(iVar1 + 0x12) & 0xfe;
      *(u8 *)(iVar1 + 0x12) = *(u8 *)(iVar1 + 0x12) | 2;
    }
  }
  FUN_02038cc8();
  return;
}


// FUN_0202a0ec @ 0x0202a0ec (136 bytes)
u32 * FUN_0202a0ec(u32 *param_1)

{
  u32 *puVar1;
  u32 *puVar2;
  
  *param_1 = DAT_0202a174;
  if (param_1[3] != 0) {
    *(u32 *)(param_1[3] + 8) = param_1[2];
  }
  if (param_1[2] == 0) {
    (*(volatile u32 *)DAT_0202a178) = param_1[3];
  }
  else {
    *(u32 *)(param_1[2] + 0xc) = param_1[3];
  }
  puVar1 = DAT_0202a180;
  puVar2 = (u32 *)(*(volatile u32 *)DAT_0202a180);
  (*(volatile u32 *)DAT_0202a17c) = (*(volatile u32 *)DAT_0202a17c) + -1;
  if (puVar2 == param_1) {
    *puVar1 = 0;
  }
  if ((u32 *)(*(volatile u32 *)DAT_0202a184) == param_1) {
    (*(volatile u32 *)DAT_0202a184) = param_1[3];
  }
  thunk_FUN_02029ab8(param_1);
  return param_1;
}

