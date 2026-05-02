"""Find PAD_Read implementation by searching for 0x04000130 literal pool refs."""
import struct

with open("extracted/arm9.bin", "rb") as f:
    base = 0x02004000
    data = f.read()

target = 0x04000130
matches = []
for i in range(0, len(data) - 4, 4):
    val = struct.unpack_from("<I", data, i)[0]
    if val == target:
        addr = base + i
        matches.append(addr)

print(f"Literal pool refs to 0x{target:08X}: {len(matches)}")
for m in matches[:10]:
    print(f"  0x{m:08X}")
    off = m - base
    for j in range(-8, 4, 4):
        if 0 <= off + j < len(data) - 4:
            v = struct.unpack_from("<I", data, off + j)[0]
            tag = " <--- target" if j == 0 else ""
            print(f"    [{j // 4:+d}] 0x{v:08X}{tag}")
