#include <OneWire.h>
#include <DallasTemperature.h>

// Thermo-pin
#define PIN_TEMPR 8

// Set up oneWire and dallasTemperature 
OneWire oneWire(PIN_TEMPR);
DallasTemperature thermSensors(&oneWire);

// adress variable
DeviceAddress adress;

// Function for printing adress
void printAddress(DeviceAddress deviceAddress)
{
  // Output deviceAdress in list-like format
  for (uint8_t i = 0; i < 7; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    Serial.print(", ");
  }
  Serial.print("0x");
  if (deviceAddress[7] < 16) Serial.print("0");
  Serial.print(deviceAddress[7], HEX);
}

void setup() {
  // COM-port openning
  Serial.begin(9600);
  // object dallasTemperature init
  thermSensors.begin();
}

void loop(){
  // Retrieve adress
  thermSensors.getAddress(adress, 0);
  delay(1000);
  Serial.print("Sensor's adress: ");
  printAddress(adress);
  Serial.println();

  //Serial.println(thermSensors.getResolution(dischTherm), DEC);
}
