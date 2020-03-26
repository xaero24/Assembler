/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

/*
TODO: Add correct handling of empty head elements of linked lists in EACH section where it may be relevant.
*/

int readLinesFirstRun(FILE* file, int lineCount, symbolLine* symLine, outputLine* outLine, dataLine* dLine)
{
    int IC = 100, DC = 0, x = 0, i = 0, j = 0;
    int labelFlag, contFlag, operandPlace, operatorGroup;
    int lineCount;
    int pass = TRUE;
    char s;
    char temp[MAX_INPUT], word[MAX_INPUT], currLabel[MAX_INPUT];
    char op1[MAX_INPUT], op2[MAX_INPUT], opTrap[MAX_INPUT];
    symbolLine* symNode = symLine;
    outputLine* outNode = outLine;
    dataLine* dNode = outLine;

    /*Set file pointer to start of file*/
    fseek(file, SEEK_SET, 0);

    /*Read a line from the file*/
    while(i<lineCount)
    {
        labelFlag = FALSE;
        contFlag = FALSE;
        s = fgetc(file);

        /*Skip all whitespaces in the beginning of the line and check if line is empty*/
        while(isSpace(s))
        {
            s = fgetc(file);
            if (s=='\n'){
                contFlag = TRUE;
                printf("Line %d: Empty line\n", i);
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
            while (s!='\n')
            {
                s = fgetc(file);
            }
            i++;
            printf("Line %d: Comment line\n", i);
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
                    /*We found a label.*/
                    /*Check if it's a legal label*/
                    if(!isLegalLabel(word))
                    {
                        pass = FALSE;
                        printf("Line %d: Illegal label\n", i);
                    }
                    /*Check if it exists already*/
                    if(!isExistingLabel(word, symLine))
                    {
                        pass = FALSE;
                        printf("Line %d: Duplicate label\n", i);
                    }
                    /*We store it and continue reading.*/
                    labelFlag = TRUE;
                    word[j-1] = '\0';
                    strcpy(currLabel, word);
                    continue;
                }

                if (word[0]=='.')
                {
                    /*The current word is an instruction.
                    We determine the type of the instruction and put it into the correct place.*/
                    
                    /*Check if data*/
                    if (strcmp(".data", temp)==0)
                    {
                        /*Data instruction found.*/
                        /*Add label, if exists, to symtable with current DC value.*/
                        if(labelFlag)
                        {
                            symNode = (symbolLine*)malloc(sizeof(symbolLine));
                            strcpy(symNode->label, currLabel);
                            strcpy(symNode->type, "data");
                            symNode->address = DC;
                            symNode->next = NULL;
                            symNode = symNode->next;
                        }
                        while(s!='\n')
                        {
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && j<MAX_INPUT)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            if(!isValidData(temp))
                            {
                                pass = FALSE;
                                printf("Line %d: Invalid data item\n", i);
                            }
                            dNode = (dataLine*)malloc(sizeof(dataLine));
                            dNode->address = DC++;
                            dNode->code = dataEncoder(temp);
                            dNode->next = NULL;
                            dNode = dNode->next;
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*Check if string*/
                    if (strcmp(".string", temp)==0)
                    {
                        /*String instruction found.*/
                        /*Add label, if exists, to symtable with current DC value.*/
                        if(labelFlag)
                        {
                            symNode = (symbolLine*)malloc(sizeof(symbolLine));
                            strcpy(symNode->label, currLabel);
                            strcpy(symNode->type, "data");
                            symNode->address = DC;
                            symNode->next = NULL;
                            symNode = symNode->next;
                        }
                        while(s!='\n'){
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!='\n' && s!=EOF && j<MAX_STRING)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            if(!isValidString(temp))
                            {
                                pass = FALSE;
                                printf("Line %d: Invalid or multiple string definition\n", i);
                            }
                            x = 1;
                            while(temp[x]!='\"')
                            {
                                dNode = (dataLine*)malloc(sizeof(dataLine));
                                dNode->address = DC++;
                                dNode->code = charEncoder(temp[x]);
                                dNode->next = NULL;
                                dNode = dNode->next;
                            }
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*Check if entry*/
                    if (strcmp(".entry", temp)==0)
                    {
                        /*Not handled in the first run*/
                        while (s!='\n')
                        {
                            s = fgetc(file);
                        }
                        i++;
                        continue;
                    }

                    /*Check if extern*/
                    else if (strcmp(".extern", temp)==0)
                    {
                        /*Extern instriction found.*/
                        /*Put each label in the symtable with the external tag.*/
                        while(s!='\n'){
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && j<MAX_INPUT)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';

                            symNode = (symbolLine*)malloc(sizeof(symbolLine));
                            strcpy(symNode->label, temp);
                            strcpy(symNode->type, "external");
                            symNode->next = NULL;
                            symNode = symNode->next;
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*If neither, skip the row and print error*/
                    else
                    {
                        pass = FALSE;
                        printf("Line %d: Bad definition of instruction\n", i);
                        while (s!='\n' && s!=EOF)
                            s = fgetc(file);
                    }
                }
                
                else
                {
                    /*This is an operator.*/
                    /*Add label, if exists, to symtable with current IC value*/
                    if(labelFlag)
                    {
                        symNode = (symbolLine*)malloc(sizeof(symbolLine));
                        strcpy(symNode->label, currLabel);
                        strcpy(symNode->type, "data");
                        symNode->address = IC;
                        symNode->next = NULL;
                        symNode = symNode->next;
                    }
                    /**/
                    operatorGroup = operatorType(word);
                    operandPlace = OPERAND1;
                    /*Set no-values to both operands before parsing them*/
                    strcpy(op1, NO_VALUE);
                    strcpy(op2, NO_VALUE);
                    /*We use opTrap to find any excessive operands in the instruction (3 or more)*/
                    strcpy(opTrap, NO_VALUE);
                    while(s!='\n'){
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
                        /*Check validity of the operand and determine its type*/
                        if(!isValidOperand(temp))
                        {
                            pass = FALSE;
                            printf("Line %d: Incorrect operand was used\n", i);
                        }
                        if(operandPlace==OPERAND1)
                        {
                            if(operatorGroup==GROUP_3)
                            {
                                /*Operators of group 3 have no allowed operands*/
                                pass = FALSE;
                                printf("Line %d: Too many operands provided\n", i);
                            }
                            strcpy(op1, temp);
                            operandPlace = OPERAND2;
                        }
                        else if(operandPlace==OPERAND2)
                        {
                            if( operatorGroup==GROUP_2 ||
                                operatorGroup==GROUP_3)
                            {
                                /*Operators of group 3 have no allowed operands, and operators of group 2 can have only one operand*/
                                pass = FALSE;
                                printf("Line %d: Too many operands provided\n", i);
                            }
                            strcpy(op2, temp);
                            operandPlace = OPERAND_3_AND_UP;
                        }
                        else if(operandPlace==OPERAND_3_AND_UP)
                        {
                            /*No operator can have more than 2 operands*/
                            pass = FALSE;
                            printf("Line %d: Too many operands provided\n", i);
                        }
                    }
                    if(isValidOperandUse(word, op1, op2))
                    {
                        pass = FALSE;
                        printf("Line %d: Incorrect addressing method was used\n", i);
                    }
                    
                }
            }
        }
        i++;
    }

    return pass;
}
