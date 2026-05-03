#!/usr/bin/env python3
"""
extract_data_inits.py - Generate a C header containing initial values
for arm9 .data static slots that the decompiled C code currently
zero-inits (which causes immediate NPE on the host port).

The script reads the user's own legally-owned ROM-derived arm9.bin and
emits a header with byte arrays + numeric constants ready to be
#included from pc/src/host_data_init.c.

USAGE:
    python extract_data_inits.py <arm9.bin> <output.h>

The header is written to a build-only directory (e.g.
pc/build/generated/) and is gitignored. ROM-derived bytes MUST NOT be
committed to source control - they are copyrighted.

The slot manifest below lists every (RAM address, byte size, C label)
tuple we want to lift out of arm9.bin. Add new entries as more decomp
work uncovers more uninitialized .data slots.
"""

import os
import sys
import struct

# arm9.bin is loaded at this RAM address by the NDS bootloader (matches
# header offset 0x28 in baserom.nds for Mario & Luigi PiT US v1.0).
ARM9_LOAD_BASE = 0x02004000


# Slot manifest: each entry is a dict-like tuple. Entry kinds:
#
#   ("scalar", ram_addr, size, c_label)
#       Emits   #define HOSTDATA_<label> 0xVALUE  (size 1/2/4)
#       Emits   static const u8 HOSTDATA_<label>_BYTES[size] = { ... }
#
#   ("blob",   ram_addr, size, c_label)
#       Emits only the bytes array (no #define). Use for tables /
#       config blobs / structs whose value is the bytes themselves.
#
#   ("ptr_blob", ptr_ram_addr, blob_size, c_label)
#       Reads the 4-byte pointer at ptr_ram_addr from arm9.bin, then
#       extracts blob_size bytes from the address that pointer points
#       to (still inside arm9.bin range). Emits:
#         #define HOSTDATA_<label>_PTR 0xVALUE   (the original RAM addr)
#         static const u8 HOSTDATA_<label>_BYTES[blob_size] = { ... }
#       The blob can be installed as the host pointer for that slot.
SLOTS = [
    # ---- game_init.c clGameMain factory slots (FUN_02005b70 family) ----
    ("scalar", 0x02005D28, 4,  "DAT_02005D28"),  # sGameState slot ptr
    ("scalar", 0x02005D2C, 4,  "DAT_02005D2C"),  # alloc size = 0x58C
    ("scalar", 0x02005D30, 4,  "DAT_02005D30"),  # cfg-blob offset = 0x57C
    ("scalar", 0x02005D34, 4,  "DAT_02005D34"),  # cfg blob ptr
    ("scalar", 0x02005D38, 4,  "DAT_02005D38"),  # disp-flag ptr
    ("scalar", 0x02005D68, 4,  "DAT_02005D68"),  # scene-jump = 0x0202A56C

    # Default config blob (32 bytes at 0x02048F44).
    ("blob",   0x02048F44, 32, "CONFIG_BLOB_02048F44"),

    # ---- display_system.c .data slots (FUN_020192f8 family) ----
    # FUN_0201913c stat-clamp constants (DAT_020192d4..f4 family).
    ("scalar", 0x020192D4, 4, "DAT_020192D4"),
    ("scalar", 0x020192D8, 4, "DAT_020192D8"),  # = 0x02059C68 (slot ptr)
    ("scalar", 0x020192DC, 4, "DAT_020192DC"),
    ("scalar", 0x020192E0, 4, "DAT_020192E0"),
    ("scalar", 0x020192E4, 4, "DAT_020192E4"),
    ("scalar", 0x020192E8, 4, "DAT_020192E8"),
    ("scalar", 0x020192EC, 4, "DAT_020192EC"),
    ("scalar", 0x020192F0, 4, "DAT_020192F0"),
    ("scalar", 0x020192F4, 4, "DAT_020192F4"),

    # DAT_02019730 = 0x02059C68 - same word as DAT_02005D28 (host
    # overrides this with the live slot pointer at runtime).
    ("scalar", 0x02019730, 4,  "DAT_02019730"),
    # DAT_02019744 / 02019768 are scalar offsets (0x41C / 0x464).
    ("scalar", 0x02019744, 4,  "DAT_02019744"),
    ("scalar", 0x02019768, 4,  "DAT_02019768"),

    # Pointer-typed slots: extract both the original RAM ptr value
    # and a backing blob from the pointer target. Sized 512 bytes to
    # cover the layer-row structures FUN_020192f8 indexes
    # (unaff_r6[iVar7*6+4] inner loop reads up to ~30 entries deep).
    ("ptr_blob", 0x02019734, 512, "DISP_TBL_02019734"),
    ("ptr_blob", 0x02019738, 256, "DISP_TBL_02019738"),
    ("ptr_blob", 0x0201973C, 256, "DISP_TBL_0201973C"),
    ("ptr_blob", 0x02019740, 256, "DISP_TBL_02019740"),
    ("ptr_blob", 0x02019748, 512, "DISP_TBL_02019748"),
    ("ptr_blob", 0x0201974C, 256, "DISP_TBL_0201974C"),
    ("ptr_blob", 0x02019750, 256, "DISP_TBL_02019750"),
    ("ptr_blob", 0x02019754, 256, "DISP_TBL_02019754"),
    ("ptr_blob", 0x02019758, 512, "DISP_TBL_02019758"),
    ("ptr_blob", 0x0201975C, 256, "DISP_TBL_0201975C"),
    ("ptr_blob", 0x02019760, 256, "DISP_TBL_02019760"),
    ("ptr_blob", 0x02019764, 256, "DISP_TBL_02019764"),
    ("ptr_blob", 0x0201976C, 512, "DISP_TBL_0201976C"),
    ("ptr_blob", 0x02019770, 256, "DISP_TBL_02019770"),
    ("ptr_blob", 0x02019774, 256, "DISP_TBL_02019774"),
    ("ptr_blob", 0x02019778, 256, "DISP_TBL_02019778"),
    # Trailing tables read after the 4-layer loop in FUN_020192f8 (line 507+).
    ("ptr_blob", 0x0201977C, 256, "DISP_TBL_0201977C"),
    ("ptr_blob", 0x02019780, 256, "DISP_TBL_02019780"),
    ("ptr_blob", 0x02019784, 256, "DISP_TBL_02019784"),
    ("ptr_blob", 0x02019788, 256, "DISP_TBL_02019788"),
    ("ptr_blob", 0x0201978C, 256, "DISP_TBL_0201978C"),
    ("ptr_blob", 0x02019790, 256, "DISP_TBL_02019790"),
]


def emit_header(arm9_path: str, out_path: str) -> None:
    with open(arm9_path, "rb") as f:
        arm9 = f.read()

    os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)

    lines = []
    lines.append("/* Auto-generated by tools/scripts/extract_data_inits.py")
    lines.append(" * DO NOT EDIT, DO NOT COMMIT.")
    lines.append(" * Contents are derived from the user's own arm9.bin and")
    lines.append(" * therefore are copyrighted Nintendo machine data.")
    lines.append(" */")
    lines.append("#ifndef HOST_DATA_INIT_GENERATED_H")
    lines.append("#define HOST_DATA_INIT_GENERATED_H")
    lines.append("")
    lines.append(f"#define HOSTDATA_ARM9_LOAD_BASE 0x{ARM9_LOAD_BASE:08X}u")
    lines.append("")

    def read_chunk(addr, size, what):
        off = addr - ARM9_LOAD_BASE
        if off < 0 or off + size > len(arm9):
            raise SystemExit(
                f"{what} 0x{addr:08X} (+{size}) out of arm9.bin range "
                f"(size {len(arm9)})")
        return arm9[off:off + size]

    def fmt_bytes(name, chunk):
        byte_str = ", ".join(f"0x{b:02X}" for b in chunk)
        return (f"static const unsigned char HOSTDATA_{name}_BYTES"
                f"[{len(chunk)}] = {{ {byte_str} }};")

    n_emitted = 0
    for entry in SLOTS:
        kind = entry[0]
        if kind == "scalar":
            _, ram_addr, size, label = entry
            chunk = read_chunk(ram_addr, size, label)
            lines.append(f"/* {label}: scalar ram=0x{ram_addr:08X} size={size} */")
            if size == 1:
                value = chunk[0]
                fmtw = 2
            elif size == 2:
                (value,) = struct.unpack_from("<H", chunk, 0)
                fmtw = 4
            elif size == 4:
                (value,) = struct.unpack_from("<I", chunk, 0)
                fmtw = 8
            else:
                raise SystemExit(f"scalar size must be 1/2/4: {entry}")
            lines.append(f"#define HOSTDATA_{label} 0x{value:0{fmtw}X}u")
            lines.append(fmt_bytes(label, chunk))
            lines.append("")
            n_emitted += 1
        elif kind == "blob":
            _, ram_addr, size, label = entry
            chunk = read_chunk(ram_addr, size, label)
            lines.append(f"/* {label}: blob ram=0x{ram_addr:08X} size={size} */")
            lines.append(fmt_bytes(label, chunk))
            lines.append("")
            n_emitted += 1
        elif kind == "ptr_blob":
            _, ptr_addr, blob_size, label = entry
            ptr_chunk = read_chunk(ptr_addr, 4, label + "_PTR")
            (target,) = struct.unpack_from("<I", ptr_chunk, 0)
            chunk = read_chunk(target, blob_size, label)
            lines.append(
                f"/* {label}: ptr@0x{ptr_addr:08X} -> 0x{target:08X} "
                f"({blob_size} bytes) */")
            lines.append(f"#define HOSTDATA_{label}_PTR 0x{target:08X}u")
            lines.append(fmt_bytes(label, chunk))
            lines.append("")
            n_emitted += 1
        else:
            raise SystemExit(f"unknown slot kind: {entry}")

    lines.append("#endif /* HOST_DATA_INIT_GENERATED_H */")
    lines.append("")

    with open(out_path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))

    print(f"[extract_data_inits] wrote {out_path} "
          f"({n_emitted} slots from {arm9_path})")


def main(argv):
    if len(argv) != 3:
        print(__doc__)
        return 2
    emit_header(argv[1], argv[2])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
