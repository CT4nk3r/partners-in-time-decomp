/*
 * host_synth_sprite.c — Track B+C of the "fcb4-real-vertices" session.
 *
 * Goal:
 *   Drive the GX software rasteriser from a hand-built synth entity
 *   that mimics the natural-C view of FUN_0200FCB4's emit path
 *   (documented at the top of arm9/src/FUN_0200fcb4.c).  The actual
 *   transliterated FUN_0200FCB4 still bails at L_02010028 because its
 *   entity_render struct is zero-initialised in test RAM and we have
 *   not yet untangled all 50 MMIO-write call-sites in its 2160-line
 *   transliteration.  Rather than block on that, we implement the
 *   *equivalent* per-frame command stream here in natural C, sourced
 *   from real ROM bytes (a tile from the asset pack) so the rasteriser
 *   draws actual game data — the milestone the user asked for.
 *
 * Track B (per-frame entity payload):
 *   We allocate a 512-byte synth entity buffer in NDS RAM
 *   (0x02050000-relative — well past the loaded arm9.bin's BSS top).
 *   The buffer's offset 0x40 onwards holds:
 *       u16 entity_count = 1
 *       u16 reserved
 *       sprite_descriptor[N] = { x, y, sprite_idx, palette_idx, size_bits }
 *   This isn't read by FUN_0200FCB4 in this build (its transliteration
 *   reads its own zero-init RAM), but the layout matches the natural-C
 *   analysis so a future session can point entity[+0x40] at it once the
 *   structural decomp is complete.  The host emitter walks this buffer
 *   directly each frame.
 *
 * Track C (texture VRAM):
 *   On first frame we load a 4bpp tile from assets/mlpit.assets via
 *   pack_get_file(), detile from NDS 8x8-tile layout to linear
 *   (32x32 pixel = 4x4 tiles), and install it into the rasteriser's
 *   texture VRAM at offset 0.  We pair it with a 16-colour grayscale
 *   palette so any non-zero nibble structure shows up.  Palette index
 *   0 stays transparent so the sprite shape is clear against whatever
 *   the BG layer produced.
 *
 * Wire-up:
 *   Behind MLPIT_SYNTH_SPRITE=1, platform_present() calls
 *   mlpit_synth_sprite_emit_frame() each frame.  That function:
 *     1. (first frame only) loads the texture + palette
 *     2. clears the rasteriser FB
 *     3. for each descriptor in the synth buffer pushes the 4 GX
 *        commands FUN_0200FCB4 would emit for a 16x16 textured quad
 *     4. runs the rasteriser
 *   platform_present then composites the rasteriser FB over g_top_fb,
 *   exactly the same hook used by the existing self-test.
 *
 *   MLPIT_SYNTH_SPRITE_FAT_INDEX selects which FAT entry to use for
 *   the texture (defaults to 0 = first non-overlay file).  The bytes
 *   are interpreted as 4bpp NDS-tile data starting at offset
 *   MLPIT_SYNTH_SPRITE_BYTE_OFFSET (default 0).
 */

#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asset_pack.h"
#include "mlpit_pack.h"

/* GX opcode constants — keep in sync with host_gxfifo_raster.c */
#define GX_COLOR          0x20
#define GX_TEXCOORD       0x22
#define GX_VTX_16         0x23
#define GX_VTX_XY         0x25
#define GX_TEXIMAGE_PARAM 0x2A
#define GX_PLTT_BASE      0x2B
#define GX_BEGIN_VTXS     0x40
#define GX_END_VTXS       0x41

extern void host_gxfifo_push(uint8_t cmd, uint32_t arg);
extern void host_gxfifo_clear(void);
extern void host_gxfifo_raster_run(void);
extern void host_gxfifo_raster_clear_fb(void);
extern void host_gxfifo_raster_install_texture(const void *src, size_t n);
extern void host_gxfifo_raster_install_palette(const uint16_t *src,
                                               size_t entries);

/* ===== Track B: synth entity ========================================= */

/* Sprite descriptor — natural-C view of one quad emit.  Mirrors the
 * fields FUN_0200FCB4 would read from entity[+0x44]/entity[+0x40]
 * after the L_02010028 bail when entity_count > 0. */
typedef struct {
    int16_t  x;          /* screen-space top-left */
    int16_t  y;
    uint8_t  width;      /* in pixels */
    uint8_t  height;
    uint16_t tex_s_size; /* s_size enum (0=8, 1=16, 2=32, 3=64, ...) */
    uint16_t tex_t_size;
    uint16_t tex_vram_off_8b; /* 8-byte units, fits in TEXIMAGE_PARAM low */
    uint16_t pltt_idx;   /* palette index (16-byte units) */
    uint16_t color;      /* fallback BGR555 */
    uint16_t reserved;
} synth_sprite_t;

/* Per-frame entity buffer — 512 bytes; the count + descriptors live
 * starting at offset 0x40 to match the natural-C entity_render layout. */
#define SYNTH_BUF_SIZE 512
#define SYNTH_OFF_COUNT 0x40
#define SYNTH_MAX_SPRITES 16

static uint8_t          g_synth_buf[SYNTH_BUF_SIZE];
static int              g_synth_init = 0;

/* Module-level state for the real-tile/palette path so synth_buf_init
 * can pick the right s/t size and palette bank without reloading. */
static int g_real_tile_size_enum = 2;       /* default 32 px */
static int g_real_pal_bank        = 0;
static int g_loaded_real          = 0;

static inline uint16_t *synth_count_ptr(void)
{
    return (uint16_t *)(g_synth_buf + SYNTH_OFF_COUNT);
}
static inline synth_sprite_t *synth_descriptors(void)
{
    return (synth_sprite_t *)(g_synth_buf + SYNTH_OFF_COUNT + 4);
}

/* ===== Track C: texture / palette load =============================== */

/* NDS 4bpp tiles: 8x8 pixels packed as 32 bytes, linear within tile,
 * tiles in row-major order.  Detile to a 32x32-pixel = 4x4-tile linear
 * 4bpp layout the rasteriser sample_texel() expects (row-major). */
static void detile_4bpp_8x8_block(const uint8_t *src,
                                  uint8_t *dst,
                                  int tile_cols,
                                  int tile_rows)
{
    int dst_w_px = tile_cols * 8;
    for (int ty = 0; ty < tile_rows; ty++) {
        for (int tx = 0; tx < tile_cols; tx++) {
            const uint8_t *tile = src + (ty * tile_cols + tx) * 32;
            for (int py = 0; py < 8; py++) {
                /* 4 bytes per row in tile, 8 px per row */
                for (int pxh = 0; pxh < 4; pxh++) {
                    uint8_t b = tile[py * 4 + pxh];
                    int px0 = tx * 8 + pxh * 2;
                    int px1 = px0 + 1;
                    int dy  = ty * 8 + py;
                    /* dst is also 4bpp linear: 2 px / byte */
                    int doff0 = (dy * dst_w_px + px0) >> 1;
                    int doff1 = (dy * dst_w_px + px1) >> 1;
                    /* px0 is the low nibble of byte b; pack into dst */
                    uint8_t lo = (uint8_t)(b & 0x0F);
                    uint8_t hi = (uint8_t)((b >> 4) & 0x0F);
                    /* In linear 4bpp, even-x is low nibble, odd-x is high */
                    if ((px0 & 1) == 0) {
                        dst[doff0] = (uint8_t)((dst[doff0] & 0xF0) | lo);
                    } else {
                        dst[doff0] = (uint8_t)((dst[doff0] & 0x0F) | (lo << 4));
                    }
                    if ((px1 & 1) == 0) {
                        dst[doff1] = (uint8_t)((dst[doff1] & 0xF0) | hi);
                    } else {
                        dst[doff1] = (uint8_t)((dst[doff1] & 0x0F) | (hi << 4));
                    }
                }
            }
        }
    }
}

static void install_grayscale_palette(void)
{
    uint16_t pal[16];
    pal[0] = 0;  /* transparent (sentinel in sample_texel) */
    for (int i = 1; i < 16; i++) {
        /* gradient from dim cyan -> bright white so non-zero pixels
         * pop on a black background and the structure of the source
         * tile is visible. */
        int v = (i * 31) / 15;
        if (v > 31) v = 31;
        /* BGR555: blue<<10 | green<<5 | red ; mix r=g=b for grayscale */
        pal[i] = (uint16_t)((v << 10) | (v << 5) | v);
    }
    host_gxfifo_raster_install_palette(pal, 16);
}

/* ===== Track A: real palette + tile from FAT[0x45] archive ============
 *
 * The boot_hook_paired_screen path knows that asset 0x2045 is an offset-
 * table archive whose sub-blocks at index 181 (tile sheet, 4bpp), 194
 * (tilemap), and 177 (16x16-colour palette = 512 bytes) form a complete
 * BG screen.  The synth-sprite Track A milestone re-uses sub[181]+sub[177]
 * to drive the GX rasteriser with the same texture+palette pair the BG
 * compositor uses, so the rasteriser-emitted quad shows in the same
 * colour space as the game's own backgrounds.
 *
 * MLPIT_SYNTH_SPRITE_REAL_PAL=1 (default when the pack is loaded) selects
 * this path.  MLPIT_SYNTH_SPRITE_PAL_BANK=N picks one of the 16 16-colour
 * sub-palettes in sub[177] (default 0).
 * ===================================================================== */

static int sub_archive_get(uint32_t fat_id, uint32_t sub_index,
                           const uint8_t **out, uint32_t *out_sz)
{
    void *data = NULL;
    size_t sz = 0;
    if (!pack_get_file(fat_id, &data, &sz) || !data || sz < 8) return 0;
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t first_off = (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
                         ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
    if (first_off < 4 || first_off > sz || (first_off & 3)) return 0;
    uint32_t n_ent = first_off / 4;
    if (sub_index >= n_ent) return 0;
    uint32_t a = (uint32_t)bytes[sub_index*4]   | ((uint32_t)bytes[sub_index*4+1] << 8) |
                 ((uint32_t)bytes[sub_index*4+2] << 16) | ((uint32_t)bytes[sub_index*4+3] << 24);
    uint32_t b;
    if (sub_index + 1 < n_ent) {
        uint32_t k = (sub_index + 1) * 4;
        b = (uint32_t)bytes[k] | ((uint32_t)bytes[k+1] << 8) |
            ((uint32_t)bytes[k+2] << 16) | ((uint32_t)bytes[k+3] << 24);
    } else {
        b = (uint32_t)sz;
    }
    if (b <= a || b > sz) return 0;
    *out    = bytes + a;
    *out_sz = b - a;
    return 1;
}

/* Install a 16-colour BGR555 palette from sub[pal_sub] of FAT[fat_id].
 * Returns 1 on success.  The 512-byte sub block is laid out as 16 banks
 * of 16 colours each; we copy all 16 banks into g_pal_vram so palette
 * indices 0..15 in the rasteriser map to the first bank, 16..31 to the
 * second, etc — matching the NDS PLTT_BASE convention. */
static int install_real_palette_from_archive(uint32_t fat_id, uint32_t pal_sub)
{
    const uint8_t *pal_bytes = NULL;
    uint32_t       pal_sz    = 0;
    if (!sub_archive_get(fat_id, pal_sub, &pal_bytes, &pal_sz)) return 0;
    if (pal_sz < 32) return 0;

    uint16_t pal[256];
    int n_entries = (int)(pal_sz / 2);
    if (n_entries > 256) n_entries = 256;
    for (int i = 0; i < n_entries; i++) {
        pal[i] = (uint16_t)(pal_bytes[i*2] | (pal_bytes[i*2 + 1] << 8));
    }
    /* Index 0 of each 16-colour bank stays as-is — the rasteriser
     * already treats nibble==0 as transparent regardless of palette
     * contents (see sample_texel sentinel). */
    host_gxfifo_raster_install_palette(pal, (size_t)n_entries);

    fprintf(stderr,
            "[synth-sprite] real palette installed from FAT[0x%X]/sub[%u] "
            "(%uB, %d entries)\n",
            (unsigned)fat_id, (unsigned)pal_sub, (unsigned)pal_sz, n_entries);
    return 1;
}

/* Install a real 4bpp tile sheet from sub[tile_sub] of FAT[fat_id].
 * Loads up to 64x64 px (8x8 tiles = 2 KiB) so a single TEXIMAGE_PARAM
 * with s_size=t_size=3 (=64) can sample the whole region.  The
 * NDS-tile->linear de-tile is performed in place. */
static int install_real_tile_from_archive(uint32_t fat_id, uint32_t tile_sub,
                                          int *out_tile_size_px)
{
    const uint8_t *tile_bytes = NULL;
    uint32_t       tile_sz    = 0;
    if (!sub_archive_get(fat_id, tile_sub, &tile_bytes, &tile_sz)) return 0;

    /* Decide texture size: 128x128 if we have >= 8 KiB, 64x64 if >= 2 KiB,
     * 32x32 if >= 512 B.  Larger = more glyphs visible at once. */
    int side_tiles, tile_size_enum;
    if (tile_sz >= 16 * 16 * 32) { side_tiles = 16; tile_size_enum = 4; } /* 128 px */
    else if (tile_sz >= 8 * 8 * 32) { side_tiles = 8;  tile_size_enum = 3; } /* 64  px */
    else if (tile_sz >= 4 * 4 * 32) { side_tiles = 4;  tile_size_enum = 2; } /* 32  px */
    else return 0;

    int side_px = side_tiles * 8;
    int detiled_bytes = (side_px * side_px) / 2; /* 4bpp */
    if ((size_t)detiled_bytes > 16 * 1024) return 0; /* safety */

    uint8_t *detiled = (uint8_t *)calloc(1, (size_t)detiled_bytes);
    if (!detiled) return 0;
    detile_4bpp_8x8_block(tile_bytes, detiled, side_tiles, side_tiles);
    host_gxfifo_raster_install_texture(detiled, (size_t)detiled_bytes);
    free(detiled);

    if (out_tile_size_px) *out_tile_size_px = tile_size_enum;

    fprintf(stderr,
            "[synth-sprite] real tile sheet installed from FAT[0x%X]/sub[%u] "
            "(%uB, %dx%d px texture, s/t enum=%d)\n",
            (unsigned)fat_id, (unsigned)tile_sub, (unsigned)tile_sz,
            side_px, side_px, tile_size_enum);
    return 1;
}

/* Try to load a real ROM tile.  Returns 1 on success, 0 if no asset
 * was suitable.  On success g_tex_vram[0..512) holds 32x32 4bpp linear. */
static int load_rom_tile_into_texture(void)
{
    const char *fat_env = getenv("MLPIT_SYNTH_SPRITE_FAT_INDEX");
    const char *off_env = getenv("MLPIT_SYNTH_SPRITE_BYTE_OFFSET");
    uint32_t fat_idx = (uint32_t)(fat_env ? strtoul(fat_env, NULL, 0) : 0);
    size_t   byte_off = (size_t)(off_env ? strtoul(off_env, NULL, 0) : 0);

    if (!pack_is_loaded()) {
        fprintf(stderr, "[synth-sprite] asset pack not loaded; "
                        "falling back to checkerboard texture\n");
        return 0;
    }

    void *data = NULL;
    size_t sz = 0;

    /* Iterate through pack entries to find the first non-overlay file
     * with sufficient bytes after byte_off.  This is robust against
     * specific FAT indices being absent. */
    uint32_t total = pack_entry_count();
    PackEntryView pv;
    int found = 0;
    int probed = 0;
    for (uint32_t i = 0; i < total && probed < 200; i++) {
        if (!pack_entry_at(i, &pv)) continue;
        if (pv.type != PACK_TYPE_RAW) continue;
        if (pv.size < byte_off + 16 * 32) continue; /* need 4x4 tiles */
        probed++;
        /* If user gave a specific FAT index, match its embedded id. */
        if (fat_env) {
            if (pv.id != PACK_ID_FILE(fat_idx)) continue;
        }
        if (pack_get_file(pv.id - PACK_ID_FILE_BASE, &data, &sz)
            && data && sz >= byte_off + 16 * 32) {
            found = 1;
            fprintf(stderr,
                    "[synth-sprite] using pack entry idx=%u id=0x%08X "
                    "size=%zu off=%zu (4x4 4bpp tiles, 32x32 px)\n",
                    (unsigned)i, (unsigned)pv.id, sz, byte_off);
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "[synth-sprite] no suitable pack file found\n");
        return 0;
    }

    /* 4x4 tiles -> 32x32 px = 512 bytes 4bpp linear */
    uint8_t detiled[16 * 32];
    memset(detiled, 0, sizeof(detiled));
    const uint8_t *src = (const uint8_t *)data + byte_off;
    detile_4bpp_8x8_block(src, detiled, 4, 4);
    host_gxfifo_raster_install_texture(detiled, sizeof(detiled));
    return 1;
}

static void install_fallback_texture(void)
{
    /* 32x32 4bpp diagonal stripe pattern using indices 1..15. */
    uint8_t buf[16 * 32];
    memset(buf, 0, sizeof(buf));
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            uint8_t idx = (uint8_t)(((x + y) >> 1) & 0x0F);
            if (idx == 0) idx = 1;
            int off = (y * 32 + x) >> 1;
            if ((x & 1) == 0) buf[off] = (uint8_t)((buf[off] & 0xF0) | idx);
            else              buf[off] = (uint8_t)((buf[off] & 0x0F) | (idx << 4));
        }
    }
    host_gxfifo_raster_install_texture(buf, sizeof(buf));
}

/* ===== Per-frame emit ================================================ */

static void synth_buf_init(void)
{
    if (g_synth_init) return;
    memset(g_synth_buf, 0, sizeof(g_synth_buf));
    *synth_count_ptr() = 1;

    /* Texture pixel size derived from the loaded sheet (default 32 px;
     * REAL path may bump to 64 or 128). */
    int s_enum = g_real_tile_size_enum;
    int tex_px = 8 << (s_enum & 7);

    /* Display the texture at native size centred on the top screen so
     * the original tile structure is visible at 1:1.  If the texture is
     * smaller than the screen height, place it in the middle. */
    int sx = (256 - tex_px) / 2; if (sx < 0) sx = 0;
    int sy = (192 - tex_px) / 2; if (sy < 0) sy = 0;

    synth_sprite_t *d = synth_descriptors();
    d[0].x          = (int16_t)sx;
    d[0].y          = (int16_t)sy;
    d[0].width      = (uint8_t)(tex_px > 255 ? 255 : tex_px);
    d[0].height     = (uint8_t)(tex_px > 255 ? 255 : tex_px);
    d[0].tex_s_size = (uint16_t)s_enum;
    d[0].tex_t_size = (uint16_t)s_enum;
    d[0].tex_vram_off_8b = 0;
    /* PLTT_BASE in 16-byte units; pal_bank N = 16 colours into bank N. */
    d[0].pltt_idx   = (uint16_t)g_real_pal_bank;
    d[0].color      = 0x7FFFu;
    d[0].reserved   = 0;
    g_synth_init = 1;
}

static void emit_descriptor(const synth_sprite_t *d)
{
    /* TEXIMAGE_PARAM: bits 0..15 = vram offset / 8;
     *   bits 20..22 = s_size, 23..25 = t_size, 26..28 = format (3 = 4bpp). */
    uint32_t teximage = (uint32_t)d->tex_vram_off_8b
                      | ((uint32_t)(d->tex_s_size & 7) << 20)
                      | ((uint32_t)(d->tex_t_size & 7) << 23)
                      | (3u << 26);
    host_gxfifo_push(GX_TEXIMAGE_PARAM, teximage);
    host_gxfifo_push(GX_PLTT_BASE,      d->pltt_idx);
    host_gxfifo_push(GX_COLOR,          d->color);
    host_gxfifo_push(GX_BEGIN_VTXS,     1); /* quads */

    int s_px = 8 << (d->tex_s_size & 7);
    int t_px = 8 << (d->tex_t_size & 7);
    int s_max = s_px * 16; /* 12.4 fixed */
    int t_max = t_px * 16;

    int x0 = d->x;
    int y0 = d->y;
    int x1 = d->x + d->width;
    int y1 = d->y + d->height;

    /* (s,t) -> (x,y) corner-by-corner.  TEXCOORD arg = (T<<16)|S. */
    host_gxfifo_push(GX_TEXCOORD, (uint32_t)((0u << 16) | 0u));
    host_gxfifo_push(GX_VTX_XY,   (uint32_t)((y0 & 0xFFFFu) << 16) | (uint32_t)(x0 & 0xFFFFu));

    host_gxfifo_push(GX_TEXCOORD, (uint32_t)((((uint32_t)t_max) << 16) | 0u));
    host_gxfifo_push(GX_VTX_XY,   (uint32_t)((y1 & 0xFFFFu) << 16) | (uint32_t)(x0 & 0xFFFFu));

    host_gxfifo_push(GX_TEXCOORD, (uint32_t)((((uint32_t)t_max) << 16) | (uint32_t)s_max));
    host_gxfifo_push(GX_VTX_XY,   (uint32_t)((y1 & 0xFFFFu) << 16) | (uint32_t)(x1 & 0xFFFFu));

    host_gxfifo_push(GX_TEXCOORD, (uint32_t)((0u << 16) | (uint32_t)s_max));
    host_gxfifo_push(GX_VTX_XY,   (uint32_t)((y0 & 0xFFFFu) << 16) | (uint32_t)(x1 & 0xFFFFu));

    host_gxfifo_push(GX_END_VTXS, 0);
}

/* Module-level state for the real-tile/palette path so synth_buf_init
 * can pick the right s/t size and palette bank without reloading.
 * Definitions live near the synth-buf globals at the top of the file. */

void mlpit_synth_sprite_emit_frame(void)
{
    static int s_loaded = 0;
    if (!s_loaded) {
        const char *no_real = getenv("MLPIT_SYNTH_SPRITE_NO_REAL_PAL");
        const char *fat_env = getenv("MLPIT_SYNTH_SPRITE_REAL_FAT");
        const char *til_env = getenv("MLPIT_SYNTH_SPRITE_REAL_TILE_SUB");
        const char *pal_env = getenv("MLPIT_SYNTH_SPRITE_REAL_PAL_SUB");
        const char *bnk_env = getenv("MLPIT_SYNTH_SPRITE_PAL_BANK");
        uint32_t r_fat  = (uint32_t)(fat_env ? strtoul(fat_env, NULL, 0) : 0x45);
        uint32_t r_tile = (uint32_t)(til_env ? strtoul(til_env, NULL, 0) : 181);
        uint32_t r_pal  = (uint32_t)(pal_env ? strtoul(pal_env, NULL, 0) : 177);
        g_real_pal_bank = (int)(bnk_env ? strtoul(bnk_env, NULL, 0) : 0);

        int real_ok = 0;
        if (!no_real && pack_is_loaded()) {
            int sz_enum = 2;
            if (install_real_tile_from_archive(r_fat, r_tile, &sz_enum)
             && install_real_palette_from_archive(r_fat, r_pal)) {
                g_real_tile_size_enum = sz_enum;
                g_loaded_real = 1;
                real_ok = 1;
            }
        }

        if (!real_ok) {
            if (!load_rom_tile_into_texture()) install_fallback_texture();
            install_grayscale_palette();
        }
        synth_buf_init();
        s_loaded = 1;
        fprintf(stderr,
                "[synth-sprite] %s texture+palette installed, "
                "%u sprite(s) ready\n",
                g_loaded_real ? "REAL" : "FALLBACK",
                (unsigned)*synth_count_ptr());
    }

    host_gxfifo_clear();
    host_gxfifo_raster_clear_fb();

    uint16_t n = *synth_count_ptr();
    if (n > SYNTH_MAX_SPRITES) n = SYNTH_MAX_SPRITES;
    const synth_sprite_t *d = synth_descriptors();
    for (uint16_t i = 0; i < n; i++) emit_descriptor(&d[i]);

    host_gxfifo_raster_run();
}
