#include "MidiMessages.h"

bool CCMessage::toBuffer(uint8_t* buffer, uint8_t bufferSize) const
{
  //FIXME throw some error or something?!
  if(bufferSize < 4)
    return false;
    //see chapter 4 "USB-MIDI Event Packets" in
  // "Universal Serial Bus Device Class Definition for MIDI Devices"
  // http://www.usb.org/developers/docs/devclass_docs/midi10.pdf

  //virtualCable is clamped by shifting (see below)
  // USB framing for CC message: 4 bit | 4bit
  //                             cable | 0xB
  const uint8_t usbFrame = (virtualCable << 4) | 0x0B;

  //MIDI command for CC message: 4 bit | 4 bit
  //                             0xB   | midi channel
  const uint8_t midiCommand = 0xB0 | (channel > 0xF ? 0xF : channel);

  //FIXME there might be a faster way to clamp the values
  buffer[0] = usbFrame;
  buffer[1] = midiCommand;
  buffer[2] = controlChannel > 119 ? 119 : controlChannel;
  buffer[3] = value > 127 ? 127 : value;
  return true;
}

bool NoteMessage::toBuffer(uint8_t* buffer, uint8_t bufferSize) const
{
  //FIXME throw some error or something?!
  if(bufferSize < 4)
    return false;

  uint8_t usbFrame = (virtualCable << 4);
  uint8_t midiCommand = (channel > 0xF ? 0xF : channel);

  if(on)
  {
    usbFrame |= 0x09;
    midiCommand |= 0x90;

  }
  else
  {
    usbFrame |= 0x08;
    midiCommand |= 0x80;
  }

  buffer[0] = usbFrame;
  buffer[1] = midiCommand;
  buffer[2] = note > 127 ? 127 : note;
  buffer[3] = velocity > 127 ? 127 : velocity;

  return true;
}
