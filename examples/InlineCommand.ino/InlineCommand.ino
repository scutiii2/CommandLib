#include <Arduino.h>
#include <CommandController.h>

CommandController controller;

class ToggleLEDCommand : public Command
{
    uint8_t pin_;

public:
    ToggleLEDCommand(uint8_t pin) : pin_(pin)
    {
        pinMode(pin_, OUTPUT);
    }

    std::string name() const override { return "toggle"; }
    std::string description() const override { return "Toggle LED on/off"; }
    std::vector<std::string> aliases() const override { return {"t"}; }

    void execute(const Args &args, const CommandContext &ctx) override
    {
        int times = 1;
        if (!args.empty())
            times = atoi(args[0].c_str());

        for (int i = 0; i < times; i++)
        {
            digitalWrite(pin_, HIGH);
            delay(200);
            digitalWrite(pin_, LOW);
            delay(200);

            if (ctx.flags.count("verbose"))
            {
                Serial.print("Toggled LED ");
                Serial.println(i + 1);
            }
        }
    }
};

void setup()
{
    Serial.begin(9600);
    controller.registerCommand(new ToggleLEDCommand(13));
}

void loop()
{
    // Execute commands as strings
    controller.executeCommand("toggle 3 ~verbose"); // Toggle LED 3 times with verbose
    controller.executeCommand("t 2");               // Using alias "t"
    controller.executeCommand("help");              // Show available commands
    delay(5000);
}
