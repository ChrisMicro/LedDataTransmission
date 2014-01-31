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

	enum bitStates { BITREADY=0, LOW_S2, HIGH_S2,HIGH_S3,HIGH_S4 };
	enum bitStates sendBit_S(uint16_t bit);

	enum SenderStates { FRAMEREADY,FRAMESTART,PREAMPLE,SENDDATA };
	enum SenderStates sendFrame_S(uint8_t *data, uint8_t dataLen);

#endif // __SENDERSTM__

#ifdef __cplusplus
}
#endif
