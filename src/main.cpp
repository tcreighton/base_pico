

#include <iostream>
#include <sstream>
#include <string>
// PICO support
#include "pico/time.h"

// Local project directories
#include "board-init.hpp"
#include "test.hpp"

#include "../lib/utils/communication.hpp"
#include "eeprom0.hpp"
#include "focus500.hpp"
#include "logger.hpp"
#include "packed-datetime.hpp"
#include "stats.hpp"
#include "utilities.hpp"

using namespace CSerrors;
using namespace CSfocus500;
using namespace CScommunication;
using namespace CSutilities;

int main()
{


    setGlobalStatusCode(StatusCode::READY);

    // Ignore the return value. We are neither displaying it nor executing logic based on it.
    // This call will also initialize the UART0.
    if (!CSdrivers::boardInit()) {
        exit(1000); // bad day!
    }
    auto connectTime = Communication::getUsbConnectTime(); // If disabled, should be 0.
    auto uartActualBaudRate = Communication::getCommandUartBaudRate();

//    sleep_ms(2 * 1000);

#if defined SHOW_INFO_AT_STARTUP

    std::stringstream ss;
    ss << "\r\n\n****************************************************************\n";
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();

    if (Communication::isUsbEnabled()) {
        ss << "USB enabled.";
    }
    if (Communication::isUartEnabled()) {
        ss << "UART enabled @ "+ std::to_string(uartActualBaudRate) + " baud. ";
    }
    Communication::serialOutputLine(ss.str());

    /*
    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    Communication::serialOutputLine(ss.str());
    */
    ss = std::stringstream();
    ss << "Focus Rack 500 initializing. USB connect time about " << connectTime << " milliseconds.";
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();
    ss << "Focus Rack 500 Power Supply ID: " << getPicoBoardId();
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();
    auto [seconds, minutes, hours, day, month, year]
                        = PackedDateTime::getUnpackedDateTime(PackedDateTime::getPackedBuildDateTime());
    ss << "Build: " << getFirmwareVersion();
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();
    ss << "Decoded: "   << static_cast<int>(year) << "-"
                        << static_cast<int>(month) << "-"
                        << static_cast<int>(day) << "-"
                        << static_cast<int>(hours) << ":"
                        << static_cast<int>(minutes) << ":"
                        << static_cast<int>(seconds);
    auto s = ss.str();
    /*
    s = "Decoded: 2025-07-24:12:43:0";
    s = "abcdefghijklmnopqrstuvwxyz";
    */
    Communication::serialOutputLine(s);
    ss = std::stringstream();

    ss << "Current board name: " << CSboard::CURRENT_BOARD_NAME;
    s = ss.str();
    Communication::serialOutputLine(s);

    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    s = ss.str();
    Communication::serialOutputLine(s);
#endif

    // Setup the application code
    logger_.setLogLevel(LogLevel::Error);   // For prod set this to Fatal.

    Focus500 focus500;

    // TODO: Setup and handle watchdog

//    volatile int debug_marker = 42;
    focus500.doWork(); // This calls init() on all the classes via the initial state.

    while (StatusCode::READY == getGlobalStatusCode()) {
        constexpr uint16_t artificialLoopDelayMs = 0;

        Communication::handleInputBuffer();

        ResponseHandler::showAllResponses();    // Display queued-up client messages.

        if (const StatusCode localStatus = focus500.doWork(); StatusCode::READY != localStatus) {

#if defined (LOG_GROUP_MAIN)
            logger_.log(LogLevel::Info,
                                     "main: localStatus: " + statusToLabel(localStatus) +
                                     "\n" + "globalStatus: " +
                                     statusToLabel(getGlobalStatusCode()) +
                                     "\n" + "globalMessage: " +
                                     getGlobalErrorMessage()
                                     );

#endif

        }

        sleep_ms(artificialLoopDelayMs);

        if (const StatusCode gs = getGlobalStatusCode();
            StatusCode::SHUT_DOWN != gs && StatusCode::PANIC != gs) {
            setGlobalStatusCode(StatusCode::READY);
            setGlobalErrorMessage("");
            setGlobalResponseCode(ResponseCode::F500_MESSAGE_OK);
            setGlobalResponseType(ResponseHandler::getDefaultResponseType());
        }

        CSstats::Stats::logMainLoopTime();  // Track how much time it takes to loop around.
    } // loop until global status != ready

    logger_.log(LogLevel::Fatal, "main: globalStatus: " + statusToLabel(getGlobalStatusCode()));
    ResponseHandler::showAllResponses();    // Display queued-up client messages.
    logger_.log(LogLevel::Info, "Exiting Focus Rack 500. Goodbye!\n\n");

    return 0;



}

