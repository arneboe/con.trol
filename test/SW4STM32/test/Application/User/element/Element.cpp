#include "Element.h"
#include "button/Button.h"
#include "fader/Fader.h"
#include "eeprom/eeprom.h"
#include "Hardware.h"
#include <cstring>

Element Elements::elements[NUM_ELEMS];
uint16_t Elements::userConfigEepromAddress[NUM_ELEMS];

void Elements::init(Eeprom& eeprom)
{
  uint16_t addr = 0;
  for (int i = 0; i < NUM_ELEMS; ++i)
  {
    uint16_t readSize = 0;
    eeprom.readObjectWithCrc(addr, &elements[i].hwCfg,
        sizeof(ElementHardwareConfig), readSize);
    addr += readSize;
    eeprom.readObjectWithCrc(addr, &elements[i].userCfg,
        sizeof(ElementUserConfig), readSize);
    userConfigEepromAddress[i] = addr;
    addr += readSize;

//    printf("Element: %d\n", i);
//    DUMP_VAR(elements[i].hwCfg.buttonNum);
//    DUMP_VAR(elements[i].hwCfg.displayNum);
//    DUMP_VAR(elements[i].hwCfg.faderNum);
//    printf("---\n");


  }
}

void Elements::init()
{
  for (int i = 0; i < NUM_ELEMS; ++i)
  {

    //cannot store in eeprom if we never loaded from eeprom in the first place
    userConfigEepromAddress[i] = 0;

    elements[i].hwCfg.buttonNum = i;
    elements[i].hwCfg.displayNum = i;
    elements[i].hwCfg.faderNum = i;
    elements[i].userCfg.faderLinear = true;
    elements[i].userCfg.midiChannel = i;
    strcpy(elements[i].userCfg.text, "empty");
  }
}

Element::Element(uint8_t displayNum, uint8_t faderNum, uint8_t buttonNum,
    uint8_t midiChannel) :
    lastButtonPress(0), lastButtonRelease(0), pressedLast(false)
{
  hwCfg.buttonNum = buttonNum;
  hwCfg.displayNum = displayNum;
  hwCfg.faderNum = faderNum;
  userCfg.faderLinear = true;
  userCfg.midiChannel = 0;
}

Element::Element() :
    lastButtonPress(0), lastButtonRelease(0), pressedLast(false)
{
  hwCfg.buttonNum = 0;
  hwCfg.displayNum = 0;
  hwCfg.faderNum = 0;
  userCfg.faderLinear = true;
  userCfg.midiChannel = 0;
}

void Elements::update()
{
  Buttons::update();
  //Faders update automatically in the background using dma
}

uint8_t Element::getLinearMidiValue() const
{
  return Faders::faders[hwCfg.faderNum].getLinearMidiValue();
}

uint8_t Element::getLogMidiValue() const
{
  return Faders::faders[hwCfg.faderNum].getLogMidiValue();
}

bool Element::isLinear() const
{
  return Faders::faders[hwCfg.faderNum].isLinear;
}

bool Element::getButtonPressed()
{
  //debounce button, once pressed it remains pressed for at least 10ms. Once released it remains released for 10ms
  if (HAL_GetTick() - lastButtonPress < 10)
    return true;

  if (HAL_GetTick() - lastButtonRelease < 10)
    return false;

  if (Buttons::pressed[hwCfg.buttonNum])
  {
    //only set the start time the first time we enter this place
    if (!pressedLast)
      lastButtonPress = HAL_GetTick();

    pressedLast = true;
    return true;
  }
  else
  {
    //only set the release time the first time we release
    if (pressedLast)
      lastButtonRelease = HAL_GetTick();
    pressedLast = false;
    return false;
  }
}

uint8_t Element::getMidiValue() const
{
  return Faders::faders[hwCfg.faderNum].getMidiValue();
}

void Elements::storeElement(uint8_t elemNum, Eeprom& eeprom)
{
  if(elemNum >= NUM_ELEMS)
  {
    printf("illegal element number given: %d\n", elemNum);
    return;
  }

  const uint16_t addr = userConfigEepromAddress[elemNum];
  uint16_t bytesWritten = 0;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &elements[elemNum].userCfg, sizeof(ElementUserConfig), bytesWritten))
  {
    printf("Error while storing element user config: %d\n", elemNum);
    return;
  }
}

