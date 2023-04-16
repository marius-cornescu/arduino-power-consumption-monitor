//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "CommCommon.h"
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================


//= VARIABLES ======================================================================================
SerialTransfer commProto;

//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMM
#ifdef DEBUG
  Serial.println(F("COMM:Setup >>>"));
#endif
  //..............................
  // Open serial communications
  Serial.begin(115200);
  commProto.begin(Serial);
  //..............................
#ifdef DEBUG
  Serial.println(F("COMM:Setup <<<"));
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
    __printPayloadIfDebug();
    //
    _Payload_To_Data();
    //
    __printDataIfDebug();
    //
    hasUpdate = true;
  }
#endif
  return hasUpdate;
}
//==================================================================================================
void _Payload_To_Data() {
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
void __printPayloadIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print(F("payload = ["));
  Serial.print(payload);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================
void __printDataIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print(F("voltage_supply = ["));
  Serial.print(voltage_supply);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================