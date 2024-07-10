/*
TO-DO:

- check if there are more invalid macro names that need to be
	added to the list
- make hash-table for macro names and their content (or array of
	type struct with name field and contents field)
- after reading line, check if the pattern searched for ("macr")
	exists in it, if so then save the line number and raise flag
	for detecting we are inside a macro (use later for finding
	endmacr and saving the line a macro ends)
	- if pattern found check if the index the pattern started is
		0, if not then there are characters before it (error)
- check about upper-case and lower-case
- consider putting getLine() and patternStartIndex() in a
	different	file and then include it(maybe will be used by
	other files too)
- check if it's ok for macro definitions to have leading and / or
	trailing spaces / tabs
- split main into more functions for organization

*/

/* Libraries*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Definitions */
#define NUM_INVALID_MACRO_NAMES	20
#define MAX_LINE_LEN			80
#define MACRO_START				"macr"
#define MACRO_END				"endmacr"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-value"

/* Prototypes */
int checkValidMacroName(char *);
int getLine(char *);
int patternStartIndex(char *, char *);

/* Struct to hold data for a single macro */
typedef struct macro_data {
	char *macro_lines[1]; /* Maybe use malloc / realloc since number of lines for macro is unknown, another option is
	doing (macro_end_line_num - macro_start_line_num) to find how many lines while creating the struct */
	int macro_lims[2]; /* [0] = start-line, [1] = end-line */
	char *macro_name;
};

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

@param1 argc Number of arguments (source files) passed
@param2 *argv[] List of pointers to strings (Names of sources files passed)
@return 0 if completed successfully, != 0 otherwise
*/
int main(int argc, char *argv[]) {
	FILE *fp;
	int file_index = 1; /* When passed as arguments, files start from 1 in argv */
	int pattern_index;
	int current_line; /* Keep track of lines for macros / errors */
	int macro_start_line_num;
	int macro_end_line_num;
	
	if(argc < 2) {
		fprintf(stderr, "No files passed as arguments\n");
		exit(1);
	}
	
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
			if((pattern_index = patternStartIndex(line, MACRO_START)) != -1) {
				if(pattern_index) {
					fprintf(stderr, "File %s, line %d, macro definition has leading spaces and / or tabs\nMoving to \
						next source file", argv[file_index], current_line);
					break;
				} else {
					macro_start_line_num = current_line;
					/* "macr" found at the start of a line */
					/* start checking from index (pattern_index + 4) or (pattern_index + strlen(MACRO_START)) to check
					what's after the "macr" keyword (skip whitespaces and check the text, compare it to valid names
					using the checkValidMacroName() function)*/
				}
			}
		}

		fclose(fp);	
	}

	return 0;
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
MAYBE DELETE BECAUSE fgets CAN READ THE LINE INTO AN ARRAY ANYWAY
Read every character from the passed string into an array (buffer)

@param *string String to read characters from
@return Number of characters read
*/
int getLine(char *string) {
	int c, i = 0;

	/* Read until newline reached */
	while((i < MAX_LINE_LEN) && ((c = getchar()) != EOF) && (c != '\n'))
		string[i++];
	/* Insert \\n and \\0 to the buffer */
	if(c == '\n')
		string[i++] = '\n';
	string[i] = '\0';

	return i; /* Use to check if maximum allowed length of a line was exceeded */
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