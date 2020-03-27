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

int isNumber(char* word, int index)
{
	int i = index;
	if(word[i]=='+' || word[i]=='-') i++;
	while(word[i]!='\0')
	{
		if(!isDigit(word[i++])) return FALSE;
	}
	return TRUE;
}

int isValidString(char* str)
{
    /*Checks if the string is a correctly formed single string*/
	int i = 1;
	int firstQuote, lastQuote;
	/*Finding the last quote sign location*/
	while(str[i]!="\0")
	{
		if(str[i]=="\"") lastQuote = i++;
	}
	if(str[0]!="\"") return FALSE;

	for(i = lastQuote+1; str[i]!='\0'; i++)
	{
		if(str[i]!=' ' && str[i]!='\t') return FALSE;
	}
	return TRUE;
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
	symbolLine *node = head;
	int exists = FALSE;
	while(node!=NULL)
	{
		if(	strcmp(label, node->label)==0 && 
			(strcmp(node->type, "data")==0 ||
			strcmp(node->type, "code")==0) )
		{
			exists = TRUE;
		}
		node = node->next;
	}
	return exists;
}

int isValidOperator(char* operator)
{
	int valid = TRUE;
	switch(operatorType(operator))
	{
		case GROUP_1:
		case GROUP_2:
		case GROUP_3:
			valid = TRUE;
			break;
		default:
			valid = FALSE;
			break;
	}

	return valid;
}

int isValidOperand(char* operand)
{
	/*Check if it's a register direct addressing*/
	if(	strlen(operand)==2 &&
		operand[0]=='r' &&
		(operand[1]>='0' && operand[1]<='7') )
	{
		return TRUE;
	}
	/*Check if it's a register indirect (asterisk) addressing*/
	else if(	strlen(operand)==3 &&
				operand[0]=='*' &&
				operand[1]=='r' &&
				(operand[2]>='0' && operand[2]<='7') )
	{
		return TRUE;
	}
	/*Check if it's a number*/
	else if(operand[0]=='#' && isNumber(operand, 1))
	{
		return TRUE;
	}
	/*Check if it's a label*/
	else if(isLegalLabel(operand))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

int isValidOperandUse(char* operator, char* op1, char* op2)
{
	int operType = getOperatorNumber(operator);
	int op1_type = operandType(op1), op2_type = operandType(op2);
	int ok = TRUE;
	switch(operType)
	{
		case MOV:
		case ADD:
		case SUB:
			if(op2_type==NUMBER) ok = FALSE;
			break;
		case LEA:
			if(op1_type!=LABEL) ok = FALSE;
			if(op2_type==NUMBER) ok = FALSE;
			break;
		case CLR:
		case NOT:
		case INC:
		case DEC:
		case RED:
			if(strcmp(op1, NO_VALUE)!=0) ok = FALSE;
			if(op2_type==NUMBER) ok = FALSE;
			break;
		case JMP:
		case BNE:
		case JSR:
			if(strcmp(op1, NO_VALUE)!=0) ok = FALSE;
			if(op2_type==NUMBER) ok = FALSE;
			if(op2_type==INDIR_REGISTER) ok = FALSE;
			break;
		case PRN:
			if(strcmp(op1, NO_VALUE)!=0) ok = FALSE;
			break;
		case RTS:
			if(strcmp(op1, NO_VALUE)!=0) ok = FALSE;
			if(strcmp(op2, NO_VALUE)!=0) ok = FALSE;
			break;
		case STOP:
			if(strcmp(op1, NO_VALUE)!=0) ok = FALSE;
			if(strcmp(op2, NO_VALUE)!=0) ok = FALSE;
			break;
	}
	return ok;
}