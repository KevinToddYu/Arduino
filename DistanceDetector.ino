//www.elegoo.com
//2016.12.08
#include "SR04.h"
#define TRIG_PIN 4
#define ECHO_PIN 3
int LED = 2;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
   Serial.begin(9600);
   delay(100);
   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

}

void loop() {
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(100);
   if (a < 30) {
      digitalWrite(LED, HIGH);
   } else {
      digitalWrite(LED, LOW);
   }
}
