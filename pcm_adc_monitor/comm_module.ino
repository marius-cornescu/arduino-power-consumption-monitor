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
  delay(50 * TIME_TICK);
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMM
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