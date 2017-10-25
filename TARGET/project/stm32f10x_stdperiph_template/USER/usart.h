#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>	 
	 
#include "stm32f10x.h"



/* Private functions ---------------------------------------------------------*/
#define USART1_CLK				RCC_APB2Periph_USART1
#define USART1_TX_PIN			GPIO_Pin_9
#define USART1_GPIO_CLK			RCC_APB2Periph_GPIOA
#define USART1_RX_PIN			GPIO_Pin_10
#define USART1_GPIO_PORT		GPIOA


/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define USART2_CLK				RCC_APB1Periph_USART2
#define USART2_TX_PIN			GPIO_Pin_2
#define USART2_GPIO_CLK			RCC_APB2Periph_GPIOA
#define USART2_RX_PIN			GPIO_Pin_3
#define USART2_GPIO_PORT		GPIOA
 
void USART1_Init(void);
void USART2_Init(void);

void Usart1_SendData_8bit(uint8_t data);
void Usart2_SendData_8bit(uint8_t data);


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */


#ifdef __cplusplus
}
#endif

#endif /*__USART_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
