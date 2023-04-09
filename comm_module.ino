//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================
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
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActIfReceivedMessage() {
#ifdef UseCOMM
  if (commProto.available()) {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    memset(payload, 48, PAYLOAD_SIZE);  // all 'zero' character
    payload[PAYLOAD_SIZE - 1] = '\0';

    commProto.rxObj(payload);

#ifdef DEBUG
    Serial.println();
    Serial.println(payload);
    Serial.println();
#endif

    for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
      byte int_as_char_size = 4;
      char valueString[int_as_char_size + 1];
      memcpy(valueString, &payload[pinId * int_as_char_size], int_as_char_size);
      voltage[pinId] = atoi(valueString);
    }

    publishVoltageDataToMqtt();
  }
#endif
}
//==================================================================================================