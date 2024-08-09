#include <stdio.h>
#include "helpful.h"
#include "my_macro.h"
#include "table.h"
#include "pre_processor_funcs.h"
#include "second_run.h"

bool lineIndexPass(line_info line, long *ic, table *symbol_table) {
    long charIndex = ZEROIZE;  /* counter for line index */
    char *ptr;
    char *indexOfColon = NULL;
    
    /* Step 1: Initialize IC and check if the file passed the first run */
    *ic = IC_START_VALUE;
    if (line.content == NULL) {
        fprintf(stderr, "Error: No file content provided for processing.\n");
        return FALSE;
    }
    MOVE_TO_NOT_WHITE(line.content, charIndex);
    
    /* Step 2: Read the next line from the source file. If end of file, go to step 9 */
    /* Continue if the line is empty or has just '\n' in content */
    if (line.content[charIndex] == ';' || line.content[charIndex] == '\n')
        return TRUE;
    
    /* Step 3: If the first field is a label, skip it */
    indexOfColon = strchr(line.content, ':');  /* Find the label */
    if (indexOfColon != NULL) {
        charIndex = indexOfColon - line.content;  /* Calculate space in memory */
        charIndex++;  /* Move one index forward in the line content */
    }
    MOVE_TO_NOT_WHITE(line.content, charIndex);  /* Skip white characters if needed */
    
    /* Step 4: Is it a .DATA, .STRING, or EXTERN directive? If yes, return to step 2 */
    if (line.content[charIndex] == '.') {
        ptr = strtok(line.content + charIndex, " \n\t");
        
        if (strcmp(ptr, ".data") == 0 || strcmp(ptr, ".string") == 0 || strcmp(ptr, ".extern") == 0) {
            /* If it's .DATA, .STRING, or .EXTERN, skip the line and continue to the next line */
            return TRUE;  /* Return to step 2 */
        }
        MOVE_TO_NOT_WHITE(line.content, charIndex);
        
        /* Step 5: Is it an ENTRY directive? If not, go to step 7 */
        if (strcmp(ptr, ".entry") == 0) {
            MOVE_TO_NOT_WHITE(line.content, charIndex);
            ptr = strtok(NULL, " \n\t");
            
            /* Step 6: Mark the corresponding symbols in the symbol table with the .ENTRY attribute, then return to step 2 */
            if (ptr == NULL) {
                printErrorAccordingToLine(line, "Error. Please specify the label name of .entry instruction.");
                return FALSE;
            }
            if (findType(*symbol_table, ptr, 1, ENTRY_SYMBOL) == NULL) {
                table_item *item;
                if (ptr[0] == '&') {
                    ptr++;
                }
                /* If the symbol is not defined as data/code */
                item = findType(*symbol_table, ptr, 2, DATA_SYMBOL, CODE_SYMBOL);
                if (item == NULL) {
                    /* If defined as external, print an error */
                    item = findType(*symbol_table, ptr, 1, EXTERNAL_SYMBOL);
                    if (item != NULL) {
                        printErrorAccordingToLine(line, "The symbol %s can be either external or entry, but not both.", item->key);
                        return FALSE;
                    }
                    /* Otherwise, print a more general error */
                    printErrorAccordingToLine(line, "The symbol %s for .entry is undefined.", ptr);
                    return FALSE;
                }
                add_item_to_table(symbol_table, item->value, ptr, ENTRY_SYMBOL);
            }
        }
        return TRUE;  /* Return to step 2 */
    }
    
    /* Step 7: Additional processing (not implemented here, but you should add your logic) */

    return TRUE;  /* If no errors occurred, return TRUE */
}
