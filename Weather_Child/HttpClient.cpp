#pragma once

#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include "Json.h"
#include <EEPROM.h>;
#include <Arduino.h>

#define READBUFFERSIZSE 40

EthernetClient client;

int responseStatusCode = -1;

int childID = -1;
unsigned long serverTime = 0;
unsigned long serverTimeSync = 0;


String getIpAddress(IPAddress address);

void sendWeatherToGateway(void) {
  if (childID != -1 && serverTime != 0 && serverstate == NET_WAITING) {
    serverstate = NET_CONNETING;
    requeststate = SENDDATA;
  }
}

void clientConnectAndSend() {
  if (client.connect(gatewayIp, 8080)) {
    switch (requeststate) {
      case REGISTER:
        sendRegister();
        break;
      case LOGIN:
        sendLogin();
        break;
      case SENDDATA:
        sendWeather();
        break;
    }
    serverstate = NET_RECEIVING;
    responseStatusCode = -1;
  } else
    debugln(F("connection failed"), NETWORK);
}

void sendWeather() {
  String body = "{";
#ifdef SENDHUMID
  body.concat("\"humidity\": ");
  body.concat(getHumidity());
#endif
#ifdef SENDTEMP
  body.concat(",\"temperature\": ");
  body.concat(getTemperature());
#endif
#ifdef SENDBRIGHTNESS
  body.concat(",\"brightness\": ");
  body.concat(getBrightness() / 10);
#endif
  body.concat(",\"timestamp\": ");
  body.concat(getTime());
  body.concat("}");

  sendWeatherHeader(body.length()); //todo: static body length?
  client.println(body);

  debug(F("Sent body: "), NETWORK);
  bebugln(body, NETWORK);
}

void sendWeatherHeader(unsigned int bodyLength) {
  client.println("PUT /child/" + String(childID) + "/measurements HTTP/1.1");
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(bodyLength);
  client.println("Connection: close");
  client.println();
}

void loginToGateway() {
  int eepromid = EEPROM.read(0);
  if (eepromid > 0) {
    debugln("eepromid =" + (String) eepromid, NETWORK);
    childID = eepromid;
    requeststate = LOGIN;
  } else {
    requeststate = REGISTER;
  }
  serverstate = NET_CONNETING;
}

void sendLogin() {
  client.print(F("POST /child/"));
  client.print(String(childID));
  client.println(F("/login HTTP/1.1"));
  debugln("POST /child/" + String(childID) + "/login HTTP/1.1", NETWORK);
  sendLoginRegisterThings();
}

void sendRegister() {
  client.println(F("POST /child/register HTTP/1.1"));
  debugln(F("POST /child/register HTTP/1.1"), NETWORK);
  sendLoginRegisterThings();
}

void sendLoginRegisterThings() {
  String body = "{";
  body = body + "\"ip\": \"" + getIpAddress(Ethernet.localIP());
  body = body + "\"}";

  client.println(F("Host: 192.168.178.55:8080"));
  client.println(F("Content-Type: application/json"));
  client.print(F("Content-Length: "));
  client.println(body.length());

  client.println(F("Connection: close \n"));
  client.println(body);

  debugln(body, NETWORK);
}

void clientReceiveAndClose() {
  unsigned int charBufferCount = 0;

  //one char extra as termination byte
  char *lineBuffer = (char *) calloc('\0', (READBUFFERSIZSE + 1) * sizeof(char));

  bool inBody = false;

  while (client.available()) {
    char c = client.read();
    if (c != '\n') {
      lineBuffer[charBufferCount] = c;
      charBufferCount++;
    }
    if (c == '\n' || charBufferCount == READBUFFERSIZSE - 1) {
      //check for two newlines, than we are in buffer
      if (charBufferCount == 1 && lineBuffer[0] == '\r') {
        inBody = true;
        continue;
      }

      if (inBody) {
        handleBodyPart(lineBuffer);
      } else {
        int code = getHttpStatusCode(lineBuffer);
        if (code != -1)
          responseStatusCode = code;
      }

      charBufferCount = 0;
      for (int i = 0; i < READBUFFERSIZSE + 1; i++)
        lineBuffer[i] = '\0';
    }
  }
  if (charBufferCount > 0)
    handleBodyPart(lineBuffer);
  free(lineBuffer);

  if (serverstate == NET_RECEIVING && !client.connected()) {
    debugln("disconnecting.", NETWORK);
    client.stop();
    serverstate = NET_WAITING;
  }
}

void handleBodyPart(char *lineBuffer) {
  switch (requeststate) {
    case REGISTER:
      readRegisterResponseLine(lineBuffer);
      break;
    case LOGIN:
      readLoginResponseLine(lineBuffer);
      break;
    case SENDDATA:
      readWeatherResponseLine(lineBuffer);
      break;
  }
}

void readRegisterResponseLine(char *lineBuffer) {
  if (responseStatusCode != 200) {
    logAndRetry(lineBuffer);
    return;
  }

  if (lineBuffer[0] == '{') {
    byte id = readIdFromJson(lineBuffer);
    debugln(id, NETWORK);
    if (id > 0) {
      EEPROM.write(0, id);
      childID = id;
      debug(F("ONTVANGEN ID:"), NETWORK);
      bebugln(id, NETWORK);
    } else {
      logAndRetry(lineBuffer);
      return;
    }

    unsigned long ti = readTimeFromJson(lineBuffer);

    requeststate = LOGIN;
    handleTimePartOfLoginRegisterResponse(lineBuffer);
  }
}

void readLoginResponseLine(char *lineBuffer) {
  if (responseStatusCode != 200) {
    logAndRetry(lineBuffer);
    return;
  }
  if (lineBuffer[0] == '{') {
    handleTimePartOfLoginRegisterResponse(lineBuffer);
  }
}

void logAndRetry(char* lineBuffer) {
  debugln(lineBuffer, NETWORK);
  debugln(F("Login/Register failed, retrying"), NETWORK);
  client.stop();
  serverstate = NET_CONNETING;
}

void handleTimePartOfLoginRegisterResponse(char* lineBuffer) {
  unsigned long ti = readTimeFromJson(lineBuffer);
  debug(F("ONTVANGEN TIJD:"), NETWORK);
  bebugln(ti, NETWORK);
  if (ti < 1) {
    logAndRetry(lineBuffer);
  } else {
    serverTime = ti;
    serverTimeSync = millis();
  }
}

void readWeatherResponseLine(char *lineBuffer) {
  if (responseStatusCode != 200)
    debugln(lineBuffer, NETWORK);
}

unsigned long getTime() {
  return serverTime + ((millis() - serverTimeSync) / 1000);
}

void resetChildID() {
  EEPROM.write(0, 0);
  debugln(F("Child ID has been reset, requesting new."), NETWORK);
  client.stop();
  serverstate = NET_CONNETING;
  requeststate = REGISTER;

  childID = -1;
  serverTime = 0;
  serverTimeSync = 0;
}

