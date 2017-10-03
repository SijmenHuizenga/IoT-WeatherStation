#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include <EEPROM.h>;

EthernetClient client;

NetServerStatus serverstate;

int childID;
bool hasID;

void sendWeather();
void sendHeader(String headerLength);
int getChildID();
String getIpAddress(IPAddress address);


void sendWeatherToGateway(void) {
  if (serverstate == NET_WAITING)
    serverstate = NET_CONNETING;
}

void updateHttpClient() {
  if (serverstate == NET_RECEIVING) {
    bool readSomething = false;
    while (client.available()) {
      char c = client.read();
      debug((String)c, NETWORK);
      readSomething = true;
    }
    if (readSomething)
      debug("\n", NETWORK);
    if (!client.connected()) {
      debugln("disconnecting.", NETWORK);
      client.stop();
      serverstate = NET_WAITING;
    }
  } else if (serverstate == NET_CONNETING) {
    if (client.connect(gatewayIp, 8080)) {
      sendWeather();
      serverstate = NET_RECEIVING;
    } else
      debugln("connection failed", NETWORK);
  }
}

void sendWeather() {
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

void loginToGateway() {
  while (!client.connect(gatewayIp, 8080)) {
    delay(100);
  }
  int eepromid = EEPROM.read(0);
  String body = "{"; body = body + "\"ip\": \"" + getIpAddress(Ethernet.localIP()); body = body + "\"}";
  if (eepromid > 0) {
    hasID = true;
    client.println("PUT /child/" + (String)getChildID() + "/login HTTP/1.1");
    debugln("PUT /child/" + (String)getChildID() + "/login HTTP/1.1", NETWORK);
  } else {
    hasID = false;
    client.println("POST /child/register HTTP/1.1");
    debugln("PUT /child/register HTTP/1.1", NETWORK);
  }
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + (String)body.length());
  client.println("Connection: close");
  client.println();

  client.println((String)body);
  debugln("Sent body: " + (String)body, NETWORK);
  serverstate = NET_RECEIVING;

}

int getChildID() {
  int data = childID;
  return data;
}

String getIpAddress(IPAddress address)
{
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

