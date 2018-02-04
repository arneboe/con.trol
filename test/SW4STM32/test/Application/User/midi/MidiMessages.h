#pragma once
#include <stdint.h>


struct MidiMessage
{
  uint8_t virtualCable; // range [0x0 .. 0xF]
  uint8_t channel; // range [0x0 .. 0xF]
  MidiMessage(uint8_t virtualCable, uint8_t channel) : virtualCable(virtualCable), channel(channel) {}
};

struct CCMessage : public MidiMessage
{
  uint8_t controlChannel; // range [0 .. 119]
  uint8_t value; // range [0 .. 127]
  CCMessage() : MidiMessage(0, 0), controlChannel(0), value(0) {}
  CCMessage(uint8_t virtualCable, uint8_t channel, uint8_t controlChannel, uint8_t value) :
    MidiMessage(virtualCable, channel), controlChannel(controlChannel), value(value) {}

  /** @return False in case of error */
  bool toBuffer(uint8_t* buffer, uint8_t bufferSize) const;

};


struct NoteMessage : public MidiMessage
{
  uint8_t note;
  uint8_t velocity;
  bool on;

  NoteMessage() : MidiMessage(0, 0), note(0), velocity(0), on(false) {}
  NoteMessage(uint8_t virtualCable, uint8_t channel, uint8_t note,
              uint8_t velocity, bool on) : MidiMessage(virtualCable, channel), note(note),
              velocity(velocity), on(on) {}

  /** @return False in case of error */
  bool toBuffer(char* buffer, uint8_t bufferSize) const;
};
