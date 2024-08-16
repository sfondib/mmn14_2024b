#ifndef MY_MACRO_H
#define MY_MACRO_H

/* Maximum length of a single source line  */
#define MAX_LINE_LEN 80

/* Maximum size of code image */
#define CODE_IMG_MAX_LENGTH 1200

#define NUM_OPS     16
#define MAX_VAL_15  16383   /* -2¹⁴ - 1 */
#define MIN_VAL_15  -16384  /* -2¹⁴ */
#define MAX_VAL_DC  100
#define MAX_VAL_IC  4096

/* Macro rules */
#define NUM_INVALID_MACRO_NAMES 20
#define MACRO_START "macr"
#define MACRO_END "endmacr"

/* Initial IC value */
#define IC_START_VALUE 100

/* zeroize variable */
#define ZEROIZE 0

/* variable of entry length - for second run */
#define ENTRY_LENGTH 6

/* Boolean helpers for functions - bool */
typedef enum booleans {
    FALSE = 0,
    TRUE = 1
} bool;

/* For both runs
   Moves the index to the next place in string where there is no white char */
#define jumpOnWhiteChar(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index))) \
        ;

/* info about file name, index of line number and etc */
typedef struct line_info {
    long line_number;
    char *file_name;
    char *content; /* Line content (source) */
} line_info;

/* Instruction type of data, extern, entry, string */
typedef enum directive {
    DATA_INST, /* data instruction */
    STRING_INST, /* string instruction */
    ENTRY_INST, /* entry instruction */
    EXTERN_INST /* extern instruction */
} directive;

#endif /* MY_MACRO_H */
