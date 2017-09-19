#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
IPAddress server(192, 168, 178, 55); // Gateway IP
IPAddress ip(192, 168, 178, 200);
EthernetClient client;

void connectEthernet(void) {
  Ethernet.begin(mac, ip);

  delay(1000);
  debugln("connecting...", NETWORK);

  if (client.connect(server, 8080)) {
    debugln("connected", NETWORK);
    client.stop();
  } else {
    debugln("connection failed", NETWORK);
  }
}

void sendWeatherToGateway(void) {
  if (client.connect(server, 8080)) {
    String body = "{";
#ifdef SENDHUMID
   body = body + "\"humidity\": " + (String)getHumidity();
#endif
#ifdef SENDTEMP
   body = body + ",\"temperature\": " + (String)getTemperature();
#endif 
#ifdef SENDBRIGHTNESS
   body = body + ",\"brightness\": " + (String)getBrightness();
#endif
      body = body + "}";

    sendHeader((String)body.length());

    client.println((String)body);
    debugln((String)body, NETWORK);
  } else {
    debugln("connection failed", NETWORK);
  }
  while (client.available()) {
    char c = client.read();
    debug((String)c, NETWORK);
  }
  if (!client.connected()) {
    debugln("",NETWORK);
    debugln("disconnecting.", NETWORK);
    client.stop();
  }
}

void sendHeader(String bodyLength) {
  client.println("PUT /child/" + (String)getChildID() + "/measurements HTTP/1.1");
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " +  bodyLength);
  client.println("Connection: close");
  client.println();
}

