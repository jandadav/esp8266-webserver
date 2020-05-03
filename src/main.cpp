#include <Arduino.h>
#include "WifiAgent.h"
#include "WebServerAgent.h"
#include <FS.h> 
#include "LogHandler.h"
#include "TimeHandler.h"
#include <ArduinoOTA.h>

WifiAgent wifiAgent;
WebServerAgent webServerAgent;
LogHandler logHandler;
TimeHandler timeHandler;

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

  timeHandler.start();

  webServerAgent.commandHandler.addCommandCallback("simple", [](String c) { return (String) ("simple command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("dummy", [](String c) { return (String) ("dummy command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("testPrintf", [](String c) { logHandler.testPrintf(); return (String) ("testPrintf issued");});
  webServerAgent.commandHandler.addCommandCallback("readLogFile", [](String c) { logHandler.readLogFile(); return (String) ("Printing /system.log to serial");});
  webServerAgent.commandHandler.addCommandCallback("clearLogFile", [](String c) { logHandler.clearLogFile(); return (String) ("Clearing /system.log. Restart device");});
  webServerAgent.commandHandler.addCommandCallback("disconnect", [](String c) { wifiAgent.disconnect(); return (String) ("Disconnecting Wifi");});
  webServerAgent.commandHandler.addCommandCallback("time", [](String c) { timeHandler.logTime(); return (String) ("Printing time"); });

    // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("mcu");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin(true);

  LOG.verbose(F("=== STARTUP COMPLETE OTA ==="));
}

void loop(void) {
  // wifiAgent.update();
  timeHandler.update();
  ArduinoOTA.handle();
  delay(200);
}