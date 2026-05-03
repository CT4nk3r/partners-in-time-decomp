#!/usr/bin/env python3
"""sprite_atlas_render.py — render NDS 4bpp tile sheets from FAT subarchives
in the .assets pack, with assorted palettes, to PNGs.

Goal: sweep the pack and find tile sheets that visibly contain real
game character / sprite art (Mario, Luigi, enemies, UI panels, etc.)
so we can show them in docs/progress/."""
import os, sys, struct
from PIL import Image

PACK = os.path.join(os.path.dirname(__file__), "..", "assets", "mlpit.assets")

with open(PACK, "rb") as f:
    pack = f.read()

magic = pack[:8]
version, n_entries, sha = struct.unpack_from("<III", pack, 8)
print(f"pack version={version} entries={n_entries}")

entries = []
for i in range(n_entries):
    eid, eoff, esz, etype = struct.unpack_from("<IIII", pack, 32 + i*16)
    entries.append((eid, eoff, esz, etype))

# Type 0 = RAW. The "FAT" sub-archive entries are RAW too in this pack.
# Each NARC-like sub-archive has a header. Let's identify them by looking
# at the first 4 bytes of the largest RAW entries.

big_raws = sorted([e for e in entries if e[3] == 0 and e[2] > 100000],
                  key=lambda e: -e[2])

print(f"Top 8 large RAW entries (probably FAT archives):")
for eid, eoff, esz, _ in big_raws[:8]:
    head = pack[eoff:eoff+16]
    print(f"  id={eid:#x} sz={esz:>10}  head={head.hex()}")

# A common pattern in NDS packed archives: 4-byte file count at start,
# then offset table, then payloads. Let's try parsing FAT[0x45] (id 0x2045)
# the way the existing C code does.
FAT_BASE_ID = 0x2045  # confirmed from prior session work
fat_idx = next(i for i,e in enumerate(entries) if e[0] == FAT_BASE_ID)
fat_eid, fat_off, fat_sz, _ = entries[fat_idx]
fat_data = pack[fat_off:fat_off+fat_sz]
print(f"\nFAT[0x45] = {fat_sz} bytes. First 32 bytes: {fat_data[:32].hex()}")

# Try interpreting as a simple offset table: u32 count, u32 offsets[count]
n_sub = struct.unpack_from("<I", fat_data, 0)[0]
print(f"Naive sub-count = {n_sub}")

if n_sub > 1024 or n_sub == 0:
    # Try alternate layout: list of (offset, size) pairs
    n_sub = struct.unpack_from("<I", fat_data, 0)[0]
    print(f"  (assuming alternate offset/size pair layout)")

# Just enumerate sub-files by scanning for plausible offsets
# (sequential u32 values < fat_sz, starting at offset 0)
offsets = []
for i in range(0, min(4096, fat_sz), 4):
    v = struct.unpack_from("<I", fat_data, i)[0]
    if 0 < v < fat_sz:
        offsets.append((i, v))
    else:
        if len(offsets) > 10:
            break
print(f"Plausible offset entries: {len(offsets)}, first 10: {offsets[:10]}")

# Helper: decode 4bpp NDS tile data using a 16-color BGR555 palette
def bgr555_to_rgb(v):
    r = ((v >>  0) & 0x1F) << 3
    g = ((v >>  5) & 0x1F) << 3
    b = ((v >> 10) & 0x1F) << 3
    return (r, g, b)

def decode_4bpp_tilesheet(data, palette, tiles_per_row=16, max_tiles=None):
    n_tiles = len(data) // 32
    if max_tiles: n_tiles = min(n_tiles, max_tiles)
    rows = (n_tiles + tiles_per_row - 1) // tiles_per_row
    img = Image.new("RGB", (tiles_per_row*8, rows*8), (255, 0, 255))
    px = img.load()
    for t in range(n_tiles):
        tx = (t % tiles_per_row) * 8
        ty = (t // tiles_per_row) * 8
        tile_off = t * 32
        for y in range(8):
            for x in range(8):
                bi = tile_off + y*4 + (x // 2)
                if bi >= len(data): break
                b = data[bi]
                idx = (b & 0x0F) if (x % 2 == 0) else (b >> 4)
                px[tx+x, ty+y] = palette[idx % len(palette)]
    return img

def decode_8bpp_tilesheet(data, palette, tiles_per_row=16, max_tiles=None):
    n_tiles = len(data) // 64
    if max_tiles: n_tiles = min(n_tiles, max_tiles)
    rows = (n_tiles + tiles_per_row - 1) // tiles_per_row
    img = Image.new("RGB", (tiles_per_row*8, rows*8), (255, 0, 255))
    px = img.load()
    for t in range(n_tiles):
        tx = (t % tiles_per_row) * 8
        ty = (t // tiles_per_row) * 8
        tile_off = t * 64
        for y in range(8):
            for x in range(8):
                bi = tile_off + y*8 + x
                if bi >= len(data): break
                idx = data[bi]
                px[tx+x, ty+y] = palette[idx % len(palette)]
    return img

# Try to find palette and tile data within FAT[0x45]
# Use the offset list to slice sub-files
sub_starts = [v for _, v in offsets]
sub_starts.sort()
sub_ranges = []
for i, s in enumerate(sub_starts):
    e = sub_starts[i+1] if i+1 < len(sub_starts) else fat_sz
    sub_ranges.append((s, e - s))

print(f"\nDerived {len(sub_ranges)} sub-files in FAT[0x45]")
for i, (s, sz) in enumerate(sub_ranges[:10]):
    print(f"  sub[{i}] off={s} size={sz}")

# We know from prior work: sub[181] = font tile sheet, sub[177] = palette
# Let's try various combos: many tile sheets x many palettes
OUT_DIR = os.path.join(os.path.dirname(__file__), "..", "docs", "progress",
                       "10_sprite_atlas_sweep")
os.makedirs(OUT_DIR, exist_ok=True)

# Load each sub-file as raw bytes
def get_sub(i):
    if i >= len(sub_ranges): return None
    s, sz = sub_ranges[i]
    return fat_data[s:s+sz]

# Find candidate tile sheets: subs that are >= 4 KB (= 128+ tiles)
tile_subs = []
for i, (s, sz) in enumerate(sub_ranges):
    if 4096 <= sz <= 131072:
        # A tile sheet is bytes that look like 4bpp data (lots of indices 0/1)
        sample = fat_data[s:s+min(512,sz)]
        nz_lo = sum(1 for b in sample if (b & 0x0F) != 0)
        nz_hi = sum(1 for b in sample if (b >> 4) != 0)
        if 50 <= nz_lo <= 480 or 50 <= nz_hi <= 480:
            tile_subs.append(i)

# Find candidate palettes: subs of size 32 or 512 (16-color or 256-color)
pal_subs_16 = [i for i,(s,sz) in enumerate(sub_ranges) if sz == 32]
pal_subs_256 = [i for i,(s,sz) in enumerate(sub_ranges) if sz == 512]

print(f"\n{len(tile_subs)} candidate tile sheets, "
      f"{len(pal_subs_16)} 16-col palettes, {len(pal_subs_256)} 256-col palettes")
print(f"First 10 tile candidates: {tile_subs[:10]}")
print(f"First 10 16-col palettes: {pal_subs_16[:10]}")

# Render each tile sheet with each of the first few palettes
saved = 0
# Use 256-color palettes if available, else build a default rainbow
if pal_subs_256:
    pal_indices = pal_subs_256[:6]
else:
    pal_indices = [None]

# Also derive 16-col palettes by taking first 32 bytes of any 256-col palette
def get_palette_16_from_256(pdata, bank=0):
    base = bank * 32
    if base + 32 > len(pdata): base = 0
    pal = []
    for j in range(16):
        v = pdata[base+j*2] | (pdata[base+j*2+1] << 8)
        pal.append(bgr555_to_rgb(v))
    return pal

def get_palette_256(pdata):
    pal = []
    for j in range(min(256, len(pdata)//2)):
        v = pdata[j*2] | (pdata[j*2+1] << 8)
        pal.append(bgr555_to_rgb(v))
    while len(pal) < 256:
        pal.append((255, 0, 255))
    return pal

for ti in tile_subs[:30]:
    tdata = get_sub(ti)
    if not tdata: continue

    # 4bpp render with each of the first 3 palettes (16-col extraction)
    for pi in pal_indices[:3]:
        if pi is None: continue
        pdata = get_sub(pi)
        if not pdata or len(pdata) < 32: continue
        for bank in range(0, 4):
            pal = get_palette_16_from_256(pdata, bank)
            img = decode_4bpp_tilesheet(tdata, pal, tiles_per_row=16, max_tiles=256)
            img = img.resize((img.width*4, img.height*4), Image.NEAREST)
            out = os.path.join(OUT_DIR, f"4bpp_t{ti:03d}_p{pi:03d}_b{bank}.png")
            img.save(out)
            saved += 1

    # 8bpp render too with first 256-color palette
    if pal_subs_256:
        pdata = get_sub(pal_subs_256[0])
        if pdata and len(pdata) >= 512:
            pal = get_palette_256(pdata)
            img = decode_8bpp_tilesheet(tdata, pal, tiles_per_row=16, max_tiles=128)
            img = img.resize((img.width*4, img.height*4), Image.NEAREST)
            out = os.path.join(OUT_DIR, f"8bpp_t{ti:03d}.png")
            img.save(out)
            saved += 1

    if saved >= 200:
        break

print(f"\nSaved {saved} sprite sheet renders to {OUT_DIR}")
