#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "midi/MidiMessages.h"
#include "midi/Midi.h"
#include "fader/Fader.h"
#include "hardware/ErrorHandler.h"
#include "display/Adafruit_SSD1306.h"
#include "hardware/Hardware.h"

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

int main(void)
{

  initHardware();

  tcaselect(0);

  Adafruit_SSD1306 display(hi2c1);
  display.begin(SSD1306_SWITCHCAPVCC, 0x78);
  display.fillScreen(BLACK);
  display.fillHeader(WHITE);
  display.display();

  display.waitForReady();

  tcaselect(1);
  display.waitForReady();
  display.begin(SSD1306_SWITCHCAPVCC, 0x78);
  display.display();

  display.waitForReady();


  //the main just updates the displays.
  //everything else is done in interrupts
  uint8_t lastMidiValues[NUM_FADERS] = {255};
  while (1)
  {

    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const uint8_t currentValue = Faders::faders[i].midiValue;
      if(currentValue != lastMidiValues[i])
      {
        lastMidiValues[i] = currentValue;
        tcaselect(i);
        display.fillHeader(BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println(currentValue);
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
    //update midi

    //message buffer is reused every time
    static CCMessage messages[NUM_FADERS];


    //used to remember the last values and avoid spam,
    //255 can never happen in midi thus it is a good initial value
    static uint8_t lastValues[NUM_FADERS] = {255};

    uint8_t nextMessageIndex = 0;

    for(int i = 0; i < NUM_FADERS; ++i)
    {
      const uint8_t value = Faders::faders[i].midiValue;

      if(value != lastValues[i])
      {
        lastValues[i] = value;
        messages[nextMessageIndex].controlChannel = i;
        messages[nextMessageIndex].value = value;
        ++nextMessageIndex;
      }
    }

    //if anything changed, send it
    if(nextMessageIndex > 0)
    {
      Midi::sendCC(messages, nextMessageIndex);
    }
  }
}

