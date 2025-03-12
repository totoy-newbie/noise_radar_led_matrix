#include "display_matrix.h"

#define UPDATE_INTERVAL 1000
unsigned long lastTickTime;

void setup() {
  DisplayInit();
  lastTickTime = millis();
}

void loop() {
  unsigned long msNow = millis();

  if (msNow - lastTickTime >= UPDATE_INTERVAL) {
    DisplayCycle();
    lastTickTime = msNow;
  }
}
