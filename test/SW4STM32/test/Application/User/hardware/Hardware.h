#pragma once
#include "stm32f1xx_hal.h"


#define DUMP_VAR(varname) printf("%s = %d\n", #varname, varname);

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;

void initHardware();


/** Returns false if timeout, true if device is ready */
bool waitForI2cReady(uint16_t timeoutMS);

void printI2cError(I2C_HandleTypeDef& i2c);
void handleI2CReturnValue(I2C_HandleTypeDef& i2c, HAL_StatusTypeDef returnStatus);

void hardResetI2C();


//public domain crc32 implementation from http://home.thep.lu.se/~bjorn/crc/crc32_simple.c
uint32_t crc32_for_byte(uint32_t r);
//@param crc needs to be 0-initialized
void calcCrc32(const void *data, size_t n_bytes, uint32_t* crc);

void hexDump(char *desc, void *addr, int len);


