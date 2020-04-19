#ifndef WifiInit_h
#define WifiInit_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "Secrets.h"
#include "LogHandler.h"

class WifiAgent
{
  public:
    WifiAgent();
    void start();
    void update();

  private:
    const char* ssid;
    const char* password;
};

WifiAgent::WifiAgent()
{
  Serial.println("WifiAgent Constructor called");
  ssid = STASSID;
  password = STAPSK;
}

void WifiAgent::start()
{
  LOG.verbose(F("WifiAgent start called"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    LOG.verbose(F("."));  
  }
  LOG.verbose(F("Connected to %s"), ssid);
  String ip = WiFi.localIP().toString();
  LOG.verbose(F("IP address: %s"), ip.c_str());
  if (!MDNS.begin("mcu")) {
    LOG.error(F("Error setting up MDNS responder!"));
    while (1) {
      delay(1000);
    }
  }
  LOG.verbose(F("mDNS responder started"));
  MDNS.addService("http", "tcp", 80);
}

void WifiAgent::update() {
  MDNS.update();
}

#endif