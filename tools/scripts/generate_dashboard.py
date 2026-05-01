"""
Dashboard Generator — Mario & Luigi: Partners in Time Decomp Progress

Reads project data (functions.csv, source files) and generates a static
HTML dashboard suitable for GitHub Pages deployment.

Usage: python tools/scripts/generate_dashboard.py
Output: docs/index.html
"""

import csv
import os
import re
import json
from datetime import datetime
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent
FUNCTIONS_CSV = PROJECT_ROOT / "config" / "functions.csv"
SRC_DIR = PROJECT_ROOT / "arm9" / "src"
ASM_FUNC_DIR = PROJECT_ROOT / "arm9" / "asm" / "func"
ASM_GAP_DIR = PROJECT_ROOT / "arm9" / "asm" / "gaps"
KNOWN_SYMBOLS = PROJECT_ROOT / "config" / "known_symbols.txt"
OUTPUT_DIR = PROJECT_ROOT / "docs"


def parse_functions_csv():
    """Parse the function map CSV."""
    functions = []
    with open(FUNCTIONS_CSV, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            functions.append({
                "address": row["address"],
                "size": int(row["size"]),
                "name": row["name"],
                "is_thunk": row["is_thunk"] == "true",
                "param_count": int(row["param_count"]),
            })
    return functions


def scan_decompiled_functions():
    """Scan arm9/src/*.c files for decompiled function addresses.

    Uses a broad approach: any FUN_XXXXXXXX reference in a source file
    counts as decompiled, since the function body exists in that file
    (either as a definition, comment marker, or thunk reference).
    Results are cross-matched against functions.csv to avoid false positives
    from extern declarations referencing functions in OTHER files.
    """
    decompiled = {}
    if not SRC_DIR.exists():
        return decompiled

    # First pass: collect all FUN_ addresses mentioned in each file
    file_addrs = {}
    for c_file in SRC_DIR.glob("*.c"):
        content = c_file.read_text(encoding="utf-8", errors="replace")
        addrs = set()

        # Match comment markers: FUN_XXXXXXXX @ 0xXXXXXXXX (any format)
        for m in re.finditer(r"FUN_([0-9a-fA-F]{8})\s*@\s*0x([0-9a-fA-F]+)", content):
            addrs.add(m.group(2).lower())

        # Match thunk comment markers: thunk_FUN_... @ 0xXXXXXXXX
        for m in re.finditer(r"thunk_FUN_[0-9a-fA-F]+\s*@\s*0x([0-9a-fA-F]+)", content):
            addrs.add(m.group(1).lower())

        # Match function definitions: returntype FUN_XXXXXXXX(
        for m in re.finditer(
            r"(?:^|\n)\s*(?:static\s+)?(?:void|int|u32|u16|u8|s32|s16|s8|BOOL|u64|s64)\s+\*?\s*FUN_([0-9a-fA-F]{8})\s*\(",
            content,
        ):
            addrs.add(m.group(1).lower())

        file_addrs[c_file.name] = addrs

    # Second pass: for each address, find the file that DEFINES it
    all_addrs = set()
    for fname, addrs in file_addrs.items():
        for addr in addrs:
            if addr not in all_addrs:
                decompiled[addr] = {"file": fname, "size": 0}
                all_addrs.add(addr)

    # Third pass: catch ANY FUN_XXXXXXXX reference (broadest match)
    for c_file in SRC_DIR.glob("*.c"):
        content = c_file.read_text(encoding="utf-8", errors="replace")
        for m in re.finditer(r"FUN_([0-9a-fA-F]{8})", content):
            addr = m.group(1).lower()
            if addr not in decompiled:
                decompiled[addr] = {"file": c_file.name, "size": 0}

    return decompiled


def parse_known_symbols():
    """Parse known_symbols.txt for identified function names."""
    symbols = {}
    if not KNOWN_SYMBOLS.exists():
        return symbols

    content = KNOWN_SYMBOLS.read_text(encoding="utf-8", errors="replace")
    # Match: [type] name @ 0xADDRESS
    for match in re.finditer(
        r"\[(\w+)\]\s+(\S+)\s+@\s*0x([0-9a-fA-F]+)", content
    ):
        tag, name, addr = match.groups()
        symbols[addr.lower()] = {"name": name, "tag": tag}

    return symbols


def categorize_functions(functions):
    """Categorize functions by size."""
    categories = {
        "tiny": {"label": "Tiny (<32B)", "min": 0, "max": 31, "count": 0, "bytes": 0},
        "small": {"label": "Small (32-128B)", "min": 32, "max": 128, "count": 0, "bytes": 0},
        "medium": {"label": "Medium (128-512B)", "min": 129, "max": 512, "count": 0, "bytes": 0},
        "large": {"label": "Large (512-2KB)", "min": 513, "max": 2048, "count": 0, "bytes": 0},
        "huge": {"label": "Huge (>2KB)", "min": 2049, "max": 999999, "count": 0, "bytes": 0},
    }

    for func in functions:
        size = func["size"]
        for cat in categories.values():
            if cat["min"] <= size <= cat["max"]:
                cat["count"] += 1
                cat["bytes"] += size
                break

    return categories


def get_source_file_stats():
    """Get statistics about decompiled source files."""
    files = []
    if not SRC_DIR.exists():
        return files

    for c_file in sorted(SRC_DIR.glob("*.c")):
        content = c_file.read_text(encoding="utf-8", errors="replace")
        func_count = len(re.findall(r"FUN_[0-9a-fA-F]{8}\s*@", content))
        line_count = len(content.splitlines())
        files.append({
            "name": c_file.name,
            "functions": func_count,
            "lines": line_count,
            "size": c_file.stat().st_size,
        })

    return files


def generate_html(functions, decompiled, symbols, categories, source_files):
    """Generate the dashboard HTML."""
    total_funcs = len(functions)
    decomp_count = len(decompiled)
    total_bytes = sum(f["size"] for f in functions)
    decomp_bytes = sum(
        f["size"] for f in functions if f["address"].lower() in decompiled
    )
    pct_funcs = (decomp_count / total_funcs * 100) if total_funcs else 0
    pct_bytes = (decomp_bytes / total_bytes * 100) if total_bytes else 0
    identified_count = len(symbols)

    # Per-category decompilation stats
    cat_decomp = {}
    for func in functions:
        size = func["size"]
        addr = func["address"].lower()
        is_decomp = addr in decompiled
        for key, cat in categories.items():
            if cat["min"] <= size <= cat["max"]:
                if key not in cat_decomp:
                    cat_decomp[key] = {"done": 0, "total": cat["count"]}
                if is_decomp:
                    cat_decomp[key]["done"] += 1
                break

    # Recent decompiled functions table
    decomp_list = []
    for func in functions:
        addr = func["address"].lower()
        if addr in decompiled:
            name = func["name"]
            if addr in symbols:
                name = symbols[addr]["name"]
            decomp_list.append({
                "address": f"0x{addr.upper()}",
                "name": name,
                "size": func["size"],
                "file": decompiled[addr]["file"],
            })

    timestamp = datetime.now(tz=__import__('datetime').timezone.utc).strftime("%Y-%m-%d %H:%M UTC")

    # Build category rows
    cat_rows = ""
    for key in ["tiny", "small", "medium", "large", "huge"]:
        cat = categories[key]
        cd = cat_decomp.get(key, {"done": 0, "total": cat["count"]})
        pct = (cd["done"] / cd["total"] * 100) if cd["total"] else 0
        cat_rows += f"""
            <tr>
                <td>{cat['label']}</td>
                <td>{cd['done']}</td>
                <td>{cd['total']}</td>
                <td>
                    <div class="progress-bar-small">
                        <div class="progress-fill" style="width: {pct:.1f}%"></div>
                    </div>
                    <span class="pct">{pct:.1f}%</span>
                </td>
            </tr>"""

    # Build decompiled functions table
    func_rows = ""
    for entry in sorted(decomp_list, key=lambda x: x["address"]):
        func_rows += f"""
            <tr>
                <td class="mono">{entry['address']}</td>
                <td>{entry['name']}</td>
                <td>{entry['size']}B</td>
                <td class="mono">{entry['file']}</td>
            </tr>"""

    # Build source files table
    src_rows = ""
    for sf in source_files:
        src_rows += f"""
            <tr>
                <td class="mono">{sf['name']}</td>
                <td>{sf['functions']}</td>
                <td>{sf['lines']}</td>
            </tr>"""

    # Chart data
    chart_data = json.dumps({
        "decompiledFuncs": decomp_count,
        "remainingFuncs": total_funcs - decomp_count,
        "decompiledBytes": decomp_bytes,
        "remainingBytes": total_bytes - decomp_bytes,
        "categories": {
            key: {
                "label": categories[key]["label"],
                "done": cat_decomp.get(key, {"done": 0})["done"],
                "total": categories[key]["count"],
            }
            for key in ["tiny", "small", "medium", "large", "huge"]
        },
    })

    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>M&amp;L: Partners in Time — Decomp Progress</title>
    <style>
        :root {{
            --bg: #0d1117;
            --card: #161b22;
            --border: #30363d;
            --text: #e6edf3;
            --text-dim: #8b949e;
            --accent: #58a6ff;
            --green: #3fb950;
            --orange: #d29922;
            --red: #f85149;
            --purple: #bc8cff;
        }}
        * {{ margin: 0; padding: 0; box-sizing: border-box; }}
        body {{
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif;
            background: var(--bg);
            color: var(--text);
            line-height: 1.5;
            padding: 20px;
        }}
        .container {{ max-width: 1200px; margin: 0 auto; }}
        h1 {{
            font-size: 1.8rem;
            margin-bottom: 4px;
            display: flex;
            align-items: center;
            gap: 12px;
        }}
        h1 .emoji {{ font-size: 2rem; }}
        .subtitle {{ color: var(--text-dim); margin-bottom: 24px; font-size: 0.9rem; }}
        .grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(260px, 1fr));
            gap: 16px;
            margin-bottom: 24px;
        }}
        .card {{
            background: var(--card);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 20px;
        }}
        .card h2 {{
            font-size: 0.85rem;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            color: var(--text-dim);
            margin-bottom: 8px;
        }}
        .stat {{
            font-size: 2.4rem;
            font-weight: 700;
            line-height: 1.2;
        }}
        .stat .unit {{ font-size: 1rem; color: var(--text-dim); font-weight: 400; }}
        .stat.green {{ color: var(--green); }}
        .stat.blue {{ color: var(--accent); }}
        .stat.orange {{ color: var(--orange); }}
        .stat.purple {{ color: var(--purple); }}
        .progress-bar {{
            width: 100%;
            height: 24px;
            background: #21262d;
            border-radius: 12px;
            overflow: hidden;
            margin: 12px 0 6px;
        }}
        .progress-bar-small {{
            display: inline-block;
            width: 100px;
            height: 12px;
            background: #21262d;
            border-radius: 6px;
            overflow: hidden;
            vertical-align: middle;
        }}
        .progress-fill {{
            height: 100%;
            background: linear-gradient(90deg, var(--green), var(--accent));
            border-radius: 12px;
            transition: width 0.5s ease;
        }}
        .progress-label {{
            display: flex;
            justify-content: space-between;
            font-size: 0.8rem;
            color: var(--text-dim);
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
            font-size: 0.85rem;
        }}
        th {{
            text-align: left;
            padding: 8px 12px;
            border-bottom: 2px solid var(--border);
            color: var(--text-dim);
            font-weight: 600;
            font-size: 0.75rem;
            text-transform: uppercase;
            letter-spacing: 0.05em;
        }}
        td {{
            padding: 6px 12px;
            border-bottom: 1px solid var(--border);
        }}
        tr:hover {{ background: rgba(88, 166, 255, 0.05); }}
        .mono {{ font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; font-size: 0.8rem; }}
        .pct {{ font-size: 0.8rem; color: var(--text-dim); margin-left: 8px; }}
        .section {{ margin-bottom: 24px; }}
        .section h2 {{
            font-size: 1.1rem;
            margin-bottom: 12px;
            padding-bottom: 8px;
            border-bottom: 1px solid var(--border);
        }}
        .legal {{
            background: #1c1206;
            border: 1px solid #6e4b00;
            border-radius: 8px;
            padding: 16px;
            margin-bottom: 24px;
            font-size: 0.85rem;
            color: var(--orange);
        }}
        .legal strong {{ color: #f0c000; }}
        .charts {{ display: grid; grid-template-columns: 1fr 1fr; gap: 16px; margin-bottom: 24px; }}
        canvas {{ max-width: 100%; }}
        .footer {{
            text-align: center;
            color: var(--text-dim);
            font-size: 0.75rem;
            margin-top: 40px;
            padding-top: 20px;
            border-top: 1px solid var(--border);
        }}
        @media (max-width: 768px) {{
            .charts {{ grid-template-columns: 1fr; }}
            .grid {{ grid-template-columns: 1fr 1fr; }}
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1><span class="emoji">🍄</span> Mario &amp; Luigi: Partners in Time</h1>
        <p class="subtitle">NDS Matching Decompilation Progress — Updated {timestamp}</p>

        <div class="legal">
            <strong>⚠️ Legal Notice:</strong> This is a clean-room decompilation project.
            No copyrighted game assets are included. You must own a legal copy of the game ROM.
        </div>

        <div class="grid">
            <div class="card">
                <h2>Functions Decompiled</h2>
                <div class="stat green">{decomp_count} <span class="unit">/ {total_funcs}</span></div>
                <div class="progress-bar">
                    <div class="progress-fill" style="width: {pct_funcs:.1f}%"></div>
                </div>
                <div class="progress-label">
                    <span>{pct_funcs:.1f}% complete</span>
                    <span>{total_funcs - decomp_count} remaining</span>
                </div>
            </div>
            <div class="card">
                <h2>Bytes Decompiled</h2>
                <div class="stat blue">{decomp_bytes:,} <span class="unit">/ {total_bytes:,} B</span></div>
                <div class="progress-bar">
                    <div class="progress-fill" style="width: {pct_bytes:.1f}%"></div>
                </div>
                <div class="progress-label">
                    <span>{pct_bytes:.1f}% of code</span>
                    <span>{total_bytes - decomp_bytes:,}B remaining</span>
                </div>
            </div>
            <div class="card">
                <h2>Source Files</h2>
                <div class="stat purple">{len(source_files)}</div>
                <p style="color: var(--text-dim); font-size: 0.85rem; margin-top: 4px;">
                    {sum(sf['lines'] for sf in source_files):,} lines of C code
                </p>
            </div>
            <div class="card">
                <h2>Symbols Identified</h2>
                <div class="stat orange">{identified_count}</div>
                <p style="color: var(--text-dim); font-size: 0.85rem; margin-top: 4px;">
                    Named functions &amp; data labels
                </p>
            </div>
        </div>

        <div class="charts">
            <div class="card">
                <h2>By Function Count</h2>
                <canvas id="chartFuncs" height="200"></canvas>
            </div>
            <div class="card">
                <h2>By Code Size</h2>
                <canvas id="chartBytes" height="200"></canvas>
            </div>
        </div>

        <div class="section card">
            <h2>Progress by Size Category</h2>
            <table>
                <thead>
                    <tr><th>Category</th><th>Done</th><th>Total</th><th>Progress</th></tr>
                </thead>
                <tbody>{cat_rows}
                </tbody>
            </table>
        </div>

        <div class="section card">
            <h2>Source Files</h2>
            <table>
                <thead>
                    <tr><th>File</th><th>Functions</th><th>Lines</th></tr>
                </thead>
                <tbody>{src_rows}
                </tbody>
            </table>
        </div>

        <div class="section card">
            <h2>Decompiled Functions ({decomp_count})</h2>
            <table>
                <thead>
                    <tr><th>Address</th><th>Name</th><th>Size</th><th>Source File</th></tr>
                </thead>
                <tbody>{func_rows}
                </tbody>
            </table>
        </div>

        <div class="footer">
            Mario &amp; Luigi: Partners in Time NDS Decompilation Project<br>
            Generated by <code>tools/scripts/generate_dashboard.py</code>
        </div>
    </div>

    <script>
    const data = {chart_data};

    function drawDonut(canvasId, done, total, doneLabel, remainLabel) {{
        const canvas = document.getElementById(canvasId);
        const ctx = canvas.getContext('2d');
        const w = canvas.width = canvas.offsetWidth * 2;
        const h = canvas.height = canvas.offsetHeight * 2;
        ctx.scale(2, 2);
        const cw = w / 2, ch = h / 2;
        const cx = cw / 2, cy = ch / 2;
        const r = Math.min(cx, cy) - 20;
        const lw = 28;

        const pct = total > 0 ? done / total : 0;
        const startAngle = -Math.PI / 2;
        const doneAngle = startAngle + pct * 2 * Math.PI;

        // Remaining arc
        ctx.beginPath();
        ctx.arc(cx, cy, r, doneAngle, startAngle + 2 * Math.PI);
        ctx.strokeStyle = '#21262d';
        ctx.lineWidth = lw;
        ctx.stroke();

        // Done arc
        if (pct > 0) {{
            ctx.beginPath();
            ctx.arc(cx, cy, r, startAngle, doneAngle);
            const grad = ctx.createLinearGradient(0, 0, cw, ch);
            grad.addColorStop(0, '#3fb950');
            grad.addColorStop(1, '#58a6ff');
            ctx.strokeStyle = grad;
            ctx.lineWidth = lw;
            ctx.lineCap = 'round';
            ctx.stroke();
        }}

        // Center text
        ctx.fillStyle = '#e6edf3';
        ctx.font = 'bold 22px -apple-system, sans-serif';
        ctx.textAlign = 'center';
        ctx.fillText((pct * 100).toFixed(1) + '%', cx, cy - 4);
        ctx.font = '11px -apple-system, sans-serif';
        ctx.fillStyle = '#8b949e';
        ctx.fillText(done.toLocaleString() + ' / ' + total.toLocaleString(), cx, cy + 16);
    }}

    drawDonut('chartFuncs', data.decompiledFuncs, data.decompiledFuncs + data.remainingFuncs, 'Decompiled', 'Remaining');
    drawDonut('chartBytes', data.decompiledBytes, data.decompiledBytes + data.remainingBytes, 'Decompiled', 'Remaining');
    </script>
</body>
</html>"""

    return html


def main():
    print("Generating decomp progress dashboard...")

    functions = parse_functions_csv()
    decompiled = scan_decompiled_functions()
    symbols = parse_known_symbols()
    categories = categorize_functions(functions)
    source_files = get_source_file_stats()

    print(f"  Functions in CSV: {len(functions)}")
    print(f"  Decompiled: {len(decompiled)}")
    print(f"  Symbols identified: {len(symbols)}")
    print(f"  Source files: {len(source_files)}")

    html = generate_html(functions, decompiled, symbols, categories, source_files)

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    output_file = OUTPUT_DIR / "index.html"
    output_file.write_text(html, encoding="utf-8")
    print(f"  Dashboard written to: {output_file}")


if __name__ == "__main__":
    main()
