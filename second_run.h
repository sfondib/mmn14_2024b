#ifndef SECOND_RUN_H
#define SECOND_RUN_H

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

#endif /* SECOND_RUN_H */
