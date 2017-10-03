#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS

enum NetServerStatus { NET_CONNETING, NET_RECEIVING, NET_WAITING } ;

void updateHttpClient();
void sendWeatherToGateway();
void loginToGateway();
