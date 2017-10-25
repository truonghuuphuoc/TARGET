

/* Includes ------------------------------------------------------------------*/

#include "usart.h"

void USART1_Init(void){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
       
	  /* USARTx configured as follow:
		- BaudRate = 115200 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	  */
	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  RCC_APB2PeriphClockCmd(USART1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	  RCC_APB2PeriphClockCmd(USART1_CLK , ENABLE);
	  
	  /* Configure USART Tx as alternate function push-pull */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);

	  /* Configure USART Rx as input floating */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	  GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);

	  /* USART configuration */
	  USART_Init(USART1, &USART_InitStructure);

	  USART_ClearFlag(USART1,USART_FLAG_RXNE);	
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	  /* Enable USART */
	  USART_Cmd(USART1, ENABLE);
 
}

void USART2_Init(void){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
       
	  /* USARTx configured as follow:
		- BaudRate = 115200 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	  */
	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  
	  RCC_APB2PeriphClockCmd(USART2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	  RCC_APB1PeriphClockCmd(USART2_CLK , ENABLE);
	  
	  /* Configure USART Tx as alternate function push-pull */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(USART2_GPIO_PORT, &GPIO_InitStructure);

	  /* Configure USART Rx as input floating */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	  GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);

	  /* USART configuration */
	  USART_Init(USART2, &USART_InitStructure);

	  USART_ClearFlag(USART2,USART_FLAG_RXNE);	
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	  //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	  /* Enable USART */
	  USART_Cmd(USART2, ENABLE);
 
}

void Usart1_SendData_8bit(uint8_t data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,data);	
}

void Usart2_SendData_8bit(uint8_t data)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	USART_SendData(USART2,data);	
}

