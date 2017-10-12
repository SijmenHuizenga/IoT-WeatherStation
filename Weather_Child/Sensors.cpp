#include "Debug.h"
#include "Sensors.h"
// static for now
Weather sensor;

Sensors* sensors = new Sensors();

void Sensors::setupSensors(void) {
  sensor.begin();
}

float Sensors::getHumidity(void) {
  float data = sensor.getRH();
  return data;

}

float Sensors::getTemperature(void) {
  float data = sensor.getTemp() + TEMPCALIBRATION;
  return data;
}


float Sensors::getBrightness(void) {
  float data = analogRead(A0);
  return data;
}


