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
#define EEPROM_ADDR 0xA0
#define SETUP_BUTTON_NUM 0

void tcaselect(uint8_t i) {

//  printf("tcaselect %d\n", i);

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


void mainDisplayLoop(Adafruit_SSD1306& display);
void setupDisplayLoop(Adafruit_SSD1306& display, Eeprom& eeprom);
void resetDisplays(Adafruit_SSD1306& display);
void displayHeader(Adafruit_SSD1306& display, uint8_t curentFaderValue, bool currentButtonValue,
                   uint8_t midiChannel, bool isLinear);

bool setupRunning = false;
SetupMenu menu;


//method used to burn the config once
void burnConfig(Eeprom& eeprom)
{
  ElementHardwareConfig hw;
  ElementUserConfig user;

  uint16_t addr = 0;
  uint16_t writeSize = 0;

  hw.faderNum = 7;
  hw.buttonNum = 8;
  hw.displayNum = 0;
  user.midiChannel = 0;
  user.faderLinear = false;
  strcpy(user.text, "Unused");
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;


//  printf("reading bytes: \n");
//  for(int i = 0; i < addr; ++i)
//  {
//    uint8_t data = 42;
//    if(HAL_OK != eeprom.readByte(i,data))
//    {
//      printf("READ ERROR\n");
//      return;
//    }
//    printf("%d ", data);
//  }
//  printf("\n");




  hw.faderNum = 6;
  hw.buttonNum = 7;
  hw.displayNum = 1;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 5;
  hw.buttonNum = 6;
  hw.displayNum = 2;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 4;
  hw.buttonNum = 5;
  hw.displayNum = 3;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 0;
  hw.buttonNum = 4;
  hw.displayNum = 4;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 1;
  hw.buttonNum = 3;
  hw.displayNum = 5;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  hw.faderNum = 2;
  hw.buttonNum = 2;
  hw.displayNum = 6;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;


  hw.faderNum = 3;
  hw.buttonNum = 1;
  hw.displayNum = 7;
  ++user.midiChannel;
  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &hw, sizeof(ElementHardwareConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;

  if(HAL_ERROR == eeprom.writeObjectWithCrc(addr, &user, sizeof(ElementUserConfig), writeSize))
  {
    printf("config write error!!!\n");
    return;
  }
  addr += writeSize;



  //read and print config for human validation
  ElementHardwareConfig hwRead;
  ElementUserConfig userRead;
  uint16_t readAddr = 0;
  uint16_t readSize = 0;


  for(int i = 0; i < 8; ++i)
  {
    if(HAL_OK == eeprom.readObjectWithCrc(readAddr, &hwRead, sizeof(ElementHardwareConfig), readSize))
    {
      readAddr += readSize;
      printf("Reading Element %d\n",i);
      DUMP_VAR(hwRead.buttonNum);
      DUMP_VAR(hwRead.displayNum);
      DUMP_VAR(hwRead.faderNum);
      if(HAL_OK == eeprom.readObjectWithCrc(readAddr, &userRead, sizeof(ElementUserConfig), readSize))
      {
        readAddr += readSize;
        DUMP_VAR(userRead.faderLinear);
        DUMP_VAR(userRead.midiChannel);
        printf("Text: %s\n", userRead.text);
      }
      else
      {
        printf("READ ERROR!!\n");
        return;
      }
    }
    else
    {
      printf("READ ERROR ELEMENT %d\n", i);
      return;
    }
    printf("-----------------------\n");
  }
}


int main(void)
{
  initHardware();

  Eeprom eeprom(&hi2c1);

  //use this code to initially burn the config
 // printf("burning config\n");
 // burnConfig(eeprom);
 // printf("done\n");
 // printf("HALT\n");
 // while(1);

  Elements::init(eeprom);

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
  resetDisplays(display);

  while(1)
  {

    //FIXME not debounced?!
    const bool setupButtonPressed = Buttons::pressed[SETUP_BUTTON_NUM];

    if(setupButtonPressed || setupRunning)
    {
      setupDisplayLoop(display, eeprom);
    }
    else
    {
      mainDisplayLoop(display);
    }
  }
}


/** special display loop for setup */
void setupDisplayLoop(Adafruit_SSD1306& display, Eeprom& eeprom)
{
  static uint8_t setupElement = 0;
  static uint32_t setupEnterTime = 0;
  static bool buttonPressed = false;

  if(setupRunning)
  {
    if(menu.done())
    {
      setupRunning = false;
      Elements::storeElement(setupElement, eeprom);
      resetDisplays(display);
      return;
    }

    //allow user to abort setup after 2 seconds
    if((Buttons::pressed[SETUP_BUTTON_NUM] && HAL_GetTick() - setupEnterTime > 2000))
    {
      menu.abort();
      Elements::storeElement(setupElement, eeprom);
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

/** The main display loop. Draws fader values etc. */
void mainDisplayLoop(Adafruit_SSD1306& display)
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

