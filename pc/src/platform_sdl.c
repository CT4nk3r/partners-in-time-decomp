/*
 * platform_sdl.c - SDL2 implementation of the platform interface
 *
 * Manages a single SDL2 window with two stacked NDS-sized screens, handles
 * keyboard input mapped to NDS buttons, and uploads BGR555 framebuffers as
 * RGB888 textures on each frame.
 */
#include "nds_platform.h"

#include <SDL2/SDL.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SCALE 2

static SDL_Window*   g_window;
static SDL_Renderer* g_renderer;
static SDL_Texture*  g_top_tex;
static SDL_Texture*  g_bot_tex;

static uint16_t g_top_fb[NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT];
static uint16_t g_bot_fb[NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT];

static NdsInput g_input;
static uint64_t g_start_us;

static uint64_t now_us(void) {
    return (uint64_t)SDL_GetPerformanceCounter() * 1000000ULL
         / (uint64_t)SDL_GetPerformanceFrequency();
}

bool platform_init(int argc, char** argv) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    int win_w = NDS_SCREEN_WIDTH  * WINDOW_SCALE;
    int win_h = NDS_SCREEN_HEIGHT * 2 * WINDOW_SCALE;

    g_window = SDL_CreateWindow(
        "Mario & Luigi: Partners in Time (PC port)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        win_w, win_h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetLogicalSize(g_renderer, NDS_SCREEN_WIDTH, NDS_SCREEN_HEIGHT * 2);

    /* NDS framebuffer is BGR555 (16bpp). SDL has SDL_PIXELFORMAT_BGR555. */
    g_top_tex = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_BGR555,
        SDL_TEXTUREACCESS_STREAMING, NDS_SCREEN_WIDTH, NDS_SCREEN_HEIGHT);
    g_bot_tex = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_BGR555,
        SDL_TEXTUREACCESS_STREAMING, NDS_SCREEN_WIDTH, NDS_SCREEN_HEIGHT);
    if (!g_top_tex || !g_bot_tex) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    g_start_us = now_us();
    memset(&g_input, 0, sizeof(g_input));
    return true;
}

void platform_shutdown(void) {
    if (g_top_tex)  SDL_DestroyTexture(g_top_tex);
    if (g_bot_tex)  SDL_DestroyTexture(g_bot_tex);
    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window)   SDL_DestroyWindow(g_window);
    SDL_Quit();
}

uint16_t* platform_top_framebuffer(void)    { return g_top_fb; }
uint16_t* platform_bottom_framebuffer(void) { return g_bot_fb; }

static void dump_screenshot_ppm(const char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    int w = NDS_SCREEN_WIDTH;
    int h = NDS_SCREEN_HEIGHT * 2;
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int s = 0; s < 2; ++s) {
        const uint16_t* fb = (s == 0) ? g_top_fb : g_bot_fb;
        for (int i = 0; i < NDS_SCREEN_WIDTH * NDS_SCREEN_HEIGHT; ++i) {
            uint16_t p = fb[i];
            uint8_t r = (uint8_t)(((p      ) & 0x1F) * 255 / 31);
            uint8_t g = (uint8_t)(((p >>  5) & 0x1F) * 255 / 31);
            uint8_t b = (uint8_t)(((p >> 10) & 0x1F) * 255 / 31);
            uint8_t rgb[3] = { r, g, b };
            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

/*
 * Build the NDS REG_KEYINPUT bitmap (active-low: bit set = released).
 * Bits: 0=A 1=B 2=Select 3=Start 4=Right 5=Left 6=Up 7=Down 8=R 9=L.
 * Bits 10..15 are the X/Y/lid/touch flags on REG_KEYINPUT_EXT (0x04000136),
 * but the lower 10 bits live at 0x04000130 and that's what game code reads.
 */
static uint16_t build_keyinput_bits(void)
{
    uint16_t pressed = 0;
    if (g_input.a)     pressed |= (1u << 0);
    if (g_input.b)     pressed |= (1u << 1);
    if (g_input.select)pressed |= (1u << 2);
    if (g_input.start) pressed |= (1u << 3);
    if (g_input.right) pressed |= (1u << 4);
    if (g_input.left)  pressed |= (1u << 5);
    if (g_input.up)    pressed |= (1u << 6);
    if (g_input.down)  pressed |= (1u << 7);
    if (g_input.r)     pressed |= (1u << 8);
    if (g_input.l)     pressed |= (1u << 9);
    /* Active-low: invert and mask to 10 bits */
    return (uint16_t)((~pressed) & 0x03FFu);
}

/*
 * REG_KEYINPUT_EXT (0x04000136) – on real hardware reports X, Y, hinge,
 * pen down/up.  Bit assignment from GBATEK:
 *   bit 0 = X (active-low)
 *   bit 1 = Y (active-low)
 *   bit 6 = pen down  (0 = touched)
 *   bit 7 = hinge open (0 = open) — PC port: always open
 *   bits 2..5 reserved (read as 1 on hardware)
 */
static uint16_t build_keyinput_ext_bits(void)
{
    uint16_t pressed = 0;
    if (g_input.x)            pressed |= (1u << 0);
    if (g_input.y)            pressed |= (1u << 1);
    if (g_input.touch_active) pressed |= (1u << 6);
    /* hinge bit 7 stays "pressed" (=0 = open) so the game thinks the lid is open */
    pressed |= (1u << 7);
    /* Reserved bits 2..5 read as 1 on hardware */
    uint16_t reserved = 0x003Cu;
    return (uint16_t)(((~pressed) & ~reserved) | reserved);
}

/* Push input state into the NDS IO shadow.  Called once per frame from
 * platform_present() so game code reading *(vu16*)0x04000130 sees the
 * latest keyboard state on its next vblank.
 *
 * MLPIT_AUTO_START_FRAME=N : simulate Start press at frame N (for headless
 * testing so the title screen auto-advances). */
static void pump_input_to_io(void)
{
    static int s_auto_start_frame = -2;
    static int s_pump_frame = 0;
    s_pump_frame++;

    if (s_auto_start_frame == -2) {
        const char *env = getenv("MLPIT_AUTO_START_FRAME");
        s_auto_start_frame = env ? atoi(env) : -1;
    }
    /* Simulate Start press for a window of frames starting at the target.
     * Use a wider window (10 frames) to avoid race conditions between the
     * main thread writing the register and the game thread reading it. */
    if (s_auto_start_frame > 0 &&
        s_pump_frame >= s_auto_start_frame &&
        s_pump_frame < s_auto_start_frame + 10) {
        g_input.start = 1;
    }
    /* Second auto-press: file select START.
     * Use a very wide 100-frame window to guarantee the game thread sees it. */
    int second_press = s_auto_start_frame > 0 ? s_auto_start_frame + 150 : -1;
    if (second_press > 0) {
        if (s_pump_frame >= second_press && s_pump_frame < second_press + 100) {
            g_input.start = 1;
        }
    }

    nds_reg_write16(0x04000130u, build_keyinput_bits());
    nds_reg_write16(0x04000136u, build_keyinput_ext_bits());

    /* Clear auto-press after writing so it doesn't stick */
    if (s_auto_start_frame > 0 && s_pump_frame >= s_auto_start_frame + 10) {
        g_input.start = 0;
    }
    if (second_press > 0 && s_pump_frame >= second_press + 100) {
        g_input.start = 0;
    }
}

/*
 * Force BG0 visible in DISPCNT every frame so the boot-screen tiles
 * stay on after game code calls GX_SetVisiblePlane(0xFFFFFFFF, 0)
 * during init.  Disabled when MLPIT_LET_GAME_DISPLAY=1 so the user
 * can see what the game actually does once it can render on its own.
 *
 * MLPIT_DISPCNT_PULSE=1 (Task 3): every 60 frames toggle DISPCNT bit 15
 * off then back on so game_start()'s inner do/while exits and the outer
 * for(;;) gets a chance to call game_do_transition().  Logs every state
 * transition observed in g_game_frame_counter / g_game_transition_counter.
 */
extern unsigned g_game_frame_counter;
extern unsigned g_game_transition_counter;
static void keep_boot_screen_visible(void)
{
    static int s_pulse_mode = -1;
    static int s_pulse_frame = 0;
    static unsigned s_last_trans = 0;
    static unsigned s_last_inner = 0;
    if (s_pulse_mode < 0) s_pulse_mode = getenv("MLPIT_DISPCNT_PULSE") ? 1 : 0;

    if (getenv("MLPIT_LET_GAME_DISPLAY")) return;

    /* Once the gameplay scene takes over, write its DISPCNT configuration
     * instead of the boot-screen override.  We include bit 15 (display on)
     * so the game thread's do/while exit condition still works.
     * The game thread wrote DISPCNT=0x40011510 once during init, but we
     * may have overwritten it before gda became visible — restore it now. */
    extern int g_game_display_active;
    if (g_game_display_active) {
        nds_reg_write32(0x04000000u, 0x40019510u);  /* gameplay main + bit15 */
        /* Sub engine: mode 0, BG2+OBJ enabled, OBJ 1D, display on */
        nds_reg_write32(0x04001000u, 0x00009410u);
        return;
    }
    uint32_t dispcnt = nds_reg_read32(0x04000000u);
    /* Mode 0 + BG0 enable (bit 8). Also preserve BG1 (bit 9) if already set
     * (e.g. title screen uses both BG0 sky + BG1 portal). */
    dispcnt = (dispcnt & ~0x7u) | 0x100u;
    /* Ensure BG1 stays on if it was configured */
    {
        uint16_t bg1cnt = nds_reg_read16(0x0400000Au);
        if (bg1cnt != 0) dispcnt |= 0x200u;
        uint16_t bg2cnt = nds_reg_read16(0x0400000Cu);
        if (bg2cnt != 0) dispcnt |= 0x400u;
    }

    if (s_pulse_mode) {
        s_pulse_frame++;
        /* Every 60 frames clear bit 15 for one frame so the game's inner
         * do/while exits.  On the next call we restore it so subsequent
         * frames continue rendering. */
        if ((s_pulse_frame % 60) == 0) {
            dispcnt &= ~0x8000u;
            fprintf(stderr,
                    "[dispcnt-pulse] frame=%d clearing bit15 "
                    "(inner=%u trans=%u, delta_inner=%u delta_trans=%u)\n",
                    s_pulse_frame,
                    g_game_frame_counter,
                    g_game_transition_counter,
                    g_game_frame_counter - s_last_inner,
                    g_game_transition_counter - s_last_trans);
            fflush(stderr);
            s_last_inner = g_game_frame_counter;
            s_last_trans = g_game_transition_counter;
        } else {
            dispcnt |= 0x8000u;
        }
    } else {
        dispcnt |= 0x8000u;  /* legacy: keep bit 15 set so loop exits each iter */
    }
    nds_reg_write32(0x04000000u, dispcnt);
    uint32_t dispcnt_sub = nds_reg_read32(0x04001000u);
    dispcnt_sub = (dispcnt_sub & ~0x7u) | 0x100u | 0x8000u;
    nds_reg_write32(0x04001000u, dispcnt_sub);
}


void platform_present(void) {
    pump_input_to_io();
    keep_boot_screen_visible();

    /* GX rasteriser self-test (Track C): when MLPIT_GXRASTER_TEST=1 we
     * build a hand-crafted command stream once, run it through the
     * rasteriser, and composite the result over the NDS top framebuffer.
     * Lets us prove the rasteriser draws even when the natural FUN_0200FCB4
     * vertex stream is still gated. */
    {
        extern void host_gxfifo_raster_self_test(void);
        extern void host_gxfifo_raster_composite_to_top(uint16_t *top_fb);
        extern int  host_gxfifo_raster_dirty(void);
        static int s_raster_test_done = -1;
        if (s_raster_test_done < 0)
            s_raster_test_done = getenv("MLPIT_GXRASTER_TEST") ? 0 : -2;
        if (s_raster_test_done == 0) {
            host_gxfifo_raster_self_test();
            s_raster_test_done = 1;
        }
        if (s_raster_test_done >= 1 && host_gxfifo_raster_dirty()) {
            host_gxfifo_raster_composite_to_top(g_top_fb);
        }
    }

    /* Synth-sprite real-vertices pipeline (Track B+C of fcb4-real-vertices).
     * When MLPIT_SYNTH_SPRITE=1, run the host-side natural-C emitter every
     * frame, which pushes GX commands derived from a synth entity buffer
     * + a real ROM-extracted texture.  This is the equivalent of what
     * FUN_0200FCB4 would emit once its bail at L_02010028 is unblocked. */
    {
        extern void mlpit_synth_sprite_emit_frame(void);
        extern void host_gxfifo_raster_composite_to_top(uint16_t *top_fb);
        extern int  host_gxfifo_raster_dirty(void);
        static int s_synth_enabled = -1;
        if (s_synth_enabled < 0)
            s_synth_enabled = getenv("MLPIT_SYNTH_SPRITE") ? 1 : 0;
        if (s_synth_enabled) {
            mlpit_synth_sprite_emit_frame();
            if (host_gxfifo_raster_dirty())
                host_gxfifo_raster_composite_to_top(g_top_fb);
        }
    }

    static int s_frame_counter = 0;
    static int s_dump_at = -2;
    /* MLPIT_SCREENSHOT_FRAMES=60,180,300,...   per-frame snapshots.
     * MLPIT_SCREENSHOT_PATH may contain a single %d which is replaced
     * with the frame number; otherwise the same file is overwritten. */
    #define MAX_SHOT_FRAMES 32
    static int  s_shot_frames[MAX_SHOT_FRAMES];
    static int  s_shot_count = 0;
    static int  s_shot_init = 0;
    if (!s_shot_init) {
        s_shot_init = 1;
        const char* mfs = getenv("MLPIT_SCREENSHOT_FRAMES");
        if (mfs) {
            const char* p = mfs;
            while (*p && s_shot_count < MAX_SHOT_FRAMES) {
                while (*p == ' ' || *p == ',') p++;
                if (!*p) break;
                int v = atoi(p);
                if (v > 0) s_shot_frames[s_shot_count++] = v;
                while (*p && *p != ',') p++;
            }
        }
    }
    if (s_dump_at == -2) {
        const char* env = getenv("MLPIT_SCREENSHOT_FRAME");
        s_dump_at = env ? atoi(env) : -1;
    }
    s_frame_counter++;

    /* state-init watcher: log when sGameState->current changes (Task 2). */
    extern void game_state_watch_tick(int frame);
    if ((s_frame_counter & 0x3F) == 0) {
        game_state_watch_tick(s_frame_counter);
    }
    if (s_dump_at > 0 && s_frame_counter == s_dump_at) {
        const char* path = getenv("MLPIT_SCREENSHOT_PATH");
        if (!path) path = "screenshot.ppm";
        dump_screenshot_ppm(path);
        fprintf(stderr, "[screenshot] frame %d -> %s\n", s_frame_counter, path);
    }
    for (int i = 0; i < s_shot_count; i++) {
        if (s_frame_counter == s_shot_frames[i]) {
            const char* path = getenv("MLPIT_SCREENSHOT_PATH");
            char buf[512];
            if (!path) path = "screenshot_%d.ppm";
            if (strstr(path, "%d")) {
                snprintf(buf, sizeof(buf), path, s_frame_counter);
            } else {
                /* No %d → suffix _<frame> before extension */
                const char* dot = strrchr(path, '.');
                if (dot) {
                    int pre = (int)(dot - path);
                    snprintf(buf, sizeof(buf), "%.*s_%d%s",
                             pre, path, s_frame_counter, dot);
                } else {
                    snprintf(buf, sizeof(buf), "%s_%d", path, s_frame_counter);
                }
            }
            dump_screenshot_ppm(buf);
            fprintf(stderr, "[screenshot] frame %d -> %s\n", s_frame_counter, buf);
        }
    }
    {
        const char* exit_env = getenv("MLPIT_EXIT_AT_FRAME");
        int exit_at = exit_env ? atoi(exit_env) : -1;
        if (exit_at > 0 && s_frame_counter >= exit_at) {
            fprintf(stderr, "[exit] reached frame %d (MLPIT_EXIT_AT_FRAME)\n", s_frame_counter);
            fflush(stderr);
            exit(0);
        }
    }
    SDL_UpdateTexture(g_top_tex, NULL, g_top_fb, NDS_SCREEN_WIDTH * 2);
    SDL_UpdateTexture(g_bot_tex, NULL, g_bot_fb, NDS_SCREEN_WIDTH * 2);

    SDL_Rect top_dst = { 0, 0,                    NDS_SCREEN_WIDTH, NDS_SCREEN_HEIGHT };
    SDL_Rect bot_dst = { 0, NDS_SCREEN_HEIGHT,    NDS_SCREEN_WIDTH, NDS_SCREEN_HEIGHT };

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, g_top_tex, NULL, &top_dst);
    SDL_RenderCopy(g_renderer, g_bot_tex, NULL, &bot_dst);
    SDL_RenderPresent(g_renderer);
}

static void update_button(SDL_Keycode key, bool pressed) {
    switch (key) {
        case SDLK_z:        g_input.a = pressed; break;
        case SDLK_x:        g_input.b = pressed; break;
        case SDLK_a:        g_input.x = pressed; break;
        case SDLK_s:        g_input.y = pressed; break;
        case SDLK_q:        g_input.l = pressed; break;
        case SDLK_w:        g_input.r = pressed; break;
        case SDLK_RETURN:   g_input.start = pressed; break;
        case SDLK_RSHIFT:
        case SDLK_BACKSPACE: g_input.select = pressed; break;
        case SDLK_UP:       g_input.up = pressed; break;
        case SDLK_DOWN:     g_input.down = pressed; break;
        case SDLK_LEFT:     g_input.left = pressed; break;
        case SDLK_RIGHT:    g_input.right = pressed; break;
        default: break;
    }
}

bool platform_poll_events(void) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == SDLK_ESCAPE) return false;
                update_button(ev.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                update_button(ev.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    g_input.touch_active = (ev.type == SDL_MOUSEBUTTONDOWN);
                }
                /* Fallthrough to capture coords */
                /* fallthrough */
            case SDL_MOUSEMOTION: {
                int mx = ev.motion.x, my = ev.motion.y;
                int win_w, win_h;
                SDL_GetWindowSize(g_window, &win_w, &win_h);
                /* Convert window coords to bottom-screen coords */
                int sx = mx * NDS_SCREEN_WIDTH / win_w;
                int sy = (my * NDS_SCREEN_HEIGHT * 2 / win_h) - NDS_SCREEN_HEIGHT;
                if (sy >= 0 && sy < NDS_SCREEN_HEIGHT
                 && sx >= 0 && sx < NDS_SCREEN_WIDTH) {
                    g_input.touch_x = sx;
                    g_input.touch_y = sy;
                } else if (ev.type != SDL_MOUSEMOTION) {
                    g_input.touch_active = false;
                }
                break;
            }
        }
    }
    return true;
}

const NdsInput* platform_input(void) { return &g_input; }

uint64_t platform_ticks_us(void) { return now_us() - g_start_us; }

void platform_sleep_us(uint64_t us) {
    SDL_Delay((uint32_t)(us / 1000));
}

void platform_audio_push(const int16_t* samples, int frame_count) {
    /* TODO: SDL_AudioStream wiring. For now we just discard samples. */
    (void)samples; (void)frame_count;
}

/* === Game thread + VBlank sync ===
 *
 * The decompiled game_start() is an infinite loop on the NDS that calls
 * GX_VBlankWait() (-> SWI 0x05 VBlankIntrWait) for frame pacing. To run it
 * unchanged on the host we drive it in a separate thread and have
 * arm_swi_05_vblank_intr_wait() block on a semaphore that the SDL main
 * thread posts to once per rendered frame.
 */
static SDL_sem*    g_vblank_sem;
static SDL_Thread* g_game_thread;
static int         g_game_thread_should_exit;
static int         g_game_started;

void platform_signal_vblank(void) {
    if (g_vblank_sem) SDL_SemPost(g_vblank_sem);
}

void platform_wait_vblank(void) {
    if (!g_vblank_sem) return;
    /* Wait up to 1 frame; we poll the exit flag so the thread can be cleaned up. */
    SDL_SemWaitTimeout(g_vblank_sem, 50);
}

bool platform_game_started(void) { return g_game_started != 0; }

void platform_start_game_thread(int (*entry)(void*), const char* name) {
    if (g_game_thread) return;
    g_vblank_sem  = SDL_CreateSemaphore(0);
    g_game_thread = SDL_CreateThread(entry, name ? name : "mlpit_game", NULL);
    g_game_started = 1;
}

void platform_stop_game_thread(void) {
    g_game_thread_should_exit = 1;
    /* Wake the thread if it's sleeping in vblank wait */
    for (int i = 0; i < 4 && g_vblank_sem; i++) SDL_SemPost(g_vblank_sem);
    if (g_game_thread) {
        SDL_WaitThread(g_game_thread, NULL);
        g_game_thread = NULL;
    }
    if (g_vblank_sem) {
        SDL_DestroySemaphore(g_vblank_sem);
        g_vblank_sem = NULL;
    }
}

int platform_game_should_exit(void) { return g_game_thread_should_exit; }

void nds_log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fflush(stdout);
}

