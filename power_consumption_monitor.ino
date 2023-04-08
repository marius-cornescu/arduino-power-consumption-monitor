/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------
*/
//= DEFINES ========================================================================================
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEBUG
//#define UseCOMM

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000  // 1 second

//= INCLUDES =======================================================================================
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//
const byte ANALOG_PIN_COUNT = 8;    // Arduino NANO has 8 analog pins
//------------------------------------------------
#ifdef DEBUG
const byte TIME_TICK = 100;
#else
const byte TIME_TICK = 10;
#endif
//------------------------------------------------
const char host_name[] = HOST_NAME;

// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;

//= VARIABLES ======================================================================================
WiFiClient espClient;

int voltage[ANALOG_PIN_COUNT];

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  //..............................
#ifdef DEBUG
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
  Serial.println(ssid);
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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  comm_ActIfActivity();
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  delay(10 * TIME_TICK);
}
//==================================================================================================