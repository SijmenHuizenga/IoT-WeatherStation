#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
IPAddress gatewayIp(192, 168, 178, 55);

IPAddress ip(192,168,178,103);

void connectEthernet(void) {
//  while(1) {
//    if(Ethernet.begin()) 
//      break;
//    debugln("Starting ethernet failed. Retrying...", NETWORK);
//    delay(1000);
//  }
  Ethernet.begin(myMac, ip);
  debug("Starting ethernet finished, IP addres: ", NETWORK);
  Serial.println(Ethernet.localIP()); // to do: debug

  delay(50);
}


