#include "pre_processor_funcs.h"
#include "first_run_funcs.h"
#include "second_run.h"
#include "table.h"

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
    char file_name_read[256];
    int file_index;
    bool success = TRUE;

    if (argc < 2) {
        fprintf(stderr, "Error: No input files provided.\n");
        return 1;
    }

    /* Loop over all files provided in the command line arguments */
    for (file_index = 1; file_index < argc; file_index++) {
        /* Step 1: Pre-processing */
        sprintf(file_name_read, "%s.as", argv[file_index]);
        file = fopen(file_name_read, "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s for reading.\n", file_name_read);
            success = FALSE;
            continue;
        }

        /* Perform the pre-processing step */
        if (!processFilePreProcessor(file, file_name_read)) {
            fprintf(stderr, "Error: Pre-processing failed for file %s.\n", file_name_read);
            success = FALSE;
            fclose(file);
            continue;
        }
        fclose(file);

        /* Step 2: First pass */
        sprintf(file_name_read, "%s.am", argv[file_index]);
        file = fopen(file_name_read, "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s for first pass.\n", file_name_read);
            success = FALSE;
            continue;
        }

        /* Perform the first pass */
        if (!firstRun(file, &symbol_table, file_name_read)) {
            fprintf(stderr, "Error: First pass failed for file %s.\n", file_name_read);
            success = FALSE;
            fclose(file);
            continue;
        }
        fclose(file);

        /* Step 3: Second pass */
        file = fopen(file_name_read, "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s for second pass.\n", file_name_read);
            success = FALSE;
            continue;
        }

        /* Perform the second pass */
        if (!processFileSecondPass(file, file_name_read, &symbol_table)) {
            fprintf(stderr, "Error: Second pass failed for file %s.\n", file_name_read);
            success = FALSE;
            fclose(file);
            continue;
        }
        fclose(file);

        /* Free symbol table for the next file */
        freeTableMemory(symbol_table);
        symbol_table = NULL;
    }

    if (!success) {
        fprintf(stderr, "Errors were encountered during processing.\n");
        return 1;
    }

    printf("All files processed successfully.\n");
    return 0;
}
