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
  char text[NUM_CHARS];

private:
  uint32_t lastButtonPress; //used internally in debouncing code
  uint32_t lastButtonRelease; //used internally in debouncing code
  bool pressedLast; //used internally in debouncing code

  void loadText();

public:
  uint8_t getMidiValue() const;
  uint8_t getLinearMidiValue() const;
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
