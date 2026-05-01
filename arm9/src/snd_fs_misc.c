/**
 * Sound, Filesystem, and Miscellaneous Functions
 *
 * Sound system wrappers, filesystem indirect callbacks,
 * queue/dequeue operations, structured data writes,
 * and numeric-to-string conversion.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// External declarations
// ============================================================

extern void FUN_02038f4c(void);
extern void FUN_0204280c(u32 param, int flag);
extern void FUN_020428b0(u32 param, int flag);
extern void FUN_02038f68(u32 param);
extern void FUN_02048ce0(void);
extern void FUN_02043bf4(int val);
extern void FUN_0203aeb4(void);
extern void FUN_0203b808(u32 a, u32 b, u32 c);
extern void FUN_0200c334(void);
extern void FUN_0202a20c(void);
extern void FUN_02027bb8(u16 val);
extern void FUN_02027cf4(int a, int b, u32 c);
extern u8   FUN_020466bc(u32 dividend, int divisor);

// ============================================================
// Sound system
// ============================================================

// FUN_020425e0 @ 0x020425e0 (32 bytes) — Sound system start wrapper
void snd_start(u32 param_1)
{
    FUN_02038f4c();
    FUN_0204280c(param_1, 1);
}

// FUN_02042600 @ 0x02042600 (32 bytes) — Sound system stop wrapper
void snd_stop(u32 param_1)
{
    FUN_020428b0(param_1, 1);
    FUN_02038f68(param_1);
}

// ============================================================
// Filesystem indirect callbacks
// ============================================================

// FUN_020412ac @ 0x020412ac (16 bytes) — FS indirect call 1
static u32 sFS_Callback1;   // DAT at func+0x10
static u32 sFS_Context1;    // DAT at func+0x14
void fs_callback_1(u32 param_1)
{
    void (*cb)(u32, u32) = (void (*)(u32, u32))sFS_Callback1;
    cb(sFS_Context1, param_1);
}

// FUN_020412c4 @ 0x020412c4 (16 bytes) — FS indirect call 2
static u32 sFS_Callback2;
static u32 sFS_Context2;
void fs_callback_2(u32 param_1)
{
    void (*cb)(u32, u32) = (void (*)(u32, u32))sFS_Callback2;
    cb(sFS_Context2, param_1);
}

// FUN_020412dc @ 0x020412dc (16 bytes) — FS indirect call 3
static u32 sFS_Callback3;
static u32 sFS_Context3;
void fs_callback_3(u32 param_1)
{
    void (*cb)(u32, u32) = (void (*)(u32, u32))sFS_Callback3;
    cb(sFS_Context3, param_1);
}

// FUN_020412f4 @ 0x020412f4 (16 bytes) — FS indirect call 4
static u32 sFS_Callback4;
static u32 sFS_Context4;
void fs_callback_4(u32 param_1)
{
    void (*cb)(u32, u32) = (void (*)(u32, u32))sFS_Callback4;
    cb(sFS_Context4, param_1);
}

// FUN_0204180c @ 0x0204180c (20 bytes) — FS dual parameter call
static u32 sFS_DualCallback;
void fs_dual_call(u32 param_1, u32 param_2)
{
    void (*cb)(u32, u32, u32) = (void (*)(u32, u32, u32))sFS_DualCallback;
    cb(0xE, param_1, param_2);
}

// ============================================================
// System callbacks & initialization
// ============================================================

// FUN_02048880 @ 0x02048880 (32 bytes) — System callback invocation
static void (**sSysCallbackPtr)(void);
void sys_invoke_callback(void)
{
    (*sSysCallbackPtr)();
}

// FUN_02048c6c @ 0x02048c6c (32 bytes) — Pointer initialization with callback
static u32 sInitValue;
void sys_ptr_init(u32 *param_1)
{
    *param_1 = sInitValue;
    FUN_02048ce0();
}

// FUN_02043f50 @ 0x02043f50 (32 bytes) — System initialization
void sys_init(void)
{
    FUN_02043bf4(2);
    FUN_0203aeb4();
}

// FUN_0202af14 @ 0x0202af14 (44 bytes) — Memory block configuration
void mem_block_config(u32 param_1)
{
    FUN_0203b808(0, param_1, 0x600);
}

// FUN_02046ed0 @ 0x02046ed0 (24 bytes) — Structure field initialization
static u32 sStructInitVal1;  // DAT at offset
static u32 sStructInitVal2;  // DAT at offset
u32 struct_field_init(u32 param_1)
{
    *(u32 *)(param_1 + 0xC) = sStructInitVal2;
    *(u32 *)(param_1 + 0x10) = sStructInitVal1;
    return 1;
}

// ============================================================
// Queue / dequeue operations (linked list)
// ============================================================

// FUN_02010bfc @ 0x02010bfc (32 bytes) — Queue dequeue at offset +8/+C
u32 *queue_dequeue_8(u32 param_1)
{
    u32 *node = *(u32 **)(param_1 + 8);
    if (node == *(u32 **)(param_1 + 0xC)) {
        return NULL;
    }
    *(u32 *)(param_1 + 8) = *node;
    return node;
}

// FUN_02017dd0 @ 0x02017dd0 (32 bytes) — Queue dequeue at offset +18/+1C
u32 *queue_dequeue_18(u32 param_1)
{
    u32 *node = *(u32 **)(param_1 + 0x18);
    if (node == *(u32 **)(param_1 + 0x1C)) {
        return NULL;
    }
    *(u32 *)(param_1 + 0x18) = *node;
    return node;
}

// FUN_02018bbc @ 0x02018bbc (32 bytes) — Queue dequeue at offset +4/+8
u32 *queue_dequeue_4(u32 param_1)
{
    u32 *node = *(u32 **)(param_1 + 4);
    if (node == *(u32 **)(param_1 + 8)) {
        return NULL;
    }
    *(u32 *)(param_1 + 4) = *node;
    return node;
}

// ============================================================
// Indirect method call
// ============================================================

// FUN_02012510 @ 0x02012510 (32 bytes) — Virtual method call at vtable+0x88
void obj_call_method_88(u32 *param_1)
{
    void (*method)(void) = *(void (**)(void))(*param_1 + 0x88);
    method();
}

// FUN_020197e4 @ 0x020197e4 (32 bytes) — Init with callback and state assign
static u32 sObjInitState;
u32 *obj_init_with_callback(u32 *param_1)
{
    FUN_0202a20c();
    *param_1 = sObjInitState;
    return param_1;
}

// ============================================================
// State management
// ============================================================

// FUN_02010518 @ 0x02010518 (116 bytes) — Reset entity state based on parameter
void entity_reset_state(u32 param_1, int param_2)
{
    FUN_0200c334();
    *(u32 *)(param_1 + 0x54) = *(u32 *)(param_1 + 0x54) & 0xFFC7FFFF;
    *(u8 *)(param_1 + 0x58) = 0;
    *(u8 *)(param_1 + 0x59) = 0xF;
    *(u32 *)(param_1 + 0x5C) = 0;
}

// FUN_020120e4 @ 0x020120e4 (144 bytes) — Write structured animation data
void anim_data_write(u32 param_1, int param_2, u16 param_3, u16 param_4,
                     u16 param_5, u16 param_6, u16 param_7, u16 param_8,
                     u32 param_9, u8 param_10, u16 param_11, u32 param_12,
                     u8 param_13)
{
    u32 entry = *(u32 *)(param_1 + 0xA8) + (u32)param_2 * 0x18;

    *(u16 *)(entry + 0)  = param_7;
    *(u16 *)(entry + 2)  = param_8;
    *(u16 *)(entry + 4)  = param_3;
    *(u16 *)(entry + 6)  = param_4;
    *(u16 *)(entry + 8)  = param_5;
    *(u16 *)(entry + 10) = param_6;
    *(u32 *)(entry + 0xC)  = param_9;
    *(u32 *)(entry + 0x10) = param_12;
    *(u16 *)(entry + 0x14) = param_11;
    *(u8 *)(entry + 0x16)  = param_10;
    *(u8 *)(entry + 0x17)  = param_13;

    if ((*(u8 *)(param_1 + 0x78C) & 3) == 2) {
        *(u8 *)(param_1 + 0x78C) = (*(u8 *)(param_1 + 0x78C) & 0xFC) | 3;
    }
}

// FUN_02012bec @ 0x02012bec (68 bytes) — State reset with flag clearing
void state_reset_flags(u32 *param_1, int param_2)
{
    *(u8 *)(param_1[0x1D7] + (u32)param_2 + 1) = 0;
    *(u32 *)(param_1[0x1D6] + ((u32)param_2 + 1) * 4) = 0;

    int result = ((int (*)(void))(*(u32 *)(*param_1 + 0x5C)))();
    if (result == 0) {
        *(u8 *)((u32)param_1 + 0x78C) = *(u8 *)((u32)param_1 + 0x78C) & 0xFC;
    }
}

// ============================================================
// Audio/sprite configuration
// ============================================================

// FUN_0201dcd8 @ 0x0201dcd8 (92 bytes) — Audio priority/bank configuration
void audio_bank_config(u32 param_1, u32 param_2)
{
    u32 bank_id = (*(u32 *)(param_2 + 8)) >> 18;
    if (bank_id != 0) {
        FUN_02027bb8((u16)bank_id);
        FUN_02027cf4((int)(((bank_id + 1) * 0x10000) >> 16), 0, 0xFFFFFFFF);
        *(u32 *)(param_2 + 0x6C) = (*(u32 *)(param_2 + 0x6C) & 0xFFF1FFFF) | 0x60000;
    }
}

// ============================================================
// Numeric to string conversion (BCD-like)
// ============================================================

// FUN_0201a698 @ 0x0201a698 (272 bytes) — Convert number to decimal string
static u32 *sDigitTable;    // max value per digit count
static u32 *sDivisorTable;  // power-of-10 divisors
void num_to_str(u32 param_1, u32 param_2, int param_3, int param_4)
{
    u8 digit;
    u32 value;
    int div_idx;
    int divisor;
    char *out;
    char *cur;
    int *div_ptr;
    BOOL started;
    BOOL is_negative;

    *(u32 *)(param_2 + 0x5C) = param_2 + 0x54;
    if (*(char *)(param_2 + 0x60) != '\0') {
        return;
    }
    *(u8 *)(param_2 + 0x60) = 1;

    value = *(u32 *)(param_2 + 0x14);
    out = (char *)*(u32 *)(param_2 + 0x5C);
    is_negative = (s32)value < 0;
    if (is_negative) {
        value = (u32)(-(s32)value);
    }

    // Clamp to max for digit count
    u32 max_val = sDigitTable[param_3];
    if (max_val < value) {
        value = max_val;
    }

    started = FALSE;
    div_idx = 6 - (param_3 + 1);
    div_ptr = (int *)&sDivisorTable[div_idx];
    divisor = *div_ptr;

    if (divisor != 0) {
        cur = out;
        do {
            digit = FUN_020466bc(value, divisor);
            value = value - (u32)digit * (u32)divisor;
            out = cur;

            if (divisor == 1 || digit != 0) {
                started = TRUE;
                if (is_negative) {
                    *cur = '-';
                    cur++;
                    out = cur;
                    is_negative = FALSE;
                }
                *out = (char)(digit + 0x30);
                out++;
            } else if (started) {
                *out = (char)(digit + 0x30);
                out++;
            } else if (param_4 != 0) {
                *cur = ' ';
                out = cur + 1;
            }

            div_ptr++;
            divisor = *div_ptr;
            cur = out;
        } while (divisor != 0);
    }
    *out = '\0';
}
