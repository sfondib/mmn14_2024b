#ifndef NULL
#define NULL ((void*)0)
#endif

#define NUM_OPS 16

int getFirstOperandData(char *field, int *operand1_method, int *operand1);
int getSecondOperandData(char *field, int *operand2_method, int *operand2);
int checkSymbolDefinition(char *first_field);
int getDataStore(char *second_field);
int getExternEntry(char *second_field);
int getOperation(char *field);
int checkDirectAddressing(char *field);
int validateOperandCount(int op_index, char *second_field, char *third_field, char *fourth_field);
int isRegisterName(char *field, int start_offset, int end_offset);
int getRegisterNumber(char *field, int start_offset, int end_offset);