/*
============================================================================

Name : signal.c
Author : ChrisMicro
Version :
Copyright : GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : Data transmission by a LED.

============================================================================
*/
#include "signal.h"

#define BITTIME_US 1e6/BAUD

uint8_t Data[]={ 0,0xff,0,0xaa,0x55,0,1,2,3,4,0x80 };

#define PREAMPLE_LENGTH 50 // bits

void sendBit(uint8_t bitval)
{
	if(bitval==0)
	{
		toggleLed();
		delayMicroseconds(BITTIME_US/2);
		toggleLed();
		delayMicroseconds(BITTIME_US/2);
	}else
	{
		toggleLed();
		delayMicroseconds(BITTIME_US);
		toggleLed();
		delayMicroseconds(BITTIME_US);
	}
}

void sendDataFSK()
{
	uint8_t n,k,value;
	// send preample
	// the preample consists of 0 bits
	for(k=0;k<PREAMPLE_LENGTH;k++)
	{
		sendBit(0);
	}
	// send data
	for(k=0;k<(uint8_t)sizeof(Data);k++)
	{

		// send start bit
		sendBit(1);

		value=Data[k];

		// send 8 bits
		for(n=0;n<8;n++)
		{
			if((value&0x80)==0) sendBit(0);
			else 				sendBit(1);

			value=value<<1; // shift to next bit
		}
	}
}

// not longer used:
// differential manchester coding
void sendDataDifferentialManchester()
{
	uint8_t n,k,value;
	// send Preample
	for(k=0;k<PREAMPLE_LENGTH;k++)
	{
		// send 0 Bits
		toggleLed();
		HALFBITDELAY;
		toggleLed();
		HALFBITDELAY;
	}
	// send data
	for(k=0;k<(uint8_t)sizeof(Data);k++)
	{

		// send start bit
		HALFBITDELAY;
		toggleLed();
		HALFBITDELAY;

		value=Data[k];
		// send 8 bits
		for(n=0;n<8;n++)
		{
			if((value&0x80)==0) toggleLed();
			HALFBITDELAY;
			toggleLed();
			HALFBITDELAY;

			value=value<<1; // shift to next bit
		}
	}
}



/*
#ifdef ATMEGA_88_168_328
void soundData()
{
	uint8_t n,k,value;
	for(k=0;k<(uint8_t)sizeof(Data);k++)
	{
		value=Data[k];
		for(n=0;n<8;n++)
		{
			if((value&0x80)==0) _delay_us(500);
			_delay_us(500);
			ledOn();
			_delay_us(10);
			ledOff();

			value=value<<1; // shift to next bit
		}
	}
}
#endif
*/
