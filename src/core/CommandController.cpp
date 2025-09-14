#include "CommandController.h"
#include "../commands/HelpCommand.h"
#include <algorithm>
#include <sstream>
#include <iostream>

std::string CommandController::toLower(const std::string &s)
{
    std::string out = s;
    std::transform(out.begin(), s.end(), out.begin(), ::tolower);
    return out;
}

CommandController::CommandController()
{
    registerCommand(new HelpCommand(this));
}

CommandController::~CommandController()
{
    for (auto &e : commands)
        delete e.cmd;
}

void CommandController::registerCommand(Command *cmd)
{
    std::string nameLower = toLower(cmd->name());
    std::vector<std::string> aliasesLower;
    for (auto &a : cmd->aliases())
        aliasesLower.push_back(toLower(a));

    for (auto &e : commands)
    {
        if (e.nameLower == nameLower)
        {
            delete cmd;
            return;
        }
        for (auto &a : e.aliasesLower)
            if (a == nameLower)
            {
                delete cmd;
                return;
            }
    }

    for (auto &alias : aliasesLower)
    {
        for (auto &e : commands)
        {
            if (e.nameLower == alias)
            {
                delete cmd;
                return;
            }
            for (auto &a : e.aliasesLower)
                if (a == alias)
                {
                    delete cmd;
                    return;
                }
        }
    }

    commands.push_back({cmd, nameLower, aliasesLower});
}

void CommandController::unregisterCommand(const std::string &name)
{
    std::string target = toLower(name);
    commands.erase(std::remove_if(commands.begin(), commands.end(),
                                  [&](CmdEntry &e)
                                  {
                                      if (e.nameLower == target)
                                      {
                                          delete e.cmd;
                                          return true;
                                      }
                                      for (auto &a : e.aliasesLower)
                                          if (a == target)
                                          {
                                              delete e.cmd;
                                              return true;
                                          }
                                      return false;
                                  }),
                   commands.end());
}

std::vector<std::string> tokenize(const std::string &input)
{
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;
    while (stream >> token)
        tokens.push_back(token);
    return tokens;
}

void CommandController::executeCommand(const std::string &input)
{
    auto tokens = tokenize(input);
    if (tokens.empty())
        return;

    std::string cmdName = toLower(tokens[0]);
    Command *cmd = nullptr;

    for (auto &e : commands)
    {
        if (e.nameLower == cmdName)
        {
            cmd = e.cmd;
            break;
        }
        for (auto &a : e.aliasesLower)
            if (a == cmdName)
            {
                cmd = e.cmd;
                break;
            }
        if (cmd)
            break;
    }
    if (!cmd)
    {
        std::cout << "Unknown command: " << tokens[0] << "\n";
        return;
    }

    CommandContext ctx;
    Command::Args args;
    std::string currentSub;

    for (size_t i = 1; i < tokens.size(); i++)
    {
        auto &t = tokens[i];
        if (t.rfind("--", 0) == 0)
        {
            currentSub = t.substr(2);
            ctx.subcommands[currentSub] = {};
        }
        else if (t.rfind("~", 0) == 0)
        {
            ctx.flags.insert(t.substr(1));
        }
        else if (!currentSub.empty())
            ctx.subcommands[currentSub].push_back(t);
        else
            args.push_back(t);
    }

    cmd->execute(args, ctx);
}

void CommandController::printHelp() const
{
    std::cout << "Available commands:\n";
    for (auto &e : commands)
    {
        std::cout << "- " << e.cmd->name();
        if (!e.cmd->aliases().empty())
        {
            std::cout << " (aliases: ";
            for (size_t i = 0; i < e.cmd->aliases().size(); i++)
            {
                std::cout << e.cmd->aliases()[i];
                if (i < e.cmd->aliases().size() - 1)
                    std::cout << ", ";
            }
            std::cout << ")";
        }
        if (!e.cmd->description().empty())
            std::cout << ": " << e.cmd->description();
        std::cout << "\n";

        auto subs = e.cmd->subcommandDescriptions();
        if (!subs.empty())
        {
            std::cout << "  Subcommands:\n";
            for (auto &[sub, desc] : subs)
                std::cout << "    --" << sub << ": " << desc << "\n";
        }

        auto flags = e.cmd->flagDescriptions();
        if (!flags.empty())
        {
            std::cout << "  Flags:\n";
            for (auto &[flag, desc] : flags)
                std::cout << "    ~" << flag << ": " << desc << "\n";
        }
    }
}
