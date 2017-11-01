
#include <Arduino.h>
#define BUTTONPIN 5
#define LONGPRESS 5000
#define DEBOUNCETIME 50
class Button {
  private:
    bool pressed;
    bool buttonState;
    bool lastButtonState = false;
    unsigned long lastDebounceTime = 0;    
  public:
    void setupButton();
    void readButton();
    void buttonActionLong();
    void buttonActionShort();
};

extern Button *resetButton;
