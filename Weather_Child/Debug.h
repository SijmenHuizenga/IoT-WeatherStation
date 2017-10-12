#pragma once
#define DEBUGWEBCLIENT
//#define DEBUGSENSOR
//#define DEBUGLED
#define DEBUGWEBSERVER
#define DEBUGCONFIG

#include <Arduino.h>

typedef enum DebugType {
  SENSOR, WEBCLIENT, LED, WEBSERVER, CONF
} DebugType;

void startDebugging();

//this needs to be generic otherwhise a lot of casting/converting hast to be done to get everything into Strings before printing it.
//Using generics there doesnt need to happen any casting :)
//see the following link for explination why this implementation is here instead of in a cpp file.
//https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
//yes, there are other solutions, but i dont want to complicate things by introducing classes.
template<typename T>
void debug(T message, DebugType type, bool printnl, bool printprefix) {
  switch (type) {
#ifdef DEBUGSENSOR
    case SENSOR:
      if (printprefix)
        Serial.print(F("[SENS] "));
      Serial.print(message);
      if (printnl)
        Serial.println();
      break;
#endif
#ifdef DEBUGCONFIG
    case CONF:
      if (printprefix)
        Serial.print(F("[CONF] "));
          Serial.print(message);
          if (printnl)
            Serial.println();
          break;
#endif
#ifdef DEBUGWEBCLIENT
    case WEBCLIENT:
      if (printprefix)
        Serial.print(F("[CLI] "));
      Serial.print(message);
      if (printnl)
        Serial.println();
      break;
#endif
#ifdef DEBUGLED
    case LED:
      if (printprefix)
        Serial.print(F("[LED] "));
      Serial.print(message);
      if (printnl)
        Serial.println();
      break;
#endif
#ifdef DEBUGWEBSERVER
    case WEBSERVER:
      if (printprefix)
        Serial.print(F("[SER] "));
      Serial.print(message);
      if (printnl)
        Serial.println();
      break;
#endif
  }
}

template<typename T>
void debug(T message, DebugType type) {
  debug(message, type, false, true);
}

template<typename T>
void debugln(T message, DebugType type) {
  debug(message, type, true, true);
}

template<typename T>
void bebugln(T message, DebugType type) {
  debug(message, type, true, false);
}

template<typename T>
void bebug(T message, DebugType type) {
  debug(message, type, false, false);
}
