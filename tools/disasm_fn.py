#!/usr/bin/env python3
"""Quick disassembler for a single ARM function in an overlay binary."""
import struct, sys

def disasm(binpath, base_addr, fn_addr, count=40):
    with open(binpath, 'rb') as f:
        data = f.read()
    offset = fn_addr - base_addr
    for i in range(count):
        addr = fn_addr + i * 4
        off = offset + i * 4
        if off + 4 > len(data):
            break
        word = struct.unpack_from('<I', data, off)[0]
        cond = (word >> 28) & 0xF
        conds = ['EQ','NE','CS','CC','MI','PL','VS','VC',
                 'HI','LS','GE','LT','GT','LE','','NV']
        cs = conds[cond] if cond != 0xE else ''

        # BL
        if (word & 0x0F000000) == 0x0B000000:
            signed_off = word & 0x00FFFFFF
            if signed_off & 0x800000:
                signed_off -= 0x1000000
            target = addr + 8 + signed_off * 4
            print(f'  0x{addr:08X}: BL{cs} 0x{target:08X}')
        # B
        elif (word & 0x0F000000) == 0x0A000000:
            signed_off = word & 0x00FFFFFF
            if signed_off & 0x800000:
                signed_off -= 0x1000000
            target = addr + 8 + signed_off * 4
            print(f'  0x{addr:08X}: B{cs} 0x{target:08X}')
        # PUSH (STMDB SP!)
        elif (word & 0x0FFF0000) == 0x092D0000:
            regs = word & 0xFFFF
            rlist = ','.join(f'R{b}' for b in range(16) if regs & (1 << b))
            print(f'  0x{addr:08X}: PUSH {{{rlist}}}')
        # POP (LDMIA SP!)
        elif (word & 0x0FFF0000) == 0x08BD0000:
            regs = word & 0xFFFF
            rlist = ','.join(f'R{b}' for b in range(16) if regs & (1 << b))
            print(f'  0x{addr:08X}: POP{cs} {{{rlist}}}')
        # BX Rm
        elif (word & 0x0FFFFFF0) == 0x012FFF10:
            rm = word & 0xF
            print(f'  0x{addr:08X}: BX{cs} R{rm}')
        # CMP Rn, #imm
        elif (word & 0x0FF00000) == 0x03500000:
            rn = (word >> 16) & 0xF
            imm = word & 0xFF
            rot = ((word >> 8) & 0xF) * 2
            val = ((imm >> rot) | (imm << (32 - rot))) & 0xFFFFFFFF if rot else imm
            print(f'  0x{addr:08X}: CMP{cs} R{rn}, #0x{val:X}')
        # MOV Rd, #imm
        elif (word & 0x0FF00000) == 0x03A00000:
            rd = (word >> 12) & 0xF
            imm = word & 0xFF
            print(f'  0x{addr:08X}: MOV{cs} R{rd}, #0x{imm:X}')
        # MOV Rd, Rm
        elif (word & 0x0FF00FF0) == 0x01A00000:
            rd = (word >> 12) & 0xF
            rm = word & 0xF
            print(f'  0x{addr:08X}: MOV{cs} R{rd}, R{rm}')
        # LDR Rd, [Rn, #off]
        elif (word & 0x0F700000) == 0x05100000 or (word & 0x0F700000) == 0x05900000:
            rd = (word >> 12) & 0xF
            rn = (word >> 16) & 0xF
            off12 = word & 0xFFF
            u = '+' if word & (1 << 23) else '-'
            print(f'  0x{addr:08X}: LDR{cs} R{rd}, [R{rn}, #{u}0x{off12:X}]')
        # STR Rd, [Rn, #off]
        elif (word & 0x0F700000) == 0x05000000 or (word & 0x0F700000) == 0x05800000:
            rd = (word >> 12) & 0xF
            rn = (word >> 16) & 0xF
            off12 = word & 0xFFF
            u = '+' if word & (1 << 23) else '-'
            print(f'  0x{addr:08X}: STR{cs} R{rd}, [R{rn}, #{u}0x{off12:X}]')
        # SUB/ADD SP
        elif (word & 0x0FEF0000) == 0x024D0000 or (word & 0x0FEF0000) == 0x028D0000:
            op = 'SUB' if (word >> 21) & 0xF == 2 else 'ADD'
            rd = (word >> 12) & 0xF
            imm = word & 0xFF
            print(f'  0x{addr:08X}: {op}{cs} R{rd}, SP, #0x{imm:X}')
        else:
            print(f'  0x{addr:08X}: 0x{word:08X}')

        # Stop at BX LR (unconditional)
        if word == 0xE12FFF1E:
            break

if __name__ == '__main__':
    binpath = sys.argv[1]
    base = int(sys.argv[2], 16)
    fn = int(sys.argv[3], 16)
    cnt = int(sys.argv[4]) if len(sys.argv) > 4 else 40
    disasm(binpath, base, fn, cnt)
