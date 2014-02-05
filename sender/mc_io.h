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

	#ifdef ATTINY_13
		#define DEBUGLED 4
	#else
		#define DEBUGLED 5
	#endif

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

		//#define initLed() { DDRB  = ( 1 << DEBUGLED );} // set the led pin to output

		//#define ledOn() {PORTB |= (1 << DEBUGLED );}
		//#define ledOff() {PORTB &=~ (1 << DEBUGLED );}
		#define ledOn() {PORTC |= 0x01;}
		#define ledOff() {PORTC &=~0x01;}

		#define setLedAsOutput() {DDRC|=0x01;}
		#define setLedAsInput() {DDRC&=~0x01;}

		#define toggleLed() {PORTB ^= (1 << DEBUGLED );} // toggle led pin by xor

		#define delay(x)	_delay_ms(x)
		#define delayMicroseconds(x) _delay_us(x)

	#else

		#define ARDUINOLED 13
		#define HALFBITDELAY delayMicroseconds(1e6/BAUD/2)

		inline void ledOn(){digitalWrite(ARDUINOLED, HIGH);}
		inline void ledOff()  {digitalWrite(ARDUINOLED, LOW);}
		inline void initLed(){pinMode(ARDUINOLED, OUTPUT);}
		void toggleLed()
		{
			static int state=0;
			if(state==0) digitalWrite(ARDUINOLED, LOW);
			else digitalWrite(ARDUINOLED, HIGH);
			state^=0x01;
		}

	#endif

#endif // __MC_IO__

#ifdef __cplusplus
}
#endif
