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
  this->sendWeatherHeader(80);

  client.print(F("{\"humidity\":")); //11
  printFloatTo5CharString(client, sensors->getHumidity()); //5
  client.print(F(",\"temperature\":")); //15
  printFloatTo5CharString(client, sensors->getTemperature());//5
  client.print(F(",\"brightness\":"));//14
  printFloatTo5CharString(client, sensors->getBrightness() / 10);//5
  client.print(F(",\"timestamp\":"));//13
  client.print(getTime()); //10
  client.println(F("}"));//1

  debugln(F("sendWeather"), WEBCLIENT);
}

void ChildHttpClient::sendWeatherHeader(unsigned int bodyLength) {
  client.print(F("PUT /child/"));
  client.print(this->childID);
  client.println(F("/measurements HTTP/1.1"));
  client.println(F("Host: 192.168.178.55:8080")); // to-do dynamisch maken
  client.println(F("Content-Type: application/json"));
  client.print(F("Content-Length: "));
  client.println(bodyLength);
  client.println(F("Connection: close"));
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

  client.print(F("Host: 192.168.178."));
  client.print(conf->ip);
  client.print(":");
  client.println(this->gatewayPort);
  client.println(F("Content-Type: application/json"));
  client.println(F("Content-Length: 25"));
  client.println(F("Connection: close \n"));

  client.print(F("{\"ip\":\"")); // 8
  client.print(network->getIpAddress(Ethernet.localIP())); //15
  client.print(F("\"}")); //2

  debugln("sent login/register things", WEBCLIENT);
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

void printFloatTo5CharString(EthernetClient client, float f){
  if(f >= 100){
    // 100-999/10 = 10-99
    client.print(String(f, 1));  //100.0
  }else if(f >= 10){
    client.print(String(f, 2));  //10.50
  }else if(f >= 0) {
    client.print(String(f, 3));  //1.000
  }else if(f > -10){
    client.print(String(f, 2));  //-5.40
  }else if(f > -100){
    client.print(String(f, 1));  //-99.9
  }
}