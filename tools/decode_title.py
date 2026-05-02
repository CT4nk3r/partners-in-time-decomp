#!/usr/bin/env python3
"""Decode TitleBG.dat from the M&L:PiT ROM and render title screen images.

Uses AlphaDream's custom compression format (varint header + LZ/RLE blocks)
as documented in MnL-Modding/mnllib.py (mnllib/bis/compression.py).
"""
import struct, sys, io
from PIL import Image


def decode_varint(data, pos):
    """Decode a variable-length integer (AlphaDream format)."""
    b = data[pos]; pos += 1
    size = b >> 6
    result = b & 0x3F
    for i in range(size):
        result |= data[pos] << ((i + 1) * 6)
        pos += 1
    return result, pos


def ad_decompress(data, offset=0):
    """AlphaDream decompression (from mnllib.py/compression.py).
    
    Format:
      varint  uncompressed_size
      varint  num_blocks - 1
      For each block:
        u16    block_size (compressed bytes)
        block_size bytes of command data:
          Each command byte has 4 commands (2 bits each, LSB first):
            0: end of block
            1: literal byte
            2: LZ back-reference (2 bytes: 12-bit offset, 4-bit length+2)
            3: RLE (2 bytes: count+2, data byte)
    """
    pos = offset
    uncomp_size, pos = decode_varint(data, pos)
    num_blocks_m1, pos = decode_varint(data, pos)
    num_blocks = num_blocks_m1 + 1

    result = bytearray()
    for _ in range(num_blocks):
        block_size = struct.unpack_from('<H', data, pos)[0]
        pos += 2
        block_end = pos + block_size
        
        done = False
        for _ in range(256):
            if done or pos >= block_end:
                break
            cmd_byte = data[pos]; pos += 1
            for _ in range(4):
                cmd = cmd_byte & 0x03
                if cmd == 0:
                    done = True
                    break
                elif cmd == 1:
                    result.append(data[pos]); pos += 1
                elif cmd == 2:
                    d1 = data[pos]; d2 = data[pos+1]; pos += 2
                    back_offset = d1 | ((d2 & 0xF0) << 4)
                    length = (d2 & 0x0F) + 2
                    src_pos = len(result) - back_offset
                    for j in range(length):
                        result.append(result[src_pos + j])
                elif cmd == 3:
                    count = data[pos] + 2; pos += 1
                    val = data[pos]; pos += 1
                    result.extend(bytes([val]) * count)
                cmd_byte >>= 2
        
        pos = block_end  # advance to next block
    
    return bytes(result), uncomp_size


def decode_bgr555_palette(data, offset, count=256):
    """Decode BGR555 palette entries."""
    pal = []
    for i in range(count):
        c = struct.unpack_from('<H', data, offset + i*2)[0]
        r = (c & 0x1F) * 8
        g = ((c >> 5) & 0x1F) * 8
        b = ((c >> 10) & 0x1F) * 8
        pal.append((r, g, b))
    return pal


def render_4bpp_tiles(tile_data, pal, cols=32, name='tiles_4bpp.png'):
    """Render 4bpp 8x8 tiles as a sheet image."""
    n_tiles = len(tile_data) // 32
    if n_tiles < 1:
        return None
    rows = (n_tiles + cols - 1) // cols
    img = Image.new('RGB', (cols * 8, rows * 8), (0, 0, 0))
    for t in range(n_tiles):
        tx = (t % cols) * 8
        ty = (t // cols) * 8
        for py in range(8):
            for px in range(8):
                byte_idx = t * 32 + py * 4 + px // 2
                if byte_idx >= len(tile_data):
                    continue
                if px & 1:
                    ci = (tile_data[byte_idx] >> 4) & 0xF
                else:
                    ci = tile_data[byte_idx] & 0xF
                if ci > 0 and ci < len(pal):
                    img.putpixel((tx + px, ty + py), pal[ci])
    img.save(name)
    print("Saved %s (%dx%d, %d tiles)" % (name, cols*8, rows*8, n_tiles))
    return img


def render_8bpp_tiles(tile_data, pal, cols=32, name='tiles_8bpp.png'):
    """Render 8bpp 8x8 tiles as a sheet image."""
    n_tiles = len(tile_data) // 64
    if n_tiles < 1:
        return None
    rows = (n_tiles + cols - 1) // cols
    img = Image.new('RGB', (cols * 8, rows * 8), (0, 0, 0))
    for t in range(n_tiles):
        tx = (t % cols) * 8
        ty = (t // cols) * 8
        for py in range(8):
            for px in range(8):
                ci = tile_data[t * 64 + py * 8 + px]
                if ci > 0 and ci < len(pal):
                    img.putpixel((tx + px, ty + py), pal[ci])
    img.save(name)
    print("Saved %s (%dx%d, %d tiles)" % (name, cols*8, rows*8, n_tiles))
    return img


def render_tilemap(tile_data, map_data, pal, bpp, name='screen.png'):
    """Render a tilemap (32x32 entries) using tiles and palette."""
    W, H = 256, 192
    img = Image.new('RGB', (W, H), (0, 0, 0))
    n_map = len(map_data) // 2
    map_w = 32
    map_h = min(n_map // map_w, H // 8)
    tile_size = 32 if bpp == 4 else 64
    
    for my in range(map_h):
        for mx in range(min(map_w, W // 8)):
            entry = struct.unpack_from('<H', map_data, (my * map_w + mx) * 2)[0]
            tile_num = entry & 0x3FF
            h_flip = (entry >> 10) & 1
            v_flip = (entry >> 11) & 1
            pal_bank = (entry >> 12) & 0xF
            
            tile_off = tile_num * tile_size
            if tile_off + tile_size > len(tile_data):
                continue
            
            for py in range(8):
                for px in range(8):
                    rpx = (7 - px) if h_flip else px
                    rpy = (7 - py) if v_flip else py
                    
                    if bpp == 4:
                        byte_idx = tile_off + rpy * 4 + rpx // 2
                        if rpx & 1:
                            ci = (tile_data[byte_idx] >> 4) & 0xF
                        else:
                            ci = tile_data[byte_idx] & 0xF
                        if ci == 0:
                            continue
                        ci += pal_bank * 16
                    else:
                        ci = tile_data[tile_off + rpy * 8 + rpx]
                        if ci == 0:
                            continue
                    
                    if ci < len(pal):
                        img.putpixel((mx*8 + px, my*8 + py), pal[ci])
    
    img.save(name)
    print("Saved %s (%dx%d, %d map entries)" % (name, W, H, n_map))
    return img


def main():
    rom = open('rom/baserom.nds', 'rb').read()
    header = open('extracted/header.bin', 'rb').read()
    fat_off = struct.unpack_from('<I', header, 0x48)[0]
    f99_start = struct.unpack_from('<I', rom, fat_off + 99*8)[0]
    f99_end = struct.unpack_from('<I', rom, fat_off + 99*8 + 4)[0]
    td = rom[f99_start:f99_end]
    print("TitleBG.dat: %d bytes" % len(td))

    # Parse offset table
    hdr_val = struct.unpack_from('<I', td, 0)[0]
    n = hdr_val // 4
    offs = [struct.unpack_from('<I', td, i*4)[0] for i in range(n)]
    offs.append(len(td))
    print("Sub-entries: %d" % n)

    # Try decompressing each sub-entry
    for idx in range(n):
        sub = td[offs[idx]:offs[idx+1]]
        sz = len(sub)
        if sz < 4:
            print("  sub[%2d]: %d bytes (too small)" % (idx, sz))
            continue
        
        # Check if first byte could be a varint header (high 2 bits = size field)
        try:
            decomp, expected_size = ad_decompress(sub, 0)
            print("  sub[%2d]: %d -> %d bytes (expected %d)%s" % (
                idx, sz, len(decomp), expected_size,
                " OK" if len(decomp) == expected_size else " SIZE MISMATCH"))
        except Exception as e:
            # Not AD compressed — could be raw palette or other data
            desc = ""
            if sz == 512:
                desc = " (palette 256-color)"
            elif sz == 16:
                desc = " (mini-palette)"
            elif sub[:4] == b'ATXI':
                w, h = struct.unpack_from('<HH', sub, 4)
                desc = " (ATXI %dx%d)" % (w, h)
            print("  sub[%2d]: %d bytes — not AD compressed%s [%s]" % (
                idx, sz, desc, str(e)[:60]))

    # Now decode the actual title screen
    # sub[0] = tiles layer 1, sub[1] = tilemap layer 1, sub[2] = palette 1
    # sub[3] = tiles layer 2, sub[4] = tilemap layer 2, sub[5] = palette 2
    
    results = {}
    for idx in range(min(n, 12)):
        sub = td[offs[idx]:offs[idx+1]]
        sz = len(sub)
        if sz == 512:
            results[idx] = ('palette', decode_bgr555_palette(td, offs[idx]))
            continue
        if sz == 16:
            results[idx] = ('mini_palette', decode_bgr555_palette(td, offs[idx], 8))
            continue
        try:
            decomp, _ = ad_decompress(sub, 0)
            results[idx] = ('data', decomp)
        except:
            results[idx] = ('raw', sub)
    
    # Render tile sheets and tilemap composites
    for layer, (tile_idx, map_idx, pal_idx) in enumerate([(0, 1, 2), (3, 4, 5)]):
        if tile_idx not in results or pal_idx not in results:
            continue
        
        tile_type, tile_data = results[tile_idx]
        if tile_type != 'data':
            print("Layer %d: tiles not decompressed" % layer)
            continue
        
        pal_type, pal = results[pal_idx]
        
        # Try 4bpp tile sheet
        render_4bpp_tiles(tile_data, pal, 32, 'title_layer%d_4bpp.png' % layer)
        # Try 8bpp tile sheet  
        render_8bpp_tiles(tile_data, pal, 32, 'title_layer%d_8bpp.png' % layer)
        
        # Render tilemap if available
        if map_idx in results:
            map_type, map_data = results[map_idx]
            if map_type == 'data':
                render_tilemap(tile_data, map_data, pal, 4, 'title_layer%d_screen4bpp.png' % layer)
                render_tilemap(tile_data, map_data, pal, 8, 'title_layer%d_screen8bpp.png' % layer)

    print("\nDone!")


if __name__ == '__main__':
    main()
