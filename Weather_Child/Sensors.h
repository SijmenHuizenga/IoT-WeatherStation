#pragma once

#include "SparkFun_Si7021_Breakout_Library.h"
//#include <Wire.h>


void setupSensors(void);
float getHumidity(void);
float getTemperature(void);
int getStationID(void);

