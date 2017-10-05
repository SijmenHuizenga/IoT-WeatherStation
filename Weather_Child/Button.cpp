#include "Button.h"
#include "HttpClient.h"
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setupButton() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
}

void readButton() {
  int reading = digitalRead(BUTTONPIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        buttonAction();
      }
    }
  }
  lastButtonState = reading;
}

void buttonAction() {
  resetChildID();
}

