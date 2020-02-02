/*
  ============================================================================

  Name :       Sender.ino
  Author :     ChrisMicro
  Version :
  Copyright :  GPL license 3

  Date :       Janurary 2014-February2020

  Description : Data transmission by a LED.


  connect an ultrabright red led to pin13 in series with a resistor of 270Ohm

  schematic:

  Arduino Pin13 ==> + Led - ==> R270 OHM ==> GND

  ============================================================================

*/

#include "LedDataTransmission.h"

LedDataTransmission LedCom;

void setup()
{
  LedCom.beginSender();
}

int Counter = 0;

void loop()
{

  ledOff();
  delay(50);
  LedCom.send("hello");
  delay(50);
  LedCom.send(Counter++);
  delay(1000);

}

