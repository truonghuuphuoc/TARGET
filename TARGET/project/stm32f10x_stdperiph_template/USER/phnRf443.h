#ifndef __PHN_RF443_H
#define __PHN_RF443_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>	 	 

#define RF443_RECV_TIMEOUT		200	 
	 
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRf443_Init(void);

	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRf443_ReceiveHandler(uint8_t data); 	 
	 
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t phnRf443_IsMessageReceived(void);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRf443_GetMessageReceived(uint8_t *message, uint16_t *length);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnRf443_SendMessage(uint8_t *message, uint16_t length); 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_RF443_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
