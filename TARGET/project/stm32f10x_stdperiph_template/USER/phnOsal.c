#include "phnOsal.h"
#include <stm32f10x.h>

volatile uint32_t gTickCounter = 0;


void phnOsal_Init(void) 
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

uint32_t phnOsal_GetCurrentTickCount()
{
	return gTickCounter;
}


uint32_t phnOsal_GetElapseTime(uint32_t dwFrom)
{
	
	uint32_t curTime = phnOsal_GetCurrentTickCount();

	if (curTime >= dwFrom)
	{
		return (curTime - dwFrom);
	}
	else
	{
		return ((uint32_t)0xFFFFFFFF - dwFrom + curTime);
	}	
}

void phnOsal_DelayUs(uint32_t dwUs)
{
	uint32_t count = 2*dwUs;
	for(;count != 0; count --);
}

void phnOsal_DelayMs(uint32_t dwMs)
{
	uint32_t timeStart = gTickCounter + dwMs;
	
	if(timeStart < gTickCounter)
	{
		phnOsal_DelayUs(10);
	}
	else
	{
		while(gTickCounter < timeStart);
	}
}
