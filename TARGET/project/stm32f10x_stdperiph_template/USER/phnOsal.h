#ifndef __PHN_OSAL_H
#define __PHN_OSAL_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>	 	 
#include "stm32f10x.h"

extern volatile uint32_t gTickCounter;

/**
  * @brief  Initialize the system tick with 1 ms to use for delay.
  * @param  None
  * @retval None
  */
void phnOsal_Init(void); 	 
	 
/**
  * @brief  Get number tick count.
  * @param  None
  * @retval Current tick count
  */
uint32_t phnOsal_GetCurrentTickCount(void);


	 
/**
  * @brief  Get time eslpase from start time.
  * @param  dwFrom: start time
  * @retval Elapse time
  */
uint32_t phnOsal_GetElapseTime(uint32_t dwFrom);	 


/**
  * @brief  Delay in micro seconds
  * @param  dwUs: the number of micro seconds need to delay
  * @retval	None
  */
void phnOsal_DelayUs(uint32_t dwUs);


/**
  * @brief  Delay in milliseconds
  * @param  dwUs: the number of milliseconds need to delay
  * @retval	None
  */ 
void phnOsal_DelayMs(uint32_t dwMs); 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_OSAL_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
