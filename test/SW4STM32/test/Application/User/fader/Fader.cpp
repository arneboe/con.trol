#include <fader/Fader.h>
#include <hardware/ErrorHandler.h>
#include <hardware/Hardware.h>
#include <cmath>
#include "Map.h"

Fader Faders::faders[NUM_FADERS];
//NOTE this is later casted to uint32* but the dma is configured to write only half-words
static uint16_t adcValues[NUM_FADERS] = { 0 };

// affects the curve of movement amount > snap amount
// smaller amounts like 0.001 make it ease slower
// larger amounts like 0.1 make it less smooth
#define SNAP_MULTIPLIER 0.1f

float snapCurve(uint16_t x)
// now calculate a 'snap curve' function, where we pass in the diff (x) and get back a number from 0-1. We want small values of x to result in an output close to zero, so when the smooth value is close to the input value it'll smooth out noise aggressively by responding slowly to sudden changes. We want a small increase in x to result in a much higher output value, so medium and large movements are snappy and responsive, and aren't made sluggish by unnecessarily filtering out noise. A hyperbola (f(x) = 1/x) curve is used. First x has an offset of 1 applied, so x = 0 now results in a value of 1 from the hyperbola function. High values of x tend toward 0, but we want an output that begins at 0 and tends toward 1, so 1-y flips this up the right way. Finally the result is multiplied by 2 and capped at a maximum of one, which means that at a certain point all larger movements are maximally snappy
{
  float y = 1.0f / (x + 1);
  y = (1 - y) * 2;
  if(y > 1) {
    return 1;
  }
  return y;
};


/**taken from arduino: used for log to lin conversion
  * https://playground.arduino.cc/Main/MultiMap */
int multiMap(uint16_t value)
{
  //adc has only 12 bit resolution, everything above has to be garbage and shouldnt be there
  if(value > 4095) value = 4095;

  //last 2 values are special cases to make sure that we always reach the highest midi value.
  //This is neccessary to compensate for integer arithmetics
  static uint16_t potiVals[] = {0, 44, 120, 222, 345, 470, 590, 895, 1715, 2465, 3220, 4050, 4094, 4095};
  static uint16_t midiVals[] = {0, 10, 21, 31, 42, 52, 63, 74, 84, 95, 105, 116, 127, 127};

  // search right interval
  uint8_t pos = 0;
  while(value > potiVals[pos]) pos++;

  // this will handle all exact "points" in the potiVals array
  if (value == potiVals[pos]) return midiVals[pos];

  //if the code reaches here pos is always > 0
  // interpolate in the right segment for the rest
  return (value - potiVals[pos-1]) * (midiVals[pos] - midiVals[pos-1]) / (potiVals[pos] - potiVals[pos-1]) + midiVals[pos-1];
}


//from http://damienclarke.me/code/posts/writing-a-better-noise-reducing-analogread
void Fader::update(uint16_t adcValue)
{
  // get difference between new input value and current smooth value
  const float diff = fabs((float)adcValue - averageAdcValue);

  // multiply the input by SNAP_MULTIPLER so input values fit the snap curve better.
  const float snap = snapCurve(diff * SNAP_MULTIPLIER);
  // calculate the exponential moving average based on the snap
  averageAdcValue += (adcValue - averageAdcValue) * snap;

  rawAdcValue = adcValue;

  if(isLinear)
  {
    midiValue = getLinearMidiValue();
  }
  else
  {
    midiValue = map((int)(averageAdcValue + 0.5f), 0, 4096, 0, 127);
  }

  //HACK map should use 4096 as max!
  if(midiValue > 127) midiValue = 127; //can happen because we are using 4093 as maximum. sliders have different limits :D
}

uint8_t Fader::getLinearMidiValue()
{
  return multiMap((int)(averageAdcValue + 0.5f));
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



