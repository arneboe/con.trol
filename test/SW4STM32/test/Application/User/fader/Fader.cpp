#include <fader/Fader.h>
#include <hardware/Handles.h>
#include <hardware/ErrorHandler.h>

Fader Faders::faders[NUM_FADERS];

#define ALPHA 0.5f //NOTE has to be float for implicit casting in calculation

//NOTE this is later casted to uint32* but the dma is configured to write only half-words
static uint16_t adcValues[NUM_FADERS] = { 0 };

void Fader::update(uint16_t adcValue)
{
  averageAdcValue = ALPHA * adcValue + (1.0f - ALPHA) * averageAdcValue;
  rawAdcValue = adcValue;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  for (int i = 0; i < NUM_FADERS; ++i)
  {
    Faders::faders[i].update(adcValues[i]);

  }
  printf("%d %d", Faders::faders[0].rawAdcValue, ((uint16_t)(Faders::faders[0].averageAdcValue + 0.5f)));
  printf("\n");


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
