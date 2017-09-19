#pragma once
#include "Network.h"
#include "Sensors.h"
#include "Debug.h"
#include "Led.h"
#include "Timer.h"

TimedAction* sendWeatherDataTimer = new TimedAction();
TimedAction* updateLedsTimer = new TimedAction();

Led* led = new Led();


void setup() {
  led->setTreshGreen(23.5);
  led->setTreshRed(24.8);
 
  startDebugging();
  setupSensors();
  connectEthernet();
  led->setupLed();

  sendWeatherDataTimer->setDelay(5000);
  sendWeatherDataTimer->setCallback(sendWeatherToGateway);

  updateLedsTimer->setDelay(400);
  updateLedsTimer->setCallback(updateLedCallback);
}

void loop() {
  sendWeatherDataTimer->update();
  updateLedsTimer->update();
}

void updateLedCallback(){
  led->updateLed();
}


