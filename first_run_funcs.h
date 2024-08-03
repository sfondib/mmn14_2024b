#ifndef NULL
#define NULL ((void*)0)
#endif

#define NUM_OPS 16

int checkSymbolDefinition(char *first_field, char *file_line, int *char_index);
int getDataStore(char *first_field, char *second_field);
int getExternEntry(char *first_field, char *second_field);
int getOperation(char *first_field, char *second_field);