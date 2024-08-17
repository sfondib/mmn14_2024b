/*
Take a string representing a file name and go through the pre-processing process
of the assembler while reporting errors and eventually creating a file with the
macros the original file had after opening them whenever called
@param *file_name Name of the file to read
@return 1 for failure, 0 for sucess
*/
int preProcessor(char *file_name);