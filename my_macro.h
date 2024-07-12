/* Maximum length of a single source line  */
#define MAX_LINE_LEN 80

/* Macro rules */
#define NUM_INVALID_MACRO_NAMES	20
#define MACRO_START			        "macr"
#define MACRO_END				"endmacr"

/* Initial IC value */
#define IC_START_VALUE 100

/* Boolean helpers - bool */
typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

/**
* For both runs
* Moves the index to the next place in string where there is no white char 
*/
#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;
