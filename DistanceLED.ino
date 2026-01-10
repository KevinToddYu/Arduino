#include <SR04.h>
#include <LiquidCrystal.h>
#define TRIG_PIN 4
#define ECHO_PIN 3
int LED = 2;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void setup() {
  lcd.begin(16, 2);
  lcd.print("Distance (CM):");
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
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(a/1);
  delay(1000);
}

