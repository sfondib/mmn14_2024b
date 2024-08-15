#ifndef NULL
#define NULL ((void*)0)
#endif

#define NUM_OPS     16
#define MAX_VAL_15  16383   /* -2¹⁴ - 1 */
#define MIN_VAL_15  -16384  /* -2¹⁴ */
#define MAX_VAL_DC  100
#define MAX_VAL_IC  4096

/*
4 fields:
ARE                     - 3 bits
Addressing destination  - 4 bits
Addressing source       - 4 bits
Operation code          - 4 bits
*/
typedef struct {
    unsigned int are         : 3;
    unsigned int add_dest    : 4;
    unsigned int add_src     : 4;
    unsigned int op_code     : 4;
} bword4;

/*
3 fields:
ARE                     - 3 bits
Addressing shared 1     - 3 bits (Shared between destination and source)
Addressing shared 2     - 3 bits (Shared between destination and source)
Blank                   - 2 bits (Not used)
Operation code          - 4 bits
*/
typedef struct {
    unsigned int are         : 3;
    unsigned int add_share1  : 3;
    unsigned int add_share2  : 3;
    unsigned int blank       : 2;
    unsigned int op_code     : 4;
} bword5;

/*
2 fields:
ARE                     - 3 bits
Operand                 - 12 bits (The value of the operand itself)
*/
typedef struct {
    unsigned int are         : 3;
    int operand              : 12;
} bword2;

/*
3 kinds:
fields4                 - 4 fields
fields 3                - 3 fields
full                    - 15 bits (1 field for when saving numbers for .data and characters for .string)
*/
typedef union {
    bword4 fields4;
    bword5 fields5;
    bword2 fields2;
    int full        : 15;
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
