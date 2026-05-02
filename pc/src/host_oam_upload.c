/*
 * host_oam_upload.c — Per-frame shadow-OAM → OAM uploader.
 *
 * Static analysis (Session: trace-real-render-entry) identified the real
 * NDS sprite data flow:
 *
 *   game logic ──writes──▶  shadow_main_OAM @ 0x0205FFC0 (1 KiB)
 *                           shadow_sub_OAM  @ 0x0205FFAC (1 KiB)
 *                           ──FUN_02029518──▶ DC_FlushRange + DMA(0x203b5b4)
 *                                              ──▶ 0x07000000 / 0x07000400
 *
 * On the host:
 *   - DC_FlushRange is a no-op (no cache to flush).
 *   - The DMA stub (FUN_0203b5b4) currently does nothing.
 *   - But the ARM9 RAM region 0x02000000..0x023FFFFF is fixed-mapped on
 *     the host (see nds_arm9_ram.c), so 0x0205FFC0 / 0x0205FFAC are real
 *     readable host addresses.
 *
 * This file simply does the upload itself, every frame, by memcpy'ing
 * 0x0205FFC0 → g_oam_main and 0x0205FFAC → g_oam_sub.  obj_render() in
 * pc/src/nds_bg_render.c then rasterises them as placeholder boxes.
 *
 * The function FUN_02029518 in arm9.bin has *zero* static callers — it
 * is presumably installed as a per-frame VBlank callback by overlay code
 * that has not yet executed in our boot.  Calling it from the host main
 * loop is *equivalent* to that VBlank callback, modulo the no-op cache
 * flush.
 *
 * If MLPIT_FORCE_OBJ_ENABLE=1, also force DISPCNT bit 12 (OBJ-enable) so
 * obj_render() actually rasterises whatever lands in the shadow buffer.
 *
 * Diagnostic: counts non-zero source bytes per frame; logs first
 * transition from all-zero to non-zero so we know exactly when the
 * game starts populating the shadow OAM.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint8_t g_oam_main[1024];
extern uint8_t g_oam_sub [1024];

extern int   nds_arm9_ram_is_mapped(void);
extern void  nds_log(const char *fmt, ...);

extern uint32_t nds_reg_read32(uint32_t addr);
extern void     nds_reg_write32(uint32_t addr, uint32_t val);

#define NDS_SHADOW_OAM_MAIN  0x0205FFC0u   /* 1 KiB src for main OAM */
#define NDS_SHADOW_OAM_SUB   0x020603C0u   /* 1 KiB src for sub  OAM */
#define NDS_OAM_BYTES        1024u

/* The clear-function (FUN_02029518) checks  *(0x0205FFAC + 0x834) == 1 to
 * skip sub-OAM upload.  Mirror that condition. */
#define NDS_SUB_DISPLAY_FLAG_BASE 0x0205FFACu
#define NDS_SUB_DISPLAY_FLAG_OFFS 0x834u

#define REG_DISPCNT_MAIN     0x04000000u
#define REG_DISPCNT_SUB      0x04001000u

static int      s_main_was_zero = 1;
static int      s_sub_was_zero  = 1;
static uint64_t s_total_uploads = 0;

static int count_nonzero(const uint8_t *p, size_t n) {
    int c = 0;
    for (size_t i = 0; i < n; i++) if (p[i]) c++;
    return c;
}

void host_oam_upload_tick(int frame_idx) {
    if (!nds_arm9_ram_is_mapped()) return;

    /* Test mode: paint synthetic sprites directly into shadow OAM each
     * frame, so we can verify the entire upload-→-rasterise pipeline
     * end-to-end without depending on overlay code populating it. */
    if (getenv("MLPIT_TEST_SHADOW_OAM")) {
        uint8_t *shadow_main_w = (uint8_t *)(uintptr_t)NDS_SHADOW_OAM_MAIN;
        memset(shadow_main_w, 0, NDS_OAM_BYTES);
        /* OBJ 0: square 16x16 at (40, 40), tile 0, palette 0 — visible */
        uint16_t a0 = (uint16_t)((0u << 14) | (0u << 8) | (40u & 0xFF));
        uint16_t a1 = (uint16_t)((1u << 14) | (40u & 0x1FF));
        uint16_t a2 = 0x0001;
        shadow_main_w[0]=a0; shadow_main_w[1]=a0>>8;
        shadow_main_w[2]=a1; shadow_main_w[3]=a1>>8;
        shadow_main_w[4]=a2; shadow_main_w[5]=a2>>8;
        /* OBJ 1: 32x32 at (90, 70) */
        a0 = (uint16_t)((0u << 14) | (0u << 8) | (70u & 0xFF));
        a1 = (uint16_t)((2u << 14) | (90u & 0x1FF));
        a2 = 0x002A;
        shadow_main_w[8]=a0; shadow_main_w[9]=a0>>8;
        shadow_main_w[10]=a1; shadow_main_w[11]=a1>>8;
        shadow_main_w[12]=a2; shadow_main_w[13]=a2>>8;
        /* OBJ 2: 64x32 horizontal at (150, 130) */
        a0 = (uint16_t)((1u << 14) | (0u << 8) | (130u & 0xFF));
        a1 = (uint16_t)((3u << 14) | (150u & 0x1FF));
        a2 = 0x00B7;
        shadow_main_w[16]=a0; shadow_main_w[17]=a0>>8;
        shadow_main_w[18]=a1; shadow_main_w[19]=a1>>8;
        shadow_main_w[20]=a2; shadow_main_w[21]=a2>>8;
    }

    const uint8_t *shadow_main = (const uint8_t *)(uintptr_t)NDS_SHADOW_OAM_MAIN;
    const uint8_t *shadow_sub  = (const uint8_t *)(uintptr_t)NDS_SHADOW_OAM_SUB;

    int nz_main = count_nonzero(shadow_main, NDS_OAM_BYTES);
    if (nz_main > 0) {
        memcpy(g_oam_main, shadow_main, NDS_OAM_BYTES);
        if (s_main_was_zero) {
            s_main_was_zero = 0;
            nds_log("[oam-up] frame=%d: shadow_main first non-zero "
                    "(%d/1024 bytes) — uploading to OAM main\n",
                    frame_idx, nz_main);
        }
        s_total_uploads++;
    }

    /* Sub display flag check: skip sub upload if [+0x834] == 1 */
    uint32_t flag = *(volatile uint32_t *)(uintptr_t)
                    (NDS_SUB_DISPLAY_FLAG_BASE + NDS_SUB_DISPLAY_FLAG_OFFS);
    if (flag != 1) {
        int nz_sub = count_nonzero(shadow_sub, NDS_OAM_BYTES);
        if (nz_sub > 0) {
            memcpy(g_oam_sub, shadow_sub, NDS_OAM_BYTES);
            if (s_sub_was_zero) {
                s_sub_was_zero = 0;
                nds_log("[oam-up] frame=%d: shadow_sub first non-zero "
                        "(%d/1024 bytes) — uploading to OAM sub\n",
                        frame_idx, nz_sub);
            }
            s_total_uploads++;
        }
    }

    /* Optional: force OBJ-enable in DISPCNT so obj_render() rasterises
     * the placeholder boxes even if the game hasn't configured DISPCNT
     * yet.  Off by default — keeps default behaviour clean. */
    if (getenv("MLPIT_FORCE_OBJ_ENABLE")) {
        uint32_t dc_main = nds_reg_read32(REG_DISPCNT_MAIN);
        if (!((dc_main >> 12) & 1)) {
            nds_reg_write32(REG_DISPCNT_MAIN, dc_main | (1u << 12));
        }
        uint32_t dc_sub  = nds_reg_read32(REG_DISPCNT_SUB);
        if (!((dc_sub  >> 12) & 1)) {
            nds_reg_write32(REG_DISPCNT_SUB,  dc_sub  | (1u << 12));
        }
    }
}

void host_oam_upload_summary(void) {
    nds_log("[oam-up] summary: total_uploads=%llu main_zero=%d sub_zero=%d\n",
            (unsigned long long)s_total_uploads,
            s_main_was_zero, s_sub_was_zero);
}
