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
#include "frameFormat.h"


#include "mc_io.h"

#include "decoder.h"
#include "stdint.h"
#include "decoderStateMachine.h"



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

state_t BrEstimationStateMachine(command_t command)
{
	static state_t state=M1_READY;
	static uint16_t counter=0,t_old=0;
	uint8_t p,tolerance;

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
/***************************************************************************************

   highBitReceived_S()

   output:  state ( BITREADY when bit received )

***************************************************************************************/
//enum bitState { BITREADY,BITSTATE1,BITSTATE2 };
//enum bitError { BITOK=0,TIMEOUT=1,TOSHORT };

uint8_t BitValue=0;
uint8_t BitError=BITOK;
#define TIMEOUTCYCLES 10000

enum bitState highBitReceived_S()
{
        static uint8_t p,t,tolerance;

        static enum bitState state;
        static uint16_t timeOutCounter;

        switch(state)
        {
        	case  BITREADY:{
        		timeOutCounter=0;
        		state=BITSTART;  					//==> BITSTATE1
        	}// attention: fallthru !
        	case BITSTART:{
        		BitValue=0;
        		//BitError=BITOK;

                if(HighTakesLonger)
                {
                	// wait for high
                	if(PINHIGH)
                	{
                        t=TIMER;
                        TIMER=0; // reset timer
                        p=PINVALUE;
                		state=BITEND; 	        //==> BITSTATE2
                	}
                }
                else
                {
                    // wait for low
                	if(PINLOW)
                	{
                		t=TIMER;
                		TIMER=0; // reset timer
                		p=PINVALUE;
                		state=BITEND;            //==> BITSTATE2
                	}
                }
        	}break;
        	case BITEND:{
                if(p!=PINVALUE) // wait for edge
                {
                	t=TIMER;
                    TIMER=0; // reset timer
                    p=PINVALUE;

                    if(HighTakesLonger)
                    {
                            tolerance=(BitTimeHigh>>2);
                            if(t>(BitTimeHigh+tolerance)) BitValue=1;
                            if(t< ( BitTimeHigh>>1) ) BitError=TOSHORT;//SystemOutDec("to short",t);
                    }else
                    {
                            tolerance=(BitTimeLow>>2);
                            if(t>(BitTimeLow+tolerance)) BitValue=1;
                            if(t< ( BitTimeLow>>1) ) BitError=TOSHORT; // SystemOutDec("to short",t);
                    }
                    state= BITREADY;                 //==>  BITREADY
                }
        	}break;
        	default: state= BITREADY;
        }
        timeOutCounter++;
        if( timeOutCounter > TIMEOUTCYCLES)
        {
        	BitError=BITTIMEOUT;
        	state= BITREADY;
        	//SystemOutDec("----BITTIMEOUT ",t);
        }

        return state;
}
/***************************************************************************************

    receiveByte_S()

    output:  state ( BYTEREADY when byte received )

***************************************************************************************/

uint8_t ReceiverData;

#define NUMBEROFBITS 8

uint8_t receiveByte_S(){

	static uint8_t state=BYTEREADY;
	static uint8_t bitCounter,dat;

	switch(state)
	{
		case BYTEREADY:
		{
           state=WAITFORSTARTBIT;                          // ==> WAITFORSTARTBIT
           bitCounter=0;
		} // fall through

		case WAITFORSTARTBIT:
		{
			if(highBitReceived_S()==BITREADY)
			{
			   if(BitValue==1) state=RECEIVEBITS;          // ==>RECEIVEBITS
			}
		}break;

		case RECEIVEBITS:
		{
			if(highBitReceived_S()==BITREADY)
			{
				dat=dat<<1;
				dat=dat|BitValue;
				bitCounter++;
				if(bitCounter==NUMBEROFBITS)
				{
					state=BYTEREADY;     // ==> RECEIVERREADY
					ReceiverData=dat;
				}
				debugLedOn();
				debugLedOff();
			}
		}break;
	}
	return state;
}
/***************************************************************************************

    receiveFrame_S()

    output:  state ( RECEIVERREADY when frame received )

***************************************************************************************/

uint8_t FrameData[FRAMESIZE];
uint8_t FrameError;

uint8_t receiveFrame_S()
{

	static uint8_t state=RECEIVERREADY;
	static uint8_t byteCounter;
	static uint16_t timeOutCounter;

	switch(state)
	{
		case RECEIVERREADY:
		{
           state=BITRATEESTIMATION;                          // ==> WAITFORSTARTBIT
           byteCounter=0;
           timeOutCounter=0;
           FrameError=FRAMEOK;
		} // fall through

		case BITRATEESTIMATION:
		{
			if(BrEstimationStateMachine(START)==M1_READY)
			{
				timeOutCounter=0;
				state=RECEIVEBYTES;          // ==>RECEIVEBITS
			}
		}break;

		case RECEIVEBYTES:
		{
			if(receiveByte_S()==BYTEREADY)
			{
				FrameData[byteCounter]=ReceiverData;
				byteCounter++;
				if(byteCounter==FRAMESIZE)
				{
					state=RECEIVERREADY;     // ==> RECEIVERREADY
				}
			}
		}break;
	}
    timeOutCounter++;
    if( timeOutCounter > TIMEOUTCYCLES)
    {
    	FrameError=FRAMETIMEOUT;
    	state=RECEIVERREADY;;
    }
	return state;
}






