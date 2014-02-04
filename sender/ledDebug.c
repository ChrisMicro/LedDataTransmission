/*
============================================================================

Name :       ledDebug.c
Author :     ChrisMicro
Version :
Copyright :  GPL license 3

Date :       Janurary 2014

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
#include "mc_io.h"
#include "senderSTM.h"
#include "../common/crc8.h"

void setup()
{
	initLed();
}

uint8_t Data[]={ 0,0xff,0,0xaa,0x55,0,1,2,3,4,0x80 };

// this routine is just to show how to call the
// data transmitter state machine
void sendWithStateMachine(uint8_t *data, uint8_t datalen)
{
	do
	{
		delayMicroseconds(BITTIME_US/2);
	}while( sendFrame_S( data, datalen) != FRAMEREADY );
}
// add crc to frame
void sendFrame(uint8_t *data, uint8_t datalen)
{
	data[0]=0;
	data[0]=crc8(data,datalen);
	sendWithStateMachine(data, datalen);
}
void loop()
{

    ledOff();
    delay(50);

    //sendDataFrame(Data,sizeof(Data));
    //sendWithStateMachine(Data,sizeof(Data));
    sendFrame(Data,sizeof(Data));
    Data[2]++; // just to test: increment third byte in data stream

    uint8_t n;

    if(Data[2]==0x80)
    {
      for(n=0;n<4*10;n++)
      {
        Data[2]=1<<(n&0x3);
        //sendDataFrame(Data,sizeof(Data));
        //sendWithStateMachine(Data,sizeof(Data));
        sendFrame(Data,sizeof(Data));
        delay(50);
      }
      Data[2]=0;
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
