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
    virtual void show(Adafruit_SSD1306& display) = 0;
    /**return null if there is no more follow up menu */
    virtual Menu* getNextMenu() = 0;
    virtual void setElemNum(uint8_t eNum) {elemNum = eNum;};
    virtual void abort(){};
  };

  struct TextMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display) override;
    virtual Menu* getNextMenu() override;
    void reset();
    virtual void abort() override;
    uint8_t currentCharIndex;
    char currentChar;
  };

  struct MidiMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display) override;
    virtual Menu* getNextMenu() override;
    virtual void abort() override;
  };

  struct ModeMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display) override;
    virtual Menu* getNextMenu() override;
    virtual void abort() override;
  };

  struct MainMenu : public Menu
  {
    virtual void show(Adafruit_SSD1306& display) override;
    virtual Menu* getNextMenu() override;
    virtual void setElemNum(uint8_t elemNum) override;
    TextMenu textMenu;
    MidiMenu midiMenu;
    ModeMenu modeMenu;
  };


  MainMenu mainMenu;
  Menu* currentMenu;
public:
  SetupMenu();
  void show(Adafruit_SSD1306& display);

  /**reset menu and set selected element  */
  void reset(uint8_t elemNum);

  /**Tell the menu that the button was pressed */
  void buttonPressed();

  /**returns true if the setup is done */
  bool done();

  /** Abort current menu (but save state) */
  void abort();

};
