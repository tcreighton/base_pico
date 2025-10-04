#pragma once
#ifndef COMMAND_HANDLER_HPP_
#include "command-declarations.hpp"

namespace CScore {

    class CommandHandler final {

    public:
        CommandHandler() = default;

        bool doCommand ();

    private:

        CommandStructure commandStruct_{};
        const CommandStructure& getCommandStruct () { return commandStruct_; }
        void setCommandStruct (const CommandStructure& commandStruct) {commandStruct_ = commandStruct; }

        // Handler function declarations follow:

        bool handleCommand ();  // dispatches based on the commandStruct command word.
    };
}

#endif  // COMMAND_HANDLER_HPP_
