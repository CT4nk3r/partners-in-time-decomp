#!/usr/bin/env python3
"""
extract_assets.py — one-time NDS ROM → MLPIT asset pack extractor

Extracts every file from a Mario & Luigi: Partners in Time .nds ROM and
writes them into a single assets/mlpit.assets pack that the PC port loads
at runtime.  After extraction the ROM is no longer needed.

Usage:
    python tools/extract_assets.py [rom_path] [pack_path]

Defaults:
    rom_path  = roms/baserom.nds
    pack_path = assets/mlpit.assets

Requirements: Python 3.8+, stdlib only (struct, hashlib, configparser).
"""

import sys
import os
import struct
import hashlib
import configparser

# ── Pack format constants (must match pc/include/mlpit_pack.h) ──
PACK_MAGIC          = b'MLPIT\x00\x00\x00'   # 8 bytes
PACK_VERSION        = 1

PACK_ID_FAT         = 0x00000001
PACK_ID_FNT         = 0x00000002
PACK_ID_OVT         = 0x00000003
PACK_ID_OVERLAY_BASE = 0x00001000
PACK_ID_FILE_BASE    = 0x00002000

PACK_TYPE_RAW       = 0
PACK_TYPE_OVERLAY   = 1
PACK_TYPE_OVT       = 2
PACK_TYPE_FAT       = 3
PACK_TYPE_FNT       = 4

# ── NDS header field offsets ──────────────────────────────────
NDS_OFF_FNT_OFF     = 0x40
NDS_OFF_FNT_SZ      = 0x44
NDS_OFF_FAT_OFF     = 0x48
NDS_OFF_FAT_SZ      = 0x4C
NDS_OFF_OVT9_OFF    = 0x50   # ARM9 overlay table
NDS_OFF_OVT9_SZ     = 0x54

OVT_ENTRY_SZ        = 32
OVT_OFF_OVL_ID      = 0      # logical overlay_id within each OVT entry
OVT_OFF_FILE_ID     = 24     # FAT index within each OVT entry


def align4(n: int) -> int:
    return (n + 3) & ~3


def read_u32(data: bytes, offset: int) -> int:
    return struct.unpack_from('<I', data, offset)[0]


def compute_sha256(path: str) -> str:
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        while True:
            chunk = f.read(1 << 20)
            if not chunk:
                break
            h.update(chunk)
    return h.hexdigest()


def validate_rom(rom_path: str, ini_path: str = 'config/versions.ini') -> str:
    """SHA-256 validate ROM against known dumps.  Warns but never fails."""
    print('[extract] Computing SHA-256 (may take a moment)...')
    digest = compute_sha256(rom_path)

    config = configparser.ConfigParser()
    config.read(ini_path)
    known = [
        config.get(s, 'sha256')
        for s in config.sections()
        if config.has_option(s, 'sha256')
    ]

    if digest in known:
        matched = next(
            s for s in config.sections()
            if config.has_option(s, 'sha256') and config.get(s, 'sha256') == digest
        )
        print(f'[extract] ROM verified: {matched} ({digest[:16]}...)')
    else:
        print(
            f'[extract] WARNING: SHA-256 {digest[:16]}... not in known list '
            '— proceeding anyway (any of the 3 known dumps is accepted)'
        )
    return digest


def extract(rom_path: str = 'roms/baserom.nds',
            pack_path: str = 'assets/mlpit.assets') -> bool:
    """Extract all NDS ROM assets and write an MLPIT asset pack."""

    # ── Validate ROM exists ───────────────────────────────────
    if not os.path.isfile(rom_path):
        print(f'[extract] ERROR: ROM not found at {rom_path}', file=sys.stderr)
        print(
            '[extract] Place your legal Mario & Luigi: Partners in Time .nds '
            'at that path.',
            file=sys.stderr,
        )
        return False

    validate_rom(rom_path)

    print(f'[extract] Reading {rom_path}...')
    with open(rom_path, 'rb') as f:
        rom = f.read()

    if len(rom) < 0x200:
        print('[extract] ERROR: ROM too small', file=sys.stderr)
        return False

    # ── Parse NDS header ──────────────────────────────────────
    fnt_off = read_u32(rom, NDS_OFF_FNT_OFF)
    fnt_sz  = read_u32(rom, NDS_OFF_FNT_SZ)
    fat_off = read_u32(rom, NDS_OFF_FAT_OFF)
    fat_sz  = read_u32(rom, NDS_OFF_FAT_SZ)
    ovt_off = read_u32(rom, NDS_OFF_OVT9_OFF)
    ovt_sz  = read_u32(rom, NDS_OFF_OVT9_SZ)

    num_files = fat_sz // 8
    num_ovt   = ovt_sz // OVT_ENTRY_SZ if ovt_sz >= OVT_ENTRY_SZ else 0

    print(f'[extract] FAT: {num_files} files | OVT: {num_ovt} overlay entries')

    # ── Map FAT index → logical overlay_id ───────────────────
    # None means "not an overlay"
    fat_to_ovl: dict[int, int] = {}
    for i in range(num_ovt):
        base    = ovt_off + i * OVT_ENTRY_SZ
        ovl_id  = read_u32(rom, base + OVT_OFF_OVL_ID)
        file_id = read_u32(rom, base + OVT_OFF_FILE_ID)
        if file_id < num_files:
            fat_to_ovl[file_id] = ovl_id

    # ── Collect entries: (id, type, payload_bytes) ───────────
    entries: list[tuple[int, int, bytes]] = []

    # Structural tables
    entries.append((PACK_ID_FAT, PACK_TYPE_FAT, rom[fat_off: fat_off + fat_sz]))
    entries.append((PACK_ID_FNT, PACK_TYPE_FNT, rom[fnt_off: fnt_off + fnt_sz]))
    if ovt_sz > 0:
        entries.append((PACK_ID_OVT, PACK_TYPE_OVT, rom[ovt_off: ovt_off + ovt_sz]))

    # Per-FAT-file entries
    skipped = 0
    for i in range(num_files):
        fe_base = fat_off + i * 8
        start   = read_u32(rom, fe_base)
        end     = read_u32(rom, fe_base + 4)
        if start >= end or end > len(rom):
            skipped += 1
            continue
        payload = rom[start:end]
        if i in fat_to_ovl:
            entries.append((PACK_ID_OVERLAY_BASE | fat_to_ovl[i],
                             PACK_TYPE_OVERLAY, payload))
        else:
            entries.append((PACK_ID_FILE_BASE | i,
                             PACK_TYPE_RAW, payload))

    if skipped:
        print(f'[extract] Skipped {skipped} empty/invalid FAT entries')

    entry_count = len(entries)

    # ── Compute layout ────────────────────────────────────────
    entry_table_bytes = entry_count * 16
    payload_start     = align4(32 + entry_table_bytes)
    offsets: list[int] = []
    cur = payload_start
    for _, _, payload in entries:
        offsets.append(cur)
        cur += align4(len(payload))
    total_size = cur

    # Version stamp = first 4 bytes of ROM
    sha_prefix = struct.unpack_from('<I', rom, 0)[0]

    # ── Write pack ────────────────────────────────────────────
    out_dir = os.path.dirname(pack_path)
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)

    print(f'[extract] Writing {entry_count} entries, '
          f'{total_size / (1024 * 1024):.2f} MB → {pack_path}')

    with open(pack_path, 'wb') as f:
        # Header (32 bytes)
        f.write(PACK_MAGIC)                                     # 8 bytes
        f.write(struct.pack('<II', PACK_VERSION, entry_count))  # 8 bytes
        f.write(struct.pack('<I', sha_prefix))                  # 4 bytes
        f.write(b'\x00' * 12)                                   # 12 bytes reserved

        # Entry table (entry_count × 16 bytes)
        for (eid, etype, payload), off in zip(entries, offsets):
            f.write(struct.pack('<IIII', eid, off, len(payload), etype))

        # Pad to payload_start
        pos = f.tell()
        if pos < payload_start:
            f.write(b'\x00' * (payload_start - pos))

        # Payloads (each 4-byte aligned)
        for (_, _, payload), off in zip(entries, offsets):
            assert f.tell() == off, \
                f'Offset mismatch: expected {off:#x}, got {f.tell():#x}'
            f.write(payload)
            pad = align4(len(payload)) - len(payload)
            if pad:
                f.write(b'\x00' * pad)

    # ── Summary ───────────────────────────────────────────────
    file_entries = [
        e for e in entries
        if e[0] not in (PACK_ID_FAT, PACK_ID_FNT, PACK_ID_OVT)
    ]
    print(
        f'[extract] Extracted {len(file_entries)} files, '
        f'{total_size / (1024 * 1024):.2f} MB pack at {pack_path}'
    )
    print('[extract] Done!  You may now delete roms/baserom.nds.')
    return True


if __name__ == '__main__':
    _rom  = sys.argv[1] if len(sys.argv) > 1 else 'roms/baserom.nds'
    _pack = sys.argv[2] if len(sys.argv) > 2 else 'assets/mlpit.assets'
    sys.exit(0 if extract(_rom, _pack) else 1)
