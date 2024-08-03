#include <stdio.h>
#include <stdarg.h>
#include "helpful.h"
#include "my_macro.h"

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

/* Instruction type of data, extern, entry, string */
typedef enum instruction {
    DATA_INST, /* data instruction */
    STRING_INST, /* string instruction */
    ENTRY_INST, /* entry instruction */
    EXTERN_INST /* extern instruction */
} instruction;
