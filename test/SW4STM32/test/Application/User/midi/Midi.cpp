#include "Midi.h"
#include "hardware/usbd_midi.h"
#include "usb_device.h"


/*
bool Midi::sendNote(const NoteMessage& msg)
{
  //FIXME duplicate code from sendCC, would introduce a vtable?!
  char buffer[4];
  msg.toBuffer(buffer, 4);
  if(usbd_ep_write_packet(usbd_dev, 0x81, buffer, 4) != 4)
  {
    errorHandler(USB_WRITE_ERROR);
    return false;
  }
  return true;
}

*/

/*
bool Midi::sendNote(const NoteMessage* messages, const uint8_t numMessages)
{
  //FIXME duplicate code from sendCC
  if(numMessages == 0) return true;

  char buffer[64];

  const int size = numMessages > 16 ? 16 : numMessages;
  for(int i = 0; i < size; ++i)
  {
    messages[i].toBuffer(buffer + 4 * i, 4);
  }

  const int usedBufferSize = size * 4;
  if(usbd_ep_write_packet(usbd_dev, 0x81, buffer, usedBufferSize) != usedBufferSize)
  {
    errorHandler(USB_WRITE_ERROR);
    return false;
  }
  return true;
}
*/



void Midi::sendMessages(MidiMessage** messages, const uint8_t numMessages)
{
  if(numMessages == 0) return;

  uint8_t buffer[64];

  const int size = numMessages > 16 ? 16 : numMessages;
  for(int i = 0; i < size; ++i)
  {
    messages[i]->toBuffer(buffer + 4 * i, 4);
  }

  const uint8_t usedBufferSize = size * 4;

  switch(USBD_LL_Transmit(&hUsbDeviceFS, MIDI_IN_EP,  buffer, usedBufferSize))
  {
  //FIXME use errorhandler!
  case USBD_OK:
    //printf("ok\n");
  break;
  case USBD_FAIL:
    printf("USB SEND FAIL\n");
  break;
  case USBD_BUSY:
    printf("USB SEND BUS\n");
    break;
  default:
  printf("USB SEND ERROR DEFAULT CASE\n");

  }
}

void Midi::sendCC(const CCMessage* messages, const uint8_t numMessages)
{
  if(numMessages == 0) return;

  uint8_t buffer[64];

  const int size = numMessages > 16 ? 16 : numMessages;
  for(int i = 0; i < size; ++i)
  {
    messages[i].toBuffer(buffer + 4 * i, 4);
  }

  const uint8_t usedBufferSize = size * 4;

  switch(USBD_LL_Transmit(&hUsbDeviceFS, MIDI_IN_EP,  buffer, usedBufferSize))
  {
  //FIXME use errorhandler!
  case USBD_OK:
    //printf("ok\n");
  break;
  case USBD_FAIL:
    printf("USB SEND FAIL\n");
  break;
  case USBD_BUSY:
    printf("USB SEND BUS\n");
    break;
  default:
  printf("USB SEND ERROR DEFAULT CASE\n");

  }
}

