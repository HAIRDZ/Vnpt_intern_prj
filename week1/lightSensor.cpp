#include <Arduino.h>

const int ldrPin = 34;

void setup() {
  Serial.begin(19200);
  pinMode(ldrPin, INPUT);
}

void loop() {
  // doc gia tri cam bien
  int lightValue = analogRead(ldrPin);

  if (lightValue > 2000) {
    Serial.println(1);  // 1 = co anh sang
  } else {
    Serial.println(0);  // 0 = anh sang yeu
  }

  delay(1000);
}
