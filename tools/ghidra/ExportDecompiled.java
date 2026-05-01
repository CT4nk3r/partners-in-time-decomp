// Ghidra script: Export decompiled C pseudocode for all functions
//
// @category NDS
// @author MLPiT Decomp

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.*;
import java.util.*;

public class ExportDecompiled extends GhidraScript {

    @Override
    public void run() throws Exception {
        String outputDir = System.getenv("MLPIT_OUTPUT_DIR");
        if (outputDir == null || outputDir.isEmpty()) {
            outputDir = ".";
        }

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator iter = fm.getFunctions(true);

        String decompDir = outputDir + File.separator + "decompiled";
        new File(decompDir).mkdirs();

        // Export individual function files
        int count = 0;
        int failed = 0;
        PrintWriter allWriter = new PrintWriter(new FileWriter(decompDir + File.separator + "all_functions.c"));
        allWriter.println("// Auto-generated Ghidra decompilation of ARM9");
        allWriter.println("// Mario & Luigi: Partners in Time (USA Rev 0)");
        allWriter.println("// THIS IS PSEUDOCODE — needs cleanup for matching compilation");
        allWriter.println("");
        allWriter.println("#include \"types.h\"");
        allWriter.println("");

        while (iter.hasNext()) {
            Function f = iter.next();
            DecompileResults results = decomp.decompileFunction(f, 30, getMonitor());

            if (results.getDecompiledFunction() != null) {
                String code = results.getDecompiledFunction().getC();

                // Write individual file
                String funcFile = decompDir + File.separator + f.getName() + ".c";
                PrintWriter fw = new PrintWriter(new FileWriter(funcFile));
                fw.println("// " + f.getName() + " @ " + f.getEntryPoint().toString());
                fw.println("// Size: " + f.getBody().getNumAddresses() + " bytes");
                fw.println("");
                fw.println(code);
                fw.close();

                // Append to combined file
                allWriter.println("// --------------------------------------------------------");
                allWriter.println("// " + f.getName() + " @ " + f.getEntryPoint());
                allWriter.println("// --------------------------------------------------------");
                allWriter.println(code);
                allWriter.println("");

                count++;
            } else {
                failed++;
            }
        }

        allWriter.close();
        decomp.dispose();

        println("");
        println("==================================================");
        println("Decompilation Export Summary");
        println("==================================================");
        println("  Functions decompiled: " + count);
        println("  Failed:              " + failed);
        println("  Output dir:          " + decompDir);
        println("  Combined file:       " + decompDir + File.separator + "all_functions.c");
        println("==================================================");
    }
}
