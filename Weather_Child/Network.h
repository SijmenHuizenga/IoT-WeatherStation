#pragma once

#include <Ethernet2.h>

#define READBUFFERSIZSE 40

void connectNetwork(void);

//utility functionalities:
int getHttpStatusCode(char* line);
String getIpAddress(IPAddress address);

void clearBuffer(char *buffer, int size);
bool startsWith(char* line, const char *needle);
