#pragma once
#include "Network.h"
#include "Sensors.h"
#include "Debug.h"
#include "Led.h"

void setup() {
  Led led;
  startDebugging();
  setupSensors();
  connectEthernet();
  led.setupLed();
}

void loop() {
  delay(5000);
  sendWeatherToGateway();
  Led led;
  led.setTreshGreen(24.5);
  led.setTreshRed(24.8);
  led.updateLed();
}
