# Asset Format — Mario & Luigi: Partners in Time PC Port

## Pack File (`assets/mlpit.assets`)

The asset pack is built by `tools/extract_assets.py` from the NDS ROM and loaded at
runtime by `pc/src/asset_pack.c`.  It uses the custom **MLPIT pack format** defined in
`pc/include/mlpit_pack.h`:

```
Offset  Size  Field
  0       8   Magic: "MLPIT\x00\x00\x00"
  8       4   Version (u32 LE, currently 1)
 12       4   Entry count (u32 LE)
 16      16   SHA-256 prefix / build ID
 32     N×16  Entry table: (id u32, offset u32, size u32, type u32) each
  …          Blob data (entries concatenated in offset order)
```

Entry types: `0=RAW`, `1=OVERLAY`, `2=OVT (overlay table)`, `3=FAT`, `4=FNT`.

---

## Container Formats Found in ROM Data

Running `python tools/inspect_archives.py` on the 104-entry pack reveals the
following formats (104 entries total):

| Format                       | Count | Notes                                    |
|------------------------------|-------|------------------------------------------|
| Offset-table archive         |  33   | Large sprite/tilemap banks               |
| Custom-0x1C archive          |  20   | Game-specific multi-block animation data |
| Palette (raw BGR555)         |  17   | 16–256 colour palettes, 32–512 B         |
| 4bpp tiles (raw)             |  12   | 8×8 pixels × 32 B each                  |
| LZ77 (BIOS SWI 0x11)         |   7   | Overlays + large tile sets               |
| RLE  (BIOS SWI 0x13)         |   7   | Overlays                                 |
| Huffman (BIOS SWI 0x12)      |   6   | Compressed audio/sprite data             |
| Unknown (FAT, FNT meta)      |   2   | ROM filesystem metadata                  |

---

## BIOS Compression (SWI 0x11 / 0x12 / 0x13)

All three formats share the same **4-byte header**:

```
Byte 0:      compression type  (0x10 = LZ77, 0x20/0x24/0x28 = Huffman, 0x30 = RLE)
Bytes 1–3:   decompressed size (little-endian 24-bit unsigned)
```

### LZ77 (type 0x10)

After the 4-byte header, data is organised as **flag blocks**:

```
For each block:
  flag_byte (1 byte):  bit 7 = oldest unit, bit 0 = newest
  8 units:
    flag bit = 0  → literal byte (1 byte)
    flag bit = 1  → back-reference (2 bytes):
                      byte A = (len-3) << 4 | disp_high4
                      byte B = disp_low8
                    len  = (A >> 4) + 3       (3..18)
                    disp = ((A & 0xF) << 8 | B) + 1  (1..4096 bytes back)
```

Reference: GBATEK §"BIOS Decompression Functions".

### Huffman (type 0x20–0x2C)

```
4-byte header (type + decompressed size)
1 byte: tree_size = (n_nodes / 2) - 1  (tree occupies (tree_size+1)*2 bytes)
Tree data: 2 bytes per node = [left_offset | flags, right_offset | flags]
  flag bits: bit 7 = right is leaf, bit 6 = left is leaf
             leaf value stored in next byte after the flags byte
Compressed data: packed bits (MSB first), 4-bit or 8-bit symbols
```

### RLE (type 0x30)

```
4-byte header (type + decompressed size)
For each run:
  flag_byte:
    bit 7 = 0  → copy next (flag & 0x7F) + 1 bytes literally
    bit 7 = 1  → repeat next 1 byte for (flag & 0x7F) + 3 times
```

---

## Custom "0x1C" Archive (M&L:PiT game-specific)

Files with the 4-byte magic `1c 00 00 00` use a game-specific multi-block archive:

```
Offset  Size  Field
  0      4    Header size = 0x1C (28 bytes; also acts as self-describing magic)
  4      4    Byte offset of end of data section 1 (= start of section 2)
  8     20    5 × file_size (repeated sentinel / section boundary info)
 28    N×4    Offset table: N u32 offsets pointing to sub-blocks within section 1.
              N = (first_sub_block_offset - 28) / 4
              First entry value = byte offset of sub-block 0.
 ...         Variable-size sub-blocks (may themselves be nested archives or
              compressed data for animation frames / sprite cells).
 sec1_end .. file_end:  Section 2 (same structure, for additional layers or palettes)
```

These files contain sprite animation data: each sub-block is a frame or sub-image.
Sub-blocks can themselves be further nested offset tables (multi-layer animations).

The format appears in ~20 RAW entries (IDs 0x2035–0x2042, 0x205A–0x2060).

---

## Offset-Table Archives

~33 entries start with an ascending sequence of u32 offsets where the first value
equals the size of the offset table itself (= `n_entries * 4`).  The sub-blocks
begin immediately after.  This is used for large sprite banks, sound data, and
multi-screen tilemaps.

Key large offset-table files:
- `0x00002061` — 397 KB, 100 sub-blocks (main sprite bank)
- `0x00002063` — 100 KB, 24 sub-blocks
- `0x00002064` — 10 KB, 273 sub-blocks (likely font/icon data)
- `0x0000204b` — 48 KB, large sprite set

---

## Palette Format

Raw 16-colour (32 B) or 256-colour (512 B) palettes in **BGR555** little-endian
format (same layout as NDS hardware palette RAM):

```
uint16_t colour[N];   // N = 16 or 256
// Colour encoding: bits[4:0]=R, bits[9:5]=G, bits[14:10]=B, bit15=0
```

Palette entries (IDs 0x1001–0x1024) are 32 bytes each = 16 colours.

---

## 4bpp Tile Data

Raw uncompressed 4bpp tiles, 32 bytes per 8×8 pixel tile (4 bits per pixel,
two pixels packed per byte in low-nibble/high-nibble order):

```
tile[t][row][col/2] = (pixel[col] & 0xF) | (pixel[col+1] << 4)
```

Each palette index selects a BGR555 colour from the 16-colour palette.

---

## Key Asset IDs

| ID           | Format   | Description                                     |
|--------------|----------|-------------------------------------------------|
| `0x00000001` | FAT      | ROM filesystem allocation table                 |
| `0x00000002` | FNT      | ROM filesystem name table                       |
| `0x00000003` | OVT      | Overlay table (overlay load addresses)          |
| `0x00001000` | LZ77     | Main overlay 0 (≈14 MB decompressed)            |
| `0x00001001` | Palette  | Overlay 0 palette (16 colours)                  |
| `0x00002046` | Offset-tbl | Large sprite bank (826 KB, 516 sub-blocks)    |
| `0x00002062` | 4bpp raw | Large tile sheet (6.5 MB, ~206K tiles)          |
| `0x0000204f` | Offset-tbl | Largest data file (28 MB)                     |
