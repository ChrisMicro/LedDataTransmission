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


#define FRAMESIZE 10

void LedDataTransmission::send(String str)
{
  uint8_t data[FRAMESIZE]={0};

  int len = str.length();
  if (len > FRAMESIZE)len = FRAMESIZE;
  for (int n = 0; n < len; n++) data[n] = str.charAt(n);

  int datalen=FRAMESIZE;
  
  do
  {
    delayMicroseconds(BITTIME_US / 2);
  } while ( sendFrame_S( data, datalen) != FRAMEREADY );
}


void LedDataTransmission::send(int value )
{
  send(String(value));
}





