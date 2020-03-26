/**/

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

/*Defining the operands in use*/
#define OPERAND1 1
#define OPERAND2 2
#define OPERAND_3_AND_UP 3

/*Definition of various checks*/
#define NO_VALUE "0-NIL"

/*Definitions of max length for the code parts*/
#define MAX_INPUT 31
#define MAX_OPERATOR 5
#define MAX_STRING 81
#define MAX_LINE_LENGTH 256

/*Defining operator types*/
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

/*Defining operator groups*/
#define GROUP_1 1
#define GROUP_2 2
#define GROUP_3 3

/*True and false statements definition*/
#define TRUE 1
#define FALSE 0

/*Symbol table structure*/
typedef struct
{
    char type[MAX_INPUT];
    char label[MAX_INPUT];
    int address;
    symbolLine *next;
} symbolLine;

/*Data table structure*/
typedef struct
{
    int address;
    unsigned int code;
    dataLine *next;
} dataLine;

/*Output table structure*/
typedef struct
{
    int address;
    unsigned int code;
    outputLine *next;
} outputLine;

/*Function definitions*/
int readLinesFirstRun(FILE*, int, symbolLine*, outputLine*, dataLine*);
int readLinesSecondRun(FILE*, char*, int);
int lineCounter(FILE*);
int isValidOperator(char*);
int isValidOperand(char*);
int isValidOperandUse(char*, char*, char*);
int isValidAddressing(char*);
int operatorType(char*);
int checkLabelExistence(char*, symbolLine*);
int addressingType(char*, char*);
int isDigit(char);
int isSpace(char);
int isLetter(char);

#endif
