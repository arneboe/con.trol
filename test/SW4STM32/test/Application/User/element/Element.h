#pragma once

#include <cstdint>

#define NUM_ELEMS 8


class Element
{
public:
  uint8_t displayNum;
  uint8_t faderNum;
  uint8_t buttonNum;
  uint8_t midiChannel;

private:
  uint32_t lastButtonPress; //used internally in debouncing code
  uint32_t lastButtonRelease; //used internally in debouncing code
  bool pressedLast; //used internally in debouncing code
public:
  uint8_t getMidiValue() const;
  bool getButtonPressed();

  Element(uint8_t displayNum, uint8_t faderNum, uint8_t buttonNum, uint8_t midiChannel);
  Element();


};

struct Elements
{
  static Element elements[NUM_ELEMS];
  static void init();
  static void update();

};
