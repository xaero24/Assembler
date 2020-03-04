
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

char* stripChars(char* string, char remove){
    /*
    This function removes from the string the requsted character and replaces it with a single space.
    If the characters are in the beginning of the string, they are removed completely.
    This works best for whitespace removal.
    */

    int i = 0, len = 0;
    int started = 0;
    char temp[100];
    char* newString = NULL;
    while(string[i]!='\0'){
        if(string[i] != remove){
            temp[len++] = string[i++];
            started = 1;
        }
        else{
            if(string[i+1] == remove || string[i+1] == '\0') i++;
            else{
                if(started){
                    temp[len++] = ' ';
                }
                i++;
            }
        }
    }
    temp[len] = '\0';
    newString = (char*)malloc(len);
    strcpy(newString, temp);
    return newString;
}

char** tokenizer(char* string, char delimiter){
    /*
    This function returns an array of tokens according to a given token.
    If the token is a space, the string will cut the string into separate words.
    Best to be used after whitespace stripping.
    */
    int start = 0, end = 0, i = 0;
    int wordCount = 1;
    char** parts = NULL;
    for(end=0 ;string[end] != '\0'; end++){
        if(string[end] == delimiter) wordCount++;
    }
    parts = (char**)malloc(wordCount);
    for(end=0 ;string[end] != '\0'; end++){
        if(string[end] == delimiter){
            parts[i] = (char*)malloc(end-start);
            strncpy(parts[i], string+start, end-start);
            strcat(parts[i], '\0');
            i++;
            start = end+1;
        }
    }
    parts[i] = (char*)malloc(end-start);
    strncpy(parts[i], string+start, end-start);
    strcat(parts[i], '\0');
    return parts;
}

int parseFile(FILE* file, codeNode* codeHead, symNode* symHead){
    /**/
    int lineNumber = 100, charIndex, lastIndex, linePart;
    char whiteSpace;
    char data[256], word[20], temp;
    codeNode* c_node = NULL;
    codeLine* c_line = NULL;
    symNode* s_node = NULL;
    symTableLine* s_line = NULL;

    printf("Reading file gives:\n\n");
    while(fgets(data, 256, file)){
        /*Read a file line and parse it*/
        charIndex = 0;
        lastIndex = 0;
        linePart = 1;

        /*Empty lines are completely ignored*/
        if(data[0] == '\n' || strlen(data) == 0) continue;

        /*Create a codeline node that holds a parsed line of code*/ 
        c_node = (codeNode*) malloc(siseof(c_node));
        if(c_node){
            /*Create a codeline structure for storing parsed parts of the line*/
            c_line = (codeLine*)malloc(sizeof(c_line));
            if(c_line){
            }
            c_node->line = c_line;
            c_node->next = NULL;
            lineNumber++;
        }
        
    }
    return 0;
}