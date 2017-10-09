#define BUTTONPIN 5
#include <Arduino.h>
class Button {
  private:
    int buttonState;
    int lastButtonState = LOW;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;    
  public:
    void setupButton();
    void readButton();
    void buttonAction();
};

extern Button *resetButton;
