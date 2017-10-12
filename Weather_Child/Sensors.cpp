#include "Debug.h"
#include "Sensors.h"
// static for now
Weather sensor;

Sensors* sensors = new Sensors();

void Sensors::setupSensors(void) {
  sensor.begin();
  debugln(F("Sensors started"), SENSOR);
}

float Sensors::getHumidity(void) {
  float data = sensor.getRH();
  debugln(data, SENSOR);
  return data;

}

float Sensors::getTemperature(void) {
  float data = sensor.getTemp() + TEMPCALIBRATION;
  debugln(data, SENSOR);
  return data;
}


float Sensors::getBrightness(void) {
  float data = analogRead(A0);
  debugln(data, SENSOR);
  return data;
}


