#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
long input = 0;
void setup() {
  lcd.begin(16, 2);
  lcd.print("Enter code:");
  Serial.begin(9600);
  delay(100);
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(input);
  delay(1000);
}

