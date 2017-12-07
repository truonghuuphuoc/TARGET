#include "phnMessage.h"
#include "phnCrc.h"
#include "phnOsal.h"


#if(PLATFORM_MASTER)
phnMessageType_t gMessageControl[3] = 
{
	{0x00, PHN_DEV_OFFLINE, PHN_STATUS_UPDATE},
	{0x00, PHN_DEV_OFFLINE, PHN_STATUS_UPDATE},
	{0x00, PHN_DEV_OFFLINE, PHN_STATUS_UPDATE},
};
#elif(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
phnMessageType_t gMessageControl =
{
	0x00, PHN_DEV_ONLINE, PHN_STATUS_UPDATE, 0
};
#endif

void phnMessage_GetMessageFormat(uint8_t *data, uint16_t inLength, uint8_t *message, uint16_t *outLength)
{
	uint8_t value, crc;
	uint16_t index;
	uint16_t position = 0;
	
	//Start message
	message[position] = MESG_STX;
	position ++;
	
	//Data
	for (index = 0; index < inLength; index++)
	{
		//Data first nibble
		value = data[index] >> 4;
		message[position] = ((value << 4) | (value ^ 0x0F));
		position ++;

		//Data second nibble
		value = data[index] & 0x0F;
		message[position] =  ((value << 4) | (value ^ 0x0F));
		position ++;
	}
	
	//End message
	message[position] = MESG_ETX;
	position ++;
	
	//Crc
	crc = phnCrc_Calculate(data, inLength);
	
	//Crc first nibble
	value = crc >> 4;
	message[position] = ((value << 4) | (value ^ 0x0F));
	position ++;

	//Crc second nibble
	value = crc & 0x0F;
	message[position] =  ((value << 4) | (value ^ 0x0F));
	position ++;
	
	*outLength = position;
}


uint8_t phnMessage_GetDeviceValue(uint8_t hostAck, uint8_t deviceAck, uint8_t deviceId)
{
	uint8_t retvalue = PHN_DEV_OFFLINE;

#if(PLATFORM_MASTER)	
	if(hostAck == deviceAck)
	{
		if(gMessageControl[deviceId].mStatus == PHN_STATUS_SEND)
		{
			if( gMessageControl[deviceId].mValue == PHN_DEV_OFFLINE ||
				gMessageControl[deviceId].mValue == PHN_DEV_ONLINE  )
			{
				retvalue = gMessageControl[deviceId].mValue;				
			}
			else
			{
				gMessageControl[deviceId].mValue = PHN_DEV_ONLINE;
				retvalue = PHN_DEV_ONLINE;
			}
			
			//change status
			gMessageControl[deviceId].mStatus = PHN_STATUS_DONE;
			
		}
		else if( gMessageControl[deviceId].mStatus == PHN_STATUS_UPDATE)
		{
			retvalue = gMessageControl[deviceId].mValue;
			
			//change status
			gMessageControl[deviceId].mStatus = PHN_STATUS_SEND;								
		}
		else
		{
			if( gMessageControl[deviceId].mValue == PHN_DEV_OFFLINE ||
				gMessageControl[deviceId].mValue == PHN_DEV_ONLINE  )
			{
				retvalue = gMessageControl[deviceId].mValue;				
			}
			else
			{
				gMessageControl[deviceId].mValue = PHN_DEV_ONLINE;
			}
		}
	}
	else
	{
		if(gMessageControl[deviceId].mStatus == PHN_STATUS_SEND)
		{
			//set value
			retvalue = gMessageControl[deviceId].mValue;
		}
		else if( gMessageControl[deviceId].mStatus == PHN_STATUS_UPDATE)
		{
			retvalue = gMessageControl[deviceId].mValue;
			
			//change status
			gMessageControl[deviceId].mStatus = PHN_STATUS_SEND;								
		}
		else
		{
			if( gMessageControl[deviceId].mValue == PHN_DEV_OFFLINE ||
				gMessageControl[deviceId].mValue == PHN_DEV_ONLINE  )
			{
				retvalue = gMessageControl[deviceId].mValue;				
			}
			else
			{
				gMessageControl[deviceId].mValue = PHN_DEV_ONLINE;
			}
		}
	}
	
#elif(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
	
	uint32_t dwTime = 0;
	
	if(hostAck == deviceAck)
	{
		if(gMessageControl.mStatus == PHN_STATUS_SEND)
		{
			
			retvalue = PHN_DEV_ONLINE;
			
			//change status
			gMessageControl.mStatus = PHN_STATUS_DONE;
			
		}
		else if( gMessageControl.mStatus == PHN_STATUS_UPDATE)
		{
			//check data already
			dwTime = phnOsal_GetElapseTime(gMessageControl.mTime);
			
			if(dwTime > 2000)
			{
				retvalue = gMessageControl.mValue;
			
				//change status
				gMessageControl.mStatus = PHN_STATUS_SEND;								
			}
			else
			{
				retvalue = PHN_DEV_ONLINE;
			}
		}
		else
		{
			retvalue = PHN_DEV_ONLINE;
		}
	}
	else
	{
		if(gMessageControl.mStatus == PHN_STATUS_SEND)
		{
			//set value
			retvalue = gMessageControl.mValue;
		}
		else if( gMessageControl.mStatus == PHN_STATUS_UPDATE)
		{
			//check data already
			dwTime = phnOsal_GetElapseTime(gMessageControl.mTime);
			
			if(dwTime > 1000)
			{
				retvalue = gMessageControl.mValue;
			
				//change status
				gMessageControl.mStatus = PHN_STATUS_SEND;								
			}
			else
			{
				retvalue = PHN_DEV_ONLINE;
			}								
		}
		else
		{
			retvalue = PHN_DEV_ONLINE;
		}
	}
#endif	
	
	return retvalue;
}

void phnMessage_UpdateDeviceValue(uint8_t deviceId, uint8_t value)
{
#if(PLATFORM_MASTER)
	
	//set ACK
	if(gMessageControl[deviceId].mAck)
	{
		gMessageControl[deviceId].mAck = 0x00;
	}
	else
	{
		gMessageControl[deviceId].mAck = 0x01;
	}
	
	if( value == PHN_DEV_OFFLINE ||
		value == PHN_DEV_ONLINE )
	{
		if(gMessageControl[deviceId].mStatus == PHN_STATUS_SEND ||
		   gMessageControl[deviceId].mStatus == PHN_STATUS_UPDATE )
		{
			if( gMessageControl[deviceId].mValue == PHN_DEV_OFFLINE ||
				gMessageControl[deviceId].mValue == PHN_DEV_ONLINE )
			{
				//Update value
				gMessageControl[deviceId].mValue 	= value;
				gMessageControl[deviceId].mStatus 	= PHN_STATUS_UPDATE;	
			}
		}
		else
		{
			//Update value
			gMessageControl[deviceId].mValue 	= value;
			gMessageControl[deviceId].mStatus 	= PHN_STATUS_UPDATE;
		}
	}
	else
	{
		//Update value
		gMessageControl[deviceId].mValue 	= value;
		gMessageControl[deviceId].mStatus 	= PHN_STATUS_UPDATE;
	}
#endif	
}


void phnMessage_LogDebug(char* message, uint8_t *data, uint16_t length)
{
	uint16_t index;
	
	printf("%s: ", message);
	for(index = 0; index < length; index ++)
	{
		printf("%02X ", data[index]); 
	}
	printf("\r\n");
}

