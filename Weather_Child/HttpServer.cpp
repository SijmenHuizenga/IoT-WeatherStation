#pragma once
#include "Debug.h"
#include "Network.h"
#include "HttpServer.h"
#include "Led.h"
#include "Json.h"

EthernetServer server(80);

HttpServerRequestType requestType;
int contentLength = 0;

void startHttpServer() {
  server.begin();
}

void updateHttpServer() {
  EthernetClient client = server.available();
  if (client) {
    debugln("client connected", WEBSERVER);
    char *lineBuffer = (char *) calloc(READBUFFERSIZSE + 1,  sizeof(char));

    contentLength = 0;
    requestType = UNKOWN;

    unsigned int charBufferCount = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n') {
          lineBuffer[charBufferCount] = c;
          charBufferCount++;
        }

        if (c == '\n' || charBufferCount == READBUFFERSIZSE) {
          if (charBufferCount == 1 && lineBuffer[0] == '\r') {
            //now we arrived in the body (or the end)
            break;
          } else {
            processRequestPart(lineBuffer);
          }

          clearBuffer(lineBuffer, charBufferCount);
          charBufferCount = 0;
        }
      }
    }
    if (contentLength > READBUFFERSIZSE) {
      debugln(F("Content length bigger than READBUFFERSIZE"), WEBSERVER);
      sendBadRequestResponse(client);
    } else if (contentLength == 0) {
      sendResponse(client, NULL);
    } else {
      clearBuffer(lineBuffer, charBufferCount);
      charBufferCount = 0;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          lineBuffer[charBufferCount] = c;
          charBufferCount++;
          if (charBufferCount == contentLength)
            break;
        }
      }
      sendResponse(client, lineBuffer);
    }

    free(lineBuffer);
    // give the web browser time to receive the data
    delay(1);
    client.stop();
    debugln("client disconnected", WEBSERVER);
  }
}

void processRequestPart(char* buffer) {
  if (startsWith(buffer, "GET /ping"))
    requestType = PING;
  else if (startsWith(buffer, "GET /settings"))
    requestType = GETSETTINGS;
  else if (startsWith(buffer, "PUT /settings"))
    requestType = PUTSETTINGS;
  else if (startsWith(buffer, "Content-Length:")) {
    contentLength = 0;
    for (int i = 15; buffer[i] != 0 && buffer[i] != '\r'; i++) {
      if (buffer[i] == ' ')
        continue;
      contentLength = contentLength * 10 + (buffer[i] - 48);
    }
  }
}

void sendResponse(EthernetClient client, char* body) {
  if (requestType == PING && body == NULL) {
    debugln(F("Ping / Pong"), WEBSERVER);
    sendOkResponse(client);
  } else if (requestType == GETSETTINGS && body == NULL)
    sendGetSettingsResponse(client);
  else if (requestType == PUTSETTINGS && body != NULL)
    sendPutSettingsResponse(client, body);
  else{
    debugln(F("Unknown request"), WEBSERVER);
    sendBadRequestResponse(client);
  }
}

void sendOkResponse(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Content-Length: 16");
  client.println("Connection: close");
  client.println();
  client.println("{\"status\": \"ok\"}");
}

//body format like: { "g": 0, "r": 0 }
void sendPutSettingsResponse(EthernetClient client, char* body) {
  debugln(body, WEBSERVER);
  Range gRange = findJsonFieldRange(body, "\"g\"");
  Range rRange = findJsonFieldRange(body, "\"r\"");
  if (&gRange == &NULLRANGER || &rRange == &NULLRANGER) {
    debugln(F("g or r not found in json body"), WEBSERVER);
    sendBadRequestResponse(client);
  } else {
    float newGreen = makeFloatFromRange(body, gRange);
    float newRed = makeFloatFromRange(body, rRange);

    if (newGreen > 100 || newRed > 100 || newGreen >= newRed) {
      debugln(F("g or r not in range or are equal"), WEBSERVER);
      sendBadRequestResponse(client);
      return;
    }
    led->setTreshGreen(newGreen);
    led->setTreshRed(newRed);

    debug(F("PutSettingsRequest with data Green "), WEBSERVER);
    bebug(newGreen, WEBSERVER);
    bebug(F(" Red "), WEBSERVER);
    bebugln(newRed, WEBSERVER);

    sendOkResponse(client);
  }
}

void sendGetSettingsResponse(EthernetClient client) {
  String body = "{\"g\":";
  body.concat(led->getTreshGreen());
  body.concat(",\"r\":");
  body.concat(led->getTreshRed());
  body.concat("}");

  debug(F("Get Settings Request. Response: "), WEBSERVER);
  debugln(body, WEBSERVER);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(body.length());
  client.println("Connection: close");
  client.println();
  client.print(body);
}

void sendBadRequestResponse(EthernetClient client) {
  debugln(F("Bad Request"), WEBSERVER);
  client.println("HTTP/1.1 400 BAD REQUEST");
  client.println("Connection: close");
  client.println();
}
