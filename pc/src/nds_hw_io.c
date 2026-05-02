/*
 * nds_hw_io.c — Per-frame OAM monitor.
 *
 * The NDS OAM RAM (main 0x07000000-0x070003FF, sub 0x07000400-0x070007FF)
 * is backed by host static buffers exported by nds_hw_stubs.c.  Writes to
 * those addresses go through nds_addr_to_host(), which returns the raw
 * buffer pointer — there is no per-write trap site we can hook without
 * touching every memcpy / memset path in the decomp.
 *
 * Instead we keep a shadow copy of the entire 2 KiB OAM RAM and call
 * nds_oam_monitor_tick() once per frame.  Any 8-byte OBJ entry (4 attr
 * words, but real OAM uses 3 × u16 + 1 × u16 affine slot) whose first
 * 6 bytes changed since last tick is logged with `[oam] obj=N attr0/1/2`.
 *
 * Throttled to one log line per (engine, obj_idx) per N changes so that
 * a steady stream of tweens doesn't flood stderr.
 *
 * At frame 600 the entire 2 KiB region is dumped to
 *   .session-work/oam_dump_frame600.bin
 * for offline inspection.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern uint8_t g_oam_main[1024];
extern uint8_t g_oam_sub [1024];

#define OAM_BYTES_PER_ENGINE  1024
#define OAM_OBJ_STRIDE        8
#define OAM_OBJS_PER_ENGINE   (OAM_BYTES_PER_ENGINE / OAM_OBJ_STRIDE)  /* 128 */

static uint8_t  s_shadow[2048];
static uint32_t s_change_count[2 * OAM_OBJS_PER_ENGINE];
static int      s_initialised = 0;
static uint64_t s_total_changes = 0;
static int      s_dumped_frame600 = 0;
/* Throttle — emit one log per OBJ slot every N detected changes. */
#define OAM_LOG_EVERY_N_CHANGES 16

static void log_obj(int is_sub, int obj_idx, const uint8_t *cur)
{
    uint16_t a0 = (uint16_t)(cur[0] | (cur[1] << 8));
    uint16_t a1 = (uint16_t)(cur[2] | (cur[3] << 8));
    uint16_t a2 = (uint16_t)(cur[4] | (cur[5] << 8));
    fprintf(stderr,
            "[oam] %s obj=%3d attr0=%04X attr1=%04X attr2=%04X "
            "(seen=%u)\n",
            is_sub ? "sub " : "main",
            obj_idx, a0, a1, a2,
            (unsigned)s_change_count[is_sub * OAM_OBJS_PER_ENGINE + obj_idx]);
    fflush(stderr);
}

static void ensure_init(void)
{
    if (s_initialised) return;
    memcpy(s_shadow + 0,    g_oam_main, OAM_BYTES_PER_ENGINE);
    memcpy(s_shadow + 1024, g_oam_sub,  OAM_BYTES_PER_ENGINE);
    memset(s_change_count, 0, sizeof(s_change_count));
    s_initialised = 1;
}

static void dump_oam_to_file(void)
{
    FILE *f = fopen(".session-work/oam_dump_frame600.bin", "wb");
    if (!f) {
        fprintf(stderr, "[oam] could not open oam_dump_frame600.bin "
                "(.session-work/ missing? — skipping dump)\n");
        return;
    }
    fwrite(g_oam_main, 1, OAM_BYTES_PER_ENGINE, f);
    fwrite(g_oam_sub,  1, OAM_BYTES_PER_ENGINE, f);
    fclose(f);
    fprintf(stderr, "[oam] dumped 2048 bytes -> "
            ".session-work/oam_dump_frame600.bin "
            "(total_changes=%llu)\n",
            (unsigned long long)s_total_changes);
    fflush(stderr);
}

void nds_oam_monitor_tick(int frame_idx)
{
    ensure_init();

    for (int eng = 0; eng < 2; eng++) {
        const uint8_t *live   = (eng == 0) ? g_oam_main : g_oam_sub;
        uint8_t       *shadow = s_shadow + (eng * OAM_BYTES_PER_ENGINE);
        for (int obj = 0; obj < OAM_OBJS_PER_ENGINE; obj++) {
            const uint8_t *cur  = live   + obj * OAM_OBJ_STRIDE;
            uint8_t       *prev = shadow + obj * OAM_OBJ_STRIDE;
            /* Compare first 6 bytes (the 3 attr u16s — the 4th is shared
             * affine data, written separately and noisier). */
            if (memcmp(cur, prev, 6) != 0) {
                int slot_idx = eng * OAM_OBJS_PER_ENGINE + obj;
                s_change_count[slot_idx]++;
                s_total_changes++;
                if ((s_change_count[slot_idx] % OAM_LOG_EVERY_N_CHANGES) == 1) {
                    log_obj(eng, obj, cur);
                }
                memcpy(prev, cur, OAM_OBJ_STRIDE);
            }
        }
    }

    if (frame_idx == 600 && !s_dumped_frame600) {
        s_dumped_frame600 = 1;
        dump_oam_to_file();
    }
}

/* Called from game_thread.c heartbeat / inner loop.  Safe to call from
 * any thread because both buffers are read-only here. */
void nds_oam_monitor_summary(void)
{
    fprintf(stderr, "[oam] summary: total_changes=%llu\n",
            (unsigned long long)s_total_changes);
    fflush(stderr);
}
