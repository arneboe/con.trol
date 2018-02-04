#pragma once
#include "stm32f1xx_hal.h"


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;

void initHardware();


/** Returns false if timeout, true if device is ready */
bool waitForI2cReady(uint16_t timeoutMS);


