#ifndef WebServerAgent_h
#define WebServerAgent_h
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebServerAgent
{
public:
    AsyncWebServer *server;
    WebServerAgent();
    void begin();
};

WebServerAgent::WebServerAgent()
{
    server = new AsyncWebServer(80);
}

void WebServerAgent::begin()
{
    Serial.println("Adding [GET] / handler");
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hello, world");
    });

    Serial.println("Adding 404 handler");
    server->onNotFound( [](AsyncWebServerRequest* r) {r->send(404, "text/plain", "Not found");} );
    server->begin();
    Serial.println("WebServerAgent started");
}

#endif