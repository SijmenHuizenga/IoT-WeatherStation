#pragma once

#include "Debug.h"
#include "Network.h"
#include "Sensors.h"
#include "HttpClient.h"
#include "Json.h"
#include <EEPROM.h>
#include <Arduino.h>
#define SENDTEMP
#define SENDHUMID
#define SENDBRIGHTNESS

#define SERVERPORT 80

enum NetClientStatus { NET_CONNETING, NET_RECEIVING, NET_WAITING };
enum NetRequestType {REGISTER, LOGIN, SENDDATA};


class ChildHttpClient {
  private:
    EthernetClient client;    
    int childID = -1;
    unsigned long serverTime = 0;
    unsigned long serverTimeSync = 0;
    NetRequestType requesttypestate;
    int responseStatusCode = -1;
  public:
    NetClientStatus clientstate = NET_WAITING;
    void updateHttpClient();
    void sendWeatherToGateway();
    void loginToGateway();
    void resetChildID();
    void clientConnectAndSend();
    void clientReceiveAndClose();
    void handleBodyPart(char *lineBuffer);
    void sendWeather();
    void sendWeatherHeader(unsigned int headerLength);
    void sendLogin();
    void sendRegister();
    void sendLoginRegisterThings();
    void readLoginResponseLine(char* lineBuffer);
    void readRegisterResponseLine(char* lineBuffer);
    void logAndRetry(char* lineBuffer);
    void handleTimePartOfLoginRegisterResponse(char* lineBuffer);
    void readWeatherResponseLine(char* lineBuffer);
    unsigned long getTime();
};

void printFloatTo5CharString(EthernetClient client, float f);

extern ChildHttpClient *httpClient;

extern NetClientStatus clientstate;
extern NetRequestType requesttypestate;






