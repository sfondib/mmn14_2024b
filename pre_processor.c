/*
TO-DO:

[X] check if there are more invalid macro names that need to be
	added to the list
[V] make hash-table for macro names and their content (OR array of
	type struct with name field and contents field OR linked-list)
[V] after reading line, check if the pattern searched for ("macr")
	exists in it, if so then save the line number and raise flag
	for detecting we are inside a macro (use later for finding
	endmacr and saving the line a macro ends)
	- if pattern found check if the index the pattern started is
		0, if not then there are characters before it (error)
[X] check about upper-case and lower-case
[-] check if it's ok for macro definitions to have leading and / or
	trailing spaces / tabs -> waiting for answer
[X] split main into more functions for organization
[X] create "pre_processor_funcs.h" and "pre_processor_funcs.c for
	all the helper functions used for the pre-processing stage
	(can delete prototypes and definitions after)
[X] convert global variables to local variables (make appropriate
	changes to functions)

*/

/* Libraries*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"

/* Definitions */
#pragma GCC diagnostic ignored "-Wunused-variable"			/* REMOVE LATER */
#pragma GCC diagnostic ignored "-Wunused-value"				/* REMOVE LATER */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	/* REMOVE LATER */
/* Struct to hold data for a single macro */
typedef struct macro_data macro_d;
struct macro_data {
	char **lines;
	int macro_lims[2]; /* [0] = start-line, [1] = end-line */
	char *macro_name;
};

/* Prototypes */
int checkValidMacroName(char *);
int checkMacroAlreadyDefined(char *, int);
int getMacroIndexFromList(char *, int);
void getMacroNameFromLine(char *, int *, char **);
void addNewMacroToList(int, char *);
void initMacroList(int);
void addMacroLinesToList(FILE *, int);
void freeMacroFromList(macro_d **);

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
	"entry",
	"extern",
};
/* Possible error messages */
char *error_messages[] = {
	"ERROR: File (%s) > Line (%d)\n\tMacro definition has leading characters\n",
	"ERROR: File (%s) > Line (%d)\n\tMissing name for macro definition\n",
	"ERROR: File (%s) > Line (%d)\n\tInvalid macro name in macro definition\n",
	"ERROR: File (%s) > Line (%d)\n\tExtra characters after macro name\n",
	"ERROR: No files passed as arguments\n",
	"ERROR: Memory allocation failed\n",
	"ERROR: File (%s) > Line (%d)\n\tAttempt to redefine already defined macro\n",
	"ERROR: File (%s) > Line (%d)\n\tDigit at start of macro name\n"
};
char line[MAX_LINE_LEN]; /* ??? Maybe make local to the calling function ??? */
macro_d *saved_macros = NULL;
int macro_list_length = 1;
int saved_macro_line_index = 0;

/*
The function receives .as files with assembly instructions and
replaces all macro calls with the matching definitions

@param argc Number of arguments (source files) passed
@param *argv[] List of pointers to strings (Names of sources files passed)
@return 0 if completed successfully, != 0 otherwise
*/
int main(int argc, char *argv[]) {
	FILE *fp;
	FILE *fp3;
	int file_index = 1; /* When passed as arguments, files start from 1 in argv */
	int pattern_index;
	int current_line; /* Keep track of lines for macros / errors */
	int macro_start_line_num;
	int macro_end_line_num;
	char *macro_name;
	int in_macro = 0;
	int saved_macros_index;
	int i;
	int j;

	saved_macros = malloc(sizeof(macro_d));
	initMacroList(0);

	/* No files passed as arguments */
	if(argc < 2) {
		PRINT_ERROR(0, 0, 4);
		exit(1);
	}

	/* Loop through all files */
	while(file_index < argc) {
		/* Attempt to open a file with the .as file extension */
		if(!(fp = fopen(argv[file_index], "r"))) {
			fprintf(stderr, "Unable to open file %s\n", argv[file_index]);
			continue;
		}
		if(!(fp3 = fopen(strcat(argv[file_index], ".am"), "w"))) {
			fprintf(stderr, "Unable to create file %s\n", argv[file_index]);
			continue;
		}
		file_index++;

		current_line = 0;
		while(fgets(line, MAX_LINE_LEN, fp) != NULL) {
			current_line++;
			/* Check if macr or endmacr keywords are in the line, if so check if it's valid */
			if((pattern_index = patternStartIndex(line, MACRO_START)) != -1) {
				if(pattern_index) {
					/* Incase macr was detected inside endmacr */
					pattern_index = patternStartIndex(line, MACRO_END);
					if(!pattern_index) {
						goto endmacr_jump;
					}
					PRINT_ERROR(argv[file_index], current_line, 0);
					continue; /* continue instead of break so all errors are printed */
				} else {
					/* Make sure not pointing to same place in memory and free it */
					macro_name = NULL; /* Overcome null terminator in previous iterations */
					free(macro_name);
					macro_name = (char *)malloc(sizeof(char));
					checkAlloc(macro_name);

					/* Macro definition found, skip keyword and whitespace characters */
					macro_start_line_num = current_line;
					pattern_index += strlen(MACRO_START) + 1; /* Use pattern index for writing to macro_name */
					MOVE_TO_NOT_WHITE(line, pattern_index);

					/* Read the name until a whitespace or end of line reached */
					while(checkWhitespace(line, &pattern_index)) {
						macro_name[strlen(macro_name)] = line[pattern_index++];
						macro_name = (char *)realloc(macro_name, strlen(macro_name) + 1); /* Make sure there is enough space for the macro_name */
						checkAlloc(macro_name);
					}
					/* Last line with text in the file might not have a newline (blank line after it) */
					/* Replace \n with \0 at the end */
					if(macro_name[strlen(macro_name) - 1] == '\n') 
						macro_name[strlen(macro_name) - 1] = '\0';
					else
						macro_name[strlen(macro_name)] = '\0';

					/* If length of macro name is 0 then no name was passed */
					if(!strlen(macro_name)) {
						PRINT_ERROR(argv[file_index], current_line, 1);
						continue;
					}
					
					if(isdigit(macro_name[0])) {
						PRINT_ERROR(argv[file_index], current_line, 7);
						continue;
					}

					/* Validate macro name */
					if(checkValidMacroName(macro_name)) {
						PRINT_ERROR(argv[file_index], current_line, 2);
						continue;
					}

					/* Check for extra text after the name in the macro definition */
					MOVE_TO_NOT_WHITE(line, pattern_index);
					if(checkWhitespace(line, &pattern_index)) {
						PRINT_ERROR(argv[file_index], current_line, 3);
						continue;
					}

					/* Check if macro name already defined */
					if(checkMacroAlreadyDefined(macro_name, macro_list_length) == 1) {
						PRINT_ERROR(argv[file_index], current_line, 6);
						continue;
					}

					/* Previous test passed (Macro name not defined), save new macro */
					addNewMacroToList(macro_start_line_num, macro_name);
					in_macro = 1;
					continue;

					endmacr_jump: /* endmacr detected, jump here, skip previous checks */
					in_macro = 0;
					saved_macros[saved_macros_index].macro_lims[1] = current_line;
					saved_macros[saved_macros_index].lines[saved_macro_line_index] = NULL;
					saved_macro_line_index = 0;
					macro_name = NULL; /* Overcome null terminator in previous iterations */
					free(macro_name);
					macro_name = (char *)malloc(sizeof(char));
					checkAlloc(macro_name);
					continue;
				}
			} else {
				/* If currently in a macro definition, save the line in the matching */
				if(in_macro) {
					line[strlen(line) - 1] = '\0';
					/* Save line in matching macro */
					saved_macros_index = getMacroIndexFromList(macro_name, macro_list_length);
					saved_macros[saved_macros_index].lines = realloc(saved_macros[saved_macros_index].lines, (saved_macro_line_index + 1) * sizeof(char *));
					saved_macros[saved_macros_index].lines[saved_macro_line_index] = malloc(strlen(line) * sizeof(char));
					strcpy(saved_macros[saved_macros_index].lines[saved_macro_line_index++], line);
				} else {
					/* Avoid segmentation fault if there are no macros yes */
					if(macro_list_length < 2) {
						fputs(line, fp3);
						continue;
					}
					/* Write line to .am file */
					for(i = 0; i < macro_list_length; i++) {
						/* Check if the line that was read has a macro name in it (macro call) */
						if(!patternStartIndex(line, saved_macros[i].macro_name)) {
							/* If so, print each and every line from the macro definition */
							for(j = 0; saved_macros[i].lines[j] != NULL; j++) {
								fputs(strcat(saved_macros[i].lines[j], "\n"), fp3);
							}
						/* If not, check if there extra characters in the macro definition */
						} else if(patternStartIndex(line, saved_macros[i].macro_name) > 0) {
							fprintf(stderr, "Extra characters in macro call\n");
						/* Lastly, if it's just a normal line, just copy it */
						} else {
							fputs(line, fp3);
						}
					}
				}
			}
		}
		/* Things to reset before moving on to the next file */
		fflush(NULL);
		fclose(fp);
		macro_list_length = 1; /* Next file has it's own list of macros */
		free(saved_macros);
		saved_macros = malloc(sizeof(macro_d));
		initMacroList(0);
		fclose(fp3);
	}
	return 0;
}

/*
This frees the macro that was passed as argument and all of it's data

CHECK IF IT REALLY NEEDS TO BE A DOUBLE POINTER OR JUST ONE POINTER
@param **macro_to_release Double pointer to the macro that we want to release because otherwise a copy would be released
@return void
*/
void freeMacroFromList(macro_d **macro_to_release) {
	/* Add here looping over list of lines of the macro and release each */
	free((*macro_to_release)->macro_name);
	free(*macro_to_release);
	return;
}

/*
**lines and *macro_name in the struct have no default value, this function is used
to initialize the first macro that is created immediately to avoid segmentation fault
when trying to access **lines or *macro_name
*/
void initMacroList(int index) {
	saved_macros[index].lines = NULL;
	saved_macros[index].macro_name = NULL;
}

/* When reading also check for endmacr (and save the line number) */
void addMacroLinesToList(FILE *fp, int macro_num_in_list) {
	return;
}

/*
Add a new macro that was detected to the list of defined macros. At first hold only
the number of the line the macro starts and the name of the macro, as the end was
not yet found

@param macro_start_line Number of the line the macro starts in
@param *macro_name The name of the macro that was detected
@return void
*/
void addNewMacroToList(int macro_start_line, char *macro_name) {
	/* If this is the first macro to be added to the list */
	if(macro_list_length == 1) {
		saved_macros[0].macro_lims[0] = macro_start_line;
		saved_macros[0].macro_name = malloc(sizeof(char) * strlen(macro_name)); /* Macro sure the struct has enough space to hold the macro name */
		strcpy(saved_macros[0].macro_name, macro_name);
		saved_macros[0].lines = (char **)malloc(saved_macro_line_index * sizeof(char *));
		macro_list_length++;
	/* If not the first macro to be added to the list */
	} else {
		macro_list_length++;
		saved_macros = realloc(saved_macros, macro_list_length);
		initMacroList(macro_list_length - 1);
		saved_macros[macro_list_length - 1].macro_lims[0] = macro_start_line;
		saved_macros[macro_list_length - 1].macro_name = malloc(sizeof(char) * strlen(macro_name));  /* Macro sure the struct has enough space to hold the macro name */
		strcpy(saved_macros[macro_list_length - 1].macro_name, macro_name);
		saved_macros[macro_list_length - 1].lines = (char **)malloc(saved_macro_line_index * sizeof(char *));
	}
}

/*
Checks if a name for a macro already exists in the macro list

@param *tested_macro_name Name of the macro to check if it already exists
@param macro_list_length Length of the list of macros
@return 0 if it's the first macro added, -1 if already exists, i for the index of where it's in saved_macros list
*/
int checkMacroAlreadyDefined(char *tested_macro_name, int macro_list_length) {
	int i;
	for(i = 0; i <= macro_list_length; i++) {
		/* Check if first macro to be added */
		if(saved_macros[i].macro_name == NULL)
			return 0;
		/* If not first macro to be added, compare it to existing macros, return 1 if redefined*/
		if(strcmp(saved_macros[i].macro_name, tested_macro_name) == 0) {
			return 1; /* If already defined, return the index */
		}
	}

	return 2;
}

int getMacroIndexFromList(char *macro_name, int macro_list_length) {
	int i;
	for(i = 0; i <= macro_list_length; i++) {
		/* If not first macro to be added, compare it to existing macros, return 1 if redefined*/
		if(strcmp(saved_macros[i].macro_name, macro_name) == 0) {
			return i; /* If already defined, return the index */
		}
	}

	return -1;
}

/* 
Check if the name of the macro is valid or not, invalid names are
names that are used for operations or instructions

@param *macro_name String holding the name of the pointer
@return 1 if invalid name found, 0 otherwise
*/
int checkValidMacroName(char *macro_name) {
	int i;

	/* Compare with each name on the list of invalid names */
	for(i = 0; i < NUM_INVALID_MACRO_NAMES; i++)
		if(!strcmp(macro_name, invalid_macro_names[i]))
			return 1;
	
	return 0;
}