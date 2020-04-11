#include <Arduino.h>
#include "WifiInit.h"
#include <ESP8266mDNS.h>

WifiAgent wifiHelper;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiHelper.start();

  if (!MDNS.begin("mcu")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  MDNS.addService("http", "tcp", 80);
}

void loop(void) {

  MDNS.update();

}