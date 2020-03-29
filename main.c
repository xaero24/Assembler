/**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char* argv[])
{
    char inputFile[FILENAME_MAX];
    char ent[MAX_INPUT], ext[MAX_INPUT], ob[MAX_INPUT];
    char *parsedFile;
    int pass_1 = FALSE, pass_2 = FALSE;
    int i, lineCount;
    int IC, DC;
    FILE *file;

    symbolLine *symLine = NULL;
    outputLine *outLine = NULL;
    dataLine *dLine = NULL;

    symbolLine *sNode = NULL;
    outputLine *oNode = NULL;
    dataLine *dNode = NULL;

    /*No provided files will immediately fail the program*/
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

        /*Files are to be given without the .as extension*/
        file = fopen (inputFile, "r");
        if (!file)
        {
            printf("File %s doesn't exist.\n", inputFile);
            return 0;
        }
        lineCount = lineCounter(file);
        strcpy(ext, argv[i]);
        strcat(ext, ".ext");

        /*Collect the input from the file*/
        pass_1 = readLinesFirstRun(file, lineCount, &symLine, &outLine, &dLine, &IC, &DC);
        if(pass_1){
            pass_2 = readLinesSecondRun(file, lineCount, symLine, outLine, ext);
        }
        else printf("Error: File %s failed on first pass\n", parsedFile);

        if(pass_2)
        {  
            /*Create files from the output*/
            strcpy(ent, argv[i]);
            strcat(ent, ".ent");
            strcpy(ob, argv[i]);
            strcat(ob, ".ob");
            createFiles(ent, ob, symLine, outLine, dLine, IC, DC);
            printf("Successfully processed %s.as\n\n", parsedFile);
        }
        else
        {
            /*Remove the created .ext file*/
            remove(ext);
            printf("Error:\nCould not process the file: %s.as\n\n", parsedFile);
        }
        fclose(file);

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
    }
    return 0;
}