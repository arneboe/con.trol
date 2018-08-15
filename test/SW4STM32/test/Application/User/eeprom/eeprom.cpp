#include "eeprom.h"

Eeprom::Eeprom(I2C_HandleTypeDef* i2cPort)
{
  i2c_port = i2cPort;
}

HAL_StatusTypeDef Eeprom::writeByte(uint16_t address, uint8_t data)
{
  if(address >= EEPROM_SIZE)
    return HAL_ERROR;

  HAL_StatusTypeDef result = HAL_I2C_Mem_Write(i2c_port, EEPROM_ADDRESS, address,
      I2C_MEMADD_SIZE_16BIT, &data, 1, EEPROM_TIMEOUT);

  HAL_Delay(EEPROM_WRITE);
  return result;
}

HAL_StatusTypeDef Eeprom::readEEPROM(uint16_t address, uint8_t* MemTarget,
    uint16_t Size)
{
  if (address + Size > EEPROM_SIZE)
    return HAL_ERROR;

  uint16_t Counter = 0;
  HAL_StatusTypeDef Result = HAL_OK;
  while (Counter < Size && Result == HAL_OK)
  {
    uint16_t Diff = Size - Counter;

    if (Diff >= EEPROM_PAGE_SIZE)
    {
      //Multi-Byte
      Result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, address + Counter,
          I2C_MEMADD_SIZE_16BIT, &MemTarget[Counter], EEPROM_PAGE_SIZE,
          EEPROM_TIMEOUT);
      Counter += EEPROM_PAGE_SIZE;
    }
    else
    {
      //and the remaining ones...low packet size
      Result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, address + Counter,
          I2C_MEMADD_SIZE_16BIT, &MemTarget[Counter], Diff, EEPROM_TIMEOUT);
      Counter += Diff;
    }
    HAL_Delay(EEPROM_WRITE / 2); //FIXME why do I need this? HAL_I2C_Mem_Read is blocking?!
  }
  return Result;
}



HAL_StatusTypeDef Eeprom::writeEEPROM(uint16_t address, uint8_t* MemTarget,
    uint16_t size)
{

  if (address + size > EEPROM_SIZE)
    return HAL_ERROR;

  //page of first byte
  const uint16_t startPage = address / EEPROM_PAGE_SIZE;
  //page of last byte
  const uint16_t lastAddress = address + size - 1;
  const uint16_t endPage = lastAddress / EEPROM_PAGE_SIZE;

//  printf("size:%d,lastAddress:%d, startPage:%d, endPage: %d\n", size, lastAddress, startPage, endPage);
  HAL_StatusTypeDef result = HAL_OK;

  if(startPage == endPage)
  {
    //we dont hit multiple pages, thus we can just write in bulk and nothing bad will happen
    result = HAL_I2C_Mem_Write(i2c_port, EEPROM_ADDRESS, address,
        I2C_MEMADD_SIZE_16BIT, MemTarget, size, EEPROM_TIMEOUT);
    //FIXME why do I need this?
    HAL_Delay(EEPROM_WRITE);
  }
  else
  {
    //data crosses page boundary, need to split up into several write operations.
    //we could implement some fancy shit here but we don't need the speed anyway, just write
    //byte-wise and be done with it.
    //TODO implement perfect page-wise writing

    for(int i = 0; i < size; ++i)
    {
      result = writeByte(address + i, MemTarget[i]);
      if(result != HAL_OK)
      {
        break;
      }
    }
  }

  return result;
}
