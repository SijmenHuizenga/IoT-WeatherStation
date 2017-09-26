#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
IPAddress server(192, 168, 178, 55); // Gateway IP
IPAddress ip(192, 168, 178, 200);
EthernetClient client;

NetServerStatus serverstate;

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
  if(serverstate == NET_WAITING)
      serverstate = NET_CONNETING;
}

void updateHttpClient() {
  if(serverstate == NET_RECEIVING){
      bool readSomething = false;
      Serial.println("listening");
      while (client.available()) {
        char c = client.read();
        debug((String)c, NETWORK);
        readSomething = true;
      }
      if(readSomething)
        debug("\n", NETWORK);
      if (!client.connected()) {
        debugln("disconnecting.", NETWORK);
        client.stop();
        serverstate = NET_WAITING;
      }
  } else if(serverstate == NET_CONNETING){
      if (client.connect(server, 8080)) {
        sendWeather();
        serverstate = NET_RECEIVING;
      } else 
        debugln("connection failed", NETWORK);
  }
}

void sendWeather(){
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

  sendHeader((String)body.length()); //todo: static body length?

  client.println((String)body);
  debugln("Sent body: " + (String)body, NETWORK);
}

void sendHeader(String bodyLength) {
  client.println("PUT /child/" + (String)getChildID() + "/measurements HTTP/1.1");
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " +  bodyLength);
  client.println("Connection: close");
  client.println();
}

