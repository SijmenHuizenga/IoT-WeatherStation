#pragma once
#include "Debug.h"
#include "Sensors.h"
 // static for now
Weather sensor;


void setupSensors(void) {
  sensor.begin();
  debugln("Sensors started", SENSOR);
}

float getHumidity(void) {
  float data = sensor.getRH();
  debugln(data, SENSOR);
  return data;
  
}

float getTemperature(void) {
  float data = sensor.getTemp() + TEMPCALIBRATION;
  debugln(data, SENSOR);
  return data;
}


float getBrightness(void) {
  float data = analogRead(A0);
  debugln(data, SENSOR);
  return data;
}


