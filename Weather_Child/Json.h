#pragma once

#include <Arduino.h>

struct Range {
  int start;
  int end;
};

Range findJsonFieldRange(char *json, char *field);

unsigned long readTimeFromJson(char* line);
byte readIdFromJson(char* line);

static const Range NULLRANGER = {.start = 0, .end = 0};
