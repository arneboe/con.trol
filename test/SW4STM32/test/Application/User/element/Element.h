#pragma once
#include <cstdint>

#define NUM_ELEMS 8
#define NUM_CHARS 31
#define MENU_START 16

class Element
{
public:
  uint8_t displayNum;
  uint8_t faderNum;
  uint8_t buttonNum;
  uint8_t midiChannel;
  bool isLinear;
  char text[NUM_CHARS];

private:
  uint32_t lastButtonPress; //used internally in debouncing code
  uint32_t lastButtonRelease; //used internally in debouncing code
  bool pressedLast; //used internally in debouncing code


public:
  uint8_t getMidiValue() const;
  uint8_t getLinearMidiValue() const;
  bool getButtonPressed();

  Element(uint8_t displayNum, uint8_t faderNum, uint8_t buttonNum, uint8_t midiChannel, bool isLinear);
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
