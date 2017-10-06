#pragma once

#include "Network.h"
#include "Sensors.h"
#include "Button.h"
#include "Debug.h"
#include "Led.h"
#include "Timer.h"
#include "HttpClient.h"
#include "HttpServer.h"

#define ETHERNETSHIELDv2


TimedAction *sendWeatherDataTimer = new TimedAction();
TimedAction *updateLedsTimer = new TimedAction();
TimedAction *updateHttpClientTimer = new TimedAction();
TimedAction *updateHttpServerTimer = new TimedAction();

void setup() {
  startDebugging();
  setupSensors();
  setupButton();
  connectNetwork();
  loginToGateway();
  led->setupLed();
  startHttpServer();

  sendWeatherDataTimer->setDelay(5000);
  sendWeatherDataTimer->setCallback(sendWeatherToGateway);

  updateHttpClientTimer->setDelay(250);
  updateHttpClientTimer->setCallback(updateHttpClient);

  updateHttpServerTimer->setDelay(240);
  updateHttpServerTimer->setCallback(updateHttpServer);

  updateLedsTimer->setDelay(400);
  updateLedsTimer->setCallback(updateLedCallback);
}

void loop() {
  readButton();
  updateHttpClientTimer->update();
  sendWeatherDataTimer->update();
  updateLedsTimer->update();
  updateHttpServerTimer->update();
}

void updateLedCallback() {
  led->updateLed();
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
