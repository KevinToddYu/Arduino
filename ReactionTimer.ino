int LED = 2;
int buttonpin = 3;
int times[] = {0,0,0,0,0};
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < (sizeof(times) / sizeof(times[0])); i++) {
    delay(random(500, 5000));
    digitalWrite(LED, HIGH);
    unsigned long timeinput = micros();
    Serial.print(timeinput/1000);
    Serial.printIn(" ms");
    digitalWrite(LED, LOW);
  }
  
}
