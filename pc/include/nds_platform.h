/*
 * nds_platform.h - Platform abstraction interface
 *
 * Declares the functions that the PC port implements to provide an NDS-like
 * environment for the decompiled ARM9 code.
 *
 * The decompiled code expects to read/write hardware registers at fixed
 * addresses (0x04000000+ for I/O, 0x06000000+ for VRAM). On PC, these are
 * routed through these helper functions or through emulated memory regions
 * allocated at startup.
 */
#ifndef NDS_PLATFORM_H
#define NDS_PLATFORM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* === NDS Display === */
#define NDS_SCREEN_WIDTH   256
#define NDS_SCREEN_HEIGHT  192

/* === Lifecycle === */
bool platform_init(int argc, char** argv);
void platform_shutdown(void);
void platform_present(void);  /* Called once per frame to flip buffers */
bool platform_poll_events(void);  /* Returns false on quit */

/* === Framebuffers (top + bottom screen, BGR555 like NDS) === */
uint16_t* platform_top_framebuffer(void);
uint16_t* platform_bottom_framebuffer(void);

/* === Time === */
uint64_t platform_ticks_us(void);  /* Microseconds since startup */
void platform_sleep_us(uint64_t us);

/* === Input === */
typedef struct {
    bool a, b, x, y;
    bool l, r;
    bool start, select;
    bool up, down, left, right;
    bool touch_active;
    int  touch_x, touch_y;  /* In bottom-screen coords */
} NdsInput;

const NdsInput* platform_input(void);

/* === Audio (16-bit signed stereo, 32768 Hz to match NDS) === */
#define NDS_AUDIO_SAMPLE_RATE 32768
void platform_audio_push(const int16_t* samples, int frame_count);

/* === VRAM banks (NDS has 9 banks: A-I) === */
void* nds_vram_bank(char bank);  /* 'A'..'I' */
uint32_t nds_vram_bank_size(char bank);

/* === Hardware register access (emulated) === */
uint32_t nds_reg_read32(uint32_t addr);
uint16_t nds_reg_read16(uint32_t addr);
uint8_t  nds_reg_read8(uint32_t addr);
void nds_reg_write32(uint32_t addr, uint32_t val);
void nds_reg_write16(uint32_t addr, uint16_t val);
void nds_reg_write8(uint32_t addr, uint8_t val);
/* Init the IO shadow (called lazily on first access; explicit call is optional) */
void nds_hw_io_init(void);

/*
 * nds_dma_immediate — perform a DMA transfer right now (no timing emulation).
 *
 * Called by func_0x01ff85cc stubs and any other code path that bypasses the
 * DMA register hook.  Translates NDS addresses to host pointers and calls
 * memcpy.  Safe to call from any thread.
 *
 *   dst_nds   — NDS physical destination address
 *   src_nds   — NDS physical source address (0 = fill with zeros)
 *   ctrl      — NDS DMA control word (bit 31 = enable, bit 26 = 32-bit,
 *               bits 20:0 = transfer count in words/halfwords)
 */
void nds_dma_immediate(uint32_t dst_nds, uint32_t src_nds, uint32_t ctrl);

/* === Game thread (run decompiled game_start in background) === */
void platform_signal_vblank(void);
void platform_wait_vblank(void);
bool platform_game_started(void);
int  platform_game_should_exit(void);
void platform_start_game_thread(int (*entry)(void*), const char* name);
void platform_stop_game_thread(void);

/* === Logging === */
void nds_log(const char* fmt, ...);

/* === BG Rendering === */
void bg_render_sync_vram(void);
void bg_render_top(uint16_t* fb);
void bg_render_bottom(uint16_t* fb);

/* === VRAM-write watcher (Task 4) ===
 * Throttled tracker — see arm9/src/link_stubs.c for the body.  Call this
 * from any code path that translates an NDS address to a host pointer
 * (DMA, MI_*, register writes) so we can see what regions the running
 * game actually touches per frame.
 */
void nds_track_write(uint32_t addr, uint32_t len, const char *origin);

#ifdef __cplusplus
}
#endif

#endif /* NDS_PLATFORM_H */
