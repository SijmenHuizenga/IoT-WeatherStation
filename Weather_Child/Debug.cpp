#include "Debug.h"
#include <Arduino.h>

void startDebugging() {
#if defined(DEBUGSENSOR) || defined(DEBUGWEBCLIENT) || defined(DEBUGLED) || defined(DEBUGWEBSERVER)
  Serial.begin(BAUDRATE);
#endif
}

