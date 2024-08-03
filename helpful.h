/* in helpul file we will find helpful functions that will take part in the whole program */
#ifndef HELPFUL_H
#define HELPFUL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "my_macro.h"

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

#endif /* HELPFUL_H */
