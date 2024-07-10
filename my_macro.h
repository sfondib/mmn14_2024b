/* Maximum length of a single source line  */
#define MAX_LINE_LENGTH 80

/* Initial IC value */
#define IC_INIT_VALUE 100

/* Boolean (t/f) helpers */
#define FALSE = 0
#define TRUE = 1

/**
* For both runs
* Moves the index to the next place in string where there is no white char 
*/
#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;
