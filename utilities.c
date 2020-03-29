/*
Authors:
Karin Yurchenko ID: 204334601
David Eliyahu Mashiah ID:318183803

This file contains utility parsing functions
*/
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
    fseek(file, 0, SEEK_SET);
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

int stringToUnsignedInt(char* string, char type)
{
    /*Gets a string representation of a number and converts is to an unsigned integer.*/
    int val;
    int i = 0, res = 0;
    int neg = FALSE;
    if(string[i]=='#') i++;
    if(string[i]=='-')
    {
        neg = TRUE;
        i++;
    }
    if(string[i]=='+') i++;
    while(string[i]!='\0')
    {
        res *= 10;
        res += (int)string[i++] - 48;
    }
    if(neg) res *= -1;
    if(type=='d') val = 32768;
    else if(type=='o') val = 4096;

    return decToUnsignedDec(res, val);
}

void decToBin(int decimal, char* res)
{
    char result[16];
    char flipped[16];
    int i = 0, size;
    /*Produces flipped number*/
    while(decimal)
    {
        if(decimal%2) flipped[i++] = '1';
        else flipped[i++] = '0';
        decimal/=2;
    }
    flipped[i] = '\0';
    /*Flips the number to be in the correct order of bits*/
    size = strlen(flipped) - 1;
    i = 0;
    while(size>=0)
    {
        result[i++] = flipped[size--];
    }
    result[i] = '\0';
    strcpy(res, result);
}

void binToOct(char* binary, char* result)
{
    /*Converts a binary number to octal*/
    char data[4];
    int i = 0, x = 0;
    strcpy(result, "");
    while(binary[i]!='\0')
    {
        data[x++] = binary[i++];
        if(x==3)
        {
            data[x] = '\0';
            x = 0;
            getDigitFromBinary(data, result);
        }
    }
}

void getDigitFromBinary(char* binary, char* target)
{
    /*Used for conversion from binary to decimal string of a register number*/
    if(strcmp(binary, "000")==0) strcat(target, "0");
    if(strcmp(binary, "001")==0) strcat(target, "1");
    if(strcmp(binary, "010")==0) strcat(target, "2");
    if(strcmp(binary, "011")==0) strcat(target, "3");
    if(strcmp(binary, "100")==0) strcat(target, "4");
    if(strcmp(binary, "101")==0) strcat(target, "5");
    if(strcmp(binary, "110")==0) strcat(target, "6");
    if(strcmp(binary, "111")==0) strcat(target, "7");
}

int decToUnsignedDec(int number, int limit)
{
    /*Returns the unsigned decimal value of a given number*/
    if(number<0) return limit + number;
    else return number;
}

void dataEncoder(char* data, char* result)
{
    /*Returns the binary representation of a number*/
    int number = stringToUnsignedInt(data, 'd'), length, i;
    char temp[16];
    decToBin(number, temp);
    strcpy(result, "");
    length = strlen(temp);
    for(i=0; i<(15-length); i++)
    {
        strcat(result, "0");
    }
    strcat(result, temp);
}

void charEncoder(char c, char* result)
{
    /*Returns the binary representation of a character ASCII code*/
    int ascii_char = (int)c, length, i;
    char temp[16];
    decToBin(ascii_char, temp);
    strcpy(result, "");
    length = strlen(temp);
    for(i=0; i<(15-length); i++)
    {
        strcat(result, "0");
    }
    strcat(result, temp);
}

void generateInstructionCode(char* operator, char* src, char* dest, char* result)
{
    /*Generates binary code for the given instruction*/
    char are[] = "100", binOpCode[5];
    int srcType, destType;
    srcType = operandType(src);
    destType = operandType(dest);
    getOperatorBinary(operator, binOpCode);
    /*Get the binary code of the operand*/
    strcpy(result, binOpCode);

    /*Add the addressing type of the source operand*/
    if(strcmp(src, NO_VALUE))
    {
        switch(srcType)
        {
            case NUMBER:
                strcat(result, "0001");
                break;
            case LABEL:
                strcat(result, "0010");
                break;
            case INDIR_REGISTER:
                strcat(result, "0100");
                break;
            case DIR_REGISTER:
                strcat(result, "1000");
                break;
        }
    }
    else
    {
        strcat(result, "0000");
    }

    /*Add the addressing type of the destination operand*/
    if(strcmp(dest, NO_VALUE))
    {
        switch(destType)
        {
            case NUMBER:
                strcat(result, "0001");
                break;
            case LABEL:
                strcat(result, "0010");
                break;
            case INDIR_REGISTER:
                strcat(result, "0100");
                break;
            case DIR_REGISTER:
                strcat(result, "1000");
                break;
        }
    }
    else
    {
        strcat(result, "0000");
    }
    /*Add the A-R-E field*/
    strcat(result, are);
}

void generateOperandCode(char* operand, int operandPlace, char* are, char* result, int lblAddr)
{
    /*Generates binary code for the given operand by type*/
    char binOper[13];
    char header[] = "000000";
    int length, i;

    if(strcmp(operand, NO_VALUE)==0)
    {
        strcpy(result, "000000000000000");
    }
    else
    {
        strcpy(result, "");
        switch(operandType(operand))
        {
            case NUMBER:
                /*Get the binary value of the operand*/
                decToBin(stringToUnsignedInt(operand, 'o'), binOper);
                length = strlen(binOper);
                for(i = 0; i<(12-length); i++)
                {
                    strcat(result, "0");
                }
                strcat(result, binOper);
                break;
            case LABEL:
                /*Get the binary code of the label's address*/
                decToBin(lblAddr, binOper);
                length = strlen(binOper);
                for(i = 0; i<(12-length); i++)
                {
                    strcat(result, "0");
                }
                strcat(result, binOper);
                break;
            case DIR_REGISTER:
                /*Create the binary code for the register based on the register number*/
                getRegNumBinary(operand[1], binOper);
                strcat(result, header);
                if(operandPlace==OPERAND1)
                {
                    strcat(result, binOper);
                    strcat(result, "000");
                }
                else if(operandPlace==OPERAND2)
                {
                    strcat(result, "000");
                    strcat(result, binOper);
                }
                break;
            case INDIR_REGISTER:
                /*Create the binary code for the register based on the register number*/
                getRegNumBinary(operand[2], binOper);
                strcat(result, header);
                if(operandPlace==OPERAND1)
                {
                    strcat(result, binOper);
                    strcat(result, "000");
                }
                else if(operandPlace==OPERAND2)
                {
                    strcat(result, "000");
                    strcat(result, binOper);
                }
                break;
        }
        strcat(result, are);
    }
}

void generateOpcodeDualRegs(char* reg1, char* reg2, char* result)
{
    /*For instructions with two registers, this function produces a single binary word.*/
    int r1_type, r2_type;
    char r1_code[4], r2_code[4];
    char header[] = "000000";

    r1_type = operandType(reg1);
    r2_type = operandType(reg2);

    /*Determine the register number*/
    if(r1_type==DIR_REGISTER)
    {
        getRegNumBinary(reg1[1], r1_code);
    }
    else if(r1_type==INDIR_REGISTER)
    {
        getRegNumBinary(reg1[2], r1_code);
    }

    if(r2_type==DIR_REGISTER)
    {
        getRegNumBinary(reg2[1], r2_code);
    }
    else if(r2_type==INDIR_REGISTER)
    {
        getRegNumBinary(reg2[2], r2_code);
    }

    /*Add 6 trailing zeros, the registers and the ARE field*/
    strcpy(result, header);
    strcat(result, r1_code);
    strcat(result, r2_code);
    strcat(result, "100");
}

int operatorType(char* operator)
{
    /*Used for divisoin of operators into 3 groups. Returns 9 as a non-valid input indicator.*/
	if(	strcmp(operator, "mov")==0 ||
		strcmp(operator, "cmp")==0 ||
		strcmp(operator, "add")==0 ||
		strcmp(operator, "sub")==0 ||
		strcmp(operator, "lea")==0)
	{
        /*2-operand operator*/
		return GROUP_1;
	}
	else if(	strcmp(operator, "clr")==0 ||
				strcmp(operator, "not")==0 ||
				strcmp(operator, "inc")==0 ||
				strcmp(operator, "dec")==0 ||
				strcmp(operator, "jmp")==0 ||
				strcmp(operator, "bne")==0 ||
				strcmp(operator, "red")==0 ||
				strcmp(operator, "prn")==0 ||
				strcmp(operator, "jsr")==0)
	{
        /*1-operand operator*/
		return GROUP_2;
	}
	else if(	strcmp(operator, "rts")==0 ||
				strcmp(operator, "stop")==0)
	{
        /*No-operand operator*/
		return GROUP_3;
	}
	else return 9;
}

void getOperatorBinary(char* operator, char* result)
{
    /*This function gets the binary code for the operators in use*/
    if(strcmp(operator, "mov")==0) strcpy(result, "0000");
	else if(strcmp(operator, "cmp")==0) strcpy(result, "0001");
	else if(strcmp(operator, "add")==0) strcpy(result, "0010");
	else if(strcmp(operator, "sub")==0) strcpy(result, "0011");
	else if(strcmp(operator, "lea")==0) strcpy(result, "0100");
	else if(strcmp(operator, "clr")==0) strcpy(result, "0101");
	else if(strcmp(operator, "not")==0) strcpy(result, "0110");
	else if(strcmp(operator, "inc")==0) strcpy(result, "0111");
	else if(strcmp(operator, "dec")==0) strcpy(result, "1000");
	else if(strcmp(operator, "jmp")==0) strcpy(result, "1001");
	else if(strcmp(operator, "bne")==0) strcpy(result, "1010");
	else if(strcmp(operator, "red")==0) strcpy(result, "1011");
	else if(strcmp(operator, "prn")==0) strcpy(result, "1100");
	else if(strcmp(operator, "jsr")==0) strcpy(result, "1101");
	else if(strcmp(operator, "rts")==0) strcpy(result, "1110");
	else if(strcmp(operator, "stop")==0) strcpy(result, "1111");
}

int getOperatorNumber(char* operator)
{
    /*This function gets the decimal code for the operators in use.
    In case of a bad operator will return 99 as a non-existent command.*/
    if(strcmp(operator, "mov")==0) return MOV;
	else if(strcmp(operator, "cmp")==0) return CMP;
	else if(strcmp(operator, "add")==0) return ADD;
	else if(strcmp(operator, "sub")==0) return SUB;
	else if(strcmp(operator, "lea")==0) return LEA;
	else if(strcmp(operator, "clr")==0) return CLR;
	else if(strcmp(operator, "not")==0) return NOT;
	else if(strcmp(operator, "inc")==0) return INC;
	else if(strcmp(operator, "dec")==0) return DEC;
	else if(strcmp(operator, "jmp")==0) return JMP;
	else if(strcmp(operator, "bne")==0) return BNE;
	else if(strcmp(operator, "red")==0) return RED;
	else if(strcmp(operator, "prn")==0) return PRN;
	else if(strcmp(operator, "jsr")==0) return JSR;
	else if(strcmp(operator, "rts")==0) return RTS;
	else if(strcmp(operator, "stop")==0) return STOP;
	else return 99;
}

void getRegNumBinary(char regNum, char* result)
{
    /*Used for getting exactly 3 digits for any register from 0 to 7*/
    if(regNum=='0') strcpy(result, "000");
    if(regNum=='1') strcpy(result, "001");
    if(regNum=='2') strcpy(result, "010");
    if(regNum=='3') strcpy(result, "011");
    if(regNum=='4') strcpy(result, "100");
    if(regNum=='5') strcpy(result, "101");
    if(regNum=='6') strcpy(result, "110");
    if(regNum=='7') strcpy(result, "111");
}

int operandType(char* operand)
{
	/*Check if it's a register direct addressing*/
    if(strlen(operand)==2 && operand[0]=='r' && (operand[1]>='0' && operand[1]<='7'))
	{
		return DIR_REGISTER;
	}
	/*Check if it's a register indirect (asterisk) addressing*/
	else if(	strlen(operand)==3 &&
				operand[0]=='*' &&
				operand[1]=='r' &&
				(operand[2]>='0' && operand[2]<='7') )
	{
		return INDIR_REGISTER;
	}
	/*Check if it's a number*/
	else if(operand[0]=='#' && isNumber(operand, 1))
	{
		return NUMBER;
	}
	/*Check if it's a label*/
	else
	{
		return LABEL;
	}
}
