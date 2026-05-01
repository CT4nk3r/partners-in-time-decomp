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

/* === Game thread (run decompiled game_start in background) === */
void platform_signal_vblank(void);
void platform_wait_vblank(void);
bool platform_game_started(void);
int  platform_game_should_exit(void);
void platform_start_game_thread(int (*entry)(void*), const char* name);
void platform_stop_game_thread(void);

/* === Logging === */
void nds_log(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* NDS_PLATFORM_H */
