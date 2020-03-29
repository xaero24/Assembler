/*
Authors:
Karin Yurchenko ID: 204334601
David Eliyahu Mashiah ID:318183803

This file handles the creation of the output files according to the given input file/s. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

void createFiles(char* ent, char* ob, symbolLine* symLine, outputLine* outLine, dataLine* dLine, int IC, int DC)
{
    /*This function handles the creation of the output files.*/
    char written[MAX_INPUT];
    char stringIC[4], stringDC[4];
    char addr[5], octCode[6];
    symbolLine* symNode = symLine;
    outputLine* outNode = outLine;
    dataLine* dNode = dLine;
    FILE *entFile, *obFile;
    sprintf(stringIC, "%d", IC);
    sprintf(stringDC, "%d", DC);

    /*Entry file creation using the symbol table*/
    entFile = fopen(ent, "w");
    while(symNode)
    {
        if( strcmp(symNode->type, "data-entry")==0 ||
            strcmp(symNode->type, "code-entry")==0 )
        {
            sprintf(addr, "%04d", symNode->address);
            strcpy(written, symNode->label);
            strcat(written, " ");
            strcat(written, addr);
            fputs(written, entFile);
            putc('\n', entFile);
        }
        symNode = symNode->next;
    }
    fclose(entFile);

    /*Object file creation using the codeframe and the dataframe*/
    obFile = fopen(ob, "w");
    strcpy(written, "   ");
    strcat(written, stringIC);
    strcat(written, " ");
    strcat(written, stringDC);
    fputs(written, obFile);
    putc('\n', obFile);
    while(outNode)
    {
        binToOct(outNode->code, octCode);
        sprintf(addr, "%04d", outNode->address);
        strcpy(written, addr);
        strcat(written, " ");
        strcat(written, octCode);
        fputs(written, obFile);
        putc('\n', obFile);

        outNode = outNode->next;
    }
    while(dNode)
    {
        binToOct(dNode->code, octCode);
        sprintf(addr, "%04d", dNode->address);
        strcpy(written, addr);
        strcat(written, " ");
        strcat(written, octCode);
        fputs(written, obFile);
        putc('\n', obFile);

        dNode = dNode->next;
    }
    fclose(obFile);
}
