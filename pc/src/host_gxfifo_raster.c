/*
 * host_gxfifo_raster.c — minimal NDS GX command interpreter + software
 * rasteriser for the PC port.
 *
 * Scope (deliberately tiny):
 *   - Only the few GX commands needed by the 2D-overlay sprite emit path
 *     observed in FUN_0200FCB4: BEGIN_VTXS / VTX_16 / VTX_XY / TEXCOORD /
 *     COLOR / END_VTXS, plus TEXIMAGE_PARAM / PLTT_BASE for state.
 *   - Primitive type 1 (quads); triangulated as two triangles.
 *   - Output buffer: 256x192 BGR555 (matches platform_sdl.c top/bottom
 *     framebuffer layout, so we composite by simple alpha-aware copy).
 *   - Texture sampling: 4bpp tex (NDS format 3) with 16-colour palette,
 *     8x8 .. 256x256 sizes via TEXIMAGE_PARAM s/t bits.
 *   - No matrix transform, no clipping beyond viewport, no Z-buffer.
 *
 * The interpreter consumes a software command queue (host_gxfifo_push_*)
 * — it is NOT driven by polling MMIO writes.  When the natural game
 * pipeline starts emitting commands through the FIFO MMIO at
 * 0x04000400, a small adapter can be added in nds_hw_io.c that mirrors
 * each write into this queue.  Until then this file is exercised by
 * the self-test path (host_gxfifo_raster_self_test) which builds a
 * checkerboard textured quad in code so platform_present() can blit a
 * visible result and prove the rasteriser draws.
 */
#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nds_platform.h"  /* NDS_SCREEN_WIDTH/HEIGHT, framebuffer accessors */

/* ===== Command queue ================================================= */

#define GXR_QUEUE_CAP 4096

typedef struct {
    uint8_t  cmd;
    uint32_t arg;
} gxr_cmd_t;

static gxr_cmd_t g_queue[GXR_QUEUE_CAP];
static int       g_queue_len = 0;

/* GX command opcode IDs.  Only a subset is decoded; unknown commands
 * are buffered with their args but ignored when popped. */
#define GX_MTX_MODE       0x10
#define GX_COLOR          0x20
#define GX_NORMAL         0x21
#define GX_TEXCOORD       0x22
#define GX_VTX_16         0x23
#define GX_VTX_10         0x24
#define GX_VTX_XY         0x25
#define GX_VTX_XZ         0x26
#define GX_VTX_YZ         0x27
#define GX_VTX_DIFF       0x28
#define GX_TEXIMAGE_PARAM 0x2A
#define GX_PLTT_BASE      0x2B
#define GX_BEGIN_VTXS     0x40
#define GX_END_VTXS       0x41
#define GX_SWAP_BUFFERS   0x50
#define GX_VIEWPORT       0x60

void host_gxfifo_push(uint8_t cmd, uint32_t arg)
{
    if (g_queue_len >= GXR_QUEUE_CAP) return;
    g_queue[g_queue_len].cmd = cmd;
    g_queue[g_queue_len].arg = arg;
    g_queue_len++;
}

void host_gxfifo_clear(void)
{
    g_queue_len = 0;
}

/* ===== Rasteriser state ============================================== */

#define GXR_FB_W 256
#define GXR_FB_H 192
#define GXR_MAX_VTX_PER_PRIM 4

static uint16_t g_fb[GXR_FB_W * GXR_FB_H];
static uint8_t  g_fb_valid[GXR_FB_W * GXR_FB_H]; /* coverage */
static int      g_fb_dirty = 0;

typedef struct {
    int32_t x, y, z;   /* fixed point: 1.0 == 4096 (12-bit fractional) */
    int32_t s, t;      /* tex coords, 1.0 == 16 */
    uint16_t color;    /* BGR555 */
} gxr_vtx_t;

static gxr_vtx_t g_vtx[GXR_MAX_VTX_PER_PRIM];
static int       g_vtx_count = 0;
static int       g_prim_type = -1;

/* Current attribute state (sticky, NDS-style). */
static int32_t  g_cur_s = 0, g_cur_t = 0;
static uint16_t g_cur_color = 0x7FFF;
static uint32_t g_cur_teximage_param = 0;
static uint32_t g_cur_pltt_base = 0;

/* Texture VRAM shadow.  In real NDS this lives in banks A/B/C/D mapped
 * by GX_SetBankForTex.  For the self-test we just point at a 4 KiB
 * blob in our process image; offset 0 == start of g_tex_vram. */
static uint8_t  g_tex_vram[256 * 256];   /* enough for one 256x256 4bpp */
static uint16_t g_pal_vram[256 * 16];    /* up to 256 16-colour palettes */

/* ===== Framebuffer access ============================================ */

const uint16_t *host_gxfifo_raster_framebuffer(void) { return g_fb; }
int host_gxfifo_raster_dirty(void) { return g_fb_dirty; }
void host_gxfifo_raster_clear_dirty(void) { g_fb_dirty = 0; }

/* Public APIs used by the synth-sprite pipeline (Track B/C of the
 * fcb4-real-vertices session) so caller code can preload texture +
 * palette VRAM with ROM-derived bytes. */
void host_gxfifo_raster_install_texture(const void *src, size_t n)
{
    if (n > sizeof(g_tex_vram)) n = sizeof(g_tex_vram);
    memcpy(g_tex_vram, src, n);
    if (n < sizeof(g_tex_vram))
        memset(g_tex_vram + n, 0, sizeof(g_tex_vram) - n);
}

void host_gxfifo_raster_install_palette(const uint16_t *src, size_t entries)
{
    size_t bytes = entries * sizeof(uint16_t);
    if (bytes > sizeof(g_pal_vram)) bytes = sizeof(g_pal_vram);
    memcpy(g_pal_vram, src, bytes);
}

void host_gxfifo_raster_clear_fb(void)
{
    memset(g_fb, 0, sizeof(g_fb));
    memset(g_fb_valid, 0, sizeof(g_fb_valid));
    g_fb_dirty = 0;
}

static void fb_clear(void)
{
    memset(g_fb, 0, sizeof(g_fb));
    memset(g_fb_valid, 0, sizeof(g_fb_valid));
}

/* ===== Texture sampling ============================================== */

/* Decode a 4bpp texel at (s,t) using the current TEXIMAGE_PARAM and
 * PLTT_BASE.  TEXIMAGE_PARAM bit layout (subset we care about):
 *   bits  0..15  VRAM offset (in 8-byte units)
 *   bits 20..22  texture S size (8 << n)
 *   bits 23..25  texture T size (8 << n)
 *   bits 26..28  format (we only handle 3 = 4bpp paletted)
 * PLTT_BASE: bits 0..12 palette base in VRAM, units of 16 bytes for
 *   4bpp/4colour textures.  We treat it as palette-index << 4. */
static uint16_t sample_texel(int32_t s_fp, int32_t t_fp)
{
    int s_size = 8 << ((g_cur_teximage_param >> 20) & 7);
    int t_size = 8 << ((g_cur_teximage_param >> 23) & 7);
    if (s_size <= 0) s_size = 8;
    if (t_size <= 0) t_size = 8;

    /* TEXCOORD is 12.4 fixed; one pixel == 16. Wrap by mask. */
    int us = ((s_fp >> 4) % s_size + s_size) % s_size;
    int ut = ((t_fp >> 4) % t_size + t_size) % t_size;

    uint32_t tex_off = (g_cur_teximage_param & 0xFFFF) << 3;
    if (tex_off + (uint32_t)((ut * s_size + us) >> 1) >= sizeof(g_tex_vram)) {
        return g_cur_color;
    }
    uint8_t byte = g_tex_vram[tex_off + ((ut * s_size + us) >> 1)];
    uint8_t idx  = (us & 1) ? (byte >> 4) : (byte & 0x0F);

    uint32_t pal_off = (g_cur_pltt_base & 0x1FFF) << 4;
    /* pal_vram is u16-indexed; convert byte offset to halfword index. */
    uint32_t pal_idx = pal_off / 2 + idx;
    if (pal_idx >= sizeof(g_pal_vram) / 2) return g_cur_color;
    uint16_t col = g_pal_vram[pal_idx];
    /* Index 0 of a 4bpp palette is conventionally transparent on NDS;
     * skip the pixel by returning a sentinel the rasteriser checks. */
    if (idx == 0) return 0xFFFF; /* sentinel: skip */
    return col & 0x7FFF;
}

/* ===== Triangle rasterisation (scanline, integer barycentric) ======== */

static int32_t edge(int32_t ax, int32_t ay, int32_t bx, int32_t by,
                    int32_t cx, int32_t cy)
{
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

static void put_pixel(int x, int y, uint16_t col)
{
    if ((unsigned)x >= GXR_FB_W || (unsigned)y >= GXR_FB_H) return;
    g_fb[y * GXR_FB_W + x] = col;
    g_fb_valid[y * GXR_FB_W + x] = 1;
    g_fb_dirty = 1;
}

static int32_t i32_min3(int32_t a, int32_t b, int32_t c)
{
    int32_t m = a < b ? a : b;
    return m < c ? m : c;
}
static int32_t i32_max3(int32_t a, int32_t b, int32_t c)
{
    int32_t m = a > b ? a : b;
    return m > c ? m : c;
}

static void draw_triangle(const gxr_vtx_t *v0,
                          const gxr_vtx_t *v1,
                          const gxr_vtx_t *v2)
{
    /* Project: divide fixed-point coords by 4096 to get pixel ints.
     * The synth stream uses screen-space coords directly so this just
     * shifts off the 12 fractional bits. */
    int32_t x0 = v0->x >> 12, y0 = v0->y >> 12;
    int32_t x1 = v1->x >> 12, y1 = v1->y >> 12;
    int32_t x2 = v2->x >> 12, y2 = v2->y >> 12;

    /* Reject degenerate / back-facing (CCW). */
    int32_t area2 = edge(x0, y0, x1, y1, x2, y2);
    if (area2 == 0) return;
    /* Force CCW so barycentric > 0 inside. */
    int swap = (area2 < 0);

    int32_t minx = i32_min3(x0, x1, x2);
    int32_t miny = i32_min3(y0, y1, y2);
    int32_t maxx = i32_max3(x0, x1, x2);
    int32_t maxy = i32_max3(y0, y1, y2);
    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx >= GXR_FB_W) maxx = GXR_FB_W - 1;
    if (maxy >= GXR_FB_H) maxy = GXR_FB_H - 1;
    if (minx > maxx || miny > maxy) return;

    int32_t total = swap ? -area2 : area2;
    if (total <= 0) return;

    for (int32_t py = miny; py <= maxy; py++) {
        for (int32_t px = minx; px <= maxx; px++) {
            int32_t w0 = edge(x1, y1, x2, y2, px, py);
            int32_t w1 = edge(x2, y2, x0, y0, px, py);
            int32_t w2 = edge(x0, y0, x1, y1, px, py);
            if (swap) { w0 = -w0; w1 = -w1; w2 = -w2; }
            if (w0 < 0 || w1 < 0 || w2 < 0) continue;

            /* Interpolate tex coords in 12.4 fixed, integer-only. */
            int64_t s = ((int64_t)w0 * v0->s + (int64_t)w1 * v1->s
                       + (int64_t)w2 * v2->s) / total;
            int64_t t = ((int64_t)w0 * v0->t + (int64_t)w1 * v1->t
                       + (int64_t)w2 * v2->t) / total;

            uint16_t col = (g_cur_teximage_param != 0)
                         ? sample_texel((int32_t)s, (int32_t)t)
                         : v0->color;
            if (col == 0xFFFF) continue;  /* transparent sentinel */
            put_pixel((int)px, (int)py, col);
        }
    }
}

/* ===== Primitive submission ========================================== */

static void submit_prim(void)
{
    if (g_prim_type < 0 || g_vtx_count < 3) {
        g_vtx_count = 0;
        return;
    }
    /* Type 0 = tri list, 1 = quad list, 2 = tri strip, 3 = quad strip.
     * For now we handle quads (the common 2D-sprite case) and tris. */
    switch (g_prim_type) {
    case 0: /* triangles */
        if (g_vtx_count >= 3) draw_triangle(&g_vtx[0], &g_vtx[1], &g_vtx[2]);
        break;
    case 1: /* quads */
        if (g_vtx_count >= 4) {
            draw_triangle(&g_vtx[0], &g_vtx[1], &g_vtx[2]);
            draw_triangle(&g_vtx[0], &g_vtx[2], &g_vtx[3]);
        }
        break;
    default:
        break;
    }
    g_vtx_count = 0;
}

/* ===== Command interpreter =========================================== */

void host_gxfifo_raster_run(void)
{
    g_vtx_count = 0;
    g_prim_type = -1;
    int i = 0;
    while (i < g_queue_len) {
        gxr_cmd_t *c = &g_queue[i++];
        switch (c->cmd) {
        case GX_BEGIN_VTXS:
            g_prim_type = (int)(c->arg & 3);
            g_vtx_count = 0;
            break;
        case GX_END_VTXS:
            submit_prim();
            g_prim_type = -1;
            break;
        case GX_COLOR:
            g_cur_color = (uint16_t)(c->arg & 0x7FFF);
            break;
        case GX_TEXCOORD:
            /* arg = (T<<16)|S, both s16 12.4 fixed. */
            g_cur_s = (int32_t)(int16_t)(c->arg & 0xFFFF);
            g_cur_t = (int32_t)(int16_t)((c->arg >> 16) & 0xFFFF);
            break;
        case GX_VTX_16: {
            /* VTX_16 is a 2-arg command: arg N = (Y<<16)|X (s16),
             * arg N+1 low 16 = Z. We pack X/Y here; pull Z from the
             * next queue entry if present. */
            int32_t x = (int32_t)(int16_t)(c->arg & 0xFFFF);
            int32_t y = (int32_t)(int16_t)((c->arg >> 16) & 0xFFFF);
            int32_t z = 0;
            if (i < g_queue_len && g_queue[i].cmd == GX_VTX_16) {
                z = (int32_t)(int16_t)(g_queue[i].arg & 0xFFFF);
                i++;
            }
            if (g_vtx_count < GXR_MAX_VTX_PER_PRIM) {
                g_vtx[g_vtx_count].x = x << 8; /* s16 -> Q12 (shift 12-4=8) */
                g_vtx[g_vtx_count].y = y << 8;
                g_vtx[g_vtx_count].z = z << 8;
                g_vtx[g_vtx_count].s = g_cur_s;
                g_vtx[g_vtx_count].t = g_cur_t;
                g_vtx[g_vtx_count].color = g_cur_color;
                g_vtx_count++;
            }
            break;
        }
        case GX_VTX_XY: {
            /* arg = (Y<<16)|X, screen-space pixel coords (no fractional). */
            int32_t x = (int32_t)(int16_t)(c->arg & 0xFFFF);
            int32_t y = (int32_t)(int16_t)((c->arg >> 16) & 0xFFFF);
            if (g_vtx_count < GXR_MAX_VTX_PER_PRIM) {
                g_vtx[g_vtx_count].x = x << 12;
                g_vtx[g_vtx_count].y = y << 12;
                g_vtx[g_vtx_count].z = 0;
                g_vtx[g_vtx_count].s = g_cur_s;
                g_vtx[g_vtx_count].t = g_cur_t;
                g_vtx[g_vtx_count].color = g_cur_color;
                g_vtx_count++;
            }
            break;
        }
        case GX_TEXIMAGE_PARAM:
            g_cur_teximage_param = c->arg;
            break;
        case GX_PLTT_BASE:
            g_cur_pltt_base = c->arg;
            break;
        default:
            /* swallow */
            break;
        }
    }
}

/* ===== Self-test: hand-crafted textured quad ========================= */

static void install_checkerboard_texture(void)
{
    /* 8x8 4bpp checkerboard, two colours: index 1 (white) and index 2
     * (red).  Index 0 is transparent. */
    memset(g_tex_vram, 0, sizeof(g_tex_vram));
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x += 2) {
            uint8_t a = (((x   ^ y) & 1) ? 1 : 2);
            uint8_t b = ((((x+1)^ y) & 1) ? 1 : 2);
            g_tex_vram[y * 4 + (x >> 1)] = (uint8_t)(a | (b << 4));
        }
    }
    /* Palette: idx0 transparent, idx1 white, idx2 red, rest 0. */
    memset(g_pal_vram, 0, sizeof(g_pal_vram));
    g_pal_vram[0] = 0;          /* never sampled — transparent */
    g_pal_vram[1] = 0x7FFF;     /* BGR555 white */
    g_pal_vram[2] = 0x001F;     /* red (R=31, G=0, B=0) */
}

void host_gxfifo_raster_self_test(void)
{
    fb_clear();
    install_checkerboard_texture();
    host_gxfifo_clear();

    /* Texture state: 8x8 (s_size=0,t_size=0), format 3 (4bpp), VRAM
     * offset 0 (bits 0..15 = 0). */
    uint32_t tex_param = (3u << 26);  /* s_size = t_size = 0 -> 8 */
    host_gxfifo_push(GX_TEXIMAGE_PARAM, tex_param);
    host_gxfifo_push(GX_PLTT_BASE, 0);

    host_gxfifo_push(GX_COLOR, 0x7FFF);
    host_gxfifo_push(GX_BEGIN_VTXS, 1); /* quads */

    /* Quad spanning (16,16)..(80,80) in screen coords, with tex coords
     * spanning (0,0)..(8*16, 8*16) in 12.4 = full texture wrap. */
    host_gxfifo_push(GX_TEXCOORD, ((  0) << 16) | (  0));
    host_gxfifo_push(GX_VTX_XY,   (16 << 16) | 16);

    host_gxfifo_push(GX_TEXCOORD, ((  0) << 16) | (8 * 16));
    host_gxfifo_push(GX_VTX_XY,   (16 << 16) | 80);

    host_gxfifo_push(GX_TEXCOORD, ((8 * 16) << 16) | (8 * 16));
    host_gxfifo_push(GX_VTX_XY,   (80 << 16) | 80);

    host_gxfifo_push(GX_TEXCOORD, ((8 * 16) << 16) | (  0));
    host_gxfifo_push(GX_VTX_XY,   (80 << 16) | 16);

    host_gxfifo_push(GX_END_VTXS, 0);

    host_gxfifo_raster_run();

    fprintf(stderr, "[gxraster] self-test: ran %d cmds, fb_dirty=%d\n",
            g_queue_len, g_fb_dirty);
}

/* ===== Composite onto NDS top framebuffer ============================ */

void host_gxfifo_raster_composite_to_top(uint16_t *top_fb)
{
    if (!g_fb_dirty) return;
    /* Where coverage exists, copy our pixel; otherwise leave the existing
     * BG layer pixel intact.  This matches BG layer 0 / 3D engine
     * behaviour where unrendered pixels show the layer below. */
    for (int i = 0; i < GXR_FB_W * GXR_FB_H; i++) {
        if (g_fb_valid[i]) top_fb[i] = g_fb[i];
    }
}
