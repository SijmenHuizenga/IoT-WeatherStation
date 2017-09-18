#pragma once
#include "Network.h"
#include "Sensor.h"


void setup() {
  Serial.begin(9600);
  setupSensors();
  connectEthernet();
}

void loop() {
  delay(5000);
  sendWeatherToGateway();
}
