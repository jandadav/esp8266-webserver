#ifndef TimeHandler_h
#define TimeHandler_h

#include <Arduino.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include "WifiAgent.h"

static const char ntpServerName[] = "cz.pool.ntp.org";
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
const int timeZone = 1;
const unsigned int localPort = 8888;

WiFiUDP Udp;

class TimeHandler {
public:
    void start();
    void update();
    time_t getNtpTime();
    void printDigits(int digits);
    void digitalClockDisplay();
private:
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    bool shouldUpdate = false;
    bool shouldSet = false;
    void sendNTPpacket(IPAddress &address);
};

void TimeHandler::start() {
    Udp.begin(localPort);
    // setSyncProvider(getNtpTime); take care on our own
    setSyncInterval(20);
    update();
}

void TimeHandler::update() {
    switch (timeStatus()){
        case timeStatus_t::timeNotSet:
            Serial.println("Time status: Not Set");
            setTime(getNtpTime());
            break;
        case timeStatus_t::timeNeedsSync:
            Serial.println("Time status: Need Sync");
            setTime(getNtpTime());
            break;
        case timeStatus_t::timeSet:
            // Serial.println("Time status: Time Set"); no spam
            break;
        }
}

time_t TimeHandler::getNtpTime() {
    IPAddress ntpServerIP; // NTP server's ip address
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
        Serial.println("Receive NTP Response");
        Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
        secsSince1900 |= (unsigned long)packetBuffer[43];
        return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
}
Serial.println("No NTP Response :-(");
return 0; // return 0 if unable to get the time
}

void TimeHandler::printDigits(int digits) {
    // utility for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10) Serial.print('0');
    Serial.print(digits);
}

void TimeHandler::digitalClockDisplay() {
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(".");
    Serial.print(month());
    Serial.print(".");
    Serial.print(year());
    Serial.println();
}

void TimeHandler::sendNTPpacket(IPAddress &address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

#endif