#pragma once
#include <Ethernet2.h>


void connectEthernet(void);
void sendWeatherToGateway(void);
void sendHeader(String headerLength);
