import sys
sys.path.insert(0, r'tools')
from scan_asm_callsites import scan_file

start, branches = scan_file(r'arm9\asm\func\FUN_02005444.s')
print(f'FUN_{start:08x} branches:')
for pc, kind, cond, target in branches:
    print(f'  pc=0x{pc:08x}  {kind} cond={cond:x}  target=0x{target:08x}')

print()
start, branches = scan_file(r'arm9\asm\func\FUN_02005380.s')
print(f'FUN_{start:08x} branches:')
for pc, kind, cond, target in branches:
    print(f'  pc=0x{pc:08x}  {kind} cond={cond:x}  target=0x{target:08x}')

print()
start, branches = scan_file(r'arm9\asm\func\FUN_0200528c.s')
print(f'FUN_{start:08x} branches:')
for pc, kind, cond, target in branches:
    print(f'  pc=0x{pc:08x}  {kind} cond={cond:x}  target=0x{target:08x}')
