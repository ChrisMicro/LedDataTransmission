/*
============================================================================

Name :      adc.c
Author :    ChrisMicro
Version :
Copyright : GPL license 3

Date :      Janurary 2014

Description : analog to digital converter
              This implements a faster ADC than in the standard Arduino libs

Hardware:     Atmega328p

============================================================================
*/
#include "adc.h"
#include "filter.h"

void adc_init()
{
	#ifndef cbi
	#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
	#endif
	#ifndef sbi
	#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
	#endif

	/*
	// set prescaler to 64 instead of 128
	// this will double the sample freqency
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	cbi(ADCSRA, ADPS0);
	*/

	// set prescaler to 32 instead of 128
	// this will make the adc faster
	sbi(ADCSRA, ADPS2);
	cbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);
	//#define DEFAULT 1

	 /* Enable the ADC */
	ADCSRA |= _BV(ADEN);
}

static uint8_t adc_reference = 1;
uint16_t adc_read(uint8_t channel)
{

	ADMUX = (adc_reference << 6) | (channel & 0x07);
	// This starts the conversion.
	ADCSRA |= _BV(ADSC);

	// ADSC is cleared when the conversion finishes
	while ( (ADCSRA & _BV(ADSC)) );

	return ADC;
}
/***************************************************************************************

	uint8_t DCremovedPinValue(uint8_t channel)

    This routine subtracts the DC-Value of the ADC-signal and returns low or high value.

	input:      adc-channel
	output: 	low/high

***************************************************************************************/
uint8_t DCremovedPinValue(uint8_t channel)
{
	uint16_t x=adc_read(channel);

	return AC_signal(x)>0;
}
