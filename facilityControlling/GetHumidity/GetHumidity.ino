// Humidity sensor library
#include "DHT.h"

#define DHTPIN 4 // humidity pin

// Humidity sensor init
DHT dht(DHTPIN, DHT22);

// humidity var, in %
float humidity;

// temperature in the chamber, *C
float chamber_tempr;

void setup() {
  Serial.begin(9600);
  dht.begin();
  // Create csv header
  Serial.println("time,chamber_thermo,humidity");
}
void loop() {
  delay(2000);
  
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
