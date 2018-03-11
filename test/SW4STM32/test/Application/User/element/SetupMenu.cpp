#include "SetupMenu.h"
#include "Map.h"

#define MENU_START 16

SetupMenu::SetupMenu() : elemNum(0), currentMenu(&mainMenu)
{
}


void SetupMenu::show(Adafruit_SSD1306& display, uint8_t faderValue)
{
  currentMenu->show(display, faderValue);
}

void SetupMenu::reset(uint8_t elemNum)
{
  currentMenu = &mainMenu;
  this->elemNum =elemNum;
}

SetupMenu::Menu* SetupMenu::MainMenu::getNextMenu()
{
  return this;
}


void SetupMenu::MainMenu::show(Adafruit_SSD1306& display, uint8_t faderValue)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

  uint8_t selection = map(faderValue, 0, 127, 2, 0);
  display.println("Configure");

  display.setCursor(0, MENU_START);
  if(selection == 0)
    display.println(">> Text");
  else
    display.println("   Text");

  if(selection == 1)
    display.println(">> Midi");
  else
    display.println("   Midi");

  if(selection == 2)
    display.println(">> Mode");
  else
    display.println("   Mode");

    display.display();
}

