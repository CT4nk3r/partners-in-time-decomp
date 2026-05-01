/**
 * Scene Manager — scene transitions, tile rendering, sprite allocation
 *
 * Handles tile rendering with palette blitting, sprite rendering dispatch,
 * sprite entry allocation/deallocation, and sprite memory clearing.
 *
 * Decompiled from 0x0201c6f8-0x0201ff8c region (22 functions)
 */
#include "types.h"

/* Forward declarations */
extern void FUN_0202cd68(u32, u32, u32);
extern void FUN_0202cc94(u32, u32, u32);
extern void FUN_0202cc10(u32, u32, u32);
extern void FUN_0201b568(int, int, u32, ...);
extern void FUN_0203b914(u32, u32, u32);
extern void FUN_0201b418(int, int);
extern void FUN_02027bb8(u32);
extern void FUN_0203b808(u32, int, u32);
void FUN_0201c6f8(int param_1, int param_2, u16 *param_3, u32 param_4);

/* Static data references */
static u32 DAT_0201ce4c;   /* tile render function table base */

// FUN_0201ccf4 @ 0x0201ccf4 (344 bytes) — Sprite rendering dispatch
void FUN_0201ccf4(int param_1, int param_2, u32 param_3, u32 param_4)
{
    u32 uVar1;
    u32 uVar2;
    BOOL bVar3;

    bVar3 = *(u32 *)(param_1 + 0x50) >> 0x1e == 0;
    if (bVar3) {
        param_4 = *(u32 *)(param_2 + 0x78);
    }
    uVar1 = (u32)(*(int *)(param_2 + 8) << 0x1b) >> 0x1c;
    if (!bVar3) {
        param_4 = *(u32 *)(param_2 + 0x74);
    }
    uVar2 = *(u32 *)((u32)&DAT_0201ce4c + uVar1 * 4);
    if (uVar1 != 0) {
        if (((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) &&
           (((*(u32 *)(param_2 + 0x10) & 7) != 0 ||
             (((*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17 & 7) != 0)))) {
            FUN_0201b568(param_1, param_2, uVar2);
            if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
                FUN_0201b568(param_1, param_2, uVar2, *(u32 *)(param_2 + 0x74));
                return;
            }
            return;
        }
        FUN_0201c6f8(param_1, param_2, (u16 *)uVar2, param_4);
        if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
            FUN_0201c6f8(param_1, param_2, (u16 *)uVar2, *(u32 *)(param_2 + 0x74));
            return;
        }
        return;
    }
    FUN_0202cc10(param_4, 0, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10);
    if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
        FUN_0202cc10(*(u32 *)(param_2 + 0x74), 0, (u32)(*(int *)(param_2 + 0x6c) << 0xf) >> 0x10);
        return;
    }
    return;
}

// FUN_0201d0cc @ 0x0201d0cc (164 bytes) — Clear sprite memory
void FUN_0201d0cc(int param_1, int param_2)
{
    int iVar1;
    int iVar2;

    if (param_2 == 0) {
        FUN_0202cc10(*(u32 *)(param_1 + 0x34), 0, 0x780);
        iVar2 = *(int *)(param_1 + 0xf10);
    } else {
        FUN_0202cc10(*(u32 *)(param_1 + 0x38), 0, 0x780);
        iVar2 = *(int *)(param_1 + 0xf20);
    }
    iVar1 = *(int *)(iVar2 + 4);
    while (iVar1 != -1) {
        if ((u32)(*(int *)(iVar2 + 8) << 0xe) >> 0x1d == 0) {
            FUN_0201b418(param_1, iVar2);
        }
        iVar2 = *(int *)(iVar2 + 4);
        iVar1 = *(int *)(iVar2 + 4);
    }
    if (param_2 == 0) {
        *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 2;
    } else {
        *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 4;
    }
    return;
}

// FUN_0201d318 @ 0x0201d318 (164 bytes) — Reset sprite rendering state
void FUN_0201d318(int param_1, int param_2)
{
    int iVar1;
    int iVar2;

    if (param_2 == 0) {
        iVar2 = *(int *)(param_1 + 0xf10);
    } else {
        iVar2 = *(int *)(param_1 + 0xf20);
    }
    iVar1 = *(int *)(iVar2 + 4);
    while (iVar1 != -1) {
        *(u32 *)(iVar2 + 0x68) = *(u32 *)(iVar2 + 0x68) | 1;
        iVar2 = *(int *)(iVar2 + 4);
        iVar1 = *(int *)(iVar2 + 4);
    }
    if (param_2 == 0) {
        *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 8;
    } else {
        *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 0x10;
    }
    return;
}

/* ===== Code to append to arm9/src/scene_manager.c ===== */

/* Forward declarations for functions defined later in this block */
void FUN_0201dae4(u32 param_1, int param_2, int param_3, int param_4);
void FUN_0201f0e0(int param_1, int param_2);
int FUN_0201fef8(u32, u8 *, u32);

/* New extern function declarations */
extern void FUN_0201a698(int, int, int, int);
extern u32 FUN_0201a960(u32, u32, u32, u32);
extern int FUN_0201a248(int *, int);
extern void FUN_02019888(int *, int);
extern void FUN_02019b44(...);
extern void FUN_02019e9c(...);
extern void FUN_020084a0(int);
extern void FUN_020200f4(u32 *);
extern void FUN_02021c34(int);
extern void FUN_02022048(u32, int, int, u32, u32, u8, u32, u32, int, int);
extern void FUN_02022200(u32);
extern void FUN_0202277c(int, u32, int);
extern void FUN_0202281c(u32, u32);
extern void FUN_02022564(u32);
extern void FUN_02022868(int, int, u32);
extern int FUN_020223e8(void);
extern void FUN_020222d8(int);
extern void FUN_02025810(u32 *, int, u32, int);
extern u16 FUN_02025f20(int);
extern void FUN_0202609c(u32, u32, int, int, int, u32, u32, u32, u32, int);
extern void FUN_0202626c(int);
extern void FUN_02026388(int, u32);
extern void FUN_02027cf4(u32, int, u32);
extern void FUN_02029e20(u32);
extern void FUN_0202a5a8(u32 *);
extern void FUN_0201dcd8(void);
extern u32 FUN_020197e4(int, int, u32, u32 *);
extern void FUN_0201b208(u32 *, int, int);
extern void FUN_0201b310(u32 *, int, int);
extern u32 FUN_02021c64(void);
extern u32 FUN_020222f4(int, int);
extern void FUN_0202605c(int, int, u32, u32 *);
extern u32 FUN_02029bf8(int, int, u32, int);
extern int FUN_02029c1c(int, int, u32, int);
extern void FUN_02029ffc(u32);
extern void FUN_0202a74c(void);
extern void thunk_FUN_02029ab8(...);

/* Extern data references */
extern u32 DAT_0201d0bc;
extern u32 DAT_0201d0c0;
extern u32 DAT_0201d0c4;
extern u32 DAT_0201d0c8;
extern u32 DAT_0201d314;
extern u32 DAT_0201d9b4;
extern u32 DAT_0201d9b8;
extern u32 DAT_0201d9bc;
extern u32 DAT_0201d9c0;
extern u32 DAT_0201dccc;
extern u32 DAT_0201dcc8;
extern u32 DAT_0201dcd0;
extern u32 DAT_0201dcd4;
extern u32 DAT_0201e820;
extern u32 DAT_0201e824;
extern u32 DAT_0201e828;
extern u32 DAT_0201e82c;
extern u32 DAT_0201e830;
extern u32 DAT_0201e834;
extern u32 DAT_0201eedc;
extern u32 DAT_0201eee0;
extern u32 DAT_0201eee4;
extern u32 DAT_0201eee8;
extern u32 DAT_0201eeec;
extern u32 DAT_0201eef0;
extern u32 DAT_0201eef4;
extern u32 DAT_0201f04c;
extern u32 DAT_0201f1e0;
extern u32 DAT_0201f1e4;
extern u32 DAT_0201f1e8;
extern u32 DAT_0201f41c;
extern u32 DAT_0201f4e4;
extern u32 DAT_0201f5a4;
extern u32 DAT_0201fa10;
extern u32 DAT_0201fa14;
extern u32 DAT_0201fa18;
extern u32 DAT_0201fa1c;
extern u32 DAT_0201fa20;
extern u32 DAT_0201fa24;
extern u32 DAT_0201fa28;
extern u32 DAT_0201fa2c;
extern u32 DAT_0201fa30;
extern u32 DAT_0201fa34;
extern u32 DAT_0201fa38;
extern u32 DAT_0201fa3c;
extern u32 DAT_0201fa40;
extern u32 DAT_0201fa44;
extern u32 DAT_0201fa48;
extern u32 DAT_0201fa4c;
extern u32 DAT_0201feb8;
extern u32 DAT_0201febc;
extern u32 DAT_0201fec0;
extern u32 DAT_0201fec4;
extern u32 DAT_0201fec8;
extern u32 DAT_0201fecc;
extern u32 DAT_0201fed0;
extern u32 DAT_0201fed4;
extern u32 DAT_0201fed8;
extern u32 DAT_0201fedc;
extern u32 DAT_0201fee0;
extern u32 DAT_0201fee4;
extern u32 DAT_0201fee8;
extern u32 DAT_0201feec;
extern u32 DAT_0201fef0;
extern u32 DAT_0201fef4;

// FUN_0201c6f8 @ 0x0201c6f8 (1532 bytes) — Tile rendering with palette blitting
void FUN_0201c6f8(int param_1, int param_2, u16 *param_3, u32 param_4)
{
  u16 uVar1;
  u16 uVar2;
  u32 uVar3;
  int iVar4;
  u32 uVar5;
  u32 uVar6;
  int iVar7;
  int iVar8;
  u32 uVar9;
  u16 *puVar10;
  u16 *puVar11;
  u16 *puVar12;
  u16 *puVar13;
  u16 **ppuVar14;
  u16 *local_a0;
  u32 local_9c;
  u32 local_98;
  int local_94;
  int local_90;
  u32 local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  u32 local_40;
  u16 *local_3c;
  u16 *local_38;
  u16 *local_34;
  u16 *local_30;
  u32 local_2c;

  local_a0 = param_3;
  local_48 = ((u32)(*(int *)(param_2 + 8) << 0x11) >> 0x1b) + 1;
  local_9c = param_4;
  uVar1 = param_3[1];
  local_90 = ((u32)(*(int *)(param_2 + 8) << 0x16) >> 0x1b) + 1;
  local_40 = (u32)uVar1;
  local_74 = local_48 - (u32)uVar1;
  uVar2 = *param_3;
  local_98 = (u32)uVar2;
  local_44 = (int)(short)uVar1;
  local_5c = local_90 - (u32)uVar2;
  uVar3 = local_5c * 0x80000;
  uVar5 = (u32)(local_90 * local_74 * 0x80000) >> 0x10;
  puVar12 = *(u16 **)(param_3 + 4);
  puVar11 = *(u16 **)(param_3 + 6);
  puVar10 = *(u16 **)(param_3 + 8);
  puVar13 = *(u16 **)(param_3 + 10);
  local_94 = local_44;
  if (0 < local_44) {
    local_4c = (uVar5 + (uVar3 >> 0x10) & 0xffff) << 2;
    local_54 = (uVar3 >> 0x10) << 2;
    local_50 = uVar5 << 2;
    local_58 = (int)(short)uVar2;
    iVar7 = local_58;
    do {
      for (; 0 < iVar7; iVar7 = (iVar7 + -1) * 0x10000 >> 0x10) {
        uVar3 = *(int *)(param_1 + 0x58) + (u32)*puVar12 * 4;
        if (uVar3 < param_4) {
          FUN_0202cd68(uVar3, param_4, 0x20);
        }
        else {
          FUN_0202cc94(uVar3, param_4, 0x20);
        }
        uVar5 = param_4 + local_54;
        uVar3 = *(int *)(param_1 + 0x58) + (u32)*puVar11 * 4;
        if (uVar3 < uVar5) {
          FUN_0202cd68(uVar3, uVar5, 0x20);
        }
        else {
          FUN_0202cc94(uVar3, uVar5, 0x20);
        }
        uVar5 = param_4 + local_50;
        uVar3 = *(int *)(param_1 + 0x58) + (u32)*puVar10 * 4;
        if (uVar3 < uVar5) {
          FUN_0202cd68(uVar3, uVar5, 0x20);
        }
        else {
          FUN_0202cc94(uVar3, uVar5, 0x20);
        }
        uVar5 = param_4 + local_4c;
        uVar3 = *(int *)(param_1 + 0x58) + (u32)*puVar13 * 4;
        if (uVar3 < uVar5) {
          FUN_0202cd68(uVar3, uVar5, 0x20);
        }
        else {
          FUN_0202cc94(uVar3, uVar5, 0x20);
        }
        param_4 = param_4 + 0x20;
        puVar10 = puVar10 + 1;
        puVar11 = puVar11 + 1;
        puVar12 = puVar12 + 1;
        puVar13 = puVar13 + 1;
      }
      param_4 = param_4 + local_54;
      local_94 = (local_94 + -1) * 0x10000 >> 0x10;
      iVar7 = local_58;
    } while (0 < local_94);
  }
  local_38 = *(u16 **)(local_a0 + 0xc);
  local_8c = (u32)*local_38;
  local_88 = (int)(short)local_98;
  uVar1 = local_38[1];
  local_30 = local_38 + local_8c * uVar1 + 2;
  iVar7 = local_90 * (local_48 - (u32)uVar1);
  if (local_88 < local_5c) {
    local_68 = (int)(short)uVar1;
    local_60 = ((local_90 - local_8c) * 0x80000 >> 0x10) << 2;
    local_70 = local_88 << 3;
    local_6c = local_8c << 3;
    local_64 = (int)(short)*local_38;
    do {
      puVar10 = local_38 + 2;
      uVar3 = local_9c + local_70 * 4;
      puVar11 = local_30;
      for (iVar4 = local_68; iVar8 = local_64, 0 < iVar4; iVar4 = (iVar4 + -1) * 0x10000 >> 0x10) {
        for (; 0 < iVar8; iVar8 = (iVar8 + -1) * 0x10000 >> 0x10) {
          uVar5 = *(int *)(param_1 + 0x58) + (u32)*puVar10 * 4;
          if (uVar5 < uVar3) {
            FUN_0202cd68(uVar5, uVar3, 0x20);
          }
          else {
            FUN_0202cc94(uVar5, uVar3, 0x20);
          }
          uVar6 = uVar3 + ((u32)(iVar7 * 0x80000) >> 0x10) * 4;
          uVar5 = *(int *)(param_1 + 0x58) + (u32)*puVar11 * 4;
          if (uVar5 < uVar6) {
            FUN_0202cd68(uVar5, uVar6, 0x20);
          }
          else {
            FUN_0202cc94(uVar5, uVar6, 0x20);
          }
          uVar3 = uVar3 + 0x20;
          puVar11 = puVar11 + 1;
          puVar10 = puVar10 + 1;
        }
        uVar3 = uVar3 + local_60;
      }
      local_70 = local_70 + local_6c;
      local_88 = (int)((local_88 + local_8c) * 0x10000) >> 0x10;
    } while (local_88 < local_5c);
  }
  local_3c = *(u16 **)(local_a0 + 0xe);
  uVar1 = *local_3c;
  local_2c = (u32)local_3c[1];
  local_34 = local_3c + uVar1 * local_2c + 2;
  if (local_44 < local_74) {
    local_78 = (int)(short)uVar1;
    iVar7 = (((local_90 - (u32)uVar1) * 0x80000 >> 0x10)) * 4;
    local_84 = local_44 * local_90;
    local_80 = local_90 * local_2c;
    local_7c = (int)(short)local_3c[1];
    do {
      puVar10 = local_3c + 2;
      uVar3 = local_9c + local_84 * 0x20;
      puVar11 = local_34;
      for (iVar4 = local_7c; iVar8 = local_78, 0 < iVar4; iVar4 = (iVar4 + -1) * 0x10000 >> 0x10) {
        for (; 0 < iVar8; iVar8 = (iVar8 + -1) * 0x10000 >> 0x10) {
          uVar5 = *(int *)(param_1 + 0x58) + (u32)*puVar10 * 4;
          if (uVar5 < uVar3) {
            FUN_0202cd68(uVar5, uVar3, 0x20);
          }
          else {
            FUN_0202cc94(uVar5, uVar3, 0x20);
          }
          uVar6 = uVar3 + iVar7;
          uVar5 = *(int *)(param_1 + 0x58) + (u32)*puVar11 * 4;
          if (uVar5 < uVar6) {
            FUN_0202cd68(uVar5, uVar6, 0x20);
          }
          else {
            FUN_0202cc94(uVar5, uVar6, 0x20);
          }
          uVar3 = uVar3 + 0x20;
          puVar11 = puVar11 + 1;
          puVar10 = puVar10 + 1;
        }
        uVar3 = uVar3 + iVar7;
      }
      local_84 = local_84 + local_80;
      local_44 = (int)((local_44 + local_2c) * 0x10000) >> 0x10;
    } while (local_44 < local_74);
  }
  iVar7 = (int)((local_48 + local_40 * -2) * 0x10000) >> 0x10;
  iVar4 = local_90 + local_98 * -2;
  iVar8 = local_9c + (local_90 * local_40 + local_98) * 0x20;
  ppuVar14 = &local_a0;
  if (iVar7 < 1) {
    ppuVar14 = (u16 **)0;
  }
  uVar9 = *(u32 *)(local_a0 + 2);
  if (0 < iVar7) {
    uVar3 = local_90 << 0x13;
    do {
      *(u32 *)((int)ppuVar14 + 0x78) = uVar9;
      FUN_0203b808(*(u32 *)((int)ppuVar14 + 0x78), iVar8, iVar4 * 0x20);
      iVar7 = (iVar7 + -1) * 0x10000 >> 0x10;
      iVar8 = iVar8 + (uVar3 >> 0xe);
    } while (0 < iVar7);
    return;
  }
  return;
}

// FUN_0201ce50 @ 0x0201ce50 (620 bytes) — Sprite entry allocation
int * FUN_0201ce50(int param_1, int param_2, u32 param_3, u32 param_4)
{
  int iVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  BOOL bVar8;
  int iStack_28;

  if ((u32)*(u16 *)(param_1 + 0xf08) ==
      ((u32)(*(int *)(param_1 + 0x50) << 0x17) >> 0x1b) +
      ((u32)(*(int *)(param_1 + 0x50) << 0x12) >> 0x1b)) {
    return (int *)0x0;
  }
  if (param_2 == 0) {
    iVar7 = *(int *)(param_1 + 0x3c);
    piVar5 = *(int **)(param_1 + 0xf10);
    iStack_28 = *(int *)(param_1 + 0x44);
    iVar1 = iVar7;
    piVar6 = piVar5;
  }
  else {
    iVar7 = *(int *)(param_1 + 0x40);
    piVar5 = *(int **)(param_1 + 0xf20);
    iStack_28 = *(int *)(param_1 + 0x48);
    iVar1 = iVar7;
    piVar6 = piVar5;
  }
  while ((int *)piVar5[1] != (int *)0xffffffff) {
    if (param_3 <= (u32)(piVar5[0x1d] - iVar7)) {
      bVar8 = TRUE;
      goto LAB_0201cf18;
    }
    iVar7 = iVar7 + (piVar5[0x1d] - iVar7) + ((u32)(piVar5[0x1b] << 0xf) >> 0x10);
    piVar5 = (int *)piVar5[1];
  }
  bVar8 = param_3 <= (u32)((iVar1 + iStack_28) - iVar7);
LAB_0201cf18:
  if (!bVar8) {
    return (int *)0x0;
  }
  if (param_4 == 0xffffffff) {
    iVar3 = (*(int **)(param_1 + 0xf00))[0x1d];
    param_4 = 0;
    piVar5 = *(int **)(param_1 + 0xf00);
    while (iVar3 != 0) {
      param_4 = (int)((param_4 + 1) * 0x10000) >> 0x10;
      iVar3 = piVar5[0x50];
      piVar5 = piVar5 + 0x33;
    }
  }
  else {
    piVar5 = (int *)((short)param_4 * 0xcc + *(int *)(param_1 + 0xf00));
  }
  FUN_0203b914((u32)piVar5, 0, 0xcc);
  piVar5[0x1d] = iVar7;
  piVar5[0x1b] = piVar5[0x1b] & DAT_0201d0c0 | (param_3 & DAT_0201d0bc) << 1;
  piVar5[0x1a] = piVar5[0x1a] & DAT_0201d0c4 |
                 (((u32)(iVar7 - iVar1) >> 5) + 1 & DAT_0201d0c8) << 0xb;
  piVar5[0x1a] = piVar5[0x1a] & 0x83ffffffU | (param_4 & 0x1f) << 0x1a;
  if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
    if (param_2 == 0) {
      iStack_28 = *(int *)(param_1 + 0xf04);
    }
    else {
      iStack_28 = *(int *)(param_1 + 0xf04) + iStack_28;
    }
    piVar5[0x1e] = iStack_28 + (iVar7 - iVar1);
  }
  if ((int *)piVar6[1] != (int *)0xffffffff) {
    piVar2 = (int *)piVar6[1];
    do {
      piVar4 = piVar2;
      if ((u32)piVar5[0x1d] < (u32)piVar6[0x1d]) break;
      piVar2 = (int *)piVar4[1];
      piVar6 = piVar4;
    } while ((int *)piVar4[1] != (int *)0xffffffff);
  }
  *piVar5 = *piVar6;
  piVar5[1] = *(int *)(*piVar6 + 4);
  *(int **)(*piVar6 + 4) = piVar5;
  *piVar6 = (int)piVar5;
  if (param_2 == 0) {
    *(u32 *)(param_1 + 0x50) =
         *(u32 *)(param_1 + 0x50) & 0xfffffe0f |
         (((*(u32 *)(param_1 + 0x50) << 0x17) >> 0x1b) + 1 & 0x1f) << 4;
  }
  else {
    *(u32 *)(param_1 + 0x50) =
         *(u32 *)(param_1 + 0x50) & 0xffffc1ff |
         (((*(u32 *)(param_1 + 0x50) << 0x12) >> 0x1b) + 1 & 0x1f) << 9;
  }
  return piVar5;
}

// FUN_0201d170 @ 0x0201d170 (420 bytes) — Sprite entry deallocation
u32 FUN_0201d170(int param_1, short param_2)
{
  u32 uVar1;
  int *piVar2;

  piVar2 = (int *)(param_2 * 0xcc + *(int *)(param_1 + 0xf00));
  if (((u32)(*(int *)(param_1 + 0x50) << 0x17) >> 0x1b) +
      ((u32)(*(int *)(param_1 + 0x50) << 0x12) >> 0x1b) == 0) {
    return 0;
  }
  if (piVar2[0x1d] != 0) {
    piVar2[0x1d] = 0;
    piVar2[0x1a] = piVar2[0x1a] & 0x7fffffffU | 0x80000000;
    if ((u32)(piVar2[0x1b] << 0xc) >> 0x1d != 0) {
      piVar2[0x1b] = piVar2[0x1b] & 0xfff1ffff;
      if ((u32)piVar2[2] >> 0x12 != 0) {
        FUN_02027bb8((u16)((u32)piVar2[2] >> 0x12));
        FUN_02027bb8((int)((((u32)piVar2[2] >> 0x12) + 1) * 0x10000) >> 0x10);
      }
    }
    uVar1 = DAT_0201d314;
    if (piVar2[0x21] << 5 < 0) {
      piVar2[0x21] = piVar2[0x21] & 0xfbffffff;
      *(short *)(piVar2 + 0x22) = (short)uVar1;
      *(int *)(piVar2[0x1f] + 4) = piVar2[0x20];
      *(int *)piVar2[0x20] = piVar2[0x1f];
    }
    uVar1 = DAT_0201d314;
    if (piVar2[0x27] << 5 < 0) {
      piVar2[0x27] = piVar2[0x27] & 0xfbffffff;
      *(short *)(piVar2 + 0x28) = (short)uVar1;
      *(int *)(piVar2[0x25] + 4) = piVar2[0x26];
      *(int *)piVar2[0x26] = piVar2[0x25];
    }
    *(int *)(*piVar2 + 4) = piVar2[1];
    *(int *)piVar2[1] = *piVar2;
    if (piVar2[2] << 0x1f < 0) {
      *(u32 *)(param_1 + 0x50) =
           *(u32 *)(param_1 + 0x50) & 0xffffc1ff |
           (((*(u32 *)(param_1 + 0x50) << 0x12) >> 0x1b) - 1 & 0x1f) << 9;
    }
    else {
      *(u32 *)(param_1 + 0x50) =
           *(u32 *)(param_1 + 0x50) & 0xfffffe0f |
           (((*(u32 *)(param_1 + 0x50) << 0x17) >> 0x1b) - 1 & 0x1f) << 4;
    }
    return 1;
  }
  return 0;
}

// FUN_0201d3bc @ 0x0201d3bc (1528 bytes) — Sprite creation/setup
/* WARNING: Restarted to delay deadcode elimination for space: stack */
int FUN_0201d3bc(u32 param_1, u32 *param_2, u32 param_3)
{
  char cVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  u32 uVar5;
  u32 *puVar6;
  u8 *pbVar7;
  u32 uVar8;
  u32 local_30;

  uVar2 = (param_2[1] << 4) >> 0x16;
  cVar1 = *(char *)(DAT_0201d9b4 + 0x515);
  if (uVar2 == DAT_0201d9b8) {
    pbVar7 = (u8 *)param_2[5];
  }
  else {
    uVar5 = param_2[5];
    if (uVar5 == 0) {
      pbVar7 = (u8 *)0x0;
    }
    else {
      pbVar7 = (u8 *)(uVar5 + *(int *)(uVar5 + uVar2 * 4));
    }
  }
  if (pbVar7 == (u8 *)0x0) {
    uVar2 = ((*param_2 << 0x16) >> 0x1b) + 1;
    uVar5 = ((*param_2 << 0x11) >> 0x1b) + 1;
  }
  else if (uVar2 == DAT_0201d9b8) {
    if (((*param_2 << 0x16) >> 0x1b == 0) || (uVar5 = param_1, (*param_2 << 0x11) >> 0x1b == 0)) {
      uVar5 = FUN_0201fef8(param_1, pbVar7, param_2[4]);
      uVar5 = uVar5 & 0xffff;
    }
    uVar2 = (*param_2 << 0x16) >> 0x1b;
    if (uVar2 == 0) {
      uVar2 = (uVar5 & DAT_0201d9bc) + 1 & 0xffff;
    }
    else {
      uVar2 = uVar2 + 1;
    }
    uVar5 = (*param_2 << 0x11) >> 0x1b;
    if (uVar5 == 0) {
      uVar5 = 1;
    }
    else {
      uVar5 = uVar5 + 1;
    }
  }
  else {
    uVar2 = (*param_2 << 0x16) >> 0x1b;
    if (uVar2 == 0) {
      uVar2 = (u32)*pbVar7;
    }
    uVar2 = uVar2 + 1;
    uVar5 = (*param_2 << 0x11) >> 0x1b;
    if (uVar5 == 0) {
      uVar5 = (u32)pbVar7[1];
    }
    uVar5 = uVar5 + 1;
  }
  uVar5 = FUN_0201a960(param_1, (*param_2 << 0x1b) >> 0x1c, uVar2 & 0xff, uVar5 & 0xff);
  uVar2 = uVar5 & 0xffff;
  uVar5 = uVar5 >> 0x10;
  uVar8 = uVar5;
  local_30 = uVar2;
  if ((*param_2 << 0xe) >> 0x1d == 0) {
    if ((param_2[2] & 7) != 0) {
      local_30 = uVar2 + 1 & 0xffff;
    }
    if (((param_2[2] << 0xe) >> 0x17 & 7) != 0) {
      uVar8 = uVar5 + 1 & 0xffff;
    }
  }
  iVar3 = (int)FUN_0201ce50(param_1, *param_2 & 1, local_30 * uVar8 * 0x20, param_3);
  if (iVar3 == 0) {
    return -1;
  }
  puVar6 = (u32 *)(iVar3 + 8);
  if (param_2 < puVar6) {
    FUN_0202cd68((u32)param_2, (u32)puVar6, 0x30);
  }
  else {
    FUN_0202cc94((u32)param_2, (u32)puVar6, 0x30);
  }
  *puVar6 = *puVar6 & 0xfffffc1f | (uVar2 - 1 & 0x1f) << 5;
  *puVar6 = *puVar6 & 0xffff83ff | (uVar5 - 1 & 0x1f) << 10;
  *(u32 *)(iVar3 + 0x10) = *(u32 *)(iVar3 + 0x10) & 0xfe07ffff | (local_30 - 1 & 0x3f) << 0x13;
  *(u32 *)(iVar3 + 0x10) = *(u32 *)(iVar3 + 0x10) & 0x81ffffff | (uVar8 - 1 & 0x3f) << 0x19;
  *(u32 *)(iVar3 + 0x6c) = *(u32 *)(iVar3 + 0x6c) | 0x10000000;
  *(u32 *)(iVar3 + 0x6c) = *(u32 *)(iVar3 + 0x6c) & 0xfff1ffff;
  FUN_0201ccf4(param_1, iVar3, 0, 0);
  iVar4 = (short)(u16)((u32)(*(int *)(iVar3 + 0x68) << 1) >> 0x1b) * 0xcc +
          *(int *)(param_1 + 0xf00);
  *(u32 *)(iVar4 + 0x68) = *(u32 *)(iVar4 + 0x68) & 0xfffffffe | 1;
  *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 8;
  if (*(u32 *)(param_1 + 0x50) >> 0x1e != 0) {
    FUN_02019e9c(param_1, iVar3);
  }
  if (pbVar7 == (u8 *)0x0) goto LAB_0201d98c;
  FUN_0203b914(iVar3 + 0x54, 0, 0x10);
  iVar4 = DAT_0201d9c0;
  switch((*param_2 << 0x1b) >> 0x1c) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    *(char *)(iVar3 + 100) =
         (char)((int)((u32)**(u16 **)(DAT_0201d9c0 + ((*puVar6 << 0x1b) >> 0x1c) * 4) << 3) >> 1
               ) + '\x06';
    *(char *)(iVar3 + 0x65) =
         (char)((int)((u32)*(u16 *)(*(int *)(iVar4 + ((*puVar6 << 0x1b) >> 0x1c) * 4) + 2) << 3)
               >> 1) + '\x06';
    goto LAB_0201d84c;
  case 3:
    break;
  default:
    *(char *)(iVar3 + 100) =
         (char)((int)((u32)**(u16 **)(DAT_0201d9c0 + ((*puVar6 << 0x1b) >> 0x1c) * 4) << 3) >> 1
               );
    *(char *)(iVar3 + 0x65) =
         (char)((int)((u32)*(u16 *)(*(int *)(iVar4 + ((*puVar6 << 0x1b) >> 0x1c) * 4) + 2) << 3)
               >> 1);
    goto LAB_0201d84c;
  }
  *(char *)(iVar3 + 100) =
       (char)((int)((u32)**(u16 **)(DAT_0201d9c0 + ((*puVar6 << 0x1b) >> 0x1c) * 4) << 3) >> 1)
       + '\x04';
  *(char *)(iVar3 + 0x65) =
       (char)((int)((u32)*(u16 *)(*(int *)(iVar4 + ((*puVar6 << 0x1b) >> 0x1c) * 4) + 2) << 3)
             >> 1) + '\x02';
LAB_0201d84c:
  if ((*puVar6 << 0xe) >> 0x1d == 0) {
    *(u8 *)(iVar3 + 100) = *(char *)(iVar3 + 100) + ((u8)*(u32 *)(iVar3 + 0x10) & 7);
    *(u8 *)(iVar3 + 0x65) =
         *(char *)(iVar3 + 0x65) + ((u8)((u32)(*(int *)(iVar3 + 0x10) << 0xe) >> 0x17) & 7);
  }
  FUN_02026388(iVar3 + 0x24, *(u32 *)(iVar3 + 0x18));
  *(u16 *)(iVar3 + 0xc2) = 0;
  if (cVar1 != '\0') {
    FUN_0201dae4(param_1, iVar3, iVar3 + 0x24, 1);
  }
LAB_0201d98c:
  return (int)(short)(u16)((u32)(*(int *)(iVar3 + 0x68) << 1) >> 0x1b);
}

// FUN_0201d9c4 @ 0x0201d9c4 (288 bytes) — Sprite position setup
void FUN_0201d9c4(int param_1, int param_2, u32 param_3, u32 param_4, char param_5, u8 param_6,
                 int param_7)
{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  short sVar4;

  if ((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) {
    iVar1 = *(int *)(param_2 + 8) << 0x1f;
    if (iVar1 < 0) {
      sVar4 = *(short *)(param_1 + 0xf38);
    }
    else {
      sVar4 = *(short *)(param_1 + 0xf34);
    }
    param_3 = (int)((param_3 - (int)sVar4) * 0x10000) >> 0x10;
    if (iVar1 < 0) {
      sVar4 = *(short *)(param_1 + 0xf3a);
    }
    else {
      sVar4 = *(short *)(param_1 + 0xf36);
    }
    param_4 = (int)((param_4 - (int)sVar4) * 0x10000) >> 0x10;
  }
  iVar1 = -((*(int *)(param_2 + 8) << 0x1f) >> 0x1f);
  if (iVar1 == 0) {
    uVar3 = *(u32 *)(param_1 + 0x54) & 0xf;
    uVar2 = *(int *)(param_1 + 0x50) << 0xc;
  }
  else {
    uVar3 = (u32)(*(int *)(param_1 + 0x54) << 0x18) >> 0x1c;
    uVar2 = *(int *)(param_1 + 0x50) << 10;
  }
  FUN_02022048(*(u32 *)(param_1 + 0x28), iVar1, (int)param_5, param_3 & 0xffff, param_4 & 0xffff,
               param_6, uVar2 >> 0x1e, uVar3, 1, param_7);
  *(u32 *)(param_7 + 8) = *(u32 *)(param_7 + 8) & 0x7fffffff | 0x80000000;
  *(u16 *)(param_7 + 0xc) = (u16)((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b);
  return;
}

// FUN_0201dae4 @ 0x0201dae4 (484 bytes) — Sprite animation frame update
void FUN_0201dae4(u32 param_1, int param_2, int param_3, int param_4)
{
  u32 uVar1;
  u32 uVar2;
  u16 uVar3;
  u32 uVar4;

  if (*(short *)(param_2 + 0xc2) == 0) {
    uVar3 = FUN_02025f20(param_3);
    *(u16 *)(param_2 + 0xc2) = uVar3;
  }
  uVar2 = DAT_0201dccc;
  uVar1 = DAT_0201dcc8;
  uVar4 = (u32)(*(int *)(param_2 + 8) << 0x11) >> 0x1b;
  if (param_4 == 0) {
    *(u32 *)(param_3 + 8) =
         *(u32 *)(param_3 + 8) & DAT_0201dccc | (*(u8 *)(param_2 + 0x65) & DAT_0201dcc8) << 10;
  }
  else if (param_4 == 1) {
    *(u32 *)(param_3 + 8) =
         *(u32 *)(param_3 + 8) & DAT_0201dccc |
         (DAT_0201dcc8 & (int)((uVar4 + 1) * 8 - (*(u16 *)(param_2 + 0xc2) & 0xff)) >> 1) << 10;
    if ((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) {
      uVar4 = *(u32 *)(param_3 + 8);
      *(u32 *)(param_3 + 8) =
           uVar4 & uVar2 |
           (((u32)(*(int *)(param_2 + 0x10) << 0xe) >> 0x17 & 7) + ((uVar4 << 0xc) >> 0x16) & uVar1
           ) << 10;
    }
  }
  else if (param_4 == 2) {
    *(u32 *)(param_3 + 8) =
         *(u32 *)(param_3 + 8) & DAT_0201dccc |
         (DAT_0201dcc8 &
         ((uVar4 + 2) -
         (u32)*(u16 *)
                (*(int *)(DAT_0201dcd0 + ((u32)(*(int *)(param_2 + 8) << 0x1b) >> 0x1c) * 4) + 2))
         * 8) << 10;
    uVar4 = *(u32 *)(param_3 + 8);
    *(u32 *)(param_3 + 8) =
         uVar4 & uVar2 |
         (((uVar4 << 0xc) >> 0x16) -
          (((uVar4 << 4) >> 0x1c) * ((int)(u32)*(u16 *)(param_2 + 0xc2) >> 8) +
           (*(u16 *)(param_2 + 0xc2) & 0xff) + 1) & uVar1) << 10;
    if ((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) {
      uVar4 = *(u32 *)(param_3 + 8);
      *(u32 *)(param_3 + 8) =
           uVar4 & uVar2 |
           (((u32)(*(int *)(param_2 + 0x10) << 0xe) >> 0x17 & 7) + ((uVar4 << 0xc) >> 0x16) & uVar1
           ) << 10;
    }
  }
  if ((u32)(*(int *)(param_2 + 0x6c) << 5) >> 0x1b != 0) {
    return;
  }
  *(u32 *)(param_3 + 0x28) =
       *(u32 *)(param_3 + 0x28) & DAT_0201dcd4 |
       (DAT_0201dcc8 & (u32)(*(int *)(param_3 + 8) << 0xc) >> 0x16) << 0x12;
  return;
}

// FUN_0201dd34 @ 0x0201dd34 (2796 bytes) — Sprite command processor
/* WARNING: Removing unreachable block (ram,0x0201dd78) */
u32 FUN_0201dd34(int param_1, int param_2, int param_3, u32 param_4, u8 *param_5)
{
  u8 bVar1;
  char cVar2;
  int iVar3;
  u32 uVar4;
  int iVar5;
  int iVar6;
  char cVar7;
  int iVar8;
  u32 uVar9;
  u32 uVar10;
  int iVar11;
  u32 unaff_r7;
  u32 unaff_r9;
  u32 unaff_r10;
  u32 uVar12;
  int iVar13;
  BOOL bVar14;
  u32 local_50;

  uVar10 = param_4 >> 0x10;
  if (DAT_0201e820 < (int)uVar10) {
    uVar4 = DAT_0201e828;
    if ((int)uVar10 <= (int)DAT_0201e824) {
      uVar4 = DAT_0201e824;
    }
    if (uVar10 == uVar4) {
      return 1;
    }
    goto switchD_0201dd84_default;
  }
  if (DAT_0201e820 <= (int)uVar10) {
    return 1;
  }
  if (uVar10 == 0) {
    return 0;
  }
  switch(uVar10) {
  case 0xff00:
    *(u32 *)(param_2 + 0x6c) =
         *(u32 *)(param_2 + 0x6c) & 0xf83fffff |
         (((*(u32 *)(param_2 + 0x6c) << 5) >> 0x1b) + 1 & 0x1f) << 0x16;
    return 1;
  case 0xff01:
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xffefffff;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xbfffffff;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf7ffffff;
    *(u32 *)(param_2 + 0x68) = *(u32 *)(param_2 + 0x68) | 2;
    *(int *)(param_3 + 4) = *(int *)(param_3 + 4) + 1;
    return 0;
  case 0xff02:
    break;
  case 0xff03:
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) | 0x100000;
    FUN_0201dcd8();
    return 1;
  case 0xff04:
    uVar4 = *(u32 *)(param_2 + 0x10);
    cVar7 = (char)uVar4;
    uVar10 = (uVar4 << 0xe) >> 0x17;
    cVar2 = (char)uVar10;
    if ((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) {
      if ((uVar4 & 7) != 0) {
        cVar7 = cVar7 - (char)(uVar4 & 7);
      }
      uVar10 = uVar10 & 7;
      if (uVar10 != 0) {
        cVar2 = cVar2 - (char)uVar10;
      }
    }
    *(char *)(param_2 + (u32)*(u8 *)(param_2 + 0xc0) * 2 + 0xac) =
         cVar7 + (char)*(u32 *)(param_3 + 8) + '\b';
    *(char *)(param_2 + (u32)*(u8 *)(param_2 + 0xc0) * 2 + 0xad) =
         cVar2 + (char)((u32)(*(int *)(param_3 + 8) << 0xc) >> 0x16) + '\a';
    *(u8 *)(param_2 + 0xc1) = 0;
    *(char *)(param_2 + 0xc0) = *(char *)(param_2 + 0xc0) + '\x01';
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf7ffffff;
    return 1;
  case 0xff05:
    break;
  case 0xff06:
    break;
  case 0xff07:
    break;
  case 0xff08:
    break;
  case 0xff09:
    break;
  case 0xff0a:
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xffefffff;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xbfffffff;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf7ffffff;
    *(u32 *)(param_2 + 0x68) = *(u32 *)(param_2 + 0x68) | 4;
    return 0;
  case 0xff0b:
    FUN_0201dae4(0, 0, 0, 0);
    return 1;
  case 0xff0c:
    *(u8 *)(param_2 + 0x66) = *param_5;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xffefffff;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xbfffffff;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf7ffffff;
    return 0;
  case 0xff0d:
    break;
  case 0xff0e:
    break;
  case 0xff0f:
    FUN_0201a698(param_1, param_2, *param_5 - 1, 0);
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfffffffe | 1;
    return 0;
  case 0xff10:
    FUN_0201a698(param_1, param_2, *param_5 - 1, 1);
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfffffffe | 1;
    return 0;
  case 0xff11:
    if ((*(u32 *)(param_2 + 0x6c) << 0xc) >> 0x1d != 0) {
      *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfff1ffff;
      FUN_02027bb8(*(u32 *)(param_2 + 8) >> 0x12);
    }
    if (*(char *)(param_2 + 0xc0) != '\0') {
      unaff_r7 = 0xb;
      *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xc3ffffff | 0x4000000;
      unaff_r9 = (u32)*(u8 *)(param_2 + 0xac);
      unaff_r10 = (u32)*(u8 *)(param_2 + 0xad);
      goto switchD_0201e110_default;
    }
    if (*param_5 == 0) {
      *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xc3ffffff | 0x8000000;
      if ((u32)(*(int *)(param_2 + 8) << 0x1b) >> 0x1c == 3) {
        unaff_r7 = 0xe;
      }
      else {
        unaff_r7 = 0xc;
      }
    }
    else if (*param_5 == 1) {
      *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xc3ffffff | 0xc000000;
      if ((u32)(*(int *)(param_2 + 8) << 0x1b) >> 0x1c == 3) {
        unaff_r7 = 0xf;
      }
      else {
        unaff_r7 = 0xd;
      }
    }
    iVar5 = *(int *)(param_2 + 8);
    switch((u32)(iVar5 << 0x1b) >> 0x1c) {
    case 0:
      goto LAB_0201e128;
    case 1:
LAB_0201e128:
      unaff_r9 = (int)(((*(u32 *)(param_2 + 0x10) & 0x1ff) + ((u32)(iVar5 << 0x16) >> 0x1b) * 8 +
                       -2) * 0x10000) >> 0x10;
      unaff_r10 = (int)((((*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17) +
                         ((u32)(iVar5 << 0x11) >> 0x1b) * 8 + -2) * 0x10000) >> 0x10;
      break;
    case 2:
      unaff_r9 = (int)(((*(u32 *)(param_2 + 0x10) & 0x1ff) + ((u32)(iVar5 << 0x16) >> 0x1b) * 8 +
                       -7) * 0x10000) >> 0x10;
      unaff_r10 = (int)((((*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17) +
                         ((u32)(iVar5 << 0x11) >> 0x1b) * 8 + -7) * 0x10000) >> 0x10;
      break;
    case 3:
      unaff_r9 = (int)(((*(u32 *)(param_2 + 0x10) & 0x1ff) + ((u32)(iVar5 << 0x16) >> 0x1b) * 8 +
                       -4) * 0x10000) >> 0x10;
      unaff_r10 = (int)((((*(u32 *)(param_2 + 0x10) << 0xe) >> 0x17) +
                         ((u32)(iVar5 << 0x11) >> 0x1b) * 8 + -4) * 0x10000) >> 0x10;
    }
switchD_0201e110_default:
    FUN_0201d9c4(param_1, param_2, unaff_r9, unaff_r10, unaff_r7, 0, param_2 + 0x7c);
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xffefffff;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf7ffffff;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xbfffffff;
    return 0;
  case 0xff12:
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) | 0x40000000;
    *(u32 *)(param_2 + 0x68) = *(u32 *)(param_2 + 0x68) & 0xfffff80f | (*param_5 & 0x7f) << 4;
    return 1;
  case 0xff13:
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xbfffffff;
    return 1;
  case 0xff14:
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) | 0x200000;
    return 0;
  case 0xff15:
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xffdfffff;
    return 0;
  case 0xff16:
    break;
  case 0xff17:
    break;
  case 0xff18:
    break;
  case 0xff19:
    break;
  case 0xff1a:
    break;
  case 0xff1b:
    break;
  case 0xff1c:
    break;
  case 0xff1d:
    break;
  case 0xff1e:
    break;
  case 0xff1f:
    break;
  case 0xff20:
    if (*(int *)(param_2 + 100) << 1 < 0) {
      return 0;
    }
    if (*(u32 *)(param_1 + 0x50) >> 0x1e != 0) {
      FUN_02019b44();
      return 0;
    }
    return 0;
  }
switchD_0201dd84_default:
  uVar4 = *(u32 *)(param_2 + 0x6c);
  if ((((-1 < (int)(uVar4 << 4)) && (-1 < (int)(uVar4 << 0xb))) &&
      (*(u32 *)(param_2 + 8) >> 0x12 != 0)) && ((uVar4 << 0xc) >> 0x1d != 1)) {
    *(u32 *)(param_2 + 0x6c) = uVar4 & 0xfff1ffff | 0x20000;
    FUN_02027bb8((int)(((*(u32 *)(param_2 + 8) >> 0x12) + 1) * 0x10000) >> 0x10);
    FUN_02027cf4(*(u32 *)(param_2 + 8) >> 0x12, 0, 0xffffffff);
  }
  if (*(int *)(param_2 + 100) << 1 < 0) {
    bVar1 = *(u8 *)(param_2 + 0xc0);
    uVar4 = *(u32 *)(param_1 + 0x50) >> 0x1e;
    bVar14 = uVar4 == 0;
    if (bVar14) {
      uVar4 = DAT_0201e830;
    }
    uVar12 = uVar4;
    if (!bVar14) {
      uVar4 = DAT_0201e834;
      uVar12 = local_50;
    }
    local_50 = uVar12;
    if (!bVar14) {
      local_50 = uVar4;
    }
    iVar5 = *(int *)(param_3 + 0x24);
    iVar11 = (param_4 & 0xff) + 2;
    if (uVar10 < 0x100) {
      iVar8 = *(int *)(*(int *)(param_3 + 0x14) + (u32)*(u16 *)(param_3 + 0x1c) * 8);
    }
    else {
      iVar8 = *(int *)(*(int *)(param_3 + 0x14) + ((u32)*(u16 *)(param_3 + 0x1c) * 2 + 1) * 4);
    }
    iVar3 = *(int *)(param_2 + 0x68);
    uVar12 = *(u32 *)(iVar8 + iVar11 * 4);
    uVar4 = *(u32 *)(param_3 + 0xc);
    uVar9 = *(u32 *)(param_2 + 0x74);
    if (*(int *)(*(int *)(param_1 + 0x2c) + 4) == 0) {
      iVar13 = -1;
    }
    else {
      iVar6 = *(int *)(*(int *)(param_1 + 0x2c) + 0xc);
      iVar13 = -1;
      if (*(int *)(iVar6 + 4) != -1) {
        *(char *)(iVar6 + 0x1c) = (char)iVar5;
        *(char *)(iVar6 + 0x1d) = (char)((u32)(iVar5 << 0xc) >> 0x16);
        *(char *)(iVar6 + 0x23) = (char)(param_4 >> 8);
        *(u8 *)(iVar6 + 0x22) = (u8)(uVar4 >> 8) >> 1;
        *(u32 *)(iVar6 + 0x18) = uVar9;
        *(u32 *)(iVar6 + 0x14) = iVar8 + (uVar12 & 0xfffffffc);
        *(u32 *)(iVar6 + 0x2c) = *(u32 *)(iVar6 + 0x2c) & 0xfff0ffff | (uVar4 & 0xf) << 0x10;
        *(u8 *)(iVar6 + 0x1e) = (u8)((u32)(iVar3 << 0x15) >> 0x19);
        *(u16 *)(iVar6 + 0x2c) = (u16)bVar1 + (u16)((u32)(iVar3 << 1) >> 0x1b) * 0x100;
        *(u32 *)(iVar6 + 0x2c) = *(u32 *)(iVar6 + 0x2c) & 0xffbfffff | (u32)(1 < bVar1) << 0x16;
        *(u32 *)(iVar6 + 0xc) = local_50;
        *(int *)(iVar6 + 0x10) = param_1;
        iVar13 = FUN_020223e8();
      }
    }
    if (iVar13 == -1) {
      if (uVar10 < 0x100) {
        iVar5 = *(int *)(*(int *)(param_3 + 0x14) + (u32)*(u16 *)(param_3 + 0x1c) * 8);
      }
      else {
        iVar5 = *(int *)(*(int *)(param_3 + 0x14) + ((u32)*(u16 *)(param_3 + 0x1c) * 2 + 1) * 4);
      }
      uVar10 = *(u32 *)(param_1 + 0x50) >> 0x1e;
      bVar14 = uVar10 == 0;
      if (bVar14) {
        uVar10 = *(u32 *)(param_2 + 0x78);
      }
      if (!bVar14) {
        uVar10 = *(u32 *)(param_2 + 0x74);
      }
      FUN_0202609c(*(u32 *)(param_3 + 0x24) & 0xff,
                   (*(u32 *)(param_3 + 0x24) << 0xc) >> 0x16 & 0xff, (int)*(short *)(param_3 + 0x10)
                   , (int)*(short *)(param_3 + 0x12), (int)(param_4 & 0xffff) >> 8,
                   (u32)(*(int *)(param_3 + 0xc) << 0x10) >> 0x19, uVar10,
                   iVar5 + (*(u32 *)(iVar5 + iVar11 * 4) & 0xfffffffc),
                   *(u32 *)(param_3 + 0xc) & 0xf, 0);
      uVar4 = *(u32 *)(param_3 + 0x24);
      uVar10 = (uVar4 << 0xc) >> 0x16;
      if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
        uVar12 = (((u32)(*(int *)(param_2 + 0x10) << 7) >> 0x1a) + 1) * 0x80000 >> 0x10;
        iVar3 = ((int)(uVar10 & 0xff) >> 3) * uVar12 + (uVar4 & 0xff & DAT_0201e82c);
        iVar5 = (((int)(((uVar4 << 6) >> 0x1a) + (uVar4 & 7)) >> 3) + 1) * 0x20;
        iVar8 = (int)(short)((int)((uVar4 >> 0x1a) + (uVar10 & 7)) >> 3);
        uVar4 = *(int *)(param_2 + 0x74) + iVar3 * 4;
        iVar11 = uVar12 * 4;
        uVar10 = *(int *)(param_2 + 0x78) + iVar3 * 4;
        do {
          if (uVar10 < uVar4) {
            FUN_0202cd68(uVar10, uVar4, iVar5);
          }
          else {
            FUN_0202cc94(uVar10, uVar4, iVar5);
          }
          uVar4 = uVar4 + iVar11;
          uVar10 = uVar10 + iVar11;
          iVar8 = (iVar8 + -1) * 0x10000 >> 0x10;
        } while (-1 < iVar8);
      }
      iVar5 = (short)(u16)((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xcc +
              *(int *)(param_1 + 0xf00);
      *(u32 *)(iVar5 + 0x68) = *(u32 *)(iVar5 + 0x68) & 0xfffffffe | 1;
      *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 8;
    }
  }
  else {
    if (*(u32 *)(param_1 + 0x50) >> 0x1e != 0) {
      FUN_02019b44(param_1, param_2, param_3, uVar10);
    }
    uVar4 = *(u32 *)(param_3 + 0x24);
    uVar10 = (uVar4 << 0xc) >> 0x16;
    if (*(u32 *)(param_1 + 0x50) >> 0x1e == 0) {
      uVar12 = (((u32)(*(int *)(param_2 + 0x10) << 7) >> 0x1a) + 1) * 0x80000 >> 0x10;
      iVar3 = ((int)(uVar10 & 0xff) >> 3) * uVar12 + (uVar4 & 0xff & DAT_0201e82c);
      iVar5 = (((int)(((uVar4 << 6) >> 0x1a) + (uVar4 & 7)) >> 3) + 1) * 0x20;
      iVar8 = (int)(short)((int)((uVar4 >> 0x1a) + (uVar10 & 7)) >> 3);
      uVar4 = *(int *)(param_2 + 0x74) + iVar3 * 4;
      iVar11 = uVar12 * 4;
      uVar10 = *(int *)(param_2 + 0x78) + iVar3 * 4;
      do {
        if (uVar10 < uVar4) {
          FUN_0202cd68(uVar10, uVar4, iVar5);
        }
        else {
          FUN_0202cc94(uVar10, uVar4, iVar5);
        }
        uVar4 = uVar4 + iVar11;
        uVar10 = uVar10 + iVar11;
        iVar8 = (iVar8 + -1) * 0x10000 >> 0x10;
      } while (-1 < iVar8);
    }
    iVar5 = (short)(u16)((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0xcc +
            *(int *)(param_1 + 0xf00);
    *(u32 *)(iVar5 + 0x68) = *(u32 *)(iVar5 + 0x68) & 0xfffffffe | 1;
    *(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) | 8;
  }
  return 0;
}

// FUN_0201e838 @ 0x0201e838 (1700 bytes) — Sprite animation tick handler
/* WARNING: Restarted to delay deadcode elimination for space: stack */
void FUN_0201e838(int *param_1, int param_2)
{
  u16 uVar1;
  short sVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  int iVar6;
  u32 uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  void (*UNRECOVERED_JUMPTABLE_00)(void);
  u32 auStack_24[2];

  if ((int)(*(u32 *)(param_2 + 0x6c) << 3) < 0) {
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xefffffff;
    return;
  }
  if (*(int *)(param_2 + 100) < 0) {
    return;
  }
  iVar10 = param_2 + 0x24;
  if (*(char *)(param_2 + 0xc0) != '\0') {
    uVar1 = *(u16 *)(DAT_0201eedc + 2);
    if (((uVar1 & 0x10) == 0) && ((uVar1 & 0x80) == 0)) {
      if (((uVar1 & 0x20) != 0) || ((uVar1 & 0x40) != 0)) {
        FUN_0202277c(param_1[0xb],
                     *(u8 *)(param_2 + 0xc1) + 1 +
                     ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100, 1);
        if (*(char *)(param_2 + 0xc1) == '\0') {
          *(u8 *)(param_2 + 0xc1) = *(u8 *)(param_2 + 0xc0);
        }
        *(char *)(param_2 + 0xc1) = *(char *)(param_2 + 0xc1) + -1;
        FUN_0202281c(param_1[0xb],
                     *(u8 *)(param_2 + 0xc1) + 1 +
                     ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100);
        FUN_02027cf4(0xe7, 0, 0xffffffff);
      }
    }
    else {
      FUN_0202277c(param_1[0xb],
                   *(u8 *)(param_2 + 0xc1) + 1 +
                   ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100, 1);
      *(char *)(param_2 + 0xc1) = *(char *)(param_2 + 0xc1) + '\x01';
      if (*(char *)(param_2 + 0xc1) == *(char *)(param_2 + 0xc0)) {
        *(u8 *)(param_2 + 0xc1) = 0;
      }
      FUN_0202281c(param_1[0xb],
                   *(u8 *)(param_2 + 0xc1) + 1 +
                   ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100);
      FUN_02027cf4(0xe7, 0, 0xffffffff);
    }
    uVar7 = DAT_0201eee4;
    if (*(int *)(param_2 + 0x84) << 5 < 0) {
      iVar9 = param_2 + (u32)*(u8 *)(param_2 + 0xc1) * 2;
      uVar5 = (u32)*(u8 *)(iVar9 + 0xac);
      uVar4 = (u32)*(u8 *)(iVar9 + 0xad);
      if ((u32)(*(int *)(param_2 + 8) << 0xe) >> 0x1d == 0) {
        iVar9 = *(int *)(param_2 + 8) << 0x1f;
        if (iVar9 < 0) {
          sVar2 = (short)param_1[0x3ce];
        }
        else {
          sVar2 = (short)param_1[0x3cd];
        }
        iVar8 = (int)sVar2;
        iVar6 = uVar5 - iVar8;
        if (-1 < iVar9) {
          iVar8 = (int)*(short *)((int)param_1 + 0xf36);
        }
        uVar5 = iVar6 * 0x10000 >> 0x10;
        if (-1 >= iVar9) {
          iVar8 = (int)*(short *)((int)param_1 + 0xf3a);
        }
        uVar4 = (int)((uVar4 - iVar8) * 0x10000) >> 0x10;
      }
      *(u32 *)(param_2 + 0x84) =
           *(u32 *)(param_2 + 0x84) & DAT_0201eee0 | (uVar5 & 0xff & DAT_0201eee4) << 2;
      *(u32 *)(param_2 + 0x84) =
           *(u32 *)(param_2 + 0x84) & DAT_0201eee8 | (uVar4 & 0xff & uVar7) << 0xb;
    }
  }
  uVar7 = (u32)(*(int *)(param_2 + 100) << 2) >> 0x1c;
  if (uVar7 != 0) {
    if ((*(u16 *)(DAT_0201eedc + 2) & *(u16 *)(param_1 + 0x3cf)) == 0) {
      (*UNRECOVERED_JUMPTABLE_00)();
      return;
    }
    if (uVar7 == 1) {
      *(u8 *)(param_2 + 0xc0) = 0;
      uVar7 = (u32)*(u8 *)(param_2 + 0xc1);
    }
    else {
      uVar7 = 0xffffffff;
    }
    if (*(u32 **)(param_2 + 0x20) != (u32 *)0x0) {
      **(u32 **)(param_2 + 0x20) = uVar7;
    }
    uVar3 = DAT_0201eeec;
    if ((int)(*(u32 *)(param_2 + 0x84) << 5) < 0) {
      *(u32 *)(param_2 + 0x84) = *(u32 *)(param_2 + 0x84) & 0xfbffffff;
      *(short *)(param_2 + 0x88) = (short)uVar3;
      *(u32 *)(*(int *)(param_2 + 0x7c) + 4) = *(u32 *)(param_2 + 0x80);
      **(u32 **)(param_2 + 0x80) = *(u32 *)(param_2 + 0x7c);
    }
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xc3ffffff;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xfcffffff;
    *(u8 *)(param_2 + 0x66) = 0;
    return;
  }
  uVar7 = *(u32 *)(param_2 + 0x68);
  if ((int)(uVar7 << 0x1d) < 0) {
    *(u32 *)(param_2 + 0x68) = uVar7 & 0x7fffffff | 0x80000000;
    if ((*(u32 *)(param_2 + 0x6c) << 0xc) >> 0x1d != 0) {
      *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfff1ffff;
      if (*(u32 *)(param_2 + 8) >> 0x12 != 0) {
        FUN_02027bb8((u16)(*(u32 *)(param_2 + 8) >> 0x12));
        FUN_02027bb8((int)(((*(u32 *)(param_2 + 8) >> 0x12) + 1) * 0x10000) >> 0x10);
      }
    }
    if (*(int *)(param_2 + 0x10) << 0xd < 0) {
      (*UNRECOVERED_JUMPTABLE_00)();
      return;
    }
    *(u32 *)(param_2 + 0x68) = *(u32 *)(param_2 + 0x68) & 0xfffffffb;
    uVar7 = (u32)*(u8 *)(param_2 + 0xc0);
    do {
      FUN_02022868(param_1[0xb], 1,
                   uVar7 + ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100 & 0xffff);
      uVar7 = (int)((uVar7 - 1) * 0x10000) >> 0x10;
    } while (-1 < (int)uVar7);
    /* vtable call */
    (*(void (**)(int *, u32))(*(u32 *)((u32)*param_1 + 0x14)))(param_1, (u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b);
    return;
  }
  if ((int)(uVar7 << 0x1e) < 0) {
    /* vtable call */
    (*(void (**)(int *, u32))(*(u32 *)((u32)*param_1 + 0x18)))(param_1, (uVar7 << 1) >> 0x1b);
    *(u32 *)(param_2 + 0x68) = *(u32 *)(param_2 + 0x68) & 0xfffffffd;
    *(u32 *)(param_2 + 100) = *(u32 *)(param_2 + 100) & 0xfcffffff;
    *(u8 *)(param_2 + 0x66) = 0;
    uVar7 = DAT_0201eef0;
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xf83fffff;
    *(u32 *)(param_2 + 0x2c) =
         *(u32 *)(param_2 + 0x2c) & 0xfffffc00 | *(u8 *)(param_2 + 100) & uVar7;
    *(u32 *)(param_2 + 0x2c) =
         *(u32 *)(param_2 + 0x2c) & DAT_0201eef4 | (*(u8 *)(param_2 + 0x65) & uVar7) << 10;
    FUN_0202626c(iVar10);
    uVar7 = (u32)*(u8 *)(param_2 + 0xc0);
    do {
      FUN_02022868(param_1[0xb], 1,
                   uVar7 + ((u32)(*(int *)(param_2 + 0x68) << 1) >> 0x1b) * 0x100 & 0xffff);
      uVar7 = (int)((uVar7 - 1) * 0x10000) >> 0x10;
    } while (-1 < (int)uVar7);
    *(u8 *)(param_2 + 0xc0) = 0;
    if ((u32)param_1[0x14] >> 0x1e != 0) {
      FUN_02019e9c(param_1, param_2);
    }
    *(u16 *)(param_2 + 0xc2) = 0;
    if ((u32)(*(int *)(param_2 + 0x30) << 1) >> 0x18 != 0) {
      FUN_0201dae4((u32)param_1, param_2, iVar10, 1);
    }
  }
  if (((*(u16 *)(DAT_0201eedc + 2) & *(u16 *)(param_1 + 0x3cf)) != 0) &&
     (*(char *)(param_2 + 0x66) == '\0')) {
    *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) | 0x8000000;
  }
  if (-1 < *(int *)(param_2 + 0x6c) << 4) {
    uVar5 = *(u32 *)(param_2 + 100);
    uVar7 = (uVar5 << 6) >> 0x1e;
    if (uVar7 != 0) {
      *(u32 *)(param_2 + 100) = uVar5 & 0xfcffffff | (uVar7 - 1 & 3) << 0x18;
      return;
    }
    *(u32 *)(param_2 + 100) = uVar5 & 0xfcffffff | 0x1000000;
    if (*(char *)(param_2 + 0x66) != '\0') {
      uVar7 = (*(u32 *)(param_2 + 0x6c) << 0xc) >> 0x1d;
      if ((uVar7 != 0) && (uVar7 != 3)) {
        *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfff1ffff | 0x40000;
        FUN_02027bb8(*(u32 *)(param_2 + 8) >> 0x12);
      }
      *(char *)(param_2 + 0x66) = *(char *)(param_2 + 0x66) + -1;
      return;
    }
  }
LAB_0201ee14:
  do {
    if (*(int *)(param_2 + 0x6c) << 0x1f < 0) {
      iVar9 = FUN_0201a248(param_1, param_2);
      if (iVar9 != 0) {
        *(u32 *)(param_2 + 0x6c) = *(u32 *)(param_2 + 0x6c) & 0xfffffffe;
      }
    }
    else {
      FUN_02025810(auStack_24, iVar10, (u32)(*(int *)(param_2 + 100) << 1) >> 0x1f, 0);
      iVar9 = FUN_0201dd34((int)param_1, param_2, iVar10, auStack_24[0], (u8 *)0);
      if (iVar9 != 0) goto LAB_0201ee14;
    }
    iVar9 = *(int *)(param_2 + 0x6c);
    if ((((-1 < iVar9 << 0xb) && (-1 < iVar9 << 10)) && (-1 < *(int *)(param_2 + 100) << 1)) &&
       (-1 < iVar9 << 4)) {
      return;
    }
  } while( TRUE );
}

// FUN_0201eef8 @ 0x0201eef8 (340 bytes) — Sprite VRAM copy
void FUN_0201eef8(int param_1)
{
  int iVar1;
  u32 uVar2;
  u16 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  int iVar9;

  uVar3 = *(u16 *)(param_1 + 0x2c);
  iVar4 = *(int *)(param_1 + 0x10);
  iVar6 = (short)(uVar3 >> 8) * 0xcc + *(int *)(iVar4 + 0xf00);
  *(u32 *)(iVar6 + 0x68) = *(u32 *)(iVar6 + 0x68) & 0xfffffffe | 1;
  *(u32 *)(iVar4 + 0x50) = *(u32 *)(iVar4 + 0x50) | 8;
  uVar8 = ((u32)*(u8 *)(param_1 + 0x1d) + (int)*(short *)(*(int *)(param_1 + 8) + 8)) -
          (int)*(short *)(param_1 + 0x2a);
  uVar7 = ((u32)*(u8 *)(param_1 + 0x1c) + (int)*(short *)(*(int *)(param_1 + 8) + 6)) -
          (int)*(short *)(param_1 + 0x28);
  iVar6 = ((int)(u32)uVar3 >> 8) * 0xcc + *(int *)(iVar4 + 0xf00);
  if (*(u32 *)(iVar4 + 0x50) >> 0x1e == 0) {
    uVar2 = (((u32)(*(int *)(iVar6 + 0x10) << 7) >> 0x1a) + 1) * 0x80000 >> 0x10;
    iVar5 = ((int)(uVar8 & 0xff) >> 3) * uVar2 + (uVar7 & 0xff & DAT_0201f04c);
    iVar4 = (((int)((u32)*(u8 *)(param_1 + 0x20) + (uVar7 & 7)) >> 3) + 1) * 0x20;
    iVar9 = (int)(short)((int)((u32)*(u8 *)(param_1 + 0x21) + (uVar8 & 7)) >> 3);
    uVar7 = *(int *)(iVar6 + 0x74) + iVar5 * 4;
    iVar1 = uVar2 * 4;
    uVar8 = *(int *)(iVar6 + 0x78) + iVar5 * 4;
    do {
      if (uVar8 < uVar7) {
        FUN_0202cd68(uVar8, uVar7, iVar4);
      }
      else {
        FUN_0202cc94(uVar8, uVar7, iVar4);
      }
      uVar7 = uVar7 + iVar1;
      uVar8 = uVar8 + iVar1;
      iVar9 = (iVar9 + -1) * 0x10000 >> 0x10;
    } while (-1 < iVar9);
    return;
  }
  return;
}

// FUN_0201f050 @ 0x0201f050 (144 bytes) — Sprite OAM finalize
void FUN_0201f050(int param_1)
{
  int iVar1;
  int iVar2;
  int *piVar3;

  piVar3 = *(int **)(param_1 + 0x10);
  iVar1 = (int)(u32)*(u16 *)(param_1 + 0x2c) >> 8;
  iVar2 = (short)(*(u16 *)(param_1 + 0x2c) >> 8) * 0xcc;
  if (-1 < *(int *)(piVar3[0x3c0] + iVar2 + 0x68) << 0x1c) {
    /* vtable call */
    (*(void (**)(int *, int))(*(u32 *)((u32)*piVar3 + 0x18)))(piVar3, iVar1);
    FUN_02019888(piVar3, iVar1 * 0xcc + piVar3[0x3c0]);
    iVar2 = piVar3[0x3c0] + iVar2;
    *(u32 *)(iVar2 + 0x68) = *(u32 *)(iVar2 + 0x68) & 0xfffffffe | 1;
    piVar3[0x14] = piVar3[0x14] | 8;
    return;
  }
  return;
}

// FUN_0201f0e0 @ 0x0201f0e0 (256 bytes) — Sprite scroll offset update
void FUN_0201f0e0(int param_1, int param_2)
{
  u32 uVar1;
  int iVar2;
  u32 uVar3;
  int iVar4;
  BOOL bVar5;

  uVar1 = DAT_0201f1e4;
  uVar3 = *(u32 *)(param_2 + 8);
  if (-1 < (int)(uVar3 << 5)) {
    return;
  }
  if ((int)uVar3 < 0) {
    *(u32 *)(param_2 + 8) = uVar3 & 0x7fffffff;
    return;
  }
  iVar4 = 0;
  iVar2 = 0;
  if ((int)(uVar3 << 0x1f) < 0) {
    bVar5 = (int)*(short *)(param_1 + 0xf38) != (int)*(short *)(param_1 + 0xf30);
    if (bVar5) {
      iVar4 = (int)*(short *)(param_1 + 0xf30) - (int)*(short *)(param_1 + 0xf38);
    }
    if (bVar5) {
      iVar4 = iVar4 << 0x10;
    }
    if (bVar5) {
      iVar4 = iVar4 >> 0x10;
    }
    if ((int)*(short *)(param_1 + 0xf3a) != (int)*(short *)(param_1 + 0xf32)) {
      iVar2 = ((int)*(short *)(param_1 + 0xf32) - (int)*(short *)(param_1 + 0xf3a)) * 0x10000 >>
              0x10;
    }
  }
  else {
    bVar5 = (int)*(short *)(param_1 + 0xf34) != (int)*(short *)(param_1 + 0xf2c);
    if (bVar5) {
      iVar4 = (int)*(short *)(param_1 + 0xf2c) - (int)*(short *)(param_1 + 0xf34);
    }
    if (bVar5) {
      iVar4 = iVar4 << 0x10;
    }
    if (bVar5) {
      iVar4 = iVar4 >> 0x10;
    }
    if ((int)*(short *)(param_1 + 0xf36) != (int)*(short *)(param_1 + 0xf2e)) {
      iVar2 = ((int)*(short *)(param_1 + 0xf2e) - (int)*(short *)(param_1 + 0xf36)) * 0x10000 >>
              0x10;
    }
  }
  *(u32 *)(param_2 + 8) =
       *(u32 *)(param_2 + 8) & DAT_0201f1e0 |
       (iVar4 + ((*(u32 *)(param_2 + 8) << 0x15) >> 0x17) & DAT_0201f1e4) << 2;
  *(u32 *)(param_2 + 8) =
       *(u32 *)(param_2 + 8) & DAT_0201f1e8 |
       (iVar2 + ((*(u32 *)(param_2 + 8) << 0xc) >> 0x17) & uVar1) << 0xb;
  return;
}

// FUN_0201f1ec @ 0x0201f1ec (372 bytes) — Process all sprite entries
void FUN_0201f1ec(int param_1)
{
  int iVar1;

  iVar1 = *(int *)(param_1 + 0xf10);
  if (*(int *)(iVar1 + 4) != -1) {
    do {
      *(u32 *)(iVar1 + 0x68) = *(u32 *)(iVar1 + 0x68) & 0xfffffff7;
      *(u32 *)(param_1 + 0x54) =
           *(u32 *)(param_1 + 0x54) & 0x7ffffff | (*(u32 *)(iVar1 + 0x68) & 0x7c000000) << 1;
      if (*(int *)(iVar1 + 0x28) != 0) {
        FUN_0201e838((int *)param_1, iVar1);
      }
      if ((u32)(*(int *)(iVar1 + 8) << 0xe) >> 0x1d == 0) {
        FUN_0201f0e0(param_1, iVar1 + 0x7c);
        FUN_0201f0e0(param_1, iVar1 + 0x94);
      }
      iVar1 = *(int *)(iVar1 + 4);
    } while (*(int *)(iVar1 + 4) != -1);
  }
  iVar1 = *(int *)(param_1 + 0xf20);
  if (*(int *)(iVar1 + 4) != -1) {
    do {
      *(u32 *)(iVar1 + 0x68) = *(u32 *)(iVar1 + 0x68) & 0xfffffff7;
      *(u32 *)(param_1 + 0x54) =
           *(u32 *)(param_1 + 0x54) & 0x7ffffff | (*(u32 *)(iVar1 + 0x68) & 0x7c000000) << 1;
      if (*(int *)(iVar1 + 0x28) != 0) {
        FUN_0201e838((int *)param_1, iVar1);
      }
      if ((u32)(*(int *)(iVar1 + 8) << 0xe) >> 0x1d == 0) {
        FUN_0201f0e0(param_1, iVar1 + 0x7c);
        FUN_0201f0e0(param_1, iVar1 + 0x94);
      }
      iVar1 = *(int *)(iVar1 + 4);
    } while (*(int *)(iVar1 + 4) != -1);
  }
  FUN_02022564(*(u32 *)(param_1 + 0x2c));
  FUN_02022200(*(u32 *)(param_1 + 0x28));
  *(u16 *)(param_1 + 0xf2c) = *(u16 *)(param_1 + 0xf34);
  *(u16 *)(param_1 + 0xf2e) = *(u16 *)(param_1 + 0xf36);
  *(u16 *)(param_1 + 0xf30) = *(u16 *)(param_1 + 0xf38);
  *(u16 *)(param_1 + 0xf32) = *(u16 *)(param_1 + 0xf3a);
  return;
}

// FUN_0201f360 @ 0x0201f360 (188 bytes) — Scene destructor (no free)
u32 * FUN_0201f360(u32 *param_1)
{
  int iVar1;

  *param_1 = DAT_0201f41c;
  FUN_02029e20(param_1[0x3d0]);
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x18);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffeffff;
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x30);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffdffff;
  FUN_020200f4(param_1);
  iVar1 = param_1[10];
  if ((iVar1 != 0) && (iVar1 != 0)) {
    FUN_02021c34(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  iVar1 = param_1[0xb];
  if (iVar1 != 0) {
    FUN_020222d8(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  if (param_1[0xc] != 0) {
    thunk_FUN_02029ab8();
  }
  FUN_0202a5a8(param_1);
  return param_1;
}

// FUN_0201f420 @ 0x0201f420 (196 bytes) — Scene destructor (with free)
u32 * FUN_0201f420(u32 *param_1)
{
  int iVar1;

  *param_1 = DAT_0201f4e4;
  FUN_02029e20(param_1[0x3d0]);
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x18);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffeffff;
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x30);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffdffff;
  FUN_020200f4(param_1);
  iVar1 = param_1[10];
  if ((iVar1 != 0) && (iVar1 != 0)) {
    FUN_02021c34(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  iVar1 = param_1[0xb];
  if (iVar1 != 0) {
    FUN_020222d8(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  if (param_1[0xc] != 0) {
    thunk_FUN_02029ab8();
  }
  FUN_0202a5a8(param_1);
  thunk_FUN_02029ab8(param_1);
  return param_1;
}

// FUN_0201f4e8 @ 0x0201f4e8 (188 bytes) — Scene destructor variant
u32 * FUN_0201f4e8(u32 *param_1)
{
  int iVar1;

  *param_1 = DAT_0201f5a4;
  FUN_02029e20(param_1[0x3d0]);
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x18);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffeffff;
  iVar1 = param_1[10];
  FUN_020084a0(iVar1 + 0x30);
  *(u32 *)(iVar1 + 0x14) = *(u32 *)(iVar1 + 0x14) & 0xfffdffff;
  FUN_020200f4(param_1);
  iVar1 = param_1[10];
  if ((iVar1 != 0) && (iVar1 != 0)) {
    FUN_02021c34(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  iVar1 = param_1[0xb];
  if (iVar1 != 0) {
    FUN_020222d8(iVar1);
    thunk_FUN_02029ab8(iVar1);
  }
  if (param_1[0xc] != 0) {
    thunk_FUN_02029ab8();
  }
  FUN_0202a5a8(param_1);
  return param_1;
}

// FUN_0201f5a8 @ 0x0201f5a8 (1128 bytes) — Scene constructor
u32 *
FUN_0201f5a8(u32 *param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5,
            u8 param_6, u8 param_7, int *param_8, int *param_9, int param_10, int param_11)
{
  u32 uVar1;
  int iVar2;
  int iVar3;
  u32 uVar4;
  u16 uVar5;
  int iVar6;
  int iVar7;

  FUN_0202a74c();
  uVar4 = DAT_0201fa14;
  *param_1 = DAT_0201fa10;
  uVar1 = DAT_0201fa18;
  param_1[0x16] = uVar4;
  param_1[0x17] = uVar1;
  param_1[0x14] = param_1[0x14] & 0xffff3fff | (param_4 & 3) << 0xe;
  param_1[0x14] = param_1[0x14] & 0xfffcffff | (param_5 & 3) << 0x10;
  param_1[0x14] = param_1[0x14] & 0xfff3ffff | (param_6 & 3) << 0x12;
  param_1[0x14] = param_1[0x14] & 0xffcfffff | (param_7 & 3) << 0x14;
  if (param_10 == 1) {
    FUN_0201b310(param_1,9,0);
    FUN_0201b208(param_1,9,0);
  }
  iVar3 = 0;
  iVar7 = 0;
  if (param_8 == (int *)0x0) {
    param_1[0x11] = 0x4800;
    param_1[0x12] = 0x4800;
    iVar3 = 0x9000;
  }
  else {
    param_1[0xf] = *param_8;
    param_1[0x10] = param_8[1];
    if (*param_8 == 0) {
      iVar6 = 0;
    }
    else {
      iVar6 = param_8[2];
    }
    param_1[0x11] = iVar6;
    if (param_8[1] == 0) {
      iVar6 = 0;
    }
    else {
      iVar6 = param_8[3];
    }
    param_1[0x12] = iVar6;
  }
  if (param_9 == (int *)0x0) {
    *(u16 *)(param_1 + 0x13) = 0x780;
    *(u16 *)((int)param_1 + 0x4e) = 0x780;
    iVar7 = 0xf00;
  }
  else {
    param_1[0xd] = *param_9;
    param_1[0xe] = param_9[1];
    if (*param_9 == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = (u16)param_9[2];
    }
    *(u16 *)(param_1 + 0x13) = uVar5;
    if (param_9[1] == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = *(u16 *)((int)param_9 + 10);
    }
    *(u16 *)((int)param_1 + 0x4e) = uVar5;
  }
  param_1[0xc] = 0;
  if (iVar3 + iVar7 != 0) {
    uVar4 = FUN_02029bf8(iVar3 + iVar7,0,DAT_0201fa1c,0);
    param_1[0xc] = uVar4;
  }
  if (param_8 == (int *)0x0) {
    param_1[0xf] = param_1[0xc] + iVar7;
    param_1[0x10] = param_1[0xf] + 0x4800;
  }
  if (param_9 == (int *)0x0) {
    param_1[0xd] = param_1[0xc];
    param_1[0xe] = param_1[0xc] + 0x780;
    FUN_0202cc10(param_1[0xd],0,0x780);
    FUN_0202cc10(param_1[0xe],0,0x780);
  }
  else {
    if (*param_9 != 0) {
      FUN_0202cc10(param_1[0xd],0,0x780);
    }
    if (param_9[1] != 0) {
      FUN_0202cc10(param_1[0xe],0,0x780);
    }
  }
  iVar3 = FUN_02029c1c(0x28,0,DAT_0201fa20,0);
  uVar4 = 0;
  if (iVar3 != 0) {
    uVar4 = FUN_020222f4(iVar3,0x1e);
  }
  param_1[0xb] = uVar4;
  param_1[10] = param_11;
  if (param_11 == 0) {
    iVar3 = FUN_02029c1c(0x6c,0,DAT_0201fa24,0);
    uVar4 = 0;
    if (iVar3 != 0) {
      uVar4 = FUN_02021c64();
    }
    param_1[10] = uVar4;
  }
  iVar3 = FUN_02029c1c(0x28,0,DAT_0201fa28,0);
  uVar4 = 0;
  if (iVar3 != 0) {
    uVar4 = FUN_020197e4(iVar3,8,DAT_0201fa2c,param_1);
  }
  param_1[0x3d0] = uVar4;
  param_1[0x14] = param_1[0x14] & 0xfffffffe | 1;
  iVar3 = DAT_0201fa30;
  param_1[0x15] = param_1[0x15] & 0xfdffffff;
  iVar7 = DAT_0201fa34;
  param_1[0x15] = param_1[0x15] & 0xfbffffff;
  iVar6 = DAT_0201fa38;
  param_1[0x14] = param_1[0x14] & 0xfffffffd;
  iVar2 = DAT_0201fa3c;
  param_1[0x14] = param_1[0x14] & 0xfffffffb;
  param_1[0x14] = param_1[0x14] & 0xfffffff7;
  uVar4 = DAT_0201fa40;
  param_1[0x14] = param_1[0x14] & 0xfffffe0f;
  param_1[0x14] = param_1[0x14] & 0xffffc1ff;
  *(u16 *)(param_1 + 0x3cd) = 0;
  *(u16 *)((int)param_1 + 0xf36) = 0;
  *(u16 *)(param_1 + 0x3ce) = 0;
  *(u16 *)((int)param_1 + 0xf3a) = 0;
  *(u16 *)(param_1 + 0x3cb) = 0;
  *(u16 *)((int)param_1 + 0xf2e) = 0;
  *(u16 *)(param_1 + 0x3cc) = 0;
  *(u16 *)((int)param_1 + 0xf32) = 0;
  param_1[0x3c0] = 0;
  *(u16 *)(param_1 + 0x3c2) = 0;
  *(short *)(param_1 + 0x3cf) = (short)uVar4;
  *(u16 *)((int)param_1 + 0xf3e) = 2;
  uVar4 = DAT_0201fa44;
  param_1[0x15] = param_1[0x15] & 0xfffffff0;
  param_1[0x15] = param_1[0x15] & 0xffffff0f;
  param_1[0x14] = param_1[0x14] & 0xfc3fffff | 0x3c00000;
  param_1[0x14] = param_1[0x14] & 0xc3ffffff | 0x3c000000;
  param_1[0x3c3] = uVar4;
  param_1[0x3c4] = (int)param_1 + iVar3;
  param_1[0x3c5] = (int)param_1 + iVar7;
  param_1[0x3c6] = 0xffffffff;
  param_1[0x3c7] = uVar4;
  param_1[0x3c8] = (int)param_1 + iVar6;
  param_1[0x3c9] = (int)param_1 + iVar2;
  param_1[0x3ca] = 0xffffffff;
  FUN_0202605c(0xb,1,0,0);
  FUN_0202605c(0xc,1,0,0);
  FUN_0202605c(0xf,1,DAT_0201fa48,param_1);
  FUN_0202605c(0x10,1,DAT_0201fa4c,param_1);
  FUN_0202605c(0x11,1,0,0);
  FUN_0202605c(0x12,1,0,0);
  FUN_02029ffc(param_1[0x3d0]);
  return param_1;
}

// FUN_0201fa50 @ 0x0201fa50 (1128 bytes) — Scene constructor variant
u32 *
FUN_0201fa50(u32 *param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5,
            u8 param_6, u8 param_7, int *param_8, int *param_9, int param_10, int param_11)
{
  u32 uVar1;
  int iVar2;
  int iVar3;
  u32 uVar4;
  u16 uVar5;
  int iVar6;
  int iVar7;

  FUN_0202a74c();
  uVar4 = DAT_0201febc;
  *param_1 = DAT_0201feb8;
  uVar1 = DAT_0201fec0;
  param_1[0x16] = uVar4;
  param_1[0x17] = uVar1;
  param_1[0x14] = param_1[0x14] & 0xffff3fff | (param_4 & 3) << 0xe;
  param_1[0x14] = param_1[0x14] & 0xfffcffff | (param_5 & 3) << 0x10;
  param_1[0x14] = param_1[0x14] & 0xfff3ffff | (param_6 & 3) << 0x12;
  param_1[0x14] = param_1[0x14] & 0xffcfffff | (param_7 & 3) << 0x14;
  if (param_10 == 1) {
    FUN_0201b310(param_1,9,0);
    FUN_0201b208(param_1,9,0);
  }
  iVar3 = 0;
  iVar7 = 0;
  if (param_8 == (int *)0x0) {
    param_1[0x11] = 0x4800;
    param_1[0x12] = 0x4800;
    iVar3 = 0x9000;
  }
  else {
    param_1[0xf] = *param_8;
    param_1[0x10] = param_8[1];
    if (*param_8 == 0) {
      iVar6 = 0;
    }
    else {
      iVar6 = param_8[2];
    }
    param_1[0x11] = iVar6;
    if (param_8[1] == 0) {
      iVar6 = 0;
    }
    else {
      iVar6 = param_8[3];
    }
    param_1[0x12] = iVar6;
  }
  if (param_9 == (int *)0x0) {
    *(u16 *)(param_1 + 0x13) = 0x780;
    *(u16 *)((int)param_1 + 0x4e) = 0x780;
    iVar7 = 0xf00;
  }
  else {
    param_1[0xd] = *param_9;
    param_1[0xe] = param_9[1];
    if (*param_9 == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = (u16)param_9[2];
    }
    *(u16 *)(param_1 + 0x13) = uVar5;
    if (param_9[1] == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = *(u16 *)((int)param_9 + 10);
    }
    *(u16 *)((int)param_1 + 0x4e) = uVar5;
  }
  param_1[0xc] = 0;
  if (iVar3 + iVar7 != 0) {
    uVar4 = FUN_02029bf8(iVar3 + iVar7,0,DAT_0201fec4,0);
    param_1[0xc] = uVar4;
  }
  if (param_8 == (int *)0x0) {
    param_1[0xf] = param_1[0xc] + iVar7;
    param_1[0x10] = param_1[0xf] + 0x4800;
  }
  if (param_9 == (int *)0x0) {
    param_1[0xd] = param_1[0xc];
    param_1[0xe] = param_1[0xc] + 0x780;
    FUN_0202cc10(param_1[0xd],0,0x780);
    FUN_0202cc10(param_1[0xe],0,0x780);
  }
  else {
    if (*param_9 != 0) {
      FUN_0202cc10(param_1[0xd],0,0x780);
    }
    if (param_9[1] != 0) {
      FUN_0202cc10(param_1[0xe],0,0x780);
    }
  }
  iVar3 = FUN_02029c1c(0x28,0,DAT_0201fec8,0);
  uVar4 = 0;
  if (iVar3 != 0) {
    uVar4 = FUN_020222f4(iVar3,0x1e);
  }
  param_1[0xb] = uVar4;
  param_1[10] = param_11;
  if (param_11 == 0) {
    iVar3 = FUN_02029c1c(0x6c,0,DAT_0201fecc,0);
    uVar4 = 0;
    if (iVar3 != 0) {
      uVar4 = FUN_02021c64();
    }
    param_1[10] = uVar4;
  }
  iVar3 = FUN_02029c1c(0x28,0,DAT_0201fed0,0);
  uVar4 = 0;
  if (iVar3 != 0) {
    uVar4 = FUN_020197e4(iVar3,8,DAT_0201fed4,param_1);
  }
  param_1[0x3d0] = uVar4;
  param_1[0x14] = param_1[0x14] & 0xfffffffe | 1;
  iVar3 = DAT_0201fed8;
  param_1[0x15] = param_1[0x15] & 0xfdffffff;
  iVar7 = DAT_0201fedc;
  param_1[0x15] = param_1[0x15] & 0xfbffffff;
  iVar6 = DAT_0201fee0;
  param_1[0x14] = param_1[0x14] & 0xfffffffd;
  iVar2 = DAT_0201fee4;
  param_1[0x14] = param_1[0x14] & 0xfffffffb;
  param_1[0x14] = param_1[0x14] & 0xfffffff7;
  uVar4 = DAT_0201fee8;
  param_1[0x14] = param_1[0x14] & 0xfffffe0f;
  param_1[0x14] = param_1[0x14] & 0xffffc1ff;
  *(u16 *)(param_1 + 0x3cd) = 0;
  *(u16 *)((int)param_1 + 0xf36) = 0;
  *(u16 *)(param_1 + 0x3ce) = 0;
  *(u16 *)((int)param_1 + 0xf3a) = 0;
  *(u16 *)(param_1 + 0x3cb) = 0;
  *(u16 *)((int)param_1 + 0xf2e) = 0;
  *(u16 *)(param_1 + 0x3cc) = 0;
  *(u16 *)((int)param_1 + 0xf32) = 0;
  param_1[0x3c0] = 0;
  *(u16 *)(param_1 + 0x3c2) = 0;
  *(short *)(param_1 + 0x3cf) = (short)uVar4;
  *(u16 *)((int)param_1 + 0xf3e) = 2;
  uVar4 = DAT_0201feec;
  param_1[0x15] = param_1[0x15] & 0xfffffff0;
  param_1[0x15] = param_1[0x15] & 0xffffff0f;
  param_1[0x14] = param_1[0x14] & 0xfc3fffff | 0x3c00000;
  param_1[0x14] = param_1[0x14] & 0xc3ffffff | 0x3c000000;
  param_1[0x3c3] = uVar4;
  param_1[0x3c4] = (int)param_1 + iVar3;
  param_1[0x3c5] = (int)param_1 + iVar7;
  param_1[0x3c6] = 0xffffffff;
  param_1[0x3c7] = uVar4;
  param_1[0x3c8] = (int)param_1 + iVar6;
  param_1[0x3c9] = (int)param_1 + iVar2;
  param_1[0x3ca] = 0xffffffff;
  FUN_0202605c(0xb,1,0,0);
  FUN_0202605c(0xc,1,0,0);
  FUN_0202605c(0xf,1,DAT_0201fef0,param_1);
  FUN_0202605c(0x10,1,DAT_0201fef4,param_1);
  FUN_0202605c(0x11,1,0,0);
  FUN_0202605c(0x12,1,0,0);
  FUN_02029ffc(param_1[0x3d0]);
  return param_1;
}

// FUN_0201fef8 @ 0x0201fef8 (144 bytes) — Sprite size query
int FUN_0201fef8(u32 param_1, u8 *param_2, u32 param_3)
{
  u32 uVar1;
  u32 uVar2;
  u16 local_8;
  u16 local_6;

  FUN_0201d318(0, 0);
  uVar1 = (u32)(u16)(local_8 + 0x18U);
  if ((local_8 + 0x18U & 1) != 0) {
    uVar1 = uVar1 + 1 & 0xffff;
  }
  uVar2 = (u32)(u16)(local_6 + 0x10U);
  if ((local_6 + 0x10U & 1) != 0) {
    uVar2 = uVar2 + 1 & 0xffff;
  }
  return (((int)uVar1 >> 3) - 1U & 0xffff) + (((int)uVar2 >> 3) + -1) * 0x10000;
}

// FUN_0201ff8c @ 0x0201ff8c (124 bytes) — Sprite OAM enable
void FUN_0201ff8c(int param_1, int param_2)
{
  int iVar1;

  iVar1 = (short)param_2 * 0xcc + *(int *)(param_1 + 0xf00);
  *(u32 *)(iVar1 + 100) = *(u32 *)(iVar1 + 100) & 0x7fffffff;
  FUN_0202281c(*(u32 *)(param_1 + 0x2c), (u32)(param_2 << 0x18) >> 0x10);
  FUN_0202281c(*(u32 *)(param_1 + 0x2c), param_2 * 0x100 + 0xffU & 0xffff);
  if (*(char *)(iVar1 + 0xc0) != '\0') {
    FUN_0202281c(*(u32 *)(param_1 + 0x2c),
                 param_2 * 0x100 + *(u8 *)(iVar1 + 0xc1) + 1 & 0xffff);
    return;
  }
  return;
}

