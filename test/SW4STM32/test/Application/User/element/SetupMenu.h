#pragma once
#include <cstdint>
#include "display/Adafruit_SSD1306.h"

class SetupMenu
{
private:
  uint8_t elemNum;

  struct Menu
  {
    uint8_t elemNum;
    virtual void show(Adafruit_SSD1306& display, uint8_t faderValue) = 0;
    /**return null if there is no more follow up menu */
    virtual Menu* getNextMenu(uint8_t faderValue) = 0;
    virtual void setElemNum(uint8_t elemNum) = 0;
    virtual void abort(){};
  };

  struct TextMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display, uint8_t faderValue) override;
    virtual Menu* getNextMenu(uint8_t faderValue) override;
    void reset();
    virtual void setElemNum(uint8_t elemNum) override;
    virtual void abort() override;
    uint8_t currentCharIndex;
    char currentChar;
  };

  struct MainMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display, uint8_t faderValue) override;
    virtual Menu* getNextMenu(uint8_t faderValue) override;
    virtual void setElemNum(uint8_t elemNum) override;
    TextMenu textMenu;
  };


  MainMenu mainMenu;
  Menu* currentMenu;
public:
  SetupMenu();
  void show(Adafruit_SSD1306& display, uint8_t faderValue);

  /**reset menu and set selected element  */
  void reset(uint8_t elemNum);

  /**Tell the menu that the button was pressed */
  void buttonPressed(uint8_t faderValue);

  /**returns true if the setup is done */
  bool done();

  /** Abort current menu (but save state) */
  void abort();

};
