#include "phnCrc.h"
#include <stm32f10x.h>


uint8_t phnCrc_Calculate(uint8_t *data, uint16_t length) 	
{
	uint8_t crc = 0;
	uint8_t inbyte;
	uint8_t i, mix;
	
	uint16_t index;
	
	for(index =0; index < length; index ++)
	{
		inbyte = data[index];
		
		for (i = 8; i; i--)
		{
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			
			if (mix)
			{
				crc ^= 0x8C;
			}
			
			inbyte >>= 1;
		}
	}
	
	return crc;
}


