#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include <EEPROM.h>;


#define READBUFFERSIZSE 40
Range NULLRANGER = {.start = 0, .end=0};

EthernetClient client;

NetServerStatus serverstate;
NetType requeststate;
int responseStatusCode = -1;

int childID;
bool hasID;
unsigned long serverTime;
unsigned long serverTimeSync;

void sendWeather();
void sendWeatherHeader(String headerLength);
void sendLogin();
void sendRegister();
void tryConnectAndSend();
void tryReceiveAndClose();
int getHttpStatusCode(char* line);
byte readIdFromJson(char* line);
unsigned long readTimeFromJson(char* line);

void readRegisterResponseLine(char* pointerToLineBuffer);
void readLoginResponseLine(char* pointerToLineBuffer);
void readWeatherResponseLine(char* pointerToLineBuffer);
unsigned long getTime();
int freeRam();

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

  //one char extra as termination byte
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
  
  if (serverstate == NET_RECEIVING && !client.connected()) {
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
    debugln(F("connection failed"), NETWORK); 
}

void sendWeather() { 
debugln(freeRam(),NETWORK); 
  String body = "{";
#ifdef SENDHUMID
  body = body + "\"humidity\": " + (String)getHumidity();
#endif
#ifdef SENDTEMP
  body = body + ",\"temperature\": " + (String)getTemperature();
#endif
#ifdef SENDBRIGHTNESS
  body = body + ",\"brightness\": " + (String)(getBrightness()/10);
debugln(freeRam(),NETWORK);
#endif
  body = body + ",\"timestamp\": ";
  body.concat(getTime());
debugln(freeRam(),NETWORK);
  body = body + "}";
debugln(freeRam(),NETWORK);
  sendWeatherHeader((String)body.length()); //todo: static body length?
  client.println(body);
  debugln(F("Sent body: "), NETWORK);
  debugln(body,NETWORK);
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
  
  client.println(F("Host: 192.168.178.55:8080"));
  client.println(F("Content-Type: application/json"));
  client.print(F("Content-Length: "));
  client.println((String)body.length());

  client.println(F("Connection: close \n"));
  client.println((String)body);

  debugln("Sent body: " + (String)body, NETWORK);
}

void sendLogin(){
    client.print(F("POST /child/")); client.print(String(childID)); client.println(F("/login HTTP/1.1"));
    debugln("POST /child/" + String(childID) + "/login HTTP/1.1", NETWORK); 
    sendLoginRegisterThings();
}

void sendRegister(){
    client.println(F("POST /child/register HTTP/1.1"));
    debugln(F("POST /child/register HTTP/1.1"), NETWORK);
    sendLoginRegisterThings();
}

void loginToGateway(){
  int eepromid = EEPROM.read(0);  
  serverstate = NET_CONNETING;
  if (eepromid > 0) {
    debugln("eepromid =" + (String)eepromid, NETWORK);
    childID = eepromid;
    requeststate = LOGIN;
  } else {
    requeststate = REGISTER;
  }
}

void readRegisterResponseLine(char* line){      
  if(responseStatusCode != 200){
      debugln(line, NETWORK);
      return;
  }
  
  if(line[0] == '{'){
    byte id = readIdFromJson(line);
    debugln(id, NETWORK);
    if(id > 0) {
      EEPROM.write(0, id);
      childID = id;
    } else {
      debugln(F("No ID received, retrying"), NETWORK);
      client.stop();
      requeststate = REGISTER;
    }
  
    unsigned long ti = readTimeFromJson(line);
    debugln(ti, NETWORK);
    //todo: check if ti == 0, want dan was er geen time beschikbaar. Anders tijd opslaan en bijhouden.
  }
}

unsigned long readTimeFromJson(char* line){
  Range timeRange = findJsonFieldRange(line, "\"time\"");
  if(&timeRange == &NULLRANGER){
     debugln(F("Could not find time in following line"), NETWORK);
     debugln(line, NETWORK);
     return 0;
  }else{
    unsigned long curTime = 0;
    for(int i = timeRange.start; i <= timeRange.end; i++){
      curTime = curTime * 10;
      curTime += line[i]-48;
    }
    return curTime;
  }
}

byte readIdFromJson(char* line){
  Range idRange = findJsonFieldRange(line, "\"id\"");

  if(&idRange == &NULLRANGER){
    debugln(F("Could not find id in following line"), NETWORK);
    debugln(line, NETWORK);
    return -1;
  }else{
    byte id = 0;

    for(int i = idRange.start; i <= idRange.end; i++){
      id = id * 10;
      id += line[i]-48;
    }
    return id;
  }
}


//find the range of the value of a field by its id in JSON or NULL if not found. example:
// field          "id"             //including the " chars!
// json           {"id": 417}
// response       {7,9}
// String values are returned excluding their surrounding "
Range findJsonFieldRange(char* json, char* field) {
    int jsoni = 0;
    int fieldi = 0;

    //search for the field name. End when the full name is read.
    while (1) {
        if (json[jsoni] == field[fieldi])
            fieldi++;
        else
            fieldi = 0;
        jsoni++;
        if (field[fieldi] == '\0')
            break; //jsoni is now at the char after the field in the json. This must be a " char.
        if (json[jsoni] == '\0'){
            return NULLRANGER; //field name not found
            Serial.println("Field name not found");
        }
    }

    jsoni++;

    //skip all whitespaces : to get to the value.
    while (json[jsoni] == ' ' || json[jsoni] == '\t' || json[jsoni] == ':')
        jsoni++;

    int valueBegin = jsoni;
    bool quoteTerminated = json[jsoni] == '"';
    if(quoteTerminated)
        valueBegin++;
    jsoni++;

    while (1) {
        if (quoteTerminated) {
            if (json[jsoni] == '"' && json[jsoni - 1] != '\\')
                break;
        } else {
            if (json[jsoni] == ',' || json[jsoni] == '}'){
                break;
            }
        }
        jsoni++;
        if (json[jsoni] == '\0'){
            Serial.println("Value terminator not found");
            return NULLRANGER; //value terminator not found
        }
    }
    jsoni--; //the last char should not be included
    Range out = { .start = valueBegin, .end = jsoni };
    return out;
}

int getHttpStatusCode(char* line){
  if(line[0] == 'H' && line[1] == 'T' && line[2] == 'T' && line[3] == 'P' && line[4] == '/'){ // detect line HTTP/1.1 200
    char statuscodestring[4] = {line[9], line[10], line[11], 0 };
    return atoi(statuscodestring);
  }
  return -1;
}
 
void readLoginResponseLine(char* line){
  if(responseStatusCode != 200){
      debugln(line, NETWORK);
      debugln(F("No ID time, retrying"), NETWORK);
      client.stop();
      serverstate = NET_CONNETING;
      delay(2000);
      return;
  }
  if(line[0] == '{'){  
    unsigned long ti = readTimeFromJson(line);
    debugln(F("ONTVANGEN TIJD:"), NETWORK);
    debugln(ti, NETWORK);
    if(ti < 1) {
      debugln(F("No ID time, retrying"), NETWORK);
      client.stop();
      serverstate = NET_CONNETING;
    } else {
      serverTime = ti;
      serverTimeSync = millis();
    }
  }
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

unsigned long getTime() {  
  return serverTime + ((millis() - serverTimeSync)/1000);
}
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

