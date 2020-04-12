#ifndef CommandHandler_h
#define CommandHandler_h
#include <Arduino.h>
class CommandHandler
{
public:
    String handle(String command);
};

String CommandHandler::handle(String command)
{
    Serial.println("Handling command: " + command);
    return "";
};
#endif