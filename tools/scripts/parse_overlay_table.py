#!/usr/bin/env python3
"""parse_overlay_table.py — emit a JSON description of every NDS overlay.

Reads `extracted/y9.bin` (the ARM9 overlay table; 32 bytes per entry)
and emits a JSON file describing every overlay:

    {
        "overlays": [
            { "id": 0, "ram": "0x020659C0", "ram_int": 33675200,
              "size": 368672, "bss": 32,
              "sinit_init": "0x020BF0BC", "sinit_init_end": "0x020BF0C0",
              "file_id": 0, "compressed": false, "ram_end": "0x020BF0C0",
              "file": "extracted/overlay/overlay_0000.bin",
              "file_size": 368672 },
            ...
        ]
    }

Per ndstool docs the 8th word ("reserved") encodes:
    bits  0..23 : overlay flags (compressed, etc.)
    bits 24..31 : "0xff" sentinel on patched binaries (we ignore)
The compressed bit is bit 0 of byte 24 (LSB of the reserved word).
For PiT every reserved word is 0 → no overlays are LZ77-compressed.
"""
import json
import os
import struct
import sys


def parse(y9_path, overlay_dir):
    data = open(y9_path, "rb").read()
    n = len(data) // 32
    overlays = []
    for i in range(n):
        (ovid, ram, size, bss, si, sie, fid, resv) = struct.unpack_from(
            "<IIIIIIII", data, i * 32
        )
        compressed = bool(resv & 1)
        # ndstool also stores a separate compressed_size in resv >> 8 (?)
        # but PiT has resv == 0 for everything, so don't bother.
        ovfile = os.path.join(overlay_dir, f"overlay_{ovid:04d}.bin")
        ovfile_size = os.path.getsize(ovfile) if os.path.isfile(ovfile) else 0
        overlays.append({
            "id": ovid,
            "ram": f"0x{ram:08X}",
            "ram_int": ram,
            "size": size,
            "bss": bss,
            "sinit_init": f"0x{si:08X}",
            "sinit_init_end": f"0x{sie:08X}",
            "file_id": fid,
            "reserved": resv,
            "compressed": compressed,
            "ram_end": f"0x{ram + size:08X}",
            "file": ovfile.replace("\\", "/"),
            "file_size": ovfile_size,
        })
    return overlays


def main(argv):
    y9_path = argv[1] if len(argv) > 1 else "extracted/y9.bin"
    overlay_dir = argv[2] if len(argv) > 2 else "extracted/overlay"
    out_path = argv[3] if len(argv) > 3 else ".session-work/overlay_table.json"
    overlays = parse(y9_path, overlay_dir)
    os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)
    with open(out_path, "w", encoding="utf-8", newline="\n") as fh:
        json.dump({"overlays": overlays}, fh, indent=2)
    print(f"wrote {out_path} ({len(overlays)} overlays)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
