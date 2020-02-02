#include "LedDataTransmission.h"

LedDataTransmission::LedDataTransmission(void)
{
  
}

void LedDataTransmission::beginReceiver()
{
  initLed();
  initPort();
  adc_init();

  // Timer 2 normal mode, clk/8, count up from 0 to 255
  // ==> timer overflow frequency @16MHz= 16MHz/8/256=7812.5Hz
  // = 128us
  //TCCR2B= _BV(CS21);
  // Timer 2 normal mode, clk/64, count up from 0 to 255
  // ==> timer overflow frequency @16MHz= 16MHz/64/256=970,56Hz
  // = 1024us

  TCCR2B = _BV(CS22);
}

#ifdef RECEIVER
#else
LedDataTransmission::LedDataTransmission(void)
{
  
}

void LedDataTransmission::beginSender(void)
{
  initLed();
}

void LedDataTransmission::send(uint8_t *data, uint8_t datalen)
{
  do
  {
    delayMicroseconds(BITTIME_US / 2);
  } while ( sendFrame_S( data, datalen) != FRAMEREADY );
}
#endif


