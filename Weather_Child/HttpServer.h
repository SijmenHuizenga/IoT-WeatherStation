#pragma once
#include "Debug.h"
#include "Network.h"

void startHttpServer(void) {
  server.begin();
  debugln("server is at " + Ethernet.localIP(), NETWORK);
  debug("server is at ", NETWORK);
}
