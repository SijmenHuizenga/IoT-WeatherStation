#pragma once

#include "Debug.h"
#include "Network.h"
#include "HttpClient.h"

#ifdef ETHERNETSHIELDv2
void connectEthernet(void) {
  while (1) {
    if (Ethernet.begin(myMac, myIp))
      break;
    debugln(F("Starting ethernet failed. Retrying..."), WEBCLIENT);
    delay(1000);
  }
}
#else
IPAddress myIp(192, 168, 178, 78);
byte myMac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02};

void connectNetwork(void) {
  Ethernet.begin(myMac, myIp);
}
#endif


int getHttpStatusCode(char* line) {
  if (line[0] == 'H' && line[1] == 'T' && line[2] == 'T' && line[3] == 'P' && line[4] == '/') { // detect line HTTP/1.1 200
    char statuscodestring[4] = {line[9], line[10], line[11], 0 };
    return atoi(statuscodestring);
  }
  return -1;
}

String getIpAddress(IPAddress address) {
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

void clearBuffer(char *buffer, int size) {
  for (int i = 0; i < size + 1; i++)
    buffer[i] = '\0';
}

bool startsWith(char *line, const char *needle) {
  for (int i = 0; needle[i] != 0; i++) {
    if (line[i] != needle[i])
      return false;
  }
  return true;
}
