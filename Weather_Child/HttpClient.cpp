#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include "configure.h"
#include "Json.h"
#include "Ram.h"
#include <EEPROM.h>
#include <Arduino.h>

ChildHttpClient* httpClient = new ChildHttpClient();

void ChildHttpClient::updateHttpClient() {
  if (this->clientstate == NET_RECEIVING) {
    this->clientReceiveAndClose();
  } else if (this->clientstate == NET_CONNETING) {
    this->clientConnectAndSend();
  }
}

void ChildHttpClient::sendWeatherToGateway(void) {
  if (this->childID != -1 && this->serverTime != 0 && this->clientstate == NET_WAITING) {
    this->clientstate = NET_CONNETING;
    this->requesttypestate = SENDDATA;
  }
}

void ChildHttpClient::clientConnectAndSend() {
  if (this->client.connect(network->gatewayIp, this->gatewayPort)) {
    switch (this->requesttypestate) {
      case REGISTER:
        this->sendRegister();
        break;
      case LOGIN:
        this->sendLogin();
        break;
      case SENDDATA:
        this->sendWeather();
        break;
    }
    this->clientstate = NET_RECEIVING;
    this->responseStatusCode = -1;
  } else
    debugln(F("connection failed"), WEBCLIENT);
}

void ChildHttpClient::sendWeather() {
  String body = "{";
#ifdef SENDHUMID
  body.concat("\"humidity\": ");
  body.concat(sensors->getHumidity());
#endif
#ifdef SENDTEMP
  body.concat(",\"temperature\": ");
  body.concat(sensors->getTemperature());
#endif
#ifdef SENDBRIGHTNESS
  body.concat(",\"brightness\": ");
  body.concat(sensors->getBrightness() / 10);
#endif
  body.concat(",\"timestamp\": ");
  body.concat(getTime());
  body.concat("}");

  this->sendWeatherHeader(body.length()); //todo: static body length?
  debugln(F("sendWeather"), WEBCLIENT);
  client.println(body);

  
}

void ChildHttpClient::sendWeatherHeader(unsigned int bodyLength) {
  client.println("PUT /child/" + String(this->childID) + "/measurements HTTP/1.1");
  client.println("Host: 192.168.178.55:8080"); // to-do dynamisch maken
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(bodyLength);
  client.println("Connection: close");
  client.println();
}

void ChildHttpClient::loginToGateway() {
  debugln(network->gatewayIp, WEBCLIENT);
  int eepromid = EEPROM.read(0);
  if (eepromid > 0) {
    debugln("eepromid =" + (String) eepromid, WEBCLIENT);
    this->childID = eepromid;
    this->requesttypestate = LOGIN;
  } else {
    this->requesttypestate = REGISTER;
  }
  this->clientstate = NET_CONNETING;
}

void ChildHttpClient::sendLogin() {
  client.print(F("POST /child/"));
  client.print(String(this->childID));
  client.println(F("/login HTTP/1.1"));
  debugln("POST /child/" + String(this->childID) + "/login HTTP/1.1", WEBCLIENT);
  this->sendLoginRegisterThings();
}

void ChildHttpClient::sendRegister() {
  client.println(F("POST /child/register HTTP/1.1"));
  debugln(F("POST /child/register HTTP/1.1"), WEBCLIENT);
  this->sendLoginRegisterThings();
}

void ChildHttpClient::sendLoginRegisterThings() {
  debug(freeRam(), WEBCLIENT);
  String body = "{";
  body.concat("\"ip\": \"");
  body.concat(network->getIpAddress(Ethernet.localIP()));
  body.concat("\"}");

  client.println(F("Host: 192.168.178.55:8080")); // to-do dynamisch maken
  client.println(F("Content-Type: application/json"));
  client.print(F("Content-Length: "));
  client.println(body.length());

  client.println(F("Connection: close \n"));
  client.println(body);

  debugln(body, WEBCLIENT);
}

void ChildHttpClient::clientReceiveAndClose() {
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
        this->handleBodyPart(lineBuffer);
      } else {
        int code = network->getHttpStatusCode(lineBuffer);
        if (code != -1) {
          this->responseStatusCode = code;
          debug(F("Status code: "), WEBCLIENT);
          bebugln(code, WEBCLIENT);
        }
      }

      network->clearBuffer(lineBuffer, charBufferCount);
      charBufferCount = 0;
    }
  }
  if (charBufferCount > 0)
    this->handleBodyPart(lineBuffer);
  free(lineBuffer);

  if (clientstate == NET_RECEIVING && !client.connected()) {
    debugln("disconnecting.", WEBCLIENT);
    this->client.stop();
    this->clientstate = NET_WAITING;
  }
}

void ChildHttpClient::handleBodyPart(char *lineBuffer) {
  switch (this->requesttypestate) {
    case REGISTER:
      this->readRegisterResponseLine(lineBuffer);
      break;
    case LOGIN:
      this->readLoginResponseLine(lineBuffer);
      break;
    case SENDDATA:
      this->readWeatherResponseLine(lineBuffer);
      break;
  }
}

void ChildHttpClient::readRegisterResponseLine(char *lineBuffer) {
  if (this->responseStatusCode != 200) {
    this->logAndRetry(lineBuffer);
    return;
  }

  if (lineBuffer[0] == '{') {
    byte id = jsonController->readIdFromJson(lineBuffer);
    debugln(id, WEBCLIENT);
    if (id > 0) {
      EEPROM.write(0, id);
      this->childID = id;
      debug(F("ONTVANGEN ID:"), WEBCLIENT);
      bebugln(id, WEBCLIENT);
    } else {
      this->logAndRetry(lineBuffer);
      return;
    }

    unsigned long ti = jsonController->readTimeFromJson(lineBuffer);

    this->requesttypestate = LOGIN;
    this->handleTimePartOfLoginRegisterResponse(lineBuffer);
  }
}

void ChildHttpClient::readLoginResponseLine(char *lineBuffer) {
  if (this->responseStatusCode != 200) {
    this->logAndRetry(lineBuffer);
    return;
  }
  if (lineBuffer[0] == '{') {
    this->handleTimePartOfLoginRegisterResponse(lineBuffer);
  }
}

void ChildHttpClient::logAndRetry(char* lineBuffer) {
  debugln(lineBuffer, WEBCLIENT);
  debugln(F("Login/Register failed, retrying"), WEBCLIENT);
  this->client.stop();
  this->clientstate = NET_CONNETING;
}

void ChildHttpClient::handleTimePartOfLoginRegisterResponse(char* lineBuffer) {
  unsigned long ti = jsonController->readTimeFromJson(lineBuffer);
  debug(F("ONTVANGEN TIJD:"), WEBCLIENT);
  bebugln(ti, WEBCLIENT);
  if (ti < 1) {
    this->logAndRetry(lineBuffer);
  } else {
    this->serverTime = ti;
    this->serverTimeSync = millis();
  }
}

void ChildHttpClient::readWeatherResponseLine(char *lineBuffer) {
  if (this->responseStatusCode != 200)
    debugln(lineBuffer, WEBCLIENT);
}

unsigned long ChildHttpClient::getTime() {
  return this->serverTime + ((millis() - this->serverTimeSync) / 1000);
}

void ChildHttpClient::resetChildID() {
  EEPROM.write(0, 0);
  debugln(F("Child ID has been reset, requesting new."), WEBCLIENT);
  client.stop();
  this->clientstate = NET_CONNETING;
  this->requesttypestate = REGISTER;

  this->childID = -1;
  this->serverTime = 0;
  this->serverTimeSync = 0;
}

