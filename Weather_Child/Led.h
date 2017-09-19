#pragma once
#define REDLED 4
#define YELLOWLED 3
#define GREENLED 2
#define LEDCOUNT 3

class Led {
  private:
    float temperature;
    float treshGreen;
    float treshRed;
  public:
    void setupLed();
    void setTreshRed(float temp);
    void setTreshGreen(float temp);
    void loadTemp(float temp);
    void updateLed();
};
