#pragma once

#include <Ethernet2.h>

#define READBUFFERSIZSE 20

class Network {
  private:
  public:
    void connectNetwork(void);
    //utility functionalities:
    int getHttpStatusCode(char* line);
    String getIpAddress(IPAddress address);
    void clearBuffer(char *buffer, int size);
    bool startsWith(char* line, const char *needle);
    IPAddress gatewayIp;
};

extern Network *network;

