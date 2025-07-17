#include<Arduino.h>

const int ledPin         = 2;
const int pwmChannel     = 0;
const int pwmFrequency   = 5000;
const int pwmResolution  = 8;    // 8 bit => resolution 0 - 255
const int maxBrightness  = 255;
const int stepDelay      = 10;

void setup() {
  pinMode(ledPin, OUTPUT);

  // void ledcSetup(channel, frequency, resolution) 8 = 255
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);  // Gan led vs pwm
}

void loop() {
  // Tang do sang led
  for (int duty = 0; duty <= maxBrightness; duty++) {
    ledcWrite(pwmChannel, duty);  // void ledcWrite(chan, duty)
    delay(stepDelay);
  }

  // Giam do sang led
  for (int duty = maxBrightness; duty >= 0; duty--) {
    ledcWrite(pwmChannel, duty);
    delay(stepDelay);
  }
}
