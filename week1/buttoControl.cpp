#include <Arduino.h>


const int ldrPin     = 34;
const int buttonPin  = 19;
const int ledPin     = 2;     // led on board
const int threshold  = 2000;  // nguong threshhold cua cam bien

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // ban dau led tat
}

void loop() {
  // nhan nut thi tro pullup = low
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);  // => led va cam bien chay
    int lightValue  = analogRead(ldrPin);
    // gia tri doc duoc tu sensor > 2000 thi output = 1 else = 0
    int sensorState = (lightValue > threshold) ? 1 : 0;
    Serial.println(sensorState);
  } else {
    // khong nhan => led va sensor ko chay
    digitalWrite(ledPin, LOW);
  }

  delay(100);
}
