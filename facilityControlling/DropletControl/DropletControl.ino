#include "DropGenerator.h"

#define pinButton 5 //button
#define pinLight 1 //Light-trigger
#define pinCam 12 //photron-trigger

#define pinLED 2 // Synchro LED

//int rmax=3 ;//количество тактов МЕНЯТЬ ЗДЕСЬ!!!!!!!!!!!!!!!!!!!!!
//int r=rmax ;//счетчик

DropGenerator dropGenerator;

void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT);
  
  pinMode(pinLight, OUTPUT);
  digitalWrite(pinLight,LOW);
  
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
  
  pinMode(pinCam, OUTPUT);
  }

void loop() {
  if(digitalRead(pinButton)==LOW){
    // Switch on the light
    Serial.println("Light on");
    digitalWrite(pinLight,HIGH);
    delay(3000);
    //delay(1);
    //digitalWrite(pinCam,LOW);
    //delay(1); // Garantee of droplets-free frames

    // Generate large droplet
    Serial.println("Droplet generating start");
    dropGenerator.largeDropPreparing(30, 3000, 5000);
    // Photron recording
    Serial.println("Photron recording");
    digitalWrite(pinCam, HIGH);
    delay(1);
    digitalWrite(pinCam,LOW);
    // generate macro impulse
    dropGenerator.oneDrop(5000);
    Serial.println("Droplet generating end");
    // LED on
    digitalWrite(pinLED, HIGH);
    Serial.println("Synchro LED ON");
    delay(500);
    digitalWrite(pinLED, LOW);
    Serial.println("Synchro LED OFF");

    // Hold on the light
    delay(5000);
    digitalWrite(pinLight,LOW);
    Serial.println("Light off");
  }
}
