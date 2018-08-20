#pragma once
#include <cstdint>
#include "eeprom/eeprom.h"

#define NUM_ELEMS 8
#define NUM_CHARS 31
#define MENU_START 16


struct ElementUserConfig
{
  uint8_t midiChannel;
  bool faderLinear;
  char text[NUM_CHARS];

  //NOTE should always be the last attribute, otherwise calculating the crc breaks
  uint32_t crc32; //crc32 over the previous data


  //load from eeprom
  //@return false if load failed
  bool load(Eeprom& eeprom, uint16_t address);
  //store in eeprom
  //@return false if store failed
  bool store(Eeprom& eeprom, uint16_t address);

}__attribute__((packed));


class Element
{
public:

  //hard coded config that depends on wiring
  uint8_t displayNum;
  uint8_t faderNum;
  uint8_t buttonNum;
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

struct Elements
{
  static Element elements[NUM_ELEMS];
  static void init();
  static void update();

  static void loadElementConfig(uint8_t elemNum);
  static void storeElementText(uint8_t elemNum);
  static void storeMidiChannel(uint8_t elemNum);
  static void storeMode(uint8_t elemNum);
  };
