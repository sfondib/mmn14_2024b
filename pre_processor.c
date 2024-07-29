#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"

int main(int argc, char *argv[]) {
	FILE *asfp;
	FILE *amfp;

	macro_dw *matching_macro_node = NULL;
	macro_dw *head = NULL;

	int file_index = 1;
	int char_index;
	int line_index;
	int in_macro = 0;
	int macro_line_index;
	int error_code;

	char *first_field = NULL;
	char *second_field = NULL;
	char file_name_read[256];
	char file_name_write[256];
	char file_line[80];

	if(argc < 2) {
		fprintf(stderr, "Error: No files passed as arguments.\n");
		exit(0);
	}

	while(file_index < argc) {
		line_index = 0;

		/* Open .as file to read from */
		sprintf(file_name_read, "%s.as", argv[file_index]);
		asfp = fopen(file_name_read, "r");
		if(!asfp) {
			fprintf(stderr, "Failed to open file %s\n", file_name_read);
			continue; /* Move to next file */
		}
		/* Create .am file to write to */
		sprintf(file_name_write, "%s.am", argv[file_index++]);
		amfp = fopen(file_name_write, "w");
		if(!amfp) {
			fprintf(stderr, "Failed to create file %s\n", file_name_write);
			continue; /* Move to next file */
		}

		/* Read all lines in the file */
		while(fgets(file_line, 80, asfp)) {
			char_index = 0;
			line_index++;

			first_field = fieldInitialAlloc();
			getFieldFromLine(&first_field, file_line, &char_index);
			/* Check if the first field is the name of a macro on the list */
			if((matching_macro_node = findMacroNodeWithName(head, first_field)) != NULL) {
				macro_line_index = 0;
				while(macro_line_index < matching_macro_node->num_lines) {
					fputs(matching_macro_node->lines[macro_line_index++], amfp); /* Copy each line to .am file */
				}
				continue;
			}
			/* Check if "macr" found */
			if(!strcmp(first_field, MACRO_START)) {
				in_macro = 1;
				/* Get the macro name */
				MOVE_TO_NOT_WHITE(file_line, char_index);
				second_field = fieldInitialAlloc();
				getFieldFromLine(&second_field, file_line, &char_index);
				/* Perform checks to see if format for macro definition is valid */
				if((error_code = macroNameValidityCheck(&head, second_field, file_line, char_index)) != 0) {
					printMacroError(error_code, file_name_read, line_index);
					printf("--------------------\n");
					continue;
				}
				appendMacroNodeToList(&head, line_index, second_field); /* Passed all checks, add new macro to list */
				continue; /* Move to the next line following the macro definition */
			}
			/* If in macro definition */
			if(in_macro) {
				/* Check if "endmacr" found */
				if(!strcmp(first_field, MACRO_END)) {
					in_macro = 0;
					free(second_field);
				/* "endmacr" not found yet, copy line to matching macro in list */
				} else {
					matching_macro_node = findMacroNodeWithName(head, second_field);
					addLineToMacro(matching_macro_node, file_line);
				}
			} else {
				fputs(file_line, amfp); /* Copy regular lines (not part of macro) to .am file */
			}
			free(first_field);
		}
		/* Reset for next file */
		freeMacroList(head);
		head = NULL;
		fclose(asfp);
		fclose(amfp);
	}

	return 0;
}