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
#define FIRMWARE_REVISION               1
	 
	 
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
#define DEVICE_ID		0x00
#endif


#if(PLATFORM_SALVE_1)
#define DEVICE_NAME		"Salve 1: "
#define DEVICE_ID		0x01
#endif

#if(PLATFORM_SALVE_2)
#define DEVICE_NAME		"Salve 2: "
#define DEVICE_ID		0x02
#endif

#if(PLATFORM_SALVE_3)
#define DEVICE_NAME		"Salve 3: "
#define DEVICE_ID		0x03
#endif

#define FIRMWARE_VER	DEVICE_NAME STRINGIZE(FIRMWARE_VERSION_MAJOR) "." STRINGIZE(FIRMWARE_VERSION_MINOR) "." STRINGIZE(FIRMWARE_REVISION)	 	 

#ifdef __cplusplus
}
#endif

#endif /*__PHN_COMPILE_H */
