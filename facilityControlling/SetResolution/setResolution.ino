#include <OneWire.h>
#include <DallasTemperature.h>

// Thermo-pin
#define PIN_TEMPR 2

// Set up oneWire and dallasTemperature 
OneWire oneWire(PIN_TEMPR);
DallasTemperature thermSensors(&oneWire);

DeviceAddress Therm_0 = {0x28, 0xC3, 0x4A, 0xA4, 0x0B, 0x00, 0x00, 0xEA};

void setup() {
  // COM-port openning
  Serial.begin(9600);
  // object dallasTemperature init
  thermSensors.begin();
  thermSensors.setResolution(12);
}

void loop() {
  // put your main code here, to run repeatedly:

}
