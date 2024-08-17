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

/*
Creates a new node for a macro that is being defined
@param start_line Line number where the macro definition starts
@param macro_name Name of the macro that is being created
@return Pointer to the macro that was created
*/
macro_dw* createMacroNode(int start_line, char *macro_name);

/*
Go through the list and find the node with the name passed as argument
@param *head Head pointer to the start of the linked-list
@param *macro_name Name of the macro that is looked for
@return Pointer to the matching node
*/
macro_dw* findMacroNodeWithName(macro_dw *head, char *macro_name);

/*
Check if the name given to the macro is valid
@param **head Head pointer to the start of the linked-list
@param macro_name Name of the tested macro
@param *line Line read from the file
@param index Character index in the line that was read
@return Integer to represent the error code for each tested case
*/
int macroNameValidityCheck(macro_dw **head, char *macro_name, char *line, int index);

/*
Add the newly created macro to the linked-list
@param **head Head pointer to the start of the linked-list
@param start_line Line number where the macro definition starts
@param macro_name Name of the macro that is being appended (for creation)
*/
void appendMacroNodeToList(macro_dw **head, int start_line, char *macro_nme);

/*
Free all macros on the list and each of their fields
@param *head Head pointer to the start of the linked-list
*/
void freeMacroList(macro_dw *head);

/*
Get the field from the line that was read (first field or second field) and after allocating the field
@param **field The field variable to edit (first field or second field)
@param *file_line Line read from the file
@param *index Character index in the line that was read
*/
void getFieldFromLine(char **field, char *file_line, int *index);

/*
Print the error message for the error that was encountered
@param error_code Number representing the error
@param *file_name Name of the file the error occured in
@param line_index The line the error occured in
*/
void printMacroError(int error_code, char *file_name, int line_index);

/*
A line from inside a macro definition to the matching macro on the list
@param *macro_node The matching macro node to add the line to
@param *file_line The line to add
*/
void addLineToMacro(macro_dw *macro_node, char *file_line);

/*
Take all four fields that can make up an instruction and initialize them using the
fieldInitialAlloc() function
@param **first_field The first field in the instruction
@param **second_field The second field in the instruction
@param **third_field The third field in the instruction
@param **fourth_field The fourth field in the instruction
*/
void initializeFields(char **first_field, char **second_field, char **third_field, char **fourth_field);

/*
Allocate enough memory for reading the field in the passed line (first field or second field)
@return Pointer to the field
*/
char* fieldInitialAlloc(void);