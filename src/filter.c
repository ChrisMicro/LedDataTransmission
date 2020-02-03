/*
============================================================================

Name :      filter.c
Author :    ChrisMicro
Version :
Copyright : GPL license 3

Date :      February 2014

Description : digital filter routines

============================================================================
*/
#include "filter.h"

/***************************************************************************************

	uint16_t lowPassUint16(uint16_t x)

	Integer IIR one pole low pass filter.

	y2=a*x+(1-a)*y1, a in 1/65536
	Filter += (ADC Wert-Filter) / K;
	K * Abtastzeit = Zeitkonstate des Filters 1. Ordnung.
	==> a=tau*65536/fab

	input: x signall
	outpu: y signal

	attention: the filter can have only one caller due to static memory

***************************************************************************************/

uint16_t lowPassUint16(uint16_t x)
{
   uint16_t a=1000; // range of a 0..65536 ==> 1..0

   int32_t y;
   static int32_t y_old=0;

   // low pass filter
   y=y_old+a*(x-(y_old>>16));
   y_old=y;
   return y>>16;
}

/*
uint16_t lowPass(uint16_t x)
{
  //low pass koeffizient
   float a=0.001;

   static float y,y_old=0;

   // low pass filter
   y=x*a+y_old*(1-a);
   y_old=y;
   return (uint16_t) y;
}*/

/***************************************************************************************

	int16_t AC_signal(uint16_t x)

    This routine is mainly a high pass filter to remove the DC part of a signal.

	input:      uint16_t signal with DC part
	output: 	int16_t AC signal, DC removed

***************************************************************************************/
int16_t AC_signal(uint16_t x)
{
	int32_t temp;
	temp=x;

	temp=temp-lowPassUint16(x);
	return temp;
}
