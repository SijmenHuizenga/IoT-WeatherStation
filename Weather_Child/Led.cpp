#pragma once
#include "Led.h"
#include "Debug.h"
#include "Sensors.h"

Led* led = new Led();

void Led::setupLed() {
  pinMode(REDLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(YELLOWLED, LOW);
  digitalWrite(GREENLED, LOW);
}

void Led::setTreshRed(float temp) {
  this->treshRed = temp;
}


void Led::setTreshGreen(float temp) {
  this->treshGreen = temp;
}


void Led::setTemp(float temp) {
  this->temperature = temp;
}

float Led::getTreshGreen(){
  return this->treshGreen;
};

float Led::getTreshRed(){
  return this->treshRed;
};


void Led::updateLed() {
  setTemp(getTemperature());

  byte ledOn = 0;

  if (this->temperature < this->treshGreen)
    ledOn = GREENLED;
  else if (this->temperature >= this->treshGreen && this->temperature <= this->treshRed)
    ledOn = YELLOWLED;
  else if (this->temperature > this->treshRed)
    ledOn = REDLED;
  else {
    debug(F("TRESH GREEN SHOULD BE < THAN TRESH RED!!"), LED);
    return;
  }

  setLedOnOrOff(GREENLED, ledOn == GREENLED);
  setLedOnOrOff(YELLOWLED, ledOn == YELLOWLED);
  setLedOnOrOff(REDLED, ledOn == REDLED);
}

void Led::setLedOnOrOff(byte lednr, bool ison) {
  digitalWrite(lednr, ison ? HIGH : LOW);
}

