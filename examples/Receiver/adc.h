/*
============================================================================
Name :        adc.h
Author :      ChrisMicro
Version :
Copyright :   GPL lizense 3


Description : analog to digital converter routines

============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __ADC__
#define __ADC__

	#include "stdint.h"
	#include <avr/io.h>

	void adc_init(void);
	uint16_t adc_read(uint8_t);
	uint8_t DCremovedPinValue(uint8_t);

#endif // __ADC__

#ifdef __cplusplus
}
#endif
