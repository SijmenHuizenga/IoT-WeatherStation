#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"

EthernetClient client;

NetServerStatus serverstate;

void sendWeather();
void sendHeader(String headerLength);

void sendWeatherToGateway(void) {
  if(serverstate == NET_WAITING)
      serverstate = NET_CONNETING;
}

void updateHttpClient() {
  if(serverstate == NET_RECEIVING){
      bool readSomething = false;
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
      if (client.connect(gatewayIp, 8080)) {
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
