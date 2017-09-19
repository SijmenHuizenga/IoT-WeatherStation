#pragma once
#include "Debug.h"
#include <Arduino.h> 


void startDebugging() {
//  #ifdef DEBUGSENSOR || DEBUGNETWORK
    Serial.begin(9600);
//  #endif
}


void debug(String message, DebugType type) {
  if(type == SENSOR) {
    #ifdef DEBUGSENSOR
    
      Serial.print(message);
    #endif
  } else if(type == NETWORK) {
    #ifdef DEBUGNETWORK
      Serial.print(message);
    #endif
  }
}
void debugln(String message, DebugType type) {
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
  }
}

