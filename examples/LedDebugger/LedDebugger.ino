/*

  ============================================================================

  Name :          LedDebugger.ino
  Author :        ChrisMicro
  Version :
  Copyright :     GPL license 3
                  ( chris (at) roboterclub-freiburg.de )
  Date : Janurary 2014-February2020

  Description :   debug errors of led data transmission

  ============================================================================


  Platform: Arduino Uno or Arduino Nano
  connect a ultrabright red LED to A0

  schematic:

  GND ==> - LED + ==> Arduino A0

*/

#include "LedDataTransmission.h"

LedDataTransmission LedCom;



void setup()
{
  Serial.begin(115200);
  Serial.println("LED data transmission receiver");

  LedCom.begin();

  Serial.println("waitn for data .... timer counts for low and high signal");

}

uint8_t FrameData[FRAMESIZE];// framesize is 10 bytes

void loop()
{
  uint8_t n;

  while(!LedCom.receiveBytes(FrameData,10));
    
  for (n = 0; n < FRAMESIZE; n++)
  {
    Serial.print(FrameData[n], HEX); Serial.print(",");
  }
  Serial.print("  ");

  for (n = 0; n < FRAMESIZE; n++)
  {
    char c = FrameData[n];
    if (c >= ' ' && c <= '~') Serial.write(c);
    else Serial.write(".");
  }

  Serial.println("");

}


