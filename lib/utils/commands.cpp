
#include <queue>

#include "command-declarations.hpp"
#include "commands.hpp"
#include "pystring.h"


namespace CScore {

    std::queue<std::string> Command::commandStrings_;    // definition of the private queue.

    std::optional<std::unique_ptr<Command>> Command::getNextCommand() {
        std::optional<std::unique_ptr<Command>> uniqueCmdPtr = std::nullopt;
        std::string nextCommandString; // initialized here. If the vector is empty, this will be empty.

        if (!commandStrings_.empty()) {
            nextCommandString = commandStrings_.front();
            commandStrings_.pop();
        }

        if (!nextCommandString.empty()) {
            uniqueCmdPtr = create(nextCommandString);
        }

        return uniqueCmdPtr;
    }

    Command::Command(const std::string& commandString) {
        // The rest of the command structure has a default initialization.
        // See its declaration in command-declarations.hpp

        // Parse the commandString and build the command.
        const auto cmdString = pystring::strip(pystring::lower(commandString)); // trim and lower
        commandStruct_.commandToken.tokenString = cmdString;    // That's all for now - string is in the struct.

        parseCommand(); // Return value doesn't really matter. The commandStruct_.status is what counts.
    }

    std::unique_ptr<Command> Command::create(const std::string &commandString) {
        return std::make_unique<Command>(commandString);
    }

    bool Command::parseCommand() {
        return false;
    }

    bool Command::parseCommand(const std::vector<std::string> &tokens) {
        return false;
    }
}
