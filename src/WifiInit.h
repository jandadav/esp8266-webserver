#ifndef WifiInit_h
#define WifiInit_h
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "Secrets.h"

class WifiAgent
{
  public:
    WifiAgent();
    void start();


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

  Serial.println("WifiAgent start called");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}



#endif