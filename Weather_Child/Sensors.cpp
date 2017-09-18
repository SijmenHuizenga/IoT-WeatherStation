#pragma once
#include "Sensors.h"

int updatesPerMinute = 15;

int stationID = 1; // static for now
Weather sensor;

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

