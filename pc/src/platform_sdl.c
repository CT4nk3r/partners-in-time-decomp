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

void platform_present(void) {
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

void nds_log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fflush(stdout);
}
