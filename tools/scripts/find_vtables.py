#!/usr/bin/env python3
"""
find_vtables.py — scan arm9.bin for candidate vtable arrays.

A vtable on the NDS is a .data table of 4-byte ARM-domain code pointers.
Each entry points into the ARM9 main code segment (typically
0x02000000..0x020563E4 for this ROM) and is even-aligned (low bit clear
for ARM mode; THUMB pointers have low bit set).

Strategy: at every 4-byte aligned offset, count how many consecutive
words look like "code pointer shaped" — i.e. value in
[0x02000000, 0x020563E4) with low bit clear. Sequences of 4+ are
candidates and probably represent vtables.

Output is gitignored (.session-work/vtables_report.txt) because it
includes ROM-derived addresses.

Usage:
    python tools/scripts/find_vtables.py [arm9.bin] [out.txt]
"""
import os
import struct
import sys

DEFAULT_BIN = os.path.join('extracted', 'arm9.bin')
DEFAULT_OUT = os.path.join('.session-work', 'vtables_report.txt')

ARM9_LOAD = 0x02004000  # arm9.bin loads here (per nds_arm9_ram.c)
CODE_LO = 0x02000000
CODE_HI = 0x020563E4
MIN_RUN = 4


def is_code_ptr(v):
    if v < CODE_LO or v >= CODE_HI:
        return False
    if v & 1:
        # THUMB pointers are valid too but vtables in this codebase are
        # ARM mode (verified via the existing decompiled vtables in
        # gx_util.c). Treat THUMB as a softer match.
        return False
    return True


def scan(buf, load_addr):
    n_words = len(buf) // 4
    words = struct.unpack('<%dI' % n_words, buf[:n_words * 4])

    runs = []
    i = 0
    while i < n_words:
        if is_code_ptr(words[i]):
            j = i
            while j < n_words and is_code_ptr(words[j]):
                j += 1
            run_len = j - i
            if run_len >= MIN_RUN:
                runs.append((i, run_len, words[i:j]))
            i = j
        else:
            i += 1

    return runs


def emit(runs, load_addr, out_path):
    os.makedirs(os.path.dirname(out_path) or '.', exist_ok=True)
    with open(out_path, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write(f'# vtable candidates in arm9.bin (load=0x{load_addr:08X})\n')
        fh.write('# format: <bin_offset> <nds_addr> count=N -> [ptrs]\n')
        fh.write(f'# total candidates: {len(runs)}\n\n')
        for word_idx, run_len, ptrs in runs:
            bin_off = word_idx * 4
            nds_addr = load_addr + bin_off
            head = ' '.join('0x%08X' % p for p in ptrs[:8])
            tail = '' if len(ptrs) <= 8 else f' ... (+{len(ptrs)-8} more)'
            fh.write(f'0x{bin_off:06X}  nds=0x{nds_addr:08X}  '
                     f'count={run_len:3d}  -> {head}{tail}\n')


def main(argv):
    bin_path = argv[1] if len(argv) > 1 else DEFAULT_BIN
    out_path = argv[2] if len(argv) > 2 else DEFAULT_OUT
    with open(bin_path, 'rb') as fh:
        buf = fh.read()
    runs = scan(buf, ARM9_LOAD)
    # Sort by length descending — long runs are more likely real vtables.
    runs.sort(key=lambda r: -r[1])
    emit(runs, ARM9_LOAD, out_path)
    print(f'wrote {out_path} ({len(runs)} candidates, longest={runs[0][1] if runs else 0} entries)')
    if runs:
        print('top 10 candidates by length:')
        for word_idx, run_len, ptrs in runs[:10]:
            nds_addr = ARM9_LOAD + word_idx * 4
            print(f'  nds=0x{nds_addr:08X}  count={run_len:3d}  '
                  f'first=0x{ptrs[0]:08X}')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
