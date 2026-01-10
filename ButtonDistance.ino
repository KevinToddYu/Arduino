#include "arduino_secrets.h"

#include "SR04.h"
#define TP 4
#define EP 3
#define buttonpin 5
int LEDW = 6;
int LED = 2;
byte leds = 0;
SR04 sr04 = SR04(EP, TP);
long a;
void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(buttonpin, INPUT);
  pinMode(LEDW, OUTPUT);
  digitalWrite(LEDW, LOW);
  }

void loop() {
  if (digitalRead(buttonpin) == HIGH) {
    a = sr04.Distance();
    digitalWrite(LED, HIGH);
    Serial.print(a);
    Serial.println('cm');
    if (a < 30) {
      digitalWrite(LEDW, HIGH);
    } else {
      digitalWrite(LEDW, LOW);
    }
  }
  delay(100);
}