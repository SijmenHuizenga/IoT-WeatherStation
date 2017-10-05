#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

IPAddress gatewayIp(192, 168, 178, 55);

void connectEthernet(void) {
  while(1) {
    if(Ethernet.begin()) 
      break;
    debugln(F("Starting ethernet failed. Retrying..."), NETWORK);
    delay(1000);
  }
}


