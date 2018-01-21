#pragma once
#include <stdbool.h>
#include <stdint.h>


typedef struct
{
	uint8_t virtualCable; // range [0x0 .. 0xF]
	uint8_t channel; // range [0x0 .. 0xF]
	uint8_t controlChannel; // range [0 .. 119]
	uint8_t value; // range [0 .. 127]
} CCMessage;



bool CCMessage_toBuffer(const CCMessage* msg, uint8_t* buffer, uint8_t bufferSize);
