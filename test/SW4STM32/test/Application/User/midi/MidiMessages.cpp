#include "MidiMessages.h"


bool CCMessage_toBuffer(const CCMessage* msg, uint8_t* buffer, uint8_t bufferSize)
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
	const uint8_t usbFrame = (msg->virtualCable << 4) | 0x0B;

	//MIDI command for CC message: 4 bit | 4 bit
	//                             0xB   | midi channel
	const uint8_t midiCommand = 0xB0 | (msg->channel > 0xF ? 0xF : msg->channel);

	//FIXME there might be a faster way to clamp the values
	buffer[0] = usbFrame;
	buffer[1] = midiCommand;
	buffer[2] = msg->controlChannel > 119 ? 119 : msg->controlChannel;
	buffer[3] = msg->value > 127 ? 127 : msg->value;
	return true;

}
