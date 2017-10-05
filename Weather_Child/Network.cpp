#pragma once

#include "Debug.h"
#include "Network.h"
#include "HttpClient.h"

IPAddress gatewayIp(192, 168, 178, 55);
NetServerStatus serverstate = NET_WAITING;
NetType requeststate;

#ifdef ETHERNETSHIELDv2
void connectEthernet(void) {
  while (1) {
    if (Ethernet.begin(myMac, myIp))
      break;
    debugln(F("Starting ethernet failed. Retrying..."), NETWORK);
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


void updateNetwork() {
  if (serverstate == NET_RECEIVING) {
    clientReceiveAndClose();
  } else if (serverstate == NET_CONNETING) {
    clientConnectAndSend();
  }
}

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
