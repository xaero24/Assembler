
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

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
    struct node* next;
} node;

int parseFile(FILE* file){
    int lineNumber = 1;
    char whiteSpace;
    char data[256], word[20], temp;
    printf("Reading file gives:\n\n");
    while(fgets(data, 256, file)){
        /*read a file line and parse it*/
        printf("%d. %s\n", lineNumber++, data);
    }
    return 0;
}