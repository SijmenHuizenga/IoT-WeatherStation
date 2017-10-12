#include "Network.h"
#include "Sensors.h"
#include "Button.h"
#include "Debug.h"
#include "Led.h"
#include "Timer.h"
#include "HttpClient.h"
#include "HttpServer.h"
#include "configure.h"

TimedAction *sendWeatherDataTimer = new TimedAction();
TimedAction *updateLedsTimer = new TimedAction();
TimedAction *updateHttpClientTimer = new TimedAction();
TimedAction *updateHttpServerTimer = new TimedAction();

void setup() {
  Serial.begin(9600);
  conf->configureChild();
  sensors->setupSensors();
  led->setupLed();
  resetButton->setupButton();
  network->connectNetwork();
  httpClient->loginToGateway();
  httpServer->startHttpServer();

  sendWeatherDataTimer->setDelay(5000);
  sendWeatherDataTimer->setCallback(callSendWeatherToGateway);

  updateHttpClientTimer->setDelay(250);
  updateHttpClientTimer->setCallback(callUpdateHttpClient);

  updateHttpServerTimer->setDelay(240);
  updateHttpServerTimer->setCallback(callUpdateHttpServer);

  updateLedsTimer->setDelay(400);
  updateLedsTimer->setCallback(callUpdateLed);
}

void loop() {
  resetButton->readButton();
  updateHttpClientTimer->update();
  sendWeatherDataTimer->update();
  updateLedsTimer->update();
  updateHttpServerTimer->update();
}

void callUpdateLed() {
  led->updateLed();
}

void callSendWeatherToGateway() {
  httpClient->sendWeatherToGateway();
}

void callUpdateHttpClient() {
  httpClient->updateHttpClient();
}

void callUpdateHttpServer() {
  httpServer->updateHttpServer();
}