/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int readLines(FILE* file, char* parsed, codeLine* line, int passNum)
{
    int IC = 100, DC, x=0, i=0, j=0;
    int labelFlag = FALSE, state, contFlag;
    int lineCount, lineIndex = 0;
    int pass = TRUE;
    char entry[FILENAME_MAX], external[FILENAME_MAX];
    char s;
    char temp[] = "asmblr";
    char word[MAX_INPUT];
    codeLine *tempLines = NULL;
    FILE *ent, *ext;

    if(passNum == 1){
        DC = 0;
    }

    strcpy(entry,parsed);
    strcpy(external,parsed);
    strcat(entry,".en");
    strcat(external,".ex");

    lineCount = lineCounter(file);
    /*Set file pointer to start of file*/
    fseek(file,SEEK_SET,0);
    /*Allocate memory for the parsed lines according to the line count*/
    tempLines = calloc(line, sizeof(codeLine));
    if (!temp)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }

    /*Read a line from the file*/
    while(i<lineCount)
    {
        state = INIT;
        contFlag = FALSE;
        ent=NULL;
        ext=NULL;

        /*Skip all whitespaces in the beginning of the line and check if line is empty*/
        do
        {
            s=fgetc(file);
            if (s == '\n'){
                printf("LINE %d) Empty line, ignored.\n", i);
                contFlag = TRUE;
                break;
            }
        } while(isSpace(s));
        if(contFlag)
        {
            i++;
            continue;
        }

        /*A commented line is not parsed*/
        if (s==';')
        {
            while (s!='\n')
            {
                s=fgetc(file);
            }
            printf("LINE %d) Comment line, ignored.\n", i);
            i++;
            continue;
        }

        /*All other lines are parsed according to the algorithm*/
        else
        {
            printf("----Parsing rest of line %d----\n", i);
            while (s != '\n' && s != EOF)
            {
                j=0;

                /*Ignore all whitespaces and commas*/
                while(isSpace(s) || s==',')
                {
                    s=fgetc(file);
                }

                /*Read a single word and save it to the temporary variable*/
                while(s!=' ' && s!=',' && s != '\n' && s != EOF && j<MAX_INPUT){
                    word[j++] = s;
                    s=fgetc(file);
                }
                word[j] = '\0';
                printf("Checking [%s]\n", word);

                if(word[j-1] == ':'){
                    printf("Found label! state: %d\n", state);
                    state = LABEL;
                }

                if(state == LABEL)
                {
                    word[j-1] = '\0';
                    printf("==LBL==\n");

                    if (s == '.') /*TODO: Define the s variable better.*/
                    {
                        printf("---Parsing ent ext on %d----\n", line);
                        x=0;
                        /*Read the .something command, 6 chars max*/
                        while (x<6 && s!=' ' && s!='\t' && s!='\n' && s!=EOF)
                        {
                            s=fgetc(file);
                            temp[x] = s;
                            x++;
                        }
                        /*Checks if data*/
                        if (strcmp("data", temp)==0)
                        {
                            printf("DATA\n");
                            /*Data instruction found.
                            Add label to data symtable with current DC value.*/
                            /*Add found data to the overall data view and update DC.*/
                            /*Read next line.*/
                            continue;
                        }
                        /*Checks if string*/
                        if (strcmp("string", temp)==0)
                        {
                            printf("STRING\n");
                            /*String instruction found.
                            Add label to symtable with data tag current DC value*/
                            /*Add found string to the overall data view and update DC.*/
                            /*Read next line.*/
                            continue;
                        }
                        /*Checks if entry*/
                        if (strcmp("entry", temp)==0)
                        {
                            printf("ENTRY\n");
                            /*Process only if it's the second pass.*/
                            if(passNum == 2){

                            }
                        }
                        /*Checks if extern*/
                        else if (strcmp("extern", temp)==0)
                        {
                            printf("EXTERN\n");
                            /*Extern instriction found.
                            Put each label in the symtable with the external tag.*/
                        }
                        /*If neither, skip the row and print error*/
                        else
                        {
                            pass = FALSE;
                            printf("LINE %d) Bad definition of .extern or .entry command.\n", i);
                            while (s!='\n' && s!=EOF)
                                s=fgetc(file);
                        }
                    }

                    if(j==MAX_INPUT)
                    {
                        pass = FALSE;
                        printf("LINE %d) Label error: label is more than 30 characters.\n", i);
                    }
                    else
                    {
                        if (isLetter(word[0]) == 0)
                        {
                            pass = FALSE;
                            printf("LINE %d) Label error: The first character in the label is wrong.\n", i);
                        }
                        else{
                            for(j=1; j<strlen(word)-1; j++){
                                if (isLetter(word[j])==0 && isDigit(word[j])==0)
                                    {
                                        pass = FALSE;
                                        printf("LINE %d) Label error: wrong character \"%c\" found in label.\n", i, word[j]);
                                    }
                            }
                        }
                        
                    }
                    state = OPERATOR;
                    continue;
                }

                else if (state == OPERATOR)
                {
                    printf("==OPER==\n");
                    strcpy(tempLines[i].operator, word);
                    if(j>MAX_OPERATOR)
                    {
                        pass = FALSE;
                        printf("LINE %d) Operator error: operator is too long.\n", i);
                    }
                    state = OPERAND1;
                    continue;
                }
                
                else if (state == OPERAND1)
                {
                    printf("==OP1==\n");
                    strcpy(tempLines[i].operand1, word);
                    if(j>MAX_OPERATOR)
                    {
                        pass = FALSE;
                        printf("LINE %d) Operand error: first operand is too long.\n", i);
                    }
                    state = OPERAND2;
                    continue;
                }
                else if (state == OPERAND2)
                {
                    printf("==OP2==\n");
                    strcpy(tempLines[i].operand2, word);
                    if(j>MAX_OPERATOR)
                    {
                        pass = FALSE;
                        printf("LINE %d) Operand error: second operand is too long.\n", i);
                    }
                    state = INIT;
                    break;
                }
            }
        }
        i++;

        /*Entry and extern commands are not part of the final code*/
        if (s == '.')
        {
            printf("---Parsing ent ext on %d----\n", line);
            x=0;
            /*Read the .something command, 6 chars max*/
            while (x<6 && s!=' ' && s!='\t' && s!='\n' && s!=EOF)
            {
                s=fgetc(file);
                temp[x] = s;
                x++;
            }
            /*Checks if entry*/
            if (strcmp("entry ", temp)==0)
            {
                printf("ENT\n");
                ent = fopen(entry,"a");
                while (s!='\n' && s!=EOF)
                {
                    while(isSpace(s))
                    {
                        s=fgetc(file);
                    }
                    fputc(s,ent);
                    s=fgetc(file);
                }
                fputc(s,ent);
                fclose(ent);
            }
            /*Checks if extern*/
            else if (strcmp("extern", temp)==0)
            {
                printf("EXT\n");
                s=fgetc(file);
                ext = fopen(external,"a");
                while (s!='\n' && s!=EOF)
                {
                    while(isSpace(s))
                    {
                        s=fgetc(file);
                    }
                    fputc(s,ext);
                    s=fgetc(file);
                }
                fputc(s,ext);
                fclose(ext);
            }
            /*If neither, skip the row and print error*/
            else
            {
                pass = FALSE;
                printf("LINE %d) Bad definition of .extern or .entry command.\n", i);
                while (s!='\n' && s!=EOF)
                    s=fgetc(file);
            }
        }

        /*Otherwise - read the code*/
    }
    /*If it's a label - raise labelFlag*/
    /**/
    /**/
    /**/
    /**/
    /**/
    /**/
    /**/
    /**/
    return pass;
}

int lineCounter(FILE *file)
{
    int count = 0;
    char c;
    /*Going back to begining of the file*/
    fseek(file,SEEK_SET,0);
    /*Check if have 1 line at least */
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