"""Find .s files whose body contains a `.word 0xADDR` matching given target addresses.
This finds function-pointer references (literal pool / data slot)."""
import os, re

ASM_DIR = r"C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\arm9\asm\func"
TARGETS = {
    0x02005444: "FUN_02005444",
    0x02005380: "FUN_02005380",
    0x0202a53c: "FUN_0202a53c",
    0x0202a56c: "FUN_0202a56c",
    0x0202a5a4: "FUN_0202a5a4",
    0x0202a510: "DAT_0202a510 (list head)",
    0x0202a514: "DAT_0202a514",
    0x0202a518: "DAT_0202a518",
    0x0202a568: "DAT_0202a568 (gx list head)",
}
WORD_RE = re.compile(r"\.word\s+0x([0-9a-fA-F]{8})")
NAME_RE = re.compile(r"FUN_([0-9a-fA-F]{8})\.s$", re.IGNORECASE)

hits = {t: [] for t in TARGETS}

for name in sorted(os.listdir(ASM_DIR)):
    if not name.endswith(".s"):
        continue
    m = NAME_RE.search(name)
    if not m:
        continue
    fstart = int(m.group(1), 16)
    pc = fstart
    with open(os.path.join(ASM_DIR, name), "r", errors="replace") as f:
        for line in f:
            wm = WORD_RE.search(line)
            if not wm:
                continue
            insn = int(wm.group(1), 16)
            if insn in TARGETS:
                hits[insn].append((fstart, pc))
            pc += 4

for t, label in TARGETS.items():
    hs = hits[t]
    print(f"\n=== '.word 0x{t:08x}' ({label}) : {len(hs)} occurrences ===")
    for fstart, pc in hs[:40]:
        print(f"  in FUN_{fstart:08x} at offset 0x{pc-fstart:x} (pc 0x{pc:08x})")
    if len(hs) > 40:
        print(f"  ... +{len(hs)-40} more")
