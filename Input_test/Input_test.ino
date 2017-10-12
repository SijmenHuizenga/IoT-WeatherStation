#include <Arduino.h>


char receivedMac[12];
String receivedIP = "";
byte ip;
byte mac[6];
boolean newData = false;
int counter = 0;
bool hasMac = false;
bool hasIP = false;
void setup() {
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
    receiveIP();


    receiveMac();
    Serial.println("Setup was super sex full");
}

void loop() {


}

void receiveMac() {
    Serial.println("Vul het mac addres in in het volgende formaat: xx-xx-xx-xx-xx-xx");
    bool error = false;
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
                mac[i] = hexToByte(receivedMac[i * 2], receivedMac[(i * 2) + 1]);
            }
            Serial.println("Macaddress correct: ");
            for (int i = 0; i < 6; i++) {
                Serial.print(mac[i], HEX);Serial.print(":");
            }
            hasMac = true;
            counter = 0;
            newData = false;
        }
        if(error) {
            Serial.println("Invalid mac address please retry");
            for(int i = 0; i < 12; i++) {
                receivedMac[i] = NULL;
            }
            counter=0;
            error = false;
        }
    }


}

void receiveIP() {
    Serial.println("Vul de laatste byte van het IP address in, 192.168.178.x");
    bool error = false;
    while (!hasIP) {
        while (Serial.available() > 0) {
            char temp;
            temp = Serial.read();
            if (isDigit(temp) && counter < 3) {
                receivedIP += (char) temp;
                newData = true;
            } else {
                error = true;
            }
            delay(5);
        }
        if (newData == true) {
            ip = receivedIP.toInt();
            receivedIP = "";
            newData = false;
            Serial.println("Ip address correct");
            Serial.println(ip);
            hasIP = true;
        }
        if(error) {
            Serial.println("Ip address incorrect");
            error = false;
        }
    }

}

byte hexToByte(char char1, char char2){
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
