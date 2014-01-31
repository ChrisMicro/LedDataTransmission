/*
============================================================================

Name :      decoder.h
Author :    ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date :      Janurary 2014

Description : Data transmission by a LED.

============================================================================
*/
/*
	Supported Microcontrollers:
	Atmega88, Atmega168, Atmega328
 */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __DECODER_
#define __DECODER_

	// you can use the adc sitnal as input
	// this will enable DC suppression without
	// external capacitor
	// ADC-sampling is slower than direct
	// digital pin sampling
	#define INPUTFROMADC

	#define DEBUGLED 5
	//#define DEBUGLED 4

	#define BAUD 4800
	#define TIMER TCNT2 // we use timer2 for measuring time

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

	#define delay(x)	_delay_ms(x);

#else
	#include "Arduino.h"
	#include "adc.h"

	#define ARDUINOLED 13
	#define HALFBITDELAY delayMicroseconds(1e6/BAUD/2)

	inline void ledOn(){digitalWrite(ARDUINOLED, HIGH);}
	inline void ledOff()  {digitalWrite(ARDUINOLED, LOW);}
	inline void initLed(){pinMode(ARDUINOLED, OUTPUT);}

	inline void initPort(){pinMode(12, INPUT);}
	#define INPUTAUDIOPIN (1<<PB4) // PB4 is Arduino Pin Number 12

	#ifdef INPUTFROMADC
		#define PINVALUE DCremovedPinValue(0)
	#else
		#define PINVALUE (PINB&INPUTAUDIOPIN)
	#endif

	#define PINLOW (PINVALUE==0)
	#define PINHIGH (!PINLOW)

#endif

	extern uint8_t BitTimeLow;
	extern uint8_t BitTimeHigh;

	#define FRAMESIZE 10

	void toggleLed(void);
	void ledOn(void);
	void ledOff(void);
	void bitRateEstimation(void);

	uint8_t receiveFrame(uint8_t * frameData);


#endif // __DECODER_

#ifdef __cplusplus
}
#endif
