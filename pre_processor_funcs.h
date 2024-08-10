#ifndef NULL
#define NULL ((void*)0)
#endif

/* Struct to hold data for a single macro */
typedef struct macro_data_word macro_dw;
struct macro_data_word {
	char **lines;
	int num_lines;
	char *name;
	macro_dw *next;
};

macro_dw* createMacroNode(int start_line, char *macro_name);
macro_dw* findMacroNodeWithName(macro_dw *head, char *macro_name);
int macroNameValidityCheck(macro_dw **head, char *macro_name, char *line, int index);
void appendMacroNodeToList(macro_dw **head, int start_line, char *macro_nme);
void freeMacroList(macro_dw *head);
void getFieldFromLine(char **field, char *file_line, int *index);
void printMacroError(int error_code, char *file_name, int line_index);
void addLineToMacro(macro_dw* macro_node, char *file_line);
void initializeFields(char **first_field, char **second_field, char **third_field, char **fourth_field);
char* fieldInitialAlloc(void);