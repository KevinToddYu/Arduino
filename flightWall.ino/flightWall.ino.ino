#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

char buf[64];
int len = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Waiting for data");
}

// Print up to 16 chars, then pad with spaces to overwrite leftovers
void printPadded(const char* s) {
  int i = 0;
  for (; i < 16 && s[i] != '\0'; i++) lcd.print(s[i]);
  for (; i < 16; i++) lcd.print(' ');
}

// Expected frame:  @<length>:<line1>|<line2>
void handleLine() {
  buf[len] = '\0';

  if (buf[0] != '@') {
    Serial.println("BAD:no start marker");
    return;
  }

  char* colon = strchr(buf, ':');
  if (colon == NULL) {
    Serial.println("BAD:no colon");
    return;
  }
  *colon = '\0';
  int expected = atoi(buf + 1);
  char* payload = colon + 1;

  if ((int)strlen(payload) != expected) {
    Serial.print("BAD:length got ");
    Serial.print(strlen(payload));
    Serial.print(" expected ");
    Serial.println(expected);
    return;
  }

  char* sep = strchr(payload, '|');
  if (sep == NULL) {
    Serial.println("BAD:no separator");
    return;
  }
  *sep = '\0';

  lcd.setCursor(0, 0);
  printPadded(payload);
  lcd.setCursor(0, 1);
  printPadded(sep + 1);

  Serial.print("OK:");
  Serial.print(payload);
  Serial.print('|');
  Serial.println(sep + 1);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\r') continue;

    if (c == '\n') {
      handleLine();
      len = 0;
    } else if (len < (int)sizeof(buf) - 1) {
      buf[len++] = c;
    } else {
      len = 0;   // overflow: drop it, the length check will reject the remainder
    }
  }
}