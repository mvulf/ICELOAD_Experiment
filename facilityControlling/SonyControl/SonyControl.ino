#include <IRremote.h> // подключаем библиотеку для IR приемника

//IRrecv irrecv(A0); // указываем пин, к которому подключен IR приемник
IRsend irsend; // указываем пин, к которому подключен IR приемник
decode_results results;


 
const byte IN_PORT = A0; 
const byte Led = A2; 
const byte Button = A5;


void setup() {
  // irrecv.enableIRIn();  // запускаем прием инфракрасного сигнала

   Serial.begin(9600); // подключаем монитор порта

   pinMode(IN_PORT, INPUT_PULLUP); // пин A0 будет входом
   pinMode(Button, OUTPUT); //  
   pinMode(Led, OUTPUT); //  
    
  
}

void loop() {
  
      
       digitalWrite(Led, LOW);
         digitalWrite(Button, HIGH);
       

      if (  digitalRead(Button)  == HIGH) // || digitalRead(IN_PORT)  == LOW
      {    
          Serial.println("make photo");
          for (int i=0; i<3; i++)
          {       
              irsend.sendSony(0xB4B8F, 20); // shutter       
              delay(40);  
          }
          digitalWrite(Led, HIGH);
          delay(40);  
      }
 


  
  /*
   if (irrecv.decode(&results)) // если данные пришли выполняем команды
   {
      Serial.println(results.value); // отправляем полученные данные на порт
    
      // включаем и выключаем светодиод, в зависимости от полученного сигнала  
      if (results.value == 16718055) { 
      digitalWrite(12, HIGH);
   }
      if (results.value == 16724175) { 
      digitalWrite(12, LOW);
   }
      irrecv.resume(); // принимаем следующий сигнал на ИК приемнике
   }
   */
}
