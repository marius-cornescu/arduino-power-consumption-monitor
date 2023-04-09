//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
void prepareMessageToSend(char* message);

//= VARIABLES ======================================================================================
RtznCommProtocol commProto("PCM-MASTER", &processReceivedMessage, &prepareMessageToSend);

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
  while (!Serial) { ; }
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
  if (commProto.hasMessageInInboxThenReadMessage()) {
    commProto.actOnMessage();
    if (commProto.isHaveToPublish()) {
      __actOnPartnerDataChanged();
    }
  }
#endif
}
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMM
  // NOT SENDING MESSAGES - I'm only listening
#endif
}
//==================================================================================================
void __actOnPartnerDataChanged() {
  publishVoltageDataToMqtt();
}
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
#ifdef UseCOMM
  int CHAR_COUNT = 2;
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    byte byte1 = message[pinId * CHAR_COUNT] - byte('0');
    byte byte2 = message[pinId * CHAR_COUNT + 1] - byte('0');
    voltage[pinId] = byte1 * 100 + byte2;
#ifdef DEBUG
    Serial.print(voltage[pinId]);Serial.print(" => ");Serial.print(byte1);Serial.print(" | ");Serial.print(byte2);Serial.println();
#endif
  }
  haveToPublish = true;
#endif
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
#ifdef UseCOMM
  //------------------------------------
  // NO PUBLISHING
  //------------------------------------
#endif
}
//==================================================================================================