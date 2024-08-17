#ifndef NULL
#define NULL ((void*)0)
#endif

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

/*
After the pre-processing phase is complete go through each line and
analyze it's structure for building the data words and building
the symbol table
@param *file_name Name of the file to go through
@return 1 for failure, 0 for success
*/
int firstRun(char *file_name, instruction memory[4096]);

/*
Reset the values of the variables passed as arguments
@param count Number of variables passed
@param ... Variables passed for resetting
*/
void resetVars(int count, ...);

/*
Convert a decimal number to it's binary representation 15-bits
@param num The decimal number to convert
@param *binary_str String to hold the binary representation string
*/
void decToBin15(int num, char *binary_str);

/*
Check which instruction it is (how many operands it accepts) and check their addressing type.
If an addressing method that does not match the type is used, an error is printed.
@param op_index Index representing the operation
@param *second_field Field holding the first operand
@param *third_field Field holding the second operand
@param *operand1 To hold the value of the first operand
@param *operand2 To hold the value of the second operand
@param *operand1_method To hold the value representing the addressing method of the first operand
@param *operand1_method To hold the value representing the addressing method of the second operand
*/
int getOperandsFromInstruction(int op_index, char *second_field, char *third_field, int *operand1, int *operand2, int *operand1_method, int *operand2_method);

/*
Get the data from the first operand (The addressing method and value)
@param *field The field holding the operand
@param *operand1_method To hold the value representing the addressing method
@param *operand1 To hold the value of the operand
@param allow0 If addressing method 0 is allowed or not
@param allow1 If addressing method 1 is allowed or not
@param allow2 If addressing method 2 is allowed or not
@param allow3 If addressing method 3 is allowed or not
@return 1 for valid, 0 otherwise
*/
int getFirstOperandData(char *field, int *operand1_method, int *operand1, int allow0, int allow1, int allow2, int allow3, int comma);

/*
Get the data from the first operand (The addressing method and value)
@param *field The field holding the operand
@param *operand1_method To hold the value representing the addressing method
@param *operand1 To hold the value of the operand
@param allow0 If addressing method 0 is allowed or not
@param allow1 If addressing method 1 is allowed or not
@param allow2 If addressing method 2 is allowed or not
@param allow3 If addressing method 3 is allowed or not
@return 1 for valid, 0 otherwise
*/
int getSecondOperandData(char *field, int *operand2_method, int *operand2, int allow0, int allow1, int allow2, int allow3);

/*
Check if a line has a tag / symbol definition, if so check it's validity and return the
appropriate error code
@param *first_field The text that holds the name of the tag / symbol
@param *file_line The entire line that was read from the file
@param *char_index The index of the current character in the line
@return Error code for each case
*/
int checkSymbolDefinition(char *first_field);

/*
Check if there is an instruction to hold .data or .string
@param *data_type The data type that was read (.data or .string or something else)
@return 1 if it's .data, 2 if it's .string, 0 otherwise
*/
int getDataStore(char *second_field);

/*
Check if there is an instruction for .extern or .entry
It can be the first field or second field in the line
@param *first_field The instruction that was read as the first field
@param *second_field The instruction that was read as the second field
@return 1 if it's .extern, 2 if it's .entry, 0 otherwise
*/
int getExternEntry(char *second_field);

/*
Get the index of the operation from the list *op_names
@param *field The string that holds the operation name
@return Index of operation in *op_names
*/
int getOperation(char *field);

/*
Check if an operand matches the direct addressing type, starts with # and has an integer
after it (0, negative or positive)
@param *operand The field that holds the operand
@return 1 if direct addressing, 0 otherwise
*/
int checkDirectAddressing(char *field);

/*
Check if the operation receives the amount of operands it needs to receive.
If it's "mov" to "lea" then it should receive 2 operands.
If it's "clr" to "jsr" then it should receive 1 operand.
If it's "rts" to "stop" then it should receive no operands.
@param op_index The index of the operation from *op_names
@param *second_field The first field capable of receiving an operand
@param *third_field The second field capable of receiving an operand
@param *fourth_field The third field capable of receiving and operand (Error for most cases)
@return 1 if error found, 0 otherwise
*/
int validateOperandCount(int op_index, char *second_field, char *third_field, char *fourth_field);

/*
Check if a given field holds a valid register name
@param *field The field that holds the operand
@param start_offset How many characters to ignore at the start of the field
@param end_offset How many characters to ignore at the end of the field
@return 1 if register name, 0 otherwise
*/
int isRegisterName(char *field, int start_offset, int end_offset);

/*
Take a validated register and return it's number
@param *field The field that holds the operand
@param start_offset How many characters to ignore at the start of the field
@param end_offset How many characters to ignore at the end of the field
@return The register's number
*/
int getRegisterNumber(char *field, int start_offset, int end_offset);
