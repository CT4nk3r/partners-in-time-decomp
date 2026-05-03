#!/usr/bin/env python3
"""Parse NDS ROM filesystem and list files in key directories."""
import struct
import sys

ROM_PATH = "rom/baserom.nds"

def main():
    with open(ROM_PATH, "rb") as f:
        # Read header
        f.seek(0x40)
        fnt_off = struct.unpack('<I', f.read(4))[0]
        fnt_size = struct.unpack('<I', f.read(4))[0]
        fat_off = struct.unpack('<I', f.read(4))[0]
        fat_size = struct.unpack('<I', f.read(4))[0]
        
        f.seek(fnt_off)
        fnt = f.read(fnt_size)
        
        # Entry 0: subtable(4) + firstFileID(2) + numDirs(2)
        num_dirs = struct.unpack_from('<H', fnt, 6)[0]
        print(f"Total directories: {num_dirs}")
        
        # Parse directory table
        dirs = {}
        for i in range(num_dirs):
            entry_off = i * 8
            subtable = struct.unpack_from('<I', fnt, entry_off)[0]
            first_file = struct.unpack_from('<H', fnt, entry_off + 4)[0]
            parent = struct.unpack_from('<H', fnt, entry_off + 6)[0]
            dirs[i] = {'subtable': subtable, 'first_file': first_file, 'parent': parent}
        
        # Parse root dir names
        root_sub = dirs[0]['subtable']
        pos = root_sub
        file_id = dirs[0]['first_file']
        dir_names = {}
        
        print(f"\nRoot directory (first_file=0x{file_id:X}):")
        while pos < len(fnt):
            type_len = fnt[pos]
            pos += 1
            if type_len == 0:
                break
            is_dir = (type_len & 0x80) != 0
            name_len = type_len & 0x7F
            name = fnt[pos:pos+name_len].decode('ascii', errors='replace')
            pos += name_len
            if is_dir:
                sub_dir_id = struct.unpack_from('<H', fnt, pos)[0]
                pos += 2
                idx = sub_dir_id & 0x0FFF
                dir_names[idx] = name
                print(f"  [DIR] {name} (idx={idx})")
            else:
                print(f"  [FILE] {name} (id=0x{file_id:04X})")
                file_id += 1
        
        # List files in key directories
        targets = sys.argv[1:] if len(sys.argv) > 1 else ['FMap', 'FObjPc', 'Title', 'Menu']
        
        for target in targets:
            target_idx = None
            for idx, n in dir_names.items():
                if n == target:
                    target_idx = idx
                    break
            if target_idx is None:
                print(f"\n{target}: NOT FOUND")
                continue
            
            info = dirs[target_idx]
            pos = info['subtable']
            fid = info['first_file']
            sub = info['subtable']
            print(f"\n{target}/ (dir_idx={target_idx}, first_file=0x{fid:X}):")
            cnt = 0
            while pos < len(fnt) and cnt < 30:
                type_len = fnt[pos]
                pos += 1
                if type_len == 0:
                    break
                is_dir = (type_len & 0x80) != 0
                name_len = type_len & 0x7F
                name = fnt[pos:pos+name_len].decode('ascii', errors='replace')
                pos += name_len
                if is_dir:
                    pos += 2
                    print(f"  [DIR] {name}")
                else:
                    f.seek(fat_off + fid * 8)
                    start = struct.unpack('<I', f.read(4))[0]
                    end = struct.unpack('<I', f.read(4))[0]
                    size = end - start
                    print(f"  {name} (id=0x{fid:X}, ROM=0x{start:X}, {size} bytes)")
                    fid += 1
                cnt += 1

if __name__ == "__main__":
    main()
