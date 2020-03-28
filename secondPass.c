/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int readLinesSecondRun(FILE* file, int lineCount, symbolLine* symLine, dataLine* dLine)
{
    int x = 0, i = 0, j = 0;
    int labelFlag, contFlag;
    int lblAddr = 0;
    int pass = TRUE;
    char s;
    char are[] = "000";
    char temp[MAX_INPUT], word[MAX_INPUT];
    char code[16];
    symbolLine* symNode;
    dataLine* dNode;

    /*Set file pointer to start of file*/
    fseek(file, 0, SEEK_SET);

    /*Read a line from the file*/
    while(i<lineCount)
    {
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
                    handle only .entry in the second run.*/

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
                            /*For each one, check if in symbol table*/
                            labelFlag = FALSE;
                            symNode = symLine;
                            while(symNode)
                            {
                                if(strcmp(symNode->label, temp)==0)
                                {
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
                        /*If yes - add to each label the "entry" parameter as the type*/
                        continue;
                    }
                }
                
                else
                {
                    /*This is an operator.*/
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
                        
                        if(operandType(temp)==LABEL)
                        {
                            labelFlag = FALSE;
                            symNode = symLine;
                            while(symNode)
                            {
                                if(strcmp(symNode->label, temp)==0)
                                {
                                    labelFlag = TRUE;
                                    if(strcmp(symNode->type, "external"))
                                    {
                                        strcpy(are, "001");
                                        lblAddr = 0;
                                    }
                                    else if(strcmp(symNode->type, "data") ||
                                            strcmp(symNode->type, "data-entry") ||
                                            strcmp(symNode->type, "code") ||
                                            strcmp(symNode->type, "code-entry"))
                                    {
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

                            if(isExistingLabel(temp, symLine)) strcpy(are, "010");
                            else strcpy(are, "001");

                            /*Generating the new code for the updated label with the address*/
                            generateOperandCode(temp, OPERAND1, are, code, lblAddr);
                            
                            dNode = dLine;
                            while(dNode)
                            {
                                if(strcmp(dNode->code, NO_VALUE)==0)
                                {
                                    strcpy(dNode->code, code);
                                }
                                dNode = dNode->next;
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