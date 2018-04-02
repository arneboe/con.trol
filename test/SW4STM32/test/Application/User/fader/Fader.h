#pragma once
#include "stm32f1xx_hal.h"
#include "ResponsiveAnalogRead.h"

#define NUM_FADERS 8

struct Fader
{
  float avg = 0;
  ResponsiveAnalogRead filter;
	bool isLinear = false;
	uint8_t getLinearMidiValue();
	uint8_t getLogMidiValue();
	uint8_t getMidiValue(); //return the correct value depending on isLinear
	void update(uint16_t adcValue);
};


//FIXME The whole static global stuff sucks but I don't see how to access the
//      faders from irq in any other way
struct Faders
{
	static Fader faders[NUM_FADERS];
	//start reading the faders
	static void start();
};
