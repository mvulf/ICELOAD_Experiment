#include "DropGenerator.h"

// Thermo libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Humidity sensor library
#include "DHT.h"

#define DHTPIN 4 // humidity pin
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
const unsigned long LED_TIME = 3500; // time of LED-shining

// Humidity sensor init
DHT dht(DHTPIN, DHT22);

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
boolean flagTempr = false; // Switch t-measuring

boolean flagMeasure = false; // Provide full measurement

// humidity var, in %
float humidity;

// temperature in the chamber, *C
float chamber_tempr;

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
  // object humiditySensor init
  dht.begin();
  }

void loop() {
  // get temperature by command "t" 
  // generate droplets by command "g"
  // turn on/off the light by command "l"
  // get humidity and ambient temperature by command "h"
  // full program by command "f"
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    // PROVIDE FULL PROGRAM
    if (incomingByte == 102) // letter 'f'
    { 
      // FLAG: generating & measure
      flagMeasure = true;
      Serial.readString();
    }
    
    // GET TEMPERATURES
    if (incomingByte == 116) // letter 't'
    { 
      // FLAG: measure temperatures
      flagTempr = !flagTempr;
      if (flagTempr){
          // Create csv header
          Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4");
        }  
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

    // GET HUMIDITY
    if (incomingByte == 104) // letter 'h'
    {
      // Create csv header
      Serial.println("time,chamber_thermo,humidity");
      Serial.print(millis());

      // measure humidity and chamber temperature
      humidity = dht.readHumidity();
      chamber_tempr = dht.readTemperature();

      //print humidity and thermo values
      Serial.print(",");
      Serial.print(chamber_tempr);
      Serial.print(",");
      Serial.print(humidity);
     
      Serial.println();
      
    }    
  }

  // TEMPERATURES MEASURING
  if(flagTempr)
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
  
  // START FULL-PROGRAM, if button is pressed or measure flag
  if((digitalRead(pinButton)==LOW)||flagMeasure){
    // Drop measure flag
    flagMeasure = false;
    
    // Switch of the light. In case of switching on by command.
    digitalWrite(pinLight,LOW);
    // Stop temperature measuring. In case of switching on by command.
    flagTempr = false;
    
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
    Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4,chamber_thermo,humidity");
    // Thermo and humidity measuring, while light is getting brighter
    while (millis()-checkMillis < LIGHT_BRIGHTENING_TIME)
    {
      // measure temperatures
      thermSensors.requestTemperatures();

      // measure humidity and chamber temperature
      humidity = dht.readHumidity();
      chamber_tempr = dht.readTemperature();
      
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

      //print humidity and thermo values
      Serial.print(",");
      Serial.print(chamber_tempr);
      Serial.print(",");
      Serial.print(humidity);
     
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
    Serial.println("time,thermo_1,thermo_2,thermo_3,thermo_4,chamber_thermo,humidity");
    // Thermo and humidity measuring, while light is getting brighter
    while (millis()-checkMillis < MAX_FREEZING_TIME)
    {
      // measure temperatures
      thermSensors.requestTemperatures();

      // measure humidity and chamber temperature
      humidity = dht.readHumidity();
      chamber_tempr = dht.readTemperature();
      
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

      //print humidity and thermo values
      Serial.print(",");
      Serial.print(chamber_tempr);
      Serial.print(",");
      Serial.print(humidity);
     
      Serial.println();

      // Switch of the LED
      if (flagLED && (millis()-checkMillis > LED_TIME))
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
