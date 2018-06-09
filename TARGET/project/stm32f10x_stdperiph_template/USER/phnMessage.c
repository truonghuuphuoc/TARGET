#include "phnMessage.h"
#include "phnCrc.h"
#include "phnOsal.h"



#if(PHN_MASTER_PLATFORM)
phnMessageType_t gMessageControl[PHN_NB_SALVE] = 
{
	{	/*01*/	
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*02*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*03*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*04*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*05*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*06*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*07*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*08*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*09*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*10*/	
		0x00, 
		0x00,
		PHN_STATUS_DONE,
		PHN_DEV_OFFLINE,
	},
	{	/*11*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*12*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*13*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*14*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*15*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*16*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*17*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
	{	/*18*/
		0x00, 
		{0x00,0x00,0x00},
		{PHN_STATUS_DONE,PHN_STATUS_DONE,PHN_STATUS_DONE},
		0x00,
		PHN_DEV_OFFLINE,
	},
};
#elif(PHN_SLAVE_PLATFORM)
phnMessageType_t gMessageControl =
{
	0x00, PHN_DEV_ONLINE, PHN_STATUS_UPDATE, 0
};
#endif

void phnMessage_Init()
{
#if(PHN_MASTER_PLATFORM)	
	int idx;
	
	for(idx = 0; idx < PHN_NB_SALVE; idx ++)
	{
		gMessageControl[idx].mAck = 0x00;
		gMessageControl[idx].mDeviceStatus = PHN_DEV_OFFLINE;		
		gMessageControl[idx].mHead = 0x00;
		gMessageControl[idx].mTail = 0x00;
		memset(gMessageControl[idx].mValue, 0x00, PHN_BUFFER_SCORE);
		memset(gMessageControl[idx].mStatus, PHN_STATUS_DONE, PHN_BUFFER_SCORE);
	}
	
#endif
}

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

#if(PHN_MASTER_PLATFORM)
	uint8_t next;

	if (gMessageControl[deviceId].mHead == gMessageControl[deviceId].mTail)
	{
		retvalue = gMessageControl[deviceId].mDeviceStatus;
	}
	else if (gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] == PHN_STATUS_UPDATE)
	{
		retvalue = gMessageControl[deviceId].mValue[gMessageControl[deviceId].mHead];

		//change status
		gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] = PHN_STATUS_SEND;
	}
	else if (gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] == PHN_STATUS_SEND)
	{
		if (hostAck == deviceAck)
		{
			
			next = (gMessageControl[deviceId].mHead + 1) % PHN_BUFFER_SCORE;

			//change status
			gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] = PHN_STATUS_DONE;

			gMessageControl[deviceId].mHead = next;

			if (gMessageControl[deviceId].mHead == gMessageControl[deviceId].mTail)
			{
				retvalue = gMessageControl[deviceId].mDeviceStatus;
			}
			else if (gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] == PHN_STATUS_UPDATE)
			{
				retvalue = gMessageControl[deviceId].mValue[gMessageControl[deviceId].mHead];

				//change status
				gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] = PHN_STATUS_SEND;
			}
			else
			{
				retvalue = gMessageControl[deviceId].mDeviceStatus;
			}
			
		}
		else
		{
			//send again
			retvalue = gMessageControl[deviceId].mValue[gMessageControl[deviceId].mHead];
		}
	}
	else
	{
		retvalue = gMessageControl[deviceId].mDeviceStatus;
	}

#elif(PHN_SLAVE_PLATFORM)

	uint32_t dwTime = 0;

	if (gMessageControl.mStatus == PHN_STATUS_DONE)
	{
		retvalue = PHN_DEV_ONLINE;
	}
	else if (gMessageControl.mStatus == PHN_STATUS_UPDATE)
	{
		//check data already
		dwTime = phnOsal_GetElapseTime(gMessageControl.mTime);

		if (dwTime > PHN_SLAVE_TARGET_UPDATE)
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
	else if (gMessageControl.mStatus == PHN_STATUS_SEND)
	{
		if (hostAck == deviceAck)
		{
			retvalue = PHN_DEV_ONLINE;

			//change status
			gMessageControl.mStatus = PHN_STATUS_DONE;
		}
		else
		{
			//send again
			retvalue = gMessageControl.mValue;
		}
	}
	else
	{
		retvalue = PHN_DEV_ONLINE;
	}
#endif	

	return retvalue;
}


void phnMessage_UpdateDeviceValue(uint8_t deviceId, uint8_t value)
{
#if(PHN_MASTER_PLATFORM)	

	uint8_t next;

	if (value == PHN_DEV_OFFLINE)
	{
		gMessageControl[deviceId].mDeviceStatus = value;
	}
	else if (value == PHN_DEV_ONLINE)
	{
		gMessageControl[deviceId].mDeviceStatus = value;
	}
	else
	{
		

		// check if FIFO has room
		next = (gMessageControl[deviceId].mTail + 1) % PHN_BUFFER_SCORE;

		if (next == gMessageControl[deviceId].mHead) 
		{
			//update value
			gMessageControl[deviceId].mValue[gMessageControl[deviceId].mTail] = value;
			gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mTail] = PHN_STATUS_UPDATE;

			gMessageControl[deviceId].mTail = next;

			gMessageControl[deviceId].mValue[gMessageControl[deviceId].mHead] = 0x00;
			gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mHead] = PHN_STATUS_DONE;

			next = (gMessageControl[deviceId].mHead + 1) % PHN_BUFFER_SCORE;
			gMessageControl[deviceId].mHead = next;
		}
		else
		{ 
			//update value
			gMessageControl[deviceId].mValue[gMessageControl[deviceId].mTail] = value;
			gMessageControl[deviceId].mStatus[gMessageControl[deviceId].mTail] = PHN_STATUS_UPDATE;

			gMessageControl[deviceId].mTail = next;
		}

		gMessageControl[deviceId].mDeviceStatus = PHN_DEV_ONLINE;
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

