/*
============================================================================

Name :        signal.c
Author :      ChrisMicro
Version :
Copyright :   GPL license 3

Date :        Janurary 23, 2014

Description : data transmission signal generator

============================================================================
*/
#include "signal.h"
#include "mc_io.h"

/***************************************************************************************

	void sendFskBit(uint8_t bitval)

    Send a FSK coded bit.
    ( for FSK see http://en.wikipedia.org/wiki/Frequency-shift_keying )

    input: bit to be send

***************************************************************************************/
#define BITTIME_US 1e6/BAUD

void sendFskBit(uint8_t bitval)
{
	if(bitval==0)
	{
		toggleLed();
		delayMicroseconds(BITTIME_US/2);
		toggleLed();
		delayMicroseconds(BITTIME_US/2);
		// remark: Common FSK would send 2 oscillations
		// for the low bit. For speed reason we use only one.
	}else
	{
		toggleLed();
		delayMicroseconds(BITTIME_US);
		toggleLed();
		delayMicroseconds(BITTIME_US);
	}
}
/***************************************************************************************

	void sendDataFrame(uint8_t *data, uint8_t dataLength)

    Send data with leading preample. The preample is used for synchronization
    by the receiver.

    input: 	*data
    		dataLenth

***************************************************************************************/

#define sendBit(x) sendFskBit(x)

#define PREAMPLE_LENGTH 50 // bits

void sendDataFrame(uint8_t *data, uint8_t dataLength)
{
	uint8_t n,k,value;

	// ******** preample *********************

	for(k=0;k<PREAMPLE_LENGTH;k++)
	{
		sendBit(0); // the preample consists of low bits
	}

	// ******** data *************************
	for(k=0;k<dataLength;k++)
	{
		// send start bit
		sendBit(1);

		value=data[k];

		// send 8 bits
		for(n=0;n<8;n++)
		{
			if((value&0x80)==0) sendBit(0);
			else 				sendBit(1);

			value=value<<1; // shift to next bit
		}
	}

}

