#include "eeprom.h"
#include "Hardware.h"
#include "element/Element.h"
#include <cstring>

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

//    printf("multi: %d\n", address);
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

HAL_StatusTypeDef Eeprom::writeObjectWithCrc(uint16_t address, void* object, uint16_t objectSize, uint16_t& writtenSize)
{

  writtenSize = 0;
  const uint8_t sizeofCrc = sizeof(uint32_t);
  if(address + objectSize + sizeofCrc >= EEPROM_SIZE)
    return HAL_ERROR;

  //calc crc32
  uint32_t crc32 = 0;
  calcCrc32(object, objectSize, &crc32);

  //retry several times if broken (communication with the eeprom is bad sometimes :D)
  for(int i = 0; i < 10; ++i)
  {
    const HAL_StatusTypeDef result = write(address, (unsigned char*)object, objectSize);
    const HAL_StatusTypeDef crcResult = write(address + objectSize, (unsigned char*)&crc32, sizeofCrc);
    writtenSize = objectSize + sizeofCrc;

    if(result == HAL_BUSY || crcResult == HAL_BUSY)
    {
      //sometimes the i2c hangs and is busy forever
      printf("busy error while storing element\n");
      hardResetI2C();
      HAL_Delay(500);
    }
    else if(result == HAL_OK && crcResult == HAL_OK)
    {
      //read back and verify
      //dynamic memory allocation is disabled, thus we limit the object size to 64 bytes (enough for this project)
      uint8_t buffer[64] = {0};
      uint16_t readSize = 0;
      HAL_StatusTypeDef readResult = readObjectWithCrc(address, buffer, objectSize, readSize);
      if(readResult == HAL_OK)
      {
        return HAL_OK;
      }
    }
  }

  return HAL_ERROR;
}

HAL_StatusTypeDef Eeprom::readObjectWithCrc(uint16_t address, void* object, uint16_t objectSize, uint16_t& readSize)
{

  const uint8_t sizeofCrc = sizeof(uint32_t);
  if(address + objectSize + sizeofCrc >= EEPROM_SIZE)
    return HAL_ERROR;


  readSize = objectSize + sizeofCrc;

  //try loading several times
  for(int i = 0; i < 10; ++i)
  {
    uint32_t loadedCrc = 0;
    HAL_StatusTypeDef result = read(address, (unsigned char*)object, objectSize);
    HAL_StatusTypeDef crcResult = read(address + objectSize, (unsigned char*)&loadedCrc, sizeof(uint32_t));
    if(result == HAL_BUSY || crcResult == HAL_BUSY)
    {
      printf("busy error while loading from eeprom\n");
      //i2c busy bug, hard reset and try again
      hardResetI2C();
      HAL_Delay(500);
    }
    else if(result == HAL_OK && crcResult == HAL_OK)
    {
      uint32_t calculatedCrc = 0;
      calcCrc32(object, objectSize, &calculatedCrc);
      if(calculatedCrc == loadedCrc)
      {
        return HAL_OK;
      }
    }
  }
  return HAL_ERROR;
}

void Eeprom::burnConfig()
{
  ElementHardwareConfig hw;
  ElementUserConfig user;

  uint16_t addr = 0;
  uint16_t writeSize = 0;

  hw.faderNum = 7;
  hw.buttonNum = 8;
  hw.displayNum = 0;
  user.midiChannel = 0;
  user.faderLinear = false;
  strcpy(user.text, "Unused");
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;


//  printf("reading bytes: \n");
//  for(int i = 0; i < addr; ++i)
//  {
//    uint8_t data = 42;
//    if(HAL_OK != eeprom.readByte(i,data))
//    {
//      printf("READ ERROR\n");
//      return;
//    }
//    printf("%d ", data);
//  }
//  printf("\n");




  hw.faderNum = 6;
  hw.buttonNum = 7;
  hw.displayNum = 1;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 5;
  hw.buttonNum = 6;
  hw.displayNum = 2;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 4;
  hw.buttonNum = 5;
  hw.displayNum = 3;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 0;
  hw.buttonNum = 4;
  hw.displayNum = 4;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 1;
  hw.buttonNum = 3;
  hw.displayNum = 5;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 2;
  hw.buttonNum = 2;
  hw.displayNum = 6;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;


  hw.faderNum = 3;
  hw.buttonNum = 1;
  hw.displayNum = 7;
  ++user.midiChannel;
  if(HAL_ERROR == this->writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == this->writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;



  //read and print config for human validation
  ElementHardwareConfig hwRead;
  ElementUserConfig userRead;
  uint16_t readAddr = 0;
  uint16_t readSize = 0;


  for(int i = 0; i < 8; ++i)
  {
    if(HAL_OK == this->readObjectWithCrc(readAddr, &hwRead, sizeof(ElementHardwareConfig), readSize))
    {
      readAddr += readSize;
      printf("Reading Element %d\n",i);
      DUMP_VAR(hwRead.buttonNum);
      DUMP_VAR(hwRead.displayNum);
      DUMP_VAR(hwRead.faderNum);
      if(HAL_OK == this->readObjectWithCrc(readAddr, &userRead, sizeof(ElementUserConfig), readSize))
      {
        readAddr += readSize;
        DUMP_VAR(userRead.faderLinear);
        DUMP_VAR(userRead.midiChannel);
        printf("Text: %s\n", userRead.text);
      }
      else
      {
        printf("READ ERROR!!\n");
        return;
      }
    }
    else
    {
      printf("READ ERROR ELEMENT %d\n", i);
      return;
    }
    printf("-----------------------\n");
  }
}


