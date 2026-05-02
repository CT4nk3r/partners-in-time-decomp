#!/usr/bin/env python3
"""
inspect_assets.py - Inspect the MLPIT asset pack contents.
Lists all entries sorted by size and flags candidates for tile data,
palettes, and map data.
"""
import struct
import sys
import os

PACK_PATH = os.path.join(os.path.dirname(__file__), "..", "assets", "mlpit.assets")

with open(PACK_PATH, "rb") as f:
    data = f.read()

magic = data[:8]
version, entry_count, sha_prefix = struct.unpack_from("<III", data, 8)
print(f"Magic: {magic!r}")
print(f"Version: {version}, Entries: {entry_count}, SHA prefix: {sha_prefix:#010x}")
print(f"Total pack size: {len(data)//1024} KB\n")

entries = []
for i in range(entry_count):
    off = 32 + i * 16
    eid, eoff, esz, etype = struct.unpack_from("<IIII", data, off)
    entries.append((eid, eoff, esz, etype))

type_names = {0: "RAW", 1: "OVERLAY", 2: "OVT", 3: "FAT", 4: "FNT"}

print(f"{'ID':>10} {'Type':>8} {'Offset':>10} {'Size':>8}  Notes")
print("-"*70)

palette_cands = []
tile_cands = []
map_cands = []

for (eid, eoff, esz, etype) in sorted(entries, key=lambda x: x[2]):
    tname = type_names.get(etype, f"T{etype}")
    payload = data[eoff:eoff+esz]
    notes = ""

    # Palette heuristic: 32..512 bytes, even size, mostly valid BGR555
    if 32 <= esz <= 512 and esz % 2 == 0:
        u16s = struct.unpack_from(f"<{esz//2}H", payload)
        valid = sum(1 for v in u16s if v < 0x8000)
        if valid / len(u16s) > 0.85:
            notes += " [PALETTE?]"
            palette_cands.append((eid, esz))

    # Tile data heuristic: multiple of 32, >= 512 bytes, lots of 0x00
    if esz >= 512 and esz % 32 == 0 and etype == 0:
        freq = [0]*256
        sample = payload[:min(512, esz)]
        for b in sample:
            freq[b] += 1
        zero_ratio = freq[0] / len(sample)
        if zero_ratio > 0.08:
            notes += f" [TILES? zero={zero_ratio:.2f}]"
            tile_cands.append((eid, esz))

    # Map data heuristic: 2KB-8KB, tile indices mostly <1024
    if 2048 <= esz <= 8192 and esz % 2 == 0 and etype == 0:
        n = min(128, esz//2)
        u16s = struct.unpack_from(f"<{n}H", payload)
        tile_ratio = sum(1 for v in u16s if v < 1024) / n
        if tile_ratio > 0.8:
            notes += f" [MAP? tile_ratio={tile_ratio:.2f}]"
            map_cands.append((eid, esz))

    print(f"  {eid:#010x} {tname:>8} {eoff:>10} {esz:>8}{notes}")

print()
print("=== CANDIDATES ===")
print(f"Palette candidates ({len(palette_cands)}): {palette_cands[:15]}")
print(f"Tile candidates ({len(tile_cands)}): {tile_cands[:15]}")
print(f"Map candidates ({len(map_cands)}): {map_cands[:15]}")

# Print hex dump of first palette candidate
if palette_cands:
    best_pal = palette_cands[0]
    eid, esz = best_pal
    idx = next(i for i,(e,o,s,t) in enumerate(entries) if e == eid)
    eoff = entries[idx][1]
    payload = data[eoff:eoff+esz]
    print(f"\nHex dump of palette candidate {eid:#010x} ({esz} bytes):")
    for i in range(0, min(64, esz), 16):
        chunk = payload[i:i+16]
        hexs = " ".join(f"{b:02x}" for b in chunk)
        # interpret as BGR555
        vals = []
        for j in range(0, len(chunk), 2):
            v = chunk[j] | (chunk[j+1] << 8)
            r = (v & 0x1f) << 3
            g = ((v >> 5) & 0x1f) << 3
            b2 = ((v >> 10) & 0x1f) << 3
            vals.append(f"RGB({r},{g},{b2})")
        print(f"  {i:04x}: {hexs}  {' '.join(vals)}")

# Print hex dump of first tile candidate
if tile_cands:
    eid, esz = tile_cands[0]
    idx = next(i for i,(e,o,s,t) in enumerate(entries) if e == eid)
    eoff = entries[idx][1]
    payload = data[eoff:eoff+esz]
    print(f"\nHex dump first 64 bytes of tile candidate {eid:#010x} ({esz} bytes):")
    for i in range(0, min(64, esz), 16):
        chunk = payload[i:i+16]
        hexs = " ".join(f"{b:02x}" for b in chunk)
        print(f"  {i:04x}: {hexs}")
