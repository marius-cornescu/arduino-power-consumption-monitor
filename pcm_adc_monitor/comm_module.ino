//= DEFINES ========================================================================================
#ifdef UseCOMMPro

//= INCLUDES =======================================================================================
#include "CommCommon.h"
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================

//= VARIABLES ======================================================================================
SerialTransfer commProto;

#endif
//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMMPro
#ifdef DEBUG
  Serial.println(F("COMM:Setup >>>"));
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  commProto.begin(Serial);
  //..............................
  delay(1 * TIME_TICK);
#ifdef DEBUG
  Serial.println(F("COMM:Setup <<<"));
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMMPro
  memset(payload, '0', PAYLOAD_SIZE);  // all 'zero' character
  payload[PAYLOAD_SIZE - 1] = '\0';    // end with array terminator
  //
  _Data_To_Payload();
  //
  __printPayloadIfDebug();
  //
  commProto.sendDatum(payload);
#endif
}
//==================================================================================================
//==================================================================================================
bool comm_ActIfReceivedMessage() {
  bool hasUpdate = false;
#ifdef UseCOMMPro
#endif
  return hasUpdate;
}
//==================================================================================================
#ifdef UseCOMMPro
//==================================================================================================
void _Data_To_Payload() {
  char valueString[INT_AS_CHAR_SIZE + 1];
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    sprintf(valueString, "%04d", voltage[pinId]);
    memcpy(&payload[pinId * INT_AS_CHAR_SIZE], valueString, INT_AS_CHAR_SIZE);
  }
  // add Vcc to payload
  sprintf(valueString, "%04d", voltage_supply);
  memcpy(&payload[ANALOG_PIN_COUNT * INT_AS_CHAR_SIZE], valueString, INT_AS_CHAR_SIZE);
}
//==================================================================================================
//==================================================================================================
void _Payload_To_Data() {
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
#endif
//==================================================================================================