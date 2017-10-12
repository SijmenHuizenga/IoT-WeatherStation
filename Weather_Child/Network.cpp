#include "Debug.h"
#include "Network.h"
#include "HttpClient.h"
#include "configure.h"

Network* network = new Network();

void Network::connectNetwork(void) {
  debugln(F("Setting up network..."), WEBCLIENT);
  while (1) {
    if (Ethernet.begin(conf->mac))
      break;
    debugln(F("Starting ethernet failed. Retrying..."), WEBCLIENT);
    delay(1000);
  }
  debug("GatewayIP is: ", WEBCLIENT); bebugln(this->gatewayIp, WEBCLIENT);
  this->gatewayIp = IPAddress(192, 168, 178, (int)conf->ip);
  debug("Set gatewayIP to: ", WEBCLIENT); bebugln(this->gatewayIp, WEBCLIENT);
}


int Network::getHttpStatusCode(char* line) {
  if (line[0] == 'H' && line[1] == 'T' && line[2] == 'T' && line[3] == 'P' && line[4] == '/') { // detect line HTTP/1.1 200
    char statuscodestring[4] = {line[9], line[10], line[11], 0 };
    return atoi(statuscodestring);
  }
  return -1;
}

String Network::getIpAddress(IPAddress address) {
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

void Network::clearBuffer(char *buffer, int size) {
  for (int i = 0; i < size + 1; i++)
    buffer[i] = '\0';
}

bool Network::startsWith(char *line, const char *needle) {
  for (int i = 0; needle[i] != 0; i++) {
    if (line[i] != needle[i])
      return false;
  }
  return true;
}
