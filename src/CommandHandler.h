#ifndef CommandHandler_h
#define CommandHandler_h
#include <Arduino.h>
class CommandHandler
{
public:
    String handle(String command);
    void addCommandCallback(String command, String (*f) (String) );
private:
    
    struct callback {
        String key;
        String (*f) (String);
    };
    
    struct callback callbackList[100];
    int numCallbacks = 0;
};



String CommandHandler::handle(String command)
{
    Serial.println("Handling command: " + command);
    String output = "No handler found";
    // TODO this should use Vector instead of struct array
    for (size_t i = 0; i < sizeof(callbackList); i++)
    {
        Serial.println("Looking up command: " + callbackList[i].key);
        if (callbackList[i].key == command) {
            Serial.println("Executing command function");
            output = callbackList[i].f(command);
            break;
        }
    }
    return output;
};

void CommandHandler::addCommandCallback(String command, String (*f) (String)) {
    struct callback c {command, f};
    callbackList[numCallbacks] = c;
    numCallbacks++;
    Serial.println("Added command callback: " + command + " total commands: "+numCallbacks);
};

#endif