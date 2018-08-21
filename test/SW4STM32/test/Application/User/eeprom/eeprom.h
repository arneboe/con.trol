//Taken from https://ralimtek.com/stm32-eeprom/

#pragma once
#include "stm32f1xx_hal.h"

#define EEPROM_SIZE 32768 //number of bytes the eeprom can store
#define EEPROM_ADDRESS 0xA0 //0x50 << 1
#define EEPROM_PAGE_SIZE 64 //important because writing wraps around when passing a page boundary

//write cyle time of the eeprom.
//i.e. how much time does the chip need to actually write the data from buffer to eeprom.
//we cannot talk to the chip while it is writing
#define EEPROM_WRITE_DELAY 10
#define EEPROM_READ_DELAY 10
#define EEPROM_TIMEOUT 5*EEPROM_WRITE_DELAY  //timeout while writing

class Eeprom
{

public:
  Eeprom(I2C_HandleTypeDef* i2cPort);
  HAL_StatusTypeDef read(uint16_t address, uint8_t* MemTarget,
      uint16_t Size);
  /** @param address Address in eeprom
   *  @param MemTarget Pointe to local data that should be written
   *  @param Size Number of bytes that should be written  */
  HAL_StatusTypeDef write(uint16_t address, uint8_t* MemTarget,
      uint16_t Size);


  HAL_StatusTypeDef writeByte(uint16_t address, uint8_t data);
  HAL_StatusTypeDef readByte(uint16_t address, uint8_t& data);


  /** Stores @p object at @p address. Stores a crc32 checksum alongside the object.
   * @p writtenSize The number of bytes written to the eeprom (including the checksum)
   * @note currently the maximum object size is 64 bytes, this might get fixed later  */
  HAL_StatusTypeDef writeObjectWithCrc(uint16_t address, void* object, uint16_t objectSize, uint16_t& writtenSize);

  /** Reads @p object from @p address. Reads and checks the crc32 checksum.  */
  HAL_StatusTypeDef readObjectWithCrc(uint16_t address, void* object, uint16_t objectSize);



private:
  I2C_HandleTypeDef* i2c_port;

};
