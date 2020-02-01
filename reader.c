
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

typedef struct{
    char label [MAX_INPUT];
    char operator [MAX_INPUT];
    char operand1 [MAX_INPUT];
    char operand2 [MAX_INPUT];
    int inputLine;
    int asteriskCount;
} codeLine;

int parseFile(FILE* file){
    int lineNumber = 1;
    char whiteSpace;
    char data;

    while(file != EOF){
        //read a file line and parse it
    }
}