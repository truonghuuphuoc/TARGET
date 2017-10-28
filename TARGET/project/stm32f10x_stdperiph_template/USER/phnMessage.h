#ifndef __PHN_MESSAGE_H
#define __PHN_MESSAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
	 
#define MESG_BUFFER_SIZE	200	 

/*Start message*/
#define MESG_STX			0x02

/*End message*/	 
#define MESG_ETX			0x03	 

/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnMessage_GetMessageFormat(uint8_t *data, uint16_t inLength, uint8_t *message, uint16_t *outLength); 	 
	 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_MESSAGE_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
