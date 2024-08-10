#ifndef SECOND_RUN_H
#define SECOND_RUN_H

#include <stdio.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"
#include "helpful.h"
#include "table.h"

/**
 * Processes a single line during the second pass of the assembler.
 * 
 * This function analyzes a given line of assembly code during the second pass, 
 * updating the instruction counter (IC) and making necessary changes to the symbol table.
 * 
 * @param line - The line information, including content, file name, and line number.
 * @param ic - A pointer to the instruction counter, which may be updated during processing.
 * @param symbol_table - A pointer to the symbol table, which may be modified based on the line content.
 * 
 * @return TRUE if processing is successful, FALSE if an error occurs.
 */
bool lineIndexPass(line_info line, long *ic, table *symbol_table);

/**
 * Process the entire source file in the second pass.
 * 
 * This function reads through each line of the source file and processes it 
 * according to the rules defined for the second pass, updating the instruction 
 * counter and the symbol table accordingly.
 * 
 * @param file - Pointer to the source file to process.
 * @param file_name - The name of the source file.
 * @param symbol_table - Pointer to the symbol table.
 * 
 * @return TRUE if processing is successful, FALSE if errors occurred.
 */
bool processFileSecondPass(FILE *file, const char *file_name, table *symbol_table);

#endif /* SECOND_RUN_H */
