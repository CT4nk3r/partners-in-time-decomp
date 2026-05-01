// Ghidra Java post-analysis script
// Exports function list with addresses, sizes, and names
//
// @category NDS
// @author MLPiT Decomp

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import java.util.*;

public class ExportFunctions extends GhidraScript {

    @Override
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();

        String outputDir = System.getenv("MLPIT_OUTPUT_DIR");
        if (outputDir == null || outputDir.isEmpty()) {
            outputDir = ".";
        }

        // Collect all functions
        List<String[]> functions = new ArrayList<>();
        FunctionIterator iter = fm.getFunctions(true);
        int thunkCount = 0;
        long totalCodeBytes = 0;

        while (iter.hasNext()) {
            Function f = iter.next();
            AddressSetView body = f.getBody();
            long size = body.getNumAddresses();
            totalCodeBytes += size;

            if (f.isThunk()) thunkCount++;

            functions.add(new String[]{
                f.getEntryPoint().toString(),
                String.valueOf(size),
                f.getName(),
                String.valueOf(f.isThunk()),
                String.valueOf(f.getParameterCount())
            });
        }

        // Write human-readable function map
        String txtPath = outputDir + File.separator + "functions.txt";
        PrintWriter txtWriter = new PrintWriter(new FileWriter(txtPath));
        txtWriter.println("# Auto-generated function map from Ghidra analysis");
        txtWriter.println("# Address        Size  Name");
        txtWriter.println("#" + "-".repeat(70));
        for (String[] func : functions) {
            txtWriter.printf("  %-14s %6s  %s%n", func[0], func[1], func[2]);
        }
        txtWriter.printf("%n# Total functions: %d%n", functions.size());
        txtWriter.close();

        // Write CSV for easy processing
        String csvPath = outputDir + File.separator + "functions.csv";
        PrintWriter csvWriter = new PrintWriter(new FileWriter(csvPath));
        csvWriter.println("address,size,name,is_thunk,param_count");
        for (String[] func : functions) {
            csvWriter.printf("%s,%s,%s,%s,%s%n", func[0], func[1], func[2], func[3], func[4]);
        }
        csvWriter.close();

        // Print summary
        println("");
        println("==================================================");
        println("Analysis Export Summary");
        println("==================================================");
        println("  Functions found:    " + functions.size());
        println("  Total code bytes:   " + totalCodeBytes);
        println("  Thunk functions:    " + thunkCount);
        println("  Output (txt):       " + txtPath);
        println("  Output (csv):       " + csvPath);
        println("==================================================");
    }
}
