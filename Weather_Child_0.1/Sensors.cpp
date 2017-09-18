#pragma once
#include "Sensors.h"

void setupSensors(void) {
  sensor.begin();
}

float getHumidity(void) {
  return sensor.getRH();
}

float getTemperature(void) {
  return sensor.getTemp();
}

int getStationID(void) {
  return stationID;
}

