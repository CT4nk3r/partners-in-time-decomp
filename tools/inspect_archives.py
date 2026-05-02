#!/usr/bin/env python3
"""
inspect_archives.py - Scan mlpit.assets and classify every entry by format.

Outputs a classification table and hexdumps the first 64 bytes of each entry.
Recognises:
  - Standard BIOS compression:   LZ77 (0x10), Huffman (0x20/0x24/0x28/0x2C),
                                  RLE (0x30), Diff8 (0x80/0x81)
  - Nintendo Nitro formats:       NARC, NCLR (palette), NCGR (tiles), NSCR (map),
                                  NANR (animation), NCER (cell)
  - Custom M&L PiT "0x1C" multi-block archive (game-specific, see below)
  - Plain tile data (multi-of-32, high zero-byte ratio)
  - Plain palette (16/256 × 2 bytes, valid BGR555 values)
  - Offset-table archive (ascending u32 offsets, first offset = table size)

Custom "0x1C" archive format (M&L:PiT specific):
  Offset  Size  Field
     0      4   Header size = 0x1C (28 bytes, i.e. 7 u32s), also used as magic
     4      4   Byte offset where first data section ends (= start of section 2)
     8      4   Byte offset of end-of-file (= total file size), repeated 5×
    28      N   Offset table: N u32 offsets, each pointing to a variable-size
                sub-block. N = (first_data_end_offset - 28) / 4.  First entry
                value gives the byte offset of the first sub-block, so the
                number of table entries = (first_sub_block_offset - 28) / 4.
    …      …   Variable-size sub-blocks (may themselves be compressed or
                be further nested offset tables for multi-layer animations).
  first_data_end_offset .. file_end:  second data section (same structure).

Usage:
  python tools/inspect_archives.py              — full report
  python tools/inspect_archives.py --hexdump    — include 64-byte hexdumps
  python tools/inspect_archives.py --id 0x2036  — single entry deep-dive
"""
import struct
import sys
import os

# ─── constants ────────────────────────────────────────────────────────────────

PACK_PATH = os.path.join(os.path.dirname(__file__), "..", "assets", "mlpit.assets")

NITRO_MAGIC = {
    b"NARC": "NARC (archive)",
    b"RGCN": "NCGR (tiles)",
    b"RLCN": "NCLR (palette)",
    b"RSCN": "NSCR (tilemap)",
    b"RNAN": "NANR (animation)",
    b"RCAN": "NCAR (cell-anim)",
    b"RECN": "NCER (cell)",
    b"RTPN": "NTPR (palette)",
}

BIOS_COMP = {
    0x10: "LZ77",
    0x20: "Huffman-4",
    0x24: "Huffman-4",
    0x28: "Huffman-8",
    0x2C: "Huffman-12",
    0x2F: "Huffman-var",
    0x30: "RLE",
    0x80: "Diff8",
    0x81: "Diff16",
}

TYPE_NAMES = {0: "RAW", 1: "OVERLAY", 2: "OVT", 3: "FAT", 4: "FNT"}

# ─── helpers ──────────────────────────────────────────────────────────────────

def hexline(data, offset):
    chunk = data[offset:offset+16]
    hexpart = " ".join(f"{b:02x}" for b in chunk)
    asc = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
    return f"  {offset:04x}: {hexpart:<47}  {asc}"


def classify(payload):
    """Return a short classification string for a blob of bytes."""
    if len(payload) < 4:
        return "tiny"

    magic4 = payload[:4]
    first  = payload[0]

    # Nitro Nintendo formats
    if magic4 in NITRO_MAGIC:
        return NITRO_MAGIC[magic4]

    # BIOS compression: byte[0] = type, bytes[1..3] = decompressed size (LE u24)
    if first in BIOS_COMP:
        decomp = struct.unpack_from("<I", payload, 0)[0] >> 8
        return f"{BIOS_COMP[first]} -> {decomp:,} B decompressed"

    # Custom M&L PiT "0x1C" multi-block archive
    if first == 0x1C and len(payload) >= 28:
        h = struct.unpack_from("<7I", payload, 0)
        if h[0] == 0x1C:
            sec1_end = h[1]
            file_end = h[2]
            # Sanity: last 5 fields should equal file_end
            if all(x == file_end for x in h[2:]):
                # Count offset-table entries
                if sec1_end > 0x1C and sec1_end < file_end:
                    first_sub_off = struct.unpack_from("<I", payload, 0x1C)[0]
                    n = max(0, (first_sub_off - 0x1C) // 4) if first_sub_off > 0x1C else 0
                    return (f"Custom-0x1C archive "
                            f"(~{n} entries, sec1={sec1_end:#x}, total={file_end:#x})")
            return "Custom-0x1C archive (malformed header)"

    # Offset-table archive: ascending u32 values where first entry = table size
    if len(payload) >= 8:
        v0 = struct.unpack_from("<I", payload, 0)[0]
        if 8 <= v0 <= len(payload) and v0 % 4 == 0:
            n = v0 // 4
            if n >= 2 and len(payload) >= n * 4:
                offsets = struct.unpack_from(f"<{n}I", payload, 0)
                if all(offsets[i] <= offsets[i+1] for i in range(n-1)):
                    return (f"Offset-table archive ({n} entries, "
                            f"data starts at {offsets[0]:#x})")

    # Heuristic: palette
    if 32 <= len(payload) <= 512 and len(payload) % 2 == 0:
        u16s = struct.unpack_from(f"<{len(payload)//2}H", payload)
        if sum(1 for v in u16s if v < 0x8000) / len(u16s) > 0.9:
            return f"Palette ({len(payload)//2} colours, BGR555)"

    # Heuristic: 4bpp tile data
    if len(payload) >= 512 and len(payload) % 32 == 0:
        sample = payload[:min(512, len(payload))]
        zero_ratio = sample.count(0) / len(sample)
        if 0.05 < zero_ratio < 0.9:
            n_tiles = len(payload) // 32
            return f"4bpp tiles? ({n_tiles} tiles, zero_ratio={zero_ratio:.2f})"

    return "unknown"


def decode_custom_1c(payload):
    """Return a short structural description for a Custom-0x1C archive."""
    if len(payload) < 28:
        return []
    h = struct.unpack_from("<7I", payload, 0)
    sec1_end = h[1]
    first_sub = struct.unpack_from("<I", payload, 0x1C)[0] if len(payload) > 0x20 else 0
    n = (first_sub - 0x1C) // 4 if first_sub > 0x1C else 0
    lines = [
        f"    Header (28 bytes):  sec1_end=0x{sec1_end:x}  file_end=0x{h[2]:x}",
        f"    Offset table @ 0x1C: {n} entries",
    ]
    if n > 0 and len(payload) >= 0x1C + n * 4:
        offs = struct.unpack_from(f"<{min(n,8)}I", payload, 0x1C)
        lines.append(f"    First {min(n,8)} offsets: {[hex(o) for o in offs]}")
        # Show sub-block sizes
        all_offs = list(struct.unpack_from(f"<{n}I", payload, 0x1C)) + [sec1_end]
        sizes = [all_offs[i+1] - all_offs[i] for i in range(min(n, 8))]
        lines.append(f"    Sub-block sizes: {sizes}")
        # Peek at first sub-block
        if offs[0] < len(payload):
            sub0 = payload[offs[0]:offs[0]+4]
            sub0_class = classify(sub0 + b"\x00" * 4)
            lines.append(f"    Sub-block[0] @ 0x{offs[0]:x}: {sub0.hex()} ({sub0_class})")
    return lines


# ─── main ─────────────────────────────────────────────────────────────────────

def main():
    want_hexdump = "--hexdump" in sys.argv
    single_id    = None
    for a in sys.argv[1:]:
        if a.startswith("--id"):
            try:
                idx = sys.argv.index(a)
                single_id = int(sys.argv[idx+1], 0)
            except (ValueError, IndexError):
                pass

    if not os.path.exists(PACK_PATH):
        print(f"ERROR: pack not found at {PACK_PATH}", file=sys.stderr)
        sys.exit(1)

    with open(PACK_PATH, "rb") as f:
        data = f.read()

    magic   = data[:8]
    version, ec, sha = struct.unpack_from("<III", data, 8)
    print(f"Pack magic : {magic!r}")
    print(f"Version    : {version}   Entries: {ec}   SHA prefix: {sha:#010x}")
    print(f"Pack size  : {len(data)//1024} KB ({len(data):,} bytes)")
    print()

    entries = []
    for i in range(ec):
        off = 32 + i * 16
        eid, eoff, esz, etype = struct.unpack_from("<IIII", data, off)
        entries.append((eid, eoff, esz, etype))

    if single_id is not None:
        found = [(eid,eoff,esz,et) for (eid,eoff,esz,et) in entries if eid == single_id]
        if not found:
            print(f"ID {single_id:#010x} not found.")
            sys.exit(1)
        entries = found
        want_hexdump = True

    # ── summary table ─────────────────────────────────────────────────────────

    fmt = f"  {'ID':>10}  {'Type':>8}  {'Offset':>10}  {'Size':>9}  Classification"
    print(fmt)
    print("  " + "-" * (len(fmt) - 2))

    stats = {}
    lz77_entries      = []
    huff_entries      = []
    rle_entries        = []
    custom1c_entries  = []
    tile_entries       = []
    palette_entries    = []

    for eid, eoff, esz, etype in sorted(entries, key=lambda x: x[0]):
        payload = data[eoff:eoff+esz] if esz > 0 else b""
        cls     = classify(payload)
        tname   = TYPE_NAMES.get(etype, f"T{etype}")

        print(f"  {eid:#010x}  {tname:>8}  {eoff:>10}  {esz:>9,}  {cls}")

        # ── deep-dive for Custom-0x1C ──────────────────────────────────────
        if "Custom-0x1C" in cls:
            if want_hexdump or single_id is not None:
                for line in decode_custom_1c(payload):
                    print(line)
            custom1c_entries.append((eid, esz))

        # ── hexdump ────────────────────────────────────────────────────────
        if want_hexdump and len(payload) > 0:
            n = min(64, len(payload))
            for off2 in range(0, n, 16):
                print(hexline(payload, off2))
            print()

        key = cls.split("(")[0].strip().split("->")[0].strip()
        stats[key] = stats.get(key, 0) + 1

        if cls.startswith("LZ77"):           lz77_entries.append((eid, esz))
        elif cls.startswith("Huffman"):      huff_entries.append((eid, esz))
        elif cls.startswith("RLE"):          rle_entries.append((eid, esz))
        elif cls.startswith("Custom-0x1C"):  custom1c_entries  # already added
        elif "tiles?" in cls.lower():        tile_entries.append((eid, esz))
        elif "Palette" in cls:               palette_entries.append((eid, esz))

    # ── statistics ────────────────────────────────────────────────────────────
    print()
    print("=== FORMAT STATISTICS ===")
    for k, v in sorted(stats.items(), key=lambda x: -x[1]):
        print(f"  {k:<40}  {v:4d} entries")

    print()
    print("=== BIOS-COMPRESSED ENTRIES ===")
    print(f"  LZ77     : {len(lz77_entries):3d}  {[hex(e) for e in [x[0] for x in lz77_entries[:8]]]}")
    print(f"  Huffman  : {len(huff_entries):3d}  {[hex(e) for e in [x[0] for x in huff_entries[:8]]]}")
    print(f"  RLE      : {len(rle_entries):3d}  {[hex(e) for e in [x[0] for x in rle_entries[:8]]]}")

    print()
    print("=== TILE / PALETTE CANDIDATES (uncompressed) ===")
    print(f"  Tiles    : {len(tile_entries):3d}  {[hex(e) for e in [x[0] for x in tile_entries[:8]]]}")
    print(f"  Palettes : {len(palette_entries):3d}  {[hex(e) for e in [x[0] for x in palette_entries[:8]]]}")

    # ── largest LZ77 file - show decompressed size ─────────────────────────
    if lz77_entries:
        biggest = max(lz77_entries, key=lambda x: x[1])
        eid, esz = biggest
        row = next((r for r in entries if r[0] == eid), None)
        if row:
            _, eoff, _, _ = row
            magic_word = struct.unpack_from("<I", data, eoff)[0]
            decomp = magic_word >> 8
            print()
            print(f"  Largest LZ77: {eid:#010x} compressed={esz:,} B, "
                  f"decompressed~{decomp:,} B")
            print(f"  -> After LZ77 decompression this is the biggest tile bank "
                  f"({decomp//32:,} 4bpp tiles)")


if __name__ == "__main__":
    main()
