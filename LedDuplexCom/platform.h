/*
============================================================================

Name : platform.h
Author : ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : platform specification

============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __PLATFORM__
#define __PLATFORM__

	//#define ATMEGA_88_168_328
	#define ARDUINO
    #define ECLIPSE  // if developing in eclipse some additional includes are necessara
                     // for Arduino

#endif // __PLATFORM__

#ifdef __cplusplus
}
#endif
