/*
============================================================================
Name :        filter.h
Author :      ChrisMicro
Version :
Copyright :   GPL lizense 3


Description : digital filter routines

============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __FILTER__
#define __FILTER__

	#include "stdint.h"

	uint16_t lowPassUint16(uint16_t );
	int16_t AC_signal(uint16_t);

#endif // __FILTER__

#ifdef __cplusplus
}
#endif
