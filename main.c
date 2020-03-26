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
    char *parsedFile;
    int pass_1 = FALSE, pass_2 = FALSE, passNumber = 1;
    int i, lineCount;
    FILE *file;

    symbolLine *symLine = NULL; /*TODO: Destroy after finishing*/
    outputLine *outLine = NULL; /*TODO: Destroy after finishing*/
    dataLine *dLine = NULL; /*TODO: Destroy after finishing*/

    if(argc==1)
    {
        printf("FATAL ERROR: No files to compile\n");
        return 0;
    }

    /*Go through the provided files and process each one*/
    for(i = 1; i < argc; i++)
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

        /*Collect the input from the file*/
        pass_1 = readLinesFirstRun(file, lineCount, symLine, outLine, dLine);
        if(pass_1){
            pass_2 = readLinesSecondRun(file, parsedFile, lineCount);
        }

        if(pass_2)
        {
            printf("Successfully processed %s.as\n", parsedFile);
        }
        else
        {
            printf("Error:\nCould not process the file: %s.as\n", parsedFile);
        }
        fclose(file);
    }

    return 0;
}