/*
============================================================================

Name : signal.h
Author : ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : Data transmission by a LED.

============================================================================
*/
/*
	Supported Microcontrollers:
	Attiny13
	Atmega88, Atmega168, Atmega328
 */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __SIGNAL__
#define __SIGNAL__

	#include "platform.h"

	#define BAUD 4800

	//#define DEBUGLED 5
	#define DEBUGLED 4

	#ifdef ATMEGA_88_168_328
	/*
		#ifdef ATTINY13
			//#define F_CPU 16000000
		#else
			#define F_CPU 9600000
		#endif
	*/

		#include <avr/io.h>
		#include <util/delay.h>

		#define HALFBITDELAY _delay_us(1e6/BAUD/2)

		#define ledOn() {PORTB |= (1 << DEBUGLED );}
		#define ledOff() {PORTB &=~ (1 << DEBUGLED );}
		#define toggleLed() {PORTB ^= (1 << DEBUGLED );} // toggle led pin by xor
		#define initLed() { DDRB  = ( 1 << DEBUGLED );} // set the led pin to output

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

	extern uint8_t Data[];

	void sendDataFSK();


#endif // __MC_IO__

#ifdef __cplusplus
}
#endif
