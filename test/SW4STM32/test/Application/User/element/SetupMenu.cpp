#include "SetupMenu.h"
#include "Map.h"
#include "Element.h"
#include <fader/Fader.h>


char* abc = " ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";

SetupMenu::SetupMenu() : elemNum(0), currentMenu(&mainMenu)
{
}


void SetupMenu::show(Adafruit_SSD1306& display)
{
  if(currentMenu != nullptr)
    currentMenu->show(display);
}

void SetupMenu::reset(uint8_t elemNum)
{
  currentMenu = &mainMenu;
  this->elemNum =elemNum;
  mainMenu.setElemNum(elemNum);
}

SetupMenu::Menu* SetupMenu::MainMenu::getNextMenu()
{

  const uint8_t faderValue = Elements::elements[elemNum].getLogMidiValue();
  uint8_t selection = map(faderValue, 0, 127, 0, 2);
  if(selection == 0)
  {
    return &modeMenu;
  }
  else if(selection == 1)
  {
    return &midiMenu;
  }
  else if(selection == 2)
  {
    textMenu.reset();
    return &textMenu;
  }
  //TODO implement others
  return this;
}


void SetupMenu::MainMenu::show(Adafruit_SSD1306& display)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

  const uint8_t faderValue = Elements::elements[elemNum].getLogMidiValue();
  uint8_t selection = map(faderValue, 0, 127, 0, 2);
  display.println("Configure");
//  display.println(selection);

  display.setCursor(0, MENU_START);
  if(selection == 2)
    display.println(">> Text");
  else
    display.println("   Text");

  if(selection == 1)
    display.println(">> Midi");
  else
    display.println("   Midi");

  if(selection == 0)
    display.println(">> Mode");
  else
    display.println("   Mode");

  display.display();
}

void SetupMenu::TextMenu::reset()
{
  currentCharIndex = 0;
}

SetupMenu::Menu* SetupMenu::TextMenu::getNextMenu()
{
  if(currentCharIndex < NUM_CHARS - 2) //-2 because last char is \0
  {
    ++currentCharIndex;
    return this;
  }
  else
  {
    return nullptr;
  }
}

void SetupMenu::TextMenu::show(Adafruit_SSD1306& display)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SET TEXT");

  const uint8_t numChars = strlen(abc);
  const uint8_t faderValue = Elements::elements[elemNum].getLogMidiValue();
  uint8_t selection = map(faderValue, 0, 127, 0, numChars - 1);
  display.setCursor(0, MENU_START);
  currentChar = abc[selection];
  Elements::elements[elemNum].userCfg.text[currentCharIndex] = currentChar;
  display.println(Elements::elements[elemNum].userCfg.text);
  display.display();
}

void SetupMenu::buttonPressed()
{
  if(currentMenu != nullptr)
    currentMenu = currentMenu->getNextMenu();
}

bool SetupMenu::done()
{
  return currentMenu == nullptr;
}

void SetupMenu::MainMenu::setElemNum(uint8_t num)
{
  elemNum = num;
  textMenu.setElemNum(num);
  midiMenu.setElemNum(num);
  modeMenu.setElemNum(num);
}


void SetupMenu::TextMenu::abort()
{
}

SetupMenu::Menu* SetupMenu::MidiMenu::getNextMenu()
{
  return nullptr;
}

void SetupMenu::MidiMenu::show(Adafruit_SSD1306& display)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SET MIDI");

  //FIXME why midi 119? There was some reason?!
  const uint8_t faderValue = Elements::elements[elemNum].getLogMidiValue();
  uint8_t midiChannel = map(faderValue, 0, 127, 0, 119);
  Elements::elements[elemNum].userCfg.midiChannel = midiChannel;
  display.setCursor(0, MENU_START);
  display.setTextSize(2);

  display.println(midiChannel);
  display.display();
}

void SetupMenu::ModeMenu::show(Adafruit_SSD1306& display)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  const uint8_t faderValue = Elements::elements[elemNum].getLogMidiValue();
  uint8_t selection = map(faderValue, 0, 127, 0, 1);
  Faders::faders[Elements::elements[elemNum].hwCfg.faderNum].isLinear = (bool) selection;
  display.println("SET MODE");
//  display.println(selection);

  display.setCursor(0, MENU_START);
  if(selection == 1)
    display.println(">> LIN");
  else
    display.println("   LIN");

  if(selection == 0)
    display.println(">> LOG");
  else
    display.println("   LOG");
  display.display();
}

SetupMenu::Menu* SetupMenu::ModeMenu::getNextMenu()
{
  return nullptr;
}

void SetupMenu::ModeMenu::abort()
{
}

void SetupMenu::MidiMenu::abort()
{
}


void SetupMenu::abort()
{
  if(currentMenu != nullptr)
    currentMenu->abort();
  currentMenu = nullptr;
}

