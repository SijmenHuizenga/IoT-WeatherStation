#define BUTTONPIN 5
#include <Arduino.h>
class Button {
  private:
    bool pressed;
    int longPress = 5000;
    bool buttonState;
    bool lastButtonState = false;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;    
  public:
    void setupButton();
    void readButton();
    void buttonActionLong();
    void buttonActionShort();
};

extern Button *resetButton;
