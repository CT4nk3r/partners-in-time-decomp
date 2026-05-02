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

#ifdef HOST_PORT
#  include "nds_platform.h"

/* NDS display registers (top engine = main, sub engine = +0x1000). */
#  define NDS_REG_DISPCNT_MAIN     0x04000000u
#  define NDS_REG_DISPCNT_SUB      0x04001000u
#  define NDS_REG_MASTER_BRIGHT_MAIN 0x0400006Cu
#  define NDS_REG_MASTER_BRIGHT_SUB  0x0400106Cu

/*
 * GX_SwapDisplay — toggle which engine drives which screen.
 *   POWCNT1 bit 15 (REG_POWCNT1 = 0x04000304) selects display swap.
 * On host we have a fixed top/bottom mapping, so just record the bit.
 */
void GX_SwapDisplay(void) {
    uint32_t pow = nds_reg_read32(0x04000304u);
    nds_reg_write32(0x04000304u, pow ^ (1u << 15));
}

/*
 * GX_SetMasterBrightness(screen, val):
 *   screen=0 → main engine (REG_MASTER_BRIGHT @ 0x0400006C)
 *   screen=1 → sub  engine (REG_MASTER_BRIGHT @ 0x0400106C)
 *   screen=0xFFFFFFFF → both
 *   val: low 5 bits = factor (0..16), bit 14..15 = mode
 *        (0=off, 1=fade-to-white, 2=fade-to-black)
 *
 * Writing here lets a future renderer apply the brightness when game code
 * fades in/out at title screens.
 */
void GX_SetMasterBrightness(u32 screen, u16 val) {
    if (screen == 0u || screen == 0xFFFFFFFFu)
        nds_reg_write16(NDS_REG_MASTER_BRIGHT_MAIN, val);
    if (screen == 1u || screen == 0xFFFFFFFFu)
        nds_reg_write16(NDS_REG_MASTER_BRIGHT_SUB,  val);
}

/*
 * GX_SetVisiblePlane(mask, bits):
 *   `mask` selects which engines (bit 0 = main, bit 1 = sub; 0xFFFFFFFF = both).
 *   `bits` is the visible-plane bitmap: bits 8..15 of DISPCNT
 *          (BG0..BG3, OBJ, win0, win1, objwin).
 *
 * Apply by RMW-ing DISPCNT bits 8..15.
 */
void GX_SetVisiblePlane(u32 mask, u16 bits) {
    uint32_t plane = ((uint32_t)bits & 0xFFu) << 8;
    if (mask & 1u) {
        uint32_t v = nds_reg_read32(NDS_REG_DISPCNT_MAIN);
        v = (v & ~(0xFFu << 8)) | plane;
        nds_reg_write32(NDS_REG_DISPCNT_MAIN, v);
    }
    if (mask & 2u) {
        uint32_t v = nds_reg_read32(NDS_REG_DISPCNT_SUB);
        v = (v & ~(0xFFu << 8)) | plane;
        nds_reg_write32(NDS_REG_DISPCNT_SUB, v);
    }
}

/* GX_ResetVisiblePlane — clear visible-plane bits on both engines. */
void GX_ResetVisiblePlane(void) {
    uint32_t v;
    v = nds_reg_read32(NDS_REG_DISPCNT_MAIN);
    nds_reg_write32(NDS_REG_DISPCNT_MAIN, v & ~(0xFFu << 8));
    v = nds_reg_read32(NDS_REG_DISPCNT_SUB);
    nds_reg_write32(NDS_REG_DISPCNT_SUB,  v & ~(0xFFu << 8));
}

/*
 * GX_UpdateDisplay / GX_FlushDisplay — on hardware these would post a
 * GX command list to the geometry engine and wait for completion.  On
 * host we drive the rasterizer from the SDL thread, so these are no-ops.
 */
void GX_UpdateDisplay(void) {}
void GX_FlushDisplay(void)  {}
void GX_DisableInterrupts(void) {}
void GX_SetDispSelect(void) {}
u32  GX_GetCurrentMode(void) { return 0; }

#else /* !HOST_PORT */

void GX_SwapDisplay(void) {}
void GX_SetMasterBrightness(u32 screen, u16 val) { (void)screen; (void)val; }
void GX_SetVisiblePlane(u32 mask, u16 bits) { (void)mask; (void)bits; }
void GX_UpdateDisplay(void) {}
void GX_DisableInterrupts(void) {}
void GX_FlushDisplay(void) {}
void GX_ResetVisiblePlane(void) {}
void GX_SetDispSelect(void) {}
u32  GX_GetCurrentMode(void) { return 0; }

#endif /* HOST_PORT */

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

/* ======== DMA SDK wrappers ======================================================
 *
 * The NDS SDK provides high-level DMA helpers (MI_DmaCopy32, DC_FlushRange, etc.)
 * that ultimately write to the DMA hardware registers or call the ARM7 via IPC.
 *
 * On the host we don't emulate DMA timing: we perform the equivalent memcpy/memset
 * immediately.  For addresses that resolve to known NDS regions (VRAM, IO shadow,
 * Palette RAM) we use nds_dma_immediate(); for host-pointer operations we use
 * memcpy/memset directly.
 *
 * func_0x01ff85cc(ch, dst, src, ctrl):
 *   ARM7 IPC DMA trigger at BIOS address 0x01FF85CC.
 *   ctrl = (word_count) | 0x84000000  (enable | 32-bit | normal)
 *   -> dst/src are NDS addresses -> translate and memcpy.
 *
 * func_0x01ff8520(ch, dst, src, ctrl):  similar, VRAM DMA variant
 * func_0x01ff8558(ch, dst, src, ctrl):  similar, 16-bit variant
 * func_0x01ff84c0(ch, dst, src, ctrl):  similar, fill variant
 *
 * ============================================================================= */

#ifdef HOST_PORT
#  include "nds_platform.h"

/* Translate (dst, src, ctrl) ARM7 IPC DMA call into an immediate host copy. */
static void do_dma_ipc(u32 ch, u32 dst, u32 src, u32 ctrl)
{
    (void)ch;
    nds_dma_immediate(dst, src, ctrl);
}

void func_0x01ff85cc(u32 ch, u32 dst, u32 src, u32 ctrl) {
    do_dma_ipc(ch, dst, src, ctrl);
}
void func_0x01ff8520(u32 ch, u32 dst, u32 src, u32 ctrl) {
    do_dma_ipc(ch, dst, src, ctrl);
}
void func_0x01ff8558(u32 ch, u32 dst, u32 src, u32 ctrl) {
    /* 16-bit variant: clear bit 26 to indicate 16-bit transfers */
    do_dma_ipc(ch, dst, src, ctrl & ~(1u << 26));
}
void func_0x01ff84c0(u32 ch, u32 dst, u32 src, u32 ctrl) {
    do_dma_ipc(ch, dst, src, ctrl);
}

/* DC_FlushRange / DC_InvalidateRange — cache operations, no-op on host */
void DC_FlushRange(const void *addr, u32 size)      { (void)addr; (void)size; }
void DC_InvalidateRange(const void *addr, u32 size)  { (void)addr; (void)size; }
void DC_StoreRange(const void *addr, u32 size)        { (void)addr; (void)size; }
void IC_InvalidateRange(const void *addr, u32 size)  { (void)addr; (void)size; }

/* MI_DmaCopy32 / MI_DmaFill32 — synchronous 32-bit DMA copy/fill */
void MI_DmaCopy32(u32 ch, const void *src, void *dst, u32 size) {
    (void)ch;
    if (src && dst && size) memcpy(dst, src, size);
}
void MI_DmaFill32(u32 ch, void *dst, u32 val, u32 size) {
    (void)ch;
    if (dst && size) {
        u32 words = size / 4;
        for (u32 i = 0; i < words; i++) ((u32 *)dst)[i] = val;
    }
}
void MI_DmaCopy16(u32 ch, const void *src, void *dst, u32 size) {
    (void)ch;
    if (src && dst && size) memcpy(dst, src, size);
}

/* ======== Game inner-loop named stubs (top callers of host_undefined_stubs) ====
 *
 * trace_stubs.py reported zero FUN_/func_ stub calls during the inner loop
 * because game_start()'s do-while body invokes named SDK symbols directly:
 *   GX_VBlankWait, PAD_Read, FS_Update, game_update_display
 * (and game_do_transition between frame transitions).  GX_VBlankWait,
 * PAD_Read and FS_Update are already implemented above; the two below were
 * still returning 0 from auto-generated host_undefined_stubs.c.  Pulling
 * them into link_stubs.c lets us bump real frame counters and keep the
 * top-engine DISPCNT bit 15 set (the loop's exit condition) without relying
 * on the SDL keep_boot_screen_visible() side-channel.
 *
 * ============================================================================ */

extern u32 g_game_frame_counter;
u32 g_game_frame_counter = 0;
extern u32 g_game_transition_counter;
u32 g_game_transition_counter = 0;

void game_update_display(void) {
    g_game_frame_counter++;
    /* Keep the inner-loop exit condition satisfied: bit 15 of DISPCNT must be
     * set for `(*sDispCnt & 0x8000) >> 15 != 1` to be false.  The IO shadow's
     * MMIO at 0x04000000 already mirrors hardware, so RMW it here per frame —
     * this is the closest "real" effect to what the SDK's display flush would
     * do (it always re-arms the visible-plane bit). */
    uint32_t dispcnt = nds_reg_read32(0x04000000u);
    nds_reg_write32(0x04000000u, dispcnt | 0x8000u);
    /* And ensure sub engine shows something too. */
    uint32_t dispcnt_sub = nds_reg_read32(0x04001000u);
    nds_reg_write32(0x04001000u, dispcnt_sub | 0x8000u);
}

void game_do_transition(u32 duration, u32 target_state, u32 flags) {
    (void)target_state; (void)flags;
    g_game_transition_counter++;
    /* On hardware this would interpolate master brightness over `duration`
     * frames.  We just snap to fully-bright so the screen is visible. */
    if (duration > 0)
        GX_SetMasterBrightness(0xFFFFFFFFu, 0);
}

/* ======== OS/RTC/FS timer + filesystem stubs (Task 3 / Task 2) =================
 *
 * As of HEAD, none of these symbols are referenced by either the decompiled
 * arm9/src code or the auto-generated host_undefined_stubs.c (the FS module
 * in arm9/src/sdk_fs.c uses only anonymous FUN_0203* names that map to the
 * underlying NitroSDK file APIs internally).  We provide functional impls
 * anyway so that the moment a future symbolication pass renames a FUN_*
 * caller into one of these names, it links and behaves reasonably.
 *
 * OS_GetTick      — 49.152 MHz tick from boot.  We emulate from frame counter
 *                   (60 Hz nominal) so values are monotonic and deterministic.
 * OS_GetVBlankCount — straight frame counter.
 * RTC_GetDate / RTC_GetTime — fixed plausible values.
 * FS_OpenFile / FS_ReadFile / FS_CloseFile / FS_GetLength —
 *                   read against the asset pack via pack_get_file().  Path
 *                   "data/XXXX.bin" (hex) maps directly to FAT id 0xXXXX.
 *                   The first u32 of each FSFile holds our internal handle.
 * ============================================================================ */

#include <stdio.h>
#include <ctype.h>

extern u64 OS_GetTick(void);
extern u32 OS_GetVBlankCount(void);

u64 OS_GetTick(void) {
    /* 49152000 Hz / 60 Hz = 819200 ticks per frame. */
    return (u64)g_game_frame_counter * 819200ULL;
}
u32 OS_GetVBlankCount(void) {
    return g_game_frame_counter;
}
u32 OS_GetSystemTick(void) { return (u32)OS_GetTick(); }
void OS_WaitVBlank(void)   { /* loop is driven from outside */ }
void OS_SpinWait(u32 cycles) { (void)cycles; }
void OS_Sleep(u32 ms)        { (void)ms; }

u32 RTC_GetDate(void *out)  { (void)out; return 0; }
u32 RTC_GetTime(void *out)  { (void)out; return 0; }
u32 RTC_GetDateTime(void *out) { (void)out; return 0; }
u32 RTC_Init(void)          { return 0; }

/* ── Minimal FS_* against asset pack.  See header comment above. ── */

typedef struct {
    /* Mirror enough of NitroSDK FSFile that game code reading offset 0..15
     * sees plausible values.  Real layout is larger; that's fine — the game
     * allocates the full struct itself. */
    u32 magic;       /* 'MLPF' = our marker */
    u32 fat_id;      /* resolved FAT id      */
    u32 size;        /* file size in bytes   */
    u32 pos;         /* current read offset  */
    const u8 *data;  /* zero-copy pointer into pack mmap */
} mlpit_fs_handle;

static int parse_fat_path(const char *path, u32 *out_id) {
    /* Accept "data/0045.bin", "0x0045", "0045", or bare hex. */
    if (!path) return 0;
    const char *p = path;
    const char *slash = path;
    for (const char *q = path; *q; ++q) if (*q == '/' || *q == '\\') slash = q + 1;
    p = slash;
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;
    char *end;
    unsigned long v = strtoul(p, &end, 16);
    if (end == p) return 0;
    *out_id = (u32)v & 0xFFF;
    return 1;
}

u32 FS_OpenFile(void *file, const char *path) {
    if (!file) return 0;
    mlpit_fs_handle *h = (mlpit_fs_handle *)file;
    h->magic  = 0x4D4C5046u;
    h->fat_id = 0;
    h->size   = 0;
    h->pos    = 0;
    h->data   = NULL;
    if (!pack_is_loaded() || !parse_fat_path(path, &h->fat_id)) return 0;
    void *data = NULL; size_t sz = 0;
    if (!pack_get_file(h->fat_id, &data, &sz) || !data) return 0;
    h->data = (const u8 *)data;
    h->size = (u32)sz;
    return 1;
}

s32 FS_ReadFile(void *file, void *buf, s32 len) {
    if (!file || !buf || len <= 0) return -1;
    mlpit_fs_handle *h = (mlpit_fs_handle *)file;
    if (h->magic != 0x4D4C5046u || !h->data) return -1;
    u32 avail = h->size > h->pos ? h->size - h->pos : 0;
    u32 take  = (u32)len < avail ? (u32)len : avail;
    if (take) memcpy(buf, h->data + h->pos, take);
    h->pos += take;
    return (s32)take;
}

u32 FS_CloseFile(void *file) {
    if (!file) return 0;
    mlpit_fs_handle *h = (mlpit_fs_handle *)file;
    h->magic = 0; h->data = NULL; h->size = h->pos = 0;
    return 1;
}

u32 FS_GetLength(void *file) {
    if (!file) return 0;
    mlpit_fs_handle *h = (mlpit_fs_handle *)file;
    return (h->magic == 0x4D4C5046u) ? h->size : 0;
}

u32 FS_SeekFile(void *file, s32 offset, u32 origin) {
    if (!file) return 0;
    mlpit_fs_handle *h = (mlpit_fs_handle *)file;
    if (h->magic != 0x4D4C5046u) return 0;
    s64 base;
    switch (origin) {
        case 0: base = 0;             break;  /* SEEK_SET */
        case 1: base = (s64)h->pos;   break;  /* SEEK_CUR */
        case 2: base = (s64)h->size;  break;  /* SEEK_END */
        default: return 0;
    }
    s64 np = base + offset;
    if (np < 0) np = 0;
    if (np > (s64)h->size) np = (s64)h->size;
    h->pos = (u32)np;
    return 1;
}

#else /* not HOST_PORT — plain stubs for native build */

void game_update_display(void) {}
void game_do_transition(u32 d, u32 t, u32 f) { (void)d; (void)t; (void)f; }
u64 OS_GetTick(void) { return 0; }
u32 OS_GetVBlankCount(void) { return 0; }
u32 OS_GetSystemTick(void) { return 0; }
void OS_WaitVBlank(void) {}
void OS_SpinWait(u32 c) { (void)c; }
void OS_Sleep(u32 m)    { (void)m; }
u32 RTC_GetDate(void *o) { (void)o; return 0; }
u32 RTC_GetTime(void *o) { (void)o; return 0; }
u32 RTC_GetDateTime(void *o) { (void)o; return 0; }
u32 RTC_Init(void) { return 0; }
u32 FS_OpenFile(void *f, const char *p) { (void)f; (void)p; return 0; }
s32 FS_ReadFile(void *f, void *b, s32 l) { (void)f; (void)b; (void)l; return -1; }
u32 FS_CloseFile(void *f) { (void)f; return 0; }
u32 FS_GetLength(void *f) { (void)f; return 0; }
u32 FS_SeekFile(void *f, s32 o, u32 r) { (void)f; (void)o; (void)r; return 0; }
void func_0x01ff85cc(u32 ch, u32 dst, u32 src, u32 ctrl) {
    (void)ch; (void)dst; (void)src; (void)ctrl;
}
void func_0x01ff8520(u32 ch, u32 dst, u32 src, u32 ctrl) {
    (void)ch; (void)dst; (void)src; (void)ctrl;
}
void func_0x01ff8558(u32 ch, u32 dst, u32 src, u32 ctrl) {
    (void)ch; (void)dst; (void)src; (void)ctrl;
}
void func_0x01ff84c0(u32 ch, u32 dst, u32 src, u32 ctrl) {
    (void)ch; (void)dst; (void)src; (void)ctrl;
}
void DC_FlushRange(const void *a, u32 s)   { (void)a; (void)s; }
void DC_InvalidateRange(const void *a, u32 s){ (void)a; (void)s; }
void MI_DmaCopy32(u32 c, const void *s2, void *d, u32 sz) {
    (void)c; (void)s2; (void)d; (void)sz;
}
void MI_DmaFill32(u32 c, void *d, u32 v, u32 sz) { (void)c; (void)d; (void)v; (void)sz; }
void MI_DmaCopy16(u32 c, const void *s2, void *d, u32 sz) {
    (void)c; (void)s2; (void)d; (void)sz;
}
#endif /* HOST_PORT */
