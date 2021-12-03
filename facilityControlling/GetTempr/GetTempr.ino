#include <OneWire.h>
#include <DallasTemperature.h>

// Thermo-pin
#define PIN_TEMPR 8

// Thermo-count
#define THERM_COUNT 4

// Set up oneWire and dallasTemperature 
OneWire oneWire(PIN_TEMPR);
DallasTemperature thermSensors(&oneWire);

// adress variables
DeviceAddress Therm_1 = {0x28, 0x76, 0xC9, 0xCF, 0x11, 0x20, 0x06, 0xBB}; //I
DeviceAddress Therm_2 = {0x28, 0xED, 0xE2, 0x94, 0x15, 0x20, 0x06, 0xA8}; //II
DeviceAddress Therm_3 = {0x28, 0x5B, 0xCF, 0xC1, 0x11, 0x20, 0x06, 0xCE}; //III
DeviceAddress Therm_4 = {0x28, 0xD1, 0xE5, 0xD5, 0x14, 0x20, 0x06, 0xAD}; //IV

// adress pointer array
DeviceAddress* ThermPtr[THERM_COUNT] = {&Therm_1, 
&Therm_2, &Therm_3, &Therm_4};

// update time
const int TEMP_MESURE_TIME = 800;

// temperature variables
float tempr[THERM_COUNT];

// Function for printing one adress
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

// Function for printing all adress
void printAllAddress(DeviceAddress* adressPtr[THERM_COUNT])
{
  for (uint8_t row = 0; row < THERM_COUNT; row++)
  {
    printAddress(*adressPtr[row]);
    Serial.println();
  }
}

void setup() {
  // COM-port openning
  Serial.begin(9600);
  // object dallasTemperature init
  thermSensors.begin();
}

void loop(){
  // measure temperatures
  thermSensors.requestTemperatures();

  // printAllAddress(ThermPtr);

  //retrieve measure result
  for (uint8_t i = 0; i < 4; i++)
  {
    tempr[i] = thermSensors.getTempC(*ThermPtr[i]);
  }

  //print values
  for (uint8_t i = 0; i < 4; i++)
  {
    Serial.print("Temperature ");
    Serial.print(String(i+1));
    Serial.print(", Celsius: ");
    Serial.println(tempr[i]);
  }
  Serial.println();
}
