#include <Arduino.h>
#include "WifiInit.h"


WifiAgent wifiHelper;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Serial started");
  wifiHelper.start();
 
}

void loop(void) {


}