#pragma once
#define DEBUGNETWORK
#define DEBUGSENSOR

#include <Arduino.h> 

typedef enum DebugType {SENSOR, NETWORK, LED} DebugType;

void startDebugging();

void debug(String message, DebugType type);
void debugln(String message, DebugType type);