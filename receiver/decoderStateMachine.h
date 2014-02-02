
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __DECODERSTATEMACHINE__
#define __DECODERSTATEMACHINE__

	#include "stdint.h"

	#define SYNCCOUNTER 12 // minimum number of bits which should have equal duration

	#define VOID 0
	#define START 1

	// state machine 1 state definitons
	#define M1_READY   0
	#define M1_PINLOW  1	// pin low state from machine 1
	#define M1_PINHIGH 2	// pin high state from machine 1

	typedef uint8_t state_t;
	typedef uint8_t command_t;

	enum bitState { BITREADY,BITSTATE1,BITSTATE2 };
	extern uint8_t BitValue;

	state_t BrEstimationStateMachine(command_t command);
	enum bitState highBitReceived_S(void);

#endif // __DECODERSTATEMACHINE__

#ifdef __cplusplus
}
#endif
