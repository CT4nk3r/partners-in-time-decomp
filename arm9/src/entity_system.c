/**
 * Entity System — Sprite sorting, VRAM tile management, animation callbacks
 *
 * This module handles the entity/sprite subsystem including render order sorting,
 * VRAM tile allocation, animation frame processing, and sprite property management.
 *
 * Decompiled from:
 *   FUN_020079b4 @ 0x020079B4 (184 bytes) - Sort render queue by priority (bubble sort)
 *   FUN_02007a74 @ 0x02007A74 (140 bytes) - Add entry to render queue
 *   FUN_02007b14 @ 0x02007B14 (56 bytes)  - Set sprite palette/color params
 *   FUN_02007b5c @ 0x02007B5C (408 bytes) - VRAM tile block allocator
 *   FUN_02007d2c @ 0x02007D2C (108 bytes) - Free VRAM tile block
 *   FUN_02007d98 @ 0x02007D98 (80 bytes)  - Compute tile VRAM address
 *   FUN_020084a0 @ 0x020084A0 (140 bytes) - Remove node from VRAM alloc list
 *   FUN_02008534 @ 0x02008534 (2636 bytes) - Main VRAM tile allocator (large)
 *   FUN_02008f8c @ 0x02008F8C (48 bytes)  - Initialize sprite buffer
 *   FUN_02008fc0 @ 0x02008FC0 (72 bytes)  - Clear render state tables
 *   FUN_02009100 @ 0x02009100 (12 bytes)  - Get VRAM lookup base
 *   FUN_02009110 @ 0x02009110 (40 bytes)  - Initialize VRAM table entry
 *   FUN_02009150 @ 0x02009150 (16 bytes)  - Call memory copy wrapper
 *   FUN_02009160 @ 0x02009160 (28 bytes)  - Call graphics function with param split
 *   FUN_0200917c @ 0x0200917C (28 bytes)  - Call graphics function variant A
 *   FUN_02009198 @ 0x02009198 (28 bytes)  - Call graphics function variant B
 *   FUN_020091b4 @ 0x020091B4 (28 bytes)  - Call graphics function variant C
 *   FUN_020091ec @ 0x020091EC (48 bytes)  - Get sprite width difference
 *   FUN_0200921c @ 0x0200921C (176 bytes) - Process sprite animation with callbacks
 *   FUN_020092d4 @ 0x020092D4 (72 bytes)  - Get color palette based on condition
 *   FUN_02009328 @ 0x02009328 (84 bytes)  - Copy sprite properties between objects
 *   FUN_0200937c @ 0x0200937C (88 bytes)  - Set sprite visibility flag
 *   FUN_020093d4 @ 0x020093D4 (104 bytes) - Update sprite animation frame timing
 */

#include "types.h"

/* Forward declarations */
extern u32 FUN_02029964(u32, u32, u32, u32);
extern void FUN_0202cd68(void);
extern void FUN_0202ce44(u32, u32, u32, u32);
extern void FUN_0202cec0(u32, u32, u32, u32);
extern void FUN_0202cf40(u32, u32, u32, u32);
extern void FUN_0202cfbc(u32, u32, u32, u32);
extern void FUN_0200c3a0(u32);

/* Static data references */
static u8 *DAT_02007a6c;      /* render queue count per slot */
static u32 DAT_02007a70;      /* render queue data base */
static u8 *DAT_02007b00;      /* render entry count per slot */
static u32 DAT_02007b04;      /* render entry data base */
static u8 *DAT_02007b08;      /* default render start offset */
static u32 DAT_02007b0c;      /* render end position table */
static u32 DAT_02007b10;      /* render offset accumulator table */
static u32 DAT_02007b4c;      /* palette index table */
static u32 DAT_02007b50;      /* palette param A table */
static u32 DAT_02007b54;      /* palette param B table */
static u32 DAT_02007b58;      /* palette param C table */
static u32 *DAT_0200852c;     /* VRAM alloc list head */
static u32 *DAT_02008530;     /* VRAM alloc list tail */
static u32 *DAT_02007de8;     /* tile VRAM base (16-color mode) */
static u32 *DAT_02007dec;     /* tile VRAM base (256-color mode) */
static u32 *sprite_buffer;    /* sprite DMA buffer pointer */
static u32 render_state_low;  /* render state table 0 */
static u32 render_state_1;    /* render state table 1 */
static u32 render_state_2;    /* render state table 2 */
static u32 render_state_3;    /* render state table 3 */
static u32 render_state_4;    /* render state table 4 */
static u32 render_state_5;    /* render state table 5 */
static u32 vram_lookup_base;  /* VRAM lookup base address */
static u32 *vram_table;       /* VRAM table pointer */
static u32 DAT_0200931c;      /* color palette dark */
static u32 DAT_02009320;      /* color palette normal */
static u32 DAT_02009324;      /* color palette light */
static u32 *func_table;       /* animation callback table */
static u32 func_table_ptr;    /* animation callback base */

// FUN_020079b4 @ 0x020079B4 (184 bytes) — Sort render queue by priority (bubble sort)
void FUN_020079b4(int param_1)
{
    u32 *puVar1;
    u32 *puVar2;
    int iVar3;
    int iVar4;
    u32 uVar5;
    u32 uVar6;
    u32 *puVar7;
    int iVar8;

    if (*(u8 *)(DAT_02007a6c + param_1) < 2) {
        return;
    }
    iVar8 = *(u8 *)(DAT_02007a6c + param_1) - 1;
    puVar7 = (u32 *)(DAT_02007a70 + param_1 * 0x400);
    if (-1 < iVar8) {
        do {
            iVar3 = -1;
            iVar4 = 1;
            puVar1 = puVar7;
            if (0 < iVar8) {
                do {
                    puVar2 = puVar1 + 2;
                    if (puVar7[iVar4 * 2] < puVar7[(iVar4 - 1) * 2]) {
                        uVar6 = *puVar2;
                        uVar5 = puVar1[3];
                        *puVar2 = *puVar1;
                        puVar1[3] = puVar1[1];
                        *puVar1 = uVar6;
                        puVar1[1] = uVar5;
                        iVar3 = iVar4 - 1;
                    }
                    iVar4 = iVar4 + 1;
                    puVar1 = puVar2;
                } while (iVar4 <= iVar8);
            }
            iVar8 = iVar3;
        } while (-1 < iVar3);
        return;
    }
    return;
}

// FUN_02007a74 @ 0x02007A74 (140 bytes) — Add entry to render queue
void FUN_02007a74(int param_1, u32 param_2, int param_3, char param_4)
{
    int iVar1;
    int iVar2;
    int iVar3;
    char *pcVar4;
    u32 uVar5;

    if (param_3 != 0) {
        uVar5 = (u32)*(u8 *)(DAT_02007b00 + param_1);
        iVar3 = DAT_02007b04 + param_1 * 0x400;
        pcVar4 = (char *)(DAT_02007b00 + param_1);
        *(u32 *)(iVar3 + uVar5 * 8) = param_2;
        iVar3 = iVar3 + uVar5 * 8;
        if (uVar5 == 0) {
            *(u8 *)(iVar3 + 4) = *(u8 *)(DAT_02007b08 + param_1);
        } else {
            *(char *)(iVar3 + 4) = *(char *)(iVar3 + -4) + *(char *)(iVar3 + -3);
        }
        *(char *)(iVar3 + 5) = (char)param_3;
        iVar2 = DAT_02007b10;
        iVar1 = DAT_02007b0c;
        *pcVar4 = *pcVar4 + 1;
        *(char *)(iVar1 + param_1) = *(char *)(iVar3 + 4) + *(char *)(iVar3 + 5);
        *(char *)(iVar2 + param_1) = *(char *)(iVar2 + param_1) + param_4;
        return;
    }
    return;
}

// FUN_02007b14 @ 0x02007B14 (56 bytes) — Set sprite palette/color params
void FUN_02007b14(int param_1, u8 param_2, u8 param_3, int param_4)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar2 = DAT_02007b54;
    iVar1 = DAT_02007b50;
    if (param_4 != -1) {
        *(char *)(DAT_02007b4c + param_1) = (char)param_4;
    }
    iVar3 = DAT_02007b58;
    *(u8 *)(iVar1 + param_1) = param_2;
    *(u8 *)(iVar2 + param_1) = param_2;
    *(u8 *)(iVar3 + param_1) = param_3;
    return;
}

// FUN_02007d98 @ 0x02007D98 (80 bytes) — Compute tile VRAM address
int FUN_02007d98(int param_1)
{
    if (*(u8 *)(param_1 + 0x11) == 0x10) {
        return *(int *)(DAT_02007de8 + (*(u8 *)(param_1 + 0x13) & 3) * 4) +
               (u32)*(u8 *)(param_1 + 0x10) * 0x20;
    }
    return *(int *)(DAT_02007dec + (*(u8 *)(param_1 + 0x13) & 3) * 4) +
           (u32)*(u8 *)(param_1 + 0x11) * 0x200 + (u32)*(u8 *)(param_1 + 0x10) * 0x20;
}

// FUN_020084a0 @ 0x020084A0 (140 bytes) — Remove node from VRAM alloc linked list
void FUN_020084a0(int param_1)
{
    u32 uVar1;
    int iVar2;

    uVar1 = *(u8 *)(param_1 + 0x14) & 3;
    iVar2 = *(int *)((u32)DAT_0200852c + uVar1 * 4);
    if ((iVar2 != 0) && (iVar2 == param_1)) {
        *(u32 *)((u32)DAT_0200852c + uVar1 * 4) = *(u32 *)(param_1 + 0xc);
    }
    iVar2 = *(int *)((u32)DAT_02008530 + uVar1 * 4);
    if ((iVar2 != 0) && (iVar2 == param_1)) {
        *(u32 *)((u32)DAT_02008530 + uVar1 * 4) = *(u32 *)(param_1 + 8);
    }
    if (*(int *)(param_1 + 8) != 0) {
        *(u32 *)(*(int *)(param_1 + 8) + 0xc) = *(u32 *)(param_1 + 0xc);
    }
    if (*(int *)(param_1 + 0xc) != 0) {
        *(u32 *)(*(int *)(param_1 + 0xc) + 8) = *(u32 *)(param_1 + 8);
    }
    *(u32 *)(param_1 + 0xc) = 0;
    *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 0xc);
    *(u8 *)(param_1 + 0x14) = *(u8 *)(param_1 + 0x14) & 0xdf;
    return;
}

// FUN_02008f8c @ 0x02008F8C (48 bytes) — Initialize sprite buffer
void FUN_02008f8c(void)
{
    u32 val;

    val = FUN_02029964(2, 0x400, 0, 0);
    *sprite_buffer = val;
    return;
}

// FUN_02008fc0 @ 0x02008FC0 (72 bytes) — Clear render state tables
void FUN_02008fc0(int param_1)
{
    u32 val1;
    u32 val2;
    u32 val3;
    u32 val4;
    u32 val5;

    val5 = render_state_5;
    val4 = render_state_4;
    val3 = render_state_3;
    val2 = render_state_2;
    val1 = render_state_1;
    if (render_state_low) *(u32 *)(render_state_low + param_1 * 4) = 0;
    if (val1) *(u32 *)(val1 + param_1 * 4) = 0;
    if (val2) *(u32 *)(val2 + param_1 * 4) = 0;
    if (val3) *(u32 *)(val3 + param_1 * 4) = 0;
    if (val4) *(u32 *)(val4 + param_1 * 4) = 0;
    if (val5) *(u32 *)(val5 + param_1 * 4) = 0;
    return;
}

// FUN_02009100 @ 0x02009100 (12 bytes) — Get VRAM lookup base
void FUN_02009100(u32 *param_1)
{
    *param_1 = vram_lookup_base;
    return;
}

// FUN_02009110 @ 0x02009110 (40 bytes) — Initialize VRAM table entry
void FUN_02009110(u32 *param_1)
{
    if (!param_1) return;
    *param_1 = vram_table ? *vram_table : 0;
    *(u8 *)(param_1 + 4) = 0xff;
    param_1[2] = 0;
    param_1[1] = param_1[2];
    param_1[3] = 0;
    return;
}

// FUN_02009150 @ 0x02009150 (16 bytes) — Call memory copy wrapper
void FUN_02009150(void)
{
    FUN_0202cd68();
    return;
}

// FUN_02009160 @ 0x02009160 (28 bytes) — Call graphics function with param split
void FUN_02009160(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0202ce44(param_1, param_2, param_4 >> 0x11, param_4 & 0xffff);
    return;
}

// FUN_0200917c @ 0x0200917C (28 bytes) — Call graphics function variant A
void FUN_0200917c(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0202cec0(param_1, param_2, param_4 >> 0x10, param_4 & 0xffff);
    return;
}

// FUN_02009198 @ 0x02009198 (28 bytes) — Call graphics function variant B
void FUN_02009198(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0202cf40(param_1, param_2, param_4 >> 0x11, param_4 & 0xffff);
    return;
}

// FUN_020091b4 @ 0x020091B4 (28 bytes) — Call graphics function variant C
void FUN_020091b4(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    FUN_0202cfbc(param_1, param_2, param_4 >> 0x10, param_4 & 0xffff);
    return;
}

// FUN_020091ec @ 0x020091EC (48 bytes) — Get sprite width difference
s16 FUN_020091ec(int param_1, int param_2)
{
    if (param_2 == -1) {
        param_2 = (int)*(s16 *)(param_1 + 0x54);
    }
    return *(s16 *)(*(int *)(param_1 + 0x48) + param_2 * 8 + 2) -
           *(s16 *)(*(int *)(param_1 + 0x48) + param_2 * 8);
}

// FUN_0200921c @ 0x0200921C (176 bytes) — Process sprite animation with callbacks
void FUN_0200921c(int param_1, u32 param_2, u32 param_3, int param_4)
{
    int base;
    u8 byte_val;
    int *func_ptr_local;
    u8 local_28;
    u8 local_27[7];

    base = param_1;
    if (param_4 == 0) {
        base = func_table_ptr;
    }
    func_ptr_local = *(int **)(func_table + param_1 * 4);
    if (param_4 == 0) {
        param_4 = base + param_1 * 0x400;
    }
    local_28 = (u8)param_2;
    local_27[0] = (u8)param_3;
    if (func_ptr_local != (int *)0) {
        do {
            (*(void (*)(int *, int, u8 *, u8 *))(*(int *)((int)func_ptr_local + 8)))(func_ptr_local, param_4, &local_28, local_27);
            byte_val = (*(u8 (*)(void))(*(int *)((int)func_ptr_local + 0x44)))();
            FUN_02007a74(param_1, byte_val, local_28 - param_2, local_27[0] - param_3);
            param_2 = (u32)local_28;
            param_3 = (u32)local_27[0];
            func_ptr_local = (int *)func_ptr_local[2];
        } while (func_ptr_local != (int *)0);
        return;
    }
    return;
}

// FUN_020092d4 @ 0x020092D4 (72 bytes) — Get color palette based on condition
u32 FUN_020092d4(int param_1, int param_2)
{
    u32 palette;

    if (param_2 == 0) {
        palette = DAT_0200931c;
        if (*(int *)(param_1 + 0x7c) << 9 < 0) {
            palette = DAT_02009320;
        }
        return palette;
    }
    FUN_0200c3a0(param_2);
    return DAT_02009324;
}

// FUN_02009328 @ 0x02009328 (84 bytes) — Copy sprite properties between objects
void FUN_02009328(int *param_1, int *param_2)
{
    u8 type;

    type = (*(u8 (*)(void))(*(int *)(*param_1 + 0x3c)))();
    (*(void (*)(int *, u8, int))(*(int *)(*param_2 + 0x34)))(param_2, type, (int)*(s16 *)((int)param_1 + 0x56));
    param_2[0x19] = param_1[0x19];
    *(u16 *)((int)param_2 + 0x5a) = *(u16 *)((int)param_1 + 0x5a);
    return;
}

// FUN_0200937c @ 0x0200937C (88 bytes) — Set sprite visibility flag
void FUN_0200937c(int *param_1, u32 param_2)
{
    if ((-1 < param_1[0x1f] << 0x17) && (param_2 != 0)) {
        (*(void (*)(int *, int))(*(int *)(*param_1 + 0x48)))(param_1, 1);
    }
    param_1[0x1f] = param_1[0x1f] & 0xfffffeffU | (param_2 & 1) << 8;
    return;
}

// FUN_020093d4 @ 0x020093D4 (104 bytes) — Update sprite animation frame timing
void FUN_020093d4(int param_1, int param_2)
{
    if ((int)(((u32)*(u16 *)
                    (*(int *)(param_1 + 0x44) +
                     ((int)*(s16 *)(param_1 + 0x56) +
                     (u32)*(u16 *)(*(int *)(param_1 + 0x48) + *(s16 *)(param_1 + 0x54) * 8)) *
                     4 + 2) << 0x17) >> 0xf) < *(int *)(param_1 + 100)) {
        *(int *)(param_1 + 100) = *(int *)(param_1 + 100) - (int)*(s16 *)(param_1 + 0x5a);
        *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + param_2;
    }
    *(s16 *)(param_1 + 0x5a) = (s16)param_2;
    return;
}
