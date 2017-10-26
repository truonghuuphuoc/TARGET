#ifndef __PHN_USART_H
#define __PHN_USART_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>	 
	 
#include "stm32f10x.h"



/**
 * @brief Definition for COM port1, connected to USART1
 */ 
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
 
/**
  * @brief  USART 1.
  * @param  None
  * @retval None
  */ 
void phnUsart1_Init(void);


/**
  * @brief  USART 2.
  * @param  None
  * @retval None
  */ 
void phnUsart2_Init(void);


/**
  * @brief  USART 1 send 1 byte.
  * @param  data: data sent
  * @retval None
  */
void phnUsart1_SendData(uint8_t data);


/**
  * @brief  USART 2 send 1 byte.
  * @param  data: data sent
  * @retval None
  */
void phnUsart2_SendData(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /*__PHN_USART_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
