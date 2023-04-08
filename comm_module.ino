//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
const char* prepareMessageToSend();

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
void comm_ActIfActivity() {
#ifdef UseCOMM
  // NO MESSAGES - I'm not listening
#endif
}
//==================================================================================================
void comm_actOnNewData() {
#ifdef UseCOMM
  // NOT SENDING MESSAGES - I'm only listening
#endif
}
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
#ifdef UseCOMM
  //------------------------------------
  // NO LISTENING
  //------------------------------------
#endif
  return haveToPublish;
}
//==================================================================================================
const char* prepareMessageToSend() {
#ifdef UseCOMM
  //------------------------------------
  // NO PUBLISHING
  //------------------------------------
#endif
  return new char[0];
}
//==================================================================================================