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
	 
#define	FIRMWARE_VERSION_MAJOR	        2
#define FIRMWARE_VERSION_MINOR	        0
#define FIRMWARE_REVISION               2
	 
#define PHN_NB_SALVE					18	 
	 
	 
#define PHN_MASTER_DEV_ID				0xA0
	 
#define PHN_SLAVE_01_DEV_ID				0xA1
#define PHN_SLAVE_02_DEV_ID				0xA2
#define PHN_SLAVE_03_DEV_ID				0xA3
#define PHN_SLAVE_04_DEV_ID				0xA4
#define PHN_SLAVE_05_DEV_ID				0xA5
#define PHN_SLAVE_06_DEV_ID				0xA6

#define PHN_SLAVE_07_DEV_ID				0xB1
#define PHN_SLAVE_08_DEV_ID				0xB2
#define PHN_SLAVE_09_DEV_ID				0xB3
#define PHN_SLAVE_10_DEV_ID				0xB4
#define PHN_SLAVE_11_DEV_ID				0xB5
#define PHN_SLAVE_12_DEV_ID				0xB6

#define PHN_SLAVE_13_DEV_ID				0xC1
#define PHN_SLAVE_14_DEV_ID				0xC2
#define PHN_SLAVE_15_DEV_ID				0xC3
#define PHN_SLAVE_16_DEV_ID				0xC4
#define PHN_SLAVE_17_DEV_ID				0xC5
#define PHN_SLAVE_18_DEV_ID				0xC6


#define PHN_INTERUPT_MODE	0

#if(PHN_INTERUPT_MODE)
#define DEVICE_MODE		" (R) "
#else
#define DEVICE_MODE		" (F) "
#endif

	 
#define PLATFORM_MASTER 				0
#define PLATFORM_SLAVE_01 				0
#define PLATFORM_SLAVE_02 				0
#define PLATFORM_SLAVE_03 				0
#define PLATFORM_SLAVE_04 				0
#define PLATFORM_SLAVE_05 				0
#define PLATFORM_SLAVE_06 				0
#define PLATFORM_SLAVE_07 				0
#define PLATFORM_SLAVE_08 				0
#define PLATFORM_SLAVE_09 				0
#define PLATFORM_SLAVE_10 				0
#define PLATFORM_SLAVE_11 				0
#define PLATFORM_SLAVE_12 				0
#define PLATFORM_SLAVE_13 				0
#define PLATFORM_SLAVE_14 				0
#define PLATFORM_SLAVE_15 				0
#define PLATFORM_SLAVE_16 				0
#define PLATFORM_SLAVE_17 				0
#define PLATFORM_SLAVE_18 				1

#if (	PLATFORM_MASTER 	+ \
		PLATFORM_SLAVE_01 	+ \
		PLATFORM_SLAVE_02 	+ \
		PLATFORM_SLAVE_03 	+ \
		PLATFORM_SLAVE_04 	+ \
		PLATFORM_SLAVE_05 	+ \
		PLATFORM_SLAVE_06 	+ \
		PLATFORM_SLAVE_07 	+ \
		PLATFORM_SLAVE_08 	+ \
		PLATFORM_SLAVE_09 	+ \
		PLATFORM_SLAVE_10 	+ \
		PLATFORM_SLAVE_11 	+ \
		PLATFORM_SLAVE_12 	+ \
		PLATFORM_SLAVE_13 	+ \
		PLATFORM_SLAVE_14 	+ \
		PLATFORM_SLAVE_15 	+ \
		PLATFORM_SLAVE_16 	+ \
		PLATFORM_SLAVE_17 	+ \
		PLATFORM_SLAVE_18  	!= 1 )
#error "Only support one platform at a time !"
#endif

#if(PLATFORM_MASTER)
#define PHN_MASTER_PLATFORM		1
#else
#define PHN_MASTER_PLATFORM		0
#endif

#if (	PLATFORM_SLAVE_01 	+ \
		PLATFORM_SLAVE_02 	+ \
		PLATFORM_SLAVE_03 	+ \
		PLATFORM_SLAVE_04 	+ \
		PLATFORM_SLAVE_05 	+ \
		PLATFORM_SLAVE_06 	+ \
		PLATFORM_SLAVE_07 	+ \
		PLATFORM_SLAVE_08 	+ \
		PLATFORM_SLAVE_09 	+ \
		PLATFORM_SLAVE_10 	+ \
		PLATFORM_SLAVE_11 	+ \
		PLATFORM_SLAVE_12 	+ \
		PLATFORM_SLAVE_13 	+ \
		PLATFORM_SLAVE_14 	+ \
		PLATFORM_SLAVE_15 	+ \
		PLATFORM_SLAVE_16 	+ \
		PLATFORM_SLAVE_17 	+ \
		PLATFORM_SLAVE_18  	== 1 )
#define PHN_SLAVE_PLATFORM		1
#else
#define PHN_SLAVE_PLATFORM		0
#endif

#if(PLATFORM_MASTER)
#define DEVICE_NAME		"Master: "
#define DEVICE_ID		PHN_MASTER_DEV_ID
#endif


#if(PLATFORM_SLAVE_01)
#define DEVICE_NAME		"Salve 01: "
#define DEVICE_ID		PHN_SLAVE_01_DEV_ID
#endif

#if(PLATFORM_SLAVE_02)
#define DEVICE_NAME		"Salve 02: "
#define DEVICE_ID		PHN_SLAVE_02_DEV_ID
#endif

#if(PLATFORM_SLAVE_03)
#define DEVICE_NAME		"Salve 03: "
#define DEVICE_ID		PHN_SLAVE_03_DEV_ID
#endif

#if(PLATFORM_SLAVE_04)
#define DEVICE_NAME		"Salve 04: "
#define DEVICE_ID		PHN_SLAVE_04_DEV_ID
#endif

#if(PLATFORM_SLAVE_05)
#define DEVICE_NAME		"Salve 05: "
#define DEVICE_ID		PHN_SLAVE_05_DEV_ID
#endif

#if(PLATFORM_SLAVE_06)
#define DEVICE_NAME		"Salve 06: "
#define DEVICE_ID		PHN_SLAVE_06_DEV_ID
#endif

#if(PLATFORM_SLAVE_07)
#define DEVICE_NAME		"Salve 07: "
#define DEVICE_ID		PHN_SLAVE_07_DEV_ID
#endif

#if(PLATFORM_SLAVE_08)
#define DEVICE_NAME		"Salve 08: "
#define DEVICE_ID		PHN_SLAVE_08_DEV_ID
#endif

#if(PLATFORM_SLAVE_09)
#define DEVICE_NAME		"Salve 09: "
#define DEVICE_ID		PHN_SLAVE_09_DEV_ID
#endif


#if(PLATFORM_SLAVE_10)
#define DEVICE_NAME		"Salve 10: "
#define DEVICE_ID		PHN_SLAVE_10_DEV_ID
#endif

#if(PLATFORM_SLAVE_11)
#define DEVICE_NAME		"Salve 11: "
#define DEVICE_ID		PHN_SLAVE_11_DEV_ID
#endif

#if(PLATFORM_SLAVE_12)
#define DEVICE_NAME		"Salve 12: "
#define DEVICE_ID		PHN_SLAVE_12_DEV_ID
#endif

#if(PLATFORM_SLAVE_13)
#define DEVICE_NAME		"Salve 13: "
#define DEVICE_ID		PHN_SLAVE_13_DEV_ID
#endif

#if(PLATFORM_SLAVE_14)
#define DEVICE_NAME		"Salve 14: "
#define DEVICE_ID		PHN_SLAVE_14_DEV_ID
#endif

#if(PLATFORM_SLAVE_15)
#define DEVICE_NAME		"Salve 15: "
#define DEVICE_ID		PHN_SLAVE_15_DEV_ID
#endif

#if(PLATFORM_SLAVE_16)
#define DEVICE_NAME		"Salve 16: "
#define DEVICE_ID		PHN_SLAVE_16_DEV_ID
#endif

#if(PLATFORM_SLAVE_17)
#define DEVICE_NAME		"Salve 17: "
#define DEVICE_ID		PHN_SLAVE_17_DEV_ID
#endif

#if(PLATFORM_SLAVE_18)
#define DEVICE_NAME		"Salve 18: "
#define DEVICE_ID		PHN_SLAVE_18_DEV_ID
#endif

#define FIRMWARE_VER	DEVICE_NAME DEVICE_MODE "("STRINGIZE(DEVICE_ID)") " STRINGIZE(FIRMWARE_VERSION_MAJOR) "." STRINGIZE(FIRMWARE_VERSION_MINOR) "." STRINGIZE(FIRMWARE_REVISION)


#define PHN_SLAVE_TARGET_UPDATE		500

#define PHN_MAST_REQ_TIME			50

#ifdef __cplusplus
}
#endif

#endif /*__PHN_COMPILE_H */
