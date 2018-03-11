#pragma once
#include "stm32f1xx_hal.h"

#define NUM_FADERS 8

struct Fader
{
	uint16_t rawAdcValue = 0;
	float averageAdcValue = 0;
	uint8_t midiValue = 0;
	bool isLinear = false;
	uint8_t getLinearMidiValue();
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
