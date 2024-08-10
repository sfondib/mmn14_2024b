/* Libraries*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#includ "my_macro.h"

/**
 * Processes a single assembly source file, and returns result - if 
 * @param file - siurce file
 * @return if Process succeeded
 */

/* will meet following functions later :P */
static bool processes(char *file)

int main(int argc, char *argv[]) {
  int i;
  bool succeeded = TRUE;
  for (i = 1; i < argc; ++i) {
    if (!succeeded) 
      puts(""); /* Break line if needed, in case we got failed */
    succeeded = processes(argv[i]); /* Check all files */
  }
  return 0;
}

static bool processes(char *file) {
  /* Counters of memory addresses */
  int ic = IC_START_VALUE, dc = ZEROIZE;
  bool success = TRUE;
  char *input_file /* Pointer of current file */
	FILE *file_des; /* Current assembly file descriptor to process */

  /* Open file, skip on failure */
	file_des = fopen(input_filename, "r");
	if (file_des == NULL) {
		/* In case file couldn't get open: */
		printf("Error: file \"%s.as\" is not readable. skip and continue.\n", file);
		free(input_file);
		return FALSE;
	}
  
/**
* =========================================================================================================
* ============================================= START FIRST RUN ===========================================
* =========================================================================================================
*/
	/* Read line - stop if read failed (when NULL returned) */
	for (curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH, file_des) != NULL; curr_line_info.line_number++) {
		/* if line is too long, the buffer doesn't include char '\n' or file isn't yet on EOF. */
		if (strchr(temp_line, '\n') == NULL && !feof(file_des)) {
			/* Print message and prevent further line processing. */
			printf("Error: Line is too long, maximum length of line is %d.", MAX_LINE_LENGTH);
			success = FALSE;
			break;
		  } else צריך לכתוב את המעבר הראשון ולהוסיף תנאי מתאים למעבר מצליח
		}
  
}
