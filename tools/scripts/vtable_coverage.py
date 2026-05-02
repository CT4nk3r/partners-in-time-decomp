#!/usr/bin/env python3
"""Coverage report: how many of the vtable candidates have all entries
resolved in our 819-entry fnptr table or as src-decompiled FUN_xxx?"""
import struct, glob, re, os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
LOAD = 0x02004000
data = open(os.path.join(ROOT,'extracted/arm9.bin'),'rb').read()
ARM9_END = LOAD + len(data)

# fnptr table addresses
fnptr = set()
fp = os.path.join(ROOT, 'pc/build/generated/fnptr_table_generated.c')
if os.path.exists(fp):
    txt = open(fp).read()
    for m in re.finditer(r'0x([0-9a-fA-F]{8})', txt):
        fnptr.add(int(m.group(1),16))

# src-decompiled FUNs
fun_src = set()
for f in glob.glob(os.path.join(ROOT,'arm9/src/*.c')):
    for m in re.finditer(r'FUN_([0-9a-fA-F]{8})\s*\(', open(f,encoding='utf-8',errors='ignore').read()):
        fun_src.add(int(m.group(1),16))

# parse vtables_report.txt
report = open(os.path.join(ROOT,'.session-work/vtables_report.txt')).read()
vtables = []
for line in report.splitlines():
    m = re.match(r'\s*0x[0-9a-fA-F]+\s+nds=0x([0-9a-fA-F]+)\s+count=\s*(\d+)', line)
    if m:
        vtables.append((int(m.group(1),16), int(m.group(2))))

print(f'fnptr-table size: {len(fnptr)}, src-decompiled: {len(fun_src)}')
print(f'arm9.bin range: 0x{LOAD:08X}..0x{ARM9_END:08X}')
print(f'total vtable candidates: {len(vtables)}')
print()

fully = partial = 0
arm9_covered_partial = 0
for nds, count in vtables:
    entries = []
    in_fnptr = in_src = in_arm9 = oor = 0
    for i in range(count):
        off = nds + i*4 - LOAD
        if off < 0 or off+4 > len(data): w = 0
        else: w = struct.unpack('<I', data[off:off+4])[0]
        a = w & ~1
        if a in fnptr: in_fnptr += 1
        if a in fun_src: in_src += 1
        if LOAD <= a < ARM9_END: in_arm9 += 1
        else: oor += 1
    resolved = max(in_fnptr, in_src)
    if resolved == count: fully += 1
    elif resolved > 0: partial += 1
    print(f'  vtable @0x{nds:08X} count={count:2d} | src={in_src:2d} fnptr={in_fnptr:2d} arm9bin-only={count-resolved-oor:2d} OUT={oor:2d}')

print()
print(f'fully resolved (all entries in fnptr or src): {fully}/{len(vtables)}')
print(f'partial:                                     {partial}/{len(vtables)}')
