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
int main(void){

	
	uint8_t message[256];
	uint16_t length, index;
	
#if(PLATFORM_MASTER)
	uint8_t data[] = {0x12, 0x34, 0x05, 0x67};
	uint32_t prevTime, deltaTime;
#else
	uint8_t data[] = {0x21, 0x43, 0x50, 0x76};
#endif

	__disable_irq();

	SystemInit();
	
	phnOsal_Init();
	
	phnNVIC_InitGroup();
	phnUsart1_Init();
	phnRs485_Init();	
	__enable_irq();

	printf("start\r\n");
	
		
#if(PLATFORM_MASTER)


		while (1)
		{
		
			phnOsal_DelayMs(1000);
			
			phnMessage_GetMessageFormat(data, sizeof(data), message, &length);
			
			for(index = 0; index < length; index ++)
			{
				printf("%02X ", message[index]);
			}
			printf("\r\n");
			
			phnRs485_SendMessage(message, length);
			
			prevTime = phnOsal_GetCurrentTickCount();
			
			while(1)
			{
				if(phnRs485_IsMessageReceived())
				{
					phnRs485_GetMessageReceived(message, &length);
					
					for(index = 0; index < length; index ++)
					{
						printf("%02X ", message[index]);
					}
					printf("\r\n");
				}
				
				deltaTime = phnOsal_GetElapseTime(prevTime);
				
				if(deltaTime > 500)
				{
					break;
				}
			}
		}
#else
		
		
		while(1)
		{
			
			if(phnRs485_IsMessageReceived())
			{
				phnRs485_GetMessageReceived(message, &length);
				
				for(index = 0; index < length; index ++)
				{
					printf("%02X ", message[index]);
				}
				printf("\r\n");
				
				phnMessage_GetMessageFormat(data, sizeof(data), message, &length);
				
				for(index = 0; index < length; index ++)
				{
					printf("%02X ", message[index]);
				}
				printf("\r\n");
				
				phnOsal_DelayMs(5);
				
				phnRs485_SendMessage(message, length);
			}
		}
#endif
}




void phnNVIC_InitGroup()
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
	
	// Enable the EXTI4_IRQn Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the EXTI9_5_IRQn Interrupt
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
