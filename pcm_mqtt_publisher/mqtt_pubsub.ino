//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

unsigned long PUBLISH_COLLDOWN_TIME = 5 * SEC;      // value in milliseconds

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);

unsigned long lastMqttConnection = 0;
unsigned long lastMqttPublish = 0;

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
#ifdef DEBUG
  Serial.println("Mqtt:Setup >>>");
#endif
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_Callback);
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
#ifdef DEBUG
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    if (mqttClient.connect(HOST_NAME)) {
#ifdef DEBUG
      Serial.println("connected");
#endif
      // Subscribe
      mqttClient.subscribe(SUBSCRIBE_TOPIC);
    } else {
#ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(500 * TIME_TICK);
    }
  }
}
//==================================================================================================
void mqtt_Callback(char* topic, byte* message, unsigned int length) {
  // Serial.print("Message arrived on topic: ");
  // Serial.print(topic);
  // Serial.print(". Message: ");
  // String messageTemp;
  
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)message[i]);
  //   messageTemp += (char)message[i];
  // }
  // Serial.println();

  // // Feel free to add more if statements to control more GPIOs with MQTT

  // // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // // Changes the output state according to the message
  // if (String(topic) == SUBSCRIBE_TOPIC) {
  //   Serial.print("Changing output to ");
  //   if(messageTemp == "on"){
  //     Serial.println("on");
  //     digitalWrite(LED_INDICATOR_PIN, HIGH);
  //   }
  //   else if(messageTemp == "off"){
  //     Serial.println("off");
  //     digitalWrite(LED_INDICATOR_PIN, LOW);
  //   }
  // }
}
//==================================================================================================
void mqtt_PublishInt(const char* topic, int value) {
  // Convert the value to a char array
  char valueString[4];
  utoa((unsigned)value, valueString, 10);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishFloat(const char* topic, float value) {
  // Convert the value to a char array
  char valueString[8];
  dtostrf(value, 1, 2, valueString);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishString(const char* topic, const char* value) {
  mqttClient.publish(topic, value);
}
//==================================================================================================
bool mqtt_ShouldPublish() {
  if (millis() - lastMqttPublish > PUBLISH_COLLDOWN_TIME) {
    return false;
  } else {
    lastMqttPublish = millis();
    return true;
  }
}
//==================================================================================================