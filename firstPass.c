/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int readLinesFirstRun(FILE* file, int lineCount, symbolLine** symLine, outputLine** outLine, dataLine** dLine, int* outerIC, int* outerDC)
{
    int IC = 100, DC = 0, x = 0, i = 0, j = 0;
    int labelFlag, contFlag, operandPlace, operatorGroup;
    int lblAddr = 0;
    int pass = TRUE, parsed, place;
    int firstQuote, lastQuote;
    char instructionCodes[3][MAX_INPUT];
    char s;
    char are[] = "000";
    char temp[MAX_INPUT], word[MAX_INPUT], currLabel[MAX_INPUT];
    char op1[MAX_INPUT], op2[MAX_INPUT], opTrap[MAX_INPUT];
    char code[16];
    symbolLine* symNode;
    outputLine* outNode;
    dataLine* dNode;

    /*Set file pointer to start of file*/
    fseek(file, 0, SEEK_SET);

    /*Read a line from the file*/
    while(i<lineCount)
    {
        labelFlag = FALSE;
        contFlag = FALSE;
        parsed = FALSE;

        s = fgetc(file);

        /*Skip all whitespaces in the beginning of the line and check if line is empty*/
        if(s=='\n' || s==EOF)
        {
            printf("Line %d: Empty line\n", i+1);
            i++;
            continue;
        }
        while(isSpace(s))
        {
            s = fgetc(file);
            if (s=='\n' || s==EOF){
                contFlag = TRUE;
                printf("Line %d: Empty line\n", i+1);
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
            printf("Line %d: Comment line\n", i+1);
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
                    /*We found a label.*/
                    /*Check if it's a legal label*/
                    word[j-1] = '\0';
                    if(isLegalLabel(word)==FALSE)
                    {
                        pass = FALSE;
                        printf("Line %d: Illegal label format for label \"%s\"\n", i+1, word);
                    }
                    /*Check if it exists already*/
                    if(isExistingLabel(word, *symLine)==TRUE)
                    {
                        pass = FALSE;
                        printf("Line %d: Duplicate label \"%s\"\n", i+1, word);
                    }
                    /*We store it and continue reading.*/
                    labelFlag = TRUE;
                    word[j-1] = '\0';
                    strcpy(currLabel, word);
                    continue;
                }

                if (word[0]=='.')
                {
                    /*The current word is a .something instruction.
                    We determine the type of the instruction and put it into the correct place.*/
                    
                    /*Check if data*/
                    if (strcmp(".data", word)==0)
                    {
                        /*Data instruction found.*/
                        /*Add label, if exists, to symtable with current DC value.*/
                        if(labelFlag)
                        {
                            if(*symLine==NULL)
                            {
                                *symLine = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = *symLine;
                            }
                            else
                            {
                                symNode->next = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = symNode->next;
                            }
                            strcpy(symNode->label, currLabel);
                            strcpy(symNode->type, "data");
                            symNode->address = DC;
                            symNode->next = NULL;
                        }
                        while(s!='\n' && s!=EOF)
                        {
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && x<MAX_INPUT)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            if(!isNumber(temp, 0))
                            {
                                pass = FALSE;
                                printf("Line %d: Invalid data item \"%s\"\n", i+1, temp);
                                continue;
                            }

                            /*Append the number to the dataframe*/
                            if(*dLine==NULL)
                            {
                                *dLine = (dataLine*)malloc(sizeof(dataLine));
                                dNode = *dLine;
                            }
                            else
                            {
                                dNode->next = (dataLine*)malloc(sizeof(dataLine));
                                dNode = dNode->next;
                            }
                            dataEncoder(temp, code);
                            dNode->address = DC++;
                            strcpy(dNode->code, code);
                            dNode->next = NULL;
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*Check if string*/
                    if (strcmp(".string", word)==0)
                    {
                        /*String instruction found.*/
                        /*Add label, if exists, to symtable with current DC value.*/
                        if(labelFlag)
                        {
                            if(*symLine==NULL)
                            {
                                *symLine = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = *symLine;
                            }
                            else
                            {
                                symNode->next = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = symNode->next;
                            }
                            strcpy(symNode->label, currLabel);
                            strcpy(symNode->type, "data");
                            symNode->address = DC;
                            symNode->next = NULL;
                        }
                        while(s!='\n' && s!=EOF){
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s))
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!='\n' && s!=EOF && x<MAX_STRING)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            if(!isValidString(temp))
                            {
                                pass = FALSE;
                                printf("Line %d: Invalid string definition or extra data after string in \"%s\"\n", i+1, temp);
                                break;
                            }
                            /*Finding the first and last quote sign location*/
                            x = 0;
                            while(temp[x]!='\0')
                            {
                                if(temp[x]=='\"')
                                {
                                    firstQuote = x;
                                    break;
                                }
                                x++;
	                        }
                            while(temp[x]!='\0')
                            {
                                if(temp[x]=='\"') lastQuote = x;
                                x++;
	                        }

                            /*Append each character to the dataframe*/
                            for(x = firstQuote+1; x<lastQuote; x++)
                            {
                                if(*dLine==NULL)
                                {
                                    *dLine = (dataLine*)malloc(sizeof(dataLine));
                                    dNode = *dLine;
                                }
                                else
                                {
                                    dNode->next = (dataLine*)malloc(sizeof(dataLine));
                                    dNode = dNode->next;
                                }
                                charEncoder(temp[x], code);
                                dNode->address = DC++;
                                strcpy(dNode->code, code);
                                dNode->next = NULL;
                            }
                            /*Add \0 to end*/
                            if(*dLine==NULL)
                            {
                                *dLine = (dataLine*)malloc(sizeof(dataLine));
                                dNode = *dLine;
                            }
                            else
                            {
                                dNode->next = (dataLine*)malloc(sizeof(dataLine));
                                dNode = dNode->next;
                            }
                            charEncoder('\0', code);
                            dNode->address = DC++;
                            strcpy(dNode->code, code);
                            dNode->next = NULL;
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*Check if entry*/
                    if (strcmp(".entry", word)==0)
                    {
                        /*Not handled in the first run*/
                        while (s!='\n' && s!=EOF)
                        {
                            s = fgetc(file);
                        }
                        break;
                    }

                    /*Check if extern*/
                    else if (strcmp(".extern", word)==0)
                    {
                        /*Extern instriction found.*/
                        /*Put each label in the symtable with the external tag.*/
                        while(s!='\n' && s!=EOF){
                            /*Ignore all whitespaces and commas*/
                            while(isSpace(s) || s==',')
                            {
                                s = fgetc(file);
                            }
                            /*Get the next data item*/
                            x = 0;
                            while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && x<MAX_INPUT)
                            {
                                temp[x++] = s;
                                s = fgetc(file);
                            }
                            temp[x] = '\0';
                            
                            /*Add label to the symbol table*/
                            if(*symLine==NULL)
                            {
                                *symLine = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = *symLine;
                            }
                            else
                            {
                                symNode->next = (symbolLine*)malloc(sizeof(symbolLine));
                                symNode = symNode->next;
                            }
                            strcpy(symNode->label, temp);
                            strcpy(symNode->type, "external");
                            symNode->next = NULL;
                        }
                        /*Read next line.*/
                        continue;
                    }

                    /*If neither, skip the line and print error*/
                    else
                    {
                        pass = FALSE;
                        printf("Line %d: Bad definition of instruction \"%s\"\n", i+1, word);
                        while (s!='\n' && s!=EOF) s = fgetc(file);
                    }
                }
                
                else
                {
                    /*This is an operator.*/
                    /*Add label, if exists, to symtable with current IC value*/
                    if(labelFlag)
                    {
                        if(*symLine==NULL)
                        {
                            *symLine = (symbolLine*)malloc(sizeof(symbolLine));
                            symNode = *symLine;
                        }
                        else
                        {
                            symNode->next = (symbolLine*)malloc(sizeof(symbolLine));
                            symNode = symNode->next;
                        }
                        strcpy(symNode->label, currLabel);
                        strcpy(symNode->type, "code");
                        symNode->address = IC;
                        symNode->next = NULL;
                    }
                    if(isValidOperator(word)==FALSE)
                    {
                        pass = FALSE;
                        printf("Line %d: Incorrect operator \"%s\" was used, line skipped\n", i+1, word);
                        while(s!='\n') s = fgetc(file);
                        i++;
                        break;
                    }
                    operatorGroup = operatorType(word);
                    operandPlace = OPERAND1;
                    /*Set no-values to both operands before parsing them*/
                    strcpy(op1, NO_VALUE);
                    strcpy(op2, NO_VALUE);
                    /*We use opTrap to find any excessive operands in the instruction (3 or more)*/
                    strcpy(opTrap, NO_VALUE);
                    while(s!='\n' && s!=EOF){
                        /*Ignore all whitespaces and commas*/
                        while(isSpace(s) || s==',')
                        {
                            s = fgetc(file);
                        }
                        /*Get the next operand*/
                        x = 0;
                        while(s!=' ' && s!=',' && s!='\t' && s!='\n' && s!=EOF && x<MAX_INPUT)
                        {
                            temp[x++] = s;
                            s = fgetc(file);
                        }
                        temp[x] = '\0';
                        /*Check validity of the operand and determine its type*/
                        if(!isValidOperand(temp))
                        {
                            pass = FALSE;
                            printf("Line %d: Incorrect operand \"%s\" was used\n", i+1, temp);
                            while (s!='\n' && s!=EOF) s = fgetc(file);
                            contFlag = TRUE;
                            break;
                        }
                        if(operandPlace==OPERAND1)
                        {
                            if(operatorGroup==GROUP_3)
                            {
                                /*Operators of group 3 have no allowed operands*/
                                pass = FALSE;
                                printf("Line %d: Too many operands provided for operator \"%s\"\n", i+1, word);
                                while (s!='\n' && s!=EOF) s = fgetc(file);
                                contFlag = TRUE;
                                break;
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
                                printf("Line %d: Too many operands provided for operator \"%s\"\n", i+1, word);
                                while (s!='\n' && s!=EOF) s = fgetc(file);
                                contFlag = TRUE;
                                break;
                            }
                            strcpy(op2, temp);
                            operandPlace = OPERAND_3_AND_UP;
                        }
                        else if(operandPlace==OPERAND_3_AND_UP)
                        {
                            /*No operator can have more than 2 operands*/
                            pass = FALSE;
                            printf("Line %d: Too many operands provided for operator \"%s\"\n", i+1, word);
                            while (s!='\n' && s!=EOF) s = fgetc(file);
                            contFlag = TRUE;
                            break;
                        }
                    }
                    if(contFlag) break;

                    /*In a single-operand instruction we put the operand as the destination operand*/
                    if(operatorGroup==GROUP_2 && strcmp(op2, NO_VALUE)==0)
                    {
                        strcpy(op2, op1);
                        strcpy(op1, NO_VALUE);
                    }

                    /*Validate correct use of operand addressing in the instruction*/
                    if(!isValidOperandUse(word, op1, op2))
                    {
                        pass = FALSE;
                        printf("Line %d: Incorrect addressing method was used for operator \"%s\"\n", i+1, word);
                        while (s!='\n' && s!=EOF) s = fgetc(file);
                        break;
                    }

                    /*Generate the operator instruction code*/
                    generateInstructionCode(word, op1, op2, code);
                    strcpy(instructionCodes[0], code);

                    /*For each operand determine its type and what addressing should be used*/
                    if(operandType(op1)==NUMBER)
                    {
                        strcpy(are, "100");
                        generateOperandCode(op1, OPERAND1, are, code, lblAddr);
                        strcpy(instructionCodes[1], code);
                    }
                    else if(operandType(op1)==LABEL)
                    {
                        strcpy(instructionCodes[1], NO_VALUE);
                    }
                    else if(operandType(op1)==DIR_REGISTER || operandType(op1)==INDIR_REGISTER)
                    {
                        if(operandType(op2)==DIR_REGISTER || operandType(op2)==INDIR_REGISTER)
                        {
                            generateOpcodeDualRegs(op1, op2, code);
                            strcpy(instructionCodes[1], code);
                            parsed = TRUE;
                        }
                        else
                        {
                            strcpy(are, "100");

                            generateOperandCode(op1, OPERAND1, are, code, lblAddr);
                            strcpy(instructionCodes[1], code);
                        }
                    }

                    if(operandType(op2)==NUMBER)
                    {
                        strcpy(are, "100");
                        generateOperandCode(op2, OPERAND2, are, code, lblAddr);
                        strcpy(instructionCodes[2], code);
                    }
                    else if(operandType(op2)==LABEL)
                    {
                        strcpy(instructionCodes[2], NO_VALUE);
                    }
                    else if(operandType(op2)==DIR_REGISTER || operandType(op2)==INDIR_REGISTER)
                    {
                        if(parsed==FALSE)
                        {
                            strcpy(are, "100");

                            generateOperandCode(op2, OPERAND2, are, code, lblAddr);
                            strcpy(instructionCodes[2], code);
                        }
                    }
                    
                    /*Populate the code table with the instruction's binary values*/
                    for(place = 0; place<3; place++)
                    {
                        if(place==2 && parsed==TRUE) break;
                        if(operatorType(word)==GROUP_2 && place==1) continue;
                        if(operatorType(word)==GROUP_3 && place>0) continue;
                        if(*outLine==NULL)
                        {
                            *outLine = (outputLine*)malloc(sizeof(outputLine));
                            outNode = *outLine;
                        }
                        else
                        {
                            outNode->next = (outputLine*)malloc(sizeof(outputLine));
                            outNode = outNode->next;
                        }
                        outNode->address = IC++;
                        strcpy(outNode->code, instructionCodes[place]);
                        outNode->next = NULL;
                    }
                }
            }
        }
        i++;
    }
    
    /*Update all DC values to correct IC+DC values*/
    symNode = *symLine;
    while(symNode)
    {
        if(strcmp(symNode->type, "data")==0)
        {
            symNode->address = symNode->address + IC;
        }
        symNode = symNode->next;
    }
    dNode = *dLine;
    while(dNode)
    {
        dNode->address = dNode->address + IC;
        dNode = dNode->next;
    }
    *outerIC = IC-100;
    *outerDC = DC;

    /*Since the memory is only 4096 bytes long, any code longer than that will be unusable*/
    if(*outerIC + *outerDC > 4095)
    {
        printf("Error: Memory overflow, last address above 4095.\n");
        pass = FALSE;
    }
    return pass;
}