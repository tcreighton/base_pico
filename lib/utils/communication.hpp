#pragma once

#ifndef COMM_HPP_
#define COMM_HPP_

#include <string>
#include <bits/stdc++.h>
#include "serial-comm.hpp"

namespace CScommunication {

    static const auto CONSOLE_COMMAND_PROMPT = std::string("Focus500: ");

    static const auto sCR = std::string("\r");        // 0x0D; carriage return: ctl-M
    static const auto sLF = std::string("\n");        // 0x0A; line feed: ctl-J
    static const auto sFF = std::string("\f");        // 0x0C; form feed: ctl-L
    static const auto sBS = std::string("\b");        // 0x08; backspace: ctl-H
    static const auto sHT = std::string("\t");        // 0x09; horizontal tab: ctl-I
    static const auto sVT = std::string("\v");        // 0x0B; vertical tab: ctl-K
    static const auto sSP = std::string(" ");         // 0x20; space: space
    static const auto sDEL = std::string("\x7F");     // 0x7F; delete: ctl-?
    static const auto sESC = std::string("\x1B");     // 0x1B; escape: ctl-[
    static const auto sSTX = std::string("\x02");     // 0x02; start of text: ctl-B
    static const auto sETX = std::string("\x03");     // 0x03; end of text: ctl-C
    static const auto sEOT = std::string("\x04");     // 0x04; end of transmission: ctl-D
    static const auto sSEMI = std::string(";");       // 0x3B; semicolon: semicolon

    static const auto GAMMA_MESSAGE_TERMINATOR = sCR+sLF;
    static const auto FOCUS500_MESSAGE_TERMINATOR = sCR+sLF;  // Originally it was just LF

    static constexpr auto CR = '\r';
    static constexpr auto LF = '\n';
    static constexpr auto FF = '\f';
    static constexpr auto BS = '\b';
    static constexpr auto HT = '\t';
    static constexpr auto VT = '\v';
    static constexpr auto SP = ' ';         // 0x20
    static constexpr auto DEL = '\x7F';
    static constexpr auto ESC = '\x1B';
    static constexpr auto STX = '\x02';
    static constexpr auto ETX = '\x03';
    static constexpr auto EOT = '\x04';
    static constexpr auto NULL_CHAR = '\0';
    static constexpr auto PLUS = '+';       // 0x2b
    static constexpr auto MINUS = '-';      // 0x2d
    static constexpr auto PERIOD = '.';     // 0x2e
    static constexpr auto COMMA = ',';      // 0x2c
    static constexpr auto COLON = ':';      // 0x3a
    static constexpr auto SEMICOLON = ';';  // 0x3b
    static constexpr auto SLASH = '/';      // 0x2f
    static constexpr auto EQUALS = '=';     // 0x3d
    static constexpr auto UNDERSCORE = '_'; // 0x5f
    static constexpr auto QUESTION = '?';   // 0x3f
    static constexpr auto AT = '@';         // 0x40
    
    // Comment out the following to disable debug mode within the comm stuff.
    // #define COMM_DEBUG_MODE
    
    const static auto STANDARD_GAMMA_ERROR = std::string("?");
    const static auto STANDARD_GAMMA_SUCCESS = std::string("OK");
    const static auto STANDARD_GAMMA_ERROR_WRAPPED = sSTX + STANDARD_GAMMA_ERROR + sETX;
    const static auto STANDARD_GAMMA_SUCCESS_WRAPPED = sSTX + STANDARD_GAMMA_SUCCESS + sETX;
    const static auto STANDARD_F500_SUCCESS = STANDARD_GAMMA_SUCCESS;
    const static auto STANDARD_F500_ERROR = std::string("Error");

    enum class DisplayMode : uint8_t {
        STANDARD_DISPLAY,   // Show information independent of request. That is, send info.
        GAMMA_DISPLAY,      // Only show information as response to request.
        WRAPPED_GAMMA_DISPLAY, // GAMMA_DISPLAY wrapped in STX/ETX.
    };

    class Communication {

        Communication() = default;  // Private constructor - everything is static!
        ~Communication() = default;


    public:

        // No copy constructor - no constructor.
        Communication(const Communication& other) = delete;
        Communication& operator=(const Communication& other) = delete;

        static DisplayMode getDisplayMode() {
            return displayMode_;
        }

        static void setDisplayMode(const DisplayMode mode) {
            displayMode_ = mode;
        }

        static bool hasCommandInterface() {
            return CScore::SerialComm::getActiveCommInterface() != CScore::CommInterface::NONE;
        }

        static void handleInputBuffer();

        // Output functions - automatically route to the active interface
        static void serialOutput(const std::string &output);
        static void serialOutputLine(const std::string& output);
        static void serialOutputFocus500(const std::string& msg);
        static void serialOutputGamma(const std::string& msg);
        static void displayStandardGammaError();
        static void displayStandardGammaPositiveResponse();

    private:
        static void recordCommands(const std::string& commandString);
        static bool processInputCharacter(char ch, std::string &inputBuffer);

        // State tracking
        static DisplayMode displayMode_;    // run silent, run deep.

    };

}

#endif  // COMM_HPP_
