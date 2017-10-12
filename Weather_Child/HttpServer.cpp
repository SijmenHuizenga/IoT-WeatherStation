#include "Debug.h"
#include "Network.h"
#include "HttpServer.h"
#include "Led.h"
#include "Json.h"
#include "Ram.h"

EthernetServer server(80);

ChildHttpServer* httpServer = new ChildHttpServer();

void ChildHttpServer::startHttpServer() {
  server.begin();
}

void ChildHttpServer::updateHttpServer() {
  EthernetClient client = server.available();
  if (client) {
    char *lineBuffer = (char *) calloc(READBUFFERSIZSE + 1,  sizeof(char));

    this->contentLength = 0;
    this->requestType = UNKOWN;

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
            this->processRequestPart(lineBuffer);
          }

          network->clearBuffer(lineBuffer, charBufferCount);
          charBufferCount = 0;
        }
      }
    }
    if (contentLength > READBUFFERSIZSE) {
      this->sendBadRequestResponse(client);
    } else if (this->contentLength == 0) {
      this->sendResponse(client, NULL);
    } else {
      network->clearBuffer(lineBuffer, charBufferCount);
      charBufferCount = 0;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          lineBuffer[charBufferCount] = c;
          charBufferCount++;
          if (charBufferCount == this->contentLength)
            break;
        }
      }
      this->sendResponse(client, lineBuffer);
    }

    free(lineBuffer);
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

void ChildHttpServer::processRequestPart(char* buffer) {
  if (network->startsWith(buffer, "GET /ping"))
    this->requestType = PING;
  else if (network->startsWith(buffer, "GET /settings"))
    this->requestType = GETSETTINGS;
  else if (network->startsWith(buffer, "PUT /settings"))
    this->requestType = PUTSETTINGS;
  else if (network->startsWith(buffer, "Content-Length:")) {
    this->contentLength = 0;
    for (int i = 15; buffer[i] != 0 && buffer[i] != '\r'; i++) {
      if (buffer[i] == ' ')
        continue;
      this->contentLength = this->contentLength * 10 + (buffer[i] - 48);
    }
  }
}

void ChildHttpServer::sendResponse(EthernetClient client, char* body) {
  if (this->requestType == PING && body == NULL) {
    this->sendOkResponse(client);
  } else if (requestType == GETSETTINGS && body == NULL)
    this->sendGetSettingsResponse(client);
  else if (requestType == PUTSETTINGS && body != NULL)
    this->sendPutSettingsResponse(client, body);
  else{
    this->sendBadRequestResponse(client);
  }
}

void ChildHttpServer::sendOkResponse(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Content-Length: 16");
  client.println("Connection: close");
  client.println();
  client.println("{\"status\": \"ok\"}");
}

//body format like: { "g": 0, "r": 0 }
void ChildHttpServer::sendPutSettingsResponse(EthernetClient client, char* body) {
  debugln(body, WEBSERVER);
  Range gRange = jsonController->findJsonFieldRange(body, "\"g\"");
  Range rRange = jsonController->findJsonFieldRange(body, "\"r\"");
  if (&gRange == &NULLRANGER || &rRange == &NULLRANGER) {
    this->sendBadRequestResponse(client);
  } else {
    float newGreen = jsonController->makeFloatFromRange(body, gRange);
    float newRed = jsonController->makeFloatFromRange(body, rRange);

    if (newGreen > 100 || newRed > 100 || newGreen >= newRed) {
      this->sendBadRequestResponse(client);
      return;
    }
    led->setTreshGreen(newGreen);
    led->setTreshRed(newRed);


    this->sendOkResponse(client);
  }
}

void ChildHttpServer::sendGetSettingsResponse(EthernetClient client) {
  String body = "{\"g\":";
  body.concat(led->getTreshGreen());
  body.concat(",\"r\":");
  body.concat(led->getTreshRed());
  body.concat("}");

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(body.length());
  client.println("Connection: close");
  client.println();
  client.print(body);
}

void ChildHttpServer::sendBadRequestResponse(EthernetClient client) {
  client.println("HTTP/1.1 400 BAD REQUEST");
  client.println("Connection: close");
  client.println();
}
