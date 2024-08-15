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
    int is_symbol;
    int data_string;
    int ic;
    int dc;
    int ic_error;
    int dc_error;
    int operand1;
    int operand2;
    int operand1_method; /* 0 - Immediate, 1 - Direct, 2 - Indirect register, 3 - Direct register */
    int operand2_method;
    int i;
    int j;

    char *first_field = NULL; /* Max length 31 */
    char *second_field = NULL;
    char *third_field = NULL;
    char *fourth_field = NULL;
    char file_name_read[256];
    char file_line[80];
    char *token;
    char binary_str[16];

    instruction memory[4096];

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

                /* Add first word to memory */
                memory[100 + ic].fields4.are = 4;
                memory[100 + ic].fields4.add_dest = operand2_method;
                memory[100 + ic].fields4.add_src = operand1_method;
                memory[100 + ic++].fields4.op_code = op_index;
                decToBin15(memory[100 + ic - 1].full, binary_str);
                printf("converted 1st: %s\n", binary_str);
                /* Add second word to memory */
                if(operand1_method == 1) {
                    memory[100 + ic].fields2.are = 4;
                    memory[100 + ic++].fields2.operand = operand1;
                } else if(operand1_method == 2) {
                    memory[100 + ic++].full = -1;
                } else if(operand1_method == 4) {
                    memory[100 + ic].fields5.are = 4;
                    memory[100 + ic].fields5.add_share1 = operand1;
                    memory[100 + ic].fields5.add_share2 = 0;
                    memory[100 + ic].fields5.blank = 0;
                    memory[100 + ic++].fields5.op_code = 0;
                } else if(operand1_method == 8) {
                    memory[100 + ic].fields5.are = 4;
                    memory[100 + ic].fields5.add_share1 = 0;
                    memory[100 + ic].fields5.add_share2 = operand1;
                    memory[100 + ic].fields5.blank = 0;
                    memory[100 + ic++].fields5.op_code = 0;
                }
                
                /* Check shared word for addressing methods 2 and 3 */
                if(operand1_method == operand2_method) {
                    memory[100 + ic - 1].fields5.are = 4;
                    memory[100 + ic - 1].fields5.add_share1 = operand2;
                    memory[100 + ic - 1].fields5.add_share2 = operand1;
                    memory[100 + ic - 1].fields5.blank = 0;
                    memory[1100 + ic - 1].fields5.op_code = 0;
                    decToBin15(memory[100 + ic - 1].full, binary_str);
                    printf("converted 2nd: %s\n", binary_str);
                    putchar('\n');
                    continue;
                } else {
                    decToBin15(memory[100 + ic - 1].full, binary_str);
                    printf("converted 2nd: %s\n", binary_str);
                }

                /* Add third word to memory */
                if(operand2_method == 1) {
                    memory[100 + ic].fields2.are = 4;
                    memory[100 + ic++].fields2.operand = operand2;
                } else if(operand2_method == 2) {
                    memory[100 + ic++].full = -1;
                } else if(operand2_method == 4) {
                    memory[100 + ic].fields5.are = 4;
                    memory[100 + ic].fields5.add_share1 = operand2;
                    memory[100 + ic].fields5.add_share2 = 0;
                    memory[100 + ic].fields5.blank = 0;
                    memory[100 + ic++].fields5.op_code = 0;
                } else if(operand2_method == 8) {
                    memory[100 + ic].fields5.are = 4;
                    memory[100 + ic].fields5.add_share1 = 0;
                    memory[100 + ic].fields5.add_share2 = operand2;
                    memory[100 + ic].fields5.blank = 0;
                    memory[100 + ic++].fields5.op_code = 0;
                }
                decToBin15(memory[100 + ic - 1].full, binary_str);
                printf("converted 3rd: %s\n", binary_str);
                
            
            /*
            There is no symbol, analyze rest of instruction (first_field, second_field, third_field, fourth_field is error)
            This needs to have .data or .string or .extern or .entry
            */
            } else if(is_symbol == 2) {
                /* Check if data instruction */
                if(!strcmp(first_field, ".data")) {
                    token = strtok(file_line, " ");
                    token = strtok(NULL, " ");

                    /* Read each number */
                    while(token != NULL) {
                        /* Check for DC overflow error (>= 100 is for instruction) */
                        if(dc >= MAX_VAL_DC) {
                            fprintf(stderr, "Error: Data overflow error, DC counter exceeding 100\n");
                            dc_error = 1;
                            break;
                        }
                        /* Check if what is after .data is a number */
                        for(j = 0; j < strlen(token) - 1; j++) {
                            /* Ignore the minus sign */
                            if(token[j] == '-')
                                continue;
                            /* Non-digit character found */
                            if(!isdigit(token[j])) {
                                fprintf(stderr, "Non-number sequence after .data\n");
                                dc_error = 1;
                                break;
                            }
                        }
                        /* Error, continue to next line */
                        if(dc_error)
                            continue;
                        /* For 15 bits, minimum value is -16384 and maximum value is 16383 */
                        if((atoi(token) < MIN_VAL_15) || (atoi(token) > MAX_VAL_15)) {
                            fprintf(stderr, "Error: Maximum or minimum allowe value exceeded\n");
                            dc_error = 1;
                            break;
                        }
                        memory[dc++].full = atoi(token); /*  Save number in memory using DC counter */
                        token = strtok(NULL, ", ");
                    }
                    /* Error, continue to next line */
                    if(dc_error)
                        continue;
                /* Chec if .string instruction */
                } else if(!strcmp(first_field, ".string")) {
                    /* Doesn't start or end with " */
                    if(!(second_field[0] == '\"') || !(second_field[strlen(second_field) - 1] == '\"')) {
                        fprintf(stderr, "Error: Non-string sequence after .string\n");
                        continue;
                    }
                    /* Read each character in the string */
                    for(i = 1; i < strlen(second_field) - 1; i++) {
                        /* Check for DC overflow error (>= 100 is for instruction) */
                        if(dc >= MAX_VAL_DC) {
                            fprintf(stderr, "Error: Data overflow error, DC counter exceeding 100\n");
                            dc_error = 1;
                            break;
                        }
                        memory[dc++].full = (int)second_field[i]; /*  Save each character in memory in order */
                    }
                    /* Error, continue to next line */
                    if(dc_error)
                        continue;
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
