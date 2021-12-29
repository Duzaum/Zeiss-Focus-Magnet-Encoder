#include <EEPROM.h>

const int reset = 8;

const int botao = 7;

volatile int long counterAB = 0;

long cont = 0;

long cont2 = 0;

long cont3 = 0;

long cont4 = 0;

float data = 0;

int estadoReset = 0;

char dataf [7];

String last;


void setup()

{

Serial.begin(4800, SERIAL_7N1);
Serial.println("Online");

pinMode (botao,OUTPUT);
pinMode (reset,INPUT);

digitalWrite(botao, HIGH);

estadoReset = digitalRead(reset);

if (estadoReset == HIGH)
{

         EEPROM.write(509,0);
         EEPROM.write(510,0);
         EEPROM.write(511,0);  // saving two bytes on memory
         EEPROM.write(512,0);
}

attachInterrupt(digitalPinToInterrupt(2), ai0, CHANGE);

attachInterrupt(digitalPinToInterrupt(3), ai1, CHANGE);



}

void loop() {

char tecla;
cont = counterAB + cont4;
delay (200);
cont2 = counterAB + cont4;

tecla = Serial.read();

     if (cont != cont2)

     {
     byte hiByte = highByte(cont2); // The EEPROM memory only saves values between 0 ~ 255, after that, we broke the value into two bytes.
     byte loByte = lowByte(cont2);
     EEPROM.write(511,hiByte);  // saving two bytes on memory
     EEPROM.write(512,loByte);
     }

         byte highByte = EEPROM.read(511);  // reading the memory
         byte lowByte = EEPROM.read(512);
         cont3  = word(highByte, lowByte);  // making the conversion of the two bytes storaged on memory to a number.


     if (cont == 0 && cont2 == 0)   //"reset"

        {
         byte hiByte = highByte(cont3);
         byte loByte = lowByte(cont3);
         EEPROM.write(509,hiByte);
         EEPROM.write(510,loByte);
         byte highByte = EEPROM.read(509);
         byte lowByte = EEPROM.read(510);
         cont4  = word(highByte, lowByte);
        }


  data = cont3 *0.01;

    if (data >= 100 && tecla == '\r')
  {

  last = "0";

  Serial.print ("    ");

  dtostrf (data, 6, 2, dataf);

  last += dataf;

  Serial.print (last);

  Serial.println ("   mm");

  }

  if ( data >= 10 && data < 100 && tecla == '\r' )
  {

  last = "00";

  Serial.print ("    ");

  dtostrf (data, 5, 2, dataf);

  last += dataf;

  Serial.print (last);

  Serial.println ("   mm");

  }
  if (data < 10 && tecla == '\r')
  {

  last = "000";

  Serial.print ("    ");

  dtostrf (data, 4, 2, dataf);

  last += dataf;

  Serial.print (last);

  Serial.println ("   mm");

  }


}


void ai0()
{

  if (digitalRead(3) == HIGH && digitalRead(2) == LOW) {
  counterAB ++;
  }
  if (digitalRead(3) == LOW && digitalRead(2) == HIGH) {
  counterAB ++;
  }
  if (digitalRead(3) == LOW && digitalRead(2) == LOW) {
  counterAB --;
  }
  if (digitalRead(3) == HIGH && digitalRead(2) == HIGH) {
  counterAB --;
  }

}

void ai1()

{
  if (digitalRead(2) == LOW && digitalRead(3) == HIGH) {
    counterAB --;
  }
  if (digitalRead(2) == HIGH && digitalRead(3) == LOW) {
    counterAB --;
  }
  if (digitalRead(3) == LOW && digitalRead(2) == LOW) {
    counterAB ++;
  }
  if (digitalRead(3) == HIGH && digitalRead(2) == HIGH) {
    counterAB ++;
  }
}
