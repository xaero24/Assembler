/**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int isDigit(char x)
{
	return x >= '0' && x <='9';
}

int isSpace(char x)
{
	return x ==  ' ' || x == '\t';
}

int isLetter(char x)
{
	return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z');
}

int isValidData(char* data)
{
    /*Checks the format of a data item - must be a decimal number with an optional leading sign*/
}

int isValidString(char* str, int size)
{
    /*Checks if the string is a correctly formed single string*/
	int i = 1;
	if(str[0]=='\"' && str[size-2]=='\"')
	{
		while(i<size-2)
		{
			if(str[i]=='\"') return FALSE;
		}
	}
	return FALSE;
}

int isLegalLabel(char* label)
{
	int i = 1;
	if(!isLetter(label[0]))
	{
		return FALSE;
	}
	while(label[i]!='\0')
	{
		if(!isLetter(label[i]) && !isDigit(label[i]))
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

int isExistingLabel(char* label, symbolLine* head)
{

}

int isValidOperator(char* operator)
{

}

int isValidAddressing(char* operand)
{

}

int isValidOperand(char* operand)
{
	
}

int isValidOperandUse(char* operator, char* op1, char* op2)
{

}

int operatorType(char* operator)
{
	if(	strcmp(operator, "mov") || 
		strcmp(operator, "cmp") || 
		strcmp(operator, "add") || 
		strcmp(operator, "sub") || 
		strcmp(operator, "lea"))
	{
		return GROUP_1;
	}
	else if(	strcmp(operator, "clr") || 
				strcmp(operator, "not") || 
				strcmp(operator, "inc") || 
				strcmp(operator, "dec") || 
				strcmp(operator, "jmp") || 
				strcmp(operator, "bne") || 
				strcmp(operator, "red") || 
				strcmp(operator, "prn") || 
				strcmp(operator, "jsr"))
	{
		return GROUP_2;
	}
	else if(	strcmp(operator, "rts") || 
				strcmp(operator, "stop"))
	{
		return GROUP_3;
	}
	
}

int operandType(char* operand)
{

}

int checkLabelExistence(char* label, symbolLine* head)
{

}

int addressingType(char* op1, char* op2)
{
	int typeOfOperand1;
	int typeOfOperand2;
	int operandTypes;

	typeOfOperand1 = operandType(op1);
	typeOfOperand2 = operandType(op2);
	operandTypes = 10*typeOfOperand1 + typeOfOperand2;

	return operandTypes;
}