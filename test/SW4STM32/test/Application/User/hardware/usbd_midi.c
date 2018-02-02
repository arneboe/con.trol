/* simple USB Midi IN device driver */
/* inspired from mbed implementation */
/* TODO: sysex support */

#include "usbd_midi.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

static uint8_t  USBD_Midi_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);
static uint8_t  USBD_Midi_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx);
static uint8_t  USBD_Midi_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);
static uint8_t  *USBD_Midi_GetCfgDesc (uint16_t *length);
static uint8_t  *USBD_Midi_GetDeviceQualifierDesc (uint16_t *length);
static uint8_t  USBD_Midi_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_Midi_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_Midi_EP0_RxReady (USBD_HandleTypeDef *pdev);
static uint8_t  USBD_Midi_EP0_TxReady (USBD_HandleTypeDef *pdev);
static uint8_t  USBD_Midi_SOF (USBD_HandleTypeDef *pdev);
static uint8_t  USBD_Midi_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_Midi_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);


USBD_ClassTypeDef  USBD_Midi_ClassDriver =
{
  USBD_Midi_Init,
  USBD_Midi_DeInit,
  USBD_Midi_Setup,
  USBD_Midi_EP0_TxReady,
  USBD_Midi_EP0_RxReady,
  USBD_Midi_DataIn,
  USBD_Midi_DataOut,
  USBD_Midi_SOF,
  USBD_Midi_IsoINIncomplete,
  USBD_Midi_IsoOutIncomplete,
  USBD_Midi_GetCfgDesc,
  USBD_Midi_GetCfgDesc,
  USBD_Midi_GetCfgDesc,
  USBD_Midi_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif

static uint8_t USBD_Midi_CfgDesc[USB_MIDI_CONFIG_DESC_SIZ] =
{
			// configuration descriptor
            0x09, //bLength
            USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION, //bDescriptorType
            LOBYTE(USB_MIDI_CONFIG_DESC_SIZ), //wTotalLength
            HIBYTE(USB_MIDI_CONFIG_DESC_SIZ), //wTotalLength
            0x02, //bNumInterfaces
            0x01, //id of this configuration
            0x00, //unused
            0x80, //bus powered device
            0x50, //max power 160mA
            // Standard AC Interface Descriptor
			//mandatory even if there is no audio support
            0x09, //blength - size of this descriptor in bytes
            0x04, //bDescriptorType - INTERFACE descriptor
            0x00, //bInterfaceNumber - index if this interface
            0x00, //bAlternateSetting - Index of this setting
            0x00, //bNumEndpoints - number of audio endpoints
            0x01, //bInterfaceClass - AUDIO
            0x01, //bInterfaceSubclass - AUDIO_CONTROL
            0x00, //unused
            0x00, //unused
			// Class-specific AC Interface Descriptor
            0x09, //bLength
            0x24, //bDescriptorType - CS_INTERFACE
            0x01, //bDescriptorSubtype - HEADER subtype
            0x00, //bcdAC - revision of class spec 1.0 ,low byte
            0x01, //bcdAC - revision of class spec 1.0, high byte
            0x09, //wTotalLength - Total size of class specific descriptors
            0x00, //wTotalLength high byte
            0x01, //binCollection - number of streaming interfaces
            0x01, //baInterfaceNr - MIDIStreaming interface 1 belongs to this AudioContrl INterfac
			// MIDIStreaming Interface Descriptors
            0x09, //bLength
            0x04, //bDescriptorType - INTERFACE descriptor
            0x01, //bInterfaceNumber - index of this interface
            0x00, //bAlternateSetting - Index of this alternate setting
            0x02, //bNumEndpoints - number of endpoints
            0x01, //bInterfaceClass - AUDIO
            0x03, //bInterfaceSubclass - MIDISTREAMING
            0x00, //unused
            0x00, //unused
			// Class-Specific MS Interface Header Descriptor
            0x07, //bLength
            0x24, //bDescriptorType - CS_INTERFACE
            0x01, //bDescriptorSubtype - MS_HEADER subtype
            0x00, //bcdADC - Revision of this class spec, low byte
            0x01, //bcdADC - Revision of this class spec, high byte
            0x41, //wTotalLength - Total size of class specific descriptors low byte
            0x00, //wTotalLength - high byte
            // MIDI IN JACKS
            0x06, //bLength
            0x24, //bDescriptorType - CS_INTERFACE
            0x02, //bDescriptorSubtype - MIDI_IN_JACK
            0x01, //bJackType - EMBEDDED
            0x01, //bJackId
            0x00, //iJack - Unused

            0x06, //bLength
            0x24, //CS_INTERFACE
            0x02, //MIDI_IN_JACK
            0x02, //bjacKType - EXTERNAL
            0x02, //bjackId
            0x00, //iJack - unused

            // MIDI OUT JACKS
            0x09, //bLength
            0x24, //CS_INTERFACE
            0x03, //MIDI_OUT_JACK
            0x01, //EMBEDDED
            0x03, //bJackId
            0x01, //bNrInputPins - number of input pins of this jack
            0x02, //BaSourceID
            0x01, //BaSourcePin
            0x00, //Unused

            0x09, //bLength
            0x24, //CS_INTERFACE
            0x03, //MIDI_OUT_JACK
            0x02, //EXTERNAL
            0x06, //bJackId //FIXME why 0x6 instead 0f 0x4?
            0x01,
            0x01,
            0x01,
            0x00,

            // OUT endpoint descriptor
			//Standard bulk out endpoint descriptor
            0x09, //bLength
            0x05, //bdescriptorType - ENDPOINT descriptor
            MIDI_OUT_EP, //bEndpointAdress
            0x02, //bmAttributes - Bulk, not shared
            0x40, //wMaxPacketSize 64 bytes, low byte
            0x00, //wMaxPacketSize, high byte
            0x00, //bInterval - unused for bulk
            0x00, //bRefresh - unused
            0x00, //bSynchAddress - unused
			//class specific ms bulk out endpoint descriptor
            0x05, //bLength
            0x25, //CS_ENDPOINT
            0x01, //MS_GENERAL subtype
            0x01, //number of embedded midi in jacks
            0x01, //id of the embedded midi in jack

            // IN endpoint descriptor
			//standard bulk in endpoint descriptor
            0x09, //bLength
            0x05, //ENDPOINT
            MIDI_IN_EP,
            0x02, //bulk, not shared
            0x40, //wMaxPacketSize low byte
            0x00, //wMaxPacketSize high byte
            0x00, //ignred
            0x00, //unused
            0x00, //unused
			//class specific ms bulk in endpoint descriptor
            0x05, //bLength
            0x25, //CS_ENDPOINT
            0x01, //MS_GENERAL subtype
            0x01, //number of embedded midi out jacks
            0x03, //id of the embedded midi out jack
  };

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
static uint8_t USBD_Midi_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
  //FIXME why is this shorter?!
  USB_LEN_DEV_QUALIFIER_DESC, //bLength
  USB_DESC_TYPE_DEVICE_QUALIFIER, //bDescriptorType
  0x00, //bcdUSB - usb version low byte
  0x02, //bcdUSB - high byte
  0x00, //bDeviceClass - Device defined at interface level
  0x00, //unused
  0x00, //unused
  0x40, //bMaxPacketSize0
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_Midi_Private_Functions
  * @{
  */

/**
  * @brief  USBD_Midi_Init
  *         Initialize the Midi interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_Midi_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{

  pdev->pClassData = USBD_malloc(sizeof (USBD_Midi_HandleTypeDef));

  if (pdev->pClassData == NULL)
  {
    return USBD_FAIL;
  }
  else
  {
   USBD_Midi_HandleTypeDef *hmidi = (USBD_Midi_HandleTypeDef*) pdev->pClassData;

  /* Open the in EP */
  USBD_LL_OpenEP(pdev,
                MIDI_IN_EP,
                USBD_EP_TYPE_BULK,
                MIDI_DATA_IN_PACKET_SIZE
                );

  /* Open the out EP */
  USBD_LL_OpenEP(pdev,
          MIDI_OUT_EP,
          USBD_EP_TYPE_BULK,
          MIDI_DATA_OUT_PACKET_SIZE
          );

  /* Prepare Out endpoint to receive next packet */
  USBD_LL_PrepareReceive(pdev,
                     MIDI_OUT_EP,
                     hmidi->rxBuffer,
                     MIDI_DATA_OUT_PACKET_SIZE);

  return USBD_OK;
  }
}

void writeMidi(USBD_HandleTypeDef *pdev)
{
	const uint8_t virtualCable = 0;
	const uint8_t channel = 0;
	const uint8_t controlChannel = 12;
	const uint8_t value = 42;

	//                             cable | 0xB
	const uint8_t usbFrame = (virtualCable << 4) | 0x0B;

	//MIDI command for CC message: 4 bit | 4 bit
	//                             0xB   | midi channel
	const uint8_t midiCommand = 0xB0 | (channel > 0xF ? 0xF : channel);

	uint8_t buffer[12];

	//just random shit so i can see something on the midi receiver
	buffer[0] = usbFrame;
	buffer[1] = midiCommand;
	buffer[2] = controlChannel > 119 ? 119 : controlChannel;
	buffer[3] = value > 127 ? 127 : value;

	buffer[4] = usbFrame;
	buffer[5] = midiCommand;
	buffer[6] = controlChannel > 119 ? 119 : controlChannel;
	buffer[7] = value > 127 ? 127 : value;

	buffer[8] = usbFrame;
	buffer[9] = midiCommand;
	buffer[10] = controlChannel > 119 ? 119 : controlChannel;
	buffer[11] = value > 127 ? 127 : value;

    switch(USBD_LL_Transmit (pdev, MIDI_IN_EP,  buffer, 8))
    {
    case USBD_OK:
    	printf("ok");
		break;
    case USBD_FAIL:
    	printf("fail\n");
		break;
    case USBD_BUSY:
    	printf("busy\n");
    	break;
    default:
		printf("default\n");

    }
}

/**
  * @brief  USBD_Midi_Init
  *         DeInitialize the Midi layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_Midi_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
  USBD_LL_CloseEP(pdev,
      MIDI_IN_EP);
  USBD_LL_CloseEP(pdev,
      MIDI_OUT_EP);
  return USBD_OK;
}

/**
  * @brief  USBD_Midi_Setup
  *         Handle the Midi specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_Midi_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {

    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL;
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {

    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL;
    }
  }
  return USBD_OK;
}


/**
  * @brief  USBD_Midi_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_Midi_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_Midi_CfgDesc);
  return USBD_Midi_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_Midi_DeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_Midi_DeviceQualifierDesc);
  return USBD_Midi_DeviceQualifierDesc;
}


/**
  * @brief  USBD_Midi_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_Midi_DataIn (USBD_HandleTypeDef *pdev,
                              uint8_t epnum)
{

  return USBD_OK;
}

/**
  * @brief  USBD_Midi_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_Midi_EP0_RxReady (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_Midi_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_Midi_EP0_TxReady (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_Midi_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_Midi_SOF (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_Midi_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_Midi_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_Midi_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_Midi_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_Midi_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_Midi_DataOut (USBD_HandleTypeDef *pdev,
                              uint8_t epnum)
{
   USBD_Midi_HandleTypeDef *hmidi = (USBD_Midi_HandleTypeDef*) pdev->pClassData;

  /* Get the received data buffer and update the counter */

//  USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;
   hmidi->rxLen = USBD_LL_GetRxDataSize (pdev, epnum);
  /* Forward the data to the user callback. */
//  ((MIDI_IF_Prop_TypeDef *)pdev->pUserData)->pIf_MidiRx((uint8_t*)&USB_Rx_Buffer, USB_Rx_Cnt);
//  APP_fops->pIf_MidiRx((uint8_t*)&USB_Rx_Buffer, USB_Rx_Cnt);

/*  switch(hmidi->rxBuffer[0])
  {
    case 0x2:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0xf:
      break;
    default:
      ((USBD_Midi_ItfTypeDef *)pdev->pUserData)->Receive(hmidi->rxBuffer, hmidi->rxLen);
      break;
  }
  */

uint8_t *buf = hmidi->rxBuffer;

for (uint32_t i=0; i<hmidi->rxLen; i+=4) {
  ((USBD_Midi_ItfTypeDef *)pdev->pUserData)->Receive(buf+i, 4);
}

//  ((USBD_Midi_ItfTypeDef *)pdev->pUserData)->Receive(hmidi->rxBuffer,hmidi->rxLen);

  USBD_LL_PrepareReceive(pdev,
                       MIDI_OUT_EP,
                       hmidi->rxBuffer,
                       MIDI_DATA_OUT_PACKET_SIZE);

  return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_Midi_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_Midi_DeviceQualifierDesc);
  return USBD_Midi_DeviceQualifierDesc;
}



/**
* @brief  USBD_CDC_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: CD  Interface callback
  * @retval status
  */
uint8_t  USBD_Midi_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                      USBD_Midi_ItfTypeDef *fops)
{
  uint8_t  ret = USBD_FAIL;

  if(fops != NULL)
  {
    pdev->pUserData= fops;
    ret = USBD_OK;
  }

  return ret;
}
