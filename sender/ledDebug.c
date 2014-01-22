/*
============================================================================

Name : ledDebug.c
Author : ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : Data transmission by a LED.

============================================================================

/*
	Led Debugger

	Data transmission by a LED.
	To suppress ambient light differential FSK coding is used.

	Supported Microcontrollers:
	Attiny13
	Atmega88, Atmega168, Atmega328

 */
#include "stdint.h"

#include "platform.h"

#include "signal.h"

void setup()
{
	initLed();
}

void loop()
{
	// blink one time
	ledOn();
	delay(100);
	ledOff();
	delay(100);

	sendDataFSK();
	Data[2]++; // just to test: increment third byte in data stream
}

#ifndef ARDUINO

	int main (void)
	{
		setup();
		while(1) loop();
		return 0;
	}

#endif
