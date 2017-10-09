#pragma once

#include "SparkFun_Si7021_Breakout_Library.h"
//#include <Wire.h>

#define TEMPCALIBRATION -1

class Sensors {
private:

public:
    void setupSensors(void);
    float getHumidity(void);
    float getTemperature(void);
    float getBrightness(void);
};

extern Sensors *sensors;