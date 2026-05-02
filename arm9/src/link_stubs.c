/**
 * Link stubs — placeholder definitions for symbols not yet decompiled.
 *
 * These allow the build to link. Each stub should be replaced with
 * a real implementation as the corresponding code is decompiled.
 */

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "arm_compat.h"

#ifdef HOST_PORT
#  include "asset_pack.h"
#  include "nds_rom.h"
#endif

/* ======== CRT0 helper functions (called from crt0.s) ======== */

void crt0_init_cp15(void) {}
void crt0_init_hardware(void) {}
/* crt0_copy_sections is now provided by FUN_02004914 in game_init.c via the
 * sdk_symbol_aliases.h #define mapping (real LZ-decompress + cache flush).
 * Keeping a no-op here would multiply-define the symbol on HOST_PORT. */
#ifndef HOST_PORT
void crt0_copy_sections(void *table) { (void)table; }
#endif

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

#ifdef HOST_PORT
/* ── HOST_PORT backing for map_control.c FUN_02018ed0 statics ──
 * On real NDS DAT_02018f08 / DAT_02018f0c are .data words pointing
 * at a 14-entry channel-id array and the clGameMain slot ptr. The
 * auto-generated host_undefined_stubs.c declares them as 4-byte
 * uint32_t which truncates host pointers; map_control.c is rewired
 * via #define DAT_02018f08 → g_mc_DAT_02018f08 to use the 64-bit-
 * safe definitions here. They are seeded from
 * host_link_stubs_init_map_control_data() called at boot. */
intptr_t g_mc_DAT_02018f08 = 0;
int    *g_mc_DAT_02018f0c  = 0;
static u32 s_mc_channel_ids[14];
extern u32 *game_state_host_get_current_slot(void);
void host_link_stubs_init_map_control_data(void)
{
    /* All-zero channel IDs are safe — FUN_02016920(0) is a no-op
     * lookup that returns 0 in the stub path. */
    memset(s_mc_channel_ids, 0, sizeof(s_mc_channel_ids));
    g_mc_DAT_02018f08 = (intptr_t)(uintptr_t)s_mc_channel_ids;
    g_mc_DAT_02018f0c = (int *)game_state_host_get_current_slot();
}
#endif

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

/* PAD_Read — read NDS buttons from REG_KEYINPUT + ext keys,
 * then update the pad state struct via FUN_02029664.
 *
 * On real hardware:
 *   main keys at 0x04000130 (10 bits, active-low)
 *   ext  keys at 0x04000136 (X/Y from ARM7 IPC, active-low)
 *   pad state struct at 0x0205FFAC
 *   FUN_02029664(pad_state, pressed_bits) stores cont/trigger/release/repeat
 *
 * On HOST_PORT, SDL writes both registers each frame via pump_input_to_io(). */
extern void FUN_02029664(u16 *param_1, u32 param_2);

void PAD_Read(void)
{
    u16 main_keys = nds_reg_read16(0x04000130u);   /* active-low, bits 0-9 */
    u16 ext_keys  = nds_reg_read16(0x04000136u);   /* active-low, bits 0-1 = X/Y */

    /* Combine: map ext bit 0 (X) → bit 10, ext bit 1 (Y) → bit 11 */
    u16 combined = main_keys | 0xFC00u;  /* bits 10-15 default to released */
    if (!(ext_keys & (1u << 0))) combined &= ~(1u << 10);  /* X pressed */
    if (!(ext_keys & (1u << 1))) combined &= ~(1u << 11);  /* Y pressed */

    u16 mask = 0x0FFFu;  /* 12 button bits */
    u16 pressed = (~combined) & mask;  /* active-high */

    /* Pad state struct at 0x0205FFAC in NDS RAM */
    volatile u16 *pad = (volatile u16 *)(uintptr_t)0x0205FFACu;
    FUN_02029664((u16 *)pad, (u32)pressed);
}

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
#ifdef HOST_PORT
extern void host_log_vram_dma(const char *kind, const void *src, void *dst, u32 size);
#endif
void MI_DmaCopy32(u32 ch, const void *src, void *dst, u32 size) {
    (void)ch;
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaCopy32", src, dst, size);
#endif
    if (src && dst && size) memcpy(dst, src, size);
}
void MI_DmaFill32(u32 ch, void *dst, u32 val, u32 size) {
    (void)ch;
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaFill32", NULL, dst, size);
#endif
    if (dst && size) {
        u32 words = size / 4;
        for (u32 i = 0; i < words; i++) ((u32 *)dst)[i] = val;
    }
}
void MI_DmaCopy16(u32 ch, const void *src, void *dst, u32 size) {
    (void)ch;
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaCopy16", src, dst, size);
#endif
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

/* Task 3: per-frame snapshot of the post-loop u16 staging arrays at
 * DAT_0201977c..02019790.  Logged the first time a write is observed
 * relative to the post-init baseline, then again every 240 frames. */
static u32 g_shadow_baseline_hash = 0;
static int g_shadow_baseline_taken = 0;
static int g_shadow_first_diff_logged = 0;

static u32 shadow_probe_hash(const volatile u8 *p, u32 len) {
    u32 h = 0;
    for (u32 i = 0; i < len; i++) h = h * 131u + p[i];
    return h;
}

static void shadow_probe_tick(u32 frame) {
    /* Range 0x02019700..0x02019820 inside our VirtualAlloc'd ARM9 RAM. */
    const volatile u8 *p = (const volatile u8 *)(uintptr_t)0x02019700u;
    const u32 len = 0x120u;
    u32 h = shadow_probe_hash(p, len);
    if (!g_shadow_baseline_taken) {
        g_shadow_baseline_hash = h;
        g_shadow_baseline_taken = 1;
        return;
    }
    int changed = (h != g_shadow_baseline_hash);
    if (changed && !g_shadow_first_diff_logged) {
        g_shadow_first_diff_logged = 1;
        u32 w0 = *(volatile u32 *)(uintptr_t)0x0201977cu;
        u32 w1 = *(volatile u32 *)(uintptr_t)0x02019780u;
        u32 w2 = *(volatile u32 *)(uintptr_t)0x02019784u;
        u32 w3 = *(volatile u32 *)(uintptr_t)0x0201978cu;
        fprintf(stderr,
                "[shadow] FIRST DIFF at frame=%u: hash %08X -> %08X "
                "w[77c]=%08X w[780]=%08X w[784]=%08X w[78c]=%08X\n",
                (unsigned)frame, (unsigned)g_shadow_baseline_hash,
                (unsigned)h, (unsigned)w0, (unsigned)w1,
                (unsigned)w2, (unsigned)w3);
        fflush(stderr);
    }
    if ((frame % 240u) == 0u) {
        fprintf(stderr,
                "[shadow] frame=%u hash=%08X (%s baseline %08X)\n",
                (unsigned)frame, (unsigned)h,
                changed ? "DIFFERS_FROM" : "matches",
                (unsigned)g_shadow_baseline_hash);
        fflush(stderr);
    }
}

/* External: 64-bit-safe wrapper around FUN_0202a33c (the linked-list
 * scene-queue processor) lives in pc/src/host_scene_queue.c.  We DO NOT
 * call FUN_0202a33c directly because the decompiled gx_util.c declares
 * DAT_0202a510 as `int *` (8 bytes on x86_64) while
 * pc/src/host_undefined_stubs.c defines it as a 4-byte uint32_t — the
 * mismatched dereference faults. */
extern void FUN_0202a33c_safe(void);
extern void host_scene_queue_log_state(const char *tag);
extern void host_scene_queue_rearm_fake(void);
extern void host_factory_rearm(void);
extern void host_gxfifo_observer_tick(void);
static int  g_queue_processor_enabled = -1;  /* tri-state: 0/1, -1=unread */

void game_update_display(void) {
    g_game_frame_counter++;
    shadow_probe_tick(g_game_frame_counter);
    /* Keep the inner-loop exit condition satisfied: bit 15 of DISPCNT must be
     * set for `(*sDispCnt & 0x8000) >> 15 != 1` to be false.  The IO shadow's
     * MMIO at 0x04000000 already mirrors hardware, so RMW it here per frame —
     * this is the closest "real" effect to what the SDL display flush would
     * do (it always re-arms the visible-plane bit). */
    uint32_t dispcnt = nds_reg_read32(0x04000000u);
    /* Task 6: force OBJ engine enable (DISPCNT bit 12) so obj_render() will
     * walk OAM and paint placeholders.  Toggle off via MLPIT_NO_OBJ_ENABLE=1
     * to compare baseline behaviour. */
    uint32_t obj_force = (getenv("MLPIT_NO_OBJ_ENABLE") == 0) ? 0x1000u : 0u;
    nds_reg_write32(0x04000000u, dispcnt | 0x8000u | obj_force);
    /* And ensure sub engine shows something too. */
    uint32_t dispcnt_sub = nds_reg_read32(0x04001000u);
    nds_reg_write32(0x04001000u, dispcnt_sub | 0x8000u | obj_force);

    /* Per-session-task: drive the linked-list scene-queue processor each
     * frame.  On real hardware FUN_02005444 (game_start) BLs FUN_0202a33c
     * once per inner-loop iteration; that call is missing from the
     * existing decomp of game_start in arm9/src/game_main.c, so wire it
     * in here.  Default ON; toggle off via MLPIT_NO_QUEUE_TICK=1.  When
     * the anchor head is NULL (current state) FUN_0202a33c just returns
     * immediately, so this is safe. */
    if (g_queue_processor_enabled < 0) {
        g_queue_processor_enabled = (getenv("MLPIT_NO_QUEUE_TICK") == 0);
        fprintf(stderr,
                "[queue-tick] FUN_0202a33c per-frame call %s\n",
                g_queue_processor_enabled ? "ENABLED" : "DISABLED");
        fflush(stderr);
    }
    if (g_queue_processor_enabled) {
        /* Re-arm the synthetic test node's "needs update" bit so its
         * vtable[2] (host_draw_test_node) fires every frame.  No-op if
         * MLPIT_FAKE_QUEUE_NODE wasn't set. */
        host_scene_queue_rearm_fake();
        host_factory_rearm();
        /* Re-log anchor every 240 frames so we can SEE if anything changed. */
        if (g_game_frame_counter == 1u ||
            (g_game_frame_counter % 240u) == 0u) {
            char tag[32];
            snprintf(tag, sizeof(tag), "frame_%u", (unsigned)g_game_frame_counter);
            host_scene_queue_log_state(tag);
        }
        FUN_0202a33c_safe();
        host_gxfifo_observer_tick();
    }
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

/* === BIOS LZ77 wrappers (HOST_PORT) =================================
 * Three FUN_<addr> entries (0x02046ffc, 0x02047010, 0x02047024) all
 * forward to BIOS SWI 0x11 (LZ77UnCompVram) on hardware.
 * NOTE: Real bodies live in arm9/src/sdk_init_d.c on host build (the
 * decompiled module already has them).  This block is intentionally
 * empty so we don't double-define.
 */

/* === Top-20 hot anonymous helpers (Task 2: hotstubs-and-display) =====
 *
 * These are aliased from FUN_<addr> in pc/include/sdk_symbol_aliases.h.
 * Identification rationale is recorded next to each entry; signatures
 * use variadic ()/(...) so the underlying ARM AAPCS arg passing (r0..r3)
 * stays compatible with every call site.
 *
 * For now only sdk_mi_cpu_copy16 / sdk_mi_cpu_fill16 / sdk_arm_udiv have
 * functionally important real bodies — the OS_* family is a no-op set
 * because the host has no preemption and a single game thread.
 *
 * ================================================================ */

/* MI_CpuCopy16(src, dst, len) — 16-bit halfword memcpy.  Signed-len
 * variants exist; we treat len as bytes (decompiled call sites pass byte
 * counts already). */
void sdk_mi_cpu_copy16(u32 src_addr, u32 dst_addr, u32 len)
{
    if (!src_addr || !dst_addr || !len) return;
    /* If addresses look like NDS physical addrs that we have shadows for,
     * route through nds_dma_immediate; otherwise treat as raw host pointers
     * (decompiled code stores host malloc results in those args via our
     * OS_AllocFromHeap impl). */
    if (src_addr < 0x10000000u && dst_addr < 0x10000000u) {
        /* enable | 16-bit | word_count = len/2 */
        nds_dma_immediate(dst_addr, src_addr, (1u << 31) | ((len / 2u) & 0x1FFFFFu));
        return;
    }
    memcpy((void *)(uintptr_t)dst_addr, (const void *)(uintptr_t)src_addr, len);
}

/* MI_CpuFill16(dst, val, len) — 16-bit halfword memset (val = pattern). */
void sdk_mi_cpu_fill16(u32 dst_addr, u32 val, u32 len)
{
    if (!dst_addr || !len) return;
    u16 pat = (u16)(val & 0xFFFFu);
    if (dst_addr < 0x10000000u) {
        /* No NDS DMA fill primitive in our shadow — write directly via
         * resolver by routing through DMA helper with src=0 and then
         * patching, but simpler to just do per-halfword writes via the
         * IO/VRAM resolvers when applicable.  Fallback: assume IO shadow
         * mapped at NDS_IO_BASE catches IO-region writes; for VRAM we
         * cannot resolve from this TU without including nds_platform —
         * but link_stubs.c already does.  Use direct host-pointer write
         * because nds_hw_stubs.c maps 0x04000000 directly via VirtualAlloc
         * and VRAM banks are calloc'd (not at NDS addrs).  Punt to memset
         * if dst is high (host pointer); for low NDS addresses this is a
         * no-op fill since we cannot resolve safely here. */
        u16 *p = NULL;
        /* IO range — 0x04000000+0x10000 is mapped via VirtualAlloc on Win64 */
        if (dst_addr >= 0x04000000u && dst_addr + len <= 0x04010000u) {
            p = (u16 *)(uintptr_t)dst_addr;
        }
        if (!p) return;
        u32 halfwords = len / 2u;
        for (u32 i = 0; i < halfwords; i++) p[i] = pat;
        return;
    }
    u16 *p = (u16 *)(uintptr_t)dst_addr;
    u32 halfwords = len / 2u;
    for (u32 i = 0; i < halfwords; i++) p[i] = pat;
}

/* OS_Free / OS_FreeToHeap — on host every alloc came from our pool, the
 * pool is rewound on shutdown; a free is a no-op for our purposes. */
void sdk_os_free(u32 ptr) { (void)ptr; }

/* OS_DestroyHeap — no-op (heap lives till exit). */
void sdk_os_destroy_heap(void) {}

/* OS_DisableInterrupts → returns saved CPSR I-bit state.
 * On host we don't have IRQs; return 0 so OS_RestoreInterrupts(0) is a
 * harmless no-op. */
u32 sdk_os_disable_irq(void) { return 0; }
void sdk_os_restore_irq(u32 saved) { (void)saved; }

/* OS_RescheduleThread / OS yield — single-threaded host, no-op. */
void sdk_os_reschedule(void) {}

/* arm_udiv — libgcc-style 32-bit unsigned divide.  Hardware ARM9 doesn't
 * have a divide instruction; FUN_020466bc is the SDK helper.  Used by
 * snd_fs_misc.c / sdk_math.c with two register args. */
u32 sdk_arm_udiv(u32 dividend, u32 divisor)
{
    return divisor ? (dividend / divisor) : 0u;
}

/* OS_SendMessage-shaped (FUN_0203a04c, takes 2 args).  Without IPC
 * threads on the host this is a no-op; returning 1 = "queued ok" for
 * any caller that checks. */
int sdk_os_send_message(u32 queue, u32 msg) { (void)queue; (void)msg; return 1; }

/* === VRAM-write watcher (Task 4) ==================================
 *
 * Throttled logger that records writes to interesting NDS regions.
 * Since VRAM banks are calloc'd at host addresses (not at NDS addrs)
 * we cannot intercept random pointer writes.  Instead, every code path
 * that DOES translate (DMA, MI_CpuCopy16/Fill16, nds_dma_immediate, the
 * register write helpers) calls nds_track_write() so we can see whether
 * game code actually reaches VRAM during a normal frame.
 *
 * Throttle: max 1 log per region per 1000 calls.  Region keys are 0..3
 * for IO / Palette / VRAM / OAM.
 * ================================================================== */

#include <stdarg.h>

void nds_track_write(u32 addr, u32 len, const char *origin)
{
    static const struct { u32 lo; u32 hi; const char *name; } regions[] = {
        { 0x04000000u, 0x04010000u, "IO"      },
        { 0x05000000u, 0x05000800u, "PAL"     },
        { 0x06000000u, 0x06180000u, "VRAM"    },
        { 0x07000000u, 0x07000400u, "OAM"     },
    };
    static u32 counts[4] = { 0, 0, 0, 0 };

    for (int i = 0; i < 4; i++) {
        if (addr >= regions[i].lo && addr < regions[i].hi) {
            counts[i]++;
            if ((counts[i] % 1000u) == 1u) {
                fprintf(stderr,
                        "[vram-watch] %-4s addr=%08X len=%u count=%u origin=%s\n",
                        regions[i].name, addr, len, counts[i],
                        origin ? origin : "?");
                fflush(stderr);
            }
            return;
        }
    }
}

/* Logger callable from arm9/src/game_main.c (which has no <stdio.h>). */
void mlpit_log_missing_calls_wired(void) {
    fprintf(stderr,
            "[game_start] inner-loop wired: FUN_020072d4 + FUN_0202a33c_safe\n");
    fflush(stderr);
}

#endif /* HOST_PORT */

#ifndef HOST_PORT
/* ====== Plain stubs for native ARM build ====== */

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
#ifdef HOST_PORT
extern void host_log_vram_dma(const char *kind, const void *src, void *dst, u32 size);
#endif
void MI_DmaCopy32(u32 c, const void *s2, void *d, u32 sz) {
    (void)c;
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaCopy32(stub)", s2, d, sz);
#endif
    (void)s2; (void)d; (void)sz;
}
void MI_DmaFill32(u32 c, void *d, u32 v, u32 sz) {
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaFill32(stub)", NULL, d, sz);
#endif
    (void)c; (void)d; (void)v; (void)sz;
}
void MI_DmaCopy16(u32 c, const void *s2, void *d, u32 sz) {
    (void)c;
#ifdef HOST_PORT
    host_log_vram_dma("MI_DmaCopy16(stub)", s2, d, sz);
#endif
    (void)s2; (void)d; (void)sz;
}

/* === BIOS LZ77 wrappers ============================================
 * Three FUN_<addr> entries (0x02046ffc, 0x02047010, 0x02047024) all
 * forward to BIOS SWI 0x11 (LZ77UnCompVram).  They differ only in the
 * exact callee tail (Vram/Wram/aligned variants); on the host all of
 * them route through arm_swi_11_lz77_decomp().
 */
void LZ77_UncompVram_1(const void *src, void *dst) { arm_swi_11_lz77_decomp(src, dst); }
void LZ77_UncompVram_2(const void *src, void *dst) { arm_swi_11_lz77_decomp(src, dst); }
void LZ77_UncompVram_3(const void *src, void *dst) { arm_swi_11_lz77_decomp(src, dst); }

#endif /* HOST_PORT */