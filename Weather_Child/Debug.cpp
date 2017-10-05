#pragma once
#include "Debug.h"
#include <Arduino.h>

void startDebugging() {
#if defined(DEBUGSENSOR) || defined(DEBUGNETWORK)
  Serial.begin(9600);
#endif
}

