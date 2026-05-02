/*
 * asset_extractor.h — one-time NDS ROM → MLPIT asset pack (C implementation)
 *
 * Invoked from main() on first launch when no pack exists but a ROM is
 * present.  Reads the NDS header, walks the FAT/FNT/OVT tables, and
 * writes every file into a single assets/mlpit.assets pack.
 *
 * Python equivalent: tools/extract_assets.py
 */
#ifndef ASSET_EXTRACTOR_H
#define ASSET_EXTRACTOR_H

/*
 * Extract all assets from rom_path and write an MLPIT pack to pack_path.
 * Creates the output directory if it does not exist.
 * Returns 1 on success, 0 on any failure (ROM absent, I/O error, etc.).
 */
int extract_assets(const char *rom_path, const char *pack_path);

#endif /* ASSET_EXTRACTOR_H */
