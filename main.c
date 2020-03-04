
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char* argv[]){
    char fileName[256], fileExt[6];
    int fileIndex, fileNameLength;
    FILE* inFile = NULL;
    codeNode* codeHead = NULL;
    symNode* symHead = NULL;

    if(argc<2){
        printf("ERROR: No files were given.\n");
        exit(0);
    }
    for(fileIndex = 1; fileIndex < argc; fileIndex++){
        /*Run assembler on each file after error checks*/
        strcpy(fileName, argv[fileIndex]);
        strcat(fileName, ".as");
        inFile = fopen(argv[fileIndex], "r");
        if (inFile == NULL){
            printf("ERROR: File name does not exist.\n");
        }
        else{
            /*parse file*/
            codeHead = (codeNode*) malloc(siseof(codeHead));
            symHead = (symNode*) malloc(siseof(symHead));
            parseFile(inFile, codeHead, symHead);
        }
    }
    return 0;
}