//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================
//----------------------------------

const int PAYLOAD_SIZE = ANALOG_PIN_COUNT * 4 + 1;

//= VARIABLES ======================================================================================
SerialTransfer commProto;

char payload[PAYLOAD_SIZE] = "00000000000000000000000000000000";

//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMM
#ifdef DEBUG
  Serial.println("COMM:Setup >>>");
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  commProto.begin(Serial);
  //..............................
#ifdef DEBUG
  Serial.println("COMM:Setup <<<");
#endif
  delay(50 * TIME_TICK);
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMM
  memset(payload, 48, PAYLOAD_SIZE);  // all 'zero' character
  payload[PAYLOAD_SIZE - 1] = '\0';

  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    byte int_as_char_size = 4;
    char valueString[int_as_char_size + 1];
    sprintf(valueString, "%04d", voltage[pinId]);
    memcpy(&payload[pinId * int_as_char_size], valueString, int_as_char_size);
  }

#ifdef DEBUG
  Serial.println();
  Serial.print("payload = [");
  Serial.print(payload);
  Serial.print("]");
  Serial.println();
#endif

  commProto.sendDatum(payload);
#endif
}
//==================================================================================================