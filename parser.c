
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

typedef struct{
    char lineChar[LINE_LEN];
    int addr;
    int dataAddr;
} symTableLine;

typedef struct{
    int addr;
    unsigned int binCode;
} outputLine;

void symTableCreator(){
    printf("Creating symTable\n");
}