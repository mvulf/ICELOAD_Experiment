// Sony infrared control setup & library
//#define DECODE_SONY
//#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

#define DELAY_AFTER_SEND 2000
#define DELAY_AFTER_LOOP 5000

# define IR_SEND_PIN 3

void setup() {
  Serial.begin(9600);
  
  //IrSender.begin();
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR-LED initialized");
}

// send data
uint16_t sAddress = 0x0000; // 0x0102
uint8_t sCommand = 0xB4B8F;
uint8_t sRepeats = 3;

void loop() {
    Serial.println();
    Serial.print(F("Send now: address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(sCommand);
    Serial.print(F(" repeats="));
    Serial.print(sRepeats);
    Serial.println();
    
    IrSender.sendSony(sAddress, sCommand, sRepeats);
    delay(DELAY_AFTER_SEND);

    sAddress += 0x0001;

    //delay(DELAY_AFTER_LOOP);
}
