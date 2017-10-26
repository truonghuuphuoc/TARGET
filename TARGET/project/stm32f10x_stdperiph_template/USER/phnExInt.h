#ifndef __PHN_EXINT_H
#define __PHN_EXINT_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>	 	 
#include "stm32f10x.h"

	 
extern volatile uint32_t gExIntCounter;

/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnExInt_Init(void); 	 
	 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_EXINT_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
