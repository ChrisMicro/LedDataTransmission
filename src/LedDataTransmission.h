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

#include "platform.h"
#include "stdlib.h"

#include "decoder.h"
#include "adc.h"
#include "filter.h"

#include "mc_io.h"
#include "crc8.h"

#include "decoderStateMachine.h"

#include "senderSTM.h"

#include "frameFormat.h"

extern "C"
{
  void debugLedOn();
 
  void debugLedOff();

  void initDebugLed();


  void setLedAsOutput();

  void setLedAsInput();


  void ledOn();

  void ledOff();


  void toggleLed();

  void SystemOutText(char * str);


  void SystemOutDec(char* text, int value);
}
  
class LedDataTransmission 
{
  public:
    /* Constructor */
    LedDataTransmission(void);

    void begin(void);
    void send(uint8_t *data, uint8_t datalen); // maximum lenght is defined in FRAMESIZE
    void send(String str);
    void send(int value);

    bool receiveBytes( uint8_t *data, uint8_t datalen );

};



#endif //LedDataTransmission_h
