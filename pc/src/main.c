/*
 * main.c - PC port entry point
 *
 * Initializes SDL2, opens a window, runs the main loop, and (eventually)
 * dispatches to the decompiled game's main loop.
 *
 * For now this is just an SDL2 scaffold that displays placeholder framebuffers
 * for the top and bottom screens.
 */
#include "nds_platform.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void render_placeholder(void) {
    uint16_t* top = platform_top_framebuffer();
    uint16_t* bot = platform_bottom_framebuffer();

    static uint32_t frame = 0;
    frame++;

    /* Top screen: animated diagonal stripes */
    for (int y = 0; y < NDS_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < NDS_SCREEN_WIDTH; x++) {
            uint16_t r = ((x + frame) & 0x1F);
            uint16_t g = ((y + frame / 2) & 0x1F);
            uint16_t b = ((x ^ y) & 0x1F);
            top[y * NDS_SCREEN_WIDTH + x] = (b << 10) | (g << 5) | r;
        }
    }

    /* Bottom screen: solid dark blue with center cross */
    for (int y = 0; y < NDS_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < NDS_SCREEN_WIDTH; x++) {
            uint16_t v = 0x1080; /* dark blue-ish */
            if (x == NDS_SCREEN_WIDTH / 2 || y == NDS_SCREEN_HEIGHT / 2) {
                v = 0x7FFF; /* white */
            }
            bot[y * NDS_SCREEN_WIDTH + x] = v;
        }
    }
}

int main(int argc, char** argv) {
    SDL_SetMainReady();
    if (!platform_init(argc, argv)) {
        fprintf(stderr, "platform_init failed\n");
        return 1;
    }

    nds_log("Mario & Luigi: Partners in Time - PC port (scaffold)\n");
    nds_log("Decompilation: 1275/1275 ARM9 functions\n");
    nds_log("Press ESC or close window to quit.\n");

    while (platform_poll_events()) {
        const NdsInput* input = platform_input();

        /* TODO: Call into decompiled game main loop here.
         * Currently the game code in arm9/src/ targets armv5te and uses
         * NDS hardware registers directly, so we cannot link it yet.
         * See pc/README.md for the porting roadmap. */

        render_placeholder();
        platform_present();

        /* ~60 fps */
        platform_sleep_us(16666);
    }

    platform_shutdown();
    return 0;
}
