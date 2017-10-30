#include "Button.h"
#include "HttpClient.h"
#include "configure.h"

Button* resetButton = new Button();

void Button::setupButton() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
}

void Button::readButton() {
  int reading = digitalRead(BUTTONPIN);
  if (reading != this->lastButtonState) {
    this->lastDebounceTime = millis();
  }
  if ((millis() - this->lastDebounceTime) > this->DEBOUNCETIME) {
    if (reading != buttonState) {
      this->buttonState = reading;
      if (buttonState == LOW) {
        pressed = true;
      } else {
          if(pressed) {
              buttonActionShort();
          }
          pressed = false;
      }
    }
  }
    if((millis() - this->lastDebounceTime) > LONGPRESS && pressed) {
        pressed = false;
        buttonActionLong();

    }
  this->lastButtonState = reading;
}

void Button::buttonActionShort() {
    Serial.println("resetting ID");
    //httpClient->resetChildID();
}
void Button::buttonActionLong() {
    conf->abortChild();
}

