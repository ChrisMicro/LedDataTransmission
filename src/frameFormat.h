/*
============================================================================

Name :      frameFormat.h
Author :    ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date :      February 2014

Description : frame format definition

============================================================================
*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __FRAME_FORMAT__
#define __FRAME_FORMAT__

     #define BAUD 2500
     #define BITTIME_US 1e6/BAUD

     // sender frame start:
    #define PREAMPLELENGTH 3 // length of preample in bytes

    // receiver start of frame detection:
	#define SYNCCOUNTER 12 // minimum number of bits which should have equal duration

    #define FRAMESIZE 11 // frame size in bytes
	#define CRC8INDEX 0  // location of crc in frame
    #define DATASTART 1	// index of first data byte

#endif // __FRAME_FORMAT__

#ifdef __cplusplus
}
#endif


