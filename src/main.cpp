#include <Arduino.h>
#include "WifiAgent.h"

WifiAgent wifiAgent;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiAgent.start();
}

void loop(void) {

  wifiAgent.update();

}