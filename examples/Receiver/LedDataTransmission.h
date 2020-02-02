#ifndef LedDataTransmission_h /* Prevent loading library twice */
#define LedDataTransmission_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h" /* This is where the standard Arduino code lies */
#endif

/*
  From C-Driver Supported Microcontrollers:
  Attiny13
  Atmega88, Atmega168, Atmega328
*/

#define RECEIVER

//##########################################################################################
#ifdef RECEIVER

#include "platform.h"
#include "decoder.h"
#include "adc.h"
#include "filter.h"

class LedDataTransmission 
{
  public:
    /* Constructor */
    LedDataTransmission(void);
    void beginReceiver(void);
    //void send(uint8_t *data, uint8_t datalen); // maximum lenght is defined in FRAMESIZE

};

#else

//##########################################################################################
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

};
#endif

#endif //LedDataTransmission_h
