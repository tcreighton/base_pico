#pragma once
#ifndef COMMANDS_HPP_

#include <memory>
#include <optional>
#include <queue>
#include <vector>

#include "command-declarations.hpp"

namespace CSutils {

    class Command final {

    public:

        CommandStructure& getCommandStruct () {
            return commandStruct_;
        }

        /**
 * @brief This is a public factory method to create a new Command instance.
 * @return Command instance
 */
        static std::optional<std::unique_ptr<Command>> getNextCommand ();

        static void recordCommandString (const std::string& commandString) {
            //            uint32_t irqState = spin_lock_blocking(CSutilities::spinLock1); // lock spinlock
            commandStrings_.push(commandString); // toLower called on front call!
            //            spin_unlock(CSutilities::spinLock1, irqState);    // unlock it!

        }

        ~Command() = default;

        /**
         * @brief Constructs a Command from a command string.
         * @note Prefer using getNextCommand() factory method for proper lifecycle management.
         * Direct construction should only be used when you need explicit ownership control.
         */
        explicit Command (const std::string& commandString);


    private:
        /**
         * @brief Custom factory method that is private.
         * This is needed so the public factory method getNextCommand can make a call to a private method
         * that has no macro (std::unique_ptr or std::make_unique) that does not have visibility from public to private.
         * It's a weird step that's needed because of how those macros are implemented.
         * @param commandString
         * @return Command object
         */
        static std::unique_ptr<Command> create(const std::string& commandString);
        bool parseCommand ();
        bool parseCommand (const std::vector<std::string>& tokens);

        CommandStructure commandStruct_;
        std::vector<TokenValue> tokenValues_{};

        std::vector<TokenValue>& getTokenValues () { return tokenValues_; }

        // This holds the command strings.
        // Produced in CScommands::Command::recordCommandString(inputBuffer);
        // Called from CScommunications::handleInputBuffer ()
        static std::queue<std::string> commandStrings_; // Note it is static!

    };
}

#endif  // COMMANDS_HPP_
