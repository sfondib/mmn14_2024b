#ifndef NULL
#define NULL ((void*)0)
#endif

#define NUM_OPS     16
#define MAX_VAL_15  16383
#define MIN_VAL_15  -16384
#define MAX_VAL_DC  100
#define MAX_VAL_IC  4096

typedef struct {
    int are       : 3;
    int add_dest  : 4;
    int add_src   : 4;
    int op_code   : 4;
} bword4;

typedef struct {
    int are       : 3;
    int add_share : 8;
    int op_code   : 4;
} bword3;

typedef union {
    bword4 fields4;
    bword3 fields3;
    int full : 15;
} instruction;

void decToBin15(int num, char* binary_str);
int getOperandsFromInstruction(int op_index, char *second_field, char *third_field, int *operand1, int *operand2, int *operand1_method, int *operand2_method);
int getFirstOperandData(char *field, int *operand1_method, int *operand1, int allow0, int allow1, int allow2, int allow3, int comma);
int getSecondOperandData(char *field, int *operand2_method, int *operand2, int allow0, int allow1, int allow2, int allow3);
int checkSymbolDefinition(char *first_field);
int getDataStore(char *second_field);
int getExternEntry(char *second_field);
int getOperation(char *field);
int checkDirectAddressing(char *field);
int validateOperandCount(int op_index, char *second_field, char *third_field, char *fourth_field);
int isRegisterName(char *field, int start_offset, int end_offset);
int getRegisterNumber(char *field, int start_offset, int end_offset);
