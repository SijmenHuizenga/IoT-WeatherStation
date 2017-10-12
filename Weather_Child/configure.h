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
    void receiveMac();
    void receiveIP();
    byte hexToByte(char char1, char char2);
    void writeMacToEeprom(byte mac1, byte mac2, byte mac3, byte mac4, byte mac5, byte mac6);
    void writeIpToEeprom(byte ip);
    bool hasMac();
    bool hasGatewayIP();
public:
    byte mac[6];
    byte ip;
    void configureChild();
    void writeIdToEeprom(int id);
    byte readMac(int byte);
    bool hasID();
    int readID();
    void abortChild();
};

extern Configure *conf;








// User input

