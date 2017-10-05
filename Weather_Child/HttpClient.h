#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS

void sendWeatherToGateway();

void loginToGateway();

void resetChildID();

void clientConnectAndSend();
void clientReceiveAndClose();
void handleBodyPart(char *lineBuffer);

void sendWeather();
void sendWeatherHeader(unsigned int headerLength);

void sendLogin();
void sendRegister();
void sendLoginRegisterThings();

void readLoginResponseLine(char* lineBuffer);
void readRegisterResponseLine(char* lineBuffer);
void logAndRetry(char* lineBuffer);
void handleTimePartOfLoginRegisterResponse(char* lineBuffer);

void readWeatherResponseLine(char* lineBuffer);

unsigned long getTime();



