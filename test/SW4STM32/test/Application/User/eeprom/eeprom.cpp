#include "eeprom.h"
#include "Hardware.h"

Eeprom::Eeprom(I2C_HandleTypeDef* i2cPort)
{
  i2c_port = i2cPort;
}

HAL_StatusTypeDef Eeprom::writeByte(uint16_t address, uint8_t data)
{
  if (address >= EEPROM_SIZE)
    return HAL_ERROR;

  HAL_StatusTypeDef result = HAL_I2C_Mem_Write(i2c_port, EEPROM_ADDRESS,
      address,
      I2C_MEMADD_SIZE_16BIT, &data, 1, EEPROM_TIMEOUT);

  HAL_Delay(EEPROM_WRITE_DELAY);
  return result;
}

HAL_StatusTypeDef Eeprom::readByte(uint16_t address, uint8_t& data)
{
  if (address >= EEPROM_SIZE)
    return HAL_ERROR;

  HAL_StatusTypeDef result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, address,
  I2C_MEMADD_SIZE_16BIT, &data, 1, EEPROM_TIMEOUT);
  HAL_Delay(EEPROM_READ_DELAY);
  return result;

}

HAL_StatusTypeDef Eeprom::read(uint16_t address, uint8_t* MemTarget,
    uint16_t size)
{
  if (address + size > EEPROM_SIZE)
    return HAL_ERROR;

  //page of first byte
  const uint16_t startPage = address / EEPROM_PAGE_SIZE;
  //page of last byte
  const uint16_t lastAddress = address + size - 1;
  const uint16_t endPage = lastAddress / EEPROM_PAGE_SIZE;

  HAL_StatusTypeDef result = HAL_OK;

  if (startPage == endPage)
  {
    //we dont hit multiple pages, thus we can just read in bulk and nothing bad will happen
    result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, address,
    I2C_MEMADD_SIZE_16BIT, MemTarget, size, EEPROM_TIMEOUT);
    //FIXME why do I need the delay when reading?
    HAL_Delay(EEPROM_READ_DELAY);
  }
  else
  {
    //data crosses page boundary, need to read page wise
    uint16_t counter = 0;
    while (counter < size && result == HAL_OK)
    {
      const uint16_t bytesRemaining = size - counter;
      const uint16_t startAddress = address + counter;
      const uint16_t currentPageStart = startAddress / EEPROM_PAGE_SIZE;
      const uint16_t nextPageStart = currentPageStart * EEPROM_PAGE_SIZE
          + EEPROM_PAGE_SIZE;
      const uint16_t bytesLefOnPage = nextPageStart - startAddress;

      if (bytesRemaining > bytesLefOnPage)
      {
        //read until page boundary
        result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, startAddress,
        I2C_MEMADD_SIZE_16BIT, MemTarget + counter, bytesLefOnPage,
            EEPROM_TIMEOUT);
        counter += bytesLefOnPage;
      }
      else
      {
        //read last bytes
        result = HAL_I2C_Mem_Read(i2c_port, EEPROM_ADDRESS, startAddress,
        I2C_MEMADD_SIZE_16BIT, MemTarget + counter, bytesRemaining,
            EEPROM_TIMEOUT);
        counter += bytesRemaining;
      }

      HAL_Delay(EEPROM_READ_DELAY);
    }
  }
  return result;
}

HAL_StatusTypeDef Eeprom::write(uint16_t address, uint8_t* MemTarget,
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

  if (startPage == endPage)
  {
    //we dont hit multiple pages, thus we can just write in bulk and nothing bad will happen
    result = HAL_I2C_Mem_Write(i2c_port, EEPROM_ADDRESS, address,
    I2C_MEMADD_SIZE_16BIT, MemTarget, size, EEPROM_TIMEOUT);
    //FIXME why do I need this?
    HAL_Delay(EEPROM_WRITE_DELAY);
  }
  else
  {
    //data crosses page boundary, need to split up into several write operations.
    //we could implement some fancy shit here but we don't need the speed anyway, just write
    //byte-wise and be done with it.
    //TODO implement perfect page-wise writing

    printf("multi: %d\n", address);
    for (int i = 0; i < size; ++i)
    {
      result = writeByte(address + i, MemTarget[i]);
      if (result != HAL_OK)
      {
        break;
      }
    }
  }

  return result;
}
