#ifndef __PHN_CRC_H
#define __PHN_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>	 	 

/**
  * @brief  
  * @param  None
  * @retval Crc value
  */
uint8_t phnCrc_Calculate(uint8_t *data, uint16_t length); 	 
	 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_CRC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
