#ifndef __PHN_LOG_H
#define __PHN_LOG_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>	 
	 

/**
  * @brief  
  * @param  
  * @retval None
  */
void phnLog_SendBuffer(uint8_t *data, uint16_t length);
	 
/**
  * @brief  
  * @param  
  * @retval None
  */
void phnLog_SendMessage(char *message);	


/**
  * @brief  
  * @param  
  * @retval 
  */
uint16_t phnLog_GetString(uint32_t value, char *buff, uint8_t radix);


/**
  * @brief  
  * @param  None
  * @retval None
  */	 
void phnLog_MessageDebug(char* message, uint8_t *data, uint16_t length);


#ifdef __cplusplus
}
#endif

#endif /*__PHN_LOG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
