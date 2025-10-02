#pragma once

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>


#define LOGGER_ENABLED_     // comment this out to remove all logging from the system (production..)

namespace CSutilities {

    // The following definitions allow us to control compilation of log statements (or other things too.)
    // Each group will stand for some set of code to optionally compile.
    //------------------------------------------------------------------
//#define LOG_GROUP_MAIN      // Main code.
//#define LOG_GROUP_I2C       // I2C communications.
//#define LOG_GROUP_PS        // Power Supply.
//#define LOG_GROUP_G1        // Grid 1.
//#define LOG_GROUP_G2        // Grid 2.
//#define LOG_GROUP_G3        // Grid 3.
//#define LOG_GROUP_HV        // High Voltage.
//#define LOG_GROUP_FILAMENT  // Filament.
//#define LOG_GROUP_GPIO      // GPIO.
//#define LOG_GROUP_ADC       // ADC.
//#define LOG_GROUP_DAC         // DAC.
//#define LOG_GROUP_CSI2C       // CSI2C.
//#define LOG_GROUP_MCP24LC32 // MCP24LC32.
//#define LOG_GROUP_COMMANDS  // Commands.
//#define LOG_GROUP_CONTAINER // Container.
//#define LOG_GROUP_GENERIC   // General Purpose - What we would get just randomly setting log statements.
//#define LOG_GROUP_INIT      // Initialization of each component.
//#define LOG_GROUP_G2_HV      // tracks how hv and g2 interact
//#define LOG_GROUP_ONE   // Mcp4728
//#define LOG_GROUP_TWO   // EEProm0
//#define LOG_GROUP_THREE //
//#define LOG_GROUP_FOUR  //
//#define LOG_GROUP_FIVE  //
//#define LOG_GROUP_SIX   //
//#define LOG_GROUP_SEVEN //
//#define LOG_GROUP_EIGHT //
//#define LOG_GROUP_NINE  //
#define LOG_GROUP_TEN   // DEBUG DAC BLOWOUT

    // State Machine and PS Tracking; comment it out to remove each class of stuff.
//#define LOG_GROUP_STATE_ONE     // State 1: Read ADC voltage values for High Voltage (ADC2/AIN0)
//#define LOG_GROUP_STATE_TWO   // State 2: Read ADC current values for High Voltage (ADC2/AIN1)
//#define LOG_GROUP_STATE_THREE // State 3:
//#define LOG_GROUP_STATE_FOUR  // State 4: Read ADC voltage values for Heater (ADC0/AIN0) & Grid 2 (extractor; ADC1/AIN0)
//#define LOG_GROUP_STATE_FIVE  // State 5: Read ADC current values for Heater (ADC0/AIN1) & Grid 2 (extractor; ADC1/AIN1)
//#define LOG_GROUP_STATE_SIX   // State 6: PsG2::controlVoltage(); PsG2 PID controller
//#define LOG_GROUP_STATE_SEVEN // State 7: Read ADC voltage values for Grid 1 (suppressor ADC0/AIN2) and Grid 3 (focus; ADC1/AIN2)
//#define LOG_GROUP_STATE_EIGHT // State 8: Read ADC current values for Grid 1 (suppressor ADC0/AIN3) and Grid 3 (focus; ADC1/AIN3)
//#define LOG_GROUP_STATE_NINE  // State 9: PsG1::controlVoltage(); PsG1 PID controller. PsG3::controlVoltage(); PsG3 PID controller

#define SHOW_INFO_AT_STARTUP  // Show info at startup.

    //------------------------------------------------------------------

/**
* @brief - A simple logger class to handle messages.
 * Log levels are defined. Components can log at these levels.
 * When Logger is constructed, a log level is defined.
 * We need to make this a runtime choice as well.
*/

    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        None
    };

/**
 * This is a simple Logger. The usual levels are supported.
 * It was initially implemented as a Singleton, but to get the Singleton correct and thread-safe was becoming tiresome.
 * I've left some of the Singleton code around in comments.
 * This implementation is a 'nice guy' implementation. If all the classes in the app are nice guys, everthing is fine.
 * It's implemented as a single global declaration in this file after the class declaration.
 * The definition is in logger.cpp.
 * The initial value of logLevel_ is set in its declaration, but can be changed via setLogLevel(),
 * so be careful where that's done!
 * Typically I expect the actual log level to be set from the main function and simply accepted elsewhere.
 */
    class Logger {
        LogLevel logLevel_ = LogLevel::Error; // default private.

    public:

        Logger() = default;

        /**
         * @brief Singleton pattern.
         * getInstance will always return the value of instance since it is static.
         * And since static, the constructor is called only once.
         * @return
         */
        /*
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        Logger(const Logger&) = delete;     // Delete copy constructor.
        Logger& operator=(const Logger&) = delete;  // Delete assignment operator.
        */

        void setLogLevel(const LogLevel level = LogLevel::Error) {
            logLevel_ = level;
        }

        [[nodiscard]] LogLevel getLogLevel() const {
            return logLevel_;
        }

        void logMethodEntry(LogLevel level, const std::string &message) const;

        void logMethodEntry(LogLevel level, const std::string &className, const std::string &methodName,
                            const std::string &message) const;

        void logMethodExit(LogLevel level, const std::string &message) const;

        void logMethodExit(LogLevel level, const std::string &className, const std::string &methodName,
                           const std::string &message) const;

        void logMethodParameters(LogLevel level, const std::string &className, const std::string &methodName,
                                 const std::string &parameterList, const std::string &message) const;

        void log(LogLevel level, const std::string &message) const;

        void log (LogLevel level, const std::string &className, const std::string &methodName,
                                    const std::string &message) const;


        static std::string logLevelToLabel(LogLevel level);

        static LogLevel labelToLogLevel(const std::string &label);

    };

    extern Logger logger_;  // Global variable - one logger.

}   // namespace CSutilities;

#endif // LOGGER_HPP_
