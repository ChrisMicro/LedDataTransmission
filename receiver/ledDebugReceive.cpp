#include "platform.h"
#ifdef ARDUINO
	#include "Arduino.h"
#endif
/*
============================================================================

Name :          ledDebugReceiver.cpp
Author :        ChrisMicro
Version :
Copyright :     GPL license 3
                           ( chris (at) roboterclub-freiburg.de )
Date : Janurary 2014

Description : 	Data transmission by a LED.

				To suppress ambient light FSK coding is used.
============================================================================
*/
/*
 	Platform: Arduino Uno
 */
#include "decoder.h"
#include "adc.h"
#include "filter.h"

#include "mc_io.h"
#include "decoderStateMachine.h"
#include "stdlib.h"
#include "../common/crc8.h"

#define LED0 8
#define LED1 9
#define LED2 10
#define LED3 11

extern "C"
{
	void ledOn(){digitalWrite(ARDUINOLED, HIGH);}
	void ledOff()  {digitalWrite(ARDUINOLED, LOW);}
	void initLed(){pinMode(ARDUINOLED, OUTPUT);}
	void toggleLed()
	{
		static int state=0;
		if(state==0) digitalWrite(ARDUINOLED, LOW);
		else digitalWrite(ARDUINOLED, HIGH);
		state^=0x01;
	}
	void SystemOutText(char * str){
		Serial.println(str);
	}

	void SystemOutDec(char* text, int value)
	{
		Serial.print(text);Serial.println(value);
	}
}

void setup()
{
	initLed();
	//initPort();
	adc_init();

	Serial.begin(9600);
	Serial.println("LED data transmission receiver");

	// Timer 2 normal mode, clk/8, count up from 0 to 255
	// ==> timer overflow frequency @16MHz= 16MHz/8/256=7812.5Hz
	// = 128us
	//TCCR2B= _BV(CS21);
	// Timer 2 normal mode, clk/64, count up from 0 to 255
	// ==> timer overflow frequency @16MHz= 16MHz/64/256=970,56Hz
	// = 1024us
	TCCR2B= _BV(CS22);

	// initialize the digital pin as an output.

	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
}



void loop()
{
	//analogRead(analogInPin);
	uint8_t n;
	Serial.println("--");
/*
	// ADC test
	while(1)
	{
		if(DCremovedPinValue(0)==0)ledOn();
		else ledOff();
	}
	while(1)
	{
		ledOn();
		//delay(500);
		ledOff();
		//delay(500);
		uint16_t x;
		x=adc_read(0);
		Serial.print(x);Serial.print("\t");
		//Serial.println(AC_signal(x));
		Serial.println(DCremovedPinValue(0));
	}*/
	//Serial.println("edge debug, infinite loop, signal out to led");
	//edgeDebug();
	//Serial.println("frame start");
	//receiveFrame();
	//Serial.println("syncDebug");
	//syncDebug();

	//Serial.println("waitn for data .... timer counts for low and high signal");
	Serial.println("wait ...");

	while(BrEstimationStateMachine(START)!=M1_READY);
	SystemOutDec("low count: ",BitTimeLow);
	SystemOutDec("    high count: ",BitTimeHigh);
	SystemOutDec("low us: ",BitTimeLow/(16e6/64)*1e6);
	//SystemOutDec("    high us: ",BitTimeHigh/(16e6/64)*1e6);
	while(1)
	{
		while(receiveFrame_S()!=FRAMEREADY);
		if(FrameError!=FRAMEOK) Serial.println("Frame Timeout");
		for(n=0;n<11;n++){
			Serial.print(FrameData[n],HEX);Serial.print(" ");
		}
		Serial.print("- ");
		Serial.print(crc8(FrameData+1,10),HEX);
		if(crc8(FrameData+1,10)!=FrameData[0])Serial.print("crc error");
		Serial.println(" ");

	}
	while(1)
	{
		//while(highBitReceived_S()!=BITREADY)delayMicroseconds(30);
		while(BrEstimationStateMachine(START)!=M1_READY);
/*
		ledOn();
		//delayMicroseconds(100);
		ledOff();
		for(n=0;n<80;n++)
		{

			while(highBitReceived_S()!=BITREADY);
			ledOn();
			//delayMicroseconds(100);
			ledOff();
		}*/
		//SystemOutText("wait for byte");
		for(n=0;n<10;n++){
			while(receiveByte_S()!=BYTEREADY);
			FrameData[n]=ReceiverData;
			//if(ReceiverData!=0)Serial.println("-----------------");
		}
		for(n=0;n<10;n++){
			Serial.print(FrameData[n],HEX);Serial.print(" ");
		}

		Serial.println(" ");
/*
		BitValue=0;
		while(BitValue!=1)
		{
			while(highBitReceived_S()!=BITREADY);
			//if(BitError==BITOK)
			//{
				if(BitValue==1)
				{
					ledOn();
					delayMicroseconds(100);
					ledOff();
				}
		}*/
		/*
		if(BitError!=0) {
			SystemOutDec("       ERROR:",(uint8_t)BitError);
		}
		else{
			SystemOutDec("BitValue: ",BitValue);
		}
		*/
		BitError=BITOK;
	}



	delay(1000);
	while(1)
	{
		receiveFrame(FrameData);
#ifdef TESTPRINT


#else
	uint8_t z;
	z=FrameData[2];
	  if(z&0x01)digitalWrite(LED0,HIGH);
	  else digitalWrite(LED0,LOW);
	  if(z&0x02)digitalWrite(LED1,HIGH);
	  else digitalWrite(LED1,LOW);
	  if(z&0x04)digitalWrite(LED2,HIGH);
	  else digitalWrite(LED2,LOW);
	  if(z&0x08)digitalWrite(LED3,HIGH);
	  else digitalWrite(LED3,LOW);
#endif
		for(n=0;n<FRAMESIZE;n++)
		{
			Serial.print(FrameData[n],HEX);
			Serial.print("\t");
		}
		Serial.print(  " low: ");	Serial.print(BitTimeLow);
		Serial.print("\t high: ");	Serial.println(BitTimeHigh);
	}
}

#ifndef ARDUINO

	int main (void)
	{
		setup();
		while(1) loop();
		return 0;
	}

#endif
