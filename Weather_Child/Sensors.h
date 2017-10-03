#pragma once

#include "SparkFun_Si7021_Breakout_Library.h"
//#include <Wire.h>

#define DEBUGSENSORS
#define TEMPCALIBRATION -1


void setupSensors(void);
float getHumidity(void);
float getTemperature(void);
float getBrightness(void);
