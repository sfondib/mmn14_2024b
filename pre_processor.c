/*
TO-DO:

[X] check if there are more invalid macro names that need to be
	added to the list
[X] make hash-table for macro names and their content (OR array of
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

*/

/* Libraries*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "my_macro.h"

/* Definitions */
#pragma GCC diagnostic ignored "-Wunused-variable"			/* REMOVE LATER */
#pragma GCC diagnostic ignored "-Wunused-value"				/* REMOVE LATER */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	/* REMOVE LATER */

/* Prototypes */
int checkValidMacroName(char *);
int patternStartIndex(char *, char *);
void checkAlloc(void *);
void getMacroNameFromLine(char *, int *, char **);

/* Struct to hold data for a single macro */
typedef struct macro_data {
	char *macro_lines[1]; /* Maybe use malloc / realloc since number of lines for macro is unknown, another option is
	doing (macro_end_line_num - macro_start_line_num) to find how many lines while creating the struct */
	int macro_lims[2]; /* [0] = start-line, [1] = end-line */
	char *macro_name;
} macro_data_d;

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
char line[MAX_LINE_LEN]; /* ??? Maybe make local to the calling function ??? */
int error_found = 0; /* ??? Maybe make local to calling function ???*/

/*
The function receives .as files with assembly instructions and
replaces all macro calls with the matching definitions

@param argc Number of arguments (source files) passed
@param *argv[] List of pointers to strings (Names of sources files passed)
@return 0 if completed successfully, != 0 otherwise
*/
int main(int argc, char *argv[]) {
	FILE *fp;
	int file_index = 1; /* When passed as arguments, files start from 1 in argv */
	int pattern_index;
	int current_line; /* Keep track of lines for macros / errors */
	int macro_start_line_num;
	int macro_end_line_num;
	char *macro_name;
	
	if(argc < 2) {
		fprintf(stderr, "ERROR: No files passed as arguments\n");
		exit(1);
	}

	/* macro_name = (char *)malloc(sizeof(char)); */

	/* Loop through all files */
	while(file_index < argc) {

		/* Attempt to open a file with the .as file extension */
		if(!(fp = fopen(strcat(argv[file_index], ".as"), "r"))) {
			fprintf(stderr, "Unable to open file %s\n", argv[file_index++]);
			continue;
		}
		
		current_line = 0;
		while(fgets(line, MAX_LINE_LEN, fp)) {
			
			current_line++;

			/* Make sure not pointing to same place in memory and free it */
			macro_name = NULL; /* Overcome null terminator in previous iterations */
			free(macro_name);

			macro_name = (char *)malloc(sizeof(char));
			checkAlloc(macro_name);
			/* macro_name[0] = '\0'; */
			
			if((pattern_index = patternStartIndex(line, MACRO_START)) != -1) {
				if(pattern_index) {
					fprintf(stderr, "ERROR: File (%s) > Line (%d)\n\tMacro definition has leading characters\n", argv[file_index], current_line);
					continue; /* continue instead of break so all errors are printed */
				} else {
					/* Macro definition found, skip keyword and whitespace characters */
					macro_start_line_num = current_line;
					pattern_index += strlen(MACRO_START) + 1; /* Use pattern index for writing to macro_name */
					MOVE_TO_NOT_WHITE(line, pattern_index);

					/* Read the name until a whitespace or end of line reached */
					while(line[pattern_index] != ' ' && line[pattern_index] != '\n', line[pattern_index] != '\t' && line[pattern_index] != '\0') {
						macro_name[strlen(macro_name)] = line[pattern_index++];
						macro_name = (char *)realloc(macro_name, strlen(macro_name) + 1); /* Make sure there is enough space for the macro_name */
						checkAlloc(macro_name);
					}

					/* Last line with text in the file might not have a newline (blank line after it)*/
					/* Replace \n with \0 at the end */
					if(macro_name[strlen(macro_name) - 1] == '\n') 
						macro_name[strlen(macro_name) - 1] = '\0';
					else
						macro_name[strlen(macro_name)] = '\0';

					/* If length of macro name is 0 then no name was passed */
					if(!strlen(macro_name)) {
						fprintf(stderr, "ERROR: File (%s) > Line (%d)\n\tMissing name for macro definition\n", argv[file_index], current_line);
						continue; /* continue instead of break so all errors are printed */
					}
					
					/* Validate macro name */
					if(checkValidMacroName(macro_name)) {
						fprintf(stderr, "ERROR: File (%s) > Line (%d)\n\tInvalid macro name in macro definition\n\t(%s) is a reserved keyword\n", argv[file_index], current_line, macro_name);
						continue;
					}

				}
			}
		}

		file_index++;
		fclose(fp);
	}

	return 0;
}

/* After fixing double pointers, replace with what's inside main */
void getMacroNameFromLine(char *fileLine, int *ptrnIdx, char **macro_name) {
	while(fileLine[*ptrnIdx] != ' ' && fileLine[*ptrnIdx] != '\n', fileLine[*ptrnIdx] != '\t' && fileLine[*ptrnIdx] != '\0') {
		*macro_name[strlen(*macro_name)] = fileLine[(*ptrnIdx)++];
		*macro_name = (char *)realloc(*macro_name, strlen(*macro_name) + 1); /* Make sure there is enough space for the macro_name */
		checkAlloc(*macro_name);
	}
	/* Last line with text in the file might not have a newline (blank line after it)*/
	/* Replace \n with \0 at the end */
	if(*macro_name[strlen(*macro_name) - 1] == '\n') 
		*macro_name[strlen(*macro_name) - 1] = '\0';
	else
		*macro_name[strlen(*macro_name)] = '\0';
}

/*
Check if pointer allocation successeded

@param *ptr Copy of the same pointer that points to the same place
@return void
*/
void checkAlloc(void *ptr) {
	if(ptr == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed\n");
		exit(1);
	}
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

/*
Find the pattern requested in the passed string

@param *string String to search in
@param *pattern Pattern searched for
@return Index where the "pattern" starts in "string"
*/
int patternStartIndex(char *string, char *pattern) {
	int i, j, k;

	for(i = 0; string[i] != '\0'; i++) {
		/* Compare the characters in the pattern with each and every character in string */
		for(j = i, k = 0; pattern[k] != '\0' && string[j] == pattern[k]; j++, k++)
			;
		/* Pattern found, return starting index in "string" */
		if(k > 0 && pattern[k] == '\0')
			return i;
	}

	return -1; /* Index not found */
}