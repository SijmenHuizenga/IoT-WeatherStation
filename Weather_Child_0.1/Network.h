#pragma once
#include <Ethernet2.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
IPAddress server(192, 168, 178, 55); // Gateway IP
IPAddress ip(192, 168, 178, 200);
EthernetClient client;

void connectEthernet(void);
void sendWeatherToGateway(void);
void sendHeader(String headerLength);
