#include <stdio.h>
#include "helpful.h"
#include "my_macro.h"
#include "table.h"
#include "pre_processor_funcs.h"
#include "second_run.h"

bool lineIndexPass(line_info line, long *ic,*******need to complite and make it right **********) {
	char colonIndex = ZEROIZE;    /* counter for colon index */
	long charIndex = ZEROIZE;     /* counter for line index */
	char *ptr; 
	

	/* step number 1: check if file passed the first run */
	long *ic = ICIC_START_VALUE 
	if (line.content == NULL) {
        fprintf(stderr, "Error: No file content provided for processing.\n");
        return FALSE;
    	}
	
	MOVE_TO_NOT_WHITE(line.content, charIndex)
	/* Step 2: Read the next line from the source file. If end of file, go to step 9 */
	/* Continue if line is empty or have just '\n' in content */
	if (line.content[charIndex] == ';' || line.content[charIndex] == '\n')
		return TRUE;
	
	/* Step 3: If the first field is a label, skip it */
	indexOfColon = strchr(line.content, ':');          /* To find the label */
	if (indexOfColon != NULL) {
		charIndex = indexOfColon - line.content;         /* In order to caclulate space in mamory */
		charIndex++;                                     /* to get one index forward to the line content */
	}
	MOVE_TO_NOT_WHITE(line.content, charIndex)          /* skip again (in needed) white chars */
	
	/* Step 4: Is it a .DATA, .STRING, or EXTERN directive? If yes, return to step 2 */
	if (line.content[charIndex] == '.') {
  	/* Extract the directive type */
    	ptr = strtok(line.content + charIndex, " \n\t");
	
    	/* Step 5: Is it an ENTRY directive? If not, go to step 7 */
    	if (strcmp(ptr, ".entry") == 0) {
        	MOVE_TO_NOT_WHITE(line.content, charIndex)
        	ptr = strtok(NULL, " \n\t");
	
        /* Step 6: Mark the corresponding symbols in the symbol table with the .ENTRY attribute, then return to step 2 */
        if (ptr == NULL) {
            printf_line_error(line, "Error. Please specify label name of .entry instruction.");
            return FALSE;
        }
	}
	    return TRUE; /* Return to step 2 */
	}



/**
* Descrition of the file
*
* @param ........
* @param .........
*/
