#!/usr/bin/env python3
"""Generate arm9/src/game_engine_a.c from Ghidra decompiled files."""
import os
import re

BASE = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'config', 'decompiled')
OUTPATH = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'arm9', 'src', 'game_engine_a.c')

ADDRS = [
    '02020008','02020094','0202016c','02020298','020202ec','020203a0','020204bc','02020560',
    '02020860','02020990','02020ab8','02020db8','02020ee0','02020f20','02020f58','02020fe8',
    '02021058','0202112c','020211b8','02021298','02021588','020216f0','02021adc','02021df0',
    '02021f7c','02021fd4','0202202c','02022770','0202296c','02022c18','02022e4c','02022e78',
    '02022ea4','02022ed0','0202309c','020231e8','02023480','02023698','02023854','02023990',
    '02023afc','02024aac','02025194','02025e74','02025fb4','02026530','020265ac','02026830',
    '020268c8'
]

# External functions that return values (used in assignments)
RETURN_INT_FUNS = {
    'FUN_0201d170', 'FUN_0201d3bc', 'FUN_02029bf8',
    'FUN_0203cda8',
}

def read_all():
    funcs = {}
    for addr in ADDRS:
        fpath = os.path.join(BASE, 'FUN_{}.c'.format(addr))
        with open(fpath, 'r') as f:
            funcs[addr] = f.read()
    return funcs

def get_size(content):
    m = re.search(r'Size:\s*(\d+)\s*bytes', content)
    return m.group(1) if m else '?'

def clean_types(text):
    text = re.sub(r'\bundefined4\b', 'u32', text)
    text = re.sub(r'\bundefined2\b', 'u16', text)
    text = re.sub(r'\bundefined1\b', 'u8', text)
    text = re.sub(r'\bundefined\b(?!\s*\*)', 'u8', text)
    text = re.sub(r'\buint\b(?!3|_|8|16|32|64)', 'u32', text)
    text = re.sub(r'\bushort\b', 'u16', text)
    text = re.sub(r'\buchar\b', 'u8', text)
    text = re.sub(r'\bbyte\b', 'u8', text)
    text = re.sub(r'\bbool\b', 'BOOL', text)
    text = re.sub(r'\buint3\b', 'u32', text)
    text = re.sub(r'\(uint3\)', '(u32)', text)
    text = re.sub(r'\bcode\s*\*', 'FuncPtr ', text)
    text = re.sub(r'\bcode\b', 'FuncPtr', text)
    return text

def fix_subword_access(text):
    text = re.sub(r'(\w+)\._2_2_', r'HWORD(\1)', text)
    text = re.sub(r'(\w+)\._1_3_', r'UBYTES123(\1)', text)
    text = re.sub(r'(\w+)\._0_2_', r'LWORD(\1)', text)
    text = re.sub(r'(\w+)\._0_1_', r'LBYTE(\1)', text)
    return text

def fix_concat(text):
    # Need to handle nested parens properly - do multiple passes
    for _ in range(5):
        # CONCAT31 - find balanced parens
        text = re.sub(r'CONCAT31\(([^()]*(?:\([^()]*\)[^()]*)*),\s*([^()]*(?:\([^()]*\)[^()]*)*)\)',
                      r'(((u32)(\1) << 8) | ((u8)(\2)))', text)
        text = re.sub(r'CONCAT22\(([^()]*(?:\([^()]*\)[^()]*)*),\s*([^()]*(?:\([^()]*\)[^()]*)*)\)',
                      r'(((u32)(\1) << 16) | ((u16)(\2)))', text)
        text = re.sub(r'SUB42\(([^()]*(?:\([^()]*\)[^()]*)*),\s*(\d+)\)',
                      r'((u16)((\1) >> ((\2) * 8)))', text)
    return text

def extract_function(content):
    lines = content.strip().split('\n')
    func_lines = []
    found_sig = False
    brace_count = 0
    for line in lines:
        stripped = line.strip()
        if not found_sig:
            if stripped.startswith('//') or stripped == '' or stripped.startswith('/*') or stripped.startswith('*') or stripped.startswith('*/'):
                continue
            found_sig = True
        func_lines.append(line)
        brace_count += line.count('{') - line.count('}')
        if brace_count <= 0 and len(func_lines) > 1 and '{' in '\n'.join(func_lines):
            break
    return '\n'.join(func_lines)

def collect_refs(all_funcs):
    our_fun_names = set('FUN_{}'.format(a) for a in ADDRS)
    all_dat = set()
    ext_funs = set()
    for addr, content in all_funcs.items():
        for m in re.finditer(r'(DAT_[0-9a-fA-F]+)', content):
            all_dat.add(m.group(1))
        for m in re.finditer(r'(?<!thunk_)(FUN_[0-9a-fA-F]+)', content):
            fn = m.group(1)
            if fn not in our_fun_names:
                ext_funs.add(fn)
        for m in re.finditer(r'(thunk_FUN_[0-9a-fA-F]+)', content):
            ext_funs.add(m.group(1))
    return sorted(all_dat), sorted(ext_funs)

def process_function(addr, content):
    func_text = extract_function(content)
    func_text = clean_types(func_text)
    func_text = fix_subword_access(func_text)
    func_text = fix_concat(func_text)
    # Remove WARNING comments
    func_text = re.sub(r'/\*\s*WARNING:.*?\*/', '', func_text)
    return func_text

def main():
    all_funcs = read_all()
    all_dat, ext_funs = collect_refs(all_funcs)

    # Determine which of our functions return values
    our_return_funs = set()
    for addr in ADDRS:
        content = all_funcs[addr]
        func_text = extract_function(content)
        # Check if it starts with int or uint
        first_line = func_text.strip().split('\n')[0].strip()
        if first_line.startswith('int ') or first_line.startswith('uint ') or first_line.startswith('undefined4 *'):
            our_return_funs.add('FUN_{}'.format(addr))

    out = []
    out.append('/**')
    out.append(' * Game Engine A -- core engine functions (0x02020xxx-0x02026xxx)')
    out.append(' * Decompiled from 0x02020008-0x020268c8 region (49 functions)')
    out.append(' */')
    out.append('#include "types.h"')
    out.append('')
    out.append('/* Ghidra sub-word access helpers */')
    out.append('#define LBYTE(x)     ((u8)(x))')
    out.append('#define LWORD(x)     ((u16)(x))')
    out.append('#define HWORD(x)     ((u16)((x) >> 16))')
    out.append('#define UBYTES123(x) ((u32)((x) >> 8))')
    out.append('')
    out.append('typedef int (*FuncPtr)();')
    out.append('')

    out.append('/* ============================================================')
    out.append(' * External data references (DAT_)')
    out.append(' * ============================================================ */')
    for dat in all_dat:
        out.append('extern u32 {};'.format(dat))

    out.append('')
    out.append('/* ============================================================')
    out.append(' * External function declarations')
    out.append(' * ============================================================ */')
    for fn in ext_funs:
        if fn in RETURN_INT_FUNS:
            out.append('int {}();'.format(fn))
        else:
            out.append('void {}();'.format(fn))

    out.append('')
    out.append('/* ============================================================')
    out.append(' * Forward declarations')
    out.append(' * ============================================================ */')
    processed = {}
    for addr in ADDRS:
        processed[addr] = process_function(addr, all_funcs[addr])

    for addr in ADDRS:
        func_text = processed[addr]
        lines = func_text.split('\n')
        sig = ''
        for line in lines:
            sig += ' ' + line.strip()
            if '{' in line:
                break
        sig = sig.strip()
        idx = sig.find('{')
        if idx > 0:
            sig = sig[:idx].strip()
        sig = re.sub(r'/\*.*?\*/', '', sig).strip()
        if sig:
            out.append('{};'.format(sig))

    out.append('')
    out.append('/* ============================================================')
    out.append(' * Function implementations')
    out.append(' * ============================================================ */')
    out.append('')

    for addr in ADDRS:
        size = get_size(all_funcs[addr])
        func_text = processed[addr]
        out.append('// FUN_{} @ 0x{} ({} bytes)'.format(addr, addr, size))
        out.append(func_text)
        out.append('')

    with open(OUTPATH, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out) + '\n')

    print('Written {} lines to {}'.format(len(out), OUTPATH))

if __name__ == '__main__':
    main()
