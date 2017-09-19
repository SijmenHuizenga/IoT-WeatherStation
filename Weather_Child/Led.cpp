#pragma once
#include "Led.h"
#include "Sensors.h"

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


void Led::loadTemp(float temp) {
  this->temperature = temp;
}


void Led::updateLed() {
  Led::loadTemp(getTemperature());
  if (this->temperature < this->treshGreen) {
    Serial.println("testtesttest1");
    digitalWrite(GREENLED, HIGH);
    digitalWrite(YELLOWLED, LOW);
    digitalWrite(REDLED, LOW);
  } else if (this->temperature >= this->treshGreen && this->temperature <= this->treshRed){
    Serial.println("testtesttest2");
    digitalWrite(GREENLED, LOW);
    digitalWrite(YELLOWLED, HIGH);
    digitalWrite(REDLED, LOW);
  } else if (this->temperature > this->treshRed){
    Serial.println("testtesttest3");
    digitalWrite(GREENLED, LOW);
    digitalWrite(YELLOWLED, LOW);
    digitalWrite(REDLED, HIGH);
  } else {
    Serial.println("testtesttest");
    digitalWrite(GREENLED, HIGH);
    digitalWrite(YELLOWLED, HIGH);
    digitalWrite(REDLED, HIGH);
  }

  
}

