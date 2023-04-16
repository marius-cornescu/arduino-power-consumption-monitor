/*
  PIN CONNECTIONS
  -------------------------------
  A0 - A7 - analog input pins
  -------------------------------
  D1 -> RX
  D0 -> TX
  -------------------------------
*/
//= DEFINES ========================================================================================
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define DEBUG
//#define DEBUG_VCC
//#define DEBUG_V
#define UseCOMM

//= INCLUDES =======================================================================================
#include "Common.h"
#include "CommCommon.h"

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//------------------------------------------------
const float OPERATING_VOLTAGE = 5.0;
//
const byte MEASUREMENT_COUNT = 5;
//------------------------------------------------

//= VARIABLES ======================================================================================
byte analog_pin[ANALOG_PIN_COUNT] = { A0, A1, A2, A3, A4, A5, A6, A7 };
unsigned int voltage_instant[ANALOG_PIN_COUNT];
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
  //
  adc_Setup();
  //
  delay(1000 * TIME_TICK);
  //..............................
#ifdef DEBUG
  Serial.println("START-UP <<<");
#endif
}
//**************************************************************************************************
void adc_Setup() {
#ifdef DEBUG
  Serial.println("NANO-ADC:Setup >>>");
#endif
  //..............................
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    pinMode(analog_pin[pinId], INPUT);
  }
  //..............................
  comm_Setup();
  //..............................
#ifdef DEBUG
  Serial.println("NANO-ADC:Setup <<<");
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  //..............................
  //
  _readAverageVoltagesOnAnalogPins(MEASUREMENT_COUNT);
  //
  comm_ActOnNewDataToSend();
  //..............................
  digitalWrite(LED_INDICATOR_PIN, LOW);
  //
  delay(200 * TIME_TICK);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void _readAverageVoltagesOnAnalogPins(byte iterations_count) {
  _averageVoltages_Reset();
  for (byte iteration = 0; iteration < iterations_count; iteration++) {
    _readVoltagesOnAnalogPins();
    _averageVoltages_Load();
  }
  _averageVoltages_Compute(iterations_count);
  _printVoltageData();
}
//==================================================================================================
void _averageVoltages_Load() {
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    voltage[pinId] = voltage[pinId] + voltage_instant[pinId];
  }
}
//==================================================================================================
void _averageVoltages_Compute(byte iterations_count) {
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    voltage[pinId] = round(voltage[pinId] / iterations_count);
  }
}
//==================================================================================================
void _averageVoltages_Reset() {
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    voltage[pinId] = 0;
  }
}
//==================================================================================================
void _readVoltagesOnAnalogPins() {
  for (byte pinId = 0; pinId < ANALOG_PIN_COUNT; pinId++) {
    unsigned int raw_analog = analogRead(analog_pin[pinId]);  // integer values between 0 and 1023
    float measured_voltage = _computeVoltage(raw_analog, 1000);
    pinMode(analog_pin[pinId], INPUT);
    voltage_instant[pinId] = round(measured_voltage);
  }
}
//==================================================================================================
float _computeVoltage(unsigned int raw_analog_value, int unit) {
  float raw_voltage = raw_analog_value * OPERATING_VOLTAGE / 1024.0;
  // read correct supply voltage
  long raw_vcc = _readVcc();
  float supply_voltage = raw_vcc * (unit / 1000.0);
  float corrected_voltage = supply_voltage / OPERATING_VOLTAGE * raw_voltage;
  voltage_supply = round(supply_voltage);

#ifdef DEBUG_VCC
  Serial.print("Vcc = ");
  Serial.print(supply_voltage);
  Serial.print(" V | ");
#endif

  return corrected_voltage;
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
//==================================================================================================
long _readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2);             // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);  // Convert
  while (bit_is_set(ADCSRA, ADSC))
    ;
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result;  // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}
//==================================================================================================