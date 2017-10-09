#pragma once

#include <Ethernet2.h>

#define READBUFFERSIZSE 40

class Network {
  private:
    byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  public:
    void connectNetwork(void);
    //utility functionalities:
    int getHttpStatusCode(char* line);
    String getIpAddress(IPAddress address);
    void clearBuffer(char *buffer, int size);
    bool startsWith(char* line, const char *needle);
};

extern Network *network;

