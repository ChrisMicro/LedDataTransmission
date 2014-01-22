/*
============================================================================

Name : decoder.c
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
	Atmega88, Atmega168, Atmega328

	uses counter TCC2

 */
#include "platform.h"
#include "decoder.h"
#include "stdint.h"

void toggleLed()
{
	static int state=0;
	if(state==0) ledOff();
	else ledOn();
	state^=0x01;
}

// debug pulse
void pulse()
{
	  ledOn();
	  delayMicroseconds(20);
	  ledOff();
}
uint8_t BitTimeLow=0;
uint8_t BitTimeHigh=0;

static uint8_t HighTakesLonger=0;

/***************************************************************************************

	void bitRateEstimation()

    Detect preample and measure high and low signal durations.

	output: 	global variables
				uint8_t BitTimeLow
				uint8_t BitTimeHigh
				uint8_t HighTakesLonger

***************************************************************************************/
#define SYNCCOUNTER 12 // minimum number of bits which should have equal duration

void bitRateEstimation()
{
	  uint16_t counter=0;

	  uint8_t p,t_old=0,tolerance;

	  //*** synchronisation and bit rate estimation **************************

	  counter=0;

	  while(counter<SYNCCOUNTER)
	  {

		while(PINLOW);				// wait for high
		BitTimeLow=TIMER;
		TIMER=0; // reset timer
		p=PINVALUE;


		while(p==PINVALUE);			// wait for edge
		BitTimeHigh=TIMER;
		TIMER=0; // reset timer
		p=PINVALUE;

		tolerance=BitTimeHigh>>2; // 1/4 t

		// check if t is close to t_old
		if( ( t_old>(BitTimeHigh-tolerance)) && (t_old<(BitTimeHigh+tolerance)) )
		{
			counter ++ ;
			//toggleLed(); // debug
		}
		else
		{
			counter=0;
		}
		t_old=BitTimeHigh;
	  }

	  HighTakesLonger=false;
	  if(BitTimeHigh>BitTimeLow)HighTakesLonger=true;

}
/***************************************************************************************

	highBitReceived()

    receive one bit
    Depending on the results of the bit rate estimation

    input: 	lobal variables from bit rate estimation BitTimeLow, BitTimeHigh duration
    		and if the BitTimeHigh duration takes longer than the BitTimeLow duration
    		( HighTakesLonger ), one bit is decoded

	output: 	flag=1 if bit is HIGH
				flag=0 if bit is LOW

***************************************************************************************/
uint8_t highBitReceived()
{
	static uint8_t p,t,tolerance;

	if(HighTakesLonger)	while(PINLOW);			// wait for high
	else			while(PINHIGH);				// wait for low

	t=TIMER;
	TIMER=0; // reset timer
	p=PINVALUE;


	while(p==PINVALUE);			// wait for edge
	t=TIMER;
	TIMER=0; // reset timer
	p=PINVALUE;

	if(HighTakesLonger)
	{
		tolerance=(BitTimeHigh>>2);
		if(t>(BitTimeHigh+tolerance)) return true;
	}else
	{
		tolerance=(BitTimeLow>>2);
		if(t>(BitTimeLow+tolerance)) return true;
	}
	return false;
}
/***************************************************************************************

	uint8_t receiveByte()

    receive one bit
    Depending on the results of the bit rate estimation

	output: 	uint8_t // received byte

***************************************************************************************/
uint8_t receiveByte()
{
	uint8_t n,data=0;

	while(!highBitReceived()); // wait for start bit
	for(n=0;n<8;n++)
	{
		data=data<<1;
		data|=highBitReceived();
	}
	return data;
}


/***************************************************************************************

	receiveFrame()

	 This routine receives a frame.
	 The routine waits for a toggling voltage level.
	 It automatically detects the transmission speed.

	 input:	pointer to frameData
	 output: 	*frameData
				uint8_t frame size

***************************************************************************************/

uint8_t receiveFrame(uint8_t * frameData)
{
	uint8_t n;
	bitRateEstimation();
	for(n=0;n<FRAMESIZE;n++)
	{
		*frameData=receiveByte();
		frameData++;
		toggleLed(); // debugging
	}

	return FRAMESIZE;
}

/******************************************************************************************
 *
 * 		the following routines are only used for development debugging purposes
 *
 *****************************************************************************************/
// detect an edge at the input pin and toggle the output pin
// this function is only for development issues
void edgeDebug()
{
	uint8_t p;

	p=PINVALUE;
	while(1)
	{
		  // wait for edge
		  while(p==PINVALUE);
		  p=PINVALUE;

		  toggleLed();
	}
}

#define STORESIZE 64
void syncDebug()
{

	  //uint16_t store[STORESIZE];

	  uint16_t counter=0;
	  volatile uint16_t time=0;

	  uint8_t p,t,t_old=0,temp;

	  //*** synchronisation and bit rate estimation **************************
	  time=0;

		  // wait for edge
		  p=PINVALUE;
		  while(p==PINVALUE);
		  p=PINVALUE;
		  TIMER=0; // reset timer

		  counter=0;

		  while(counter<SYNCCOUNTER)
		  {
			// wait for high
			while(PINLOW);
			t=TIMER;
			TIMER=0; // reset timer

			p=PINVALUE;

			// wait for edge
			while(p==PINVALUE);
			t=TIMER;
			TIMER=0; // reset timer
			p=PINVALUE;

			temp=t>>2; // 1/4 t
			// check if t is close to t_old
			if( ( t_old>(t-temp)) && (t_old<(t+temp)) )
			{
				counter ++ ;
				toggleLed();
			}
			else
			{
				counter=0;
				//startindex=index;
			}

			//store[index++]=t;
			//if(index>=STORESIZE)index=0;


			//if(n>=8)time+=t; // time accumulator for mean period calculation only the last 8 times are used
			t_old=t;
		  }
		  //ledOn();
		  //delayMicroseconds(100);
		  //ledOff();


	#ifdef ARDUINO_DEBUG
	  //********************** debug ********************************
	  for(n=0;n<SYNCCOUNTER ;n+=2)
	  {
		  Serial.print(store[n]);
		  Serial.print("\t ");
		  Serial.println(store[n+1]);
	  }
	#endif
}
