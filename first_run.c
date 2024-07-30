/* add function that are related to first run of the code */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

/**
* Descrition of the file
*
* @param ........
* @param .........
*/

/*
When first_run.c is done, convert main to a regular function that accepts all the
necessary parameters because everything goes through assembler.c and not first_run.c
*/
int main(int argc, char *argv[]) {
    FILE *sfp; /* Source file pointer */

    int file_index = 1;
    int line_index;
    int is_symbol = 0;
    int ic = 0;
    int dc = 0;
    int char_index;
    int error_code;
    int tag_char_index;
    int data_or_entry; /* 0 for data, 1 for entry */

    char *first_field = NULL; /* Max length 31 */
    char file_name_read[256];
    char file_line[80];

    if(argc < 2) {
        fprintf(stderr, "No files passed as arguments.\n");
        exit(1);
    }

    while(file_index < argc) {
        line_index = 0;

		/* Open .as file to read from */
		sprintf(file_name_read, "%s.am", argv[file_index++]);
		sfp = fopen(file_name_read, "r");
		if(!sfp) {
			fprintf(stderr, "Failed to open file %s\n", file_name_read);
			continue; /* Move to next file */
		}

        while(fgets(file_line, 80, sfp)) {
            char_index = 0;
            line_index++;

            first_field = fieldInitialAlloc();
            getFieldFromLine(&first_field, file_line, &char_index);

            /* For each symbol return matching status code */
            /* Tag (symbol) name validity check */
            /* Starts with alphabetical character and end with : (has space after)*/
            error_code = checkSymbolDefinition(first_field, file_line, &char_index); /* 0 is valid */
            switch(error_code) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                default:
                    break;
            }


            free(first_field);
        }

        fclose(sfp);
    }

    return 0;
}