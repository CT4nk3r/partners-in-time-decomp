#!/usr/bin/env python3
"""Dump and resolve a vtable in arm9.bin against decompiled FUN_xxx C files."""
import struct, glob, re, sys, os

LOAD = 0x02004000
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
data = open(os.path.join(ROOT, 'extracted/arm9.bin'), 'rb').read()
def word(nds):
    off = nds - LOAD
    return struct.unpack('<I', data[off:off+4])[0]

fnptr_table = set()
fp = os.path.join(ROOT, 'pc/build/generated/fnptr_table_generated.c')
if os.path.exists(fp):
    with open(fp, encoding='utf-8', errors='ignore') as fh:
        for m in re.finditer(r'0x([0-9a-fA-F]{8})', fh.read()):
            fnptr_table.add(int(m.group(1), 16))

fun_in_src = {}
for f in glob.glob(os.path.join(ROOT, 'arm9/src/*.c')):
    fname = os.path.basename(f)
    with open(f, encoding='utf-8', errors='ignore') as fh:
        for m in re.finditer(r'^\s*\w[\w\s\*]*\s+FUN_([0-9a-fA-F]{8})\s*\(', fh.read(), re.M):
            a = int(m.group(1), 16)
            fun_in_src.setdefault(a, fname)

def resolve(addr):
    a = addr & ~1
    if a in fun_in_src:
        return f'src:{fun_in_src[a]}'
    if a in fnptr_table:
        return 'fnptr-only'
    if 0x02004000 <= a < 0x02000000 + 353252 + 0x4000:
        return 'arm9.bin (no decomp)'
    return 'OUT-OF-RANGE'

target = int(sys.argv[1], 16) if len(sys.argv) > 1 else 0x02050B68
count = int(sys.argv[2]) if len(sys.argv) > 2 else 53

print(f'=== vtable at NDS 0x{target:08X} ({count} entries) ===')
print(f'fnptr table size: {len(fnptr_table)}, decompiled FUN_xxx in arm9/src: {len(fun_in_src)}')
print()
src_resolved = ftab_resolved = arm9_only = oor = 0
for i in range(count):
    nds = target + i*4
    w = word(nds)
    r = resolve(w)
    if r.startswith('src:'): src_resolved += 1
    elif r == 'fnptr-only': ftab_resolved += 1
    elif r.startswith('arm9.bin'): arm9_only += 1
    else: oor += 1
    print(f'  [{i:2d}] @{nds:08X} -> {w:08X}  {r}')
print()
print(f'src-decompiled : {src_resolved}/{count}')
print(f'fnptr-table only: {ftab_resolved}/{count}')
print(f'in arm9.bin (undecomp): {arm9_only}/{count}')
print(f'out of range   : {oor}/{count}')
