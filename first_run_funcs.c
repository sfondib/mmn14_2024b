#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include "pre_processor_funcs.h"
#include "my_macro.h"
#include "first_run_funcs.h"

char *first_run_error_codes[] = {
    NULL,
    "Invalid line, not symbol definition or valid operation name",
    "Possible symbol definition starts with non-alphabetical character or doesn't end with ':'",
    "Not symbol definition",
};

char *op_names[] = {
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
};

char *register_names[] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",
};

/*
Check which instruction it is (how many operands it accepts) and check their addressing type.
If an addressing method that does not match the type is used, an error is printed.
@param op_index Index representing the operation
@param *second_field Field holding the first operand
@param *third_field Field holding the second operand
@param *operand1 To hold the value of the first operand
@param *operand2 To hold the value of the second operand
@param *operand1_method To hold the value representing the addressing method of the first operand
@param *operand1_method To hold the value representing the addressing method of the second operand
*/
int getOperandsFromInstruction(int op_index, char *field1, char *field2, int *operand1, int *operand2, int *operand1_method, int *operand2_method) {
    switch(op_index) {
        /*
        Source: 0,1,2,3
        Dest:     1,2,3
        */
        case 0:
        case 2:
        case 3:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 1, 1, 1, 1, 1)) {
                return 0;
            }
            /* Second operand */
            if(!getSecondOperandData(field2, operand2_method, operand2, 0, 1, 1, 1)) {
                return 0;
            }
            break;
        /*
        Source: 0,1,2,3
        Dest:   0,1,2,3
        */
        case 1:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 1, 1, 1, 1, 1)) {
                return 0;
            }
            /* Second operand */
            if(!getSecondOperandData(field2, operand2_method, operand2, 1, 1, 1, 1)) {
                return 0;
            }
            break;
        /*
        Source:   1
        Dest:     1,2,3
        */
        case 4:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 0, 1, 0, 0, 1)) {
                return 0;
            }
            /* Second operand */
            if(!getSecondOperandData(field2, operand2_method, operand2, 0, 1, 1, 1)) {
                return 0;
            }
            break;
        /*
        Source: Non
        Dest:     1,2,3
        */
        case 5:
        case 6:
        case 7:
        case 8:
        case 11:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 0, 1, 1, 1, 0)) {
                return 0;
            }
            break;
        /*
        Source: Non
        Dest:     1,2
        */
        case 9:
        case 10:
        case 13:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 0, 1, 1, 0, 0)) {
                return 0;
            }
            break;
        /*
        Source: Non
        Dest:   0,1,2,3
        */
        case 12:
            /* First operand */
            if(!getFirstOperandData(field1, operand1_method, operand1, 0, 1, 1, 1, 0)) {
                return 0;
            }
            break;
        /*
        Source: Non
        Dest:   Non
        */
        case 14:
        case 15:
            break;
    }
    return 1;
}

/*
Get the data from the first operand (The addressing method and value)
@param *field The field holding the operand
@param *operand1_method To hold the value representing the addressing method
@param *operand1 To hold the value of the operand
@param allow0 If addressing method 0 is allowed or not
@param allow1 If addressing method 1 is allowed or not
@param allow2 If addressing method 2 is allowed or not
@param allow3 If addressing method 3 is allowed or not
@return 1 for valid, 0 otherwise
*/
int getFirstOperandData(char *field, int *operand1_method, int *operand1, int allow0, int allow1, int allow2, int allow3, int comma) {
    /* Check if immediate addressing method (0) and if it's allowed */
    if(field[0] == '#') {
        if(allow0) {
            if(!checkDirectAddressing(field)) {
                fprintf(stderr, "Error: Invalid characters after #\n");
                return 0;
            }
            *operand1_method = 0;
            sscanf(field, "%d", operand1);
        } else {
            fprintf(stderr, "Error: Immediate addressing (method 0) not allowed for operation\n");
            return 0;
        }
    /* Check if indirect register addressing (2) and if it's allowed */
    } else if(field[0] == '*') {
        if(allow2) {
            if(!isRegisterName(field, 1, 1)) {
                fprintf(stderr, "Error: Invalid register name\n");
                return 0;
            }
            *operand1_method = 2;
            *operand1 = getRegisterNumber(field, 2, comma);
        } else {
            fprintf(stderr, "Error: Indirect register addressing (method 2) not allowed for operation\n");
            return 0;
        }
    /* Check if direct register addressing (3) and if it's allowed */
    } else if((isRegisterName(field, 0, 1))) {
        if(allow3) {
            *operand1_method = 3;
            *operand1 = getRegisterNumber(field, 1, comma);
        } else {
            fprintf(stderr, "Error: Direct register addressing (method 3) not allowed for operation\n");
            return 0;
        }
    /* Check if direct addressing with symbol (1) and if it's allowed */
    } else {
        if(allow1) {
            *operand1_method = 1;
            /* operand1 = symbol / address of symbol */
        } else {
            fprintf(stderr, "Error: Direct addressing with symbol (method 1) not allowed for operation\n");
            return 0;
        }
    }
    return 1; /* Valid operand use */
}

/*
Get the data from the first operand (The addressing method and value)
@param *field The field holding the operand
@param *operand1_method To hold the value representing the addressing method
@param *operand1 To hold the value of the operand
@param allow0 If addressing method 0 is allowed or not
@param allow1 If addressing method 1 is allowed or not
@param allow2 If addressing method 2 is allowed or not
@param allow3 If addressing method 3 is allowed or not
@return 1 for valid, 0 otherwise
*/
int getSecondOperandData(char *field, int *operand2_method, int *operand2, int allow0, int allow1, int allow2, int allow3) {
    /* Check if immediate addressing method (0) and if it's allowed */
    if(field[0] == '#') {
        if(allow0) {
            if(!checkDirectAddressing(field)) {
                fprintf(stderr, "Error: Invalid characters after #\n");
                return 0;
            }
            *operand2_method = 0;
            sscanf(field, "#%d", operand2);
        } else {
            fprintf(stderr, "Error: Immediate addressing (method 0) not allowed for operation\n");
            return 0;
        }
    /* Check if indirect register addressing (2) and if it's allowed */
    } else if(field[0] == '*') {
        if(allow2) {
            if(!isRegisterName(field, 1, 0)) {
                fprintf(stderr, "Error: Invalid register name\n");
                return 0;
            }
            *operand2_method = 2;
            *operand2 = getRegisterNumber(field, 2, 0);
        } else {
            fprintf(stderr, "Error: Indirect register addressing (method 2) not allowed for operation\n");
            return 0;
        }
    /* Check if direct register addressing (3) and if it's allowed */
    } else if((isRegisterName(field, 0, 0))) {
        if(allow3) {
            *operand2_method = 3;
            *operand2 = getRegisterNumber(field, 1, 0);
        } else {
            fprintf(stderr, "Error: Direct register addressing (method 3) not allowed for operation\n");
            return 0;
        }
    /* Check if direct addressing with symbol (1) and if it's allowed */
    } else {
        if(allow1) {
            *operand2_method = 1;
            /* operand1 = symbol / address of symbol */
        } else {
            fprintf(stderr, "Error: Direct addressing with symbol (method 1) not allowed for operation\n");
            return 0;
        }
    }
    return 1; /* Valid operand use */
}

/*
Check if the operation receives the amount of operands it needs to receive.
If it's "mov" to "lea" then it should receive 2 operands.
If it's "clr" to "jsr" then it should receive 1 operand.
If it's "rts" to "stop" then it should receive no operands.
@param op_index The index of the operation from *op_names
@param *second_field The first field capable of receiving an operand
@param *third_field The second field capable of receiving an operand
@param *fourth_field The third field capable of receiving and operand (Error for most cases)
@return 1 if error found, 0 otherwise
*/
int validateOperandCount(int op_index, char *second_field, char *third_field, char *fourth_field) {
    if(op_index == -1) {
        fprintf(stderr, "Error: Invalid operation name\n");
        return 1;
    }

    if(op_index >= 0 && op_index <= 4) {
        if(!(strcmp(second_field, "")) || !(strcmp(third_field, ""))) {
            fprintf(stderr, "Error: Missing operand for operation which requires 2 operands\n");
            return 1;
        }
        if(strcmp(fourth_field, "")) {
            fprintf(stderr, "Error: Extra text after operation which requires 2 operands\n");
            return 1;
        }
    } else if(op_index >= 5 && op_index <= 13) {
        if(!(strcmp(second_field, ""))) {
            fprintf(stderr, "Error: Missing operand for operation which requires 1 operand\n");
            return 1;
        }
        if(strcmp(third_field, "")) {
            fprintf(stderr, "Error: Extra text after operation which requires 1 operand\n");
            return 1;
        }
    } else if(op_index >= 14 && op_index <= 15) {
        if(strcmp(second_field, "")) {
            fprintf(stderr, "Error: Extra text after operation which requires no operands\n");
            return 1;
        }
    }
    
    return 0;
}

/*
Check if a given field holds a valid register name
@param *field The field that holds the operand
@param start_offset How many characters to ignore at the start of the field
@param end_offset How many characters to ignore at the end of the field
@return 1 if register name, 0 otherwise
*/
int isRegisterName(char *field, int start_offset, int end_offset) {
    int i;
    char *temp_field;

    /* Avoid symbols */
    if(strlen(field) < 2)
        return 0;

    /* Take the part which is r<register number> */
    temp_field = (char *)malloc(sizeof(char) * strlen(field));
    strncpy(temp_field, field + start_offset, strlen(field) - start_offset - end_offset);
    temp_field[strlen(field) - start_offset - end_offset] = '\0';
    
    /* Loop through possible register names */
    for(i = 0; i < 8; i++) {
        if(strstr(register_names[i], temp_field)){
            free(temp_field);
            return 1; /* Register name */
        }
    }
    free(temp_field);
    return 0; /* Not register name */
}

/*
Take a validated register and return it's number
@param *field The field that holds the operand
@param start_offset How many characters to ignore at the start of the field
@param end_offset How many characters to ignore at the end of the field
@return The register's number
*/
int getRegisterNumber(char *field, int start_offset, int end_offset) {
    char *temp_field;
    int register_number;

    /* Take just the register number */
    temp_field = (char *)malloc(sizeof(char) * strlen(field));
    strncpy(temp_field, field + start_offset, strlen(field) - start_offset - end_offset);
    temp_field[strlen(field) - start_offset - end_offset] = '\0';

    /* Convert to integer and free pointer */
    register_number = atoi(temp_field);
    free(temp_field);

    return register_number;
}

/*
Check if an operand matches the direct addressing type, starts with # and has an integer
after it (0, negative or positive)
@param *operand The field that holds the operand
@return 1 if direct addressing, 0 otherwise
*/
int checkDirectAddressing(char *field) {
    int i;

    for(i = 1; i < strlen(field) - 1; i++) {
        if(!isdigit(field[i]) && field[i] != '-') {
            fprintf(stderr, "Error: Non-digit in direct addressing\n");
            return 0;
        }
    }
    return 1;
}

/*
Check if a line has a tag / symbol definition, if so check it's validity and return the
appropriate error code
@param *first_field The text that holds the name of the tag / symbol
@param *file_line The entire line that was read from the file
@param *char_index The index of the current character in the line
@return Error code for each case
*/
int checkSymbolDefinition(char *first_field) {
    int i;

    if(isalpha(first_field[0])) {
        if(first_field[strlen(first_field) - 1] == ':') {
            for(i = 0; i < strlen(first_field) - 1; i++) {
                if(!isalnum(first_field[i])) {
                    return 1; /* Non-alphanumeric in name */
                }
            }
            return 0; /* Valid line, starts with alpha, ends with ':' and is all alpha-numeric characters */
        }
        return 2/* Can be operation but not symbol definition */;
    } else if(first_field[0] == '.') {
        return 3;
    } else {
        return 4;
    }
}

/*
Check if there is an instruction to hold .data or .string
@param *data_type The data type that was read (.data or .string or something else)
@return 1 if it's .data, 2 if it's .string, 0 otherwise
*/
int getDataStore(char *second_field) {
    if(!strcmp(second_field, ".data"))
        return 1;
    else if(!strcmp(second_field, ".string"))
        return 2;
    return 0;
}

void processTokens(char *file_line, int *dc, int *dc_error, instruction *memory) {
    char *token;
    char binary_str[16];

    token = strtok(file_line, " ");
    token = strtok(NULL, " ");

    while(token != NULL) {
        if(*dc >= 100) {
            fprintf(stderr, "Error: Data overflow error, DC counter exceeding 100\n");
            *dc_error = 1;
            break;
        }
        memory[(*dc)++].full = atoi(token);
        decToBin15(memory[*dc - 1].full, binary_str);
        printf("memory[%d] is %d in decimal and %s in binary\n", *dc - 1, memory[*dc - 1].full, binary_str);
        token = strtok(NULL, ", ");
    }
}


/*
Check if there is an instruction for .extern or .entry
It can be the first field or second field in the line
@param *first_field The instruction that was read as the first field
@param *second_field The instruction that was read as the second field
@return 1 if it's .extern, 2 if it's .entry, 0 otherwise
*/
int getExternEntry(char *second_field) {
    if(!strcmp(second_field, ".extern"))
        return 1;
    else if(!strcmp(second_field, ".entry"))
        return 2;
    return 0;
}

/*
Get the index of the operation from the list *op_names
@param *field The string that holds the operation name
@return Index of operation in *op_names
*/
int getOperation(char *field) {
    int i;

    for(i = 0; i < NUM_OPS; i++) {
        if(!strcmp(field, op_names[i]))
            return i;
    }
    return -1;
}

void decToBin15(int num, char* binary_str) {
    int i;

    binary_str[15] = '\0';

    for (i = 14; i >= 0; i--) {
        binary_str[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
}