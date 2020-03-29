/*
Authors:
Karin Yurchenko ID: 204334601
David Eliyahu Mashiah ID:318183803

In this file the definitions of different variables are kept.
Also the functions and data structures are defined in this file.
*/

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

/*A unique value string is used for signing a no-value content for later parsing*/
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
    int address;
    char type[MAX_INPUT];
    char label[MAX_INPUT];
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
int readLinesFirstRun(FILE*, int, symbolLine**, outputLine**, dataLine**, int*, int*);
int readLinesSecondRun(FILE*, int, symbolLine*, outputLine*, char*);
void createFiles(char*, char*, symbolLine*, outputLine*, dataLine*, int, int);

int lineCounter(FILE*);
int stringToUnsignedInt(char*, char);
void decToBin(int, char*);
void binToOct(char*, char*);
void getDigitFromBinary(char*, char*);
int decToUnsignedDec(int, int);
void dataEncoder(char*, char*);
void charEncoder(char, char*);
void generateInstructionCode(char*, char*, char*, char*);
void generateOperandCode(char*, int, char*, char*, int);
void generateOpcodeDualRegs(char*, char*, char*);
int operatorType(char*);
void getOperatorBinary(char*, char*);
int getOperatorNumber(char*);
void getRegNumBinary(char, char*);
int operandType(char*);

int isDigit(char);
int isSpace(char);
int isLetter(char);
int isNumber(char*, int);
int isValidString(char*);
int isLegalLabel(char*);
int isExistingLabel(char*, symbolLine*);
int isValidOperator(char*);
int isValidOperand(char*);
int isValidOperandUse(char*, char*, char*);

#endif
