#pragma once

#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

int updatesPerMinute = 15;

int stationID = 1; // static for now
Weather sensor;


void setupSensors(void);
float getHumidity(void);
float getTemperature(void);
int getStationID(void);
