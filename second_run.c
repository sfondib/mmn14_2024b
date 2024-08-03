#include <stdio.h>
#include "helpful.h"
#include "my_macro.h"
#include "table.h"
#include "pre_processor_funcs.h"
#include "second_pass.h"

bool lineIndexPass(line_info line, long *ic,*******need to complite and make it right **********) {
	char colonIndex = zeroize;    /* counter for colon index */
	long charIndex = zeroize;     /* counter for line index */
	
	/* step number 1: check if file pass the first run */
	if (line.content == NULL) {
        fprintf(stderr, "Error: No file content provided for processing.\n");
        return FALSE;
    }
	
	MOVE_TO_NOT_WHITE(line.content, charIndex)
	
	/* Continue if line is empty or have just '\n' in content */
	if (line.content[charIndex] == ';' || line.content[charIndex] == '\n')
		return TRUE;
	indexOfColon = strchr(line.content, ':');          /* To find the label */
	if (indexOfColon != NULL) {
		charIndex = indexOfColon - line.content;         /* In order to caclulate space in mamory */
		charIndex++;                                     /* to get one index forward to the line content */
	}
	MOVE_TO_NOT_WHITE(line.content, charIndex)          /* skip again (in needed) white chars */









/**
* Descrition of the file
*
* @param ........
* @param .........
*/
