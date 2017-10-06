#include "Json.h"
#include "Debug.h"

unsigned long readTimeFromJson(char* line) {
  Range timeRange = findJsonFieldRange(line, "\"time\"");
  if (&timeRange == &NULLRANGER) {
    debugln(F("Could not find time in following line"), WEBCLIENT);
    debugln(line, WEBCLIENT);
    return 0;
  } else {
    unsigned long curTime = 0;
    for (int i = timeRange.start; i <= timeRange.end; i++) {
      curTime = curTime * 10;
      curTime += line[i] - 48;
    }
    return curTime;
  }
}

byte readIdFromJson(char* line) {
  Range idRange = findJsonFieldRange(line, "\"id\"");

  if (&idRange == &NULLRANGER) {
    debugln(F("Could not find id in following line"), WEBCLIENT);
    debugln(line, WEBCLIENT);
    return -1;
  } else {
    byte id = 0;

    for (int i = idRange.start; i <= idRange.end; i++) {
      id = id * 10;
      id += line[i] - 48;
    }
    return id;
  }
}

float makeFloatFromRange(char* line, Range range){
  float out = 0;

  int i = range.start;
  int dot = -1;
  while (i <= range.end) {
    if(line[i] == '.'){
      dot = i;
      i++;
    }
    if(dot == -1)
      out = out * 10 + (line[i] - 48);
    else
      out += ((float)(line[i] - 48))/((float)pow(10, (i-dot)));
    i++;
  }
  return out;
}


//find the range of the value of a field by its id in JSON or NULL if not found. example:
// field          "id"             //including the " chars!
// json           {"id": 417}
// response       {7,9}
// String values are returned excluding their surrounding "
Range findJsonFieldRange(char* json, char* field) {
  int jsoni = 0;
  int fieldi = 0;

  //search for the field name. End when the full name is read.
  while (1) {
    if (json[jsoni] == field[fieldi])
      fieldi++;
    else
      fieldi = 0;
    jsoni++;
    if (field[fieldi] == '\0')
      break; //jsoni is now at the char after the field in the json. This must be a " char.
    if (json[jsoni] == '\0') {
      return NULLRANGER; //field name not found
    }
  }

  jsoni++;

  //skip all whitespaces : to get to the value.
  while (json[jsoni] == ' ' || json[jsoni] == '\t' || json[jsoni] == ':')
    jsoni++;

  int valueBegin = jsoni;
  bool quoteTerminated = json[jsoni] == '"';
  if (quoteTerminated)
    valueBegin++;
  jsoni++;

  while (1) {
    if (quoteTerminated) {
      if (json[jsoni] == '"' && json[jsoni - 1] != '\\')
        break;
    } else {
      if (json[jsoni] == ',' || json[jsoni] == '}') {
        break;
      }
    }
    jsoni++;
    if (json[jsoni] == '\0') {
      Serial.println("Value terminator not found");
      return NULLRANGER; //value terminator not found
    }
  }
  jsoni--; //the last char should not be included
  Range out = { .start = valueBegin, .end = jsoni };
  return out;
}
