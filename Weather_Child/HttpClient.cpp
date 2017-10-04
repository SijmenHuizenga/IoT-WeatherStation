#pragma once
#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include <EEPROM.h>;

#define READBUFFERSIZSE 40

EthernetClient client;

NetServerStatus serverstate;
NetType requeststate;

Range NULLRANGER = {.start = 0, .end=0};

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
int readIdFromJson(char* line);
unsigned long readTimeFromJson(char* line);

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
  if(responseStatusCode != 200){
      debugln(line, NETWORK);
      return;
  }
  
  if(line[0] == '{'){
    int id = readIdFromJson(line);
    debugln(id, NETWORK);
    //todo: check if -1 wan dan is er geen id gekomen. Als wel id beschikbaar dan opslaan in eeprom ergens en meesturen met alle andere requests
  
    unsigned long ti = readTimeFromJson(line);
    debugln(ti, NETWORK);
    //todo: check if ti == 0, want dan was er geen time beschikbaar. Anders tijd opslaan en bijhouden.
  }
}

unsigned long readTimeFromJson(char* line){
  Range timeRange = findJsonFieldRange(line, "\"time\"");
  if(&timeRange == &NULLRANGER){
     debugln("Could not find time in following line", NETWORK);
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

int readIdFromJson(char* line){
  Range idRange = findJsonFieldRange(line, "\"id\"");

  if(&idRange == &NULLRANGER){
    debugln("Could not find id in following line", NETWORK);
    debugln(line, NETWORK);
    return -1;
  }else{
    int id = 0;

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
      return;
  }
  
  if(line[0] == '{'){
    int id = readIdFromJson(line);
    debugln(id, NETWORK);
    //todo: check if -1 wan dan is er geen id gekomen. Als wel id beschikbaar dan controleren met bestaande id en als dat goed is dan verder.
    //als het id niet overeenkomt ben bekende id dan is er iets heel raars en dan het nieuwe id overnemen misschien?? todo: even overleggen
  
    unsigned long ti = readTimeFromJson(line);
    debugln(ti, NETWORK);
    //todo: check if ti == 0, want dan was er geen time beschikbaar. Anders tijd opslaan en bijhouden.
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

