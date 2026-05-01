/**
 * GX Display Functions — Graphics Register Wrappers
 *
 * Display layer management, VRAM address calculation,
 * display mode switching, and render queue operations.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// Static data pointers
// ============================================================

static vu32 *sDisplayCtrl;     // display control state
static vu32 *sDisplayState;    // display state/mode
static vu32 *sDisplayFlags;    // display flag register
static vu32 *sCurrentFrame;    // current frame counter
static vu32 *sVRAMControl;     // VRAM mapping control
static vu32 *sAnimState;       // layer animation state
static vu32 *sOAMBuffer;       // OAM render callback
static vu32 *sCurrentDisplayPtr; // current display pointer

// ============================================================
// Forward declarations (external functions)
// ============================================================

extern void gx_display_indirect_call(u32 param_1);
extern void gx_clear_flag(vu32 *ptr, u32 val);
extern u32  gx_get_tile_state(u32 val);
extern void gx_set_tile_state(u32 val1, u32 val2);
extern void gx_update_display_entry(u32 layer, s16 val, u32 z, u32 p);
extern void gx_refresh_display(void);
extern void gx_process_display_frame(void);
extern void gx_update_layer_animation(void);
extern void gx_update_display_sprites(void);
extern int  gx_get_display_status(void);
extern int  gx_get_current_mode(void);
extern void gx_update_display_state(void);
extern int  gx_check_render_available(void);
extern void gx_disable_vram_mapping(u16 val);
extern void gx_disable_oam_mapping(u16 val);
extern u32  gx_alloc_render_memory(void);
extern void gx_enable_vram_mapping(u16 val);
extern u32  gx_get_display_buffer(void);
extern int  gx_verify_region_crc(u32 a, u32 b, u32 c, u32 d);
extern void gx_display_reset(void);
extern void gx_update_base_display(u32 p1, u32 p3);
extern void gx_set_display_mode(int param_1);

// ============================================================
// Display layer initialization
// ============================================================

// FUN_02027710 @ 0x02027710 (64 bytes) — Initialize 4 display layer entries
void gx_init_display_layers(void)
{
    int base_addr;
    int entry_idx;
    int entry_addr;

    base_addr = (int)*sCurrentDisplayPtr;
    entry_idx = 3;
    do {
        entry_addr = base_addr + entry_idx * 8;
        *(u16 *)(entry_addr) = 0;
        *(u16 *)(entry_addr + 2) = 0xFFFF;
        *(u32 *)(entry_addr + 4) = 0;
        entry_idx = entry_idx - 1;
    } while (entry_idx >= 0);
}

// FUN_02027798 @ 0x02027798 (20 bytes) — Indirect display function call
void gx_indirect_call(u32 param_1)
{
    void (*func_ptr)(u32, u32, u32);
    func_ptr = (void (*)(u32, u32, u32))(u32)*sDisplayState;
    func_ptr(*sDisplayCtrl, param_1, 0);
}

// ============================================================
// Layer query
// ============================================================

// FUN_02027b38 @ 0x02027b38 (80 bytes) — Get active display layers bitmask
u32 gx_get_active_layers(void)
{
    u32 layer_mask;
    int layer_idx;

    layer_mask = 0;
    layer_idx = 3;
    do {
        if (*(int *)((u32)sAnimState + layer_idx * 4) != 0) {
            layer_mask = layer_mask | (1 << (layer_idx & 0xFF));
        }
        layer_idx = (int)((layer_idx - 1) * 0x10000) >> 16;
    } while (layer_idx >= 0);
    return layer_mask;
}

// FUN_02027b8c @ 0x02027b8c (40 bytes) — Reset display configuration
void gx_reset_display_config(void)
{
    gx_indirect_call(*sCurrentFrame);
    gx_init_display_layers();
}

// ============================================================
// Display update & mode switching
// ============================================================

// FUN_02027cf4 @ 0x02027cf4 (52 bytes) — Conditional display update
void gx_update_display_conditional(u32 param_1, int param_2, u32 param_3)
{
    if (param_2 != 0) {
        gx_display_reset();
        return;
    }
    gx_update_base_display(param_1, param_3);
}

// FUN_02027d28 @ 0x02027d28 (108 bytes) — Set display flag/state
void gx_set_display_flag(u32 param_1)
{
    u32 state_val;

    if (*(u8 *)((u32)sDisplayFlags + 4) == (u8)param_1) {
        gx_clear_flag(sVRAMControl, 0);
    }
    if (param_1 == 0) {
        state_val = *(u32 *)sCurrentFrame;
    } else {
        state_val = *(u32 *)sCurrentFrame;
    }
    state_val = gx_get_tile_state(state_val);
    gx_set_tile_state(state_val, state_val);
    *(u16 *)((u32)sDisplayFlags + param_1 * 2) = 0xFFFF;
}

// FUN_02027e5c @ 0x02027e5c (20 bytes) — Set display mode with default
void gx_set_mode(int param_1)
{
    if (param_1 == 0) {
        param_1 = 8;
    }
    void (*func)(u32, int);
    func = (void (*)(u32, int))(u32)*sDisplayState;
    func(*sDisplayCtrl, param_1);
}

// FUN_02027ee0 @ 0x02027ee0 (120 bytes) — Update display with state check
void gx_update_display_checked(int param_1, u32 param_2, u32 param_3)
{
    if (param_1 == 0) {
        return;
    }
    if (*(int *)((u32)sDisplayCtrl + 0x20) == 0) {
        if ((param_2 == *(u8 *)((u32)sDisplayState + 4)) &&
            (param_1 != *(s16 *)((u32)sDisplayState + param_2 * 2))) {
            gx_set_mode(0);
        }
        gx_update_display_entry(param_2, (s16)param_1, 0, param_3);
    }
}

// ============================================================
// Display refresh & frame update
// ============================================================

// FUN_02027ff4 @ 0x02027ff4 (48 bytes) — Handle display refresh
void gx_handle_refresh(void)
{
    if (*(int *)((u32)sDisplayCtrl + 0x20) != 0) {
        gx_refresh_display();
    }
}

// FUN_02028028 @ 0x02028028 (76 bytes) — Main display update loop
void gx_main_update(void)
{
    u32 delta_frames;

    delta_frames = *sCurrentFrame - *sVRAMControl;
    if (delta_frames > 5) {
        delta_frames = 5;
    }
    *sVRAMControl = *sCurrentFrame;
    for (; delta_frames != 0; delta_frames--) {
        gx_process_display_frame();
    }
    gx_handle_refresh();
    gx_update_layer_animation();
    gx_update_display_sprites();
}

// ============================================================
// CRC / integrity verification
// ============================================================

// FUN_020283c8 @ 0x020283c8 (64 bytes) — Verify display CRC
BOOL gx_verify_crc(int param_1, int param_2)
{
    u32 calc_crc;
    u32 (*crc_func)(int, int) = (u32 (*)(int, int))(u32)*sDisplayState;

    calc_crc = crc_func(param_1, param_2 - 2);
    return calc_crc == *(u16 *)(param_1 + param_2 - 2);
}

// FUN_02028440 @ 0x02028440 (68 bytes) — Check if display is ready
BOOL gx_check_ready(void)
{
    int is_ready;

    is_ready = gx_get_display_status();
    if (is_ready == 0) {
        return FALSE;
    }
    return *sDisplayState != 2;
}

// ============================================================
// Display mode initialization
// ============================================================

// FUN_02028488 @ 0x02028488 (72 bytes) — Initialize display mode A
void gx_init_mode_a(void)
{
    int mode_val;

    mode_val = gx_get_current_mode();
    *sDisplayState = (u32)mode_val;
    gx_update_display_state();
    if (*sDisplayState == 0) {
        *sDisplayFlags = 0;
    } else {
        *sDisplayFlags = 2;
    }
}

// FUN_020284d8 @ 0x020284d8 (72 bytes) — Initialize display mode B
void gx_init_mode_b(void)
{
    int mode_val;

    mode_val = gx_get_current_mode();
    *sVRAMControl = (u32)mode_val;
    gx_update_display_state();
    if (*sVRAMControl == 0) {
        *sCurrentFrame = 0;
    } else {
        *sCurrentFrame = 2;
    }
}

// ============================================================
// Render queue operations
// ============================================================

// FUN_02028528 @ 0x02028528 (136 bytes) — Queue display render operation
void gx_queue_render(u32 param_1, u32 param_2, u32 param_3)
{
    int is_available;

    is_available = gx_check_render_available();
    if (is_available != 0) {
        *sCurrentDisplayPtr = param_1;
        *sDisplayState = param_2;
        *sDisplayFlags = param_3;
        void (*render_func)(u32, u32, u32, u32);
        render_func = (void (*)(u32, u32, u32, u32))(u32)*sOAMBuffer;
        render_func(param_1, *sDisplayState, param_3, *sVRAMControl);
        *sDisplayCtrl = 1;
    } else {
        *sDisplayCtrl = 0;
    }
}

// FUN_020286b0 @ 0x020286b0 (92 bytes) — Cleanup display rendering state
void gx_cleanup_render(void)
{
    if (*sVRAMControl != 0xFFFFFFFD) {
        gx_disable_vram_mapping((u16)(*sVRAMControl & 0xFFFF));
        gx_disable_oam_mapping((u16)(*sVRAMControl & 0xFFFF));
        *sVRAMControl = 0xFFFFFFFD;
    }
}

// FUN_02028710 @ 0x02028710 (96 bytes) — Allocate and setup display resources
u32 gx_alloc_display_resources(void)
{
    u32 alloc_result;

    if (*sCurrentDisplayPtr != 0xFFFFFFFD) {
        gx_cleanup_render();
    }
    alloc_result = gx_alloc_render_memory();
    *sCurrentDisplayPtr = alloc_result;
    if (alloc_result != 0xFFFFFFFD) {
        gx_enable_vram_mapping((u16)(alloc_result & 0xFFFF));
        return 1;
    }
    return 0;
}

// ============================================================
// Validation & signature functions
// ============================================================

// FUN_020289f4 @ 0x020289f4 (44 bytes) — Validate display buffer state
BOOL gx_validate_buffer(void)
{
    u32 buffer_ptr;
    int crc_valid;

    buffer_ptr = gx_get_display_buffer();
    crc_valid = gx_verify_crc(buffer_ptr, *sDisplayState);
    return crc_valid != 0;
}

// FUN_02028a30 @ 0x02028a30 (68 bytes) — Validate display region
BOOL gx_validate_region(u32 param_1, int param_2, u32 param_3)
{
    u32 buffer_ptr;
    int crc_valid;

    buffer_ptr = gx_get_display_buffer();
    crc_valid = gx_verify_region_crc(param_2 * *sCurrentFrame + 0x20,
                                      buffer_ptr, *sCurrentFrame, param_3);
    return crc_valid != 0;
}

// FUN_02028b04 @ 0x02028b04 (44 bytes) — Check header integrity
BOOL gx_check_header(void)
{
    int crc_valid;
    crc_valid = gx_verify_crc(*sDisplayState, 0x14);
    return crc_valid != 0;
}

// FUN_02028b50 @ 0x02028b50 (44 bytes) — Check footer integrity
BOOL gx_check_footer(void)
{
    int crc_valid;
    crc_valid = gx_verify_crc(*sCurrentFrame, 4);
    return crc_valid != 0;
}

// FUN_02028c20 @ 0x02028c20 (76 bytes) — Compare 6-byte display signature
u32 gx_compare_signature(void)
{
    char *sig_ptr1;
    char *sig_ptr2;
    int byte_idx;

    byte_idx = 0;
    sig_ptr1 = (char *)*sCurrentDisplayPtr;
    sig_ptr2 = (char *)*sDisplayState;
    do {
        if (*sig_ptr1 != *sig_ptr2) {
            return 0xFFFFFFFF;
        }
        byte_idx++;
        sig_ptr2++;
        sig_ptr1++;
    } while (byte_idx < 6);
    return (u32)(((char *)*sCurrentDisplayPtr)[6] == '\x07');
}

// FUN_02028cdc @ 0x02028cdc (72 bytes) — Execute display command
BOOL gx_execute_command(void)
{
    u8 cmd_buffer[16];

    gx_queue_render(0, (u32)cmd_buffer, 8);
    if (*sDisplayCtrl != 0) {
        int result = gx_check_ready();
        return result != 0;
    }
    return FALSE;
}
