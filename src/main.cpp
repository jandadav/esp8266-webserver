#include <Arduino.h>
#include "WifiAgent.h"
#include "WebServerAgent.h"
#include <FS.h> 
#include "LogHandler.h"

WifiAgent wifiAgent;
WebServerAgent webServerAgent;
LogHandler logHandler;

void setup(void) {
  // order is important for some
  Serial.begin(115200);
  while (!Serial && !Serial.available()) {};
  delay(300);
  Serial.println();

  logHandler.beginRollingFile();
  LOG.verbose(F("=== SYSTEM STARTING ==="));

  wifiAgent.start();
  webServerAgent.begin();

  webServerAgent.commandHandler.addCommandCallback("simple", [](String c) { return (String) ("simple command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("dummy", [](String c) { return (String) ("dummy command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("testPrintf", [](String c) { logHandler.testPrintf(); return (String) ("testPrintf issued");});
  webServerAgent.commandHandler.addCommandCallback("readLogFile", [](String c) { logHandler.readLogFile(); return (String) ("Printing /system.log to serial");});
  webServerAgent.commandHandler.addCommandCallback("clearLogFile", [](String c) { logHandler.clearLogFile(); return (String) ("Clearing /system.log");});
}

void loop(void) {
  wifiAgent.update();
  delay(200);
}