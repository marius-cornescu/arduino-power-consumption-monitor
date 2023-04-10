#ifndef _HEADERFILE_COMMON
#define _HEADERFILE_COMMON
//= CONSTANTS ======================================================================================
#define SW_VERSION "2023.04.10"
//------------------------------------------------
//------------------------------------------------
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000L  // 1 second

#ifdef DEBUG
#define TIME_TICK 40L
#else
#define TIME_TICK 10L
#endif

//------------------------------------------------
// Arduino NANO has 8 analog pins
const byte ANALOG_PIN_COUNT = 8;
// Number of bytes to use to represent the int value (voltage between 0 - 9999)
const byte INT_AS_CHAR_SIZE = 4;
// Size of the communication payload (4 byte per port) + Vcc data
const int PAYLOAD_SIZE = (ANALOG_PIN_COUNT + 1) * INT_AS_CHAR_SIZE + 1;

//------------------------------------------------
//==================================================================================================
#endif  // _HEADERFILE_COMMON
