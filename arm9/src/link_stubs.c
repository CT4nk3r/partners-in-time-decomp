/**
 * Link stubs — placeholder definitions for symbols not yet decompiled.
 *
 * These allow the build to link. Each stub should be replaced with
 * a real implementation as the corresponding code is decompiled.
 */

#include <string.h>
#include "types.h"
#include "arm_compat.h"

#ifdef HOST_PORT
#  include "asset_pack.h"
#  include "nds_rom.h"
#endif

/* ======== CRT0 helper functions (called from crt0.s) ======== */

void crt0_init_cp15(void) {}
void crt0_init_hardware(void) {}
void crt0_copy_sections(void *table) { (void)table; }

/* ======== SDK init functions ======== */

void OS_Init(void) {}
void GX_Init(void) {}
void FS_Init(void) {}

/* ======== OS / heap functions ======== */

void OS_MemClear(void *ptr, u32 offset, u32 size) {
    if (ptr && size > 0) memset((u8*)ptr + offset, 0, size);
}
void OS_LockMutex(void *mutex) { (void)mutex; }
void OS_UnlockMutex(void *mutex) { (void)mutex; }
u32  OS_CreateHeap(void *arena, u32 start, u32 end) {
    (void)arena; (void)start; (void)end; return 0;
}
void OS_ActivateHeap(u32 id) { (void)id; }
void OS_SetHeapConfig(void *cfg) { (void)cfg; }
void OS_SetHeapMode(u32 mode) { (void)mode; }

/* ======== IRQ / system functions ======== */

void irq_enable(void) {}
void irq_setup(void) {}
void lock_acquire(void *lock) { (void)lock; }
void lock_release(void *lock) { (void)lock; }
void panic_check1(void) {}
void panic_check2(void) {}

/* ======== DMA functions ======== */

void dma_command(u32 ch, u32 src, u32 dst, u32 ctrl) {
    (void)ch; (void)src; (void)dst; (void)ctrl;
}
void dma_channel_enable(u32 ch) { (void)ch; }
void dma_channel_reset(u32 ch) { (void)ch; }
u32  dma_aux_param = 0;

/* ======== GX / display functions ======== */

void gx_display_reset(void) {}
void gx_clear_flag(void) {}
u32  gx_check_render_available(void) { return 1; }
u32  gx_get_current_mode(void) { return 0; }
void *gx_get_display_buffer(void) { return (void*)0; }
u32  gx_get_display_status(void) { return 0; }
void gx_refresh_display(void) {}
void gx_update_base_display(void) {}
void gx_update_display_entry(void) {}

/* ======== GameProp external access ======== */

u32 GameProp_ExternalGet3000(void) { return 0; }
u32 GameProp_ExternalGet4000(void) { return 0; }
u32 GameProp_ExternalGet7000(void) { return 0; }
void GameProp_ExternalSet3000(u32 val) { (void)val; }
void GameProp_ExternalSet7000(u32 val) { (void)val; }

/* ======== Misc functions ======== */

void power_request_fifo(u32 cmd) { (void)cmd; }
u32  get_priority_used1(void) { return 0; }
u32  get_priority_used2(void) { return 0; }
void struct_setup_advanced(void *p) { (void)p; }
void struct_setup_state(void *p) { (void)p; }

/* ======== NitroMain — game entry point (calls into game_main.c) ======== */

void NitroMain(void) {}

/* ======== Global variables ======== */

u32 gGameStateBase = 0;
u32 sHeapBuffer = 0;
u32 sHeapConfig = 0;
u32 sHeapReady = 0;
u32 sHeapTable = 0;
u32 sHeapTableBack = 0;

/* ======== SDK name wrappers (Task 1) ======== */

void GX_VBlankWait(void) { arm_swi_05_vblank_intr_wait(); }
void GX_SwapDisplay(void) {}
void GX_SetMasterBrightness(u32 screen, u16 val) { (void)screen; (void)val; }
void GX_SetVisiblePlane(u32 mask, u16 bits) { (void)mask; (void)bits; }
void GX_UpdateDisplay(void) {}
void GX_DisableInterrupts(void) {}
void GX_FlushDisplay(void) {}
void GX_ResetVisiblePlane(void) {}
void GX_SetDispSelect(void) {}
u32  GX_GetCurrentMode(void) { return 0; }

void FS_InitOverlay(void) {}
u32  FS_LoadOverlay(u32 id, u32 flags, void *callback, u32 param) {
    (void)flags; (void)callback; (void)param;
#ifdef HOST_PORT
    if (pack_is_loaded())
        pack_get_overlay(id, NULL, NULL);   /* prefetch from pack */
    else
        rom_read_overlay((int)id);          /* ROM fallback       */
#endif
    return id + 1; /* non-zero handle */
}
void FS_AttachOverlay(u32 handle, u32 id) { (void)handle; (void)id; }
void FS_Update(void) {}

void SND_Init(void) {}
void SND_Update(void) {}

void PAD_Read(void) {}

void OBJ_Init(void) {}
u32  OBJ_Create(void *param) { (void)param; return 1; }
void OBJ_Update(u32 handle) { (void)handle; }
