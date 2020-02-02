#ifndef LedDataTransmission_h /* Prevent loading library twice */
#define LedDataTransmission_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h" /* This is where the standard Arduino code lies */
#endif

/*
  supported microcontrollers by C-driver:
  Attiny13
  Atmega88, Atmega168, Atmega328
*/

//#include "stdint.h"
#include "platform.h"
#include "signal.h"
#include "mc_io.h"
#include "senderSTM.h"

class LedDataTransmission
{
  public:
    /* Constructor */
    LedDataTransmission(void);
    void beginSender(void);
    void send(uint8_t *data, uint8_t datalen); // maximum lenght is defined in FRAMESIZE
    void send(String str);
    void send(int value);

};

#endif //LedDataTransmission_h
