#include "Button.h"
#include "HttpClient.h"

Button* resetButton = new Button();

void Button::setupButton() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
}

void Button::readButton() {
  int reading = digitalRead(BUTTONPIN);
  if (reading != this->lastButtonState) {
    this->lastDebounceTime = millis();
  }
  if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
    if (reading != buttonState) {
      this->buttonState = reading;
      if (buttonState == LOW) {
        this->buttonAction();
      }
    }
  }
  this->lastButtonState = reading;
}

void Button::buttonAction() {
  httpClient->resetChildID();
}

