#pragma once
#ifndef COMMAND_DECLARATIONS_HPP_
#include <cstdint>
#include <string>

namespace CScommands {

    enum class CommandWord : uint8_t {
        HELP    = 0,
        SHOW_INFO,          // Display product and version info.
        UNKNOWN
    };

    enum class CommandStatus : uint8_t {
        CMD_OK   = 0,       // Parse worked. All pieces needed are there.
        UNKNOWN_COMMAND,    // Did not recognize the CommandWord
        INVALID_PARAMETER,  // Some other syntax error - don't know if this is needed.
        UNKNOWN_PARAMETER,  // This just lets the command handler decide what to do with it. See the test command.
        QUIT,               // Quit the command loop
        NOT_VALID_STATUS    // Any other problem.
    };

    // Every CommandWord has a language token. But there are language tokens for all syntax elements.
    enum class LanguageTokenId {
        UNKNOWN,
        HELP,
        SHOW_INFO,
    };

    struct TokenValue {
        LanguageTokenId tokenId{LanguageTokenId::UNKNOWN};
        std::string     tokenString;
        CommandWord     commandWord{CommandWord::UNKNOWN};    // If it is a command token this gets filled in.
        float           tokenValue{0};     // If it needs to be an integer, we just lround it.
    };

    struct CommandStructure {
        TokenValue      commandToken;  // The command token that was found.
        CommandStatus   status{CommandStatus::NOT_VALID_STATUS};    // The status of the command.
        float           numbers[3]{0,0,0};     // Holds 3 numeric parameters
        std::string     strings[3]{"","", ""};     // Holds 3 string parameters
    };

}

#endif  // COMMAND_DECLARATIONS_HPP_
