#include <fader/Fader.h>
#include <hardware/ErrorHandler.h>
#include <hardware/Hardware.h>
#include "Map.h"

Fader Faders::faders[NUM_FADERS];
//NOTE this is later casted to uint32* but the dma is configured to write only half-words
static uint16_t adcValues[NUM_FADERS] = { 0 };


/**taken from arduino: used for log to lin conversion
  * https://playground.arduino.cc/Main/MultiMap */
int multiMap(uint16_t value)
{
  //adc has only 12 bit resolution, everything above has to be garbage and shouldn't be there
  if(value > 4095) value = 4095;

//  static uint16_t potiVals[] = {0, 44, 120, 222, 345, 470, 590, 895, 1715, 2465, 3220, 4050, 4094, 4095};
  static float potiVals[] = {0, 12, 87, 130, 677, 4009, 4030, 4050};
//  static uint16_t midiVals[] = {0, 10, 21, 31, 42, 52, 63, 74, 84, 95, 105, 116, 127, 127};
  static float midiVals[] = {0, 0, 26, 29, 70, 120, 121, 127};

  // search right interval
  uint8_t pos = 0;
  while(value > potiVals[pos]) pos++;

  // this will handle all exact "points" in the potiVals array
  if (value == potiVals[pos]) return midiVals[pos];

  //if the code reaches here pos is always > 0
  // interpolate in the right segment for the rest
  return (value - potiVals[pos-1]) * (midiVals[pos] - midiVals[pos-1]) / (potiVals[pos] - potiVals[pos-1]) + midiVals[pos-1];
}


void Fader::update(uint16_t adcValue)
{
//  avg = 0.5f * avg + 0.5f * adcValue;
  filter.update(adcValue);
}

uint8_t Fader::getLinearMidiValue()
{
  const uint8_t midiValue = multiMap(filter.getValue());
  if(midiValue > 127) return 127; //happens because mapping from log to lin is not perfect (different faders have different maximums)
  return midiValue;
}

uint8_t Fader::getLogMidiValue()
{
  const uint8_t midiValue = map(filter.getValue(), 0, 4085, 0, 127);
  //we use 4085 as maximum because most faders cant reach 4096,
  if(midiValue > 127) return 127;//happens if value is > 4085
  return midiValue;
}

uint8_t Fader::getMidiValue()
{
  if(isLinear)
  {
    return getLinearMidiValue();
  }
  return getLogMidiValue();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  for (int i = 0; i < NUM_FADERS; ++i)
  {
    Faders::faders[i].update(adcValues[i]);
  }
  //start next reading
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcValues, NUM_FADERS) != HAL_OK)
    Error_Handler();
}

void Faders::start()
{
  //calibrate adc
  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    Error_Handler();

  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcValues, NUM_FADERS) != HAL_OK)
    Error_Handler();
}



