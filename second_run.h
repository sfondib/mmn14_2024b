#ifndef SECOND_RUN_H
#define SECOND_RUN_H

#include <stdio.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"
#include "helpful.h"
#include "symbol_table.h"
#include "first_run_funcs.h"

/**
 * Processes a single line during the second pass of the assembler.
 * @param line - The line information, including content, file name, and line number.
 * @param ic - A pointer to the instruction counter.
 * @param symbol_table - A pointer to the symbol table.
 * @return TRUE if processing is successful, FALSE if an error occurs.
 */
bool lineIndexPass(line_info line, long *ic, table *symbol_table);

/**
 * Process the entire source file in the second pass.
 * @param file - Pointer to the source file to process.
 * @param file_name - The name of the source file.
 * @param symbol_table - Pointer to the symbol table.
 * @param memory - Pointer to the instruction memory.
 * @param dc - The data counter value.
 * @return TRUE if processing is successful, FALSE if errors occurred.
 */
bool processFileSecondPass(FILE *file, const char *file_name, table *symbol_table, instruction *memory, long dc);

/**
 * Encodes the operands of an instruction during the second pass.
 * @param line - The line information, including content, file name, and line number.
 * @param ic - A pointer to the instruction counter.
 * @param symbol_table - A pointer to the symbol table.
 * @param operand1_method - Pointer to store the addressing method of the first operand.
 * @param operand2_method - Pointer to store the addressing method of the second operand.
 * @return TRUE if encoding is successful, FALSE if an error occurs.
 */
bool encodeOperands(line_info line, long *ic, table *symbol_table, int *operand1_method, int *operand2_method);

/**
 * Creates the .ob file by writing the encoded instructions and data to it.
 * @param file_name - The base name of the source file.
 * @param memory - The memory structure holding the encoded instructions.
 * @param ic - The final instruction counter value.
 * @param dc - The final data counter value.
 * @return TRUE if the file is created successfully, FALSE if an error occurs.
 */
bool createObjectFile(const char *file_name, instruction *memory, long ic, long dc);

/**
 * Creates the .ob file by writing the encoded instructions and data to it.
 * @param file_name - The base name of the source file.
 * @param memory - The memory structure holding the encoded instructions.
 * @param ic - The final instruction counter value.
 * @param dc - The final data counter value.
 * @return TRUE if the file is created successfully, FALSE if an error occurs.
 */
bool createObjectFile(const char *file_name, instruction *memory, long ic, long dc);

#endif /* SECOND_RUN_H */
