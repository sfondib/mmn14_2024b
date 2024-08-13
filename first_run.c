#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_macro.h"
#include "pre_processor_funcs.h"
#include "first_run_funcs.h"

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

typedef struct {
    int are       : 3;
    int add_dest  : 4;
    int add_src   : 4;
    int op_code   : 4;
} bword4;

typedef struct {
    int are       : 3;
    int add_share : 8;
    int op_code   : 4;
} bword3;

typedef union {
    bword4 fields4;
    bword3 fields3;
    int full : 15;
} instruction;

/*
When first_run.c is done, convert main to a regular function that accepts all the
necessary parameters because everything goes through assembler.c and not first_run.c
*/
int main(int argc, char *argv[]) {
    FILE *sfp; /* Source file pointer */

    int file_index = 1;
    int line_index;
    int op_index;
    int is_symbol;
    int ic;
    int dc;
    int ic_error;
    int dc_error;
    int char_index;
    int data_string; /* 0 for data, 1 for string */
    int ext_ent; /* 0 for extern, 1 for entry */
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
    char *token;
    char binary_str[16];

    instruction memory[4096];
    instruction inst1; /* First translated instruction word */
    instruction inst2; /* Second translated instruction word */
    instruction inst3; /* Third translated instruction word */

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
            ic_error = 0;
            dc_error = 0;

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
            } else if(is_symbol == 2) {
                /* Check if data instruction */
                if(!strcmp(first_field, ".data")) {
                    token = strtok(file_line, " ");
                    token = strtok(NULL, " ");

                    /* Read each number and put in memory using DC counter */
                    while(token != NULL) {
                        /* Check for DC overflow error (>= 100 is for instruction) */
                        if(dc >= 100) {
                            fprintf(stderr, "Error: Data overflow error, DC counter exceeding 100\n");
                            dc_error = 1;
                            break;
                        }
                        memory[dc++].full = atoi(token);
                        token = strtok(NULL, ", ");
                    }
                    if(dc_error) {
                        continue;
                    }
                } else if(!strcmp(first_field, ".string")) {
                    ;
                } else if(!strcmp(first_field, ".extern")) {
                    ;
                } else if(!strcmp(first_field, ".entry")) {
                    ;
                } else {
                    fprintf(stderr, "Error: Unknown instruction\n");
                    continue;
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
