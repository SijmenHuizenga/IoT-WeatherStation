#pragma once

#include <Ethernet2.h>

enum NetServerStatus { NET_CONNETING, NET_RECEIVING, NET_WAITING };
enum NetType {REGISTER, LOGIN, SENDDATA};

extern NetServerStatus serverstate;
extern NetType requeststate;

extern IPAddress gatewayIp;

void updateNetwork();
void connectNetwork(void);

//utility functionalities:
int getHttpStatusCode(char* line);
String getIpAddress(IPAddress address);

