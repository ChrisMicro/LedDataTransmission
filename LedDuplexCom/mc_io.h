/*
============================================================================
Name :        mc_io.h
Author :      ChrisMicro
Version :
Copyright :   GPL lizense 3


Description : micontroller in/output abstraction
============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __MC_IO__
#define __MC_IO__

	#include "platform.h"
	#include "stdlib.h"

        #define true ( 1==1 )
        #define false ( !true )

	#ifdef ATTINY_13
		#define DEBUGLED 4
	#else
		#define DEBUGLED 5
	#endif

	void ledOn(void);
	void ledOff(void);
	void initLed(void);
	void toggleLed(void);
	void SystemOutText(char * str);
	void SystemOutDec(char* str, int);

	#if defined( ATTINY_13) || defined ( ATMEGA_88_168_328 )
	// make shure that your compiler environment sets F_CPU
	/*
		#ifdef ATTINY13
			#define F_CPU 9600000
		#else
			#define F_CPU 16000000
		#endif
	*/

		#include <avr/io.h>
		#include <util/delay.h>

		#define HALFBITDELAY _delay_us(1e6/BAUD/2)

	#else

		#define ARDUINOLED 13
		#define HALFBITDELAY delayMicroseconds(1e6/BAUD/2)

	#endif

#endif // __MC_IO__

#ifdef __cplusplus
}
#endif
