/**
 * Object Manager — Sprite data sizes, tile allocation, animation rendering
 *
 * This module manages game object lifecycle: calculating sprite data sizes,
 * allocating/freeing VRAM tiles with linked-list management, rendering sprites
 * with rotation/scale, and loading animation data from ROM.
 *
 * Decompiled from:
 *   FUN_02009460 @ 0x02009460 (96 bytes)   - Get sprite data size from header
 *   FUN_02009560 @ 0x02009560 (152 bytes)  - Compute extended sprite data size
 *   FUN_0200960c @ 0x0200960C (108 bytes)  - Copy tile data to VRAM slot
 *   FUN_02009678 @ 0x02009678 (1648 bytes) - VRAM tile block manager (large)
 *   FUN_02009cec @ 0x02009CEC (272 bytes)  - Update VRAM tile allocation
 *   FUN_02009e00 @ 0x02009E00 (444 bytes)  - Reload tile data for screen
 *   FUN_02009fc4 @ 0x02009FC4 (760 bytes)  - Load tile data with palette
 *   FUN_0200a2c4 @ 0x0200A2C4 (352 bytes)  - Get sprite sub-resource pointer
 *   FUN_0200a424 @ 0x0200A424 (104 bytes)  - Set sprite base properties
 *   FUN_0200a494 @ 0x0200A494 (2868 bytes) - Full sprite initialization (large)
 *   FUN_0200aff0 @ 0x0200AFF0 (96 bytes)   - Check if sprite collision distance exceeded
 *   FUN_0200b050 @ 0x0200B050 (552 bytes)  - Update animation frame counter
 *   FUN_0200b278 @ 0x0200B278 (196 bytes)  - Set animation frame and row
 *   FUN_0200b33c @ 0x0200B33C (832 bytes)  - Draw sprite with rotation/scale
 *   FUN_0200b68c @ 0x0200B68C (252 bytes)  - Render sprite if valid
 *   FUN_0200b790 @ 0x0200B790 (96 bytes)   - Check draw validity condition
 *   FUN_0200b958 @ 0x0200B958 (156 bytes)  - Insert sprite into display list
 *   FUN_0200b9fc @ 0x0200B9FC (1024 bytes) - Load sprite animation data (large)
 *   FUN_0200be04 @ 0x0200BE04 (12 bytes)   - Indirect function call wrapper
 *   FUN_0200be14 @ 0x0200BE14 (48 bytes)   - Initialize object type 0
 *   FUN_0200be48 @ 0x0200BE48 (56 bytes)   - Initialize object type 1
 *   FUN_0200be84 @ 0x0200BE84 (48 bytes)   - Initialize object type 2
 *   FUN_0200beb8 @ 0x0200BEB8 (812 bytes)  - Initialize and load object from data block
 */

#include "types.h"

/* Forward declarations */
extern void FUN_020084a0(int);
extern void FUN_02009100(u32 *);
extern void FUN_02009110(u32 *);
extern void FUN_0200a2c4(void *, u32);
extern u32 FUN_02009460(u16 *);
extern void FUN_0200a424(int *, u32);
extern void FUN_0200b278(int *, int, int);
extern void FUN_0200b33c(u32, u16 *, u32, u32, int);
extern void FUN_0202cd68(u32, u32, u32);
extern void FUN_0202cc94(u32, u32, u32);
extern void FUN_0202cc10(void *, u32, u32);
extern void FUN_0203a04c(u32, u32);
extern void FUN_0203b5b4(u32, u32, u32, u32);
extern void FUN_02008534(int *, char, u32, u32, u32, u32, u32, u32);
extern void FUN_0200960c(u32, u32, u32, u8);
extern void FUN_0200c1ec(u32, u32, u16 *);
extern void FUN_02009e00(char, int, u16 *);
extern void FUN_02009fc4(char, int, u16 *, u32);
extern void thunk_FUN_02029ab8(void *);
extern u32 FUN_02035530(void);
extern u32 FUN_02035548(void);
extern u32 FUN_02035500(void);
extern u32 FUN_0202d9d0(u32, u8);

/* Static data references */
static u32 DAT_020094c0;      /* sprite data size mask */
static u32 *memory_table_low;  /* VRAM allocation list head per bank */
static u32 *memory_table_high; /* VRAM allocation list tail per bank */
static u32 **memory_table_ptr; /* VRAM bank config pointers */
static u32 DAT_0200bdfc;       /* VRAM bank config table base */
static u32 DAT_0200be00;       /* VRAM size mask */
static u32 DAT_0200be44;       /* vtable for object type 0 */
static u32 DAT_0200be80;       /* vtable for object type 1 */
static u32 DAT_0200beb4;       /* vtable for object type 2 */
static u32 DAT_0200c1e4;       /* object data config base */
static u32 *DAT_0200b688;      /* DMA channel pointer */
static u32 *DAT_0200b78c;      /* VRAM offset table A */
static u32 *DAT_0200b788;      /* VRAM config table */
static u32 DAT_0200b67c;       /* render function table */
static u32 *DAT_0200b680;      /* sprite size lookup */
static u32 *DAT_0200b684;      /* sprite stride lookup */
static u32 *DAT_0200b9f4;      /* display list head table */
static u32 DAT_0200b9f8;       /* display list tail table */
static void (*DAT_0200be10)(int); /* indirect cleanup function */

// FUN_02009460 @ 0x02009460 (96 bytes) — Get sprite data size from header
u32 FUN_02009460_impl(u16 *param_1)
{
    u32 uVar1;

    uVar1 = ((u32)*param_1 << 0x19) >> 0x1d;
    if (uVar1 < 5) {
        if ((int)((u32)*param_1 << 0x11) < 0) {
            uVar1 = ((u32)(*(int *)(param_1 + 4) << 0xc) >> 0x16) << 6;
        } else {
            uVar1 = (u32)(*(int *)(param_1 + 4) << 0x16) >> 0x11;
        }
        return uVar1;
    }
    return (DAT_020094c0 & *(u32 *)(param_1 + 4) >> ((uVar1 - 5) * 10 & 0xff)) << uVar1;
}

// FUN_0200aff0 @ 0x0200AFF0 (96 bytes) — Check if sprite collision distance exceeded
BOOL FUN_0200aff0(int param_1)
{
    int iVar1;

    iVar1 = (int)*(s16 *)(param_1 + 0x5a);
    if (iVar1 < 1) {
        iVar1 = -iVar1;
    }
    return (int)(*(u16 *)
                 (*(int *)(param_1 + 0x44) +
                  ((int)*(s16 *)(param_1 + 0x56) +
                  (u32)*(u16 *)(*(int *)(param_1 + 0x48) + *(s16 *)(param_1 + 0x54) * 8)) * 4 +
                 2) & 0x1ff) <= *(int *)(param_1 + 100) + iVar1 >> 8;
}

// FUN_0200b790 @ 0x0200B790 (96 bytes) — Check draw validity condition
u32 FUN_0200b790(int param_1, int param_2)
{
    if (param_2 != 0) {
        FUN_020084a0(param_1 + 0x14);
        return 1;
    }
    if ((u32)(*(int *)(param_1 + 0x7c) << 0xd) >> 0x1d == 3) {
        FUN_020084a0(param_1 + 0x14);
        return 1;
    }
    return 0;
}

// FUN_0200b958 @ 0x0200B958 (156 bytes) — Insert sprite into display list
void FUN_0200b958(int param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar3 = DAT_0200b9f8;
    iVar2 = (int)DAT_0200b9f4;
    if (-1 < *(int *)(param_1 + 0x7c) << 0x1b) {
        iVar1 = (u32)*(u8 *)(param_1 + 0x10) * 4;
        if (*(int *)(iVar2 + (u32)*(u8 *)(param_1 + 0x10) * 4) == 0) {
            *(int *)(iVar3 + iVar1) = param_1;
            *(u32 *)(iVar2 + (u32)*(u8 *)(param_1 + 0x10) * 4) =
                 *(u32 *)(iVar3 + (u32)*(u8 *)(param_1 + 0x10) * 4);
            *(u32 *)(param_1 + 4) = 0;
            *(u32 *)(param_1 + 8) = 0;
        } else {
            *(int *)(*(int *)(iVar3 + iVar1) + 8) = param_1;
            *(u32 *)(param_1 + 4) = *(u32 *)(iVar3 + (u32)*(u8 *)(param_1 + 0x10) * 4);
            *(u32 *)(param_1 + 8) = 0;
            *(int *)(iVar3 + (u32)*(u8 *)(param_1 + 0x10) * 4) = param_1;
        }
        *(u32 *)(param_1 + 0x7c) = *(u32 *)(param_1 + 0x7c) | 0x10;
        return;
    }
    return;
}

// FUN_0200be04 @ 0x0200BE04 (12 bytes) — Indirect function call wrapper
void FUN_0200be04(int param_1)
{
    (*DAT_0200be10)(param_1 + 0x14);
    return;
}

// FUN_0200be14 @ 0x0200BE14 (48 bytes) — Initialize object type 0
int *FUN_0200be14(int *param_1)
{
    *param_1 = DAT_0200be44;
    (*(void (*)(void))(*(int *)(*param_1 + 0x1c)))();
    FUN_02009100((u32 *)param_1);
    return param_1;
}

// FUN_0200be48 @ 0x0200BE48 (56 bytes) — Initialize object type 1
int *FUN_0200be48(int *param_1)
{
    *param_1 = DAT_0200be80;
    (*(void (*)(void))(*(int *)(*param_1 + 0x1c)))();
    FUN_02009100((u32 *)param_1);
    thunk_FUN_02029ab8(param_1);
    return param_1;
}

// FUN_0200be84 @ 0x0200BE84 (48 bytes) — Initialize object type 2
int *FUN_0200be84(int *param_1)
{
    *param_1 = DAT_0200beb4;
    (*(void (*)(void))(*(int *)(*param_1 + 0x1c)))();
    FUN_02009100((u32 *)param_1);
    return param_1;
}
