#include <fader/Fader.h>
#include <hardware/ErrorHandler.h>
#include <hardware/Hardware.h>
#include <cmath>

Fader Faders::faders[NUM_FADERS];
//NOTE this is later casted to uint32* but the dma is configured to write only half-words
static uint16_t adcValues[NUM_FADERS] = { 0 };


/**map from arduino */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  if ((in_max - in_min) > (out_max - out_min)) {
    return (x - in_min) * (out_max - out_min+1) / (in_max - in_min+1) + out_min;
  }
  else
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
}

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
  midiValue = map((int)(averageAdcValue + 0.5f), 0, 4096, 0, 127);
  //HACK map should use 4096 as max!
  if(midiValue > 127) midiValue = 127; //can happen because we are using 4093 as maximum. sliders have different limits :D
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



