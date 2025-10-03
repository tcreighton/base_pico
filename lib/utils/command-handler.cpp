
#include "command-handler.hpp"

#include <iostream>

#include "commands.hpp"
#include "pico/time.h"

namespace CScommands {
    bool CommandHandler::doCommand() {
        auto retValue = false;

        if (const auto optCmd = Command::getNextCommand()) {
            // access command object from the optional wrapper
            const std::unique_ptr<Command>& cmdPtr = optCmd.value();

#ifdef LOG_GROUP_COMMANDS
            const auto startTime = get_absolute_time();
#endif
            // Now retrieve the command struct that also has the parse status.
            if (setCommandStruct(cmdPtr->getCommandStruct());
                CommandStatus::CMD_OK == getCommandStruct().status && handleCommand()) {
                    retValue = true;
                } else {
                    // Report the bad news.
#ifdef LOG_GROUP_COMMANDS
                    logger_.log(LogLevel::Error, std::string("CommandHandler::doCommand(): Command error!"));
                    logger_.log(LogLevel::Error,
                                             getCommandStruct().message + " in " +
                                                     getCommandStruct().commandString);
#endif
                    //  TODO:               ResponseHandler::recordResponse(ResponseCode::F500_INVALID_COMMAND_ERROR);
                    std::cout << "CommandHandler::doCommand(): Command error! commandString: " <<
                                                getCommandStruct().commandToken.tokenString << std::endl;
                }
#ifdef LOG_GROUP_COMMANDS
            const auto endTime = get_absolute_time();
            const auto elapsedTime = absolute_time_diff_us(startTime, endTime);
            logger_.log(LogLevel::Info, std::string("Command time: ") + std::to_string(elapsedTime) + " us");
#endif

        }   // Notice if getNextCommand returns null (nullopt) we just ignore it.


        return retValue;
    }

    bool CommandHandler::handleCommand() {
        auto retValue = false;

        switch (getCommandStruct().commandToken.commandWord) {
            default:
                break;

            case CommandWord::HELP:
                retValue = true;
                break;
        }

        return retValue;
    }
}
