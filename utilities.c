/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int lineCounter(FILE *file)
{
    /*Counts the amount of lines in a file*/
    int count = 0;
    char c;

    /*Going back to the beginning of the file*/
    fseek(file,SEEK_SET,0);
    /*Check if the file has 1 line at least */
    if (fgetc(file)!= EOF)
        count++;
    else {
        printf("FATAL ERROR: File is empty!\n");
        exit(0);
    }
    while ((c = fgetc(file))!= EOF)
    {
        if (c == '\n')
            count++;
    }
    /*return number lines */
    return count;
}

int binToOct(int binary)
{
    /*Returns the octal representation of a binary*/
}

int decToBin(int decimal)
{
    /*Returns the binary representation of a decimal*/
}

int decToUnsignedDec(int number)
{
    /*Returns the unsigned decimal value of a given value*/
}

int dataEncoder(char* data)
{
    /*Returns the octal representation of an unsigned number*/
}

int charEncoder(char c)
{
    /*Returns the octal representation of a character ASCII code*/
}
