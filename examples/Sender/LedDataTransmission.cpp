#include "LedDataTransmission.h"

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


