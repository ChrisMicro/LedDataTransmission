
#include "platform.h"

#ifdef ARDUINO
	#ifdef ECLIPSE
		#include "Arduino.h"
	#endif
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

#include "stdlib.h"

#include "decoder.h"
#include "adc.h"
#include "filter.h"

#include "mc_io.h"
#include "crc8.h"

#include "decoderStateMachine.h"

#include "senderSTM.h"

#include "frameFormat.h"

#define LED0 8
#define LED1 9
#define LED2 10
#define LED3 11

extern "C"
{
	void debugLedOn(){digitalWrite(ARDUINOLED, HIGH);}
	void debugLedOff()  {digitalWrite(ARDUINOLED, LOW);}
	//void initLed(){pinMode(ARDUINOLED, OUTPUT);}

	void setLedAsOutput() {DDRC|=0x01;}
	void setLedAsInput() {DDRC&=~0x01;PORTC &=~0x01;}

	void ledOn(){PORTC |= 0x01;}
	void ledOff(){PORTC &=~0x01;}

	void toggleLed()
	{
		static int state=0;
		if(state==0) ledOff();
		else ledOn();
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
	//initLed();
	setLedAsInput();
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

//uint8_t TestData[]={0,1,2,3,4,5,6,7,8,9,10};

void sendWithStateMachine(uint8_t *data, uint8_t datalen)
{

	do
	{
		delayMicroseconds(BITTIME_US/2);

	}while( sendFrame_S( data, datalen) != FRAMEREADY );
}
// add crc to frame

void sendFrame(uint8_t *data, uint8_t datalen)
{
	data[0]=0;
	data[0]=crc8(data,datalen);
	sendWithStateMachine(data, datalen);
}
void loop()
{
	//analogRead(analogInPin);
	uint8_t n;
	Serial.println("-1-");

	Serial.println("wait ...");

	while(BrEstimationStateMachine(START)!=M1_READY);
	SystemOutDec("low count: ",BitTimeLow);
	SystemOutDec("    high count: ",BitTimeHigh);
	SystemOutDec("low us: ",BitTimeLow/(16e6/64)*1e6);
	//SystemOutDec("    high us: ",BitTimeHigh/(16e6/64)*1e6);
	while(1)
	{
		while(receiveFrame_S()!=RECEIVERREADY);
		if(FrameError!=FRAMEOK) Serial.println("Frame Timeout");
		for(n=0;n<FRAMESIZE;n++){
			Serial.print(FrameData[n],HEX);Serial.print(" ");
		}
		Serial.print("- ");
		Serial.print(crc8(FrameData+DATASTART,FRAMESIZE-DATASTART),HEX);
		if(crc8(FrameData+DATASTART,FRAMESIZE-DATASTART)!=FrameData[CRC8INDEX])Serial.print("crc error");
		Serial.println(" ");
		if(FrameError==FRAMEOK)
		{
			setLedAsOutput();
			sendFrame(FrameData,FRAMESIZE);
			//ledOn();
			//delay(100);
			ledOff();
			setLedAsInput();
			//delay(100);
		}
		//FrameError=FRAMEOK;
		//setLedAsInput();


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
