/**/

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

/*Defining the parts of instruction in use*/
#define OPERATOR 0
#define OPERAND1 1
#define OPERAND2 2
#define OPERAND_3_AND_UP 3

/*Defining the operands in use*/
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define LEA 4
#define CLR 5
#define NOT 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

/*Definition of various checks*/
#define NO_VALUE "0-NIL"

/*Definitions of max length for the code parts*/
#define MAX_INPUT 31
#define MAX_OPERATOR 5
#define MAX_STRING 81
#define MAX_LINE_LENGTH 81
#define BIN_NUM 16

/*Defining addressing methods*/
#define IMMEDIATE 0
#define DIRECT 1
#define REG_INDIRECT 2
#define REG_DIRECT 3

/*Defining operand types*/
#define NUMBER 0
#define LABEL 1
#define INDIR_REGISTER 2
#define DIR_REGISTER 3

/*Defining operator groups*/
#define GROUP_1 1
#define GROUP_2 2
#define GROUP_3 3

/*True and false statements definition*/
#define TRUE 1
#define FALSE 0

/*Symbol table structure*/
typedef struct sym_line
{
    char type[MAX_INPUT];
    char label[MAX_INPUT];
    int address;
    struct sym_line *next;
} symbolLine;

/*Data table structure*/
typedef struct data_line
{
    int address;
    char code[MAX_INPUT];
    struct data_line *next;
} dataLine;

/*Output table structure*/
typedef struct out_line
{
    int address;
    char code[MAX_INPUT];
    struct out_line *next;
} outputLine;

/*Function definitions*/
int readLinesFirstRun(FILE* file, int lineCount, symbolLine** symLine, outputLine** outLine, dataLine** dLine, int*, int*);
int readLinesSecondRun(FILE* file, int lineCount, symbolLine* symLine, dataLine* dLine);

int lineCounter(FILE *file);
int stringToUnsignedInt(char* string);
void decToBin(int decimal, char* res);
void binToOct(char* binary, char* result);
void getDigitFromBinary(char* binary, char* target);
int decToUnsignedDec(int number);
void dataEncoder(char* data, char* result);
void charEncoder(char c, char* result);
void generateInstructionCode(char* operator, char* src, char* dest, char* result);
void generateOperandCode(char* operand, int operandPlace, char* are, char* result, int lblAddr);
void generateOpcodeDualRegs(char* reg1, char* reg2, char* result);
int operatorType(char* operator);
void getOperatorBinary(char* operator, char* result);
int getOperatorNumber(char* operator);
void getRegNumBinary(char regNum, char* result);
int operandType(char* operand);

int isDigit(char x);
int isSpace(char x);
int isLetter(char x);
int isNumber(char* word, int index);
int isValidString(char* str);
int isLegalLabel(char* label);
int isExistingLabel(char* label, symbolLine* head);
int isValidOperator(char* operator);
int isValidOperand(char* operand);
int isValidOperandUse(char* operator, char* op1, char* op2);

#endif
