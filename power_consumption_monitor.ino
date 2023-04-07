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

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define SEC 1000  // 1 second

//= INCLUDES =======================================================================================
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
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

int analogPin = A0;  // connected to analog pin 0 - +3.3V
int val = 0;         // variable to store the value read

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
  //
  pinMode(A0, INPUT);
  //..............................
  wifi_Setup();
  //
  mqtt_Setup();
  //..............................
#ifdef DEBUG
  Serial.println(">>> Smart-PCM:Setup");
#endif
}
//**************************************************************************************************
void wifi_Setup() {
  delay(TIME_TICK);
  WiFi.hostname(host_name);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  int x = analogRead(A0);
  Serial.print("Analog value : ");
  Serial.println(x);
  //
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  //
  delay(10 * TIME_TICK);
}
//==================================================================================================
bool shouldSendRfCode() {
  return true;
}
//==================================================================================================
void transmitRfCode(unsigned long code, unsigned int code_length) {
}
//==================================================================================================