#include "second_run.h"

/**
 * Processes a single line during the second pass of the assembler.
 * @param line - The line information, including content, file name, and line number.
 * @param ic - A pointer to the instruction counter.
 * @param symbol_table - A pointer to the symbol table.
 * @return TRUE if processing is successful, FALSE if an error occurs.
 */
bool lineIndexPass(line_info line, long *ic, table *symbol_table) {
    long charIndex = ZEROIZE;  /* Counter for line index */
    char *ptr;  /* Pointer for string operations */
    char *indexOfColon = NULL;  /* Pointer to locate a colon in the line */

    /* Step 1: Initialize IC and check if the file passed the first run */
    *ic = IC_START_VALUE;
    if (line.content == NULL) {
        /* Error: No content provided for processing */
        fprintf(stderr, "Error: No file content provided for processing.\n");
        return FALSE;
    }
    /* Skip initial whitespace characters */
    MOVE_TO_NOT_WHITE(line.content, charIndex);
    
    /* Step 2: Read the next line from the source file. If end of file, go to step 9 */
    /* Check if the line is a comment or empty */
    if (line.content[charIndex] == ';' || line.content[charIndex] == '\n')
        return TRUE;
    
    /* Step 3: If the first field is a label, skip it */
    indexOfColon = strchr(line.content, ':');  /* Find the label (if present) */
    if (indexOfColon != NULL) {
        charIndex = indexOfColon - line.content;  /* Calculate the position in memory */
        charIndex++;  /* Move one index forward to skip the colon */
    }
    /* Skip any additional whitespace characters */
    MOVE_TO_NOT_WHITE(line.content, charIndex);
    
    /* Step 4: Is it a .DATA, .STRING, or EXTERN directive? If yes, return to step 2 */
    if (line.content[charIndex] == '.') {
        ptr = strtok(line.content + charIndex, " \n\t");
        
        if (strcmp(ptr, ".data") == 0 || strcmp(ptr, ".string") == 0 || strcmp(ptr, ".extern") == 0) {
            /* Skip this line and move to the next line */
            return TRUE;  /* Return to step 2 */
        }
        /* Skip additional whitespace characters */
        MOVE_TO_NOT_WHITE(line.content, charIndex);
        
        /* Step 5: Is it an ENTRY directive? If not, go to step 7 */
        if (strcmp(ptr, ".entry") == 0) {
            /* Skip to the next field after .entry */
            MOVE_TO_NOT_WHITE(line.content, charIndex);
            ptr = strtok(NULL, " \n\t");
            
            /* Step 6: Mark the corresponding symbols in the symbol table with the .ENTRY attribute, then return to step 2 */
            if (ptr == NULL) {
                /* Error: .entry directive requires a label name */
                printErrorAccordingToLine(line, "Error: .entry directive requires a label name.");
                return FALSE;
            }
            if (findType(*symbol_table, ptr, 1, ENTRY_SYMBOL) == NULL) {
                table_item *item;
                if (ptr[0] == '&') {
                    /* If the label starts with &, skip this character */
                    ptr++;
                }
                /* Check if the symbol is defined as data or code */
                item = findType(*symbol_table, ptr, 2, DATA_SYMBOL, CODE_SYMBOL);
                if (item == NULL) {
                    /* If the symbol is defined as external, print an error */
                    item = findType(*symbol_table, ptr, 1, EXTERNAL_SYMBOL);
                    if (item != NULL) {
                        /* Error: A symbol cannot be both external and entry */
                        printErrorAccordingToLine(line, "Error: The symbol %s cannot be both external and entry.", item->key);
                        return FALSE;
                    }
                    /* Error: The symbol is undefined */
                    printErrorAccordingToLine(line, "Error: The symbol %s for .entry is undefined.", ptr);
                    return FALSE;
                }
                /* Add the symbol to the table with the ENTRY_SYMBOL type */
                addItemToTable(symbol_table, item->value, ptr, ENTRY_SYMBOL);
            }
        }
        return TRUE;  /* Return to step 2 */
    }
    
    /* Step 7: need to complite */

    return TRUE;  /* If no errors occurred, return TRUE */
}

/**
 * Process the entire source file in the second pass.
 * @param file - Pointer to the source file to process.
 * @param file_name - The name of the source file.
 * @param symbol_table - Pointer to the symbol table.
 * @return TRUE if processing is successful, FALSE if errors occurred.
 */
bool processFileSecondPass(FILE *file, const char *file_name, table *symbol_table) {
    char line_content[MAX_LINE_LEN];
    long ic = IC_START_VALUE;  /* Step 1: Initialize IC */
    line_info current_line;
    int line_number = 0;
    bool success = TRUE;  /* Track if any errors occur during processing */

    /* Set the file name in the line_info structure */
    current_line.file_name = (char *)file_name;

    /* Step 2: Read the next line from the source file. If end of file, go to step 9 */
    while (fgets(line_content, MAX_LINE_LEN, file) != NULL) {
        line_number++;
        current_line.line_number = line_number;
        current_line.content = line_content;

        /* Process the current line */
        if (!lineIndexPass(current_line, &ic, symbol_table)) {
            /* If an error occurs in processing the line, track it */
            success = FALSE;
        }
    }

    /* Step 9: If the entire source file is read */
    if (!success) {
        /* Errors were found during the second pass */
        fprintf(stderr, "Errors were found during the second pass. Stopping.\n");
        return FALSE;
    }

    return TRUE;
}
