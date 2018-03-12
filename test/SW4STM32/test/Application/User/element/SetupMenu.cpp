#include "SetupMenu.h"
#include "Map.h"
#include "Element.h"


char* abc = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_=<>!/|+#";

SetupMenu::SetupMenu() : elemNum(0), currentMenu(&mainMenu)
{
}


void SetupMenu::show(Adafruit_SSD1306& display, uint8_t faderValue)
{
  if(currentMenu != nullptr)
    currentMenu->show(display, faderValue);
}

void SetupMenu::reset(uint8_t elemNum)
{
  currentMenu = &mainMenu;
  this->elemNum =elemNum;
  mainMenu.setElemNum(elemNum);
}

SetupMenu::Menu* SetupMenu::MainMenu::getNextMenu(uint8_t faderValue)
{

  uint8_t selection = map(faderValue, 0, 127, 0, 2);
  if(selection == 2)
  {
    textMenu.reset();
    return &textMenu;
  }
  //TODO implement others
  return this;
}


void SetupMenu::MainMenu::show(Adafruit_SSD1306& display, uint8_t faderValue)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

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

SetupMenu::Menu* SetupMenu::TextMenu::getNextMenu(uint8_t faderValue)
{
  if(currentCharIndex < NUM_CHARS - 2) //-2 because last char is \0
  {
    ++currentCharIndex;
    return this;
  }
  else
  {
    Elements::storeElementConfig(elemNum);
    return nullptr;
  }
}

void SetupMenu::TextMenu::show(Adafruit_SSD1306& display, uint8_t faderValue)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Configure");

  const uint8_t numChars = strlen(abc);

  uint8_t selection = map(faderValue, 0, 127, 0, numChars - 1);
  display.setCursor(0, MENU_START);
  currentChar = abc[selection];
  Elements::elements[elemNum].text[currentCharIndex] = currentChar;
  display.println(Elements::elements[elemNum].text);
  display.display();
}

void SetupMenu::buttonPressed(uint8_t faderValue)
{
  if(currentMenu != nullptr)
    currentMenu = currentMenu->getNextMenu(faderValue);
}

bool SetupMenu::done()
{
  return currentMenu == nullptr;
}

void SetupMenu::MainMenu::setElemNum(uint8_t num)
{
  elemNum = num;
  textMenu.setElemNum(num);
}

void SetupMenu::TextMenu::setElemNum(uint8_t num)
{
  elemNum = num;
}

