#pragma once
#include <Ethernet2.h>

#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS

enum NetServerStatus { NET_CONNETING, NET_RECEIVING, NET_WAITING } ;

void connectEthernet(void);
void updateNetServer();
void sendWeatherToGateway(void);
void sendWeather();
void sendHeader(String headerLength);

