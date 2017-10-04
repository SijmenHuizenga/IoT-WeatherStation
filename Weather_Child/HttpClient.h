#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS

enum NetServerStatus { NET_CONNETING, NET_RECEIVING, NET_WAITING };
enum NetType {REGISTER, LOGIN, SENDDATA};

void updateHttpClient();
void sendWeatherToGateway();
void loginToGateway();

struct Range {
  int start;
  int end;
};

Range findJsonFieldRange(char *json, char *field);



