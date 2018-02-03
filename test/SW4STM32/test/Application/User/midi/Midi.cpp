#include "Midi.h"
#include "hardware/usbd_midi.h"
#include "usb_device.h"

void Midi_sendCC(const CCMessage* messages, const uint8_t numMessages)
{
	static uint8_t buffer[64];

	const int size = numMessages > 16 ? 16 : numMessages;
	for(int i = 0; i < size; ++i)
	{
		CCMessage_toBuffer(&messages[i], buffer + 4 * i, 4);
	}

	const int usedBufferSize = size * 4;

	printf("size: %d: ", usedBufferSize);
	for(int i = 0; i < usedBufferSize; ++i)
	{
		printf("%d ", buffer[i]);
	}
	printf("\n");

    switch(USBD_LL_Transmit(&hUsbDeviceFS, MIDI_IN_EP,  buffer, usedBufferSize))
    {
    //FIXME use errorhandler!
    case USBD_OK:
    	printf("ok\n");
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
