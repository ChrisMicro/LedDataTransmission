/*
============================================================================

Name :      decoderStateMachine.c
Author :    ChrisMicro
Version :
Copyright : GPL license 3

Date :      Janurary 2014

Description : data decoder for FSK realized as state machine

============================================================================
*/
/*
	Supported Microcontrollers:
	Atmega88, Atmega168, Atmega328

//	uses counter TCC2

 */
#include "platform.h"
#include "decoder.h"
#include "stdint.h"

/***************************************************************************************

	state_t BrEstimationStateMachine(command_t command)

    Detect preample and measure high and low signal durations with this state machine

	input: command
	1. start the statemachine with (BrEstimationStateMachine(START)
	2. cylic call BrEstimationStateMachine(VOID) until result state is M1_READY

	output: 	state ( M1_READY when preample detected

	global variables output
				uint8_t BitTimeLow
				uint8_t BitTimeHigh
				uint8_t HighTakesLonger

***************************************************************************************/
#define SYNCCOUNTER 12 // minimum number of bits which should have equal duration

#define VOID 0
#define START 1

// state machine 1 state definitons
#define M1_READY   0
#define M1_PINLOW  1	// pin low state from machine 1
#define M1_PINHIGH 2	// pin high state from machine 1

typedef uint8_t state_t;
typedef uint8_t command_t;

state_t BrEstimationStateMachine(command_t command)
{
	static state_t state=M1_READY;

	static uint16_t counter;

	uint8_t p,t_old=0,tolerance;

	counter=0;

	switch(state)
	{
		case M1_READY:{
			if(command==START)
			{
 				counter=0;
				// we don't know the pin state but
 				// we assume it is low to start the state machine
				state=M1_PINLOW;						// ======> switch to low state
			}
		}break;

		case M1_PINLOW:{
			if(PINHIGH)
			{
				BitTimeLow=TIMER;
				TIMER=0; // reset timer
				p=PINVALUE;

				state=M1_PINHIGH; 						// ======> switch to high state
			}
		}break;

		case M1_PINHIGH:{
			if(PINLOW)
			{
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
				if (counter>=SYNCCOUNTER)
				{
					state=M1_READY; 					// ======> all sync bits received

					HighTakesLonger=false;
					if(BitTimeHigh>BitTimeLow)HighTakesLonger=true;
				}
				else state=M1_PINLOW;					 // ======> switch to low state
			}
		}break;
	}
	return state;
}
/*
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
*/

