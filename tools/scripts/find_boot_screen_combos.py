#!/usr/bin/env python3
"""
find_boot_screen_combos.py — Scan the asset pack for plausible
(tilemap, tile sheet, palette) triples we can feed to the runtime
MLPIT_BOOT_TRIPLE override.

Strategy:
  - Iterate every FAT entry.
  - For entries that look like an offset-table or Custom-0x1C archive,
    walk every sub-block.
  - Classify each sub-block by Nitro magic (RGCN/RCSN/RLCN) OR by
    heuristic shape (32K = 8bpp tile sheet, 4096 B = 64x32 map,
    512 B = 256-colour palette).
  - For each archive that contains at least one of each kind, emit
    every (tile, map, palette) triple where the three sub-indices
    are within `--proximity` of each other (default 8).

Output:
  Default: stdout summary.
  --out <file>: write a one-triple-per-line table; pipe into the
                runtime via MLPIT_BOOT_TRIPLE=fatHex:tile:map:pal.
"""
import os
import struct
import sys

PACK_PATH = os.path.join(os.path.dirname(__file__), "..", "..",
                         "assets", "mlpit.assets")

NCGR_MAGIC = b"RGCN"
NSCR_MAGIC = b"RCSN"
NCLR_MAGIC = b"RLCN"


def read_pack(path):
    with open(path, "rb") as f:
        data = f.read()
    magic = data[:8]
    version, ec, sha = struct.unpack_from("<III", data, 8)
    entries = []
    for i in range(ec):
        off = 32 + i * 16
        eid, eoff, esz, etype = struct.unpack_from("<IIII", data, off)
        entries.append((eid, eoff, esz, etype))
    return data, entries


def parse_offset_table(payload):
    """Return list of (offset, size) sub-blocks for an offset-table archive,
    or None if the bytes don't look like one."""
    if len(payload) < 8:
        return None
    first_off = struct.unpack_from("<I", payload, 0)[0]
    if first_off < 4 or first_off > len(payload) or first_off % 4 != 0:
        return None
    n = first_off // 4
    if n < 2 or n > 8192:
        return None
    if len(payload) < n * 4:
        return None
    offs = list(struct.unpack_from(f"<{n}I", payload, 0))
    if not all(offs[i] <= offs[i + 1] for i in range(n - 1)):
        return None
    if offs[-1] > len(payload):
        return None
    blocks = []
    for i in range(n):
        a = offs[i]
        b = offs[i + 1] if i + 1 < n else len(payload)
        if b < a:
            return None
        blocks.append((a, b - a))
    return blocks


def parse_custom_1c(payload):
    """Custom M&L 0x1C archive — same logic as offset table but with 28-byte header."""
    if len(payload) < 32:
        return None
    if struct.unpack_from("<I", payload, 0)[0] != 0x1C:
        return None
    h = struct.unpack_from("<7I", payload, 0)
    sec1_end, file_end = h[1], h[2]
    if sec1_end <= 0x1C or sec1_end > len(payload):
        return None
    first_sub = struct.unpack_from("<I", payload, 0x1C)[0]
    if first_sub <= 0x1C or first_sub > sec1_end:
        return None
    n = (first_sub - 0x1C) // 4
    if n < 2 or n > 8192:
        return None
    offs = list(struct.unpack_from(f"<{n}I", payload, 0x1C))
    blocks = []
    for i in range(n):
        a = offs[i]
        b = offs[i + 1] if i + 1 < n else sec1_end
        if b < a or b > len(payload):
            return None
        blocks.append((a, b - a))
    return blocks


def looks_like_palette_256(payload):
    if len(payload) != 512:
        return False
    high_clear = sum(1 for i in range(0, 512, 2)
                     if (payload[i + 1] & 0x80) == 0)
    return high_clear >= 240  # nearly all entries have bit15==0


def looks_like_8bpp_tilesheet(payload):
    if len(payload) < 16384 or len(payload) > 65536:
        return False
    if len(payload) % 64 != 0:
        return False
    # Real game tile sheets are not pure noise: avg byte should be < 0xC0
    # and at least some byte values should repeat (not random).
    distinct = len(set(payload[:1024]))
    return distinct < 220


def looks_like_64x32_map(payload):
    if len(payload) != 4096:
        return False
    # tilemap entries are 16-bit; tile# (low 10 bits) typically <= 1023
    # plus we expect at least some repetition (most tiles are 0 or low).
    distinct_tiles = set()
    for i in range(0, 4096, 2):
        e = struct.unpack_from("<H", payload, i)[0] & 0x3FF
        distinct_tiles.add(e)
    # Real maps reference at most ~1024 unique tiles and usually far fewer.
    return 1 <= len(distinct_tiles) <= 1024


def classify_sub(payload):
    if len(payload) < 4:
        return None
    m4 = payload[:4]
    if m4 == NCGR_MAGIC:
        return "NCGR"
    if m4 == NSCR_MAGIC:
        return "NSCR"
    if m4 == NCLR_MAGIC:
        return "NCLR"
    if looks_like_palette_256(payload):
        return "PAL256"
    if looks_like_64x32_map(payload):
        return "MAP64x32"
    if looks_like_8bpp_tilesheet(payload):
        return "TILE8BPP"
    return None


def main():
    out_path = None
    proximity = 16
    args = sys.argv[1:]
    while args:
        a = args.pop(0)
        if a == "--out":
            out_path = args.pop(0)
        elif a == "--proximity":
            proximity = int(args.pop(0))
        elif a in ("-h", "--help"):
            print(__doc__); sys.exit(0)
        else:
            print(f"unknown arg: {a}", file=sys.stderr); sys.exit(1)

    if not os.path.exists(PACK_PATH):
        print(f"ERROR: {PACK_PATH} not found", file=sys.stderr); sys.exit(1)

    data, entries = read_pack(PACK_PATH)
    print(f"[scanner] {len(entries)} pack entries from {PACK_PATH}")

    # Look at type-0 (RAW = FAT files) entries — those carry the file data.
    triples_out = []
    for eid, eoff, esz, etype in entries:
        if etype != 0 or esz < 64:
            continue
        # FAT id is the low 12 bits of eid (0x2045 -> fat 0x45)
        fat_id = eid & 0xFFF
        payload = data[eoff:eoff + esz]
        blocks = parse_offset_table(payload) or parse_custom_1c(payload)
        if not blocks:
            continue
        # Classify each sub-block
        tagged = []
        for idx, (off, sz) in enumerate(blocks):
            sub = payload[off:off + sz]
            kind = classify_sub(sub)
            if kind is not None:
                tagged.append((idx, kind, sz))
        if not tagged:
            continue

        tiles = [(i, sz) for (i, k, sz) in tagged if k in ("TILE8BPP", "NCGR")]
        maps  = [(i, sz) for (i, k, sz) in tagged if k in ("MAP64x32", "NSCR")]
        pals  = [(i, sz) for (i, k, sz) in tagged if k in ("PAL256",   "NCLR")]
        if not (tiles and maps and pals):
            continue

        # Form triples within `proximity` indices
        for ti, tsz in tiles:
            for mi, msz in maps:
                if abs(mi - ti) > proximity:
                    continue
                for pi, psz in pals:
                    if abs(pi - ti) > proximity and abs(pi - mi) > proximity:
                        continue
                    triples_out.append((fat_id, ti, mi, pi, tsz, msz, psz))

    print(f"[scanner] found {len(triples_out)} candidate triples "
          f"(within {proximity} sub-indices of each other)")

    # Sort by likely "best": tiles 32K, map 4K, palette 512 first
    def score(t):
        _, _, _, _, tsz, msz, psz = t
        s = 0
        s += abs(tsz - 32768)
        s += abs(msz - 4096) * 4
        s += abs(psz - 512) * 8
        return s
    triples_out.sort(key=score)

    header = "fatHex   tile  map   pal     tile_sz   map_sz   pal_sz   env_value"
    lines = [header, "-" * len(header)]
    for fat, ti, mi, pi, tsz, msz, psz in triples_out:
        env = f"{fat:x}:{ti}:{mi}:{pi}"
        lines.append(f"  {fat:04x}  {ti:4d}  {mi:4d}  {pi:4d}  {tsz:8d}  {msz:7d}  {psz:6d}   {env}")

    text = "\n".join(lines) + "\n"
    if out_path:
        with open(out_path, "w") as f:
            f.write(text)
        print(f"[scanner] wrote {out_path}")
    print(text[:4000])
    print("…(truncated)" if len(text) > 4000 else "")


if __name__ == "__main__":
    main()
