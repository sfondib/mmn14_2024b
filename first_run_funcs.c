#include "first_run_funcs.h"

char *first_run_error_codes[] = {
    NULL,
    "Invalid line, not symbol definition or valid operation name",
    "Possible symbol definition starts with non-alphabetical character or doesn't end with ':'",
    "Not symbol definition",
};

int checkSymbolDefinition(char *first_field, char *file_line, int char_index) {
    int tag_char_index;

    if(isalpha(first_field[0])) {
        if(first_field[char_index - 2] == ':' && file_line[char_index - 1] == ' ') {
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