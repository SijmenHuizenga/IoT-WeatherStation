#include "Led.h"
#include "Debug.h"
#include "Sensors.h"
#include <EEPROM.h>

#define LEDEEPROMADRESSR 150
#define LEDEEPROMADRESSG 180

Led* led = new Led();

void Led::setupLed() {
  pinMode(REDLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(YELLOWLED, LOW);
  digitalWrite(GREENLED, LOW);

  EEPROM.get(LEDEEPROMADRESSR, this->treshRed);
  EEPROM.get(LEDEEPROMADRESSG, this->treshGreen);

  if(this->treshRed < 0 || this->treshRed > 100 || this->treshGreen < 0 || this->treshGreen > 100 || this->treshRed <= this->treshGreen || isnan(this->treshGreen) || isnan(this->treshRed)){
    debugln(F("Read tresholds from eeprom. Values are invalid. Setting to default"), LED);
    this->setTreshGreen(18);
    this->setTreshRed(23);
  }else{
    debug(F("Read tresholds from eeprom. Values are "), LED);
    bebug(this->getTreshGreen(), LED);
    bebug(" => ", LED);
    bebugln(this->getTreshRed(), LED);
  }

}

void Led::setTreshRed(float temp) {
  EEPROM.put(LEDEEPROMADRESSR, temp);
  this->treshRed = temp;
}


void Led::setTreshGreen(float temp) {
  EEPROM.put(LEDEEPROMADRESSG, temp);
  this->treshGreen = temp;
}


void Led::setTemp(float temp) {
  this->temperature = temp;
}

float Led::getTreshGreen() {
  return this->treshGreen;
};

float Led::getTreshRed() {
  return this->treshRed;
};


void Led::updateLed() {
  setTemp(sensors->getTemperature());

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

