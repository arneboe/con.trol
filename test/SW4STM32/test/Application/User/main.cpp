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

struct Index
{
  uint8_t tca;
  uint8_t fader;
  uint8_t button;
  uint8_t midiChannel;
};

Index indexMap[NUM_FADERS];


void initIndexMap()
{
  indexMap[0].tca = 2;
  indexMap[0].fader = 7;
  indexMap[0].button = 0;
  indexMap[0].midiChannel = 10;

  indexMap[1].tca = 3;
  indexMap[1].fader = 6;
  indexMap[1].button = 3;
  indexMap[0].midiChannel = 11;

  indexMap[2].tca = 5;
  indexMap[2].fader = 5;
  indexMap[2].button = 2;
  indexMap[0].midiChannel = 12;

  indexMap[3].tca = 4;
  indexMap[3].fader = 4;
  indexMap[3].button = 6;
  indexMap[0].midiChannel = 13;

  indexMap[4].tca = 6;
  indexMap[4].fader = 3;
  indexMap[4].button = 7;
  indexMap[0].midiChannel = 14;

  indexMap[5].tca = 7;
  indexMap[5].fader = 2;
  indexMap[5].button = 5;
  indexMap[0].midiChannel = 15;

  indexMap[6].tca = 1;
  indexMap[6].fader = 1;
  indexMap[6].button = 1;
  indexMap[0].midiChannel = 16;

  indexMap[7].tca = 0;
  indexMap[7].fader = 0;
  indexMap[7].button = 8;
  indexMap[0].midiChannel = 17;

  //button index 4 is program button

}

int main(void)
{
  initIndexMap();
  initHardware();

  Adafruit_SSD1306 display(hi2c1);
  for(int i = 0; i < 8; ++i)
  {
    tcaselect(i);
    display.begin(SSD1306_SWITCHCAPVCC, 0x78);
    display.fillScreen(BLACK);
    display.fillHeader(WHITE);
    display.display();
    display.waitForReady();
  }

  //the main just updates the displays.
  //everything else is done in interrupts
  uint8_t lastMidiValues[NUM_FADERS] = {255};
  bool lastButtons[NUM_BUTTONS] = {false};
  while (1)
  {
    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const uint8_t faderI = indexMap[i].fader;
      const uint8_t buttonI = indexMap[i].button;
      const uint8_t tcaI = indexMap[i].tca;

      const uint8_t currentValue = Faders::faders[faderI].midiValue;
      const bool currentButtonValue = Buttons::pressed[buttonI];
      if(currentValue != lastMidiValues[i] || currentButtonValue != lastButtons[i])
      {
        lastMidiValues[i] = currentValue;
        lastButtons[i] = currentButtonValue;
        tcaselect(tcaI);
        display.fillHeader(BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println(currentValue);

        if(currentButtonValue)
          display.fillCircle(50, 5, 5, WHITE);

        if(!display.display())
        {
          printf("ERROR: Display %d did not respond\n", i);
        }
        if(!waitForI2cReady(50))
        {
          printf("ERROR: Display %d failed\n", i);
        }
      }
    }
  }
}


extern "C"
{
  //handle midi messages at 1000hz
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {

    //read button states, doesn't take any time so just do it here
    Buttons::update();

    //update midi

    //message buffer is reused every time
    static CCMessage ccMessages[NUM_FADERS];


    //used to remember the last values and avoid spam,
    //255 can never happen in midi thus it is a good initial value
    static uint8_t lastValues[NUM_FADERS] = {255};

    uint8_t nextMessageIndex = 0;

    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const uint8_t faderI = indexMap[i].fader;
      const uint8_t value = Faders::faders[faderI].midiValue;

      if(value != lastValues[i])
      {
        lastValues[i] = value;
        ccMessages[nextMessageIndex].controlChannel = i;
        ccMessages[nextMessageIndex].value = value;
        ++nextMessageIndex;
      }
    }

    //if anything changed, send it
    if(nextMessageIndex > 0)
    {
      Midi::sendCC(ccMessages, nextMessageIndex);
    }
  }
}

