/*
============================================================================

Name : signal.h
Author : ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : Data encoder definition

============================================================================
*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __FRAME_FORMAT__
#define __FRAME_FORMAT__

	#define BAUD 1400
	#define BITTIME_US 1e6/BAUD

	#define FRAMESIZE 11 // frame size in bytes
	#define CRC8INDEX 0  // location of crc in frame
    #define DATASTART 1	// index of first data byte

#endif // __FRAME_FORMAT__

#ifdef __cplusplus
}
#endif


