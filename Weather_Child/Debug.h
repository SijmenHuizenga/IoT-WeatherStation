#pragma once
#define DEBUGNETWORK
#define DEBUGSENSOR

#include <Arduino.h> 

typedef enum DebugType {SENSOR, NETWORK, LED} DebugType;

void startDebugging();

//this needs to be generic otherwhise a lot of casting/converting hast to be done to get everything into Strings before printing it.
//Using generics there doesnt need to happen any casting :)
//see the following link for explination why this implementation is here instead of in a cpp file.
//https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
//yes, there are other solutions, but i dont want to complicate things by introducing classes.
template<typename T>
void debugln(T message, DebugType type) {
  if(type == SENSOR) {
    #ifdef DEBUGSENSOR
      Serial.print("[SENS] ");
      Serial.println(message);
    #endif
  } else if(type == NETWORK) {
    #ifdef DEBUGNETWORK
      Serial.print("[NET] ");
      Serial.println(message);
    #endif
  } else if(type == LED) {
    #ifdef LED
      Serial.print("[LED] ");
      Serial.println(message);
    #endif
  }
}

template<typename T>
void debug(T message, DebugType type) {
  if(type == SENSOR) {
    #ifdef DEBUGSENSOR
      Serial.print(message);
    #endif
  } else if(type == NETWORK) {
    #ifdef DEBUGNETWORK
      Serial.print(message);
    #endif
  } else if(type == LED) {
    #ifdef LED
      Serial.print(message);
    #endif
  }
}
