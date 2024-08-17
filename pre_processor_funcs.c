#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "pre_processor_funcs.h"
#include "my_macro.h"

/* Macro names must not be names of operations or instructions */
char *invalid_macro_names[] = {
	"mov",
	"cmp",
	"add",
	"sub",
	"lea",
	"clr",
	"not",
	"inc",
	"dec",
	"jmp",
	"bne",
	"red",
	"prn",
	"jsr",
	"rts",
	"stop",
	".data",
	".string",
	".entry",
	".extern",
	"r0",
	"r1",
	"r2",
	"r3",
	"r4",
	"r5",
	"r6",
	"r7",
};

char *error_codes[] = {
	NULL,
	"No name passed in macro decleration.",
	"First character of macro name is not alphanumeric.",
	"Reserved word used as macro name.",
	"Extra characters after macro name in definition.",
	"Redefining existing macro.",
	"No files passed as arguments",
};

void printMacroError(int error_code, char *file_name, int line_index) {
	fprintf(stderr, "Error: %s\nFile: %s\nLine: %d\n", error_codes[error_code], file_name, line_index);
}

void addLineToMacro(macro_dw* macro_node, char *file_line) {
	if(macro_node->lines == NULL) {
		macro_node->lines = (char **)malloc(2 * sizeof(char *));
		if(macro_node->lines == NULL) {
			printf("Memory allocation failed\n");
			exit(1);
		}
	/* Not the first line read into the macro, resize */
	} else {
		macro_node->lines = (char **)realloc(macro_node->lines,(macro_node->num_lines + 2) * sizeof(char *));
	}
	/* Copy the line into the macro */
	macro_node->lines[macro_node->num_lines] = (char *)malloc(strlen(file_line) * sizeof(char));
	strcpy(macro_node->lines[macro_node->num_lines++], file_line);
	macro_node->lines[macro_node->num_lines] = NULL;
}

/*
Allocate enough memory for reading the field in the passed line (first field or second field)
@return Pointer to the field
*/
char* fieldInitialAlloc(void) {
	char *field = (char *)malloc(sizeof(char));
	if(field == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	return field;
}

/*
Take all four fields that can make up an instruction and initialize them using the
fieldInitialAlloc() function
@param **first_field The first field in the instruction
@param **second_field The second field in the instruction
@param **third_field The third field in the instruction
@param **fourth_field The fourth field in the instruction
*/
void initializeFields(char **first_field, char **second_field, char **third_field, char **fourth_field) {
	*first_field = fieldInitialAlloc();
	*second_field = fieldInitialAlloc();
	*third_field = fieldInitialAlloc();
	*fourth_field = fieldInitialAlloc();
	strcpy(*first_field, "");
	strcpy(*second_field, "");
	strcpy(*third_field, "");
	strcpy(*fourth_field, "");
}

/*
Check if the name given to the macro is valid
@param **head Head pointer to the start of the linked-list
@param macro_name Name of the tested macro
@param *line Line read from the file
@param index Character index in the line that was read
@return Integer to represent the error code for each tested case
*/
int macroNameValidityCheck(macro_dw **head, char *macro_name, char *line, int index) {
	int i = 0;

	/* Length of macro name is 0 (No name passed) */
	if(!strlen(macro_name))
		return 1;
	/* First character is not alphanumeric */
	if(!isalpha(macro_name[0]))
		return 2;
	while(invalid_macro_names[i] != NULL){
		/* Reserved word used for macro name in definition */
		if(!strcmp(macro_name, invalid_macro_names[i++]))
			return 3;
	}
	/* Extra characters after macro name in definition */
	jumpOnWhiteChar(line, index);
	if(line[index] != ' ' && line[index] != '\n' && line[index] != '\t' && line[index] != '\0')
		return 4;
	/* Redefinition of macro */
	if(findMacroNodeWithName(*head, macro_name) != NULL)
		return 5;
	return 0; /* All tests passed */
}

/*
Get the field from the line that was read (first field or second field) and after allocating the field
@param **field The field variable to edit (first field or second field)
@param *file_line Line read from the file
@param *index Character index in the line that was read
*/
void getFieldFromLine(char **field, char *file_line, int *index) {
	char file_character;
	int length = 0;

	/* Read each and every character in the word until a whitespace is found (end of word) */
	while((file_character = file_line[(*index)++]) != ' ' && file_character != '\n' && file_character != '\t' && file_character != '\0') {
		*field = (char *)realloc(*field, length + 1); /* Make sure it has enough space to hold the word */
		(*field)[length++] = file_character;
		(*field)[length] = '\0'; /* Mark end of word */
	}
}

/*
Creates a new node for a macro that is being defined
@param start_line Line number where the macro definition starts
@param macro_name Name of the macro that is being created
@return Pointer to the macro that was created
*/
macro_dw* createMacroNode(int start_line, char *macro_name) {
	macro_dw *new_node = (macro_dw *)malloc(sizeof(macro_dw));

	if(!new_node) {
		return NULL;
	}

	/* Initialize the values of the newly created macro */
	new_node->lines = NULL;
	new_node->num_lines = 0;
	/* Make new_node->name have enough space for macro_name before saving it*/
	new_node->name = malloc(sizeof(char) * strlen(macro_name));
	strcpy(new_node->name, macro_name);
	new_node->next = NULL;

	return new_node;
}

/*
Add the newly created macro to the linked-list
@param **head Head pointer to the start of the linked-list
@param start_line Line number where the macro definition starts
@param macro_name Name of the macro that is being appended (for creation)
*/
void appendMacroNodeToList(macro_dw **head, int start_line, char *macro_name) {
	macro_dw *temp = *head;
	macro_dw *new_node = createMacroNode(start_line, macro_name);

	if(!new_node) {
		return;
	}

	if(*head == NULL) {
		*head = new_node; /* Node appended is first on the linked-list */
	} else {
		temp = *head;
		/* Keep going to the next node until the last one was found */
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_node; /* new_node is the new last node and temp will point to it */
	}
}

/*
Go through the list and find the node with the name passed as argument
@param *head Head pointer to the start of the linked-list
@param *macro_name Name of the macro that is looked for
@return Pointer to the matching node
*/
macro_dw* findMacroNodeWithName(macro_dw *head, char *macro_name) {
	while(head != NULL) {
		/* Macro with that name was found, return head which points to that macro node */
		if(strcmp(head->name, macro_name) == 0) {
			return head;
		}
		head = head->next;
	}

	return NULL; /* Macro with that name was not found */
}

/*
Free all macros on the list and each of their fields
@param *head Head pointer to the start of the linked-list
*/
void freeMacroList(macro_dw *head) {
	macro_dw *temp;
	int i;

	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp->name); /* Free the macro's name field */
		if (temp->lines != NULL) {
			for (i = 0; temp->lines[i] != NULL; i++) {
				free(temp->lines[i]); /* Free each of the macro's lines */
			}
			free(temp->lines); /* Free the whole line field */
		}
		free(temp); /* Free the macro itself (The registry in the macro list) */
	}
}