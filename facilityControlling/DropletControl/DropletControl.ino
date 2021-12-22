#include "DropGenerator.h"

// Thermo libraries
#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_TEMPR 8 // thermo-pin
#define THERM_COUNT 4 // count of thermo sensors

#define pinButton 5 // button
#define pinLight 1 // light-trigger
#define pinCam 12 // photron-trigger
#define pinLED 2 // synchro LED

const int IMPULSE_COUNT = 50; // count of microimpulses
const int MICRO_IMPULSE_TIME = 4000; // time of micro impulse
const int BETWEEN_IMPULSE_TIME = 5000; // time between impulses
const int LARGE_IMPULSE_TIME = 12000; // time of large impulse

const int TEMP_MESURE_TIME = 800; // update time
const int LIGHT_BRIGHTENING_TIME = 3500; // light increasing time
const unsigned long MAX_FREEZING_TIME = 25000; // freezing time

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

int incomingByte = 0; // for incoming data
String testName;
float tempr[THERM_COUNT]; // temperature variables
unsigned long checkMillis; // time variable
boolean flagLED; // LED switching flag

// Init dropGenerator object
DropGenerator dropGenerator;

void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT);
  
  pinMode(pinLight, OUTPUT);
  digitalWrite(pinLight,LOW);
  
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  
  pinMode(pinCam, OUTPUT);

  // object dallasTemperature init
  thermSensors.begin();
  }

void loop() {
  // get temperature by command "t" 
  // generate droplets by command "g"
  // turn on/off the light by command "l"
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    if (incomingByte == 116) // letter 't'
    {
      // GET TEMPERATURES
      // Create csv header
      Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4");
      // Thermo measuring, while light is getting brighter
      // measure temperatures
        thermSensors.requestTemperatures();
  
        //retrieve measure result
        for (uint8_t i = 0; i < 4; i++)
        {
          tempr[i] = thermSensors.getTempC(*ThermPtr[i]);
        }
      
        //print thermo-values
        Serial.print(millis());
        for (uint8_t i = 0; i < 4; i++)
        {
          Serial.print(",");
          Serial.print(tempr[i]);
        }
        Serial.println();
    }

    // DROPLET GENERATOR CONTROL
    if (incomingByte == 103) // letter 'g'
    {
      // Prepare large droplet
      Serial.print(millis());
      Serial.println(". Droplet generating start");
      dropGenerator.largeDropPreparing(IMPULSE_COUNT, MICRO_IMPULSE_TIME, BETWEEN_IMPULSE_TIME);

      // Generate large droplet (with macro impulse)
      dropGenerator.oneDrop(LARGE_IMPULSE_TIME);
      Serial.print(millis());
      Serial.println(". Droplet generating end");  
    }

    // LIGHT CONTROL
    if (incomingByte == 108) // letter 'l'
    {
      if (digitalRead(pinLight))
      {
        // Switch of the light.
        digitalWrite(pinLight,LOW);
        Serial.print(millis());
        Serial.println(". Light OFF");
      }
      else
      {
        // Switch on the light
        digitalWrite(pinLight,HIGH);
        Serial.print(millis());
        Serial.println(". Light ON");
      }
    }
  }
  
  // START FULL-PROGRAM, if button is pressed
  if(digitalRead(pinButton)==LOW){
    // Switch of the light. In case of switching on by command.
    digitalWrite(pinLight,LOW);
    
    Serial.println();
    Serial.println("Write test name!");
    while (Serial.available() == 0)
    {
      }
    testName = Serial.readString();
    Serial.println();
    Serial.println();
    Serial.print("BEGIN TEST #");
    Serial.print(testName);
    // Switch on the light
    digitalWrite(pinLight,HIGH);
    checkMillis = millis();
    Serial.print(checkMillis);
    Serial.println(". Light ON");
    

    // GET TEMPERATURES: before drop generating
    // Create csv header
    Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4");
    // Thermo measuring, while light is getting brighter
    while (millis()-checkMillis < LIGHT_BRIGHTENING_TIME)
    {
      // measure temperatures
      thermSensors.requestTemperatures();

      //retrieve measure result
      for (uint8_t i = 0; i < 4; i++)
      {
        tempr[i] = thermSensors.getTempC(*ThermPtr[i]);
      }
    
      //print thermo-values
      Serial.print(millis());
      for (uint8_t i = 0; i < 4; i++)
      {
        Serial.print(",");
        Serial.print(tempr[i]);
      }
      Serial.println();
    }

    // Prepare large droplet
    Serial.print(millis());
    Serial.println(". Droplet generating start");
    dropGenerator.largeDropPreparing(IMPULSE_COUNT, MICRO_IMPULSE_TIME, BETWEEN_IMPULSE_TIME);
        
    // Generate large droplet (with macro impulse)
    dropGenerator.oneDrop(LARGE_IMPULSE_TIME);
    
    // Photron recording
    Serial.print(millis());
    Serial.println(". Photron recording start");
    digitalWrite(pinCam, HIGH);
    delay(1);
    digitalWrite(pinCam,LOW);
    
    // LED on
    digitalWrite(pinLED, HIGH);
    // Time of droplet falling begin
    checkMillis = millis();
    Serial.print(checkMillis);
    Serial.println(". Droplet generating end. LED ON");
    flagLED = true;

    // GET TEMPERATURES: after drop generating
    // Create csv header
    Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4");
    // Thermo measuring, while light is getting brighter
    while (millis()-checkMillis < MAX_FREEZING_TIME)
    {
      // measure temperatures
      thermSensors.requestTemperatures();

      //retrieve measure result
      for (uint8_t i = 0; i < 4; i++)
      {
        tempr[i] = thermSensors.getTempC(*ThermPtr[i]);
      }
    
      //print thermo-values
      Serial.print(millis());
      for (uint8_t i = 0; i < 4; i++)
      {
        Serial.print(",");
        Serial.print(tempr[i]);
      }
      Serial.println();

      // Switch of the LED
      if (flagLED)
      {
        digitalWrite(pinLED, LOW);
        Serial.print(millis());
        Serial.println(". LED OFF");
        flagLED = false;
      }
    }
      
    // Switch of the light.
    digitalWrite(pinLight,LOW);
    Serial.print(millis());
    Serial.println(". Light OFF");
    Serial.print("END TEST #");
    Serial.print(testName);
  }
}
