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


/**Variable types for eeprom */
uint16_t VirtAddVarTab[NB_OF_VAR];

void tcaselect(uint8_t i) {
  if (i > 7) return;

  uint8_t data[1] = {1 << i};

  if(HAL_I2C_Master_Transmit(&hi2c1, 0xe0, data, 1, 10000) != HAL_OK)
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
void displayHeader(Adafruit_SSD1306& display, uint8_t curentFaderValue, bool currentButtonValue);

bool setupRunning = false;
SetupMenu menu;

int main(void)
{
  //FIXME move this to hardware?
  //init eeprom variable ids (one id for each variable that we want to store)
  for(int i = 0; i < NB_OF_VAR; ++i)
  {
    VirtAddVarTab[i] = i;
  }

  initHardware();


  Adafruit_SSD1306 display(hi2c1);
  for(int i = 0; i < 8; ++i)
  {
    tcaselect(i);
    display.begin(SSD1306_SWITCHCAPVCC, 0x78);
    waitForI2cReady(50);
  }

  //FIXME move this to hardware?
  HAL_FLASH_Unlock();
  EE_Init();
  Elements::init();

  resetDisplays(display);

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
  if(setupRunning)
  {
    //allow user to abort setup after 2 seconds
    if((Buttons::pressed[4] && HAL_GetTick() - setupEnterTime > 2000) ||
       menu.done()) //FIXME magic constant
    {
      setupRunning = false;
      resetDisplays(display);
      return;
    }

    if(Elements::elements[setupElement].getButtonPressed() && !buttonPressed &&
       HAL_GetTick() - setupEnterTime > 500)
    {
      buttonPressed = true;
      menu.buttonPressed(Elements::elements[setupElement].getLinearMidiValue());
    }
    else if(!Elements::elements[setupElement].getButtonPressed())
    {
      buttonPressed = false;
    }

    tcaselect(Elements::elements[setupElement].displayNum);

    //always use linear value for menu navigation, much smoother
    menu.show(display, Elements::elements[setupElement].getLinearMidiValue());

    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[setupElement].displayNum);
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
    if(curentFaderValue != lastMidiValues[i] || currentButtonValue != lastButtons[i])
    {
      lastMidiValues[i] = curentFaderValue;
      lastButtons[i] = currentButtonValue;
      tcaselect(Elements::elements[i].displayNum);
      displayHeader(display, curentFaderValue, currentButtonValue);
    }
  }
}

/**sets the display text for all displays */
void resetDisplays(Adafruit_SSD1306& display)
{
  for(int i = 0; i < 8; ++i)
  {
    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].displayNum);
    }
    tcaselect(Elements::elements[i].displayNum);
    display.fillScreen(BLACK);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, MENU_START);
    display.println(Elements::elements[i].text);
    display.display();

    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].displayNum);
    }
    displayHeader(display, Elements::elements[i].getMidiValue(), Elements::elements[i].getButtonPressed());
    if(!waitForI2cReady(50))
    {
      printf("ERROR: Display %d failed\n", Elements::elements[i].displayNum);
    }
    //TODO display text!
  }
}

void displayHeader(Adafruit_SSD1306& display, uint8_t curentFaderValue, bool currentButtonValue)
{
  display.fillHeader(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(curentFaderValue);

  if(currentButtonValue)
    display.fillCircle(50, 5, 5, WHITE);

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

//    //message buffer is reused every time
//    static CCMessage ccMessages[NUM_FADERS];
//
//
//    //used to remember the last values and avoid spam,
//    //255 can never happen in midi thus it is a good initial value
//    static uint8_t lastValues[NUM_FADERS] = {255};
//
//    uint8_t nextMessageIndex = 0;
//
//    for(int i = 0; i < NUM_FADERS; ++i)
//    {
//      const uint8_t faderI = indexMap[i].fader;
//      const uint8_t value = Faders::faders[faderI].midiValue;
//
//      if(value != lastValues[i])
//      {
//        lastValues[i] = value;
//        ccMessages[nextMessageIndex].controlChannel = i;
//        ccMessages[nextMessageIndex].value = value;
//        ++nextMessageIndex;
//      }
//    }
//
//    //if anything changed, send it
//    if(nextMessageIndex > 0)
//    {
//      Midi::sendCC(ccMessages, nextMessageIndex);
//    }
  }
}

