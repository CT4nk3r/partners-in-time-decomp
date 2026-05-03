# Ghidra headless post-analysis script
# Exports function list, entry points, and basic stats
#
# @category NDS
# @author MLPiT Decomp

import json
import os

def run():
    program = currentProgram
    listing = program.getListing()
    fm = program.getFunctionManager()
    mem = program.getMemory()

    output_dir = os.environ.get("MLPIT_OUTPUT_DIR", ".")

    # Collect all functions
    functions = []
    func_iter = fm.getFunctions(True)
    while func_iter.hasNext():
        f = func_iter.next()
        body = f.getBody()
        size = 0
        for rng in body:
            size += rng.getLength()
        functions.append({
            "name": f.getName(),
            "address": str(f.getEntryPoint()),
            "size": size,
            "is_thunk": f.isThunk(),
            "calling_convention": str(f.getCallingConventionName()),
            "param_count": f.getParameterCount(),
        })

    # Sort by address
    functions.sort(key=lambda x: x["address"])

    # Write function list as JSON
    json_path = os.path.join(output_dir, "functions.json")
    with open(json_path, "w") as fp:
        json.dump(functions, fp, indent=2)

    # Write human-readable function map
    txt_path = os.path.join(output_dir, "functions.txt")
    with open(txt_path, "w") as fp:
        fp.write("# Auto-generated function map\n")
        fp.write("# Address      Size  Name\n")
        fp.write("#" + "-" * 70 + "\n")
        for f in functions:
            fp.write("  %-12s %6d  %s\n" % (f["address"], f["size"], f["name"]))
        fp.write("\n# Total functions: %d\n" % len(functions))

    # Summary stats
    total_code_bytes = sum(f["size"] for f in functions)
    print("")
    print("=" * 50)
    print("Analysis Export Summary")
    print("=" * 50)
    print("  Functions found:    %d" % len(functions))
    print("  Total code bytes:   %d" % total_code_bytes)
    print("  Thunk functions:    %d" % sum(1 for f in functions if f["is_thunk"]))
    print("  Output:             %s" % json_path)
    print("  Output:             %s" % txt_path)
    print("=" * 50)

run()
