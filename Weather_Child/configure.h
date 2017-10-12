#pragma once
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
#include <Arduino.h>

class Configure {
private:
    void Configure::receiveMac();
    void Configure::receiveIP();
    byte Configure::hexToByte(char char1, char char2);
    void Configure::writeMacToEeprom(byte mac1, byte mac2, byte mac3, byte mac4, byte mac5, byte mac6);
    void Configure::writeIpToEeprom(byte ip);
    bool Configure::hasMac();
    bool Configure::hasGatewayIP();
public:
    byte mac[6];
    byte ip;
    void Configure::configureChild();
    void Configure::writeIdToEeprom(int id);
    byte Configure::readMac(int byte);
    bool Configure::hasID();
    int Configure::readID();
};

extern Configure *conf;








// User input

