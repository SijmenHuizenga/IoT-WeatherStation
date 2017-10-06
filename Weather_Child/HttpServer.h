#pragma once
#include "Debug.h"
#include "Network.h"

enum HttpServerRequestType {PING, GETSETTINGS, PUTSETTINGS, UNKOWN};

void startHttpServer();
void updateHttpServer();

void processRequestPart(char* buffer);
void sendResponse(EthernetClient client, char* body);

void sendOkResponse(EthernetClient client);
void sendPutSettingsResponse(EthernetClient client, char* body);
void sendGetSettingsResponse(EthernetClient client);
void sendBadRequestResponse(EthernetClient client);
