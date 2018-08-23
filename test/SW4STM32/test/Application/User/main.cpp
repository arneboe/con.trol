#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "midi/MidiMessages.h"
#include "midi/Midi.h"
#include "fader/Fader.h"
#include "hardware/ErrorHandler.h"
#include "display/Adafruit_SSD1306.h"
#include "hardware/Hardware.h"
#include "button/Button.h"
#include "element/Element.h"
#include "element/SetupMenu.h"
#include "eeprom/eeprom.h"


#define I2C_MULTIPLEXER_ADDR 0xE0
#define DISPLAY_ADDR 0x78

void tcaselect(uint8_t i) {

  printf("tcaselect %d\n", i);

  if (i > 7) return;

  uint8_t data[1] = {1 << i};

  if(HAL_I2C_Master_Transmit(&hi2c1, I2C_MULTIPLEXER_ADDR, data, 1, 100) != HAL_OK)
  {
    printf("Failed to set addr %d\n", i);
  }
  else
  {
    //printf("Set multiplexer target to %d\n", i);
    if(!waitForI2cReady(10))
    {
      printf("I2C timeout in tcaselect(%d)\n", i);
    }
  }
}


void updateDisplayLoop(Adafruit_SSD1306& display);
void setupDisplayLoop(Adafruit_SSD1306& display);
void resetDisplays(Adafruit_SSD1306& display);
void displayHeader(Adafruit_SSD1306& display, uint8_t curentFaderValue, bool currentButtonValue,
                   uint8_t midiChannel, bool isLinear);

bool setupRunning = false;
SetupMenu menu;


//method used to burn the config once
void burnConfig(Eeprom& eeprom)
{
  //8 times hw config/user config
/*
  ElementHardwareConfig hw;
  ElementUserConfig user;

  uint16_t addr = 0;
  uint16_t writeSize = 0;

  hw.faderNum = 0;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 1;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 2;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 3;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 4;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 5;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;

  hw.faderNum = 6;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;


  hw.faderNum = 7;
  eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize);
  addr += writeSize;
  eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize);
  addr += writeSize;
*/

}


int main(void)
{
  initHardware();



//  Eeprom eeprom(&hi2c1);

  //printf("burning config\n");
  //burnConfig(eeprom);
  //printf("done\n");

  Elements::init();


  Adafruit_SSD1306 display(hi2c1);
  for(int i = 0; i < NUM_ELEMS; ++i)
  {
    tcaselect(i);

    if(HAL_I2C_IsDeviceReady(&hi2c1, DISPLAY_ADDR, 2, 10) != HAL_OK)
    {
      printf("display %d not ready!!\n", i);
      continue;
    }
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR);
    waitForI2cReady(50);
  }

  printf("adrer dis\n");

  resetDisplays(display);

  printf("after reset\n");


  while(1);


  while(1)
  {

    //FIXME not debounced?!
    const bool setupButtonPressed = Buttons::pressed[4]; //FIXME magic constant

    if(setupButtonPressed || setupRunning)
    {
      setupDisplayLoop(display);
    }
    else
    {
      updateDisplayLoop(display);
    }
  }


}


void setupDisplayLoop(Adafruit_SSD1306& display)
{
  static uint8_t setupElement = 0;
  static uint32_t setupEnterTime = 0;
  static bool buttonPressed = false;

  //buffers whether the fader is linear or not.
  if(setupRunning)
  {
    if(menu.done())
    {
      setupRunning = false;
      resetDisplays(display);
      return;
    }

    //allow user to abort setup after 2 seconds
    if((Buttons::pressed[4] && HAL_GetTick() - setupEnterTime > 2000)) //FIXME magic constant
    {
      menu.abort();
      setupRunning = false;
      resetDisplays(display);
      return;
    }

    if(Elements::elements[setupElement].getButtonPressed() && !buttonPressed &&
       HAL_GetTick() - setupEnterTime > 500)
    {
      buttonPressed = true;
      menu.buttonPressed();
    }
    else if(!Elements::elements[setupElement].getButtonPressed())
    {
      buttonPressed = false;
    }

    tcaselect(Elements::elements[setupElement].hwCfg.displayNum);

    menu.show(display);

    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[setupElement].hwCfg.displayNum);
    }
//    HAL_Delay(100);

  }
  else
  {
    //wait for button to be pressed
    for(int i = 0; i < NUM_ELEMS; ++i)
    {
      if(Elements::elements[i].getButtonPressed())
      {
        setupElement = i;
        setupRunning = true;
        setupEnterTime = HAL_GetTick();
        menu.reset(i);
        break;
      }
    }
  }
}

void updateDisplayLoop(Adafruit_SSD1306& display)
{
  //the main just updates the displays.
  //everything else is done in interrupts
  static uint8_t lastMidiValues[NUM_ELEMS] = {255};
  static bool lastButtons[NUM_ELEMS] = {false};
  for(int i = 0; i < NUM_ELEMS; ++i)
  {
    const uint8_t curentFaderValue = Elements::elements[i].getMidiValue();
    const bool currentButtonValue = Elements::elements[i].getButtonPressed();
    const uint8_t midiChannel = Elements::elements[i].userCfg.midiChannel;
    const bool linear = Elements::elements[i].isLinear();
    if(curentFaderValue != lastMidiValues[i] || currentButtonValue != lastButtons[i])
    {
      lastMidiValues[i] = curentFaderValue;
      lastButtons[i] = currentButtonValue;
      tcaselect(Elements::elements[i].hwCfg.displayNum);
      displayHeader(display, curentFaderValue, currentButtonValue, midiChannel, linear);
    }
  }
}

/**sets the display text for all displays */
void resetDisplays(Adafruit_SSD1306& display)
{

  for(int i = 0; i < NUM_ELEMS; ++i)
  {
    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].hwCfg.displayNum);
      continue;
    }
    tcaselect(i);

    if(HAL_I2C_IsDeviceReady(&hi2c1, DISPLAY_ADDR, 2, 10) != HAL_OK)
    {
      printf("display %d not ready!!\n", i);
      continue;
    }

    display.fillScreen(BLACK);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, MENU_START);
    display.println("test");
    display.display();
  }
  return;



  for(int i = 0; i < NUM_ELEMS; ++i)
  {
    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].hwCfg.displayNum);
    }
    tcaselect(Elements::elements[i].hwCfg.displayNum);

    if(HAL_I2C_IsDeviceReady(&hi2c1, DISPLAY_ADDR, 2, 10) != HAL_OK)
    {
      printf("display %d not ready!!\n", i);
      continue;
    }

    display.fillScreen(BLACK);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, MENU_START);
    display.println(Elements::elements[i].userCfg.text);
    display.display();

    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].hwCfg.displayNum);
    }
    displayHeader(display, Elements::elements[i].getMidiValue(), Elements::elements[i].getButtonPressed(),
                  Elements::elements[i].userCfg.midiChannel, Elements::elements[i].isLinear());
    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].hwCfg.displayNum);
    }
    //TODO display text!
  }
}

void displayHeader(Adafruit_SSD1306& display, uint8_t curentFaderValue, bool currentButtonValue,
                   uint8_t midiChannel, bool isLinear){
  display.fillHeader(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(50, 0);
  display.println(curentFaderValue);

  display.setTextSize(1);
  display.setCursor(110, 0);
  display.println(midiChannel);
  display.setCursor(110, 9);
  if(isLinear)
    display.println("LIN");
  else
    display.println("LOG");


  if(currentButtonValue)
    display.fillCircle(5, 5, 5, WHITE);

  if(!display.displayHeader())
  {
    printf("ERROR: Display did not respond\n");
  }
  if(!waitForI2cReady(50))
  {
    printf("ERROR: Display failed\n");
  }
}


extern "C"
{
  //handle midi messages at 1000hz
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {

    //read button states, doesn't take any time so just do it here
    Elements::update();

    //update midi

    //static storage locations for message structs
    static CCMessage ccMessages[NUM_FADERS];
    static NoteMessage noteMessages[NUM_FADERS];


    MidiMessage* messages[16];


    //used to remember the last values and avoid spam,
    //255 can never happen in midi thus it is a good initial value
    static uint8_t lastFaderValues[NUM_FADERS] = {255};

    uint8_t nextCCIndex = 0;
    uint8_t nextMessageIndex = 0;

    //check faders
    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const uint8_t faderValue = Elements::elements[i].getMidiValue();
      const uint8_t midiChannel = Elements::elements[i].userCfg.midiChannel;
      if(faderValue != lastFaderValues[i])
      {
        lastFaderValues[i] = faderValue;
        ccMessages[nextCCIndex].controlChannel = midiChannel;
        ccMessages[nextCCIndex].value = faderValue;
        messages[nextMessageIndex] = &ccMessages[nextCCIndex];
        ++nextMessageIndex;
        ++nextCCIndex;
      }
    }

    uint8_t nextNoteIndex = 0;

    static uint8_t lastButtonPressed[NUM_FADERS] = {0};
    //check buttons
    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const bool pressed = Elements::elements[i].getButtonPressed();

      if(pressed != lastButtonPressed[i])
      {
        lastButtonPressed[i] = pressed;
        noteMessages[nextNoteIndex].note = Elements::elements[i].userCfg.midiChannel;
        noteMessages[nextNoteIndex].on = pressed;
        if(pressed)
        {
          noteMessages[nextNoteIndex].velocity = 127;
        }
        else
        {
          noteMessages[nextNoteIndex].velocity = 0;
        }

        messages[nextMessageIndex] = &noteMessages[nextNoteIndex];
        ++nextMessageIndex;
        ++nextNoteIndex;
      }
    }

    //if anything changed, send it
    if(nextMessageIndex > 0)
    {
      Midi::sendMessages(messages, nextMessageIndex);
    }
  }
}

