/*
 * mlpit_pack.h — MLPIT asset pack format definition
 *
 * The asset pack is generated ONCE from the user's NDS ROM by either
 * tools/extract_assets.py or pc/src/asset_extractor.c.  After extraction
 * the ROM is no longer needed at runtime.
 *
 * File layout:
 *   [32-byte MlpitPackHeader]
 *   [entry_count × 16-byte MlpitPackEntry table]
 *   [4-byte-aligned file payloads]
 *
 * All integers are little-endian.  All payload offsets are absolute file
 * offsets and are always 4-byte aligned.
 */
#ifndef MLPIT_PACK_H
#define MLPIT_PACK_H

#include <stdint.h>

/* ── Magic / version ─────────────────────────────────────────── */
#define MLPIT_PACK_MAGIC    "MLPIT\0\0\0"   /* 8 bytes, last 3 are NUL */
#define MLPIT_PACK_VERSION  1u

/* ── Header (32 bytes) ───────────────────────────────────────── */
typedef struct {
    uint8_t  magic[8];          /* "MLPIT\0\0\0"                */
    uint32_t version;           /* = MLPIT_PACK_VERSION         */
    uint32_t entry_count;       /* number of entries that follow */
    uint32_t sha256_prefix;     /* first 4 ROM bytes as stamp   */
    uint32_t reserved[3];       /* pad to 32 bytes, must be 0   */
} MlpitPackHeader;              /* sizeof == 32                  */

/* ── Entry (16 bytes each) ───────────────────────────────────── */
typedef struct {
    uint32_t id;                /* asset ID — see PACK_ID_* below */
    uint32_t offset;            /* absolute offset in pack file   */
    uint32_t size;              /* payload size in bytes          */
    uint32_t type;              /* PACK_TYPE_*                    */
} MlpitPackEntry;               /* sizeof == 16                   */

/* ── Asset types ─────────────────────────────────────────────── */
#define PACK_TYPE_RAW       0u  /* generic data file              */
#define PACK_TYPE_OVERLAY   1u  /* ARM9 overlay binary            */
#define PACK_TYPE_OVT       2u  /* ARM9 overlay table (NDS fmt)   */
#define PACK_TYPE_FAT       3u  /* NDS File Allocation Table      */
#define PACK_TYPE_FNT       4u  /* NDS Filename Table             */

/* ── Well-known asset IDs ────────────────────────────────────── */
#define PACK_ID_FAT             0x00000001u  /* FAT table                  */
#define PACK_ID_FNT             0x00000002u  /* FNT table                  */
#define PACK_ID_OVT             0x00000003u  /* ARM9 overlay table         */
#define PACK_ID_OVERLAY_BASE    0x00001000u  /* overlay logical_id 0..N    */
#define PACK_ID_FILE_BASE       0x00002000u  /* FAT index 0..M (non-ovl)   */

/* Compute ID for a given overlay or data file */
#define PACK_ID_OVERLAY(n)  (PACK_ID_OVERLAY_BASE | (uint32_t)(n))
#define PACK_ID_FILE(n)     (PACK_ID_FILE_BASE    | (uint32_t)(n))

#endif /* MLPIT_PACK_H */
