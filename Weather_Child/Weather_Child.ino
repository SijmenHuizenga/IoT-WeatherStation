#pragma once
#include "Network.h"
#include "Sensors.h"
#include "Debug.h"

void setup() {
  startDebugging();
  setupSensors();
  connectEthernet();
}

void loop() {
  delay(5000);
  sendWeatherToGateway();
}
