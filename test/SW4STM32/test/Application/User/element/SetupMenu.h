#pragma once
#include <cstdint>
#include "display/Adafruit_SSD1306.h"

class SetupMenu
{
private:
  uint8_t elemNum;

  struct Menu
  {
    virtual void show(Adafruit_SSD1306& display, uint8_t faderValue) {printf("base show\n");};
    virtual Menu* getNextMenu() = 0;
  };


  struct MainMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display, uint8_t faderValue) override;
    virtual Menu* getNextMenu() override;
  };


  MainMenu mainMenu;
  Menu* currentMenu;
public:
  SetupMenu();
  void show(Adafruit_SSD1306& display, uint8_t faderValue);

  /**reset menu and set selected element  */
  void reset(uint8_t elemNum);

  /**Tell the menu that the button was pressed */
  void buttonPressed();
};
