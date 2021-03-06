/*
  ============================================================================

  Name :          Receiver.ino
  Author :        ChrisMicro
  Version :
  Copyright :     GPL license 3
                  ( chris (at) roboterclub-freiburg.de )
  Date : Janurary 2014-February2020

  Description :   data reception with led

  ============================================================================

  Platform: Arduino Uno or Arduino Nano
  connect a ultrabright red LED in series with a resistor to A0

  schematic:

  GND ==> - LED + ==> R270Ohm => Arduino A0

*/

#include "LedDataTransmission.h"

LedDataTransmission LedCom;

void setup()
{
  Serial.begin(115200);


  LedCom.begin();

  Serial.println("waiting for data ....");
}

uint8_t FrameData[FRAMESIZE];// framesize is 10 bytes

void loop()
{
  uint8_t n;

  while(!LedCom.receiveBytes(FrameData,10));
    

  for (n = 0; n < FRAMESIZE; n++)
  {
    char c = FrameData[n];
    if (c >= ' ' && c <= '~') Serial.write(c);
    else Serial.write(".");
  }

  Serial.println("");

}


