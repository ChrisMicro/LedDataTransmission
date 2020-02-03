/*
============================================================================
Name :        senderSTM.h
Author :      ChrisMicro
Version :
Copyright :   GPL license 3

Description : sender state machine
============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __SENDERSTM__
#define __SENDERSTM__

	#include <stdlib.h>

	enum bitStates { SENDERBITREADY=0, LOW_S2, HIGH_S2,HIGH_S3,HIGH_S4 };
	enum bitStates sendBit_S(uint16_t bit);

	enum byteSenderStates { READYFORNEXTBYTE,STARTSENDING,SENDING };
	enum byteSenderStates sendByte_S(uint16_t value);
	//enum SenderStates { FRAMEREADY=0,FRAMESTART,PREAMPLE,SENDDATA,FINISHED };

	#define FRAMEREADY 0
	#define	FRAMESTART 1
	#define	PREAMPLE 2
	#define	SENDDATA 3
	#define	FINISHED 4

	uint8_t sendFrame_S(uint8_t *data, uint8_t dataLen);

#endif // __SENDERSTM__

#ifdef __cplusplus
}
#endif
