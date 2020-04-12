## ESP8266 Web server
Stub of server to be used together with https://github.com/jandadav/react-canvasjs

This is meant to be esp8266 project stub to develop onto. Paired with the REST Web ui it can provide data visualization, debugging and command interface.

## Before Build

src/Secrets.h file is required to be created before building, that has the following structure. This file is not checked into scm.
```c
#ifndef Secrets_h
#define Secrets_h
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif
```