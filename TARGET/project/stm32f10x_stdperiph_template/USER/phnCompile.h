#ifndef __PHN_COMPILE_H
#define __PHN_COMPILE_H
#ifdef __cplusplus
 extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>	

	 
#define STRINGIZE2(s) 					#s
#define STRINGIZE(s) 					STRINGIZE2(s)	 
	 
#define	FIRMWARE_VERSION_MAJOR	        1
#define FIRMWARE_VERSION_MINOR	        0
#define FIRMWARE_REVISION               3
	 
	 
#define PHN_MASTER_DEV_ID				0xA0
#define PHN_SLAVE_1_DEV_ID				0xA1
#define PHN_SLAVE_2_DEV_ID				0xA2
#define PHN_SLAVE_3_DEV_ID				0xA3	 
	 
#define PLATFORM_MASTER 				1
#define PLATFORM_SALVE_1 				0
#define PLATFORM_SALVE_2 				0
#define PLATFORM_SALVE_3 				0

#if (	PLATFORM_MASTER 	+ \
		PLATFORM_SALVE_1 	+ \
		PLATFORM_SALVE_2 	+ \
		PLATFORM_SALVE_3  	!= 1 )
#error "Only support one platform at a time !"
#endif

#if(PLATFORM_MASTER)
#define DEVICE_NAME		"Master: "
#define DEVICE_ID		PHN_MASTER_DEV_ID
#endif


#if(PLATFORM_SALVE_1)
#define DEVICE_NAME		"Salve 1: "
#define DEVICE_ID		PHN_SLAVE_1_DEV_ID
#endif

#if(PLATFORM_SALVE_2)
#define DEVICE_NAME		"Salve 2: "
#define DEVICE_ID		PHN_SLAVE_2_DEV_ID
#endif

#if(PLATFORM_SALVE_3)
#define DEVICE_NAME		"Salve 3: "
#define DEVICE_ID		PHN_SLAVE_3_DEV_ID
#endif

#define FIRMWARE_VER	DEVICE_NAME "("STRINGIZE(DEVICE_ID)") " STRINGIZE(FIRMWARE_VERSION_MAJOR) "." STRINGIZE(FIRMWARE_VERSION_MINOR) "." STRINGIZE(FIRMWARE_REVISION)


#ifdef __cplusplus
}
#endif

#endif /*__PHN_COMPILE_H */
