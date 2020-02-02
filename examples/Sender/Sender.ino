/*
  ============================================================================

  Name :       Sender.ino
  Author :     ChrisMicro
  Version :
  Copyright :  GPL license 3

  Date :       Janurary 2014-February2020

  Description : Data transmission by a LED.

  To suppress ambient light differential FSK coding is used.

  Supported Microcontrollers:
  Attiny13
  Atmega88, Atmega168, Atmega328

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

uint8_t Data[] = { 0, 0xff, 0, 0xaa, 0x55, 0, 1, 2, 3, 4, 0x80 };

void loop()
{

  ledOff();
  delay(50);

  LedCom.send(Data, sizeof(Data));
  Data[2]++; // just to test: increment third byte in data stream

}

