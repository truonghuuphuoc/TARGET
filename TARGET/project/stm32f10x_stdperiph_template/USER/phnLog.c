

/* Includes ------------------------------------------------------------------*/

#include "phnLog.h"
#include "phnUsart.h"

#include <string.h>

static int cvt(	unsigned int val,
				char *buf,
				long radix,
				char *digits, 
				char build)
{
    char 	temp[10];
    char 	*cp 	= temp;
    int 	length 	= 0;
	
    if ( !val )
    {
        /* Special case */
        *cp++ = '0';
		
		length++;
    }
    else
    {
        while (val)
        {
            *cp++ = digits[val % radix];
            val /= radix;
            length++;
        }
    }

    if(build && (length % 2) != 0)
    {
    	*buf++ = '0';
		length ++;
    }

    while (cp != temp)
    {
        *buf++ = *--cp;
    }
	
    *buf = '\0';

    return (length);
}


void phnLog_SendBuffer(uint8_t *data, uint16_t length)
{
	uint16_t index, post, result;
	uint8_t message[200];
	
	post = 0;
	for(index =0; index < length; index ++)
	{
		result = phnLog_GetString(data[index],(char *) &message[post], 16);
		
		post += result;
		
		message[post] = ' ';
		post ++;
	}
	
	message[post] = '\r';
	post ++;
	
	message[post] = '\n';
	post ++;
	
	phnUsart1_SendBuffer(message, post);
}
	 

void phnLog_SendMessage(char *message)
{
	phnUsart1_SendBuffer((uint8_t *)message, strlen(message));
}



uint16_t phnLog_GetString(uint32_t value, char *buff, uint8_t radix)
{
	uint16_t result = 0;
	
	switch(radix)
	{
		case 16:
			result = cvt(value, buff, 16, "0123456789ABCDEF", 1);
			break;
		
		case 10:
			result = cvt(value, buff, 10, "0123456789", 0);
			break;
	}
	
	return result;
}


void phnLog_MessageDebug(char* message, uint8_t *data, uint16_t length)
{
	uint16_t index, post, result;
	uint8_t sndMesg[300];
	
	strcpy((char *)sndMesg, message);
	post = strlen(message);
	
	for(index =0; index < length; index ++)
	{
		result = phnLog_GetString(data[index],(char *) &sndMesg[post], 16);
		
		post += result;
		
		sndMesg[post] = ' ';
		post ++;
	}
	
	message[post] = '\r';
	post ++;
	
	message[post] = '\n';
	post ++;
	
	phnUsart1_SendBuffer(sndMesg, post);
}
