#include <Arduino.h>
#include <EEPROM.h>
#define IDADDRESS 0
#define REDADDRESS 1
#define GREENADDRESS 5
#define MACBYTE1 9
#define MACBYTE2 10
#define MACBYTE3 11
#define MACBYTE4 12
#define MACBYTE5 13
#define MACBYTE6 14
#define GATEWAYIP 15
void setup() {
    Serial.begin(9600);
    float zero = 0;
    EEPROM.update(IDADDRESS, 0);
    Serial.println("ID set to 0.");
    EEPROM.put(GREENADDRESS, zero);
    Serial.println("Green treshold set to 0.");
    EEPROM.put(REDADDRESS, zero);
    Serial.println("Red treshold set to 0.");
    EEPROM.update(MACBYTE1, 0);
    EEPROM.update(MACBYTE2, 0);
    EEPROM.update(MACBYTE3, 0);
    EEPROM.update(MACBYTE4, 0);
    EEPROM.update(MACBYTE5, 0);
    EEPROM.update(MACBYTE6, 0);
    Serial.println("Mac address set to 00:00:00:00:00:00.");
    EEPROM.update(GATEWAYIP, 0);
    Serial.println("Gateway IP address set to 192.168.178.0.");
}

void loop() {

}