/**
  ******************************************************************************
  * @file    USART/Printf/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "phnOsal.h"
#include "phnExInt.h"
#include "phnCompile.h"
#include "phnRs485.h"
#include "phnRf443.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 


/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	gTickCounter ++;
}

void TIM2_IRQHandler(void){
	if((TIM2->SR &0x0001) != 0)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	}
}

void USART1_IRQHandler(void)
{
	uint8_t recv;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//(void) USART1->DR;
		recv = USART_ReceiveData(USART1);
		
#if(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
		if(recv == 'v' || recv == 'V')
		{
			printf("%s\r\n", FIRMWARE_VER);	
		}
#elif(PLATFORM_MASTER)
		phnRf443_ReceiveHandler(recv);
		
		if(recv == 'v' || recv == 'V')
		{
			printf("%s\r\n", FIRMWARE_VER);	
		}
#else
		phnUsart1_SendData(recv);
#endif		
	} 
}


void USART2_IRQHandler(void)
{
	uint8_t recv;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		//(void) USART2->DR;
		recv = USART_ReceiveData(USART2);
		
		phnRs485_ReceiveHandler(recv);
	} 
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI15_10_IRQHandler(void)
{
			
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	if (EXTI_GetITStatus(EXTI_Line13) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	
	if (EXTI_GetITStatus(EXTI_Line14) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if (EXTI_GetITStatus(EXTI_Line15) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
