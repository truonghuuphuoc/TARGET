#ifndef __MAIN_H
#define __MAIN_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "phnCompile.h"

typedef enum
{
	PHN_MAST_SEND_REQUEST	= 0x00,
	PHN_MAST_WAIT_RESPONSE	= 0x01,
	PHN_MAST_RESP_HOST		= 0x02,
	PHN_MAST_IDLE_SLEEP		= 0x03,
}phnMasterProcess_t;
 

/**
  * @brief  Initialize the interrupt priority.
  * @param  None
  * @retval None
  */	 
void phnNvic_InitGroup(void);

#if(PLATFORM_MASTER)
/**
  * @brief  Main process of master.
  * @param  None
  * @retval None
  */	 
void phnMaster_Processing(void);
#endif
	 

#if(PLATFORM_SALVE_1 || PLATFORM_SALVE_2 || PLATFORM_SALVE_3)
/**
  * @brief  Main process of salve.
  * @param  None
  * @retval None
  */	 
void phnSlave_Processing(void);	
#endif

void phnStatus_Processing(uint32_t *lastTime);

#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */
