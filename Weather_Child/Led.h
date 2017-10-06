#pragma once
#include <Arduino.h>

#define REDLED 4
#define YELLOWLED 3
#define GREENLED 2


class Led {
  private:
    float temperature;
    float treshGreen;
    float treshRed;
    void setLedOnOrOff(byte lednr, bool ison);
  public:
    void setupLed();
    void setTreshRed(float temp);
    void setTreshGreen(float temp);
    float getTreshGreen();
    float getTreshRed();
    void setTemp(float temp);
    void updateLed();
};

extern Led *led;