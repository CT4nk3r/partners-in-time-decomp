/*
 * main.c - PC port entry point
 *
 * Initializes SDL2, opens a window, runs the main loop, and (eventually)
 * dispatches to the decompiled game's main loop.
 *
 * Boot flow (Ship-of-Harkinian style):
 *   1. Try assets/mlpit.assets  → load and run (no ROM needed)
 *   2. ROM found?  → extract to assets/mlpit.assets, then load
 *   3. Neither?    → print hint, run with stub data
 */
#include "nds_platform.h"
#include "nds_rom.h"
#include "asset_pack.h"
#include "asset_extractor.h"
#include "nds_boot_hook.h"
#include "asset_viewer.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int game_thread_main(void* user);

/* Optional: per-frame call into overlay-0's high-level scene render
 * (FUN_02065a10).  Gated by MLPIT_CALL_REAL_RENDER=1 because without
 * a faithful scene struct, dereferencing scene->sub at offset +4 etc.
 * could NPE; the C body NULL-guards this safely.  The wiring exists
 * so loading overlay 0 becomes visibly observable end-to-end. */
extern void FUN_02065a10(void *scene);

static void render_frame(void) {
    uint16_t* top = platform_top_framebuffer();
    uint16_t* bot = platform_bottom_framebuffer();

    static int s_main_frame = 0;
    static int s_real_render_logged = 0;

    if (getenv("MLPIT_CALL_REAL_RENDER")) {
        if (!s_real_render_logged) {
            s_real_render_logged = 1;
            fprintf(stderr, "[render] MLPIT_CALL_REAL_RENDER=1 — calling "
                            "FUN_02065a10(scene) per frame\n");
        }
        /* MLPIT_SYNTH_SCENE=1: hand-built minimal scene struct to
         * exercise FUN_02065a10's entity loop end-to-end.  Layout
         * mirrors the offsets the function dereferences (verified
         * against the overlay-0 disassembly):
         *   gate at NDS 0x020607AC + 0x14 bit 8 — must be set.
         *   scene[+0x04]  = sub pointer
         *   scene[+0x28]  = flags (bit 0 = repop GFX)
         *   sub[+0x10]    = kind (must be 0..2 to enter render path)
         *   sub[+0x3B0]   = entity ptr slot 0
         *   sub[+0x3B4]   = entity ptr slot 1
         *
         * The entity at slot 0 is placed at NDS 0x02300200 so it
         * survives the Win64 32-bit pointer truncation in any decomp
         * function we reach.  Under MLPIT_FAKE_NODE_FN=fcb4 the per-
         * entity dispatcher (FUN_0206E06C) forwards to FUN_0200FCB4
         * so the GXFIFO path is exercised. */
        static uint8_t s_scene[0x100];
        static uint8_t s_sub  [0x400];
        static int     s_synth_initialised = 0;
        if (getenv("MLPIT_SYNTH_SCENE") && !s_synth_initialised) {
            extern int nds_arm9_ram_is_mapped(void);
            /* Defer the one-shot init until the game thread has
             * mmapped the NDS ARM9 RAM region — otherwise the gate
             * write at 0x020607AC + 0x14 lands in unmapped memory
             * and the entity_nds placement is silently skipped. */
            if (!nds_arm9_ram_is_mapped()) {
                /* Try again next frame. */
            } else {
            s_synth_initialised = 1;
            *(void **)(s_scene + 0x04) = s_sub;
            *(uint32_t *)(s_scene + 0x28) = 0; /* don't enter GFX repop branch */
            *(uint32_t *)(s_sub  + 0x10) = 0; /* kind = 0 */

            /* Set the render-enable gate at 0x020607AC + 0x14 bit 8.
             * Without this FUN_02065a10 returns immediately. */
            uint32_t entity_nds = 0;
            volatile uint16_t *gate =
                (volatile uint16_t *)(uintptr_t)(0x020607AC + 0x14);
            *gate = (uint16_t)(*gate | 0x100);

            /* Park a 256-byte zero-initialised entity stub at
             * NDS 0x02300200 (well beyond ov0/ov6 footprint).
             * If MLPIT_INSTANTIATE_REAL=1 was set, host_factory_instantiate
             * has already populated this slab with a real vtable + aux
             * buffers — DON'T clear it, just point at it. */
            entity_nds = 0x02300200u;
            if (!getenv("MLPIT_INSTANTIATE_REAL")) {
                memset((void *)(uintptr_t)entity_nds, 0, 256);
                /* Flag bytes some FCB4 paths read at +0x00..+0x10. */
                *(uint16_t *)(uintptr_t)(entity_nds + 0x00) = 0x0001;
                *(uint16_t *)(uintptr_t)(entity_nds + 0x02) = 0x0001;
            }
            *(uint32_t *)(s_sub + 0x3B0) = entity_nds;
            *(uint32_t *)(s_sub + 0x3B4) = 0; /* leave slot 1 NULL */

            fprintf(stderr,
                    "[render-synth] scene=%p sub=%p entity_nds=0x%08X "
                    "(kind=0, flags=0, gate=set)\n",
                    (void*)s_scene, (void*)s_sub, (unsigned)entity_nds);
            }
        }
        void *scene_arg = getenv("MLPIT_SYNTH_SCENE") ? (void*)s_scene : NULL;
        FUN_02065a10(scene_arg);
    }

    /* Pull shadow OAM (NDS RAM 0x0205FFC0/0x0205FFAC) into g_oam_main /
     * g_oam_sub before obj_render() reads them.  See host_oam_upload.c. */
    extern void host_oam_upload_tick(int);
    host_oam_upload_tick(s_main_frame++);
    bg_render_sync_vram();
    bg_render_top(top);
    bg_render_bottom(bot);
    obj_render(top, 0);
    obj_render(bot, 1);
}

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
    /* Parse CLI arguments before SDL init so --show-asset can be used
     * in headless/scripted tests without a display.
     * --show-asset N : load FAT file N raw bytes as 4bpp tiles and render.
     * --list-assets  : print all asset IDs and exit (no window needed).
     */
    int show_asset_idx = -1;   /* -1 = normal boot */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--show-asset") == 0 && i + 1 < argc) {
            show_asset_idx = atoi(argv[++i]);
        }
    }

    SDL_SetMainReady();
    if (!platform_init(argc, argv)) {
        fprintf(stderr, "platform_init failed\n");
        return 1;
    }

    nds_log("Mario & Luigi: Partners in Time - PC port (scaffold)\n");
    nds_log("Decompilation: 1275/1275 ARM9 functions\n");
    nds_log("Press ESC or close window to quit.\n");

    /* ── Asset pack boot flow ───────────────────────────────────────
     * 1. Prefer the pre-extracted pack (normal every-run path).
     * 2. No pack → try to auto-extract from the ROM (first-run setup).
     * 3. No ROM either → print a helpful hint and run with stub data.
     * ──────────────────────────────────────────────────────────── */
    static const char* const k_rom_paths[] = {
        "roms/baserom.nds",
        "rom/baserom.nds",
        "rom/baserom_usa_rev1.nds",
        "rom/baserom_eur.nds",
        "../../rom/baserom.nds",
        "../../rom/baserom_usa_rev1.nds",
        "../../rom/baserom_eur.nds",
        "../../roms/baserom.nds",
        NULL
    };

    if (pack_load("assets/mlpit.assets")) {
        nds_log("[boot] Asset pack loaded — no ROM needed.\n");
    } else {
        nds_log("[boot] No asset pack found. Checking for ROM...\n");
        const char* found_rom = NULL;
        for (int i = 0; k_rom_paths[i]; ++i) {
            FILE* f = fopen(k_rom_paths[i], "rb");
            if (f) { fclose(f); found_rom = k_rom_paths[i]; break; }
        }
        if (found_rom) {
            nds_log("[boot] Found ROM at %s\n", found_rom);
            nds_log("[boot] First run: extracting assets "
                    "(this will take a few seconds)...\n");
            if (extract_assets(found_rom, "assets/mlpit.assets")) {
                nds_log("[boot] Extraction complete. Loading asset pack...\n");
                pack_load("assets/mlpit.assets");
                nds_log("[boot] Asset pack loaded.\n");
            } else {
                nds_log("[boot] Extraction FAILED — running with stub data.\n");
            }
        } else {
            nds_log("[boot] No asset pack or ROM found — running with stub data.\n");
            nds_log("[boot] To enable game assets:\n");
            nds_log("[boot]   1. cp /path/to/your.nds rom/baserom.nds   (one time)\n");
            nds_log("[boot]   2. Re-run — assets auto-extract to assets/mlpit.assets\n");
            nds_log("[boot]   3. Delete rom/baserom.nds after extraction (optional)\n");
        }
    }

    /* Always memory-map the ROM if available — needed for FS layer */
    if (!rom_data()) {
        for (int i = 0; k_rom_paths[i]; ++i) {
            if (rom_load(k_rom_paths[i])) break;
        }
    }

    /* Initialize the host FS layer from the ROM's FAT/FNT tables */
    {
        extern void host_nds_fs_init(void);
        host_nds_fs_init();
    }

    /* === MLPIT_VIEWER mode ============================================
     * When MLPIT_VIEWER=1, we skip the SIGSEGV-prone game thread entirely
     * and run an interactive asset explorer over the loaded pack.  No
     * boot-hook seeding (the viewer paints both screens itself).
     * ================================================================ */
    if (viewer_is_enabled()) {
        nds_log("[boot] MLPIT_VIEWER=1 - skipping game thread, entering viewer\n");
        int rc = viewer_run();
        platform_shutdown();
        return rc;
    }

    /* ── VRAM boot hook ─────────────────────────────────────────────
     * Populate VRAM + configure BG registers BEFORE the game thread
     * starts, so the very first rendered frame shows something other
     * than black — even if game_start() crashes immediately.
     *
     * --show-asset N: use FAT file N's raw bytes as 4bpp tile data.
     * Normal boot:    try real game tiles first (FAT[0x62]), then synthetic.
     * ──────────────────────────────────────────────────────────── */
    if (show_asset_idx >= 0) {
        nds_log("[boot] --show-asset %d: loading asset as 4bpp tiles\n",
                show_asset_idx);
        if (!show_asset_in_vram(show_asset_idx)) {
            nds_log("[boot] Asset %d not found, falling back to synthetic\n",
                    show_asset_idx);
            boot_hook_vram();
        }
    } else if (getenv("MLPIT_DISABLE_BOOT_HOOK")) {
        /* Skip every boot-hook seed so any organic VRAM/PAL/OAM writes
         * coming from the running game are visible without our paint-by-
         * numbers fallback overlapping them. */
        nds_log("[boot] MLPIT_DISABLE_BOOT_HOOK=1 — leaving VRAM blank\n");
    } else {
        /* Try paired tile/map/palette screen first; on failure fall back to
         * the raw FAT[0x62] tile sheet, then to the synthetic test pattern. */
        if (!boot_hook_paired_screen() && !boot_hook_real_tiles()) {
            boot_hook_vram();
        }
        (void)boot_hook_paired_screen_sub();
    }

    /* In --show-asset mode, skip the game thread so only our tile
     * data is shown (makes the pipeline proof clean).             */
    if (show_asset_idx < 0) {
        /* Start the game thread. Currently runs a vblank heartbeat stub
         * (see pc/src/game_thread.c for why we cannot yet call game_start()). */
        platform_start_game_thread(game_thread_main, "mlpit_game");
    } else {
        nds_log("[show_asset] Game thread skipped — displaying asset tiles.\n");
    }

    while (platform_poll_events()) {
        const NdsInput* input = platform_input();
        (void)input;

        render_frame();
        platform_present();

        /* Tell the game thread "vblank happened" - unblocks GX_VBlankWait.
         * Safe to call even when no game thread is running. */
        platform_signal_vblank();

        platform_sleep_us(16666);
    }

    if (show_asset_idx < 0) {
        platform_stop_game_thread();
    }
    platform_shutdown();
    return 0;
}
