#ifndef NULL
#define NULL ((void*)0)
#endif

int checkSymbolDefinition(char *first_field, char *file_line, int *char_index);
int getDataStore(char *data_type);
int getExternEntry(char *first_field, char *second_field);