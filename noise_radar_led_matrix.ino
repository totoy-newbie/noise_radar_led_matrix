
#include <Arduino.h>
#include "led_display.h"

#define UPDATE_INTERVAL 1000
unsigned long lastTickTime;

void setup() {
  Serial.begin(115200);
  LedInit();
  lastTickTime = millis();
}

void loop() {
  unsigned long msNow = millis();

  if (msNow - lastTickTime >= UPDATE_INTERVAL) {
    LedCycle();
    lastTickTime = msNow;
  }
}
