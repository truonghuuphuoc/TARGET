/**
  ******************************************************************************
  * @file    USART/Printf/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


#include "main.h"

#include "stm32f10x.h"
#include "phnUsart.h"
#include "phnRs485.h"
#include "phnRf443.h"
#include "phnOsal.h"
#include "phnMessage.h"
#include "phnCompile.h"
#include "phnExInt.h"
#include "phnLed.h"


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

#if(PLATFORM_MASTER)
	phnMaster_Processing();
#elif(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
	phnSlave_Processing();
#endif	

}

#if(PLATFORM_MASTER)
void phnMaster_Processing()
{
	uint8_t deviceAck		= 0x00;
	uint32_t timeStatus 	= 0x00;
	
	uint8_t dataRequest[MESG_BUFFER_SIZE];
	uint8_t messRequest[MESG_BUFFER_SIZE];
	
	uint16_t messLength	= 0x00;
	
	uint8_t deviceIndex	= 0x00;
	
	uint32_t prevTime	= 0x00;
	uint32_t deltaTime	= 0x00;
	
	uint8_t currState	= 0x00;
	uint8_t prevState	= 0x00;
	
	uint8_t salveDevice[3] = {	PHN_SLAVE_1_DEV_ID, 
								PHN_SLAVE_2_DEV_ID, 
								PHN_SLAVE_3_DEV_ID};
	
	//Disalbe all interrupt
	__disable_irq();

	//System initialize
	SystemInit();

	//Osal initialize
	phnOsal_Init();
	
	//Interrupt priority initialize
	phnNvic_InitGroup();
	
	//Rf443 initialize
	phnRf443_Init();
	
	//Rs485 initialize
	phnRs485_Init();
	
	//Led of status and device intitialize	
	phnLed_Init();
	
	//Enable all interrupt
	__enable_irq();
	
	
	//Set Master Led
	phnLed_SetDeviceLeds(LED_DEV_MASSTER);
	
	//Initialze state machine
	currState = PHN_MAST_IDLE_SLEEP;
	prevState = PHN_MAST_IDLE_SLEEP;
	
	//Get current tick count
	prevTime = phnOsal_GetCurrentTickCount();
	timeStatus = phnOsal_GetCurrentTickCount();
	
	deviceIndex = 0;
	
	while(1)
	{
		switch(currState)
		{
			case PHN_MAST_SEND_REQUEST:
				
				//increase index of slave device
				deviceIndex = (deviceIndex + 1) % 3;
				
				//fill data request
				dataRequest[0] = salveDevice[deviceIndex];
				dataRequest[1] = DEVICE_ID;
				dataRequest[2] = gMessageControl[deviceIndex].mAck;
			
				phnMessage_LogDebug("REQ", dataRequest, 3);
			
				//format message
				phnMessage_GetMessageFormat(dataRequest, 3, messRequest, &messLength);
			
				//send message
				phnRs485_SendMessage(messRequest, messLength);
			
				//get current tick 
				prevTime = phnOsal_GetCurrentTickCount();
			
				//switch to wait response mode
				prevState = currState;
				currState = PHN_MAST_WAIT_RESPONSE;
				break;

			
			case PHN_MAST_WAIT_RESPONSE:
				
				//check host request inforamtion
				if(phnRf443_IsMessageReceived())
				{
					//store current state
					prevState = currState;
					
					//switch to response mode
					currState = PHN_MAST_RESP_HOST;
					
					//set last time receive message
					timeStatus = phnOsal_GetCurrentTickCount();
					phnLed_SetLedStatus();
					
					break;
				}
				
				//recevie message slave respond
				if(phnRs485_IsMessageReceived())
				{
					//handle data response
					phnRs485_GetMessageReceived(messRequest, &messLength);
					
					phnMessage_LogDebug("RSP", messRequest, messLength);
					
					if( messLength == 4 && 
						messRequest[0] == DEVICE_ID && 
						messRequest[1] == salveDevice[deviceIndex])
					{
						phnMessage_UpdateDeviceValue(deviceIndex, messRequest[3]);						
					}
				}
				
				deltaTime = phnOsal_GetElapseTime(prevTime);
				
				//500 millisecond 
				if(deltaTime >= 500)
				{
					//store current state
					prevState = currState;
					
					//switch to response mode
					currState = PHN_MAST_SEND_REQUEST;
					
					//Update value & not change ACK
					gMessageControl[deviceIndex].mValue 	= PHN_DEV_OFFLINE;
					gMessageControl[deviceIndex].mStatus 	= PHN_STATUS_UPDATE;
					
					break;
				}
				
				//led status process
				phnStatus_Processing(&timeStatus);
				
				break;
			
			case PHN_MAST_RESP_HOST:
				
				//check host request inforamtion
				if(phnRf443_IsMessageReceived())
				{
					phnRf443_GetMessageReceived(messRequest, &messLength);
					
					if(	messLength == 3 &&
						messRequest[0] == DEVICE_ID)
					{
						messLength = 0;
						
						//set destination
						dataRequest[messLength] = messRequest[1];
						messLength ++;
						
						//set its address
						dataRequest[messLength] = DEVICE_ID;
						messLength ++;
						
						//set ACK
						dataRequest[messLength] = messRequest[2];
						messLength ++;
						
						//set value of slave 1
						dataRequest[messLength] = phnMessage_GetDeviceValue( messRequest[2], deviceAck, 0);
						messLength ++;
						
						//set value of slave 2
						dataRequest[messLength] = phnMessage_GetDeviceValue( messRequest[2], deviceAck, 1);
						messLength ++;
						
						//set value of slave 3
						dataRequest[messLength] = phnMessage_GetDeviceValue( messRequest[2], deviceAck, 2);
						messLength ++;
						
						//set host ACK
						if(messRequest[2] == deviceAck)
						{
							if(deviceAck)
							{
								deviceAck = 0x00;
							}
							else
							{
								deviceAck = 0x01;
							}
						}
						
						//send message reponse to host
						phnMessage_GetMessageFormat(dataRequest, 6, messRequest, &messLength);
						phnRf443_SendMessage(messRequest, messLength);
					}
					
					break;
				}
				
				//set state machine
				currState = prevState;
				break;
			
			case PHN_MAST_IDLE_SLEEP:
				
				//check host request inforamtion
				if(phnRf443_IsMessageReceived())
				{
					//store current state
					prevState = currState;
					
					//switch to response mode
					currState = PHN_MAST_RESP_HOST;
					
					break;
				}
				
				deltaTime = phnOsal_GetElapseTime(prevTime);
				
				//500 millisecond 
				if(deltaTime >= 500)
				{
					//store current state
					prevState = currState;
					
					//switch to response mode
					currState = PHN_MAST_SEND_REQUEST;
					
					break;
				}
				
				//led status process
				phnStatus_Processing(&timeStatus);
				
				break;
			
			default:
				//get current tick count
				prevTime = phnOsal_GetCurrentTickCount();
				
				//switch to sleep mode
				currState = PHN_MAST_IDLE_SLEEP;
				break;
		}
	}
}
#endif

#if(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
void phnSlave_Processing()
{
	uint32_t timeStatus 	= 0x00;
	
	uint8_t dataRequest[MESG_BUFFER_SIZE];
	uint8_t messRequest[MESG_BUFFER_SIZE];
	
	uint16_t messLength	= 0x00;
	
	//Disalbe all interrupt
	__disable_irq();

	//System initialize
	SystemInit();

	//Osal initialize
	phnOsal_Init();
	
	//Interrupt priority initialize
	phnNvic_InitGroup();
	
	//Debug log nitialize
	phnUsart1_Init();
	
	//Rs485 initialize
	phnRs485_Init();
	
	//Gpio interrrupt initialize
	phnExInt_Init();
	
	//Led of status and device intitialize	
	phnLed_Init();
	
	//Enable all interrupt
	__enable_irq();
	
	//Set Slave Led
#if(PLATFORM_SALVE_1)
	phnLed_SetDeviceLeds(LED_DEV_SLAVE_1);
#elif(PLATFORM_SALVE_2)
	phnLed_SetDeviceLeds(LED_DEV_SLAVE_2);
#elif(PLATFORM_SALVE_3)
	phnLed_SetDeviceLeds(LED_DEV_SLAVE_3);
#endif
	
		
	//Get current tick count
	timeStatus = phnOsal_GetCurrentTickCount();
	
	while(1)
	{
		phnStatus_Processing(&timeStatus);
		
		if(!phnRs485_IsMessageReceived())
		{
			//next process
			continue;
		}
		
		//handle message
		phnRs485_GetMessageReceived(messRequest, &messLength);
		
		phnMessage_LogDebug("RCV", messRequest, messLength);
		
		if( messLength == 3 && 
			messRequest[0] == DEVICE_ID)
		{	
			//set time staus and led
			timeStatus = phnOsal_GetCurrentTickCount();
			phnLed_SetLedStatus();
			
			//destination
			dataRequest[0] = messRequest[1];
			
			//source
			dataRequest[1] = DEVICE_ID;
			
			//set ACK
			dataRequest[2] = messRequest[2];
			
			//set value
			dataRequest[3] = phnMessage_GetDeviceValue(messRequest[2], gMessageControl.mAck, 0);
			
			phnMessage_LogDebug("RSP", dataRequest, 4);
			
			if(gMessageControl.mAck == messRequest[2])
			{
				if(gMessageControl.mAck)
				{
					gMessageControl.mAck = 0x00;
				}
				else
				{
					gMessageControl.mAck = 0x01;
				}
			}
			
			//delay 5ms for host ready
			phnMessage_GetMessageFormat(dataRequest, 4, messRequest, &messLength);
			phnRs485_SendMessage(messRequest, messLength);
		}
	}
}
#endif


void phnStatus_Processing(uint32_t *lastTime)
{
	uint32_t dwTime = phnOsal_GetElapseTime(*lastTime);
	
#if(PLATFORM_MASTER)
	//5 second
	if(dwTime > 5000)
	{
		phnLed_ClearLedStatus();
		*lastTime = phnOsal_GetCurrentTickCount();
	}
#else
	//2 second
	if(dwTime > 3000)
	{
		phnLed_ClearLedStatus();
		*lastTime = phnOsal_GetCurrentTickCount();
	}
#endif
}

void phnNvic_InitGroup()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// Configure the NVIC Preemption Priority Bits   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// Enable the USART1 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	// Enable the USART2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the EXTI3_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the EXTI4_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the EXTI9_5_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the EXTI15_10_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


PUTCHAR_PROTOTYPE
{
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
		
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
