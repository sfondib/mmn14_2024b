#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"
#include "first_run_funcs.h"

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

/*
When first_run.c is done, convert main to a regular function that accepts all the
necessary parameters because everything goes through assembler.c and not first_run.c
*/
int main(int argc, char *argv[]) {
    FILE *sfp; /* Source file pointer */

    int file_index = 1;
    int line_index;
    int op_index;
    int data_string;
    int is_symbol;
    int ic;
    int dc;
    int char_index;
    int data_or_entry; /* 0 for data, 1 for entry */
    int operand1;
    int operand2;
    int operand1_method; /* 0 - Immediate, 1 - Direct, 2 - Indirect register, 3 - Direct register */
    int operand2_method;

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
        ic = 0;
        dc = 0;
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
            is_symbol = 0;
            operand1 = 0;
            operand1_method = 0;
            operand2 = 0;
            operand2_method = 0;

            /* Initialize fields */
            initializeFields(&first_field, &second_field, &third_field, &fourth_field);
            sscanf(file_line, "%s %s %s %s", first_field, second_field, third_field, fourth_field);
            printf("f1: %s\nf2: %s\nf3: %s\nf4: %s\n", first_field, second_field, third_field, fourth_field);

            /* Is symbol definition */
            if(!(is_symbol = checkSymbolDefinition(first_field))) {
                is_symbol = 0;
            /* Non-alphanumeric character found in the name when defining symbol */
            } else if(is_symbol == 1) {
                fprintf(stderr, "Error: Non-alphanumeric in symbol name\n");
                continue;
            /* Can be operation but not symbol definition */
            } else if(is_symbol == 2) {
                is_symbol = 1;
            /* Not symbol definition, it can be .data or .string */
            } else if(is_symbol == 3) {
                is_symbol = 2;
            } else {
                fprintf(stderr, "Error: Non-alpha or . at start\n");
                continue;
            }

            /*
            There is a symbol, analyze rest of instruction (second_field, third_field, fourth_field, text after is error)
            This can be anything, operation, .data, .string, .extern, .entry
            */
            if(!is_symbol) {
                op_index = getOperation(second_field);
                data_string = getDataStore(second_field);
                if((op_index == -1) && (data_string == 0)) {
                    fprintf(stderr, "Error: Invalid operation name after symbol definition\n");
                    continue;
                }

            /*
            There is no symbol, analyze rest of instruction (first_field, second_field, third_field, fourth_field is error)
            This needs to be a normal operation
            */
            } else if(is_symbol == 1) {

                op_index = getOperation(first_field); /* Get the index matching to the operation */

                /* Validate the number of operands passed matches the operation */
                if(validateOperandCount(op_index, second_field, third_field, fourth_field))
                    continue;

                /* Get operands and addressing type for each */
                if(!getOperandsFromInstruction(op_index, second_field, third_field, &operand1, &operand2, &operand1_method, &operand2_method))
                    continue;

                printf("op1: %d\top1_method: %d\nop2: %d\top2_method: %d\n", operand1, operand1_method, operand2, operand2_method);
            
            /*
            There is no symbol, analyze rest of instruction (first_field, second_field, third_field, fourth_field is error)
            This needs to have .data or .string or .extern or .entry
            */
            } else if(is_symbol == 3) {
                if(!(strcmp(first_field, ".data")) || !(strcmp(first_field, ".string"))) {
                    ;
                } else if(!(strcmp(first_field, ".extern")) || !(strcmp(first_field, ".entry"))) {
                    ;
                }
            }

            putchar('\n');

        }

        free(first_field);
        free(second_field);
        free(third_field);
        free(fourth_field);

        fclose(sfp);
    }

    return 0;
}
