#include "phnRs485.h"

#include <string.h>
#include <stdio.h>

#include <stm32f10x.h>
#include "phnOsal.h"
#include "phnCrc.h"
#include "phnUsart.h"
#include "phnMessage.h"

volatile uint32_t 	gStartTime 			= 0;

volatile uint8_t	gIsStartFrame		= 0;
volatile uint8_t	gIsEndFrame			= 0;
volatile uint8_t	gIsFisrtNibble		= 0;

volatile uint8_t	gCurrentByte		= 0;
volatile uint8_t	gIsMessageRecv		= 0;


uint8_t	gMessageData[MESG_BUFFER_SIZE];
volatile uint16_t 	gDataPosition		= 0;

static void phnRs485_Reset()
{
	gIsStartFrame 	= 0;
	gIsEndFrame 	= 0;
	gDataPosition	= 0;
	gIsFisrtNibble	= 0;
	gIsMessageRecv	= 0;
}

void phnRs485_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	phnRs485_EnableReceive();
	
	phnRs485_Reset();
	
	phnUsart2_Init();
}


void phnRs485_EnableReceive()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}


void phnRs485_EnableTransmit()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void phnRs485_ReceiveHandler(uint8_t data) 		
{
	uint32_t dwTime;
	
	printf("%02X ", data);
	
	switch (data)
	{
	case MESG_STX:   // start of text
		phnRs485_Reset();
	
		gStartTime		= phnOsal_GetCurrentTickCount();
		gIsStartFrame 	= 1;
		gIsFisrtNibble	= 1;
		break;

	case MESG_ETX:   // end of text
		dwTime = phnOsal_GetElapseTime(gStartTime);
		
		// timeout
		if(dwTime > RS485_RECV_TIMEOUT)
		{
			printf("\r\nTime out 1\r\n");
			phnRs485_Reset();
			break;
		}
		// reset start time
		gStartTime		= phnOsal_GetCurrentTickCount();
		
		// have end frame
		gIsEndFrame 	= 1;
		break;

	default:
		// wait until packet officially starts
		if (!gIsStartFrame)
		{
			printf("\r\nNot start\r\n");
			break;
		}
		
		dwTime = phnOsal_GetElapseTime(gStartTime);
		
		// timeout
		if(dwTime > RS485_RECV_TIMEOUT)
		{
			phnRs485_Reset();
			break;
		}
		// reset start time
		gStartTime = phnOsal_GetCurrentTickCount();

		// check byte is in valid form (4 bits followed by 4 bits complemented)
		if ((data >> 4) != ((data & 0x0F) ^ 0x0F) )
		{
			// bad character
			phnRs485_Reset();
			break;  
		}
		// convert back
		data >>= 4;

		// high-order nibble?
		if(gIsFisrtNibble)
		{
			gCurrentByte = data;
			gIsFisrtNibble = 0;
			break;
		}

		// low-order nibble
		gCurrentByte	<<= 4;
		gCurrentByte 	|= data;
		gIsFisrtNibble 	= 1;

		// if we have the ETX this must be the CRC
		if (gIsEndFrame)
		{
			uint8_t crc = phnCrc_Calculate(gMessageData,gDataPosition); 
			if (crc != gCurrentByte)
			{
				phnRs485_Reset();
			}
			else
			{
				//have message
				gIsMessageRecv = 1;
			}
			
			break;
		}  // end if have ETX already

		// keep adding if not full
		gMessageData[gDataPosition ++] = gCurrentByte;
		break;
	}
}


uint8_t phnRs485_IsMessageReceived()
{
	return gIsMessageRecv;
}

void phnRs485_GetMessageReceived(uint8_t *message, uint16_t *length)
{
	memcpy(message, gMessageData, gDataPosition);
	*length = gDataPosition;
	
	gDataPosition 	= 0;
	gIsMessageRecv	= 0;
}


void phnRs485_SendMessage(uint8_t *message, uint16_t length)
{
	phnRs485_EnableTransmit();
	
	phnUsart2_SendBuffer(message, length);
	
	phnUsart2_SendBuffer((uint8_t *)"\x00\x00", 2);
	
	phnRs485_EnableReceive();
}


