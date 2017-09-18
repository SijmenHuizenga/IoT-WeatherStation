#pragma once
#include "Network.h"
#include "Sensors.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };
IPAddress server(192, 168, 178, 55); // Gateway IP
IPAddress ip(192, 168, 178, 200);
EthernetClient client;

void connectEthernet(void) {
  Ethernet.begin(mac, ip);

  delay(1000);
  Serial.println("connecting...");

  if (client.connect(server, 8080)) {
    Serial.println("connected");
    client.stop();
  } else {
    Serial.println("connection failed");
  }
}

void sendWeatherToGateway(void) {
  if (client.connect(server, 8080)) {
    String body = "{\"childid\":" + (String)getStationID() + ",";
//    if (sendHumidity) {
      body = body + "\"humidity\": " + (String)getHumidity() + "}";
//    }
//    if (sendTemperature) {
//      body = body + "\"temperature\": " + (String)getTemperature() + "}";
//    }
//    if (sendBrightness) {
//      body = body + "\"brightness\": " + (String)getBrightness() + "}";
//    }


    sendHeader((String)body.length());

    client.println((String)body);
    Serial.println((String)body);
  } else {
    Serial.println("connection failed");
  }
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}

void sendHeader(String bodyLength) {
  client.println("PUT / HTTP/1.1");
  client.println("Host: 192.168.178.55:8080/child/1/measurements");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " +  bodyLength);
  client.println("Connection: close");
  client.println();
}

