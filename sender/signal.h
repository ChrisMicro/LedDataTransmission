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

#ifndef __SIGNAL__
#define __SIGNAL__

	#include "stdint.h"

	#define BAUD 4800

	void sendDataFrame(uint8_t *, uint8_t);

#endif // __MC_IO__

#ifdef __cplusplus
}
#endif
