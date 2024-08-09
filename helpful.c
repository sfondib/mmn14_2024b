#include <stdio.h>
#include <stdarg.h>
#include "helpful.h"
#include "my_macro.h"
#include "table.h"

void *malloc_check(long size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Memory allocation has failed, please try again.\n");
        exit(1);
    }
    return ptr;
}

int printErrorAccordingToLine(line_info line, char *message, ...) {
    int chars_printed;
    va_list args; /* for formatting */
    
    /* Print file name and line number */
    fprintf(stderr,"Error In %s:%ld: ", line.file_name, line.line_number);
    /* Initialize args to store all values after 'message' */
    va_start(args, message);
    chars_printed = vfprintf(stderr, message, args);
    va_end(args);
    fprintf(stderr, "\n"); /* Print a new line after error - we shouldn't calculate '\n' char */
    return chars_printed;
}

int is_type_matching(table tbl, char *key, va_list arglist, int symbol_count) {
    int i;
    symbol_type current_type;

    /* Loop through all the valid symbol types passed in the variadic arguments */
    for (i = 0; i < symbol_count; i++) {
        current_type = va_arg(arglist, symbol_type);
        /* If the type matches and the key is the same, return 1 (match found) */
        if (current_type == tbl->type && strcmp(key, tbl->key) == 0) {
            return 1;
        }
    }
    return 0; /* No match found */
}
