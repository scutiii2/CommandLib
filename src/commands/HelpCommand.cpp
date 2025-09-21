#include "HelpCommand.h"
#include "../CommandController.h"
#include <iostream>

HelpCommand::HelpCommand(CommandController *ctrl) : controller(ctrl) {}

std::string HelpCommand::name() const { return "help"; }
std::string HelpCommand::description() const { return "Displays all commands"; }
std::vector<std::string> HelpCommand::aliases() const { return {"h", "?"}; }

void HelpCommand::execute(const Args &, const CommandContext &)
{
    controller->printHelp();
}
