#!/usr/bin/env python3
"""
sweep_boot_triples.py — Headlessly run mlpit_pc.exe for every boot-screen
triple in boot_screen_combos.txt, score the resulting PPM, and report the
top candidates.

Scoring is "scene-likeness": we want triples that look like a real game
background (large color clusters, gradients, multi-bank palette use)
rather than the default font/text atlas (single bank, regular grid).

Usage:
    python tools/scripts/sweep_boot_triples.py \
        --combos C:\\path\\to\\boot_screen_combos.txt \
        --outdir C:\\path\\to\\session_files \
        [--frame 90] [--exit 120] [--limit N] [--keep-tmp]
"""
from __future__ import annotations

import argparse
import os
import shutil
import struct
import subprocess
import sys
from collections import Counter
from pathlib import Path

REPO    = Path(__file__).resolve().parents[2]
EXE     = REPO / "pc" / "build" / "mlpit_pc.exe"
SWEEP_DIR = REPO / ".sweep_tmp"
TOP_W   = 256
TOP_H   = 192
SCREEN_H_TOTAL = 384   # top + bottom stacked


# ---------- PPM loader ----------------------------------------------------

def load_ppm(path: Path):
    """Returns (width, height, bytes) for a P6 PPM, or None on failure."""
    try:
        with open(path, "rb") as f:
            magic = f.readline().strip()
            if magic != b"P6":
                return None
            # skip comments
            while True:
                line = f.readline()
                if not line:
                    return None
                if line.startswith(b"#"):
                    continue
                break
            w, h = map(int, line.split())
            maxv = int(f.readline().strip())
            if maxv != 255:
                return None
            data = f.read(w * h * 3)
            if len(data) != w * h * 3:
                return None
            return (w, h, data)
    except Exception as e:
        print(f"  [load_ppm] {path}: {e}", file=sys.stderr)
        return None


# ---------- Scoring helpers ----------------------------------------------

def split_screens(w: int, h: int, data: bytes):
    """Yield (label, w, h, bytes) for top and bottom screens."""
    if h >= 2 * TOP_H:
        row_bytes = w * 3
        top = data[:TOP_H * row_bytes]
        bot = data[TOP_H * row_bytes : 2 * TOP_H * row_bytes]
        yield ("top", w, TOP_H, top)
        yield ("bot", w, TOP_H, bot)
    else:
        yield ("img", w, h, data)


def color_histogram(pixels: bytes):
    """Iterate as 3-byte tuples and return Counter of RGB."""
    c = Counter()
    n = len(pixels) // 3
    for i in range(n):
        b = pixels[i*3:(i+1)*3]
        c[bytes(b)] += 1
    return c


def stddev(values):
    if not values:
        return 0.0
    m = sum(values) / len(values)
    var = sum((v - m) ** 2 for v in values) / len(values)
    return var ** 0.5


def edge_density(w: int, h: int, pixels: bytes):
    """Fraction of horizontally-adjacent pixels that differ.

    Real bg art has SOME edges (silhouettes, sprites) but not
    everywhere — pure noise scores 1.0; a flat color scores 0.0;
    a font atlas tends ~0.4-0.5 (grid).
    """
    differ = 0
    total  = 0
    for y in range(h):
        row = pixels[y*w*3:(y+1)*w*3]
        for x in range(w - 1):
            a = row[x*3:x*3+3]
            b = row[(x+1)*3:(x+1)*3+3]
            if a != b:
                differ += 1
            total += 1
    return differ / max(total, 1)


def regularity_penalty(w: int, h: int, pixels: bytes):
    """Detect tile-grid regularity (font atlas).

    For each 8x8 tile, hash its first row's color set. If many tiles
    share the same row-color signature, the screen is repetitive.
    Returns a value in [0,1] where 1 = highly regular.
    """
    sigs = Counter()
    tiles_x = w // 8
    tiles_y = h // 8
    for ty in range(tiles_y):
        for tx in range(tiles_x):
            row = pixels[(ty*8)*w*3 + tx*8*3 : (ty*8)*w*3 + (tx*8 + 8)*3]
            sigs[bytes(row)] += 1
    if not sigs:
        return 0.0
    total_tiles = tiles_x * tiles_y
    top_sig = max(sigs.values())
    return top_sig / total_tiles


def palette_bank_dominance(hist: Counter):
    """Approx: how concentrated are colors?

    We don't know the actual palette banks from the rendered image,
    but if >80% of pixels share <=16 unique colors, that's effectively
    one-bank dominance. Returns fraction of pixels in top-16 colors.
    """
    if not hist:
        return 0.0
    total = sum(hist.values())
    top16 = sum(c for _, c in hist.most_common(16))
    return top16 / total


def score_screen(w, h, pixels):
    """Return (composite_score, breakdown_dict)."""
    hist = color_histogram(pixels)
    unique = len(hist)
    bank_dom = palette_bank_dominance(hist)
    edge = edge_density(w, h, pixels)
    reg  = regularity_penalty(w, h, pixels)

    # Per-channel stddev — gradients & varied art bump this.
    rs = [pixels[i] for i in range(0, len(pixels), 3)]
    gs = [pixels[i] for i in range(1, len(pixels), 3)]
    bs = [pixels[i] for i in range(2, len(pixels), 3)]
    sd = (stddev(rs) + stddev(gs) + stddev(bs)) / 3.0

    # Composite:
    #   reward unique colors (capped at 256), color stddev (~0..80),
    #     moderate edges (peak around 0.25 — gradients/sprites)
    #   penalize bank dominance and tile regularity
    unique_score = min(unique, 256) / 256.0          # 0..1
    sd_score     = min(sd, 80.0) / 80.0              # 0..1
    edge_score   = 1.0 - abs(edge - 0.25) * 2.0      # peak 1.0 at 0.25
    edge_score   = max(0.0, edge_score)
    bank_pen     = max(0.0, bank_dom - 0.5) * 2.0    # 0 if <=0.5
    reg_pen      = reg                                # already 0..1

    composite = (
        2.0 * unique_score +
        2.0 * sd_score     +
        1.5 * edge_score   -
        2.5 * bank_pen     -
        2.0 * reg_pen
    )
    return composite, {
        "unique": unique,
        "stddev": round(sd, 2),
        "edge":   round(edge, 3),
        "regularity": round(reg, 3),
        "bank_dom": round(bank_dom, 3),
    }


def score_ppm(path: Path):
    img = load_ppm(path)
    if img is None:
        return None
    w, h, data = img
    parts = []
    total = 0.0
    for label, sw, sh, sd in split_screens(w, h, data):
        s, br = score_screen(sw, sh, sd)
        parts.append((label, s, br))
        total += s
    return total, parts


# ---------- Runner --------------------------------------------------------

def parse_combos(path: Path):
    out = []
    for line in path.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("fatHex") or set(line) <= {"-"}:
            continue
        toks = line.split()
        # last token is the env value fat:tile:map:pal
        env = toks[-1]
        if env.count(":") != 3:
            continue
        out.append(env)
    return out


def run_one(env_value: str, frame: int, exit_at: int, ppm_path: Path,
            log_path: Path, timeout: int = 25) -> bool:
    env = os.environ.copy()
    env["MLPIT_BOOT_TRIPLE"]    = env_value
    env["MLPIT_SCREENSHOT_FRAME"] = str(frame)
    env["MLPIT_SCREENSHOT_PATH"]  = str(ppm_path)
    env["MLPIT_EXIT_AT_FRAME"]    = str(exit_at)
    # Ensure deterministic / no stdin
    try:
        with open(log_path, "wb") as logf:
            r = subprocess.run(
                [str(EXE)],
                env=env,
                stdout=logf, stderr=subprocess.STDOUT,
                stdin=subprocess.DEVNULL,
                timeout=timeout,
            )
        return ppm_path.exists() and ppm_path.stat().st_size > 100
    except subprocess.TimeoutExpired:
        return ppm_path.exists() and ppm_path.stat().st_size > 100
    except Exception as e:
        print(f"  [run_one] {env_value}: {e}", file=sys.stderr)
        return False


# ---------- PPM -> PNG ----------------------------------------------------

def ppm_to_png(ppm: Path, png: Path) -> bool:
    try:
        from PIL import Image
        img = Image.open(ppm)
        img.save(png, "PNG")
        return True
    except Exception as e:
        print(f"  [ppm_to_png] {e}", file=sys.stderr)
        return False


# ---------- Main ----------------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--combos", required=True)
    ap.add_argument("--outdir", required=True)
    ap.add_argument("--frame",  type=int, default=90)
    ap.add_argument("--exit",   type=int, default=120, dest="exit_at")
    ap.add_argument("--timeout", type=int, default=25)
    ap.add_argument("--limit",  type=int, default=0,
                    help="If >0, only run first N triples")
    ap.add_argument("--keep-tmp", action="store_true")
    args = ap.parse_args()

    if not EXE.exists():
        print(f"ERROR: {EXE} missing — build first", file=sys.stderr)
        sys.exit(1)
    combos_path = Path(args.combos)
    outdir = Path(args.outdir)
    outdir.mkdir(parents=True, exist_ok=True)
    SWEEP_DIR.mkdir(exist_ok=True)

    triples = parse_combos(combos_path)
    if args.limit > 0:
        triples = triples[: args.limit]
    print(f"[sweep] {len(triples)} triples to test")

    results = []   # (score, env_value, breakdown_list, ppm_path)
    for i, env in enumerate(triples, 1):
        safe = env.replace(":", "_")
        ppm  = SWEEP_DIR / f"shot_{safe}.ppm"
        log  = SWEEP_DIR / f"log_{safe}.txt"
        if ppm.exists():
            ppm.unlink()
        ok = run_one(env, args.frame, args.exit_at, ppm, log,
                     timeout=args.timeout)
        if not ok:
            print(f"  [{i:2d}/{len(triples)}] {env:30s} FAIL")
            continue
        s = score_ppm(ppm)
        if s is None:
            print(f"  [{i:2d}/{len(triples)}] {env:30s} SCORE_FAIL")
            continue
        score, parts = s
        print(f"  [{i:2d}/{len(triples)}] {env:30s} score={score:6.3f}  "
              + " ".join(f"{lab}:{b['unique']}u/{b['regularity']}r/{b['bank_dom']}d"
                         for lab, _, b in parts))
        results.append((score, env, parts, ppm))

    if not results:
        print("[sweep] no successful runs", file=sys.stderr)
        sys.exit(2)

    results.sort(key=lambda r: r[0], reverse=True)
    print("\n=== Top 10 ranked ===")
    print(f"{'rank':>4}  {'score':>7}  triple                  details")
    for r, (sc, env, parts, _) in enumerate(results[:10], 1):
        print(f"{r:>4}  {sc:7.3f}  {env:22s}  "
              + " | ".join(f"{lab} u={b['unique']} sd={b['stddev']} "
                          f"edge={b['edge']} reg={b['regularity']} "
                          f"dom={b['bank_dom']}"
                          for lab, _, b in parts))

    # Save top-3 PNGs
    for r, (sc, env, _, ppm) in enumerate(results[:3], 1):
        dst = outdir / f"triple_top{r}.png"
        if ppm_to_png(ppm, dst):
            print(f"  saved {dst}  ({env}, score={sc:.3f})")

    # Write ranking text
    rank_path = outdir / "triple_sweep_results.txt"
    with rank_path.open("w") as f:
        f.write(f"# Boot-triple sweep — {len(results)}/{len(triples)} successful\n")
        f.write(f"# frame={args.frame} exit_at={args.exit_at}\n\n")
        f.write(f"{'rank':>4}  {'score':>7}  triple                  details\n")
        for r, (sc, env, parts, _) in enumerate(results, 1):
            line = (f"{r:>4}  {sc:7.3f}  {env:22s}  "
                    + " | ".join(f"{lab} u={b['unique']} sd={b['stddev']} "
                                f"edge={b['edge']} reg={b['regularity']} "
                                f"dom={b['bank_dom']}"
                                for lab, _, b in parts))
            f.write(line + "\n")
    print(f"\n[sweep] wrote {rank_path}")

    if not args.keep_tmp:
        shutil.rmtree(SWEEP_DIR, ignore_errors=True)


if __name__ == "__main__":
    main()
