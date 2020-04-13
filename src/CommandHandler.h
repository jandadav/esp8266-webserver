#ifndef CommandHandler_h
#define CommandHandler_h
#include <Arduino.h>

class CommandHandler
{
public:
    String handle(String command);
    void addCommandCallback(String command, String (*f) (String) );
private:
    String keyList[100];
    String (*functionList[100]) (String);
    int callbacksStored = 0;
};

String CommandHandler::handle(String command)
{
    String commandKey = command.substring(0, command.indexOf(' '));
    Serial.println("Handling command: '" + command + "', identified as '" + commandKey + "'");
    String output = "No handler found";
    for (int i = 0; i < callbacksStored; i++){
        Serial.println("Looking up callback: " + keyList[i]);
        if( commandKey == keyList[i] ) {
            Serial.println("Found match, executing: " + keyList[i]);
            output =  (*functionList[i]) ( command.substring(keyList[i].length() + 1) ); 
            break;
        }
    }
    return output;
};

void CommandHandler::addCommandCallback(String command, String (*f) (String)) {
    // TODO: handle uniqueness of keys
    keyList[callbacksStored] = command;
    functionList[callbacksStored] = f;
    callbacksStored++;
    Serial.println("Added command callback: '" + command + "', total commands: " + callbacksStored);
};

#endif