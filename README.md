# LedDataTransmission

Using a led connected to the ADC of a microcontroller for data transmission.

## What?

You can use a simple LED to send and receive data. 

## Why?

Using LEDs is very cost effective and sometimes there are no other interfaces available on a microcontroller.
A lot of devices have LEDs as status indicators but they can also be used to send some secret messages invisiblle for humans.

## How?

This library works for an Arduino Uno or Arduino Nano. 
On the sender side, you have to connect an LED to Arduino Pin 13.
On the receiver side you should connect an LED with the Anode to the analog input A0 and the cathode to GND.

You should use ultrabright LEDs. Best working are red and orange LEDs.

Here is a [picture with two arduinos](https://github.com/ChrisMicro/LedDataTransmission/blob/master/doc/arduinoLed.png)
