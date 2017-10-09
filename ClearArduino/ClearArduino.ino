#include <Arduino.h>
#include <EEPROM.h>
#define LEDEEPROMADRESSR 150
#define LEDEEPROMADRESSG 180

void setup() {
    Serial.begin(9600);
    float zero = 0;
    EEPROM.update(0, 0);
    Serial.println("ID set to 0.");
    EEPROM.put(LEDEEPROMADRESSG, zero);
    Serial.println("Green treshold set to 0.");
    EEPROM.put(LEDEEPROMADRESSR, zero);
    Serial.println("Red treshold set to 0.");

}

void loop() {

}