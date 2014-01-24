#include "platform.h"
#ifdef ARDUINO
	#include "Arduino.h"
#endif
/*
============================================================================

Name :          ledDebugReceiver.cpp
Author :        ChrisMicro
Version :
Copyright :     GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : 	Data transmission by a LED.

				To suppress ambient light FSK coding is used.
============================================================================
*/
/*
 	Platform: Arduino Uno
 */
#include "decoder.h"
#include "adc.h"
#include "filter.h"

void setup()
{
	initLed();
	initPort();
	adc_init();

	Serial.begin(9600);
    Serial.println("LED data transmission receiver");

    // Timer 2 normal mode, clk/8, count up from 0 to 255
    // ==> timer overflow frequency @16MHz= 16MHz/8/256=7812.5Hz
    // = 128us
    //TCCR2B= _BV(CS21);
    // Timer 2 normal mode, clk/64, count up from 0 to 255
    // ==> timer overflow frequency @16MHz= 16MHz/64/256=970,56Hz
    // = 1024us

    TCCR2B= _BV(CS22);
}

uint8_t FrameData[FRAMESIZE];

void loop()
{
	//analogRead(analogInPin);
	uint8_t n;
	Serial.println("--");
/*
	// ADC test
	while(1)
	{
		if(DCremovedPinValue(0)==0)ledOn();
		else ledOff();
	}
	while(1)
	{
		ledOn();
		//delay(500);
		ledOff();
		//delay(500);
		uint16_t x;
		x=adc_read(0);
		Serial.print(x);Serial.print("\t");
		//Serial.println(AC_signal(x));
		Serial.println(DCremovedPinValue(0));
	}*/
	//Serial.println("edge debug, infinite loop, signal out to led");
	//edgeDebug();
	//Serial.println("frame start");
	//receiveFrame();
	//Serial.println("syncDebug");
	//syncDebug();

	Serial.println("waitn for data .... timer counts for low and high signal");
	delay(1000);
	while(1)
	{

		receiveFrame(FrameData);
		for(n=0;n<FRAMESIZE;n++)
		{
			Serial.print(FrameData[n],HEX);
			Serial.print("\t");
		}
		Serial.print(  " low: ");	Serial.print(BitTimeLow);
		Serial.print("\t high: ");	Serial.println(BitTimeHigh);

	}
}

#ifndef ARDUINO

	int main (void)
	{
		setup();
		while(1) loop();
		return 0;
	}

#endif
