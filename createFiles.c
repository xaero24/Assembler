/**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

void createFiles(char* ent, char* ext, char* ob, symbolLine* symLine, outputLine* outLine, dataLine* dLine, int IC, int DC)
{
    int minAddr;
    char written[MAX_INPUT];
    char stringIC[4], stringDC[4];
    symbolLine* symNode = symLine;
    outputLine* outNode = outLine;
    dataLine* dNode = dLine;
    FILE *entFile, *extFile, *obFile;
    sprintf(stringIC, "%d", IC);
    sprintf(stringDC, "%d", DC);

    entFile = fopen(ent, "w");
    while(symNode)
    {
        if( strcmp(symNode->type, "data-entry")==0 ||
            strcmp(symNode->type, "code-entry")==0 )
        {
            strcpy(written, symNode->label);
            strcat(written, " ");
            strcat(written, symNode->address);
            fputs(written, entFile);
            putc('\n', entFile);
        }
        symNode = symNode->next;
    }
    fclose(entFile);

    extFile = fopen(ext, "w");
    while(symNode)
    {
        if(strcmp(symNode->type, "external")==0)
        {
            strcpy(written, symNode->label);
            strcat(written, " ");
            strcat(written, symNode->address);
            fputs(written, extFile);
            putc('\n', extFile);
        }
        symNode = symNode->next;
    }
    fclose(extFile);

    obFile = fopen(ob, "w");
        strcpy(written, "   ");
        strcat(written, stringIC);
        strcat(written, " ");
        strcat(written, stringIC);
        fputs(written, obFile);
        putc('\n', obFile);
    while(outNode)
    {
        strcpy(written, outNode->address);
        strcat(written, " ");
        strcat(written, outNode->code);
        fputs(written, obFile);
        putc('\n', obFile);

        outNode = outNode->next;
    }
    while(dNode)
    {
        strcpy(written, dNode->address);
        strcat(written, " ");
        strcat(written, dNode->code);
        fputs(written, obFile);
        putc('\n', obFile);
        
        dNode = dNode->next;
    }
    fclose(obFile);
}