#ifndef WebServerAgent_h
#define WebServerAgent_h
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "CommandHandler.h"
#include "LogHandler.h"

const String PARAM_COMMAND = "command";
const String PARAM_FILE = "file";

class WebServerAgent
{
public:
    AsyncWebServer *server;
    WebServerAgent();
    void begin();
    CommandHandler commandHandler;
};

WebServerAgent::WebServerAgent()
{
    server = new AsyncWebServer(80);
}

void WebServerAgent::begin()
{
    LOG.verbose(F("Adding [GET] '/' handler"));
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hello from MCU");
    });

    LOG.verbose(F("Adding [GET] '/data' handler"));
    server->on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse *response = new AsyncJsonResponse();
        response->addHeader("Access-Control-Allow-Origin", "*");
        JsonVariant &root = response->getRoot();
        root["heap"] = ESP.getFreeHeap();
        root["vcc"] = ESP.getVcc();
        root["ssid"] = WiFi.SSID();
        response->setLength();
        request->send(response);
    });

    LOG.verbose(F("Adding [GET] '/fs' handler"));
    server->on("/fs", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam(PARAM_FILE)) {
            String param = request->getParam(PARAM_FILE)->value();
            LOG.trace(F("/fs requesting file: %s"), param.c_str());
            if (SPIFFS.exists(param)) {
                AsyncWebServerResponse *response = request->beginResponse(SPIFFS, param);
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
            } else {
                request->send(404, "text/plain", "File '" + param + "' not found on fs");
            }
        } else {
            request->send(400, "text/plain", "Query parameter 'file' missing");
        }
        
    });

    LOG.verbose(F("Adding [POST] '/command' handler"));
    server->on("/command", HTTP_POST, [this](AsyncWebServerRequest *request) {
        
        if (request->hasParam(PARAM_COMMAND, true))
        {
            String command = request->getParam(PARAM_COMMAND, true)->value();
            String commandOutput = commandHandler.handle(command);
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", commandOutput);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
        else
        {
            AsyncWebServerResponse *response = request->beginResponse(400, "text/plain", "'command' parameter not found in request");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
    });

    LOG.verbose(F("Adding 404 handler"));
    server->onNotFound([](AsyncWebServerRequest *r) { r->send(404, "text/plain", "Not found"); });
    server->begin();
    LOG.verbose(F("WebServerAgent started"));
}

#endif