#ifndef __PHN_RS485_H
#define __PHN_RS485_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>	 	 

#define RS485_RECV_TIMEOUT		50	 
	 
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_Init(void);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_EnableReceive(void);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_EnableTransmit(void); 
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_ReceiveHandler(uint8_t data); 	 
	 
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t phnRs485_IsMessageReceived(void);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_GetMessageReceived(uint8_t *message, uint16_t *length);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRs485_SendMessage(uint8_t *message, uint16_t length); 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_RS485_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
