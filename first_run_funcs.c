#include<string.h>
#include<ctype.h>
#include "first_run_funcs.h"

char *first_run_error_codes[] = {
    NULL,
    "Invalid line, not symbol definition or valid operation name",
    "Possible symbol definition starts with non-alphabetical character or doesn't end with ':'",
    "Not symbol definition",
};

/*
Check if a line has a tag / symbol definition, if so check it's validity and return the
appropriate error code
@param *first_field The text that holds the name of the tag / symbol
@param *file_line The entire line that was read from the file
@param *char_index The index of the current character in the line
@return Error code for each case
*/
int checkSymbolDefinition(char *first_field, char *file_line, int *char_index) {
    int tag_char_index;

    if(isalpha(first_field[0])) {
        if(first_field[*char_index - 2] == ':' && file_line[*char_index - 1] == ' ') {
            for(tag_char_index = 1; tag_char_index < strlen(first_field) - 1; tag_char_index++) {
                if(!isalnum(first_field[tag_char_index])) {
                    return 1; /* Invalid line, has non-alphanumeric symbol definition name */
                }
            }
            return 0; /* Valid line and is of the form XYZ: */
        }
        return 2; /* Doesn't end with (: ) so not symbol definition */
    }
    return 1; /* Starts with non-alphabetical character so invalid line */
}

/*
Check if there is an instruction to hold .data or .string
@param *data_type The data type that was read (.data or .string or something else)
@return 1 if it's .data, 2 if it's .string, 0 otherwise
*/
int getDataStore(char *data_type) {
    if(!strcmp(".data", data_type))
        return 1;
    else if(!strcmp(".string", data_type))
        return 2;
    return 0;
}

/*
Check if there is an instruction for .extern or .entry
It can be the first field or second field in the line
@param *first_field The instruction that was read as the first field
@param *second_field The instruction that was read as the second field
@return 1 if it's .extern, 2 if it's .entry, 0 otherwise
*/
int getExternEntry(char *first_field, char *second_field) {
    if(!strcmp(first_field, ".extern") || !strcmp(second_field, ".extern"))
        return 1;
    else if(!strcmp(first_field, ".entry") || !strcmp(second_field, ".entry"))
        return 2;
    return 0;
}