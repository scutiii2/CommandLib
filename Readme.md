# CommandLib

A lightweight Arduino library to create **commands with arguments, subcommands, flags, and aliases**, all with **case-insensitive execution** and automatic `help` generation.

---

## Features

- Single-level commands with optional **arguments**, **subcommands**, and **flags**
- **Aliases** support for commands (case-insensitive)
- Automatic **help command** with descriptions of commands, subcommands, and flags
- Easy registration and unregistration of commands
- Simple and modular structure: each command can be in its own header/source file
- No blocking input required; works with string-based command execution

---

## Installation

1. Copy the `CommandLib` folder into your Arduino `libraries` directory.
2. Include headers in your sketch:

```cpp
#include <CommandController.h>
#include "commands/BlinkCommand.h"
```

---

## Folder Structure

```
CommandLib/
│
├── library.properties
├── README.md
│
├── src/
│   ├── core/
│   │   ├── Command.h
│   │   ├── CommandController.h
│   │   └── CommandController.cpp
│   │
│   └── commands/
│       ├── HelpCommand.h
│       ├── HelpCommand.cpp
│       └── BlinkCommand.h      <-- Sample Command
│       └── BlinkCommand.cpp    <-- optional, or implement in header
│
└── examples/
    └── CommandExample/
        └── CommandExample.ino
```

---

## Usage

### 1. Include and Initialize

```cpp
#include <Arduino.h>
#include "core/CommandController.h"
#include "commands/BlinkCommand.h"

CommandController controller;
```

### 2. Register Commands

```cpp
void setup() {
    Serial.begin(9600);
    controller.registerCommand(new BlinkCommand(13)); // LED pin 13
}
```

### 3. Execute Commands

```cpp
void loop() {
    controller.executeCommand("blink 3 100 --fast ~verbose");
    controller.executeCommand("flash 2 300 --slow");  // using alias
    controller.executeCommand("help");                // shows all commands
    delay(5000);
}
```

### 4. Command Syntax

```
[command or alias] [arg1] [arg2] ... --[subcommand] ... ~[flag] ...
```

- **Command prefix** is not required in this version (first token is treated as command name or alias)
- **Subcommands** are prefixed with `--`
- **Flags** are prefixed with `~`
- **Arguments** are everything else until a subcommand or flag is detected

Example:

```
blink 5 200 --fast ~verbose
```

- Command: `blink`
- Args: `5`, `200`
- Subcommand: `fast`
- Flag: `verbose`

---

## Creating Your Own Commands

1. Create a new header in `src/commands/`:

```cpp
#pragma once
#include "../core/Command.h"
#include <Arduino.h>

class MyCommand : public Command {
public:
    std::string name() const override { return "mycmd"; }
    std::string description() const override { return "My custom command"; }
    std::vector<std::string> aliases() const override { return {"mc"}; }
    std::unordered_map<std::string,std::string> subcommandDescriptions() const override {
        return { {"sub","Subcommand description"} };
    }
    std::unordered_map<std::string,std::string> flagDescriptions() const override {
        return { {"debug","Print debug info"} };
    }

    void execute(const Args& args, const CommandContext& ctx) override {
        // Your logic here
    }
};
```

2. Register in `setup()`:

```cpp
controller.registerCommand(new MyCommand());
```

---

## Help Command

- Automatically available as `help`, `h`, or `?`
- Shows all commands, subcommands, flags, and their descriptions:

```
> help
- blink (aliases: flash): Blink LED
  Subcommands:
    --fast: 50ms blink
    --slow: 1000ms blink
  Flags:
    ~verbose: Print each blink
```

---

## Notes

- **Case-insensitive**: "Blink", "BLINK", and "flash" all match the same command
- **Single-level subcommands** only (no nested subcommands)
- **Aliases** are enforced unique (case-insensitive)
