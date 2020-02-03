#include "LedDataTransmission.h"

extern "C"
{
  void debugLedOn() {
    digitalWrite(ARDUINOLED, HIGH);
  }
  void debugLedOff()  {
    digitalWrite(ARDUINOLED, LOW);
  }
  void initDebugLed() {
    pinMode(ARDUINOLED, OUTPUT);
  }

  void setLedAsOutput() {
    DDRC |= 0x01;
  }
  void setLedAsInput() {
    DDRC &= ~0x01;
    PORTC &= ~0x01;
  }

  void ledOn() {
    PORTC |= 0x01;
  }
  void ledOff() {
    PORTC &= ~0x01;
  }

  void toggleLed()
  {
    static int state = 0;
    if (state == 0) ledOff();
    else ledOn();
    state ^= 0x01;
  }
  void SystemOutText(char * str) {
    Serial.println(str);
  }

  void SystemOutDec(char* text, int value)
  {
    Serial.print(text); Serial.println(value);
  }
}

LedDataTransmission::LedDataTransmission(void)
{
  
}
void LedDataTransmission::begin()
{
  setLedAsInput();
  initDebugLed();

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



void LedDataTransmission::send(uint8_t *data, uint8_t datalen)
{
  setLedAsOutput();
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
  send(data,FRAMESIZE);

}


void LedDataTransmission::send(int value )
{
  send(String(value));
}

// return flag: true if OK
// false if error
bool LedDataTransmission::receiveBytes( uint8_t *data, uint8_t datalen )
{
  uint8_t len=datalen;
  if (len > FRAMESIZE) len = FRAMESIZE;
  while (receiveFrame_S() != RECEIVERREADY);
  //if (FrameError != FRAMEOK) Serial.println("Frame Timeout");

  for (int n = 0; n < len; n++) 
  {
    data[n]=FrameData[n];
  }

  return !FrameError;
}

