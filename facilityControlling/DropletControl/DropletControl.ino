#include <digitalWriteFast.h>
#define pinOrange 6
#define pinBrown 7
#define pinBlue 9
#define pinGreen 10
#define Ledpin 13
#define Pinbutton 5 //button
#define pinCam 12 //photron-trigger

int rmax=3 ;//количество тактов МЕНЯТЬ ЗДЕСЬ!!!!!!!!!!!!!!!!!!!!!
int r=rmax ;//счетчик

void setup() {
  pinModeFast(pinOrange, OUTPUT);
  pinModeFast(pinBrown, OUTPUT);
  pinModeFast(pinBlue, OUTPUT);
  pinModeFast(pinGreen, OUTPUT);
  pinModeFast(Ledpin, OUTPUT);
  pinMode(Pinbutton, INPUT);
  pinMode(pinCam, OUTPUT);
  //digitalWrite(Pinbutton, LOW);
  }

void loop() {
if(digitalRead(Pinbutton)==LOW){
r=0;
delay(200);
}

//the pin is toggled multiple time before looping is because it took too long that the pin stayed low for 600ns, while clearing or setting the pin only took 125ns. For 16MHz Arduino Uno.
 if (r<rmax) {
  if (r==0) { // 
    digitalWrite(pinCam,HIGH);
    delay(1);
    digitalWrite(pinCam,LOW);
    delay(10); // Garantee of droplets-free frames
    }
  digitalWriteFast(pinBlue, HIGH);
  //delay(10);
  digitalWriteFast(pinGreen, HIGH);
  //delay(30 );//ON(black wire)ONLY FOR CLEANING
  delayMicroseconds(5000);//ON(black wire)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!us

  digitalWriteFast(pinBlue, LOW);
  digitalWriteFast(pinBrown, HIGH);
delayMicroseconds(5000);
  digitalWriteFast(pinGreen, LOW);
digitalWriteFast(pinBrown, LOW);
  
  delay(10);//ms

    
  digitalWriteFast(pinOrange, HIGH);
  //delay(100);
  digitalWriteFast(pinBrown, HIGH);
  delayMicroseconds(5000);//OFF(red wire)

  digitalWriteFast(pinOrange, LOW);
digitalWriteFast(pinGreen, HIGH);

delayMicroseconds(5000);
  digitalWriteFast(pinBrown, LOW);
  digitalWriteFast(pinGreen, LOW);

  
  digitalWriteFast(Ledpin, HIGH);
  delay(200);//Break между каплями !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  digitalWriteFast(Ledpin, LOW);
r=r+1 ;//учет такта, добавление к счетчику 1
   }
else{
digitalWrite(Ledpin, HIGH);//включение светодиода
  delay(100);//время свечения
   digitalWrite(Ledpin, LOW);//включение светодиода
 delay(200);//пауза между морганием
}
}
