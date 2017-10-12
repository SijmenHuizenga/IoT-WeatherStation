#include "configure.h"
#include "Debug.h"
#include <EEPROM.h>
#include <Arduino.h>
Configure* conf = new Configure();

void Configure::configureChild() {
    if(!this->hasMac() || ! this->hasGatewayIP()) {
        if(!this->hasMac()){
            this->receiveMac();
        }
        if(!this->hasGatewayIP()){
            this->receiveIP();
        }
        Serial.println(F("Setup is super sex full"));
    } else {
        for(int i = 0; i < 6; i++) {
            this->mac[i] = EEPROM.read(MACBYTE1+i);
        }
        this->ip = EEPROM.read(GATEWAYIP);
        debug(F("The mac address is: "), CONF);
        for(int i = 0; i < 5; i++) {
            bebug(String(this->mac[i], HEX), CONF);
            bebug(F(":"), CONF);
        }
        bebugln(String(this->mac[5], HEX), CONF);
        debug("The gateway IP address is: 192.168.178.", CONF);
        bebugln(this->ip, CONF);

        return;
    }

}

void Configure::writeIdToEeprom(int id) {
    EEPROM.update(IDADDRESS, id);
}

void Configure::writeMacToEeprom(byte mac1, byte mac2, byte mac3, byte mac4, byte mac5, byte mac6) {
    EEPROM.update(MACBYTE1, mac1);
    EEPROM.update(MACBYTE2, mac2);
    EEPROM.update(MACBYTE3, mac3);
    EEPROM.update(MACBYTE4, mac4);
    EEPROM.update(MACBYTE5, mac5);
    EEPROM.update(MACBYTE6, mac6);

}

void Configure::writeIpToEeprom(byte ip){
    EEPROM.update(GATEWAYIP,ip);
}

bool Configure::hasID() {
    if(!(EEPROM.read(IDADDRESS) > 0) || isnan(EEPROM.read(IDADDRESS))){
        return false;
    } else {
        return true;
    }
}

bool Configure::hasMac() {
    if(!(EEPROM.read(MACBYTE1) > 0) || isnan(EEPROM.read(MACBYTE1))){
        return false;
    } else if (!(EEPROM.read(MACBYTE2) > 0) || isnan(EEPROM.read(MACBYTE2))){
        return false;
    } else if (!(EEPROM.read(MACBYTE3) > 0) || isnan(EEPROM.read(MACBYTE3))){
        return false;
    } else if (!(EEPROM.read(MACBYTE4) > 0) || isnan(EEPROM.read(MACBYTE4))){
        return false;
    } else {
        return true;
    }
}

bool Configure::hasGatewayIP() {
    if(!(EEPROM.read(GATEWAYIP) > 0) || isnan(EEPROM.read(GATEWAYIP))){
        return false;
    } else {
        return true;
    }

}

int Configure::readID() {
    int id;
    if (this->hasID()) {
        id = EEPROM.read(IDADDRESS);
    } else {
        id = 0;
    }
    return id;
}

byte Configure::readMac(int b){
    byte mac;
    mac = EEPROM.read(MACBYTE1+b);
    return mac;
}

// User input

void Configure::receiveMac() {
    Serial.println(F("Vul het mac addres in in het volgende formaat: xx-xx-xx-xx-xx-xx"));
    bool error = false;
    bool newData = false;
    bool hasMac = false;
    char receivedMac[12];
    byte counter = 0;
    while(!hasMac) {
        while (Serial.available() > 0) {
            char temp;
            temp = Serial.read();
            if ((temp >= '0' && temp <= '9') || (temp >= 'A' && temp <= 'F') ||
                (temp >= 'a' && temp <= 'f') && counter < 12) {
                receivedMac[counter] = temp;
                newData = true;
                counter++;
            }
            delay(5);
        }
        if (counter < 11 && newData == true) {
            error = true;
            newData = false;
        }
        if (newData == true) {
            for (int i = 0; i < 6; i++) {
                this->mac[i] = hexToByte(receivedMac[i * 2], receivedMac[(i * 2) + 1]);
            }
            Serial.println(F("Macaddress correct: "));
            for (int i = 0; i < 6; i++) {
                Serial.print(this->mac[i], HEX);Serial.print(F(":"));
            }
            hasMac = true;
            counter = 0;
            newData = false;
            this->writeMacToEeprom(this->mac[0],this->mac[1],this->mac[2],this->mac[3],this->mac[4],this->mac[5]);
            return;
        }
        if(error) {
            Serial.println(F("Invalid mac address please retry"));
            for(int i = 0; i < 12; i++) {
                receivedMac[i] = NULL;
            }
            counter=0;
            error = false;
        }
    }



}

void Configure::receiveIP() {
    Serial.println(F("\nVul de laatste byte van het IP address in, 192.168.178.x"));
    String receivedIP = "";
    bool error = false;
    byte counter = 0;
    bool hasIP = false;
    bool newData = false;
    while (!hasIP) {
        while (Serial.available() > 0) {
            char temp;
            temp = Serial.read();

            if (isDigit(temp) && temp > '0' && counter < 3) {
                receivedIP += (char) temp;
                counter++;
                newData = true;
            } else {
                error = true;
                newData = false;
            }
            delay(5);
        }
        if (newData == true) {
            this->ip = receivedIP.toInt();
            receivedIP = "";
            newData = false;
            Serial.println(F("Ip address correct"));
            Serial.println(this->ip);
            hasIP = true;
            this->writeIpToEeprom(this->ip);
            return;
        }
        if(error) {
            Serial.println(F("Ip address incorrect"));
            counter = 0;
            error = false;
        }
    }

}

byte Configure::hexToByte(char char1, char char2){
    byte result=0;
    byte result2=0;
    if(char1>='0' && char1<='9') result = char1-'0';
    else if(char1>='A'&& char1<='F') result = char1 - 'A' + 0xa;
    else if(char1>='a'&& char1<='f') result = char1 - 'a' + 0xa;
    else{
        Serial.println("invalid mac.");//error
    }
    if(char2>='0' && char2<='9') result2 = char2-'0';
    else if(char2>='A'&& char2<='F') result2 = char2 - 'A' + 0xa;
    else if(char2>='a'&& char2<='f') result2 = char2 - 'a' + 0xa;
    else{
        Serial.println("invalid mac.");//error
    }
    //idem c2 in result2
    result=result<<4;
    result+=result2;
    return result;
}


