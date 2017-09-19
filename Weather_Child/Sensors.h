#pragma once

#include "SparkFun_Si7021_Breakout_Library.h"
//#include <Wire.h>

#define DEBUGSENSORS


void setupSensors(void);
float getHumidity(void);
float getTemperature(void);
int getChildID(void);
float getBrightness(void);
