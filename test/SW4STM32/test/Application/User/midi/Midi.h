#pragma once
#include "MidiMessages.h"
#include <stdbool.h>

/** Sends up to 16 messages in bulk.
*  @warning numMessages will silently be clamped to 16.
*/
void Midi_sendCC(const CCMessage* messages, const uint8_t numMessages);
