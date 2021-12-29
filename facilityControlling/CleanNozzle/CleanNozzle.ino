#include <digitalWriteFast.h>
#define pinOrange 6
#define pinBrown 7
#define pinBlue 9
#define pinGreen 10
#define Ledpin 13
#define Pinbutton 5 //button
#define pinCam 4 //photron-trigger

int rmax=10000 ;//количество тактов МЕНЯТЬ ЗДЕСЬ!!!!!!!!!!!!!!!!!!!!! // 10 и более
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
    }
  digitalWriteFast(pinBlue, HIGH);
  digitalWriteFast(pinGreen, HIGH);
  
 delayMicroseconds(12000);//ON SMALL(black wire)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!us
  

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
  delay(50);//Break между каплями !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  digitalWriteFast(Ledpin, LOW);
r=r+1 ;//учет такта, добавление к счетчику 
 }else if (r==rmax)
 {
    digitalWriteFast(pinBlue, HIGH);
  digitalWriteFast(pinGreen, HIGH);
  
 delayMicroseconds(4500);//ON LAST(black wire)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!us
  

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
  delay(50);//Break между каплями !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  digitalWriteFast(Ledpin, LOW);
  r=r+1;
   }
else{
digitalWrite(Ledpin, HIGH);//включение светодиода
  delay(100);//время свечения
   digitalWrite(Ledpin, LOW);//включение светодиода
 delay(200);//пауза между морганием
}
}
