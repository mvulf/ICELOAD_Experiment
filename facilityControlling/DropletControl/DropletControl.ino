#include "DropGenerator.h"

// Thermo libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Humidity sensor library
#include "DHT.h"

// #define MAX_INPUT 50

#define DHTPIN 4 // humidity pin
#define PIN_TEMPR 8 // thermo-pin
#define THERM_COUNT 4 // count of thermo sensors

#define pinButton 5 // button
#define pinLight 1 // light-trigger
#define pinCam 12 // photron-trigger
#define pinLED 2 // synchro LED

const int TEMP_MESURE_TIME = 800; // update time
const int LIGHT_BRIGHTENING_TIME = 3500; // light increasing time
const unsigned long MAX_FREEZING_TIME = 180000; // freezing time
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

// droplet generating parameters
int impulseCount = 50; // count of microimpulses
int microImpulseTime = 4000; // time of micro impulse
int betweenImpulseTime = 5000; // time between impulses
int largeImpulseTime = 12000; // time of large impulse

int incomingByte = 0; // for incoming data
String testName;

// char inputBuffer[MAX_INPUT+1];

char generatingValues[90];

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

//  inputBuffer[0] = '\0';

  Serial.println("Default settings of droplet generating:");
  Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");
  Serial.print(impulseCount);
  Serial.print(",");
  Serial.print(microImpulseTime);
  Serial.print(",");
  Serial.print(largeImpulseTime);
  Serial.print(",");
  Serial.println(betweenImpulseTime);
  }

void loop() {
  // get temperature by command "t" 
  // generate droplets by command "g"
  // turn on/off the light by command "l"
  // get humidity and ambient temperature by command "h"
  // full program by command "f". Stop command by "f"
  // change settings by command "s".
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
      Serial.println("Current settings of droplet generating:");
      Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");
      Serial.print(impulseCount);
      Serial.print(",");
      Serial.print(microImpulseTime);
      Serial.print(",");
      Serial.print(largeImpulseTime);
      Serial.print(",");
      Serial.println(betweenImpulseTime);
        
      // Prepare large droplet
      Serial.print(millis());
      Serial.println(". Droplet generating start");
      dropGenerator.largeDropPreparing(impulseCount, microImpulseTime, betweenImpulseTime);

      // Generate large droplet (with macro impulse)
      dropGenerator.oneDrop(largeImpulseTime);
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
    
    // CHANGE DROPLET GENERATING SETTINGS
    if (incomingByte == 115) // letter 's'
    {
      // display settings
      Serial.println("Current settings of droplet generating:");
      Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");
      Serial.print(impulseCount);
      Serial.print(",");
      Serial.print(microImpulseTime);
      Serial.print(",");
      Serial.print(largeImpulseTime);
      Serial.print(",");
      Serial.println(betweenImpulseTime);

      Serial.println();

      Serial.readString();
      Serial.println("Do you want to change them?");
      Serial.println("Write (c) for changing impulseCount");
      Serial.println("Write (m) for changing microImpulseTime");
      Serial.println("Write (l) for changing largeImpulseTime");
      Serial.println("Write (b) for changing betweenImpulseTime");
      Serial.println("Write any other letter for cancelling settings changing.");
      while (Serial.available() == 0)
      {
        }

      //Serial.println("Write new settings, using comma as delimiter");
      //Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");       
      incomingByte = Serial.read();
      Serial.readString();
      if (incomingByte == 99) // letter 'c'
      {
        Serial.println("Write impulseCount:");
        while (Serial.available() == 0)
        {
          }
        impulseCount = Serial.readString().toInt();
      }

      if (incomingByte == 109) // letter 'm'
      {
        Serial.println("Write microImpulseTime:");
        while (Serial.available() == 0)
        {
          }
        microImpulseTime = Serial.readString().toInt();
      }

      if (incomingByte == 108) // letter 'l'
      {
        Serial.println("Write largeImpulseTime:");
        while (Serial.available() == 0)
        {
          }
        largeImpulseTime = Serial.readString().toInt();
      }

      if (incomingByte == 98) // letter 'b'
      {
        Serial.println("Write betweenImpulseTime:");
        while (Serial.available() == 0)
        {
          }
        betweenImpulseTime = Serial.readString().toInt();
      }
        //TODO: Write values parser! 
        //char input = Serial.read();
        //static int s_len; // static variables default to 0
        //if (s_len>=MAX_INPUT) {
          // Have received already the maximum number of characters
          // Ignore all new input until line termination occurs
        //} else if (input != '\n' && input != '\r') {
        //  inputBuffer[s_len++] = input;
        //} else {
          // Have received a LF or CR character
    
          // INSERT YOUR CODE HERE TO PROCESS THE RECEIVED DATA //
          // YOU COULD COPY TO A NEW VARIABLE WITH strncpy() OR //
          // SET A FLAG TO SAY TO START SOME OTHER TASK         //
         // Serial.print("RECEIVED MSG: ");
         // Serial.println(inputBuffer);
    
         // memset(inputBuffer, 0, sizeof(inputBuffer));
         // s_len = 0;             // Reset input buffer here if you
                                 // have already copied the data.
                                 // If you don't reset here, then
                                 // you can't start receiving more
                                 // serial port data. This is your
                                 // 'software' serial buffer, contrast
                                 // with the hardware serial buffer.
       // }
              
          // values for parsing
        //char generatingValues[] = Serial.readString();
       // char generatingValues[] 
       // char* pch;

        Serial.println("New settings of droplet generating:");
        Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");
        Serial.print(impulseCount);
        Serial.print(",");
        Serial.print(microImpulseTime);
        Serial.print(",");
        Serial.print(largeImpulseTime);
        Serial.print(",");
        Serial.println(betweenImpulseTime);

       // Serial.println("Changing settings cancelled"); 
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

    Serial.println("Current settings of droplet generating:");
    Serial.println("impulseCount,microImpulseTime,largeImpulseTime,betweenImpulseTime");
    Serial.print(impulseCount);
    Serial.print(",");
    Serial.print(microImpulseTime);
    Serial.print(",");
    Serial.print(largeImpulseTime);
    Serial.print(",");
    Serial.println(betweenImpulseTime);

    // Prepare large droplet
    Serial.print(millis());
    Serial.println(". Droplet generating start");
    dropGenerator.largeDropPreparing(impulseCount, microImpulseTime, betweenImpulseTime);
        
    // Generate large droplet (with macro impulse)
    dropGenerator.oneDrop(largeImpulseTime);
    
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

      // CHECK STOP COMMAND: if LED is off!
      if ((!flagLED) && (Serial.available() > 0))
      {
        incomingByte = Serial.read();
    
        // STOP COMMAND
        if (incomingByte == 102) // letter 'f'
        { 
          Serial.readString();
          break;
        }
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
