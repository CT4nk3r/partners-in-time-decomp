/**
 * Map Control - tilemap management, scrolling, layer configuration
 *
 * Handles tilemap layer linked lists, curve interpolation for scroll
 * effects, sprite transform scripting, and layer animation state updates.
 *
 * Decompiled from 0x02016254-0x02018ed0 region (27 functions)
 */
#include "types.h"

/* Forward declarations - external functions */
extern void FUN_0202cc10(void *, u32, u32);
extern void FUN_0202cd68(u32, u32, u32);
extern void FUN_0202cc94(u32, u32, u32);
extern int FUN_020464b0(int, int);
extern int FUN_02018194(s16 *, int);
extern void FUN_02015ff4(int, int, int *, int *);
extern void FUN_02034274(int *, int *, int *);
extern void FUN_0203f2a0(void *);
extern int FUN_0203ef50(void *, u32);
extern int FUN_02029964(int, u32, u32, int);
extern int FUN_0203edcc(void *, int, int);
extern void FUN_02029ab8(int);
extern void FUN_0203ef08(void *);
extern u32 *FUN_02017dd0(void);
extern u32 *FUN_02018bbc(int);
extern void FUN_0203a04c(u32, int);
extern void func_0x01ff861c(int, u32, int);
extern void FUN_02016178(int, int, void *, void *);

/* Forward declarations - functions defined in this file */
void FUN_02016254(int, int, void *, void *);
void FUN_020162fc(int *, int);
int FUN_020163b8(int *, int *);
void FUN_02016588(int *, int *, int *, int *);
int FUN_020166ac(int *, int *, int);
u32 FUN_020167dc(int *, int);
int FUN_02016864(int *);
int FUN_02016920(u32);
u32 FUN_020169e0(void *);
short *FUN_02016a34(int, void *, short *);
void FUN_02016bd4(int, int, void *);
void FUN_02017988(int *, int);
void FUN_02017b14(int);
u32 *FUN_02017d00(int, u32, u16, u16, u16, int);
int FUN_02017ef8(short *, int);
void FUN_0201806c(short *, int);
void FUN_02018254(int, short *, int);
void FUN_02018748(int);
void FUN_02018abc(int, int, int, int);
int *FUN_02018be0(int *, int, int, int);
void FUN_02018ca8(u32 *, int, int, int);
void FUN_02018d14(u32 *, int, int, u32, int, int, int, int);
void FUN_02018ed0(void);

/* Data references (ROM/BSS symbols) */
extern u16 * DAT_020163ac;
extern int * DAT_020163b0;
extern int * DAT_020163b4;
extern int * DAT_02016570;
extern int * DAT_02016574;
extern int * DAT_02016578;
extern u16 * DAT_0201657c;
extern int * DAT_02016580;
extern int * DAT_02016584;
extern u32 DAT_020169bc;
extern int DAT_02016bcc;
extern int DAT_02016bd0;
extern int DAT_02017980;
extern u32 DAT_02017984;
extern u32 DAT_02017b10;
extern int * DAT_02017cfc;
extern u32 DAT_02017dcc;
extern int * DAT_02018728;
extern int DAT_0201872c;
extern int DAT_02018730;
extern u32 DAT_02018734;
extern u32 DAT_02018738;
extern u32 DAT_0201873c;
extern u32 DAT_02018740;
extern u32 DAT_02018744;
extern u32 DAT_02018ab0;
extern u32 * DAT_02018ab4;
extern int * DAT_02018ab8;
extern u32 DAT_02018bb8;
extern u32 DAT_02018ca4;
extern u32 DAT_02018d0c;
extern u32 DAT_02018d10;
extern u32 DAT_02018e98;
extern u32 DAT_02018e9c;
#ifdef HOST_PORT
/* Aliased to 64-bit-safe shadow defined in link_stubs.c. The auto-
 * generated host_undefined_stubs.c declares these as 4-byte uint32_t
 * which truncates host pointers; FUN_02018ed0 dereferences them. */
extern intptr_t g_mc_DAT_02018f08;
extern int     *g_mc_DAT_02018f0c;
#define DAT_02018f08 g_mc_DAT_02018f08
#define DAT_02018f0c g_mc_DAT_02018f0c
#else
extern int DAT_02018f08;
extern int * DAT_02018f0c;
#endif

/* Static data references */
static u32 param_addr;

/* LZCOUNT intrinsic - maps to ARM CLZ instruction */
#define LZCOUNT(x) ((x) == 0 ? 32 : __builtin_clz(x))

// FUN_02017df4 @ 0x02017df4 (156 bytes) -- Initialize tilemap layer list
int FUN_02017df4(int param_1, u32 param_2, int param_3, int param_4)
{
    int iVar1;
    u32 *puVar2;
    u32 *puVar3;

    FUN_0202cc10((void *)param_1, 0, 0x28);
    FUN_0202cc10((void *)param_2, 0, param_3 * 0x30 + param_4);
    *(u32 *)(param_1 + 0x14) = param_2;
    *(int *)(param_1 + 0x24) = param_3 * 0x30 + *(int *)(param_1 + 0x14);
    puVar2 = *(u32 **)(param_1 + 0x14);
    *(u32 **)(param_1 + 0x18) = puVar2;
    iVar1 = 0;
    puVar3 = puVar2;
    if (0 < param_3 - 1) {
        do {
            puVar2 = puVar3 + 0xc;
            iVar1 = iVar1 + 1;
            *puVar3 = (u32)puVar2;
            puVar3 = puVar2;
        } while (iVar1 < param_3 - 1);
    }
    *puVar2 = 0;
    *(u32 **)(param_1 + 0x1c) = puVar2;
    *(u32 *)(param_1 + 0x20) = 0;
    return param_1;
}

// FUN_02017eb0 @ 0x02017eb0 (68 bytes) -- Check tilemap layer bounds
int FUN_02017eb0(u32 param_1, int param_2)
{
    int iVar1;
    int iVar2;

    iVar1 = FUN_02018194((s16 *)0, 0);
    if (-1 < param_2 + *(int *)param_addr) {
        iVar2 = FUN_02018194((s16 *)param_1, 0);
        if (iVar1 == iVar2) {
            iVar1 = -1;
        }
        return iVar1;
    }
    return iVar1;
}

// FUN_02018194 @ 0x02018194 (68 bytes) -- Find curve segment and interpolate
int FUN_02018194_impl(s16 *param_1, int param_2)
{
    s16 *psVar1;
    int iVar2;
    int iVar3;

    iVar2 = (int)(param_2 + ((u32)(param_2 >> 0xb) >> 0x14)) >> 0xc;
    psVar1 = param_1;
    do {
        iVar3 = (int)psVar1[2];
        if (param_1 + param_1[1] <= psVar1 + 3) {
            return iVar3 << 4;
        }
        iVar2 = iVar2 - psVar1[3];
        psVar1 = psVar1 + 2;
    } while (-1 < iVar2);
    return iVar3 << 4;
}

// FUN_020181d8 @ 0x020181d8 (124 bytes) -- Interpolate between curve points
int FUN_020181d8(int param_1, int param_2)
{
    s16 sVar1;
    s16 sVar2;
    s16 *psVar3;
    int iVar4;
    int iVar5;

    iVar4 = (int)(param_2 + ((u32)(param_2 >> 7) >> 0x18)) >> 8;
    psVar3 = (s16 *)(param_1 + 4);
    do {
        sVar1 = *psVar3;
        if ((s16 *)(param_1 + *(s16 *)(param_1 + 2) * 2) <= psVar3 + 1) {
            return (int)sVar1 << 4;
        }
        iVar5 = (int)psVar3[1];
        psVar3 = psVar3 + 2;
        iVar4 = iVar4 + iVar5 * -0x10;
    } while (-1 < iVar4);
    if (iVar5 != 0) {
        sVar2 = *psVar3;
        iVar4 = FUN_020464b0(iVar4 * ((int)sVar2 - (int)sVar1), iVar5);
        return iVar4 + sVar2 * 0x10;
    }
    return (int)*psVar3 << 4;
}

// FUN_02016254 @ 0x02016254 (168 bytes) -- Apply rotation to 3x3 matrix columns
void FUN_02016254(int param_1,int param_2,void *param_3_v,void *param_4_v)

{
  int *param_3 = (int *)param_3_v;
  int *param_4 = (int *)param_4_v;
  int iVar1;
  int iVar2;
  
  iVar1 = *param_3;
  iVar2 = *param_4;
  *param_3 = param_1 * (iVar1 >> 6) - param_2 * (iVar2 >> 6) >> 6;
  *param_4 = param_2 * (iVar1 >> 6) + param_1 * (iVar2 >> 6) >> 6;
  iVar1 = param_3[3] >> 6;
  iVar2 = param_4[3] >> 6;
  param_3[3] = param_1 * iVar1 - param_2 * iVar2 >> 6;
  param_4[3] = param_2 * iVar1 + param_1 * iVar2 >> 6;
  iVar1 = param_3[6] >> 6;
  iVar2 = param_4[6] >> 6;
  param_3[6] = param_1 * iVar1 - param_2 * iVar2 >> 6;
  param_4[6] = param_2 * iVar1 + param_1 * iVar2 >> 6;
  return;
}

// FUN_020162fc @ 0x020162fc (176 bytes) -- Normalize vector with hardware sqrt
void FUN_020162fc(int *param_1,int param_2)

{
  u16 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  u32 uVar6;
  
  if (0x10 - LZCOUNT(param_2) < 1) {
    iVar3 = 0xe;
  }
  else {
    iVar3 = 0xe - (0x10 - LZCOUNT(param_2));
  }
  FUN_020167dc(param_1,iVar3);
  piVar2 = DAT_020163b0;
  puVar1 = DAT_020163ac;
  iVar3 = param_1[1];
  iVar4 = *param_1;
  iVar5 = param_1[2];
  *DAT_020163ac = 0;
  *piVar2 = iVar5 * iVar5 + iVar4 * iVar4 + iVar3 * iVar3;
  do {
  } while ((*puVar1 & 0x8000) != 0);
  uVar6 = *DAT_020163b4;
  iVar3 = FUN_020464b0(*param_1 * param_2,uVar6);
  *param_1 = iVar3;
  iVar3 = FUN_020464b0(param_1[1] * param_2,uVar6);
  param_1[1] = iVar3;
  iVar3 = FUN_020464b0(param_1[2] * param_2,uVar6);
  param_1[2] = iVar3;
  return;
}

// FUN_020163b8 @ 0x020163b8 (440 bytes) -- Compute dot product angle between vectors
int FUN_020163b8(int *param_1,int *param_2)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  u16 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  u32 uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  
  FUN_020167dc(param_1,0xe);
  FUN_020167dc(param_2,0xe);
  piVar2 = DAT_02016570;
  *DAT_02016570 = *param_1 * *param_1;
  piVar1 = DAT_02016574;
  piVar2[1] = param_1[1] * param_1[1];
  piVar3 = DAT_02016578;
  piVar2[2] = param_1[2] * param_1[2];
  *piVar1 = *param_2 * *param_2;
  piVar1[1] = param_2[1] * param_2[1];
  piVar1[2] = param_2[2] * param_2[2];
  *piVar3 = *param_1 * *param_2;
  piVar3[1] = param_1[1] * param_2[1];
  piVar3[2] = param_1[2] * param_2[2];
  iVar5 = FUN_020166ac(piVar2,piVar1,0x18);
  piVar2 = DAT_02016578;
  uVar8 = iVar5 / 2 + 0xc;
  if ((int)uVar8 < 1) {
    uVar8 = -uVar8;
    piVar1 = DAT_02016578 + 1;
    iVar5 = DAT_02016578[2];
    *DAT_02016578 = *DAT_02016578 >> (uVar8 & 0xff);
    piVar2[1] = *piVar1 >> (uVar8 & 0xff);
    piVar2[2] = iVar5 >> (uVar8 & 0xff);
  }
  else {
    piVar1 = DAT_02016578 + 1;
    iVar5 = DAT_02016578[2];
    *DAT_02016578 = *DAT_02016578 << (uVar8 & 0xff);
    piVar2[1] = *piVar1 << (uVar8 & 0xff);
    piVar2[2] = iVar5 << (uVar8 & 0xff);
  }
  piVar2 = DAT_02016580;
  puVar4 = DAT_0201657c;
  iVar10 = *DAT_02016570;
  iVar9 = DAT_02016570[1];
  iVar7 = *DAT_02016574;
  iVar6 = DAT_02016574[1];
  iVar11 = DAT_02016570[2];
  iVar5 = DAT_02016574[2];
  *DAT_0201657c = 0;
  *piVar2 = (iVar11 + iVar10 + iVar9) * (iVar5 + iVar7 + iVar6);
  do {
  } while ((*puVar4 & 0x8000) != 0);
  iVar5 = FUN_020464b0(DAT_02016578[2] + *DAT_02016578 + DAT_02016578[1],*DAT_02016584);
  if (0x1000 < iVar5) {
    iVar5 = 0x1000;
  }
  if (iVar5 < -0x1000) {
    iVar5 = -0x1000;
  }
  return iVar5;
}

// FUN_02016588 @ 0x02016588 (292 bytes) -- Cross product of two 3D vectors
void FUN_02016588(int *param_1,int *param_2,int *param_3,int *param_4)

{
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  
  if (param_4 == (int *)0x0) {
    local_20 = *param_2;
    local_1c = param_2[1];
    local_18 = param_2[2];
    local_14 = *param_3;
    local_10 = param_3[1];
    local_c = param_3[2];
  }
  else {
    local_20 = *param_2 - *param_4;
    local_1c = param_2[1] - param_4[1];
    local_18 = param_2[2] - param_4[2];
    local_14 = *param_3 - *param_4;
    local_10 = param_3[1] - param_4[1];
    local_c = param_3[2] - param_4[2];
  }
  FUN_020166ac(&local_20,&local_14,0x1e);
  *param_1 = local_1c * local_c - local_10 * local_18;
  param_1[1] = local_18 * local_14 - local_c * local_20;
  param_1[2] = local_20 * local_10 - local_14 * local_1c;
  return;
}

// FUN_020166ac @ 0x020166ac (304 bytes) -- Scale two vectors for fixed-point precision
int FUN_020166ac(int *param_1,int *param_2,int param_3)

{
  u32 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = FUN_02016864(param_1);
  iVar3 = FUN_02016864(param_2);
  param_3 = param_3 - (iVar2 + iVar3);
  iVar5 = 0;
  if (param_3 < 1) {
    if ((param_3 < 0) && (iVar5 = (param_3 + -1) / 2 << 1, iVar5 != 0)) {
      iVar4 = iVar5;
      do {
        if (iVar2 < iVar3) {
          iVar3 = iVar3 + -1;
          *param_2 = *param_2 >> 1;
          param_2[1] = param_2[1] >> 1;
          param_2[2] = param_2[2] >> 1;
        }
        else {
          iVar2 = iVar2 + -1;
          *param_1 = *param_1 >> 1;
          param_1[1] = param_1[1] >> 1;
          param_1[2] = param_1[2] >> 1;
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 != 0);
    }
  }
  else {
    uVar1 = (param_3 + 1) / 2;
    iVar5 = uVar1 << 1;
    *param_1 = *param_1 << (uVar1 & 0xff);
    param_1[1] = param_1[1] << (uVar1 & 0xff);
    param_1[2] = param_1[2] << (uVar1 & 0xff);
    *param_2 = *param_2 << (uVar1 & 0xff);
    param_2[1] = param_2[1] << (uVar1 & 0xff);
    param_2[2] = param_2[2] << (uVar1 & 0xff);
  }
  return iVar5;
}

// FUN_020167dc @ 0x020167dc (136 bytes) -- Shift vector components to target precision
u32 FUN_020167dc(int *param_1,int param_2)

{
  u32 uVar1;
  int iVar2;
  u32 uVar3;
  
  iVar2 = FUN_02016864(param_1);
  uVar3 = param_2 - iVar2;
  if ((int)uVar3 < 1) {
    if ((int)uVar3 < 0) {
      uVar1 = -uVar3;
      *param_1 = *param_1 >> (uVar1 & 0xff);
      param_1[1] = param_1[1] >> (uVar1 & 0xff);
      param_1[2] = param_1[2] >> (uVar1 & 0xff);
    }
  }
  else {
    *param_1 = *param_1 << (uVar3 & 0xff);
    param_1[1] = param_1[1] << (uVar3 & 0xff);
    param_1[2] = param_1[2] << (uVar3 & 0xff);
  }
  return uVar3;
}

// FUN_02016864 @ 0x02016864 (84 bytes) -- Count leading zeros across 3 vector components
int FUN_02016864(int *param_1)

{
  int iVar1;
  u32 uVar2;
  
  iVar1 = *param_1;
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  uVar2 = 0x1f;
  if ((u32)LZCOUNT(iVar1) < 0x1f) {
    uVar2 = LZCOUNT(iVar1);
  }
  iVar1 = param_1[1];
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  if ((u32)LZCOUNT(iVar1) < uVar2) {
    uVar2 = LZCOUNT(iVar1);
  }
  iVar1 = param_1[2];
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  if ((u32)LZCOUNT(iVar1) < uVar2) {
    uVar2 = LZCOUNT(iVar1);
  }
  return 0x20 - uVar2;
}

// FUN_02016920 @ 0x02016920 (156 bytes) -- Load and decompress archive resource
int FUN_02016920(u32 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  u8 auStack_50 [32];
  int iStack_30;
  int iStack_2c;
  
  FUN_0203f2a0(auStack_50);
  iVar1 = FUN_0203ef50(auStack_50,param_1);
  if (iVar1 != 0) {
    iVar3 = iStack_2c - iStack_30;
    iVar1 = FUN_02029964(1,iVar3 + 3U & 0xfffffffc,DAT_020169bc,0);
    if ((iVar1 != 0) && (iVar2 = FUN_0203edcc(auStack_50,iVar1,iVar3), iVar3 != iVar2)) {
      FUN_02029ab8(iVar1);
      iVar1 = 0;
    }
    FUN_0203ef08(auStack_50);
    return iVar1;
  }
  return 0;
}

// FUN_020169e0 @ 0x020169e0 (84 bytes) -- Pack RGB5 color from component array
u32 FUN_020169e0(void *param_1_v)

{
  u32 *param_1 = (u32 *)param_1_v;
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  
  uVar3 = *param_1;
  uVar2 = param_1[1];
  uVar1 = param_1[2];
  if ((int)uVar3 < 0x20) {
    if ((int)uVar3 < 0) {
      uVar3 = 0;
    }
  }
  else {
    uVar3 = 0x1f;
  }
  if ((int)uVar2 < 0x20) {
    if ((int)uVar2 < 0) {
      uVar2 = 0;
    }
  }
  else {
    uVar2 = 0x1f;
  }
  if ((int)uVar1 < 0x20) {
    if ((int)uVar1 < 0) {
      uVar1 = 0;
    }
  }
  else {
    uVar1 = 0x1f;
  }
  return uVar3 | uVar2 << 5 | uVar1 << 10;
}

// FUN_02016a34 @ 0x02016a34 (408 bytes) -- Evaluate animation keyframe value
short * FUN_02016a34(int param_1,void *param_2_v,short *param_3)

{
  int *param_2 = (int *)param_2_v;
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  sVar1 = param_3[1];
  if (sVar1 == 0xf) {
    iVar3 = (int)param_3[2];
    iVar6 = (int)*param_3;
    iVar2 = 0;
    iVar5 = 0;
    do {
      iVar4 = (int)param_3[iVar5 + 4];
      if (*(int *)(param_1 + 4) < iVar4) {
        iVar2 = FUN_020464b0((*(int *)(param_1 + 4) - iVar2) * (param_3[iVar5 + 3] - iVar6) * 0x40,
                             iVar4 - iVar2);
        *param_2 = iVar6 * 0x1000 + iVar2 * 0x40;
        return param_3 + iVar3 * 2 + 3;
      }
      iVar7 = iVar5 + 2;
      iVar6 = (int)param_3[iVar5 + 3];
      iVar2 = iVar4;
      iVar5 = iVar7;
    } while (iVar3 * 2 != iVar7);
    *param_2 = iVar6 << 0xc;
    return param_3 + iVar3 * 2 + 3;
  }
  if (sVar1 != 0x10) {
    if (sVar1 != 0x11) {
      *param_2 = (int)*param_3 << 0xc;
      return param_3 + 1;
    }
    iVar5 = *(int *)(param_1 + 4);
    iVar2 = iVar5 * ((int)(param_3[3] * iVar5 + ((u32)(param_3[3] * iVar5 >> 2) >> 0x1d)) >> 3);
    *param_2 = *param_3 * 0x1000 + param_3[2] * iVar5 * 0x10 +
               ((int)(iVar2 + ((u32)(iVar2 >> 2) >> 0x1d)) >> 3) / 2;
    return param_3 + 5;
  }
  *param_2 = (int)param_3[2] *
             (int)*(short *)(DAT_02016bd0 +
                            ((int)(param_3[4] * 0x100 +
                                   (int)param_3[3] * *(int *)(param_1 + 4) * 0x10 & DAT_02016bcc) >>
                            4) * 4) + *param_3 * 0x1000;
  return param_3 + 5;
}

// FUN_02016bd4 @ 0x02016bd4 (3500 bytes) -- Build sprite display list from script commands
void FUN_02016bd4(int param_1,int param_2,void *param_3_v)

{
  u16 *param_3 = (u16 *)param_3_v;
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  u32 *puVar4;
  int iVar5;
  int iVar6;
  u32 *puVar7;
  int *piVar8;
  u32 *puVar9;
  u32 *puVar10;
  int *piVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  u32 *puVar17;
  u32 uStack_14c;
  int local_148;
  int iStack_144;
  int iStack_13c;
  int iStack_134;
  int iStack_130;
  int iStack_12c;
  int iStack_128;
  int iStack_124;
  int iStack_11c;
  int iStack_118;
  int iStack_114;
  u32 uStack_d8;
  int iStack_d4;
  u32 uStack_d0;
  int iStack_cc;
  int iStack_c8;
  int local_c4;
  int local_c0;
  u32 local_bc;
  u32 local_b8;
  u32 local_b4;
  u32 local_b0;
  int iStack_ac;
  int aiStack_a8 [8];
  int aiStack_88 [3];
  u8 auStack_7c [80];
  int aiStack_2c [2];
  
  iVar14 = 0;
  local_148 = (int)*(short *)(param_2 + 0x12) << 0xc;
  iVar15 = 0;
  local_c4 = 0;
  local_c0 = 0;
  local_bc = 0;
  local_b8 = 0;
  local_b4 = 0;
  local_b0 = 0;
  iVar13 = 1;
  iVar12 = 3;
  piVar11 = aiStack_a8;
  piVar8 = aiStack_88;
  if (param_3 == (u16 *)0x0) {
    return;
  }
  iStack_11c = 7;
  iStack_114 = 3;
  iStack_118 = 0;
  iStack_124 = 1;
  iStack_128 = 0;
  iStack_12c = 4;
  do {
    switch(*param_3) {
    case 0:
      return;
    case 1:
      return;
    case 2:
      if (0x1f < iStack_c8) {
        iStack_c8 = 0x1f;
      }
      puVar17 = *(u32 **)(param_1 + 0x24);
      *puVar17 = 0x29;
      puVar17[1] = *(int *)(param_1 + 0xc) << 0x18 | 0xc0U | iStack_c8 << 0x10;
      puVar17[2] = 0x11;
      puVar17[3] = 0x19;
      puVar17[4] = *(u32 *)(param_2 + 0x18);
      puVar17[5] = *(u32 *)(param_2 + 0x1c);
      puVar17[6] = 0;
      puVar17[7] = *(u32 *)(param_2 + 0x20);
      puVar7 = puVar17 + 0x11;
      puVar17[8] = *(u32 *)(param_2 + 0x24);
      puVar17[9] = 0;
      puVar17[10] = 0;
      puVar17[0xb] = 0;
      puVar17[0xc] = 0x1000;
      puVar10 = puVar17 + 0x1d;
      puVar17[0xd] = local_c4 + *(short *)(param_2 + 0xe) * 0x1000;
      puVar17[0xe] = local_c0 + *(short *)(param_2 + 0x10) * 0x1000;
      puVar17[0xf] = local_148;
      puVar17[0x10] = 0x19;
      uVar1 = FUN_020169e0((void *)aiStack_88);
      uStack_14c = uVar1;
      if (iVar15 - 3U < 5) {
        uStack_14c = FUN_020169e0((void *)auStack_7c);
      }
      switch(iVar15) {
      case 0:
        break;
      case 1:
        goto LAB_020171a0;
      case 2:
        goto LAB_02017334;
      case 3:
        break;
      case 4:
        goto LAB_020171a0;
      case 5:
LAB_02017334:
        *puVar10 = 0x40;
        puVar17[0x1e] = 3;
        puVar10 = puVar17 + 0x1f;
        iVar15 = 0;
        if (-1 < iVar12) {
          iStack_134 = 0;
          do {
            *puVar10 = 0x20;
            puVar10[1] = uVar1;
            puVar10[2] = 0x25;
            uVar2 = FUN_020464b0(iStack_134,iVar12);
            iVar16 = (int)(uVar2 & 0xffff) >> 4;
            iVar5 = (int)*(short *)(DAT_02017980 + iVar16 * 4);
            iVar15 = iVar15 + 1;
            uVar2 = (u32)*(short *)(DAT_02017980 + (iVar16 * 2 + 1) * 2);
            puVar4 = puVar10 + 7;
            iStack_134 = iStack_134 + 0x10000;
            puVar10[3] = uVar2 & DAT_02017984 | iVar5 << 0x10;
            puVar10[4] = 0x20;
            puVar10[5] = uStack_14c;
            puVar10[6] = 0x25;
            puVar10 = puVar10 + 8;
            iVar16 = (aiStack_a8[0] + 0x1000) * uVar2;
            iVar5 = (aiStack_a8[0] + 0x1000) * iVar5;
            *puVar4 = DAT_02017984 & (int)(iVar16 + ((u32)(iVar16 >> 0xb) >> 0x14)) >> 0xc |
                      ((int)(iVar5 + ((u32)(iVar5 >> 0xb) >> 0x14)) >> 0xc) << 0x10;
          } while (iVar15 <= iVar12);
        }
        goto switchD_02017094_default;
      case 6:
        break;
      case 7:
LAB_020171a0:
        *puVar10 = 0x40;
        puVar17[0x1e] = 2;
        iVar15 = 0;
        iStack_130 = 0;
        puVar9 = puVar17 + 0x1f;
        goto LAB_020171dc;
      default:
        goto switchD_02017094_default;
      }
      *puVar10 = 0x40;
      puVar17[0x1e] = 2;
      puVar10 = puVar17 + 0x1f;
      iStack_144 = 0;
      if (-1 < iVar12) {
        iVar15 = 0;
        do {
          *puVar10 = 0x20;
          puVar10[1] = uStack_14c;
          puVar10[2] = 0x24;
          puVar10[3] = 0;
          puVar10[4] = 0x20;
          puVar10[5] = uVar1;
          puVar10[6] = 0x25;
          uVar2 = FUN_020464b0(iVar15,iVar12);
          iVar16 = (int)(uVar2 & 0xffff) >> 4;
          puVar4 = puVar10 + 7;
          puVar10 = puVar10 + 8;
          iVar15 = iVar15 + 0x10000;
          *puVar4 = (int)*(short *)(DAT_02017980 + (iVar16 * 2 + 1) * 2) & DAT_02017984 |
                    (int)*(short *)(DAT_02017980 + iVar16 * 4) << 0x10;
          iStack_144 = iStack_144 + 1;
        } while (iStack_144 <= iVar12);
      }
      goto switchD_02017094_default;
    case 3:
      iVar16 = (int)(short)param_3[1];
      param_3 = param_3 + 2;
      iVar14 = iStack_12c;
      if ((iVar16 < 5) && (iVar14 = iVar16, iVar16 < 0)) {
        iVar14 = iStack_128;
      }
      break;
    case 4:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&iStack_c8,param_3 + 1);
      iStack_c8 = (int)(iStack_c8 + ((u32)(iStack_c8 >> 0xb) >> 0x14)) >> 0xc;
      if (0x1f < iStack_c8) {
        iStack_c8 = 0x1f;
      }
      if (iStack_c8 < 1) {
        return;
      }
      break;
    case 5:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&iStack_ac,param_3 + 1);
      local_148 = local_148 + iStack_ac;
      break;
    case 6:
      iVar13 = (int)(short)param_3[1];
      param_3 = param_3 + 2;
      if (iVar13 < 0x41) {
        if (iVar13 < 1) {
          iVar13 = iStack_124;
        }
      }
      else {
        iVar13 = 0x40;
      }
      break;
    case 7:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&uStack_d8,param_3 + 1);
      uStack_d8 = (int)(uStack_d8 + ((u32)((int)uStack_d8 >> 3) >> 0x1c)) >> 4;
      break;
    case 8:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&iStack_d4,param_3 + 1);
      break;
    case 9:
      iVar16 = (int)(short)param_3[1];
      param_3 = param_3 + 2;
      iVar15 = iStack_11c;
      if ((iVar16 < 8) && (iVar15 = iVar16, iVar16 < 0)) {
        iVar15 = iStack_118;
      }
      break;
    case 10:
      iVar12 = (int)(short)param_3[1];
      param_3 = param_3 + 2;
      if (iVar12 < 0x41) {
        if (iVar12 < 3) {
          iVar12 = iStack_114;
        }
      }
      else {
        iVar12 = 0x40;
      }
      break;
    case 0xb:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&uStack_d0,param_3 + 1);
      uStack_d0 = (int)(uStack_d0 + ((u32)((int)uStack_d0 >> 3) >> 0x1c)) >> 4;
      break;
    case 0xc:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&iStack_cc,param_3 + 1);
      break;
    case 0xd:
      param_3 = (u16 *)FUN_02016a34(param_1,piVar11,param_3 + 1);
      *piVar11 = (int)(*piVar11 + ((u32)(*piVar11 >> 7) >> 0x18)) >> 8;
      if (piVar11 < aiStack_a8 + 7) {
        piVar11 = piVar11 + 1;
      }
      break;
    case 0xe:
      param_3 = (u16 *)FUN_02016a34(param_1,piVar8,param_3 + 1);
      *piVar8 = (int)(*piVar8 + ((u32)(*piVar8 >> 0xb) >> 0x14)) >> 0xc;
      if (piVar8 < aiStack_2c) {
        piVar8 = piVar8 + 1;
      }
      break;
    case 0xf:
      return;
    case 0x10:
      return;
    case 0x11:
      return;
    case 0x12:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_c4,param_3 + 1);
      break;
    case 0x13:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_c0,param_3 + 1);
      break;
    case 0x14:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_bc,param_3 + 1);
      local_bc = (int)(local_bc + ((u32)((int)local_bc >> 3) >> 0x1c)) >> 4;
      break;
    case 0x15:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_b8,param_3 + 1);
      local_b8 = (int)(local_b8 + ((u32)((int)local_b8 >> 3) >> 0x1c)) >> 4;
      break;
    case 0x16:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_b4,param_3 + 1);
      local_b4 = (int)(local_b4 + ((u32)((int)local_b4 >> 3) >> 0x1c)) >> 4;
      break;
    case 0x17:
      param_3 = (u16 *)FUN_02016a34(param_1,(void *)&local_b0,param_3 + 1);
      local_b0 = (int)(local_b0 + ((u32)((int)local_b0 >> 3) >> 0x1c)) >> 4;
      break;
    case 0x18:
      param_3 = param_3 + 2;
      break;
    default:
      return;
    }
    if (param_3 == (u16 *)0x0) {
      return;
    }
  } while( true );
LAB_020171dc:
  *puVar9 = 0x20;
  puVar9[1] = uStack_14c;
  puVar9[2] = 0x24;
  puVar9[3] = 0;
  puVar9[4] = 0x20;
  puVar9[5] = uVar1;
  puVar9[6] = 0x25;
  uVar2 = FUN_020464b0(iStack_130,iVar12);
  iVar16 = (int)(uVar2 & 0xffff) >> 4;
  puVar10 = puVar9 + 8;
  puVar9[7] = (int)*(short *)(DAT_02017980 + (iVar16 * 2 + 1) * 2) & DAT_02017984 |
              (int)*(short *)(DAT_02017980 + iVar16 * 4) << 0x10;
  if (iVar15 == iVar12) goto switchD_02017094_default;
  *puVar10 = 0x20;
  puVar9[9] = uStack_14c;
  puVar9[10] = 0x24;
  puVar9[0xb] = 0;
  puVar9[0xc] = 0x20;
  puVar9[0xd] = uVar1;
  puVar9[0xe] = 0x25;
  uVar2 = FUN_020464b0((iVar15 + 1) * 0x10000,iVar12);
  iVar16 = (int)(uVar2 & 0xffff) >> 4;
  iVar5 = (aiStack_a8[0] + 0x1000) * (int)*(short *)(DAT_02017980 + (iVar16 * 2 + 1) * 2);
  iVar16 = (aiStack_a8[0] + 0x1000) * (int)*(short *)(DAT_02017980 + iVar16 * 4);
  puVar10 = puVar9 + 0x10;
  puVar9[0xf] = DAT_02017984 & (int)(iVar5 + ((u32)(iVar5 >> 0xb) >> 0x14)) >> 0xc |
                ((int)(iVar16 + ((u32)(iVar16 >> 0xb) >> 0x14)) >> 0xc) << 0x10;
  if (iVar15 + 1 == iVar12) goto switchD_02017094_default;
  iVar15 = iVar15 + 2;
  iStack_130 = iStack_130 + 0x20000;
  puVar9 = puVar10;
  goto LAB_020171dc;
switchD_02017094_default:
  *puVar10 = 0x41;
  puVar10[1] = 0x12;
  puVar10[2] = 1;
  uVar2 = DAT_02017984;
  iVar16 = DAT_02017980;
  uStack_d0 = uStack_d0 & DAT_02017984;
  iVar12 = (int)(iStack_cc + ((u32)(iStack_cc >> 7) >> 0x18)) >> 8;
  iVar15 = iVar12 * *(short *)(DAT_02017980 + (((int)uStack_d0 >> 4) * 2 + 1) * 2);
  iVar12 = iVar12 * *(short *)(DAT_02017980 + ((int)uStack_d0 >> 4) * 4);
  iVar5 = *(int *)(param_1 + 0x24);
  iVar15 = (int)(iVar15 + ((u32)(iVar15 >> 3) >> 0x1c)) >> 4;
  iVar12 = (int)(iVar12 + ((u32)(iVar12 >> 3) >> 0x1c)) >> 4;
  puVar9 = puVar10 + 3;
  puVar10[3] = iVar15;
  puVar10[4] = iVar12;
  puVar10[5] = 0;
  puVar10[6] = -iVar12;
  puVar10[7] = iVar15;
  puVar10[8] = 0;
  puVar10[9] = 0;
  puVar10[10] = 0;
  puVar10[0xb] = 0x1000;
  iVar5 = (int)puVar9 - iVar5;
  local_b4 = local_b4 & uVar2;
  if (local_b4 != 0) {
    FUN_02016254((int)*(short *)(iVar16 + (((int)local_b4 >> 4) * 2 + 1) * 2),
                 (int)*(short *)(iVar16 + ((int)local_b4 >> 4) * 4),puVar10 + 4,puVar10 + 5);
  }
  local_b0 = local_b0 & DAT_02017984;
  if (local_b0 != 0) {
    FUN_02016254((int)*(short *)(DAT_02017980 + (((int)local_b0 >> 4) * 2 + 1) * 2),
                 (int)*(short *)(DAT_02017980 + ((int)local_b0 >> 4) * 4),puVar10 + 5,puVar9);
  }
  iVar12 = DAT_02017980;
  switch(iVar14) {
  case 0:
    *puVar7 = *puVar9;
    uVar2 = DAT_02017984;
    puVar17[0x12] = puVar10[4];
    puVar17[0x13] = puVar10[5];
    puVar17[0x14] = puVar10[6];
    puVar17[0x15] = puVar10[7];
    puVar17[0x16] = puVar10[8];
    puVar17[0x17] = puVar10[9];
    puVar17[0x18] = puVar10[10];
    puVar17[0x19] = puVar10[0xb];
    puVar17[0x1a] = 0;
    puVar17[0x1b] = 0;
    puVar17[0x1c] = 0;
    uStack_d8 = uStack_d8 & uVar2;
    if (uStack_d8 != 0) {
      FUN_02016254((int)*(short *)(DAT_02017980 + (((int)uStack_d8 >> 4) * 2 + 1) * 2),
                   (int)*(short *)(DAT_02017980 + ((int)uStack_d8 >> 4) * 4),puVar7,puVar17 + 0x12);
    }
    local_bc = local_bc & DAT_02017984;
    if (local_bc != 0) {
      FUN_02016254((int)*(short *)(DAT_02017980 + (((int)local_bc >> 4) * 2 + 1) * 2),
                   (int)*(short *)(DAT_02017980 + ((int)local_bc >> 4) * 4),puVar17 + 0x12,
                   puVar17 + 0x13);
    }
    local_b8 = local_b8 & DAT_02017984;
    if (local_b8 != 0) {
      FUN_02016254((int)*(short *)(DAT_02017980 + (((int)local_b8 >> 4) * 2 + 1) * 2),
                   (int)*(short *)(DAT_02017980 + ((int)local_b8 >> 4) * 4),puVar17 + 0x13,puVar7);
    }
    FUN_0203a04c(*(u32 *)(param_1 + 0x24),iVar5);
    func_0x01ff861c(1,*(u32 *)(param_1 + 0x24),iVar5);
    goto switchD_02017564_default;
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  default:
    goto switchD_02017564_default;
  }
  local_bc = local_bc & DAT_02017984;
  local_b8 = local_b8 & DAT_02017984;
  uVar2 = uStack_d8 & DAT_02017984;
  iStack_13c = 0;
  if (0 < iVar13) {
    iVar15 = 0;
    do {
      *puVar7 = *puVar9;
      puVar17[0x12] = puVar10[4];
      puVar17[0x13] = puVar10[5];
      puVar17[0x14] = puVar10[6];
      puVar17[0x15] = puVar10[7];
      puVar17[0x16] = puVar10[8];
      puVar17[0x17] = puVar10[9];
      puVar17[0x18] = puVar10[10];
      puVar17[0x19] = puVar10[0xb];
      if ((iVar14 == 1) || (iVar14 == 3)) {
        uVar3 = FUN_020464b0(iVar15,iVar13);
        iVar16 = (int)(uVar3 & DAT_02017984) >> 4;
        iVar6 = ((int)(iStack_d4 + ((u32)(iStack_d4 >> 7) >> 0x18)) >> 8) *
                (int)*(short *)(iVar12 + (iVar16 * 2 + 1) * 2);
        puVar17[0x1a] = (int)(iVar6 + ((u32)(iVar6 >> 3) >> 0x1c)) >> 4;
        iVar16 = ((int)(iStack_d4 + ((u32)(iStack_d4 >> 7) >> 0x18)) >> 8) *
                 (int)*(short *)(iVar12 + iVar16 * 4);
        puVar17[0x1b] = (int)(iVar16 + ((u32)(iVar16 >> 3) >> 0x1c)) >> 4;
      }
      else {
        iVar16 = FUN_020464b0(iVar15,iVar13);
        uVar2 = DAT_02017984;
        puVar17[0x1a] = iStack_d4;
        uVar2 = uStack_d8 + iVar16 & uVar2;
        puVar17[0x1b] = 0;
      }
      puVar17[0x1c] = 0;
      if (uVar2 != 0) {
        FUN_02016178((int)*(short *)(iVar12 + (((int)uVar2 >> 4) * 2 + 1) * 2),
                     (int)*(short *)(iVar12 + ((int)uVar2 >> 4) * 4),puVar7,puVar17 + 0x12);
      }
      if (local_bc != 0) {
        FUN_02016178((int)*(short *)(iVar12 + (((int)local_bc >> 4) * 2 + 1) * 2),
                     (int)*(short *)(iVar12 + ((int)local_bc >> 4) * 4),puVar17 + 0x12,
                     puVar17 + 0x13);
      }
      if (local_b8 != 0) {
        FUN_02016178((int)*(short *)(iVar12 + (((int)local_b8 >> 4) * 2 + 1) * 2),
                     (int)*(short *)(iVar12 + ((int)local_b8 >> 4) * 4),puVar17 + 0x13,puVar7);
      }
      if (iVar14 - 3U < 2) {
        puVar17[0x12] = (int)puVar17[0x12] / 2;
        puVar17[0x15] = (int)puVar17[0x15] / 2;
        puVar17[0x18] = (int)puVar17[0x18] / 2;
        puVar17[0x1b] = (int)puVar17[0x1b] / 2;
      }
      FUN_0203a04c(*(u32 *)(param_1 + 0x24),iVar5);
      func_0x01ff861c(1,*(u32 *)(param_1 + 0x24),iVar5);
      iVar15 = iVar15 + 0x10000;
      iStack_13c = iStack_13c + 1;
    } while (iStack_13c < iVar13);
  }
switchD_02017564_default:
  if (*(int *)(param_1 + 0xc) < 0x3f) {
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
  }
  else {
    *(u32 *)(param_1 + 0xc) = 0;
  }
  return;
}

// FUN_02017988 @ 0x02017988 (380 bytes) -- Process sprite animation script opcodes
void FUN_02017988(int *param_1,int param_2)

{
  short *psVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  psVar6 = *(short **)(param_2 + 4);
  if (psVar6 == (short *)0x0) {
    return;
  }
  iVar5 = 0;
  iVar4 = 0;
LAB_020179b8:
  while( true ) {
    while (sVar2 = *psVar6, 0xf < sVar2) {
      if (sVar2 < 0x11) {
        if (sVar2 != 0x10) goto LAB_02017afc;
        psVar6 = psVar6 + 4;
      }
      else {
        if (sVar2 != 0x11) goto LAB_02017afc;
        psVar6 = psVar6 + 4;
      }
    }
    if (sVar2 < 0xf) break;
    psVar6 = psVar6 + psVar6[1] * 2 + 2;
  }
  if ((sVar2 < 3) && (-1 < sVar2)) {
    if (sVar2 == 0) {
      *(u16 *)(param_2 + 0x14) =
           *(u16 *)(param_2 + 0x14) & 0xc000 |
           (u16)DAT_02017b10 & (u16)((iVar4 + ((u32)(iVar4 >> 7) >> 0x18)) * 0x100 >> 0x10);
      return;
    }
    if (sVar2 == 1) {
      psVar1 = psVar6 + 1;
      psVar6 = psVar6 + 2;
      iVar5 = *psVar1 * 0x100;
      if (iVar5 < 0x100) {
        iVar5 = 0x100;
      }
      param_1[4] = (int)psVar6;
      goto LAB_020179b8;
    }
    if (sVar2 == 2) {
      iVar3 = iVar5 + psVar6[1] * 0x100;
      if (iVar4 < iVar3) {
        iVar4 = iVar3;
      }
      if ((((*(u16 *)(param_2 + 0x14) & 0x3fff) != 0) &&
          (iVar3 = *(int *)(param_2 + 8) + psVar6[1] * -0x100, -1 < iVar3)) && (iVar3 <= iVar5)) {
        *param_1 = iVar3;
        iVar3 = FUN_020464b0(iVar3 * 4,(int)(iVar5 + ((u32)(iVar5 >> 7) >> 0x18)) >> 8);
        param_1[1] = iVar3;
        FUN_02016bd4((int)param_1,param_2,(void *)(long)param_1[4]);
      }
      psVar6 = psVar6 + 2;
      goto LAB_020179b8;
    }
  }
LAB_02017afc:
  psVar6 = psVar6 + 2;
  goto LAB_020179b8;
}

// FUN_02017b14 @ 0x02017b14 (488 bytes) -- Update all active sprite animation states
void FUN_02017b14(int param_1)

{
  u32 *puVar1;
  u32 uVar2;
  int iVar3;
  u32 *puVar4;
  u32 *puVar5;
  
  puVar1 = (u32 *)0x0;
  *DAT_02017cfc = 0;
  puVar4 = *(u32 **)(param_1 + 0x20);
  *(u32 *)(param_1 + 0xc) = 10;
  if (puVar4 != (u32 *)0x0) {
    do {
      if (puVar4[1] == 0) {
        if ((void (*)(void))(long)puVar4[10] == (void (*)(void))0) {
          **(u32 **)(param_1 + 0x1c) = (u32)puVar4;
          *(u32 **)(param_1 + 0x1c) = puVar4;
          if (puVar1 == (u32 *)0x0) {
            *(u32 *)(param_1 + 0x20) = *puVar4;
          }
          else {
            *puVar1 = *puVar4;
          }
          puVar5 = (u32 *)*puVar4;
        }
        else {
          ((void (*)(u32 *))puVar4[10])(puVar4);
          puVar4[10] = 0;
          puVar5 = (u32 *)*puVar4;
          puVar1 = puVar4;
        }
      }
      else {
        if ((int)((u32)*(u16 *)(puVar4 + 5) << 0x11) < 0) {
          if ((*(u16 *)(puVar4 + 5) & 0x3fff) != 0) {
            iVar3 = puVar4[2];
            if (iVar3 < 0) {
              do {
                puVar4[2] = puVar4[2] + (((u32)*(u16 *)(puVar4 + 5) << 0x12) >> 10);
                iVar3 = puVar4[2];
              } while (iVar3 < 0);
            }
            uVar2 = ((u32)*(u16 *)(puVar4 + 5) << 0x12) >> 10;
            if ((int)uVar2 < iVar3) {
              do {
                puVar4[2] = puVar4[2] - uVar2;
                uVar2 = ((u32)*(u16 *)(puVar4 + 5) << 0x12) >> 10;
              } while ((int)uVar2 < (int)puVar4[2]);
            }
          }
        }
        else {
          if ((int)puVar4[2] < 0) {
            puVar4[2] = 0;
          }
          uVar2 = ((u32)*(u16 *)(puVar4 + 5) << 0x12) >> 10;
          if ((int)uVar2 < (int)puVar4[2]) {
            puVar4[2] = uVar2;
          }
        }
        FUN_02017988((int *)param_1,(int)(u32)puVar4);
        uVar2 = (u32)*(u16 *)(puVar4 + 5);
        if (-1 < (int)(uVar2 << 0x10)) {
          if ((int)(uVar2 << 0x11) < 0) {
            puVar4[2] = puVar4[2] + (int)*(short *)(puVar4 + 3);
          }
          else {
            iVar3 = (int)*(short *)(puVar4 + 3);
            if ((iVar3 < 1) || ((int)puVar4[2] < (int)((uVar2 << 0x12) >> 10))) {
              if ((iVar3 < 0) && ((int)puVar4[2] < 1)) {
                puVar4[1] = 0;
              }
              else {
                puVar4[2] = puVar4[2] + iVar3;
              }
            }
            else {
              puVar4[1] = 0;
            }
          }
        }
        puVar5 = (u32 *)*puVar4;
        puVar1 = puVar4;
      }
      puVar4 = puVar5;
    } while (puVar5 != (u32 *)0x0);
    return;
  }
  return;
}

// FUN_02017d00 @ 0x02017d00 (204 bytes) -- Create and initialize sprite instance
u32 *
FUN_02017d00(int param_1,u32 param_2,u16 param_3,u16 param_4,u16 param_5
            ,int param_6)

{
  u32 uVar1;
  u32 *puVar2;
  
  puVar2 = (u32 *)FUN_02017dd0();
  if (puVar2 != (u32 *)0x0) {
    *puVar2 = *(u32 *)(param_1 + 0x20);
    *(u32 **)(param_1 + 0x20) = puVar2;
    puVar2[1] = param_2;
    *(u16 *)((int)puVar2 + 0xe) = param_3;
    *(u16 *)(puVar2 + 4) = param_4;
    *(u16 *)((int)puVar2 + 0x12) = param_5;
    puVar2[2] = 0;
    *(short *)(puVar2 + 3) = (short)param_6;
    *(u16 *)(puVar2 + 5) = *(u16 *)(puVar2 + 5) & 0xc000;
    uVar1 = DAT_02017dcc;
    *(u16 *)(puVar2 + 5) = *(u16 *)(puVar2 + 5) & 0xbfff;
    *(u16 *)(puVar2 + 5) = *(u16 *)(puVar2 + 5) & 0x7fff;
    puVar2[6] = 0x1000;
    puVar2[7] = 0;
    puVar2[8] = 0;
    puVar2[9] = 0x1000;
    puVar2[10] = uVar1;
    puVar2[0xb] = 0;
    FUN_02017988((int *)param_1,(int)(u32)puVar2);
    if (param_6 < 0) {
      puVar2[2] = ((u32)*(u16 *)(puVar2 + 5) << 0x12) >> 10;
    }
  }
  return puVar2;
}

// FUN_02017ef8 @ 0x02017ef8 (372 bytes) -- Interpolate looping curve with Hermite blending
int FUN_02017ef8(short *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  short *psVar9;
  int iVar10;
  short *psVar11;
  short *psVar12;
  
  sVar3 = param_1[1];
  psVar9 = param_1 + sVar3 + -1;
  iVar10 = (int)(param_2 + ((u32)(param_2 >> 7) >> 0x18)) >> 8;
  psVar11 = param_1;
  do {
    psVar12 = psVar11;
    psVar11 = psVar12 + 2;
    if (psVar9 <= psVar11) {
      return (int)*psVar11 << 4;
    }
    iVar1 = psVar12[3] * 0x10;
    iVar10 = iVar10 + psVar12[3] * -0x10;
  } while (-1 < iVar10);
  iVar7 = psVar12[4] * 0x10;
  iVar10 = iVar10 + iVar1;
  iVar2 = *psVar11 * 0x10;
  iVar8 = iVar2 * iVar1 + iVar10 * (iVar7 + *psVar11 * -0x10);
  if ((psVar12 + 5 < psVar9) || (iVar6 = iVar1, param_1[2] == *psVar9)) {
    if (psVar12 + 5 < psVar9) {
      sVar4 = psVar12[5];
      sVar5 = psVar12[6];
    }
    else {
      sVar4 = param_1[3];
      sVar5 = param_1[4];
    }
    if (sVar4 != 0) {
      iVar6 = FUN_020464b0(iVar10 * (((int)(iVar7 + ((u32)(iVar7 >> 3) >> 0x1c)) >> 4) - (int)sVar5
                                    ), iVar1 - iVar10);
      iVar8 = iVar8 + iVar6;
    }
    iVar8 = iVar7 * iVar10 + iVar8;
    iVar6 = iVar1 + iVar10;
  }
  if ((param_1 < psVar12) || (param_1[2] == *psVar9)) {
    if (param_1 < psVar12) {
      sVar4 = psVar12[1];
      sVar3 = *psVar12;
    }
    else {
      sVar4 = param_1[sVar3 + -2];
      sVar3 = param_1[sVar3 + -3];
    }
    if (sVar4 != 0) {
      iVar7 = FUN_020464b0(iVar10 * (((int)(iVar2 + ((u32)(iVar2 >> 3) >> 0x1c)) >> 4) - (int)sVar3
                                    ), iVar1 - iVar10);
      iVar8 = iVar8 + iVar7;
    }
    iVar8 = iVar2 * (iVar1 - iVar10) + iVar8;
    iVar6 = iVar6 + (iVar1 - iVar10);
  }
  iVar10 = FUN_020464b0(iVar8,iVar6);
  return iVar10;
}

// FUN_0201806c @ 0x0201806c (296 bytes) -- Interpolate open curve with Hermite blending
void FUN_0201806c(short *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  short *psVar8;
  
  iVar6 = (int)(param_2 + ((u32)(param_2 >> 7) >> 0x18)) >> 8;
  psVar7 = param_1;
  do {
    psVar8 = psVar7;
    psVar7 = psVar8 + 2;
    if (param_1 + param_1[1] + -1 <= psVar7) {
      return;
    }
    iVar1 = psVar8[3] * 0x10;
    iVar6 = iVar6 + psVar8[3] * -0x10;
  } while (-1 < iVar6);
  iVar4 = psVar8[4] * 0x10;
  iVar6 = iVar6 + iVar1;
  iVar2 = *psVar7 * 0x10;
  iVar5 = iVar2 * iVar1 + iVar6 * (iVar4 + *psVar7 * -0x10);
  iVar3 = iVar1;
  if (psVar8 + 5 < param_1 + param_1[1] + -1) {
    if (psVar8[5] != 0) {
      iVar3 = FUN_020464b0(iVar6 * (((int)(iVar4 + ((u32)(iVar4 >> 3) >> 0x1c)) >> 4) -
                                   (int)psVar8[6]), iVar1 - iVar6);
      iVar5 = iVar5 + iVar3;
    }
    iVar5 = iVar4 * iVar6 + iVar5;
    iVar3 = iVar1 + iVar6;
  }
  if (param_1 < psVar8) {
    if (psVar8[1] != 0) {
      iVar4 = FUN_020464b0(iVar6 * (((int)(iVar2 + ((u32)(iVar2 >> 3) >> 0x1c)) >> 4) -
                                   (int)*psVar8), iVar1 - iVar6);
      iVar5 = iVar5 + iVar4;
    }
    iVar5 = iVar2 * (iVar1 - iVar6) + iVar5;
    iVar3 = iVar3 + (iVar1 - iVar6);
  }
  FUN_020464b0(iVar5,iVar3);
  return;
}

// FUN_02018254 @ 0x02018254 (1228 bytes) -- Execute transform script on matrix stack
void FUN_02018254(int param_1,short *param_2,int param_3)

{
  short sVar1;
  u32 uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  short *psVar6;
  
  param_3 = param_3 + *param_2 * -0x1000;
  if (param_3 < 0) {
    return;
  }
  iVar4 = *(int *)(*(int *)(param_1 + 0x18) + 4);
  iVar3 = (int)*(short *)(iVar4 + (param_2[1] + 1) * 2);
  if (*(short *)(iVar4 + iVar3 * 2) * 0x1000 < param_3) {
    return;
  }
  piVar5 = *(int **)(param_1 + 0x14);
  psVar6 = (short *)(iVar4 + iVar3 * 2 + 2);
  if ((u32)DAT_02018728 < (u32)piVar5) {
    FUN_0202cd68((u32)DAT_02018728,(u32)piVar5,0x40);
  }
  else {
    FUN_0202cc94((u32)DAT_02018728,(u32)piVar5,0x40);
  }
LAB_020182dc:
  do {
    sVar1 = *psVar6;
    switch((int)sVar1) {
    case 0:
      return;
    case 1:
      *(int *)(param_1 + 0x60) = param_3;
      FUN_02034274(piVar5,piVar5 + -0x10,piVar5 + 0x10);
      (*(void (*)(int, int *))(*(u32 *)(*(int *)(param_1 + 0x18) + 0x28)))((int)psVar6[1], piVar5 + 0x10);
      psVar6 = psVar6 + 2;
      goto LAB_020182dc;
    case 2:
      if ((-1 < psVar6[2]) && (psVar6[2] < **(short **)(*(int *)(param_1 + 0x18) + 4))) {
        FUN_02034274(piVar5,piVar5 + -0x10,piVar5 + 0x10);
        *(int **)(param_1 + 0x14) = piVar5 + 0x20;
        FUN_02018254(param_1,psVar6 + 1,param_3);
        *(int **)(param_1 + 0x14) = piVar5 + -0x20;
      }
      psVar6 = psVar6 + 3;
      goto LAB_020182dc;
    case 3:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      *piVar5 = (int)(*piVar5 * iVar3 + ((u32)(*piVar5 * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[4] = (int)(piVar5[4] * iVar3 + ((u32)(piVar5[4] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[8] = (int)(piVar5[8] * iVar3 + ((u32)(piVar5[8] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[0xc] = (int)(piVar5[0xc] * iVar3 + ((u32)(piVar5[0xc] * iVar3 >> 0xb) >> 0x14)) >> 0xc
      ;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 4:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      piVar5[1] = (int)(piVar5[1] * iVar3 + ((u32)(piVar5[1] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[5] = (int)(piVar5[5] * iVar3 + ((u32)(piVar5[5] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[9] = (int)(piVar5[9] * iVar3 + ((u32)(piVar5[9] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[0xd] = (int)(piVar5[0xd] * iVar3 + ((u32)(piVar5[0xd] * iVar3 >> 0xb) >> 0x14)) >> 0xc
      ;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 5:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      piVar5[2] = (int)(piVar5[2] * iVar3 + ((u32)(piVar5[2] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[6] = (int)(piVar5[6] * iVar3 + ((u32)(piVar5[6] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[10] = (int)(piVar5[10] * iVar3 + ((u32)(piVar5[10] * iVar3 >> 0xb) >> 0x14)) >> 0xc;
      piVar5[0xe] = (int)(piVar5[0xe] * iVar3 + ((u32)(piVar5[0xe] * iVar3 >> 0xb) >> 0x14)) >> 0xc
      ;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 6:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      piVar5[0xc] = piVar5[0xc] + iVar3 * 0x100;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 7:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      piVar5[0xd] = piVar5[0xd] + iVar3 * 0x100;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 8:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      piVar5[0xe] = piVar5[0xe] + iVar3 * 0x100;
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 9:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      iVar3 = (int)(DAT_0201872c & iVar3 << 4) >> 4;
      FUN_02015ff4((int)*(short *)(DAT_02018730 + (iVar3 * 2 + 1) * 2),
                   (int)*(short *)(DAT_02018730 + iVar3 * 4),piVar5 + 1,piVar5 + 2);
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 10:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      iVar3 = (int)(DAT_0201872c & iVar3 << 4) >> 4;
      FUN_02015ff4((int)*(short *)(DAT_02018730 + (iVar3 * 2 + 1) * 2),
                   (int)*(short *)(DAT_02018730 + iVar3 * 4),piVar5 + 2,piVar5);
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 0xb:
      iVar3 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
      iVar3 = (int)(DAT_0201872c & iVar3 << 4) >> 4;
      FUN_02015ff4((int)*(short *)(DAT_02018730 + (iVar3 * 2 + 1) * 2),
                   (int)*(short *)(DAT_02018730 + iVar3 * 4),piVar5,piVar5 + 1);
      psVar6 = psVar6 + psVar6[1];
      goto LAB_020182dc;
    case 0xc:
      switch(psVar6[1]) {
      case 0:
        *(u32 *)(param_1 + 0x1c) = DAT_02018734;
        break;
      case 1:
        *(u32 *)(param_1 + 0x1c) = DAT_02018738;
        break;
      case 2:
        *(u32 *)(param_1 + 0x1c) = DAT_0201873c;
        break;
      case 3:
        *(u32 *)(param_1 + 0x1c) = DAT_02018740;
        break;
      case 4:
        *(u32 *)(param_1 + 0x1c) = DAT_02018744;
      }
      psVar6 = psVar6 + 2;
      goto LAB_020182dc;
    case 0xd:
      break;
    case 0xe:
      break;
    case 0xf:
    }
    uVar2 = (*(int (*)(short *, int))(*(u32 *)(param_1 + 0x1c)))(psVar6, param_3);
    *(u32 *)(param_1 + sVar1 * 4 + -0x20) = uVar2;
    psVar6 = psVar6 + psVar6[1];
  } while( true );
}

// FUN_02018748 @ 0x02018748 (872 bytes) -- Tick all active layer animation instances
void FUN_02018748(int param_1)

{
  u32 uVar1;
  short *psVar2;
  int extraout_r1 = 0;
  u32 *puVar3;
  int iVar4;
  int iVar5;
  u32 *puVar6;
  u32 *puVar7;
  int *piVar8;
  u16 local_28;
  short local_26;
  
  puVar3 = DAT_02018ab4;
  uVar1 = DAT_02018ab0;
  for (piVar8 = *(int **)(param_1 + 0xc); piVar8 != (int *)0x0; piVar8 = (int *)*piVar8) {
    *puVar3 = piVar8[1];
    if ((piVar8[1] != 0) && (piVar8[10] != 0)) {
      local_28 = 0;
      local_26 = 0;
      *(int **)(param_1 + 0x18) = piVar8;
      *DAT_02018ab8 = piVar8[3] - piVar8[2];
      if ((int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1d) < 0) {
        psVar2 = (short *)piVar8[1];
        iVar4 = 0;
        if (0 < *psVar2) {
          do {
            iVar5 = piVar8[2];
            iVar4 = (int)psVar2[psVar2[iVar4 + 1]];
            if ((int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1f) < 0) {
              if (iVar4 != 0) {
                FUN_020464b0(iVar5,iVar4 * 0x1000);
                iVar5 = extraout_r1;
                if (extraout_r1 < 0) {
                  iVar5 = extraout_r1 + iVar4 * 0x1000;
                }
              }
            }
            else {
              if (iVar5 < 0) {
                iVar5 = 0;
              }
              if (iVar4 * 0x1000 < iVar5) {
                iVar5 = iVar4 * 0x1000;
              }
            }
            *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x10) + 0x40;
            *(u32 *)(param_1 + 0x1c) = uVar1;
            FUN_02018254(param_1,&local_28,iVar5);
            local_26 = local_26 + 1;
            psVar2 = (short *)piVar8[1];
            iVar4 = (int)local_26;
          } while (iVar4 < *psVar2);
        }
      }
      else {
        if ((int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1f) < 0) {
          if (*(short *)(piVar8 + 4) != 0) {
            iVar4 = piVar8[2];
            if (iVar4 < 0) {
              do {
                piVar8[2] = piVar8[2] + *(short *)(piVar8 + 4) * 0x1000;
                iVar4 = piVar8[2];
              } while (iVar4 < 0);
            }
            iVar5 = *(short *)(piVar8 + 4) * 0x1000;
            if (iVar5 < iVar4) {
              do {
                piVar8[2] = piVar8[2] - iVar5;
                iVar5 = *(short *)(piVar8 + 4) * 0x1000;
              } while (iVar5 < piVar8[2]);
            }
          }
        }
        else {
          if (piVar8[2] < 0) {
            piVar8[2] = 0;
          }
          if (*(short *)(piVar8 + 4) * 0x1000 < piVar8[2]) {
            piVar8[2] = *(short *)(piVar8 + 4) * 0x1000;
          }
        }
        *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x10) + 0x40;
        *(u32 *)(param_1 + 0x1c) = uVar1;
        FUN_0202cc10((void *)(long)(param_1 + 0x20),0,0x40);
        FUN_02018254(param_1,&local_28,piVar8[2]);
      }
      piVar8[3] = piVar8[2];
      if (-1 < (int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1e)) {
        *(u16 *)((int)piVar8 + 0x1a) = *(u16 *)((int)piVar8 + 0x1a) & 0xffef;
        if ((int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1f) < 0) {
          piVar8[2] = piVar8[2] + (int)*(short *)((int)piVar8 + 0x12);
        }
        else {
          iVar4 = (int)*(short *)((int)piVar8 + 0x12);
          if ((iVar4 < 1) || (piVar8[2] < *(short *)(piVar8 + 4) * 0x1000)) {
            if ((iVar4 < 0) && (piVar8[2] < 1)) {
              *(u16 *)((int)piVar8 + 0x1a) = *(u16 *)((int)piVar8 + 0x1a) | 0x10;
              if (-1 < (int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1c)) {
                piVar8[1] = 0;
              }
            }
            else {
              piVar8[2] = piVar8[2] + iVar4;
            }
          }
          else {
            *(u16 *)((int)piVar8 + 0x1a) = *(u16 *)((int)piVar8 + 0x1a) | 0x10;
            if (-1 < (int)((u32)*(u16 *)((int)piVar8 + 0x1a) << 0x1c)) {
              piVar8[1] = 0;
            }
          }
        }
      }
    }
    if (((piVar8[1] == 0) || (piVar8[10] == 0)) && ((void (*)(void))(long)piVar8[0xc] != (void (*)(void))0)) {
      ((void (*)(int *))piVar8[0xc])(piVar8);
    }
  }
  puVar3 = (u32 *)0x0;
  puVar6 = *(u32 **)(param_1 + 0xc);
  if (*(u32 **)(param_1 + 0xc) != (u32 *)0x0) {
    do {
      if ((puVar6[1] == 0) || (puVar6[10] == 0)) {
        **(u32 **)(param_1 + 8) = (u32)puVar6;
        *(u32 **)(param_1 + 8) = puVar6;
        if (puVar3 == (u32 *)0x0) {
          *(u32 *)(param_1 + 0xc) = *puVar6;
        }
        else {
          *puVar3 = *puVar6;
        }
        puVar7 = (u32 *)*puVar6;
      }
      else {
        puVar7 = (u32 *)*puVar6;
        puVar3 = puVar6;
      }
      puVar6 = puVar7;
    } while (puVar7 != (u32 *)0x0);
    return;
  }
  return;
}

// FUN_02018abc @ 0x02018abc (252 bytes) -- Create layer animation instance from data
void FUN_02018abc(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  u32 uVar2;
  u32 *puVar3;
  int iVar4;
  void (*UNRECOVERED_JUMPTABLE)(void) = (void (*)(void))0;
  
  if (param_2 == 0) {
    return;
  }
  if (param_3 != 0) {
    puVar3 = (u32 *)FUN_02018bbc(param_1);
    if (puVar3 != (u32 *)0x0) {
      *puVar3 = *(u32 *)(param_1 + 0xc);
      *(u32 **)(param_1 + 0xc) = puVar3;
      puVar3[1] = param_2;
      sVar1 = *(short *)(param_2 + *(short *)(param_2 + 2) * 2);
      *(short *)(puVar3 + 4) = sVar1;
      if (param_4 < 0) {
        sVar1 = *(short *)(puVar3 + 4);
      }
      iVar4 = (int)sVar1;
      if (param_4 >= 0) {
        iVar4 = 0;
      }
      puVar3[2] = iVar4;
      puVar3[3] = puVar3[2] - param_4;
      *(short *)((int)puVar3 + 0x12) = (short)param_4;
      uVar2 = DAT_02018bb8;
      *(u16 *)((int)puVar3 + 0x1a) = *(u16 *)((int)puVar3 + 0x1a) & 0xfffe;
      *(u16 *)((int)puVar3 + 0x1a) = *(u16 *)((int)puVar3 + 0x1a) & 0xfffd;
      *(u16 *)((int)puVar3 + 0x1a) = *(u16 *)((int)puVar3 + 0x1a) & 0xfffb;
      *(u16 *)((int)puVar3 + 0x1a) = *(u16 *)((int)puVar3 + 0x1a) & 0xfff7;
      *(u16 *)((int)puVar3 + 0x1a) = *(u16 *)((int)puVar3 + 0x1a) & 0xffef;
      puVar3[10] = param_3;
      puVar3[0xb] = 0;
      puVar3[7] = 0;
      puVar3[8] = 0;
      puVar3[9] = 0;
      puVar3[0xc] = uVar2;
      puVar3[0xd] = 0;
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x02018b08. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    if (UNRECOVERED_JUMPTABLE) (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  return;
}

// FUN_02018be0 @ 0x02018be0 (196 bytes) -- Initialize layer animation linked list
int * FUN_02018be0(int *param_1,int param_2,int param_3,int param_4)

{
  u32 uVar1;
  int iVar2;
  u32 *puVar3;
  u32 *puVar4;
  
  FUN_0202cc10((void *)param_1,0,100);
  FUN_0202cc10((void *)(long)param_2,0,param_3 * 0x38 + param_4 * 0x40);
  *param_1 = param_2;
  param_1[4] = param_3 * 0x38 + *param_1;
  uVar1 = param_1[4];
  if (DAT_02018ca4 < uVar1) {
    FUN_0202cd68(DAT_02018ca4,uVar1,0x40);
  }
  else {
    FUN_0202cc94(DAT_02018ca4,uVar1,0x40);
  }
  puVar3 = (u32 *)*param_1;
  param_1[1] = (int)puVar3;
  iVar2 = 0;
  puVar4 = puVar3;
  if (0 < param_3 + -1) {
    do {
      puVar3 = puVar4 + 0xe;
      iVar2 = iVar2 + 1;
      *puVar4 = (u32)puVar3;
      puVar4 = puVar3;
    } while (iVar2 < param_3 + -1);
  }
  *puVar3 = 0;
  param_1[2] = (int)puVar3;
  param_1[3] = 0;
  return param_1;
}

// FUN_02018ca8 @ 0x02018ca8 (100 bytes) -- Apply brightness fade to palette
void FUN_02018ca8(u32 *param_1,int param_2,int param_3,int param_4)

{
  u32 uVar1;
  u32 uVar2;
  
  uVar2 = DAT_02018d10;
  uVar1 = DAT_02018d0c;
  param_2 = param_2 * param_3;
  if (param_2 < 1) {
    return;
  }
  do {
    param_2 = param_2 + -1;
    *param_1 = uVar1 & (*param_1 & uVar1) * param_4 >> 4 | (uVar1 & *param_1 >> 4) * param_4 & uVar2
    ;
    param_1 = param_1 + 1;
  } while (0 < param_2);
  return;
}

// FUN_02018d14 @ 0x02018d14 (388 bytes) -- Blit sprite tiles with additive blending
void FUN_02018d14(u32 *param_1,int param_2,int param_3,u32 param_4,int param_5,int param_6,
                 int param_7,int param_8)

{
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  u32 *puVar4;
  int iVar5;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  u32 uVar9;
  int iVar10;
  
  if (param_5 < 0) {
    iVar1 = param_5 * param_2;
    param_3 = param_3 + param_5;
    param_5 = 0;
    param_1 = param_1 + -iVar1;
  }
  if (param_8 < param_5 + param_3) {
    param_3 = param_8 - param_5;
  }
  iVar1 = param_7 * 8;
  iVar5 = param_4 + param_2 * 8;
  if (iVar1 < iVar5) {
    iVar5 = iVar1;
  }
  param_6 = param_6 + param_5 * param_7 * 4;
  if (0 < param_3) {
    do {
      param_3 = param_3 + -1;
      uVar6 = 0;
      uVar3 = param_4;
      if ((int)param_4 < iVar5) {
        iVar10 = (int)param_4 >> 3;
        puVar4 = param_1;
        do {
          uVar2 = *puVar4;
          if (-1 < (int)uVar3) {
            uVar9 = *(u32 *)(param_6 + iVar10 * 4);
            uVar6 = uVar6 | uVar2 << (param_4 & 7) * 4;
            uVar7 = (uVar9 & uVar6) + (((uVar9 ^ uVar6) & DAT_02018e98) >> 1) & DAT_02018e9c;
            uVar8 = uVar7 | uVar7 >> 1;
            *(u32 *)(param_6 + iVar10 * 4) = uVar9 + uVar6 + uVar7 * -2 | uVar8 | uVar8 >> 2;
          }
          uVar6 = uVar2 >> ((param_4 & 7) * -4 + 0x20 & 0xff);
          iVar10 = iVar10 + 1;
          uVar3 = uVar3 + 8;
          puVar4 = puVar4 + 1;
        } while ((int)uVar3 < iVar5);
      }
      if (((int)uVar3 < iVar1) && (-1 < (int)uVar3)) {
        uVar2 = *(u32 *)(param_6 + ((int)uVar3 >> 3) * 4);
        uVar8 = (uVar2 & uVar6) + (((uVar2 ^ uVar6) & DAT_02018e98) >> 1) & DAT_02018e9c;
        uVar7 = uVar8 | uVar8 >> 1;
        *(u32 *)(param_6 + ((int)uVar3 >> 3) * 4) = uVar2 + uVar6 + uVar8 * -2 | uVar7 | uVar7 >> 2
        ;
      }
      param_1 = param_1 + param_2;
      param_6 = param_6 + param_7 * 4;
    } while (0 < param_3);
    return;
  }
  return;
}

// FUN_02018ed0 @ 0x02018ed0 (56 bytes) -- Process all layer animation channels
void FUN_02018ed0(void)

{
  int iVar1;
#ifdef HOST_PORT
  intptr_t iVar2;
#else
  int iVar2;
#endif
  int *piVar3;
  u32 uVar4;
  int iVar5;
  
  piVar3 = DAT_02018f0c;
  iVar2 = DAT_02018f08;
  iVar5 = 0;
  do {
    uVar4 = FUN_02016920(*(u32 *)(iVar2 + iVar5 * 4));
    iVar1 = iVar5 * 4;
    iVar5 = iVar5 + 1;
    *(u32 *)(*piVar3 + iVar1 + 0x520) = uVar4;
  } while (iVar5 < 0xe);
  return;
}

