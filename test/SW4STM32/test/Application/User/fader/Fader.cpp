#include <fader/Fader.h>
#include <hardware/ErrorHandler.h>
#include <hardware/Hardware.h>
#include "Map.h"

Fader Faders::faders[NUM_FADERS];
//NOTE this is later casted to uint32* but the dma is configured to write only half-words
static uint16_t adcValues[NUM_FADERS] = { 0 };


/** maps from linear to logarithmic fader scale
 * modified from: https://playground.arduino.cc/Main/MultiMap */
int linToLog(uint16_t value)
{
  //adc has only 12 bit resolution, everything above has to be garbage and shouldn't be there
  if(value > 4095) value = 4095;

  static float potiVals[] = {0, 516, 741, 870, 999, 1096, 1193, 1257, 1354, 1419, 1483, 1548, 1741, 1935, 2128, 2547, 3031, 3547, 4095};
  static float midiVals[] = {0, 1  , 2  , 3  , 4  , 5   , 6   , 7   , 8   , 9   , 10  , 11  , 15  , 20  , 25  , 39  , 60  , 89  , 127};

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
  filter.update(adcValue);
}

uint8_t Fader::getLinearMidiValue()
{
  const uint8_t midiValue = map(filter.getValue(), 0, 4093, 0, 127);
  //we use 4093 as maximum because most faders cant reach 4096,
  if(midiValue > 127) return 127;//happens if value is > 4093
  return midiValue;
}

uint8_t Fader::getLogMidiValue()
{
  return linToLog(filter.getValue());
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



