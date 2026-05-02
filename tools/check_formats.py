#!/usr/bin/env python3
"""Check all RAW file headers for known NDS formats."""
import struct

with open('assets/mlpit.assets', 'rb') as f:
    data = f.read()

ec = struct.unpack_from('<I', data, 12)[0]

print('=== All RAW files - checking for NITRO/known formats ===')
print(f'{"ID":>10} {"Size":>9}  {"First 8 bytes":20s}  Magic')
for i in range(ec):
    off = 32 + i*16
    eid, eoff, esz, etype = struct.unpack_from('<IIII', data, off)
    if etype != 0:
        continue
    pay = data[eoff:eoff+min(16, esz)]
    m4 = pay[:4]
    m4str = ''.join(chr(b) if 32<=b<127 else '.' for b in m4)
    # Check for NITRO (LE magic - files are stored with byte order mark 0xFFFE)
    if len(pay) >= 6 and pay[4] == 0xFF and pay[5] == 0xFE:
        fmt = "NITRO"
    elif m4str == 'SDAT':
        fmt = "SDAT-AUDIO"
    elif m4 == b'NARC':
        fmt = "NARC-ARCHIVE"
    else:
        fmt = ""
    print(f'  {eid:#010x} {esz:>9}  {pay.hex()[:16]:16s}  "{m4str}" {fmt}')
