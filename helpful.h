/* In helpful file we will find helpful functions that will take part in the whole program */
#ifndef HELPFUL_H
#define HELPFUL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "my_macro.h"
#include "table.h"

/**
 * Allocates memory in required size, if process has been failed - exits the program
 * @param size - the size to allocate in bytes
 * @return a generic pointer to allocated memory
 */
void *malloc_check(long size);

/**
 * Prints an error message
 * @param line - a structure containing the file name and line number where the error occurred
 * @param message - a format string similar to printf, used to format the error message
 * @param ... - additional arguments required by the format string
 * @return the total number of characters printed (including line number and file name)
 */
int printErrorAccordingToLine(line_info line, char *message, ...);

/**
 * Checks if the symbol type matches any of the valid types provided.
 * @param tbl The current table item
 * @param key The key to match
 * @param arglist The variadic argument list containing the valid types
 * @param symbol_count The number of valid symbol types
 * @return 1 if a match is found, 0 otherwise
 */
int is_type_matching(table tbl, char *key, va_list arglist, int symbol_count);

#endif /* HELPFUL_H */
