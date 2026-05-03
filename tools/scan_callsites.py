#!/usr/bin/env python3
"""Scan arm9.bin for BL/B instructions targeting given functions.

ARM branch encoding:
  bits 31..28 = cond
  bits 27..25 = 0b101
  bit  24     = L (BL=1, B=0)
  bits 23..0  = signed imm24, target = PC+8+imm*4

The arm9 binary is loaded at 0x02000000 (per project convention).
"""
import sys, struct

BIN = r"C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\extracted\arm9.bin"
LOAD = 0x02000000

# functions of interest (the linked-list helpers)
TARGETS = {
    0x0202a33c: "FUN_0202a33c (list processor: walks DAT_0202a510 chain)",
    0x0202a51c: "FUN_0202a51c (list walker: marks update bit at +0x12)",
    0x0202a53c: "FUN_0202a53c (list-push prologue: head==NULL ? a5a4 : a56c)",
    0x0202a56c: "FUN_0202a56c (list-append fast path)",
    0x0202a5a4: "FUN_0202a5a4 (list first-insertion)",
    0x0202a62c: "FUN_0202a62c (list unlink/destroy)",
}

# DATA-pointer addresses we want to find readers/writers of (PC-rel ldr literal pools)
DATA_TARGETS = {
    0x0202a510: "DAT_0202a510 (master list head ptr)",
    0x0202a514: "DAT_0202a514 (list current slot)",
    0x0202a518: "DAT_0202a518 (list queued slot)",
    0x0202a568: "DAT_0202a568 (gx list head ptr)",
}


def load_func_addrs():
    addrs = []
    try:
        for line in open(r"C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\tools\arm9_func_addrs.txt"):
            line = line.strip()
            if line:
                addrs.append(int(line, 16))
    except FileNotFoundError:
        pass
    addrs.sort()
    return addrs


def func_containing(addrs, pc):
    """Return largest addr <= pc."""
    lo, hi = 0, len(addrs)
    while lo < hi:
        mid = (lo + hi) // 2
        if addrs[mid] <= pc:
            lo = mid + 1
        else:
            hi = mid
    return addrs[lo - 1] if lo > 0 else 0


def main():
    data = open(BIN, "rb").read()
    func_addrs = load_func_addrs()
    print(f"arm9.bin size = 0x{len(data):x} ({len(data)} bytes); known funcs={len(func_addrs)}")
    branches = {t: [] for t in TARGETS}
    lit_pools = {t: [] for t in DATA_TARGETS}

    n = len(data) & ~3
    for off in range(0, n, 4):
        w = struct.unpack_from("<I", data, off)[0]
        pc = LOAD + off
        # ARM branch: cond_xxx 101 L imm24
        if ((w >> 25) & 0x7) == 0x5:
            imm = w & 0xFFFFFF
            if imm & 0x800000:
                imm |= ~0xFFFFFF
            target = pc + 8 + (imm << 2)
            target &= 0xFFFFFFFF
            if target in TARGETS:
                L = (w >> 24) & 1
                cond = (w >> 28) & 0xF
                branches[target].append((pc, "BL" if L else "B", cond))

        # literal pool entry: word equals address-of one of DATA_TARGETS
        # Such words appear at end of functions and are loaded via LDR Rn,[PC, #imm]
        if w in DATA_TARGETS:
            lit_pools[w].append(pc)

    for t, name in TARGETS.items():
        b = branches[t]
        print(f"\n=== {name} @ 0x{t:08x} : {len(b)} direct branch callsites ===")
        for pc, kind, cond in b[:60]:
            owner = func_containing(func_addrs, pc) if func_addrs else 0
            owner_str = f"  [in FUN_{owner:08x}]" if owner else ""
            print(f"  {kind} cond={cond:x}  from 0x{pc:08x}{owner_str}")
        if len(b) > 60:
            print(f"  ... ({len(b)-60} more)")

    for t, name in DATA_TARGETS.items():
        ps = lit_pools[t]
        print(f"\n=== {name} @ 0x{t:08x} : {len(ps)} literal-pool refs ===")
        for pc in ps[:80]:
            owner = func_containing(func_addrs, pc) if func_addrs else 0
            owner_str = f"  [in FUN_{owner:08x}]" if owner else ""
            print(f"  word 0x{t:08x} at 0x{pc:08x}{owner_str}")
        if len(ps) > 80:
            print(f"  ... ({len(ps)-80} more)")


if __name__ == "__main__":
    main()
