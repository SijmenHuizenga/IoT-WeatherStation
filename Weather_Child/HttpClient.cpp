#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include <EEPROM.h>;

#define READBUFFERSIZSE 50

EthernetClient client;

NetServerStatus serverstate;
NetType requeststate;

int childID;
bool hasID;

int responseStatusCode = -1;

void sendWeather();
void sendWeatherHeader(String headerLength);
void sendLogin();
void sendRegister();
void tryConnectAndSend();
void tryReceiveAndClose();
int getHttpStatusCode(char* line);

void readRegisterResponseLine(char* pointerToLineBuffer);
void readLoginResponseLine(char* pointerToLineBuffer);
void readWeatherResponseLine(char* pointerToLineBuffer);

String getIpAddress(IPAddress address);

void sendWeatherToGateway(void) {
  if (serverstate == NET_WAITING){
    serverstate = NET_CONNETING;
    requeststate = SENDDATA;
  }
}

void updateHttpClient() {
  if (serverstate == NET_RECEIVING) {
      tryReceiveAndClose();
  } else if (serverstate == NET_CONNETING) {
      tryConnectAndSend();
  }
}

void tryReceiveAndClose(){
  unsigned int charBufferCount = 0;

  //on char extra as termination byte
  char* lineBuffer = calloc('\0', (READBUFFERSIZSE+1)*sizeof(char));

  bool inBody = false;
  
  while (client.available()) {
    char c = client.read();
    if(c != '\n'){
      lineBuffer[charBufferCount] = c;
      charBufferCount++;
    }
    if(c == '\n' || charBufferCount == READBUFFERSIZSE-1){
      if(charBufferCount == 1 && lineBuffer[0] == '\r'){
        inBody = true;
        continue;
      }

      if(inBody){
        switch(requeststate){
          case REGISTER: readRegisterResponseLine(lineBuffer); break;
          case LOGIN: readLoginResponseLine(lineBuffer); break;
          case SENDDATA: readWeatherResponseLine(lineBuffer); break;
        }
      } else {
        int code = getHttpStatusCode(lineBuffer);
        if(code != -1)
          responseStatusCode = code;
      }
      
      charBufferCount = 0;
      for(int i = 0; i < READBUFFERSIZSE+1; i++)
        lineBuffer[i] = '\0';
    }
  }
  free(lineBuffer);
  
  if (!client.connected()) {
    debugln("disconnecting.", NETWORK);
    client.stop();
    serverstate = NET_WAITING;
  }
}

void tryConnectAndSend(){
  if (client.connect(gatewayIp, 8080)) {
    switch(requeststate){
      case REGISTER: sendRegister(); break;
      case LOGIN: sendLogin(); break;
      case SENDDATA: sendWeather(); break;
    }
    serverstate = NET_RECEIVING;
    responseStatusCode = -1;
  } else
    debugln("connection failed", NETWORK); 
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

  sendWeatherHeader((String)body.length()); //todo: static body length?

  client.println((String)body);
  debugln("Sent body: " + (String)body, NETWORK);
}

void sendWeatherHeader(String bodyLength) {
  client.println("PUT /child/" + String(childID) + "/measurements HTTP/1.1");
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " +  bodyLength);
  client.println("Connection: close");
  client.println();
}

void sendLoginRegisterThings(){
  String body = "{"; body = body + "\"ip\": \"" + getIpAddress(Ethernet.localIP()); body = body + "\"}";
  
  client.println("Host: 192.168.178.55:8080");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + (String)body.length());
  client.println("Connection: close");
  client.println();
  client.println((String)body);

  debugln("Sent body: " + (String)body, NETWORK);
}

void sendLogin(){
    client.println("PUT /child/" + String(childID) + "/login HTTP/1.1");
    debugln("PUT /child/" + String(childID) + "/login HTTP/1.1", NETWORK); 
    sendLoginRegisterThings();
}

void sendRegister(){
    client.println("POST /child/register HTTP/1.1");
    debugln("PUT /child/register HTTP/1.1", NETWORK);
    sendLoginRegisterThings();
}

void loginToGateway(){
  int eepromid = EEPROM.read(0);
  serverstate = NET_CONNETING;
  if (eepromid > 0) {
    requeststate = LOGIN;
  } else {
    requeststate = REGISTER;
  }
}

void readRegisterResponseLine(char* line){
  //         0123456
  //example: {"id":41,"time":1507028043}
  if(line[0] == '{'){
    int i = 1;
    while(line[i] != ':')
      i++;
    i++;
      
    int id = 0;
    while(line[i] != ','){
      id = id * 10;
      id += line[i]-48;
      i++;
    }
    debugln("The ID IS " + String(id), NETWORK);
  }
  if(responseStatusCode != 200)
    debugln(line, NETWORK);
}

int getHttpStatusCode(char* line){
  if(line[0] == 'H' && line[1] == 'T' && line[2] == 'T' && line[3] == 'P' && line[4] == '/'){ // detect line HTTP/1.1 200
    char statuscodestring[4] = {line[9], line[10], line[11], 0 };
    return atoi(statuscodestring);
  }
  return -1;
}

void readLoginResponseLine(char* line){
  Serial.println(line);
}

void readWeatherResponseLine(char* line){
  if(responseStatusCode != 200)
    debugln(line, NETWORK);
}

String getIpAddress(IPAddress address){
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

