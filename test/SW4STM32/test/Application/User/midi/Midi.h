#pragma once
#include "MidiMessages.h"

class Midi
{
public:

  /** Sends up to 16 messages in bulk.
   *  @warning messages.size() will be clamped to 16.
   *  //FIXME should be able to send 19 messages not 16 leftover from old opencm3 code, test if this is better now
   */
  static void sendCC(const CCMessage* messages, const uint8_t numMessages);



  /** Sends a note on/off message.
    * Calls errorHandler in case of error
    * @warning ranges are enforced by clamping, no waring is generated
    * @note update has to be called after every send. otherwise messages wil get lost.
    *       If you need to send more than one msg, use the bulk version below.
    * @return False in case of error. */
  static bool sendNote(const NoteMessage& msg);
  static bool sendNote(const NoteMessage* messages, const uint8_t numMessages);
};


