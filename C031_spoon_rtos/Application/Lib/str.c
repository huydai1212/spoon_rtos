/*******************************************************************************
** @file:      	str.c
** @author:    	HUY DAI
** @version:   	V1.0.0
** @time:      	11:23 AM Thursday, October 10, 2019
** @brief:		 
** @revision:
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
*******************************************************************************/

/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include "str.h"
#include "cmn.h"
#include "string.h"
#include <ctype.h>
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/

/*******************************************************************************
**                           FUNCTION DEFINITIONS
*******************************************************************************/

/*
** @brief		parse string from betweent 2 charactor 'startChar' and 'endChar'
** @param		inputStr: string input
** @return		
** @time		10:04 AM Tuesday, 27 November, 2018
** @Note:       1, This function automatically appends a null character to the character sequence resulting
**              2, The minimum values of startIndex and endChar is 1.
**              if startIndex = 0 split start from inputStr[0] and
**              if endIndex = 0 split end from inputStr[length]
** @revision	
**             
*/
void strnsplit(char *inputStr, uint8_t startChar, uint16_t startIndex, uint8_t endChar, uint16_t endIndex, char *retStr, uint16_t retSize)
{
    ASSERT(inputStr);
    ASSERT(retStr);
	uint16_t u16Len = strlen(inputStr);
	uint16_t u16IndexFirst = 0xffff;
	uint16_t u16IndexLast = 0xffff;
	uint16_t i;
	for (i = 0; i < u16Len; ++i)
	{
		if (( inputStr[i] == startChar) && (0 != startIndex) && (0 != startChar))
		{
			startIndex--;
			if (0 == startIndex)
			{
				u16IndexFirst = i + 1;
			}
		}
		
		if (( inputStr[i] == endChar) && (0 != endIndex) && (0 != endChar))
		{
			endIndex--;
			if (0 == endIndex)
			{
				u16IndexLast = i;
			}
		}
	}
	if(0 == startChar)
	{
		u16IndexFirst = 0;
	}
	if(0 == endChar)
	{
		u16IndexLast = u16Len;
	}
	//LREP("first-last[%d,%d]\r\n",u16IndexFirst,u16IndexLast);
	if((u16IndexLast <= u16IndexFirst ) || (0xffff == u16IndexLast) || (0xffff == u16IndexFirst))
	{
		retStr[0] = 0; 
	}
	else
	{
		if((u16IndexLast - u16IndexFirst ) < retSize)
		{
			retSize = u16IndexLast - u16IndexFirst ;
		}
		else
		{
			retSize--;
		}
		strncpy(retStr,(char*)(inputStr + u16IndexFirst), retSize);
		/* Append a null charactor */
		retStr[retSize] = 0x00;
	}
}


/*
** @brief		search string ptr2 in ptr1
** @param		ptr1, ptr2
** @return
** @time
** @Note:		if ptr1 = hello world, ptr2 = hello => return 1,
** 				if ptr1 = 1hello world, ptr2 = hello => return 2,
**
** @revision
**
*/
uint16_t strSearch(char  *ptr1, char *ptr2)
{
	uint16_t max_length = 0;
	char * old_ptr2=ptr2;
	uint16_t i = 0;
	uint16_t flag = 0;
	max_length=strlen((char *)ptr1)+1;
	old_ptr2=ptr2;
	flag=0;
	for(i=0;i<max_length;i++)
	{
		if(*ptr2==0)
		{
			return(flag);
		}
		else if(*(ptr1+i)!=*ptr2)
		{
			if(flag!=0)//
			{
				i=flag;
				flag=0;
				ptr2=old_ptr2;
			}
		}
		else
		{
			if(flag==0)
			{
				flag=i+1;
			}
			ptr2++;
		}
	}
	return(0);
}


/*
** @brief       check all characters input string is hexadecimal digit
** @param       inputStr: string input.
** @return      TRUE if is hexadecimal then FALSE.
** @time
** @Note:       "1234567890ABCDef" is hexadecimal string.
**              "1234567890ABCDefiklm" is not hexadecimal string.
** @revision
**
*/
BOOL strIsxdigit(char *inputStr, uint32_t length)
{
    uint32_t i = 0;
    if(0 == length)
    {
        return FALSE;
    }
    for(i = 0; i < length; ++i)
    {
        if(0 == isxdigit((int)inputStr[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}


/******************************** End of file **********************************/
