/*
 * asset_pack.c — runtime MLPIT asset pack loader
 *
 * Loads assets/mlpit.assets (written by extract_assets), validates the
 * header, parses the entry table, and builds an open-addressing hash table
 * for O(1) lookup by asset ID.
 *
 * Memory strategy:
 *   Windows  — CreateFileMapping / MapViewOfFile (zero-copy mmap)
 *   Elsewhere — malloc + fread (avoids platform mmap complexity for now)
 */
#include "asset_pack.h"
#include "mlpit_pack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

/* ── Internal types ──────────────────────────────────────────── */

typedef struct {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    uint32_t type;
} PackEntry;

typedef struct {
    uint32_t id;
    uint32_t index;  /* index into g_entries[] */
    int      used;
} HashSlot;

/* ── Module state ────────────────────────────────────────────── */

static uint8_t   *g_data    = NULL;
static size_t     g_data_sz = 0;
static int        g_loaded  = 0;
static uint32_t   g_ec      = 0;
static PackEntry *g_entries = NULL;
static HashSlot  *g_htable  = NULL;
static uint32_t   g_hsize   = 0;   /* power of 2 */

#ifdef _WIN32
static HANDLE g_fh = INVALID_HANDLE_VALUE;
static HANDLE g_mh = NULL;
#endif

/* ── Helpers ─────────────────────────────────────────────────── */

static uint32_t r32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint32_t next_pow2(uint32_t n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16;
    return n + 1;
}

static void htable_insert(uint32_t id, uint32_t index) {
    uint32_t mask = g_hsize - 1;
    uint32_t slot = id & mask;
    while (g_htable[slot].used)
        slot = (slot + 1) & mask;
    g_htable[slot].id    = id;
    g_htable[slot].index = index;
    g_htable[slot].used  = 1;
}

static int htable_find(uint32_t id, uint32_t *out_index) {
    if (!g_htable) return 0;
    uint32_t mask   = g_hsize - 1;
    uint32_t slot   = id & mask;
    uint32_t probes = 0;
    while (g_htable[slot].used) {
        if (g_htable[slot].id == id) {
            *out_index = g_htable[slot].index;
            return 1;
        }
        slot = (slot + 1) & mask;
        if (++probes >= g_hsize) break;
    }
    return 0;
}

/* ── Public API ──────────────────────────────────────────────── */

int pack_load(const char *path) {
    if (g_loaded) pack_unload();

#ifdef _WIN32
    g_fh = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_fh == INVALID_HANDLE_VALUE) return 0;

    LARGE_INTEGER fsz;
    if (!GetFileSizeEx(g_fh, &fsz)) {
        CloseHandle(g_fh); g_fh = INVALID_HANDLE_VALUE; return 0;
    }
    g_data_sz = (size_t)fsz.QuadPart;

    g_mh = CreateFileMappingA(g_fh, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!g_mh) {
        CloseHandle(g_fh); g_fh = INVALID_HANDLE_VALUE; return 0;
    }
    g_data = (uint8_t *)MapViewOfFile(g_mh, FILE_MAP_READ, 0, 0, 0);
    if (!g_data) {
        CloseHandle(g_mh); g_mh = NULL;
        CloseHandle(g_fh); g_fh = INVALID_HANDLE_VALUE;
        return 0;
    }
#else
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) { fclose(f); return 0; }
    g_data_sz = (size_t)sz;
    g_data = (uint8_t *)malloc(g_data_sz);
    if (!g_data) { fclose(f); return 0; }
    if (fread(g_data, 1, g_data_sz, f) != g_data_sz) {
        free(g_data); g_data = NULL; fclose(f); return 0;
    }
    fclose(f);
#endif

    /* Validate header */
    if (g_data_sz < 32) goto fail;
    if (memcmp(g_data, MLPIT_PACK_MAGIC, 8) != 0) {
        fprintf(stderr, "[pack] Bad magic in %s\n", path);
        goto fail;
    }
    {
        uint32_t ver = r32(g_data + 8);
        if (ver != MLPIT_PACK_VERSION) {
            fprintf(stderr, "[pack] Version mismatch: got %u, expected %u\n",
                    ver, MLPIT_PACK_VERSION);
            goto fail;
        }
    }
    g_ec = r32(g_data + 12);
    if (32 + (uint64_t)g_ec * 16 > g_data_sz) goto fail;

    /* Parse entry table */
    g_entries = (PackEntry *)malloc(g_ec * sizeof(PackEntry));
    if (!g_entries) goto fail;
    for (uint32_t i = 0; i < g_ec; i++) {
        const uint8_t *e = g_data + 32 + i * 16;
        g_entries[i].id     = r32(e + 0);
        g_entries[i].offset = r32(e + 4);
        g_entries[i].size   = r32(e + 8);
        g_entries[i].type   = r32(e + 12);
    }

    /* Build open-addressing hash table (load factor ≤ 0.5) */
    g_hsize  = next_pow2(g_ec * 2 + 1);
    g_htable = (HashSlot *)calloc(g_hsize, sizeof(HashSlot));
    if (!g_htable) goto fail;
    for (uint32_t i = 0; i < g_ec; i++)
        htable_insert(g_entries[i].id, i);

    g_loaded = 1;
    fprintf(stdout, "[pack] Loaded %u entries from %s\n", g_ec, path);
    return 1;

fail:
    pack_unload();
    return 0;
}

const void *pack_get(uint32_t id, size_t *out_size) {
    if (!g_loaded) return NULL;
    uint32_t idx;
    if (!htable_find(id, &idx)) return NULL;
    uint32_t off  = g_entries[idx].offset;
    uint32_t size = g_entries[idx].size;
    if ((uint64_t)off + size > g_data_sz) return NULL;
    if (out_size) *out_size = (size_t)size;
    return g_data + off;
}

int pack_get_overlay(uint32_t overlay_id, void **data, size_t *size) {
    size_t sz = 0;
    const void *p = pack_get(PACK_ID_OVERLAY(overlay_id), &sz);
    if (!p) return 0;
    if (data) *data = (void *)p;
    if (size) *size = sz;
    return 1;
}

int pack_get_file(uint32_t fat_index, void **data, size_t *size) {
    size_t sz = 0;
    const void *p = pack_get(PACK_ID_FILE(fat_index), &sz);
    if (!p) return 0;
    if (data) *data = (void *)p;
    if (size) *size = sz;
    return 1;
}

void pack_unload(void) {
    if (g_entries) { free(g_entries); g_entries = NULL; }
    if (g_htable)  { free(g_htable);  g_htable  = NULL; }
    g_ec = 0; g_hsize = 0;

#ifdef _WIN32
    if (g_data && g_data != (uint8_t *)(uintptr_t)-1)
        UnmapViewOfFile(g_data);
    if (g_mh != NULL)                  { CloseHandle(g_mh); g_mh = NULL; }
    if (g_fh != INVALID_HANDLE_VALUE)  { CloseHandle(g_fh); g_fh = INVALID_HANDLE_VALUE; }
#else
    if (g_data) free(g_data);
#endif
    g_data    = NULL;
    g_data_sz = 0;
    g_loaded  = 0;
}

bool pack_is_loaded(void) { return g_loaded != 0; }

uint32_t pack_entry_count(void) { return g_loaded ? g_ec : 0; }

int pack_entry_at(uint32_t i, PackEntryView *out) {
    if (!g_loaded || i >= g_ec || !out) return 0;
    out->id     = g_entries[i].id;
    out->offset = g_entries[i].offset;
    out->size   = g_entries[i].size;
    out->type   = g_entries[i].type;
    return 1;
}

const void *pack_data_base(size_t *total_size) {
    if (total_size) *total_size = g_data_sz;
    return g_data;
}
