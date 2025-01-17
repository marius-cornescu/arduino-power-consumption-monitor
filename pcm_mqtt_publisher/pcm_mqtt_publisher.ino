/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------
  -------------------------------
  Variables and constants in RAM (global, static), used 30004 / 80192 bytes (37%)
  Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 61115 / 65536 bytes (93%)
  Code in flash (default, ICACHE_FLASH_ATTR), used 253464 / 1048576 bytes (24%)
  -------------------------------
*/
//= DEFINES ========================================================================================
//
#define PUBLISH_PORT_TOPIC "home/pcm/unit-A/port/"
#define PUBLISH_STATUS_TOPIC "home/pcm/unit-A/status"

#define SUBSCRIBE_TOPIC "home/pcm/unit-A/command/+"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define DEBUG
//#define DEBUG_V
//#define DEBUG_MQTT
#define UseCOMMPro

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
#endif
  debugPrintln(F("START-UP >>>>>>>>>>>>>>>"));
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
  debugPrintln(F("START-UP <<<<<<<<<<<<<<<"));
}
//**************************************************************************************************
void wifi_Setup() {
  delay(TIME_TICK);
  WiFi.hostname(host_name);
  WiFi.setOutputPower(0);  // Sets WiFi RF power output to lowest level, lowest RF power usage
  // We start by connecting to a WiFi network
  debugPrintln(F(""));
  debugPrint(F("Connecting to "));
  debugPrint(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50 * TIME_TICK);
    debugPrint(F("."));
  }

  debugPrintln(F(""));
  debugPrint(F("WiFi connected | "));
  debugPrint(F("IP address: "));
  debugPrintln(WiFi.localIP());
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  mqtt_MaintainConnection();
  //
  if (comm_ActIfReceivedMessage() && mqtt_ShouldPublish()) {
    publishVoltageDataToMqtt();
    publishStatusDataToMqtt();
  }
  //
  delay(100 * TIME_TICK);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void publishVoltageDataToMqtt() {
  _printVoltageData();

  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    char port_topic[] = PUBLISH_PORT_TOPIC;
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
  char statusReport[4];
  utoa((unsigned)voltage_supply, statusReport, 10);
  mqtt_PublishString(PUBLISH_STATUS_TOPIC, statusReport);
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
}
//==================================================================================================
void publishStatusDataToMqtt() {
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  // could print sw version; params like PUBLISH_COLLDOWN_TIME
  //
  char statusReport[4];
  utoa((unsigned)voltage_supply, statusReport, 10);
  mqtt_PublishString(PUBLISH_STATUS_TOPIC, statusReport);
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
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