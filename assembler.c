#include "pre_processor_funcs.h"
#include "first_run_funcs.h"
#include "second_run.h"
#include "symbol_table.h"
#include "helpful.h"
#include "pre_processor.h"


/**
 * Main function for processing assembler files.
 * This program processes multiple assembler files in three main steps:
 * 1. Pre-processing: Expands macros and prepares the source code for assembly.
 * 2. First pass: Constructs the symbol table and prepares for binary code generation.
 * 3. Second pass: Generates the final machine code using the symbol table.
 * Each step is performed sequentially for each input file provided as a command-line argument.
 * @param argc - The number of command-line arguments.
 * @param argv - Array of command-line arguments containing the file names to process.
 * @return 0 if all files are processed successfully, 1 if errors were encountered.
 */

int main(int argc, char *argv[]) {
    FILE *file;
    table symbol_table = NULL;
    instruction memory[MAX_VAL_IC];
    char file_name_read[256];
    int file_index;
    long dc = 0;
    bool success = TRUE;

    if (argc < 2) {
        fprintf(stderr, "Error: No input files provided.\n");
        return 1;
    }

    /* Loop over all files provided in the command line arguments */
    for (file_index = 1; file_index < argc; file_index++) {
        /* Step 1: Pre-processing */
        sprintf(file_name_read, "%s.as", argv[file_index]);
        if (preProcessor(file_name_read)) {
            fprintf(stderr, "Error: Pre-processing failed for file %s.\n", file_name_read);
            success = FALSE;
            continue;
        }

        /* Step 2: First pass */
        sprintf(file_name_read, "%s.am", argv[file_index]);
        if (firstRun(file_name_read, memory)) {
            fprintf(stderr, "Error: First pass failed for file %s.\n", file_name_read);
            success = FALSE;
            continue;
        }

        /* Step 3: Second pass */
        file = fopen(file_name_read, "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s for second pass.\n", file_name_read);
            success = FALSE;
            continue;
        }

        if (!processFileSecondPass(file, file_name_read, &symbol_table, memory, dc)) {
            fprintf(stderr, "Error: Second pass failed for file %s.\n", file_name_read);
            success = FALSE;
        }

        fclose(file);

        /* Free symbol table for the next file */
        freeTableMemory(&symbol_table);
    }

    if (!success) {
        fprintf(stderr, "Errors were encountered during processing.\n");
        return 1;
    }

    printf("All files processed successfully! Thanks for your time and good luck!\n");
    return 0;
}
