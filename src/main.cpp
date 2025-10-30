

#include <iostream>
#include <sstream>
#include <string>
// PICO support
#include "pico/time.h"

// Local project includes

#include "communication.hpp"
#include "packed-datetime.hpp"
#include "logger.hpp"
#include "product-info.hpp"
#include "worker.hpp"
#include "utilities.hpp"

using namespace CScore;
using namespace CSutils;

int main()
{


    // Ignore the return value. We are neither displaying it nor executing logic based on it.
    // This call will also initialize the UART0.
    if (!boardInit()) {
        exit(1000); // bad day!
    }
    auto connectTime = SerialComm::getUsbConnectTime(); // If disabled, should be 0.
    auto uartActualBaudRate = SerialComm::getCommandUartBaudRate();

//    sleep_ms(2 * 1000);

#if defined SHOW_INFO_AT_STARTUP

    std::stringstream ss;
    ss << "\r\n\n****************************************************************\n";
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();

    if (SerialComm::isUsbEnabled()) {
        ss << "USB enabled.";
    }
    if (SerialComm::isUartEnabled()) {
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

    Communication::serialOutputLine(s);
    ss = std::stringstream();

    ss << "Current board name: " << CURRENT_BOARD_NAME;
    s = ss.str();
    Communication::serialOutputLine(s);

    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    s = ss.str();
    Communication::serialOutputLine(s);
#endif

    // Setup the application code
    logger_.setLogLevel(LogLevel::Error);   // For prod set this to Fatal.

    CSworkers::Worker worker;

    // TODO: Setup and handle watchdog

//    volatile int debug_marker = 42;
    auto workOk = true;
    while (workOk) {
        constexpr uint16_t artificialLoopDelayMs = 0;

        Communication::handleInputBuffer();

        workOk = worker.doWork();

//        ResponseHandler::showAllResponses();    // Display queued-up client messages.

        /*
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
        */

        sleep_ms(artificialLoopDelayMs);

//        CSstats::Stats::logMainLoopTime();  // Track how much time it takes to loop around.
    } // loop until global status != ready

    /*
    logger_.log(LogLevel::Fatal, "main: globalStatus: " + statusToLabel(getGlobalStatusCode()));
    ResponseHandler::showAllResponses();    // Display queued-up client messages.
    */
    logger_.log(LogLevel::Info, "Exiting Focus Rack 500. Goodbye!\n\n");

    return 0;



}

