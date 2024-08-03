/* add function that are related to first run of the code */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"
#include "first_run_funcs.h"

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
    int is_symbol;
    int is_data_store;
    int is_extern_entry;
    int ic = 0;
    int dc = 0;
    int char_index;
    int symbol_code;
    int tag_char_index;
    int data_or_entry; /* 0 for data, 1 for entry */
    int op_index;
    int l_count;

    char *first_field = NULL; /* Max length 31 */
    char *second_field = NULL;
    char *third_field = NULL;
    char *fourth_field = NULL;
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
            is_symbol = 0;
            is_data_store = 0;
            is_extern_entry = 0;
            char_index = 0;
            op_index = -1;
            l_count = 1;
            line_index++;

            MOVE_TO_NOT_WHITE(file_line, char_index);
            first_field = fieldInitialAlloc();
            getFieldFromLine(&first_field, file_line, &char_index);

            /* For each symbol return matching status code */
            /* Tag (symbol) name validity check */
            /* Starts with alphabetical character and end with : (has space after)*/
            symbol_code = checkSymbolDefinition(first_field, file_line, &char_index); /* 0 is valid */
            switch(symbol_code) {
                case 0:
                    is_symbol = 1;
                    break;
                case 1:
                    break;
                case 2:
                    break;
                default:
                    break;
            }
            MOVE_TO_NOT_WHITE(file_line, char_index);
            second_field = fieldInitialAlloc();
            getFieldFromLine(&second_field, file_line, &char_index);
            is_data_store = getDataStore(first_field, second_field);
            /*
            if(!is_data_store)
                continue;
            */
            is_extern_entry = getExternEntry(first_field, second_field);
            /*
            if(!is_extern_entry)
                continue;
            */
            /* Check instruction name (mov, jne, etc...) */
            op_index = getOperation(first_field, second_field);
            /* If not an operation, check if it's .data, .string, .extern or .entry */
            if(op_index == -1 && is_data_store && is_extern_entry)
                fprintf(stderr, "Error: Operation name error\t(%d)\n", (op_index == -1 && is_data_store && is_extern_entry));
            /*
            We got a valid op-code (operation), for that check:
            1. Specifically what is the operation and what it receives (source and / or destination)
                1.1. Check field after operation
                    1.1.1. Check if field type matches what the operation accepts
                1.2. Check for second field after operation
                    1.2.1. Check if field type matches what the operation accepts
            2. Calculate l_count using the structure of the operation
            3. Build binary code and update ic = ic + l_count
            */
            if(op_index >= 0 && op_index <= 4) {
                ;
            } else if(op_index >= 5 && op_index <= 13) {
                ;
            } else if(op_index >= 14) {

            } /* No need for else because that would be (-1) which was already addressed */

            printf("first_field: (%s)\tsecond_field: (%s)\top_index: (%d)\tis_data_store: (%d)\tis_extern_entry: (%d)\n\n", first_field, second_field, op_index, is_data_store, is_extern_entry);

            free(second_field);
            free(first_field);
        }

        fclose(sfp);
    }

    return 0;
}