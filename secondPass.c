/*
Authors:
Karin Yurchenko ID: 204334601
David Eliyahu Mashiah ID:318183803

This file gets the data from the first file read and adds missing label addresses and entry points.
Another function of this file is to create the .ext file and populate it.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int readLinesSecondRun(FILE* file, int lineCount, symbolLine* symLine, outputLine* outLine, char* pfile)
{
    int x = 0, i = 0, j = 0, IC = 100;
    int labelFlag, contFlag, regFlag = FALSE;
    int lblAddr = 0;
    int pass = TRUE;
    char s;
    char are[] = "000";
    char temp[MAX_INPUT], word[MAX_INPUT], addr[MAX_INPUT], written[MAX_INPUT];
    char code[16];
    symbolLine* symNode;
    outputLine* outNode;
    FILE* extFile;

    extFile = fopen(pfile, "w");

    /*Set file pointer to start of file*/
    fseek(file, 0, SEEK_SET);

    /*Read a line from the file*/
    while(i<lineCount)
    {
        regFlag = FALSE;
        contFlag = FALSE;
        s = fgetc(file);

        /*Skip all whitespaces in the beginning of the line and check if line is empty*/
        if(s=='\n' || s==EOF)
        {
            i++;
            continue;
        }
        while(isSpace(s))
        {
            s = fgetc(file);
            if (s=='\n' || s==EOF){
                contFlag = TRUE;
                break;
            }
        }
        if(contFlag)
        {
            i++;
            continue;
        }

        /*A commented line is not parsed and thus ignored*/
        if (s==';')
        {
            while (s!='\n' && s!=EOF)
            {
                s = fgetc(file);
            }
            i++;
            continue;
        }

        /*All other lines are parsed according to the algorithm*/
        else
        {
            while (s!='\n' && s!=EOF)
            {
                j = 0;
                /*Ignore all whitespaces and commas*/
                while(isSpace(s) || s==',')
                {
                    s = fgetc(file);
                }

                /*Read a single word and save it to the temporary variable*/
                while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && j<MAX_INPUT)
                {
                    word[j++] = s;
                    s = fgetc(file);
                }
                word[j] = '\0';

                if(word[j-1]==':')
                {
                    /*We found a label, this time we ignore it.*/
                    continue;
                }

                if (word[0]=='.')
                {
                    /*The current word is a .something instruction.
                    Handle only .entry in the second run.*/

                    if (strcmp(".extern", word)==0||
                        strcmp(".data", word)==0 ||
                        strcmp(".string", word)==0 )
                    {
                        /*Not handled in the second run*/
                        while (s!='\n' && s!=EOF)
                        {
                            s = fgetc(file);
                        }
                        break;
                    }

                    /*Check if entry*/
                    if (strcmp(".entry", word)==0)
                    {
                        while(s!='\n' && s!=EOF){
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Parse app the labels of the .entry command*/
                            x = 0;
                            while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && x<MAX_INPUT)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            /*For each label, check if in symbol table*/
                            labelFlag = FALSE;
                            symNode = symLine;
                            while(symNode)
                            {
                                if(strcmp(symNode->label, temp)==0)
                                {
                                    /*If yes - add to each label the "entry" parameter as the type*/
                                    labelFlag = TRUE;
                                    strcat(symNode->type, "-entry");
                                }
                                symNode = symNode->next;
                            }
                            if(labelFlag==FALSE){
                                pass = FALSE;
                                printf("Line %d: Label \"%s\" doesn't exist in symbol table\n", i+1, temp);
                            }
                        }
                        continue;
                    }
                }

                else
                {
                    /*This is an operator.*/
                    IC++;
                    while(s!='\n' && s!=EOF){
                        /*Ignore all whitespaces and commas*/
                        while(isSpace(s) || s==',')
                        {
                            s = fgetc(file);
                        }
                        /*Get the next operand*/
                        x = 0;
                        while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && j<MAX_INPUT)
                        {
                            temp[x++] = s;
                            s = fgetc(file);
                        }
                        temp[x] = '\0';

                        /*If the operand is a register, we check for a second register immediately after it.
                        This code section is used to easily write the external labels with their IC values in the code.*/
                        if(operandType(temp)==DIR_REGISTER || operandType(temp)==INDIR_REGISTER)
                        {
                            if(regFlag==TRUE) regFlag = FALSE;
                            else
                            {
                                regFlag = TRUE;
                                IC++;
                            }
                        }
                        else
                        {
                            regFlag = FALSE;
                            IC++;
                        }

                        /*For each label the assembler encounters, it will check whether it's an internal or external label and will produce a binary code for it.*/
                        if(operandType(temp)==LABEL)
                        {
                            labelFlag = FALSE;
                            symNode = symLine;
                            while(symNode)
                            {
                                if(strcmp(symNode->label, temp)==0)
                                {
                                    labelFlag = TRUE;
                                    if(!strcmp(symNode->type, "external"))
                                    {
                                        /*External labels have 14 zeros and a single 1*/
                                        strcpy(are, "001");
                                        lblAddr = 0;

                                        /*Writing the label and its address to the file*/
                                        sprintf(addr, "%04d", IC-1);
                                        strcpy(written, symNode->label);
                                        strcat(written, " ");
                                        strcat(written, addr);
                                        fputs(written, extFile);
                                        putc('\n', extFile);
                                    }
                                    else if(!strcmp(symNode->type, "data-entry") ||
                                            !strcmp(symNode->type, "code-entry") ||
                                            !strcmp(symNode->type, "data") ||
                                            !strcmp(symNode->type, "code"))
                                    {
                                        /*Any other label gets its code address and a 010 ARE value*/
                                        strcpy(are, "010");
                                        lblAddr = symNode->address;
                                    }
                                }
                                symNode = symNode->next;
                            }
                            if(labelFlag==FALSE){
                                pass = FALSE;
                                printf("Line %d: Code Error: Label \"%s\" doesn't exist in symbol table\n", i+1, temp);
                            }

                            /*Generating the new code for the updated label with the address*/
                            generateOperandCode(temp, OPERAND1, are, code, lblAddr);

                            outNode = outLine;
                            while(outNode)
                            {
                                if(strcmp(outNode->code, NO_VALUE)==0)
                                {
                                    strcpy(outNode->code, code);
                                    break;
                                }
                                outNode = outNode->next;
                            }
                        }
                    }
                }
            }
        }
        i++;
    }
    return pass;
}
