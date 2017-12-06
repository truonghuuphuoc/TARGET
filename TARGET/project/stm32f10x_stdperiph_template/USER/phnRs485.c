#include "phnRs485.h"

#include <string.h>
#include <stdio.h>

#include <stm32f10x.h>
#include "phnOsal.h"
#include "phnCrc.h"
#include "phnUsart.h"
#include "phnMessage.h"

volatile uint32_t 	gRs485_StartTime 			= 0;

volatile uint8_t	gRs485_IsStartFrame			= 0;
volatile uint8_t	gRs485_IsEndFrame			= 0;
volatile uint8_t	gRs485_IsFisrtNibble		= 0;

volatile uint8_t	gRs485_CurrentByte			= 0;
volatile uint8_t	gRs485_IsMessageRecv		= 0;


uint8_t	gRs485_MessageData[MESG_BUFFER_SIZE];
volatile uint16_t 	gRs485_DataPosition			= 0;

static void phnRs485_Reset()
{
	gRs485_IsStartFrame 	= 0;
	gRs485_IsEndFrame 		= 0;
	gRs485_DataPosition		= 0;
	gRs485_IsFisrtNibble	= 0;
	gRs485_IsMessageRecv	= 0;
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
	
	switch (data)
	{
	case MESG_STX:   // start of text
		phnRs485_Reset();
	
		gRs485_StartTime		= phnOsal_GetCurrentTickCount();
		gRs485_IsStartFrame 	= 1;
		gRs485_IsFisrtNibble	= 1;
		break;

	case MESG_ETX:   // end of text
		dwTime = phnOsal_GetElapseTime(gRs485_StartTime);
		
		// timeout
		if(dwTime > RS485_RECV_TIMEOUT)
		{
			phnRs485_Reset();
			break;
		}
		// reset start time
		gRs485_StartTime		= phnOsal_GetCurrentTickCount();
		
		// have end frame
		gRs485_IsEndFrame 		= 1;
		break;

	default:
		// wait until packet officially starts
		if (!gRs485_IsStartFrame)
		{
			break;
		}
		
		dwTime = phnOsal_GetElapseTime(gRs485_StartTime);
		
		// timeout
		if(dwTime > RS485_RECV_TIMEOUT)
		{
			phnRs485_Reset();
			break;
		}
		// reset start time
		gRs485_StartTime = phnOsal_GetCurrentTickCount();

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
		if(gRs485_IsFisrtNibble)
		{
			gRs485_CurrentByte = data;
			gRs485_IsFisrtNibble = 0;
			break;
		}

		// low-order nibble
		gRs485_CurrentByte	<<= 4;
		gRs485_CurrentByte 	|= data;
		gRs485_IsFisrtNibble 	= 1;

		// if we have the ETX this must be the CRC
		if (gRs485_IsEndFrame)
		{
			uint8_t crc = phnCrc_Calculate(gRs485_MessageData,gRs485_DataPosition); 
			if (crc != gRs485_CurrentByte)
			{
				phnRs485_Reset();
			}
			else
			{
				//have message
				gRs485_IsMessageRecv = 1;
			}
			
			break;
		}  // end if have ETX already

		// keep adding if not full
		gRs485_MessageData[gRs485_DataPosition ++] = gRs485_CurrentByte;
		break;
	}
}


uint8_t phnRs485_IsMessageReceived()
{
	return gRs485_IsMessageRecv;
}

void phnRs485_GetMessageReceived(uint8_t *message, uint16_t *length)
{
	memcpy(message, gRs485_MessageData, gRs485_DataPosition);
	*length = gRs485_DataPosition;
	
	gRs485_DataPosition 	= 0;
	gRs485_IsMessageRecv	= 0;
}


void phnRs485_SendMessage(uint8_t *message, uint16_t length)
{
	phnRs485_EnableTransmit();
	
	phnUsart2_SendBuffer(message, length);
	
	phnUsart2_SendBuffer((uint8_t *)"\x00\x00", 2);
	
	phnRs485_EnableReceive();
}


