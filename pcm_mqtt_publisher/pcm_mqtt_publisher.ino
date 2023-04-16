/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------
*/
//= DEFINES ========================================================================================
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define DEBUG
//#define DEBUG_V
//#define DEBUG_MQTT
#define UseCOMM

//= INCLUDES =======================================================================================
#include "Common.h"
#include "CommCommon.h"
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//------------------------------------------------
const char host_name[] = HOST_NAME;

// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;

//= VARIABLES ======================================================================================
WiFiClient espClient;

static unsigned int voltage[ANALOG_PIN_COUNT];

unsigned int voltage_supply;

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) { ; }
  Serial.println("START-UP >>>");
#endif
  //..............................
  // initialize digital pin LED_INDICATOR_PIN as an output.
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  //..............................
  //
  wifi_Setup();
  //
  mqtt_Setup();
  //
  comm_Setup();
  //..............................
#ifdef DEBUG
  Serial.println("START-UP <<<");
#endif
}
//**************************************************************************************************
void wifi_Setup() {
  delay(TIME_TICK);
  WiFi.hostname(host_name);
  // We start by connecting to a WiFi network
#ifdef DEBUG
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
#endif

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50 * TIME_TICK);
#ifdef DEBUG
    Serial.print(".");
#endif
  }

#ifdef DEBUG
  Serial.println("");
  Serial.print("WiFi connected | ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  mqtt_MaintainConnection();
  //
  if (comm_ActIfReceivedMessage()) {
    publishVoltageDataToMqtt();
  }
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  delay(100 * TIME_TICK);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void publishVoltageDataToMqtt() {
  _printVoltageData();

  if (mqtt_SkipPublish()) {
    return;
  }

  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    char port_topic[] = "home/pcm/unit-A/port/";
    port_topic[21] = pinId + byte('0');
    port_topic[22] = '\0';
#ifdef DEBUG_MQTT
    Serial.print(port_topic);
    Serial.print(" => ");
    Serial.println(voltage[pinId]);
#endif
    mqtt_PublishInt(port_topic, voltage[pinId]);
  }
  //
  mqtt_PublishInt("home/pcm/unit-A/Vcc", voltage_supply);
}
//==================================================================================================
//==================================================================================================
void _printVoltageData() {
#ifdef DEBUG_V
  Serial.println("");
  Serial.println("---------------------------------------");
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    Serial.print("Pin [A");
    Serial.print(pinId);
    Serial.print("] = ");
    Serial.print(voltage[pinId]);
    Serial.print(" mV | ");
  }
  Serial.print("Vcc = ");
  Serial.print(voltage_supply);
  Serial.print(" mV");
  Serial.println();
  Serial.println("---------------------------------------");
#endif
}
//==================================================================================================