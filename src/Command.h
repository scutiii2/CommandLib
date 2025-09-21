#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct CommandContext
{
    std::vector<std::string> args;
    std::unordered_map<std::string, std::vector<std::string>> subcommands;
    std::unordered_set<std::string> flags;
};

class Command
{
public:
    using Args = std::vector<std::string>;
    virtual ~Command() = default;

    virtual std::string name() const = 0;
    virtual std::string description() const { return ""; }
    virtual std::string usage() const { return ""; }
    virtual std::vector<std::string> aliases() const { return {}; }
    virtual std::unordered_map<std::string, std::string> subcommandDescriptions() const { return {}; }
    virtual std::unordered_map<std::string, std::string> flagDescriptions() const { return {}; }

    virtual void execute(const Args &args, const CommandContext &ctx) = 0;
};
