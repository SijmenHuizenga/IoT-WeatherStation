#pragma once

#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include "Json.h"
#include <EEPROM.h>;
#include <Arduino.h>

EthernetClient client;

IPAddress gatewayIp(192, 168, 178, 55);
NetClientStatus clientstate = NET_WAITING;
NetRequestType requesttypestate;
int responseStatusCode = -1;

int childID = -1;
unsigned long serverTime = 0;
unsigned long serverTimeSync = 0;

void updateHttpClient() {
  if (clientstate == NET_RECEIVING) {
    clientReceiveAndClose();
  } else if (clientstate == NET_CONNETING) {
    clientConnectAndSend();
  }
}

void sendWeatherToGateway(void) {
  if (childID != -1 && serverTime != 0 && clientstate == NET_WAITING) {
    clientstate = NET_CONNETING;
    requesttypestate = SENDDATA;
  }
}

void clientConnectAndSend() {
  if (client.connect(gatewayIp, 8080)) {
    switch (requesttypestate) {
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
    clientstate = NET_RECEIVING;
    responseStatusCode = -1;
  } else
    debugln(F("connection failed"), WEBCLIENT);
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

  debug(F("Sent body: "), WEBCLIENT);
  bebugln(body, WEBCLIENT);
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
    debugln("eepromid =" + (String) eepromid, WEBCLIENT);
    childID = eepromid;
    requesttypestate = LOGIN;
  } else {
    requesttypestate = REGISTER;
  }
  clientstate = NET_CONNETING;
}

void sendLogin() {
  client.print(F("POST /child/"));
  client.print(String(childID));
  client.println(F("/login HTTP/1.1"));
  debugln("POST /child/" + String(childID) + "/login HTTP/1.1", WEBCLIENT);
  sendLoginRegisterThings();
}

void sendRegister() {
  client.println(F("POST /child/register HTTP/1.1"));
  debugln(F("POST /child/register HTTP/1.1"), WEBCLIENT);
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

  debugln(body, WEBCLIENT);
}

void clientReceiveAndClose() {
  unsigned int charBufferCount = 0;

  //one char extra as termination byte
  char *lineBuffer = (char *) calloc(READBUFFERSIZSE + 1, sizeof(char));

  bool inBody = false;

  while (client.available()) {
    char c = client.read();
    if (c != '\n') {
      lineBuffer[charBufferCount] = c;
      charBufferCount++;
    }
    if (c == '\n' || charBufferCount == READBUFFERSIZSE) {
      //check for two newlines, than we are on the seperation of headers and body
      if (charBufferCount == 1 && lineBuffer[0] == '\r') {
        inBody = true;
        continue;
      }

      if (inBody) {
        handleBodyPart(lineBuffer);
      } else {
        int code = getHttpStatusCode(lineBuffer);
        if (code != -1) {
          responseStatusCode = code;
          debug(F("Status code: "), WEBCLIENT);
          bebugln(code, WEBCLIENT);
        }
      }

      clearBuffer(lineBuffer, charBufferCount);
      charBufferCount = 0;
    }
  }
  if (charBufferCount > 0)
    handleBodyPart(lineBuffer);
  free(lineBuffer);

  if (clientstate == NET_RECEIVING && !client.connected()) {
    debugln("disconnecting.", WEBCLIENT);
    client.stop();
    clientstate = NET_WAITING;
  }
}

void handleBodyPart(char *lineBuffer) {
  switch (requesttypestate) {
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
    debugln(id, WEBCLIENT);
    if (id > 0) {
      EEPROM.write(0, id);
      childID = id;
      debug(F("ONTVANGEN ID:"), WEBCLIENT);
      bebugln(id, WEBCLIENT);
    } else {
      logAndRetry(lineBuffer);
      return;
    }

    unsigned long ti = readTimeFromJson(lineBuffer);

    requesttypestate = LOGIN;
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
  debugln(lineBuffer, WEBCLIENT);
  debugln(F("Login/Register failed, retrying"), WEBCLIENT);
  client.stop();
  clientstate = NET_CONNETING;
}

void handleTimePartOfLoginRegisterResponse(char* lineBuffer) {
  unsigned long ti = readTimeFromJson(lineBuffer);
  debug(F("ONTVANGEN TIJD:"), WEBCLIENT);
  bebugln(ti, WEBCLIENT);
  if (ti < 1) {
    logAndRetry(lineBuffer);
  } else {
    serverTime = ti;
    serverTimeSync = millis();
  }
}

void readWeatherResponseLine(char *lineBuffer) {
  if (responseStatusCode != 200)
    debugln(lineBuffer, WEBCLIENT);
}

unsigned long getTime() {
  return serverTime + ((millis() - serverTimeSync) / 1000);
}

void resetChildID() {
  EEPROM.write(0, 0);
  debugln(F("Child ID has been reset, requesting new."), WEBCLIENT);
  client.stop();
  clientstate = NET_CONNETING;
  requesttypestate = REGISTER;

  childID = -1;
  serverTime = 0;
  serverTimeSync = 0;
}

