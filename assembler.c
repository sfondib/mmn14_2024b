/* Libraries*/
#include <stdio.h>
#include <string.h>
#includ "my_macro.h"

/* will meet following functions later :P */
static bool right_process(char *file)

int main(int argc, char *argv[])
{
  int i;
  bool succeeded = TRUE;
  for (i = 1; i < argc; ++i) {
    if (!succeeded) 
      puts(""); /* break line if needed, if we got failed */
    succeeded = right_process(argv[i]); /* Check all files */
  }
  return 0;
}

static bool right_process(char *file)
{
  long ic = 


}
