#pragma once
#include <Ethernet2.h>

#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS


void connectEthernet(void);
void sendWeatherToGateway(void);
void sendHeader(String headerLength);

