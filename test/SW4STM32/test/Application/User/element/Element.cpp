#include "Element.h"
#include "button/Button.h"
#include "fader/Fader.h"
#include "eeprom/eeprom.h"
#include "Hardware.h"

Element Elements::elements[NUM_ELEMS];

bool ElementUserConfig::load(Eeprom& eeprom, uint16_t address)
{
  //try loading several times
  for(int i = 0; i < 5; ++i)
  {
    HAL_StatusTypeDef result = eeprom.read(address, (unsigned char*)this, sizeof(ElementUserConfig));
    if(result == HAL_OK)
    {
      uint32_t loadedCrc = 0;
      calcCrc32(this, sizeof(ElementUserConfig) - sizeof(uint32_t), &loadedCrc);
      if(loadedCrc == crc32)
      {
        return true;
      }
      else
      {
        printf("load crc failed: %d != %d, retrying\n", loadedCrc, crc32);
      }
    }
    else if(result == HAL_BUSY)
    {
      printf("busy error while loading element\n");
      //i2c busy bug, hard reset and try again
      hardResetI2C();
      HAL_Delay(500);
    }
    else
    {
      printf("unknown error while loading element\n");
      //some other random bug, just try again after some time
      HAL_Delay(500);
    }
  }
  return false;
}

bool ElementUserConfig::store(Eeprom& eeprom, uint16_t address)
{
  //calc crc32
  crc32 = 0;
  calcCrc32(this, sizeof(ElementUserConfig) - sizeof(uint32_t), &crc32);

//  hexDump("store", this, sizeof(ElementUserConfig));


  //try the whole write process 20 times, if it fails more often assume everything is broken!
  for(int i = 0; i < 20; ++i)
  {

    HAL_StatusTypeDef result = eeprom.write(address, (unsigned char*)this, sizeof(ElementUserConfig));
    if(result == HAL_OK)
    {
      //try to load and verify that data has been written without corruption
      ElementUserConfig cfg;
      if(cfg.load(eeprom, address))
      {
        return true;
      }
      else
      {
        printf("store verify failed, retrying\n");
      }
    }
    else if(result == HAL_BUSY)
    {
      printf("busy error while storing element\n");
      hardResetI2C();
      HAL_Delay(500);
    }
    else
    {
      printf("unknown error while storing element\n");
      HAL_Delay(500);
    }
  }
}

void Elements::init()
{
  //FIXME use ctor instead

  elements[0].displayNum = 2;
  elements[0].faderNum = 7;
  elements[0].buttonNum = 0;
  elements[0].userCfg.midiChannel = 0;

  elements[1].displayNum = 3;
  elements[1].faderNum = 6;
  elements[1].buttonNum = 3;
  elements[0].userCfg.midiChannel = 1;

  elements[2].displayNum = 5;
  elements[2].faderNum = 5;
  elements[2].buttonNum = 2;
  elements[0].userCfg.midiChannel = 2;

  elements[3].displayNum = 4;
  elements[3].faderNum = 4;
  elements[3].buttonNum = 6;
  elements[0].userCfg.midiChannel = 4;

  elements[4].displayNum = 6;
  elements[4].faderNum = 3;
  elements[4].buttonNum = 7;
  elements[0].userCfg.midiChannel = 5;

  elements[5].displayNum = 7;
  elements[5].faderNum = 2;
  elements[5].buttonNum = 5;
  elements[0].userCfg.midiChannel = 6;

  elements[6].displayNum = 1;
  elements[6].faderNum = 1;
  elements[6].buttonNum = 1;
  elements[0].userCfg.midiChannel = 7;

  elements[7].displayNum = 0;
  elements[7].faderNum = 0;
  elements[7].buttonNum = 8;
  elements[0].userCfg.midiChannel = 8;

  for(int i = 0; i < NUM_ELEMS; ++i)
    loadElementConfig(i);

  //buttonNum index 4 is program buttonNum
}

Element::Element(uint8_t displayNum, uint8_t faderNum, uint8_t buttonNum, uint8_t midiChannel) :
    displayNum(displayNum), faderNum(faderNum), buttonNum(buttonNum),
    lastButtonPress(0), lastButtonRelease(0), pressedLast(false)
{
}

Element::Element() : displayNum(0), faderNum(0), buttonNum(0), lastButtonPress(0),
    lastButtonRelease(0), pressedLast(false)
{
}

void Elements::update()
{
  Buttons::update();
  //Faders update automatically
}


uint8_t Element::getLinearMidiValue() const
{
  return Faders::faders[faderNum].getLinearMidiValue();
}

uint8_t Element::getLogMidiValue() const
{
  return Faders::faders[faderNum].getLogMidiValue();
}

bool Element::isLinear() const
{
  return Faders::faders[faderNum].isLinear;
}

bool Element::getButtonPressed()
{
  //debounce button, once pressed it remains pressed for at least 10ms. Once released it remains released for 10ms
  if(HAL_GetTick() - lastButtonPress < 10)
    return true;

  if(HAL_GetTick() - lastButtonRelease < 10)
    return false;


  if(Buttons::pressed[buttonNum])
  {
    //only set the start time the first time we enter this place
    if(!pressedLast)
      lastButtonPress = HAL_GetTick();

    pressedLast = true;
    return true;
  }
  else
  {
    //only set the release time the first time we release
    if(pressedLast)
      lastButtonRelease = HAL_GetTick();
    pressedLast = false;
    return false;
  }
}

uint8_t Element::getMidiValue() const
{
  return Faders::faders[faderNum].getMidiValue();
}

void Elements::loadElementConfig(uint8_t elemNum)
{

//  //read text
//  uint16_t virtAddr = elemNum * NUM_CHARS;
//  uint16_t remainingChars = NUM_CHARS;
//  uint8_t currentChar = 0;
//  while(remainingChars >= 2)
//  {
//    uint16_t data = 0;
//    EE_ReadVariable(virtAddr, &data);
//    elements[elemNum].text[currentChar] = data; //set low byte
//    ++currentChar;
//    elements[elemNum].text[currentChar] = data >> 8; //set high byte
//    ++currentChar;
//    ++virtAddr;
//    remainingChars -= 2;
//  }
//
//  //num chars was odd,read last char
//  if(remainingChars > 0)
//  {
//    uint16_t data = 0;
//    EE_ReadVariable(virtAddr, &data);
//    elements[elemNum].text[currentChar] = data;
//  }
//
//  //read midi value
//  virtAddr = NUM_ELEMS * NUM_CHARS + elemNum;
//  uint16_t data = 0;
//  EE_ReadVariable(virtAddr, &data);
//  elements[elemNum].midiChannel = data;
//
//  //read mode
//  virtAddr = NUM_ELEMS * NUM_CHARS + NUM_ELEMS + elemNum;
//  data = 0;
//  EE_ReadVariable(virtAddr, &data);
//  Faders::faders[Elements::elements[elemNum].faderNum].isLinear = data;
}

void Elements::storeElementText(uint8_t elemNum)
{
//  uint16_t virtAddr = elemNum * NUM_CHARS;
//  uint16_t remainingChars = NUM_CHARS;
//  uint8_t currentChar = 0;
//  while(remainingChars >= 2)
//  {
//    uint16_t data = elements[elemNum].text[currentChar];
//    ++currentChar;
//    data = (elements[elemNum].text[currentChar] << 8) | data;
//    ++currentChar;
//    EE_WriteVariable(virtAddr, data);
//    ++virtAddr;
//    remainingChars -= 2;
//  }
//  //num chars was odd,write last char
//  if(remainingChars > 0)
//  {
//    uint16_t data = elements[elemNum].text[currentChar];
//    EE_WriteVariable(virtAddr, data);
//  }
}

void Elements::storeMidiChannel(uint8_t elemNum)
{
//  const uint16_t virtAddr = NUM_ELEMS * NUM_CHARS + elemNum;
//  const uint16_t data = elements[elemNum].midiChannel;
//  EE_WriteVariable(virtAddr, data);
}

void Elements::storeMode(uint8_t elemNum)
{
//  const uint16_t virtAddr = NUM_ELEMS * NUM_CHARS + NUM_ELEMS + elemNum;
//  const uint16_t data = Faders::faders[Elements::elements[elemNum].faderNum].isLinear;
//  EE_WriteVariable(virtAddr, data);
}
