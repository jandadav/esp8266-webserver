#include <Arduino.h>
#include "WifiAgent.h"
#include "WebServerAgent.h"

WifiAgent wifiAgent;
WebServerAgent webServerAgent;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiAgent.start();
  webServerAgent.begin();
  webServerAgent.commandHandler.addCommandCallback("simple", [](String c) { return (String) ("simple command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("dummy", [](String c) { return (String) ("dummy command handler receiving: "+c);});
}

void loop(void) {
  wifiAgent.update();
}