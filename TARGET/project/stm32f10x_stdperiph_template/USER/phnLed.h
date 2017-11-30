#ifndef __PHN_LED_H
#define __PHN_LED_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>	 	 

#define LED_DEV_MASSTER		0x00
#define LED_DEV_SLAVE_1		0x01			 
#define LED_DEV_SLAVE_2		0x02
#define LED_DEV_SLAVE_3		0x03
	 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void phnLed_Init(void);


/**
  * @brief  Set led to indicate device with 3 leds
  * @param  
  * @retval None
  */
void phnLed_SetDeviceLeds(uint8_t device);

/**
  * @brief  Turn on LED status
  * @param  None
  * @retval None
  */
void phnLed_SetLedStatus(void);

/**
  * @brief  Turn off LED status
  * @param  None
  * @retval None
  */
void phnLed_ClearLedStatus(void);


/**
  * @brief  Turn off all LED
  * @param  None
  * @retval None
  */
void phnLed_ClearAllLed(void);

#ifdef __cplusplus
}
#endif

#endif /*__PHN_LED_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
