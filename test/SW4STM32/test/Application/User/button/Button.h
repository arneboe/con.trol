#pragma once

#define NUM_BUTTONS 9

struct Buttons
{
  static bool pressed[NUM_BUTTONS];

  static void update();
};
