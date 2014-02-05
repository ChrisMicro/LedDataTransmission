/*
============================================================================
Name :        mc_io.c
Author :      ChrisMicro
Version :
Copyright :   GPL lizense 3


Description : micontroller in/output abstraction
============================================================================
*/
#include "platform.h"
#include "mc_io.h"


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

	#define initLed() { DDRB  = ( 1 << DEBUGLED );} // set the led pin to output

	#define ledOn() {PORTB |= (1 << DEBUGLED );}
	#define ledOff() {PORTB &=~ (1 << DEBUGLED );}

	#define toggleLed() {PORTB ^= (1 << DEBUGLED );} // toggle led pin by xor

	#define delay(x)	_delay_ms(x)
	#define delayMicroseconds(x) _delay_us(x)

#else



#endif
