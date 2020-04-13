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
    Serial.println("Handling command: " + command);
    String output = "No handler found";
    for (int i = 0; i < callbacksStored; i++){
        Serial.println("Looking up callback: " + keyList[i]);
        if(keyList[i] == command) {
            Serial.println("Found match, executing: " + keyList[i]);
            output =  (*functionList[i]) (command); 
            break;
        }
    }
    return output;
};

void CommandHandler::addCommandCallback(String command, String (*f) (String)) {
    // handle uniqueness of keys
    keyList[callbacksStored] = command;
    functionList[callbacksStored] = f;
    callbacksStored++;
    Serial.println("Added command callback: '" + command + "', total commands: " + callbacksStored);
};

#endif