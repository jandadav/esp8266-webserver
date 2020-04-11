#include <Arduino.h>
#include "WifiAgent.h"
//#include "WebServerAgent.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

WifiAgent wifiAgent;
AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiAgent.start();

  Serial.println("AsyncWebServer started");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("[GET] /");
      request->send(200, "text/plain", "Hello, world");
  });
  server.onNotFound(notFound);

  server.begin();

}

void loop(void) {

  wifiAgent.update();

}