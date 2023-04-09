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

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000  // 1 second

#ifdef DEBUG
#define TIME_TICK 100
#else
#define TIME_TICK 10
#endif

//= INCLUDES =======================================================================================
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//
const byte ANALOG_PIN_COUNT = 8;  // Arduino NANO has 8 analog pins
//------------------------------------------------
const char host_name[] = HOST_NAME;

// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;

//= VARIABLES ======================================================================================
WiFiClient espClient;

static int voltage[ANALOG_PIN_COUNT];

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) { ; }
  Serial.println("Smart-PCM:Setup >>>");
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
  Serial.println("Smart-PCM:Setup <<<");
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
  comm_ActIfReceivedMessage();
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  delay(1 * TIME_TICK);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void publishVoltageDataToMqtt() {
  _printVoltageData();
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    char port_topic[] = "home/pcm/unit-A/port/";
    port_topic[21] = pinId + byte('0');
    port_topic[22] = '\0';
#ifdef DEBUG_MQTT
    Serial.print(port_topic);Serial.print(" => ");Serial.println(voltage[pinId]);
#endif
    mqtt_PublishInt(port_topic, voltage[pinId]);
  }
}
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
  Serial.println("");
  Serial.println("---------------------------------------");
#endif
}
//==================================================================================================