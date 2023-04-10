//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================


//= VARIABLES ======================================================================================
SerialTransfer commProto;

char payload[PAYLOAD_SIZE];

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
bool comm_ActIfReceivedMessage() {
  bool hasUpdate = false;
#ifdef UseCOMM
  if (commProto.available()) {
    memset(payload, '0', PAYLOAD_SIZE);  // all 'zero' character
    payload[PAYLOAD_SIZE - 1] = '\0';    // end with array terminator
    //
    commProto.rxObj(payload);
    //
    _printPayloadIfDebug();
    //
    _payload_To_VoltageData();
    //
    hasUpdate = true;
  }
#endif
  return hasUpdate;
}
//==================================================================================================
void _payload_To_VoltageData() {
  char valueString[INT_AS_CHAR_SIZE + 1];
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    memcpy(valueString, &payload[pinId * INT_AS_CHAR_SIZE], INT_AS_CHAR_SIZE);
    voltage[pinId] = atoi(valueString);
  }
  // read Vcc from payload
  memcpy(valueString, &payload[ANALOG_PIN_COUNT * INT_AS_CHAR_SIZE], INT_AS_CHAR_SIZE);
  voltage_supply = atoi(valueString);
}
//==================================================================================================
void _printPayloadIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print("payload = [");
  Serial.print(payload);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================