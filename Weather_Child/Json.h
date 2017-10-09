#pragma once

#include <Arduino.h>

struct Range {
  int start;
  int end;
};

static const Range NULLRANGER = {.start = 0, .end = 0};

class Json {
  private:

  public:
    Range findJsonFieldRange(char *json, char *field);
    unsigned long readTimeFromJson(char* line);
    byte readIdFromJson(char* line);
    float makeFloatFromRange(char* line, Range range);
    
};

extern Json *jsonController;

