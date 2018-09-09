#pragma once
#include <cstdint>
#include "eeprom/eeprom.h"

#define NUM_ELEMS 8
#define NUM_CHARS 31
#define MENU_START 16


struct ElementUserConfig
{
  uint8_t midiChannel = 0;
  bool faderLinear = false;
  char text[NUM_CHARS] = "UNUSED";
}__attribute__((packed));

struct ElementHardwareConfig
{
  uint8_t displayNum = 0;
  uint8_t faderNum = 0;
  uint8_t buttonNum = 0;
}__attribute__((packed));


class Element
{
public:

  //hard coded config that depends on wiring
  ElementHardwareConfig hwCfg;
  //user configurable part
  ElementUserConfig userCfg;

private:
  uint32_t lastButtonPress; //used internally in debouncing code
  uint32_t lastButtonRelease; //used internally in debouncing code
  bool pressedLast; //used internally in debouncing code


public:
  uint8_t getMidiValue() const;
  uint8_t getLinearMidiValue() const;
  uint8_t getLogMidiValue() const;
  bool isLinear() const;
  bool getButtonPressed();

  Element(uint8_t displayNum, uint8_t faderNum, uint8_t buttonNum, uint8_t midiChannel);
  Element();


};

class Elements
{
private:
  /**Start addresses of user configs in eeprom */
  static uint16_t userConfigEepromAddress[NUM_ELEMS];

public:
  static Element elements[NUM_ELEMS];

  //init from eeprom
  static void init(Eeprom& eeprom);
  //init using defaults
  static void init();

  static void defaultInit();
  static void update();
  /**Stores element config in eeprom */
  static void storeElement(uint8_t elemNum, Eeprom& eeprom);
  };
