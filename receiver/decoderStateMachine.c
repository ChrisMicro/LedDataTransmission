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
/***************************************************************************************

        highBitReceived_S()



***************************************************************************************/
//enum bitState { BITREADY,BITSTATE1,BITSTATE2 };
uint8_t BitValue=true;

enum bitState highBitReceived_S()
{
        static uint8_t p,t,tolerance;

        static enum bitState state;

        switch(state)
        {
        	case BITREADY:{
        		state=BITSTATE1;  					//==> BITSTATE1
        	}// attention: fallthru !
        	case BITSTATE1:{
        		BitValue=false;
                if(HighTakesLonger)
                {
                	// wait for high
                	if(PINHIGH)
                	{
                        t=TIMER;
                        TIMER=0; // reset timer
                        p=PINVALUE;
                		state=BITSTATE2; 	        //==> BITSTATE2

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
                		state=BITSTATE2;            //==> BITSTATE2
                	}
                }
        	}break;
        	case BITSTATE2:{
                if(p!=PINVALUE)                // wait for edge
                {
                	t=TIMER;
                    TIMER=0; // reset timer
                    p=PINVALUE;
                    //SystemOutDec("t: ",t);
                    if(HighTakesLonger)
                    {
                            tolerance=(BitTimeHigh>>2);
                            if(t>(BitTimeHigh+tolerance)) BitValue=true;

                    }else
                    {
                            tolerance=(BitTimeLow>>2);
                            if(t>(BitTimeLow+tolerance)) BitValue=true;
                    }
                    state=BITREADY;                 //==> BITSTATE2
                }
        	}break;
        	default: state=BITREADY;
        }
        return state;
}

