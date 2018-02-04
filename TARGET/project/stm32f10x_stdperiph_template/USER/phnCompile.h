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
#define FIRMWARE_REVISION               4
	 
#define PHN_NB_SALVE					4	 
	 
	 
#define PHN_MASTER_DEV_ID				0xA0
#define PHN_SLAVE_1_DEV_ID				0xA1
#define PHN_SLAVE_2_DEV_ID				0xA2
#define PHN_SLAVE_3_DEV_ID				0xA3
#define PHN_SLAVE_4_DEV_ID				0xA4		 
	 
#define PLATFORM_MASTER 				0
#define PLATFORM_SLAVE_1 				0
#define PLATFORM_SLAVE_2 				0
#define PLATFORM_SLAVE_3 				0
#define PLATFORM_SLAVE_4 				1

#if (	PLATFORM_MASTER 	+ \
		PLATFORM_SLAVE_1 	+ \
		PLATFORM_SLAVE_2 	+ \
		PLATFORM_SLAVE_3 	+ \
		PLATFORM_SLAVE_4  	!= 1 )
#error "Only support one platform at a time !"
#endif

#if(PLATFORM_MASTER)
#define PHN_MASTER_PLATFORM		1
#else
#define PHN_MASTER_PLATFORM		0
#endif

#if (	PLATFORM_SLAVE_1 	+ \
		PLATFORM_SLAVE_2 	+ \
		PLATFORM_SLAVE_3 	+ \
		PLATFORM_SLAVE_4  	== 1 )
#define PHN_SLAVE_PLATFORM		1
#else
#define PHN_SLAVE_PLATFORM		0
#endif

#if(PLATFORM_MASTER)
#define DEVICE_NAME		"Master: "
#define DEVICE_ID		PHN_MASTER_DEV_ID
#endif


#if(PLATFORM_SLAVE_1)
#define DEVICE_NAME		"Salve 1: "
#define DEVICE_ID		PHN_SLAVE_1_DEV_ID
#endif

#if(PLATFORM_SLAVE_2)
#define DEVICE_NAME		"Salve 2: "
#define DEVICE_ID		PHN_SLAVE_2_DEV_ID
#endif

#if(PLATFORM_SLAVE_3)
#define DEVICE_NAME		"Salve 3: "
#define DEVICE_ID		PHN_SLAVE_3_DEV_ID
#endif

#if(PLATFORM_SLAVE_4)
#define DEVICE_NAME		"Salve 4: "
#define DEVICE_ID		PHN_SLAVE_4_DEV_ID
#endif

#define FIRMWARE_VER	DEVICE_NAME "("STRINGIZE(DEVICE_ID)") " STRINGIZE(FIRMWARE_VERSION_MAJOR) "." STRINGIZE(FIRMWARE_VERSION_MINOR) "." STRINGIZE(FIRMWARE_REVISION)


#define PHN_SLAVE_TARGET_UPDATE		500

#define PHN_MAST_REQ_TIME			50

#ifdef __cplusplus
}
#endif

#endif /*__PHN_COMPILE_H */
