#include "display_matrix.h"
#include "debug_print.h"

#define UPDATE_INTERVAL 1000
unsigned long lastTickTime;

void setup() {
  DebugInit();
  DisplayInit();
  lastTickTime = millis();
}

void loop() {
  unsigned long msNow = millis();

  if (msNow - lastTickTime >= UPDATE_INTERVAL) {
    DebugCycle();
    DisplayCycle();
    lastTickTime = msNow;
  }
}
