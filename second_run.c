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
    int operand1_method = -1, operand2_method = -1;

    if (line.content == NULL) {
        /* Error: No content provided for processing */
        fprintf(stderr, "Error: No file content provided for processing.\n");
        return FALSE;
    }
    /* Skip initial whitespace characters */
    jumpOnWhiteChar(line.content, charIndex);
    
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
    jumpOnWhiteChar(line.content, charIndex);
    
    /* Step 4: Is it a .DATA, .STRING, or EXTERN directive? If yes, return to step 2 */
    if (line.content[charIndex] == '.') {
        ptr = strtok(line.content + charIndex, " \n\t");
        
        if (strcmp(ptr, ".data") == 0 || strcmp(ptr, ".string") == 0 || strcmp(ptr, ".extern") == 0) {
            /* Skip this line and move to the next line */
            return TRUE;  /* Return to step 2 */
        }
        /* Skip additional whitespace characters */
        jumpOnWhiteChar(line.content, charIndex);
        
        /* Step 5: Is it an ENTRY directive? If not, go to step 7 */
        if (strcmp(ptr, ".entry") == 0) {
            /* Skip to the next field after .entry */
            jumpOnWhiteChar(line.content, charIndex);
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
    
    /* Step 7: Encode the operands starting from the second word in the binary code of the instruction */
    if (!encodeOperands(line, ic, symbol_table, &operand1_method, &operand2_method)) {
        return FALSE;
    }

    /* Step 8: Update IC and return to step 2 */
    *ic += calculateInstructionLength(operand1_method, operand2_method);
    return TRUE;
}

/**
 * Process the entire source file in the second pass.
 * @param file - Pointer to the source file to process.
 * @param file_name - The name of the source file.
 * @param symbol_table - Pointer to the symbol table.
 * @param memory - Pointer to the instruction memory.
 * @param dc - The data counter value.
 * @return TRUE if processing is successful, FALSE if errors occurred.
 */
bool processFileSecondPass(FILE *file, const char *file_name, table *symbol_table, instruction *memory, long dc) {
    char line_content[MAX_LINE_LEN];
    long ic = IC_START_VALUE;  /* Initialize IC once at the beginning of processing */
    line_info current_line;
    int line_number = 0;
    bool success = TRUE;  /* Track if any errors occur during processing */

    /* Set the file name in the line_info structure */
    current_line.file_name = (char *)file_name;  /* Necessary for error reporting */

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
    if (success) {
        if (!createObjectFile(file_name, memory, ic, dc)) {
            fprintf(stderr, "Error: Failed to create the object file.\n");
            return FALSE;
        }
    } else {
        fprintf(stderr, "Errors were found during the second pass. Stopping.\n");
        return FALSE;
    }

    return TRUE;
}

/**
 * Encodes the operands of an instruction during the second pass.
 * @param line - The line information, including content, file name, and line number.
 * @param ic - A pointer to the instruction counter.
 * @param symbol_table - A pointer to the symbol table.
 * @param operand1_method - Pointer to store the addressing method of the first operand.
 * @param operand2_method - Pointer to store the addressing method of the second operand.
 * @return TRUE if encoding is successful, FALSE if an error occurs.
 */
bool encodeOperands(line_info line, long *ic, table *symbol_table, int *operand1_method, int *operand2_method) {
    int operand1, operand2;
    char *token;
    
    /* Extract operands and methods from the line content */
    token = strtok(line.content, " \t");
    token = strtok(NULL, " \t"); /* Skip operation */
    
    /* First operand */
    if (!getFirstOperandData(token, operand1_method, &operand1, TRUE, TRUE, TRUE, TRUE, FALSE)) {
        printErrorAccordingToLine(line, "Error: Invalid first operand.");
        return FALSE;
    }
    
    token = strtok(NULL, " ,\t");
    
    /* Second operand */
    if (token != NULL && !getSecondOperandData(token, operand2_method, &operand2, TRUE, TRUE, TRUE, TRUE)) {
        printErrorAccordingToLine(line, "Error: Invalid second operand.");
        return FALSE;
    }
    
    /* Encoding logic based on operand methods and updating the instruction counter */
    
    return TRUE;
}

/**
 * Creates the .ob file by writing the encoded instructions and data to it.
 * @param file_name - The base name of the source file.
 * @param memory - The memory structure holding the encoded instructions.
 * @param ic - The final instruction counter value.
 * @param dc - The final data counter value.
 * @return TRUE if the file is created successfully, FALSE if an error occurs.
 */
bool createObjectFile(const char *file_name, instruction *memory, long ic, long dc) {
    char ob_file_name[MAX_FILE_NAME_LEN];
    FILE *ob_file;
    int i;
    char binary_str[16];  /* String to store binary conversion */
    long address;  /* כדי לוודא שהכתובת נשמרת כסוג long */

    /* Create the .ob file name by adding the extension */
    sprintf(ob_file_name, "%s.ob", file_name);

    /* Open the .ob file for writing */
    ob_file = fopen(ob_file_name, "w");
    if (ob_file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s for writing.\n", ob_file_name);
        return FALSE;
    }

    /* Write the instruction count and data count at the top */
    fprintf(ob_file, "%ld %ld", ic - IC_START_VALUE, dc);

    /* Iterate over the instruction memory and write the encoded instructions to the file */
    for (i = 0; i < ic - IC_START_VALUE; i++) {
        /* Convert the full field to a binary string */
        decToBin15(memory[i].full, binary_str);
        address = IC_START_VALUE + i;
        fprintf(ob_file, "%.4ld %.5o\n", address, (unsigned)memory[i].full);
    }

    /* Write the data image */
    for (i = 0; i < dc; i++) {
        /* Convert the full field to a binary string */
        decToBin15(memory[ic + i].full, binary_str);
        address = IC_START_VALUE + ic + i;
        /* Write the binary string to the file */
        fprintf(ob_file, "%.4ld %.5o\n", address, (unsigned)memory[ic + i].full);
    }

    /* Close the file */
    fclose(ob_file);
    return TRUE;
}
