#include "Button.h"
#include "stm32f1xx_hal.h"

bool Buttons::pressed[NUM_BUTTONS];


void Buttons::update()
{

  pressed[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) != GPIO_PIN_SET;

  pressed[1] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) != GPIO_PIN_SET;
  pressed[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) != GPIO_PIN_SET;
  pressed[3] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) != GPIO_PIN_SET;
  pressed[4] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) != GPIO_PIN_SET;
  pressed[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) != GPIO_PIN_SET;
  pressed[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) != GPIO_PIN_SET;
  pressed[7] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) != GPIO_PIN_SET;
  pressed[8] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) != GPIO_PIN_SET;
}
