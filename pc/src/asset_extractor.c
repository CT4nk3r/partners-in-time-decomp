/*
 * asset_extractor.c — one-time NDS ROM → MLPIT asset pack extractor
 *
 * Invoked on first launch when assets/mlpit.assets is absent but
 * roms/baserom.nds is present.  Parses the NDS ROM header, extracts the
 * FAT/FNT/OVT tables and all data files, and writes a single
 * assets/mlpit.assets pack in MLPIT format.
 *
 * Python equivalent: tools/extract_assets.py
 */
#include "asset_extractor.h"
#include "mlpit_pack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <direct.h>
#  define MKDIR_ONE(d)  _mkdir(d)
#else
#  include <sys/stat.h>
#  define MKDIR_ONE(d)  mkdir((d), 0755)
#endif

/* ── NDS header field offsets ────────────────────────────────── */
#define NDS_OFF_FNT_OFF   0x40   /* FNT offset                  */
#define NDS_OFF_FNT_SZ    0x44   /* FNT size                    */
#define NDS_OFF_FAT_OFF   0x48   /* FAT offset                  */
#define NDS_OFF_FAT_SZ    0x4C   /* FAT size                    */
#define NDS_OFF_OVT9_OFF  0x50   /* ARM9 overlay table offset   */
#define NDS_OFF_OVT9_SZ   0x54   /* ARM9 overlay table size     */

/* OVT: 32 bytes/entry; logical overlay_id at byte 0, FAT file_id at 24 */
#define OVT_ENTRY_SZ      32
#define OVT_OFF_OVL_ID    0
#define OVT_OFF_FILE_ID   24

/* ── Helpers ─────────────────────────────────────────────────── */

static uint32_t r32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void w32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;       p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

static uint32_t align4(uint32_t v) { return (v + 3u) & ~3u; }

/* Create the directory component of a file path (single level). */
static void ensure_parent_dir(const char *filepath) {
    char tmp[512];
    strncpy(tmp, filepath, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    char *sep = strrchr(tmp, '/');
    if (!sep) sep = strrchr(tmp, '\\');
    if (sep) { *sep = '\0'; MKDIR_ONE(tmp); }
}

/* Read entire file into a malloc-allocated buffer. */
static uint8_t *slurp(const char *path, size_t *out_sz) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) { fclose(f); return NULL; }
    uint8_t *buf = (uint8_t *)malloc((size_t)sz);
    if (!buf) { fclose(f); return NULL; }
    if (fread(buf, 1, (size_t)sz, f) != (size_t)sz) {
        free(buf); fclose(f); return NULL;
    }
    fclose(f);
    *out_sz = (size_t)sz;
    return buf;
}

/* ── Entry descriptor used while building the pack ──────────── */
typedef struct {
    uint32_t id;
    uint32_t type;
    uint32_t rom_off;
    uint32_t size;
    uint32_t pack_off;  /* absolute offset in the output pack */
} EntryInfo;

/* ── Public API ──────────────────────────────────────────────── */

int extract_assets(const char *rom_path, const char *pack_path) {
    size_t   rom_sz = 0;
    uint8_t *rom    = slurp(rom_path, &rom_sz);
    if (!rom) {
        fprintf(stderr,
            "[extract] ROM not found at %s\n"
            "[extract] Place your legal Mario & Luigi: Partners in Time .nds there.\n",
            rom_path);
        return 0;
    }
    if (rom_sz < 0x200) {
        fprintf(stderr, "[extract] ROM too small (%zu bytes)\n", rom_sz);
        free(rom); return 0;
    }

    /* Parse NDS header fields */
    uint32_t fnt_off = r32(rom + NDS_OFF_FNT_OFF);
    uint32_t fnt_sz  = r32(rom + NDS_OFF_FNT_SZ);
    uint32_t fat_off = r32(rom + NDS_OFF_FAT_OFF);
    uint32_t fat_sz  = r32(rom + NDS_OFF_FAT_SZ);
    uint32_t ovt_off = r32(rom + NDS_OFF_OVT9_OFF);
    uint32_t ovt_sz  = r32(rom + NDS_OFF_OVT9_SZ);

    if (fat_off + fat_sz > (uint32_t)rom_sz ||
        fnt_off + fnt_sz > (uint32_t)rom_sz) {
        fprintf(stderr, "[extract] ROM header fields point outside file\n");
        free(rom); return 0;
    }

    uint32_t num_files = fat_sz / 8;
    uint32_t num_ovt   = (ovt_sz > 0 && ovt_off + ovt_sz <= (uint32_t)rom_sz)
                         ? ovt_sz / OVT_ENTRY_SZ : 0;

    fprintf(stdout, "[extract] %u FAT files, %u overlay table entries\n",
            num_files, num_ovt);

    /* Build fat_index → logical overlay_id map (0xFFFFFFFF = not an overlay) */
    uint32_t *ovl_for_fat = (uint32_t *)malloc(num_files * sizeof(uint32_t));
    if (!ovl_for_fat) { free(rom); return 0; }
    memset(ovl_for_fat, 0xFF, num_files * sizeof(uint32_t));

    for (uint32_t i = 0; i < num_ovt; i++) {
        uint32_t base    = ovt_off + i * OVT_ENTRY_SZ;
        uint32_t ovl_id  = r32(rom + base + OVT_OFF_OVL_ID);
        uint32_t file_id = r32(rom + base + OVT_OFF_FILE_ID);
        if (file_id < num_files)
            ovl_for_fat[file_id] = ovl_id;
    }

    /* Allocate entry list: FAT + FNT + OVT + num_files (worst case) */
    uint32_t  max_ec  = 3 + num_files;
    EntryInfo *entries = (EntryInfo *)calloc(max_ec, sizeof(EntryInfo));
    if (!entries) { free(ovl_for_fat); free(rom); return 0; }

    uint32_t ec = 0;

    /* Well-known structural tables */
    entries[ec++] = (EntryInfo){ PACK_ID_FAT, PACK_TYPE_FAT, fat_off, fat_sz, 0 };
    entries[ec++] = (EntryInfo){ PACK_ID_FNT, PACK_TYPE_FNT, fnt_off, fnt_sz, 0 };
    if (num_ovt > 0)
        entries[ec++] = (EntryInfo){ PACK_ID_OVT, PACK_TYPE_OVT, ovt_off, ovt_sz, 0 };

    /* Per-FAT-entry files: overlays get PACK_ID_OVERLAY, others PACK_ID_FILE */
    for (uint32_t i = 0; i < num_files; i++) {
        const uint8_t *fe    = rom + fat_off + i * 8;
        uint32_t       start = r32(fe);
        uint32_t       end   = r32(fe + 4);
        if (start >= end || end > (uint32_t)rom_sz) continue;  /* empty / invalid */

        uint32_t id, type;
        if (ovl_for_fat[i] != 0xFFFFFFFFu) {
            id   = PACK_ID_OVERLAY(ovl_for_fat[i]);
            type = PACK_TYPE_OVERLAY;
        } else {
            id   = PACK_ID_FILE(i);
            type = PACK_TYPE_RAW;
        }
        entries[ec++] = (EntryInfo){ id, type, start, end - start, 0 };
    }
    free(ovl_for_fat);

    /* Compute absolute pack offsets */
    uint32_t payload_start = align4(32u + ec * 16u);
    uint32_t cur           = payload_start;
    for (uint32_t i = 0; i < ec; i++) {
        entries[i].pack_off = cur;
        cur += align4(entries[i].size);
    }

    /* Create output directory, then open file */
    ensure_parent_dir(pack_path);
    FILE *out = fopen(pack_path, "wb");
    if (!out) {
        fprintf(stderr, "[extract] Cannot create pack at %s\n", pack_path);
        free(entries); free(rom); return 0;
    }

    /* Header (32 bytes) */
    uint8_t hdr[32];
    memset(hdr, 0, sizeof(hdr));
    memcpy(hdr, MLPIT_PACK_MAGIC, 8);
    w32(hdr + 8,  MLPIT_PACK_VERSION);
    w32(hdr + 12, ec);
    w32(hdr + 16, r32(rom));   /* first 4 ROM bytes as version stamp */
    fwrite(hdr, 1, 32, out);

    /* Entry table */
    for (uint32_t i = 0; i < ec; i++) {
        uint8_t e[16];
        w32(e + 0,  entries[i].id);
        w32(e + 4,  entries[i].pack_off);
        w32(e + 8,  entries[i].size);
        w32(e + 12, entries[i].type);
        fwrite(e, 1, 16, out);
    }

    /* Pad up to payload_start */
    {
        long pos = ftell(out);
        while ((uint32_t)pos < payload_start) { fputc(0, out); pos++; }
    }

    /* Write payloads, each 4-byte aligned */
    for (uint32_t i = 0; i < ec; i++) {
        fwrite(rom + entries[i].rom_off, 1, entries[i].size, out);
        uint32_t pad = align4(entries[i].size) - entries[i].size;
        for (uint32_t j = 0; j < pad; j++) fputc(0, out);
    }

    fclose(out);
    free(entries);
    free(rom);

    fprintf(stdout,
        "[extract] Extracted %u entries, %.2f MB → %s\n"
        "[extract] After first run you may delete roms/baserom.nds.\n",
        ec, (double)cur / (1024.0 * 1024.0), pack_path);
    return 1;
}
