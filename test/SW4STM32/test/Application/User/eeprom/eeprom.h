//Taken from https://ralimtek.com/stm32-eeprom/

#pragma once
#include "stm32f1xx_hal.h"

#define EEPROM_SIZE 32768 //number of bytes the eeprom can store
#define EEPROM_ADDRESS 0xA0 //0x50 << 1
#define EEPROM_PAGE_SIZE 64 //important because writing wraps around when passing a page boundary

//write cyle time of the eeprom.
//i.e. how much time does the chip need to actually write the data from buffer to eeprom.
//we cannot talk to the chip while it is writing
#define EEPROM_WRITE 5
#define EEPROM_TIMEOUT 5*EEPROM_WRITE  //timeout while writing

class Eeprom
{

public:
  Eeprom(I2C_HandleTypeDef* i2cPort);
  HAL_StatusTypeDef readEEPROM(uint16_t address, uint8_t* MemTarget,
      uint16_t Size);
  /** @param address Address in eeprom
   *  @param MemTarget Pointe to local data that should be written
   *  @param Size Number of bytes that should be written  */
  HAL_StatusTypeDef writeEEPROM(uint16_t address, uint8_t* MemTarget,
      uint16_t Size);


  HAL_StatusTypeDef writeByte(uint16_t address, uint8_t data);


private:
  I2C_HandleTypeDef* i2c_port;

};
