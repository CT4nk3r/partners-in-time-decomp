/**
 * Math, Mutex, and State Management Functions
 *
 * VRAM address calculations, color blending/interpolation,
 * RNG (linear congruential generator), bit allocation,
 * mutex/lock primitives, linked list operations,
 * and global flag management.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// NDS hardware register
// ============================================================

#define REG_DISPCNT  (*(vu32 *)0x04000000)

// ============================================================
// External declarations
// ============================================================

extern void struct_setup_advanced(u32 a, u32 b, u32 c, u32 d, u32 e);

// ============================================================
// VRAM address calculation (bitwise math)
// ============================================================

// FUN_02036658 @ 0x02036658 (48 bytes) — Calculate VRAM character base address
static vu8 sCharBaseCtrl0;  // control byte at DAT_02036688
u32 vram_calc_char_base_0(void)
{
    return ((REG_DISPCNT & 0x07000000) >> 24) * 0x10000
         + 0x06000000
         + ((int)(sCharBaseCtrl0 & 0x3C) >> 2) * 0x4000;
}

// FUN_020366ac @ 0x020366ac (48 bytes) — Calculate VRAM character base (variant)
static vu8 sCharBaseCtrl1;  // DAT_020366dc
u32 vram_calc_char_base_1(void)
{
    return ((REG_DISPCNT & 0x07000000) >> 24) * 0x10000
         + 0x06000000
         + ((int)(sCharBaseCtrl1 & 0x3C) >> 2) * 0x4000;
}

// FUN_02036918 @ 0x02036918 (48 bytes) — Calculate VRAM screen base address
static vu16 sScreenBaseCtrl0;  // DAT_02036948
u32 vram_calc_screen_base_0(void)
{
    return ((REG_DISPCNT & 0x38000000) >> 27) * 0x10000
         + 0x06000000
         + ((int)(sScreenBaseCtrl0 & 0x1F00) >> 8) * 0x800;
}

// FUN_0203696c @ 0x0203696c (48 bytes) — Calculate VRAM screen base (variant)
static vu16 sScreenBaseCtrl1;  // DAT_0203699c
u32 vram_calc_screen_base_1(void)
{
    return ((REG_DISPCNT & 0x38000000) >> 27) * 0x10000
         + 0x06000000
         + ((int)(sScreenBaseCtrl1 & 0x1F00) >> 8) * 0x800;
}

// ============================================================
// VRAM address getters with validation
// ============================================================

// FUN_020364e8 @ 0x020364e8 (72 bytes) — Get VRAM address with mode validation
static vu8 sVramMode0;   // DAT_02036530
static vu8 sVramFlags0;  // DAT_02036534
u32 vram_get_addr_validated_0(void)
{
    u8 mode = sVramMode0 & 7;
    if (mode > 2 && (mode > 5 || (sVramFlags0 & 0x80) != 0)) {
        return 0;
    }
    return ((sVramFlags0 & 0x3C) >> 2) * 0x4000 + 0x06200000;
}

// FUN_02036598 @ 0x02036598 (64 bytes) — Get VRAM address with mode validation (variant)
static vu8 sVramMode1;   // DAT_020365d8
static vu8 sVramFlags1;  // DAT_020365dc
u32 vram_get_addr_validated_1(void)
{
    if ((sVramMode1 & 7) > 4 && (sVramFlags1 & 0x80) != 0) {
        return 0;
    }
    return ((sVramFlags1 & 0x3C) >> 2) * 0x4000 + 0x06200000;
}

// ============================================================
// Struct setup / initialization
// ============================================================

// FUN_02036b38 @ 0x02036b38 (48 bytes) — Multi-parameter struct setup wrapper
static void struct_setup_state(void);
void struct_setup_wrapper(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5)
{
    struct_setup_state();
    struct_setup_advanced(p1, 3, p3, p4, p5);
}

// FUN_02036c20 @ 0x02036c20 (64 bytes) — Init struct with type and 3 data fields
void struct_init_full(u32 *param_1, u8 type, u32 val1, u32 val2, u32 val3)
{
    param_1[4] = 0;
    *(u8 *)param_1 = type;
    *(u32 *)(param_1[1])     = val1;
    *(u32 *)(param_1[1] + 4) = val2;
    *(u32 *)(param_1[1] + 8) = val3;
}

// FUN_02036c60 @ 0x02036c60 (36 bytes) — Init struct with type and 2 data fields
void struct_init_short(u32 *param_1, u8 type, u32 val1, u32 val2)
{
    param_1[4] = 0;
    *(u8 *)param_1 = type;
    *(u32 *)(param_1[1])     = val1;
    *(u32 *)(param_1[1] + 4) = val2;
}

// FUN_02036ca0 @ 0x02036ca0 (36 bytes) — Set struct field with flag validation
void struct_set_with_flag(u32 *param_1, int value)
{
    if (value != 0 && (*param_1 & 3) != 0) {
        param_1[4] = 1;
    }
    *(u8 *)param_1 = (u8)value;
}

// ============================================================
// Global array access
// ============================================================

// FUN_02036dbc @ 0x02036dbc (64 bytes) — Read 3 values from global array
static vu8  sArrayFlag;     // DAT_02036dfc
static vu16 sArrayVal0;     // DAT_02036e00
static vu16 sArrayVal1;     // DAT_02036e04
static vu16 sArrayVal2;     // DAT_02036e08
u32 global_array_read(u16 *out)
{
    if ((sArrayFlag & 1) == 0) {
        out[0] = sArrayVal0;
        out[1] = sArrayVal1;
        out[2] = sArrayVal2;
        return 0;
    }
    return 0xFFFFFFFF;
}

// FUN_02036e0c @ 0x02036e0c (40 bytes) — Read status flag from global
static vu32 sStatusWord;  // DAT_02036e34
u32 global_flag_read(u32 *out)
{
    if ((sStatusWord & 0x4000) == 0) {
        *out = (sStatusWord & 0x2000) >> 13;
        return 0;
    }
    return 0xFFFFFFFF;
}

// ============================================================
// Color blending / interpolation (15-bit RGB555)
// ============================================================

static const u32 RGB555_MASK = 0x03E07C1F;  // masks for R/G/B in expanded form

// FUN_0202d03c @ 0x0202d03c (56 bytes) — Blend two colors (linear interpolation)
u32 color_blend(u32 color_a, u32 color_b, int factor)
{
    u32 a_exp = (color_a | (color_a << 15)) & RGB555_MASK;
    u32 b_exp = (color_b | (color_b << 15)) & RGB555_MASK;
    u32 result = (RGB555_MASK & ((32 - factor) * a_exp >> 5))
               + (RGB555_MASK & (factor * b_exp >> 5));
    return (result | (result * 0x8000)) >> 15;
}

// FUN_0202d078 @ 0x0202d078 (60 bytes) — Blend with brightness increase (saturate)
u32 color_brighten(u32 color, u32 white, int factor)
{
    u32 c_exp = (color | (color << 15)) & RGB555_MASK;
    u32 w_exp = (white | (white << 15)) & RGB555_MASK;
    u32 result = c_exp + (RGB555_MASK & (factor * w_exp >> 5));
    u32 carry = result & ~RGB555_MASK;
    result = (result | (carry - (carry >> 5))) & RGB555_MASK;
    return (result | (result << 15)) >> 15;
}

// FUN_0202d0b8 @ 0x0202d0b8 (64 bytes) — Blend with brightness decrease (saturate)
static u32 sColorBase;  // pre-computed full-bright base
u32 color_darken(u32 color, u32 black, int factor)
{
    u32 c_exp = (color | (color << 15)) & RGB555_MASK;
    u32 b_exp = (black | (black << 15)) & RGB555_MASK;
    u32 result = c_exp + (sColorBase - (RGB555_MASK & (factor * b_exp >> 5)));
    u32 carry = result & ~RGB555_MASK;
    result = result & (carry - (carry >> 5));
    return (result | (result << 15)) >> 15;
}

// FUN_0202d15c @ 0x0202d15c (52 bytes) — Single-color interpolation toward complement
u32 color_interpolate(u32 color, int factor)
{
    u32 c_exp = (color | (color << 15)) & RGB555_MASK;
    u32 result = (RGB555_MASK & ((32 - factor) * c_exp >> 5))
               + (RGB555_MASK & (factor * (RGB555_MASK - c_exp) >> 5));
    return (result | (result * 0x8000)) >> 15;
}

// ============================================================
// Random Number Generator (LCG)
// ============================================================

// FUN_0202cba8 @ 0x0202cba8 (72 bytes) — Linear congruential generator
static u32 sRngSeed;         // current seed
static u32 sRngDefaultSeed;  // fallback seed
u32 rng_next(void)
{
    u32 value = sRngSeed;
    if (value == 0) {
        value = sRngDefaultSeed;
    }
    value = value * 0x29;
    value = (value + ((value >> 16) | ((value & 0xFFFF) << 16))) << 15;
    sRngSeed = value;
    return value >> 16;
}

// ============================================================
// Bit allocation (bitmap-based slot allocator)
// ============================================================

// FUN_0202ebc8 @ 0x0202ebc8 (64 bytes) — Find and claim first free bit in 8-bit map
static vu32 sBitmap;  // 8-slot bitmap
int bit_alloc_slot(void)
{
    u32 mask = 1;
    int index = 0;

    do {
        if ((sBitmap & mask) == 0) {
            sBitmap |= mask;
            return index;
        }
        index++;
        mask <<= 1;
    } while (index < 8);

    return -1;
}

// ============================================================
// Mutex / lock operations (IRQ-safe)
// ============================================================

// FUN_02038c4c @ 0x02038c4c (44 bytes) — Swap value atomically (IRQ disable/restore)
static vu16 sLockFlag0;   // IME register mirror
static vu32 sLockValue0;  // protected value
u32 os_swap_value(u32 new_val)
{
    u16 old_ime = sLockFlag0;
    sLockFlag0 = 0;
    u32 old_val = sLockValue0;
    sLockValue0 = new_val;
    sLockFlag0 = old_ime;
    return old_val;
}

// FUN_02038c80 @ 0x02038c80 (64 bytes) — Clear bits in flag (IRQ-safe)
static vu16 sLockFlag1;
static vu32 sLockBits1;
void os_clear_bits(u32 mask)
{
    u16 old_ime = sLockFlag1;
    sLockFlag1 = 0;
    sLockBits1 &= ~mask;
    sLockFlag1 = old_ime;
}

// FUN_02038cc8 @ 0x02038cc8 (48 bytes) — Set bits in flag, return previous (IRQ-safe)
static vu16 sLockFlag2;
static vu32 sLockBits2;
u32 os_set_bits(u32 mask)
{
    u16 old_ime = sLockFlag2;
    sLockFlag2 = 0;
    u32 old_val = sLockBits2;
    sLockBits2 = old_val | mask;
    sLockFlag2 = old_ime;
    return old_val;
}

// FUN_02038d00 @ 0x02038d00 (44 bytes) — Swap value atomically (variant)
static vu16 sLockFlag3;
static vu32 sLockValue3;
u32 os_swap_value_alt(u32 new_val)
{
    u16 old_ime = sLockFlag3;
    sLockFlag3 = 0;
    u32 old_val = sLockValue3;
    sLockValue3 = new_val;
    sLockFlag3 = old_ime;
    return old_val;
}

// ============================================================
// Callback queue operations
// ============================================================

// FUN_02038d34 @ 0x02038d34 (64 bytes) — Queue callback with index (variant A)
static vu32 *sQueueBase0A;
static vu32 *sQueueBase1A;
static vu32 *sQueueBase2A;
void queue_callback_a(int slot, u32 func, u32 param)
{
    u32 offset = (u32)slot * 0xC;
    *(u32 *)(*sQueueBase0A + offset) = func;
    *(u32 *)(*sQueueBase1A + offset) = param;
    os_set_bits(1 << ((u32)(slot + 3) & 0xFF));
    *(u32 *)(*sQueueBase2A + offset) = 1;
}

// FUN_02038d80 @ 0x02038d80 (64 bytes) — Queue callback with index (variant B)
static vu32 *sQueueBase0B;
static vu32 *sQueueBase1B;
static vu32 *sQueueBase2B;
void queue_callback_b(int slot, u32 func, u32 param)
{
    u32 offset = (u32)slot * 0xC;
    *(u32 *)(*sQueueBase0B + offset) = func;
    *(u32 *)(*sQueueBase1B + offset) = param;
    u32 prev = os_set_bits(1 << ((u32)(slot + 8) & 0xFF));
    *(u32 *)(*sQueueBase2B + offset) = prev & (1 << ((u32)(slot + 8) & 0xFF));
}

// ============================================================
// Global flag management
// ============================================================

// FUN_02038ed4 @ 0x02038ed4 (44 bytes) — Set bit in global flag register
static vu32 sGlobalFlags[2];  // DAT_02038f00
void set_global_flag(int param_1)
{
    u32 shift;
    vu32 *flag_ptr;

    if (param_1 - 0x60 < 0) {
        shift = (u32)(param_1 - 0x40);
        flag_ptr = &sGlobalFlags[0];
    } else {
        shift = (u32)(param_1 - 0x60);
        flag_ptr = &sGlobalFlags[1];
    }
    *flag_ptr |= 0x80000000U >> (shift & 0xFF);
}

// ============================================================
// Thread-safe setter
// ============================================================

// FUN_020392a4 @ 0x020392a4 (48 bytes) — Locked setter with lock/unlock
extern void lock_acquire(void);
extern void lock_release(void);
static vu32 *sLockedDataPtr;  // DAT_020392d4
u32 locked_set(u32 new_val)
{
    lock_acquire();
    u32 old_val = *(u32 *)((u32)sLockedDataPtr + 0x10);
    *(u32 *)((u32)sLockedDataPtr + 0x10) = new_val;
    lock_release();
    return old_val;
}

// ============================================================
// Array / list search
// ============================================================

// FUN_02039474 @ 0x02039474 (40 bytes) — Search linked list for active entry
static vu32 *sListPtr;  // DAT_0203949c
void search_list_active(void)
{
    u32 current = *(u32 *)(*sListPtr + 0xC);

    while (current != 0) {
        if (*(int *)(current + 100) == 1) {
            break;
        }
        current = *(u32 *)(current + 0x68);
    }
}

// FUN_02039944 @ 0x02039944 (44 bytes) — Find first free slot in 16-entry array
static vu32 *sSlotArrayBase;  // DAT_02039970
int find_free_slot(void)
{
    int index = 0;
    do {
        if (*(int *)(*sSlotArrayBase + (u32)index * 4 + 0x14) == 0) {
            return index;
        }
        index++;
    } while (index < 0x10);
    return -1;
}

// ============================================================
// Doubly-linked list operations
// ============================================================

// FUN_02039d64 @ 0x02039d64 (40 bytes) — Pop head from doubly-linked list
u32 list_pop_head(u32 param_1)
{
    u32 head = *(u32 *)(param_1 + 0x7C);
    u32 next = *(u32 *)(head + 0x10);

    if (next == 0) {
        *(u32 *)(param_1 + 0x80) = 0;
    } else {
        *(u32 *)(next + 0xC) = 0;
    }
    *(u32 *)(param_1 + 0x7C) = next;
    return head;
}

// FUN_02039d8c @ 0x02039d8c (36 bytes) — Remove node from doubly-linked list
void list_remove_node(u32 param_1, u32 param_2)
{
    u32 next = *(u32 *)(param_2 + 0x10);
    u32 prev = *(u32 *)(param_2 + 0xC);

    if (next == 0) {
        *(u32 *)(param_1 + 0x80) = prev;
    } else {
        *(u32 *)(next + 0xC) = prev;
    }

    if (prev == 0) {
        *(u32 *)(param_1 + 0x7C) = next;
    } else {
        *(u32 *)(prev + 0x10) = next;
    }
}

// FUN_02039db0 @ 0x02039db0 (36 bytes) — Insert node at tail of doubly-linked list
void list_insert_tail(u32 param_1, u32 param_2)
{
    u32 tail = *(u32 *)(param_1 + 0x80);

    if (tail == 0) {
        *(u32 *)(param_1 + 0x7C) = param_2;
    } else {
        *(u32 *)(tail + 0x10) = param_2;
    }

    *(u32 *)(param_2 + 0xC) = tail;
    *(u32 *)(param_2 + 0x10) = 0;
    *(u32 *)(param_1 + 0x80) = param_2;
}
