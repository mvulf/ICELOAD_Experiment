#include "Arduino.h"
#include "DropGenerator.h"
#include <digitalWriteFast.h>

#define pinOrange 6
#define pinBrown 7
#define pinBlue 9
#define pinGreen 10


DropGenerator::DropGenerator()
{
  pinModeFast(pinOrange, OUTPUT);
  pinModeFast(pinBrown, OUTPUT);
  pinModeFast(pinBlue, OUTPUT);
  pinModeFast(pinGreen, OUTPUT);
}

void DropGenerator::oneDrop(int impulseTime)
{
  // OPEN NOZZLE - IMPULSE UP: piezo element's power on
  // black wire - power on
  digitalWriteFast(pinBlue, HIGH);
  // red wire - to ground
  digitalWriteFast(pinGreen, HIGH);
  // Hold piezo element's power on during impulse time (black wire ON!)
  delayMicroseconds(impulseTime); 
  
  // CLOSE NOZZLE - IMPULSE DOWN: piezo element's power off
  // black wire - power off
  digitalWriteFast(pinBlue, LOW);
  // black wire - to ground (piezo element's discharge)
  digitalWriteFast(pinBrown, HIGH);
  // Hold on piezo element's connection to ground
  delayMicroseconds(5000);
  // Unplug black and red wires from ground
  digitalWriteFast(pinGreen, LOW);
  digitalWriteFast(pinBrown, LOW);
  // Hold on piezo element without connection
  delay(10);//ms
  
  // REVERSE IMPULSE - UP: change piezo element's connection for total discharging
  // red wire - power on
  digitalWriteFast(pinOrange, HIGH);
  // black wire - to ground
  digitalWriteFast(pinBrown, HIGH);
  // Hold piezo element's reverse power on (red wire ON!)
  delayMicroseconds(5000);

  // REVERSE IMPULSE - DOWN: piezo element's reversed power off
  // red wire - power off
  digitalWriteFast(pinOrange, LOW);
  // red wire - to ground (piezo element's discharge)
  digitalWriteFast(pinGreen, HIGH);
  // Hold on piezo element's connection to ground
  delayMicroseconds(5000);
  // Unplug black and red wires from ground
  digitalWriteFast(pinBrown, LOW);
  digitalWriteFast(pinGreen, LOW);
}

// microImpulseTime, macroImpulseTime, betweenImpulsesTime, us; 
void DropGenerator::largeDropPreparing(int microImpulseCount, int microImpulseTime, int betweenImpulsesTime)
{
  // init impulse counter
  int r = 0;

  // generate micro impulses
  while(r<microImpulseCount)
  {
    oneDrop(microImpulseTime);
    delayMicroseconds(betweenImpulsesTime);
    r++;
  }
}
