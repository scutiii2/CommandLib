#pragma once
#include "../Command.h"
#include <Arduino.h>
#include <iostream>

class BlinkCommand : public Command
{
    uint8_t pin_;

public:
    BlinkCommand(uint8_t pin) : pin_(pin) { pinMode(pin_, OUTPUT); }
    std::string name() const override { return "blink"; }
    std::string description() const override { return "Blink LED"; }
    std::string usage() const override { return "blink [times] [delay_ms] --fast --slow ~verbose"; }
    std::vector<std::string> aliases() const override { return {"flash"}; }
    std::unordered_map<std::string, std::string> subcommandDescriptions() const override
    {
        return {{"fast", "50ms blink"}, {"slow", "1000ms blink"}};
    }
    std::unordered_map<std::string, std::string> flagDescriptions() const override
    {
        return {{"verbose", "Print each blink"}};
    }

    void execute(const Args &args, const CommandContext &ctx) override
    {
        int times = args.size() > 0 ? atoi(args[0].c_str()) : 3;
        int delayMs = args.size() > 1 ? atoi(args[1].c_str()) : 200;
        if (ctx.subcommands.count("fast"))
            delayMs = 50;
        if (ctx.subcommands.count("slow"))
            delayMs = 1000;

        for (int i = 0; i < times; i++)
        {
            if (ctx.flags.count("verbose"))
            {
                Serial.print("Blink ");
                Serial.println(i + 1);
            }
            digitalWrite(pin_, HIGH);
            delay(delayMs);
            digitalWrite(pin_, LOW);
            delay(delayMs);
        }
    }
};
