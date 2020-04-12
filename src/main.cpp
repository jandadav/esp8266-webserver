#include <Arduino.h>
#include "WifiAgent.h"
#include "WebServerAgent.h"
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

WifiAgent wifiAgent;
WebServerAgent webServerAgent;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiAgent.start();
  webServerAgent.begin();
  webServerAgent.commandHandler.addCommandCallback("simple", [](String c) { return (String) ("Hi Handler "+c);});
}

void loop(void) {

  wifiAgent.update();

}