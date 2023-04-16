#pragma once
#ifndef _HEADERFILE_COMM_COMMON
#define _HEADERFILE_COMM_COMMON
//= CONSTANTS ======================================================================================
// Arduino NANO has 8 analog pins
const byte ANALOG_PIN_COUNT = 8;
// Number of bytes to use to represent the int value (voltage between 0 - 9999)
const byte INT_AS_CHAR_SIZE = 4;
// Size of the communication payload (4 byte per port) + Vcc data
const int PAYLOAD_SIZE = (ANALOG_PIN_COUNT + 1) * INT_AS_CHAR_SIZE + 1;

//= VARIABLES ======================================================================================
//
char payload[PAYLOAD_SIZE];
//------------------------------------------------
//==================================================================================================
#endif  // _HEADERFILE_COMM_COMMON
