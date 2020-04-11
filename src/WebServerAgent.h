#ifndef WebServerAgent_h
#define WebServerAgent_h
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebServerAgent
{
public:
    WebServerAgent();
};

WebServerAgent::WebServerAgent()
{
    AsyncWebServer server(80);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String responseText = "{\"value\": \"";
        responseText += "Not implemented";
        responseText += "\"}";
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", responseText);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    server.begin();
}

#endif