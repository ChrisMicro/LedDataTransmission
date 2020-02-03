/*
  ============================================================================

  Name :       Sender.ino
  Author :     ChrisMicro
  Version :
  Copyright :  GPL license 3

  Date :       Janurary 2014-February2020

  Description : Data transmission by a LED.


  connect an ultrabright red led to analog pin A0 in series with a resistor of 270Ohm

  schematic:

  Arduino A0 ==> + Led - ==> R270 OHM ==> GND

  ============================================================================

*/

#include "LedDataTransmission.h"

LedDataTransmission LedCom;

void setup()
{
  LedCom.begin();
}

int Counter = 0;

void loop()
{
  LedCom.send("hello");
  ledOff();
  delay(50);

  for(int n=0;n<10;n++)
  {
    LedCom.send(Counter++);
    ledOff();
    delay(50);
  }
}

