#ifndef __PHN_MESSAGE_H
#define __PHN_MESSAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
	 
#include "phnCompile.h"	 
	 
#define MESG_BUFFER_SIZE	80	 

/*Start message*/
#define MESG_STX			0x02

/*End message*/	 
#define MESG_ETX			0x03

typedef enum
{
	PHN_DEV_ONLINE	= 0xFF,
	PHN_DEV_OFFLINE	= 0xFE,
}phnMessageValue_t;

typedef enum
{
	PHN_STATUS_UPDATE 	= 0x00,
	PHN_STATUS_SEND		= 0x01,
	PHN_STATUS_DONE		= 0x02,
}phnMessageStatus_t;

typedef struct
{
	uint8_t mAck;
#if(PHN_MASTER_PLATFORM)
	uint8_t mValue[PHN_BUFFER_SCORE];
	uint8_t mStatus[PHN_BUFFER_SCORE];
	uint8_t mHead;
	uint8_t mTail;
	uint8_t mDeviceStatus;
#elif(PHN_SLAVE_PLATFORM)
	uint8_t mValue;
	uint8_t mStatus;
	uint32_t mTime;
#endif
}phnMessageType_t;


#if(PHN_MASTER_PLATFORM)
extern phnMessageType_t gMessageControl[PHN_NB_SALVE];
#elif(PHN_SLAVE_PLATFORM)
extern phnMessageType_t gMessageControl;
#endif

/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnMessage_Init(void);
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnMessage_GetMessageFormat(uint8_t *data, uint16_t inLength, uint8_t *message, uint16_t *outLength);


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t phnMessage_GetDeviceValue(uint8_t hostAck, uint8_t deviceAck, uint8_t deviceId);


/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnMessage_UpdateDeviceValue(uint8_t deviceId, uint8_t value);

/**
  * @brief  
  * @param  None
  * @retval None
  */	 
void phnMessage_LogDebug(char* message, uint8_t *data, uint16_t length);

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
