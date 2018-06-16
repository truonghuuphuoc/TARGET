#include "phnRf443.h"

#include <string.h>
#include <stdio.h>

#include <stm32f10x.h>
#include "phnOsal.h"
#include "phnCrc.h"
#include "phnUsart.h"
#include "phnMessage.h"

volatile uint32_t 	gRf433_StartTime 			= 0;

volatile uint8_t	gRf433_IsStartFrame			= 0;
volatile uint8_t	gRf433_IsEndFrame			= 0;
volatile uint8_t	gRf433_IsFisrtNibble		= 0;

volatile uint8_t	gRf433_CurrentByte			= 0;
volatile uint8_t	gRf433_IsMessageRecv		= 0;


uint8_t	gRf433_MessageData[MESG_BUFFER_SIZE];
volatile uint16_t 	gRf433_DataPosition			= 0;

static void phnRf443_Reset()
{
	gRf433_IsStartFrame 	= 0;
	gRf433_IsEndFrame 		= 0;
	gRf433_DataPosition		= 0;
	gRf433_IsFisrtNibble	= 0;
	gRf433_IsMessageRecv	= 0;
}

void phnRf443_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	phnRf443_Reset();
	phnUsart1_Init();
	
		
	/* Enable the Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	
	/**
	 *	Pin B.15 - M0 
	 */
	
	/* Configure pin as output pull-up */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	/**
	 *	Pin A.8 - M1 
	 */
	
	/* Configure pin as output pull-up */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	/**
	 *	Pin A.11 - AUX 
	 */
	
	/* Configure pin as output pull-up */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Set mode 00
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
}

void phnRf443_ReceiveHandler(uint8_t data) 		
{
	uint32_t dwTime;
	
	switch (data)
	{
	case MESG_STX:   // start of text
		phnRf443_Reset();
	
		gRf433_StartTime		= phnOsal_GetCurrentTickCount();
		gRf433_IsStartFrame 	= 1;
		gRf433_IsFisrtNibble	= 1;
		break;

	case MESG_ETX:   // end of text
		dwTime = phnOsal_GetElapseTime(gRf433_StartTime);
		
		// timeout
		if(dwTime > RF443_RECV_TIMEOUT)
		{
			phnRf443_Reset();
			break;
		}
		// reset start time
		gRf433_StartTime		= phnOsal_GetCurrentTickCount();
		
		// have end frame
		gRf433_IsEndFrame 	= 1;
		break;

	default:
		// wait until packet officially starts
		if (!gRf433_IsStartFrame)
		{
			break;
		}
		
		dwTime = phnOsal_GetElapseTime(gRf433_StartTime);
		
		// timeout
		if(dwTime > RF443_RECV_TIMEOUT)
		{
			phnRf443_Reset();
			break;
		}
		// reset start time
		gRf433_StartTime = phnOsal_GetCurrentTickCount();

		// check byte is in valid form (4 bits followed by 4 bits complemented)
		if ((data >> 4) != ((data & 0x0F) ^ 0x0F) )
		{
			// bad character
			phnRf443_Reset();
			break;  
		}
		// convert back
		data >>= 4;

		// high-order nibble?
		if(gRf433_IsFisrtNibble)
		{
			gRf433_CurrentByte = data;
			gRf433_IsFisrtNibble = 0;
			break;
		}

		// low-order nibble
		gRf433_CurrentByte	<<= 4;
		gRf433_CurrentByte 	|= data;
		gRf433_IsFisrtNibble 	= 1;

		// if we have the ETX this must be the CRC
		if (gRf433_IsEndFrame)
		{
			uint8_t crc = phnCrc_Calculate(gRf433_MessageData,gRf433_DataPosition); 
			if (crc != gRf433_CurrentByte)
			{
				phnRf443_Reset();
			}
			else
			{
				//have message
				gRf433_IsMessageRecv = 1;
			}
			
			break;
		}  // end if have ETX already

		// keep adding if not full
		gRf433_MessageData[gRf433_DataPosition ++] = gRf433_CurrentByte;
		break;
	}
}


uint8_t phnRf443_IsMessageReceived()
{
	return gRf433_IsMessageRecv;
}

void phnRf443_GetMessageReceived(uint8_t *message, uint16_t *length)
{
	memcpy(message, gRf433_MessageData, gRf433_DataPosition);
	*length = gRf433_DataPosition;
	
	gRf433_DataPosition 	= 0;
	gRf433_IsMessageRecv	= 0;
}


void phnRf443_SendMessage(uint8_t *message, uint16_t length)
{
	phnUsart1_SendBuffer(message, length);
}


