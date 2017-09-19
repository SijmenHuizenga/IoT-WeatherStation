#pragma once
#include "Debug.h"
#include "Sensors.h"

int stationID = 7; // static for now
Weather sensor;


void setupSensors(void) {
  sensor.begin();
  debugln("Sensors started", SENSOR);
}

float getHumidity(void) {
  float data = sensor.getRH();
  debugln((String)data, SENSOR);
  return data;
  
}

float getTemperature(void) {
  float data = sensor.getTemp();
  debugln((String)data, SENSOR);
  return data;
}

int getChildID(void) {
  int data = stationID;
  debugln((String)data, SENSOR);
  return data;

}

float getBrightness(void) {
  float data = analogRead(A0);
  debugln((String)data, SENSOR);
  return data;
}


