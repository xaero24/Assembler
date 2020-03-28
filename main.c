/**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

/*

TODO: Remove all todos

*/

int main(int argc, char* argv[])
{
    char inputFile[FILENAME_MAX];
    char ent[MAX_INPUT], ext[MAX_INPUT], ob[MAX_INPUT];
    char *parsedFile;
    int pass_1 = FALSE, pass_2 = FALSE;
    int i, lineCount;
    int IC, DC;
    FILE *file;

    symbolLine *symLine = NULL; /*TODO: Destroy after finishing*/
    outputLine *outLine = NULL; /*TODO: Destroy after finishing*/
    dataLine *dLine = NULL; /*TODO: Destroy after finishing*/

    symbolLine *sNode = NULL; /*TODO: Destroy after finishing*/
    outputLine *oNode = NULL; /*TODO: Destroy after finishing*/
    dataLine *dNode = NULL; /*TODO: Destroy after finishing*/

    if(argc==1)
    {
        printf("FATAL ERROR: No files to compile\n");
        return 0;
    }

    /*Go through the provided files and process each one*/
    for(i = 1; i<argc; i++)
    {
        parsedFile = argv[i];
        strcpy(inputFile, argv[i]);
        strcat(inputFile, ".as");

        file = fopen (inputFile, "r");
        if (!file)
        {
            printf("File %s doesn't exist.\n", inputFile);
            return 0;
        }
        lineCount = lineCounter(file);
        printf("    Lines: %d\n", lineCount);

        /*Collect the input from the file*/
        pass_1 = readLinesFirstRun(file, lineCount, &symLine, &outLine, &dLine, &IC, &DC);
        if(pass_1){
            printf("File %s completed first pass\n", parsedFile);
            printf("IC is: %d, DC is %d\n", IC, DC);
            pass_2 = readLinesSecondRun(file, lineCount, symLine, dLine);
        }
        else printf("Error: File %s failed on first pass\n", parsedFile);

        if(pass_2)
        {  
            /*Create files from the output*/
            strcpy(ent, argv[i]);
            strcat(ent, ".ent");
            strcpy(ext, argv[i]);
            strcat(ext, ".ex");
            strcpy(ob, argv[i]);
            strcat(ob, ".ob");
            createOutputFiles(ent, ext, ob, symLine, outLine, dLine, IC, DC);
            printf("Successfully processed %s.as\n", parsedFile);
        }
        else
        {
            printf("Error:\nCould not process the file: %s.as\n", parsedFile);
        }
        fclose(file);
    }

    /*Free all the structures that use dynamic memory*/
    sNode = symLine;
    while(symLine)
    {
        sNode = symLine->next;
        free(symLine);
        symLine = sNode;
    }
    oNode = outLine;
    while(outLine)
    {
        oNode = outLine->next;
        free(outLine);
        outLine = oNode;
    }
    dNode = dLine;
    while(dLine)
    {
        dNode = dLine->next;
        free(dLine);
        dLine = dNode;
    }
    return 0;
}