#include <Arduino.h>
#include <CommandController.h>
#include "commands/BlinkCommand.h"

CommandController controller;

void setup()
{
    Serial.begin(9600);
    controller.registerCommand(new BlinkCommand(13));
}

void loop()
{
    controller.executeCommand("blink 3 100 --fast ~verbose");
    controller.executeCommand("flash 2 300 --slow");
    controller.executeCommand("help");
    delay(5000);
}
