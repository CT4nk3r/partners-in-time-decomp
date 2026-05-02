/*
 * asset_pack.h — runtime MLPIT asset pack loader
 *
 * After the one-time extraction step, all game assets live in
 * assets/mlpit.assets.  This module memory-maps the pack (Win32) or
 * reads it into heap memory, validates the header, and provides O(1)
 * lookup via an open-addressing hash table.
 *
 * Typical boot sequence:
 *   if (!pack_load("assets/mlpit.assets")) {
 *       extract_assets("roms/baserom.nds", "assets/mlpit.assets");
 *       pack_load("assets/mlpit.assets");
 *   }
 */
#ifndef ASSET_PACK_H
#define ASSET_PACK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Load and memory-map the pack file.
 * Returns 1 on success, 0 on failure (file missing, bad magic, etc.).
 */
int  pack_load(const char *path);

/*
 * Look up an asset by its PACK_ID_* identifier.
 * Returns a pointer into the mapped data — never free() it.
 * Sets *out_size if non-NULL.  Returns NULL when not found.
 */
const void *pack_get(uint32_t id, size_t *out_size);

/*
 * Convenience wrappers using PACK_ID_OVERLAY / PACK_ID_FILE encoding.
 * Set *data and *size on success.  Return 1 on success, 0 when absent.
 */
int  pack_get_overlay(uint32_t overlay_id, void **data, size_t *size);
int  pack_get_file(uint32_t fat_index,     void **data, size_t *size);

/* Release all resources and unmap the file. */
void pack_unload(void);

/* Returns true if a pack is currently loaded and ready. */
bool pack_is_loaded(void);

/* === Iteration (used by the asset viewer) ====================== */
typedef struct {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    uint32_t type;
} PackEntryView;

uint32_t pack_entry_count(void);
int      pack_entry_at(uint32_t i, PackEntryView *out);
const void *pack_data_base(size_t *total_size);

#endif /* ASSET_PACK_H */
