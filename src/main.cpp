#include <Arduino.h>
#include "WifiAgent.h"
#include "WebServerAgent.h"
#include <Log4Esp.h>
#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

const char *FILENAME = "/logger.log";
Logger _logger;

WifiAgent wifiAgent;
WebServerAgent webServerAgent;



void testPrintf(Logger logger) {
  // http://www.cplusplus.com/reference/cstdio/printf/

  int d = 392;
  uint16_t u = 7235;
  uint16_t o = 10; // 12 octal
  uint16_t x = 15; // f
  uint16_t X = 15; // F
  float f = 392.65;
  float F = 392.65;
  float e = 3.9265e+2;
  float E = 3.9265e+2;
  float g = 392.65;
  float G = 392.65;
  String s = "sample";
  char c = 'a';

  logger.verbose("int = [%d]", d);                   // V|int = [392]
  logger.verbose("uint16_t = [%u]", u);              // V|uint16_t = [7235]
  logger.verbose("octal = [%o]", o);                 // V|octal = [12]
  logger.verbose("hexadecimal = [%x]", x);           // V|hexadecimal = [f]
  logger.verbose("hexadecimal uppercase = [%X]", X); // V|hexadecimal uppercase = [F]
  logger.verbose("float (f) = [%f]", f);             // V|float (f) = [392.649994]
  logger.verbose("float (F) = [%F]", F);             // V|float (F) = [3.92649994F+02]
  logger.verbose("float (e) = [%e]", e);             // V|float (e) = [3.926500e+02]
  logger.verbose("float (E) = [%E]", E);             // V|float (E) = [3.926500E+02]
  logger.verbose("float (g) = [%g]", g);             // V|float (g) = [392.65]
  logger.verbose("float (G) = [%G]", G);             // V|float (G) = [392.65]
  logger.verbose("String = [%s]", s.c_str());        // V|String = [sample]
  logger.verbose("char = [%c]", c);                  // V|char = [a]
  logger.verbose(F("PROGMEM"));                      // V|PROGMEM
  logger.verbose("%%");                              // V|%
  logger.verbose("%s(%s:%d)", __func__, __FILE__, __LINE__); // V|testPrintf(../arduino/libraries/log4Esp/examples/BasicLogging/BasicLogging.ino:137)
}

void callMyLoggerWithAppender(Logger logger) {
  Serial.println("+------------------------+");
  Serial.println("| MyLogger with Appender |");
  Serial.println("+------------------------+");

  testPrintf(logger);
}

void readLogFile() {
  Serial.printf("\nOutput log file: %s\n\n", FILENAME);

  if (SPIFFS.exists(FILENAME)) {
    File file = SPIFFS.open(FILENAME, "r");
    String line;
    while (file.available()) {
      line = file.readStringUntil('\n');
      Serial.println(line);
    }
    file.close();
  }
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial && !Serial.available()) {};
  delay(300);
  Serial.println();

  SPIFFS.begin(); // don't forget to start SPIFFS first

  _logger.getAppender().push_back(new RollingFileAppender(FILENAME, 32, 10, true));
  
  wifiAgent.start();
  webServerAgent.begin();
  webServerAgent.commandHandler.addCommandCallback("simple", [](String c) { return (String) ("simple command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("dummy", [](String c) { return (String) ("dummy command handler receiving: "+c);});
  webServerAgent.commandHandler.addCommandCallback("logger", [](String c) { callMyLoggerWithAppender(_logger); return (String) ("logger issued");});
  webServerAgent.commandHandler.addCommandCallback("logread", [](String c) { readLogFile(); return (String) ("loggerread issued");});
}

void loop(void) {
  wifiAgent.update();
  delay(200);
}