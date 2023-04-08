//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;

//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
#ifdef DEBUG
  Serial.println("Mqtt:Setup >>>");
#endif
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  //mqttClient.setCallback(callback);
  //..............................
  delay(10 * TIME_TICK);
#ifdef DEBUG
  Serial.println("Mqtt:Setup <<<");
#endif
}
//**************************************************************************************************
//==================================================================================================
void mqtt_MaintainConnection() {
  if (!mqttClient.connected()) {
    mqtt_Reconnect();
  }
  mqttClient.loop();
}
//==================================================================================================
void mqtt_Reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(HOST_NAME)) {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500 * TIME_TICK);
    }
  }
}
//==================================================================================================
void mqtt_PublishFloat(const char* topic, float value) {
  // Convert the value to a char array
  char valueString[8];
  dtostrf(value, 1, 2, valueString);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================