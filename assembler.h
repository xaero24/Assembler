#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

/*Boolean operators*/
#define TRUE 1
#define FALSE 0

/*Opcodes group 1 - dual operands*/
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define LEA 4

/*Opcodes group 2 - single operand*/
#define CLR 5
#define NOT 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13

/*Opcodes group 3 - no operands*/
#define RTS 14
#define STOP 15

/*Various definitions for parsing*/
#define LINE_LEN 80
#define BLANK_LINE 0
#define COMMENT 1
#define DIRECTION 2
#define OPERATION 3
#define START_ADDR 100
#define MAX_INPUT 32
#define MEM_SIZE 4095
#define CELL_SIZE 15

/*Structures for parsing the file and building the symbol table*/
typedef struct code_line
{
    char label [MAX_INPUT];
    char operator [MAX_INPUT];
    char operand1 [MAX_INPUT];
    char operand2 [MAX_INPUT];
} codeLine;

typedef struct code_list
{
    codeLine* line;
    struct codeNode* next;
} codeNode;

typedef struct{
    char label[LINE_LEN];
    int addr;
} symTableLine;

typedef struct table_list
{
    symTableLine* line;
    struct symNode* next;
} symNode;

typedef struct{
    int addr;
    unsigned int binCode;
} outputLine;

void symTableCreator();
int parseFile(FILE*, codeNode*, symNode*);

#endif