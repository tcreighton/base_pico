
#include <iostream>
#include <class/cdc/cdc_device.h>
#include <device/usbd.h>
#include <hardware/gpio.h>
#include <pico/time.h>

#include "commands.hpp"
#include "communication.hpp"
#include "pystring.h"

namespace CScommunication {

    constexpr uint16_t msDelayEachTry = 100;
    constexpr uint16_t maxDelayTries = 30;  // should initialize in about 1500 milliseconds.

    // Static member definitions
    bool Communication::usbEnabled_ = false;
    uint16_t Communication::usbConnectTime_ = 0;
    DisplayMode Communication::displayMode_ = DisplayMode::GAMMA_DISPLAY;
    bool Communication::uartEnabled_[2] = {false, false};
    uint Communication::actualUartBaudRate_[2] = {0, 0};

    CommInterface Communication::getActiveCommInterface() {
        // Priority 1: USB if connected
        if (isUsbEnabled()) {
            return CommInterface::USB;
        }

        // Priority 2: Command handler UART if available
        // ReSharper disable once CppDFAConstantConditions
        if (const auto* cmdUart = getCommandUartConfig(); nullptr != cmdUart && cmdUart->enabled) {
            if (CSdrivers::UartId uartId = CSdrivers::BOARD.uart.getCommandHandlerId();
                uartEnabled_[static_cast<size_t>(uartId)]) {
                return CommInterface::UART;
            }
        }

        // Priority 3: Any enabled UART (fallback for backward compatibility)
        if (uartEnabled_[0] || uartEnabled_[1]) {
            return CommInterface::UART;
        }

        return CommInterface::NONE;
    }

    uart_inst_t* Communication::getCommandUartHardware() {  // Can return nullptr
        const auto* cmdUart = getCommandUartConfig();
        // ReSharper disable once CppDFAConstantConditions
        if (nullptr == cmdUart) {   // This is always true for board 0 and A. Can't predict others.
            return nullptr;         // Could become reachable based on compile options.
        }

        // Use runtime lookup instead of stored pointer
        return CSdrivers::getUartHardware(cmdUart->uartId);
    }

    const CSdrivers::SingleUARTConfig* Communication::getCommandUartConfig() {
        return CSdrivers::BOARD.uart.getCommandHandlerUART(); // can return nullptr
    }


    /**
     * Extract potential commands from the buffer.
     */
    void Communication::handleInputBuffer() {
        static std::string inputBuffer;
        std::string commandString;

        switch (getActiveCommInterface()) {

            case CommInterface::USB:
                tud_task();
                while (tud_cdc_available()) {
                    // Because the while loop quits when no characters are in the tud input buffer,
                    // we can use blocking read_char
                    if (const char ch = tud_cdc_read_char(); processInputCharacter(ch, inputBuffer)) {
                        // If we return true, we have a termination character.
                        // We need to process the command string.
                        commandString = inputBuffer;
                        recordCommands(commandString);
//                        CScommands::Command::recordCommandString(commandString);
                        inputBuffer.clear();
                        return; // Process one command at a time.
                    }
                }
                break;

            case CommInterface::UART: {
                uart_inst_t* uart_hw = getCommandUartHardware();  // Could be nullptr
                // ReSharper disable once CppDFAConstantConditions
                while (nullptr != uart_hw && uart_is_readable(uart_hw)) { // compiler options could make this nullptr
                    if (const char ch = uart_getc(uart_hw); processInputCharacter(ch, inputBuffer)) {
                        // If we return true, we have a termination character.
                        // We need to process the command string.
                        commandString = inputBuffer;
                        recordCommands(commandString);
//                        CScommands::Command::recordCommandString(commandString);
                        inputBuffer.clear();
                        return; // Process one command at a time.
                    }
                }
            }
                break;

            case CommInterface::NONE: // no active interface - skip it.
                break;
        }
    }

    // If a termination character was found, we return true, else false.
    bool Communication::processInputCharacter(const char ch, std::string& inputBuffer) {
        bool retVal = false;

        switch (ch) {
            case LF:
            case CR:
                retVal = true;
                break;
            case BS:  // backspace
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
                break;
            default:
                if (ch == SP || ch == PLUS || ch == MINUS || ch == PERIOD || ch == SEMICOLON ||
                    ch == EQUALS || ch == UNDERSCORE ||
                    (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9')) {
                        inputBuffer += ch;
                    }
                break;
        }
        return retVal;
    }

    bool Communication::initUsb() {
        uint16_t msWaited = 0U;
        auto maxTries = maxDelayTries;

        while (maxTries > 0U && !tud_cdc_connected()) {
            sleep_ms(msDelayEachTry);
            maxTries--;
            msWaited += msDelayEachTry;
        }

        if (tud_cdc_connected()) {
            setUsbEnabled();
        }

        usbConnectTime_ = msWaited; // Note the result. We might have quit before a connection was made. Doesn't matter!

        return isUsbEnabled(); // Note the result. We might have quit before a connection was made. Doesn't matter!
    }

    bool Communication::initUart(const CSdrivers::UartId uartId) {
        const auto& config = CSdrivers::BOARD;
        
        // Get the specific UART configuration
        const auto& uart_config = (uartId == CSdrivers::UartId::UART0) ?
                                    config.uart.uartConfig0 : config.uart.uartConfig1;
        
        // Check board capabilities first
        const bool board_supports_uart = (uartId == CSdrivers::UartId::UART0) ?
                                            config.capabilities.hasUART0 : config.capabilities.hasUART1;
        
        if (!board_supports_uart || !uart_config.enabled) {
            uartEnabled_[static_cast<size_t>(uartId)] = false;
            return false;
        }
        
        // Validate configuration before proceeding
        if (!uart_config.isValid() ||
            CSdrivers::isInvalidGPIOPin(uart_config.tx_pin) ||
            CSdrivers::isInvalidGPIOPin(uart_config.rx_pin)) {
            uartEnabled_[static_cast<size_t>(uartId)] = false;
            return false;
        }

        // Get hardware instance at runtime
        uart_inst_t* uart_hw = CSdrivers::getUartHardware(uart_config.uartId);

        // Initialize UART hardware
        actualUartBaudRate_[static_cast<size_t>(uartId)] =
            uart_init(uart_hw, uart_config.baud_rate);

        // Initialize GPIO pins for UART
        initUartGpio(uartId);

        // Configure UART settings
        uart_set_hw_flow(uart_hw, false, false);
        uart_set_format(uart_hw, 8, 1, UART_PARITY_NONE);
        uart_set_fifo_enabled(uart_hw, false);
        sleep_us(10);
        uart_set_fifo_enabled(uart_hw, true);

        // Flush buffers
        for (int count = 0; count < 100 && uart_is_readable(uart_hw); count++) {
            uart_getc(uart_hw);
        }

        while (uart_is_writable(uart_hw) && !uart_is_enabled(uart_hw)) {
            sleep_us(1);
        }

        uartEnabled_[static_cast<size_t>(uartId)] = true;
        return true;

    }

    void Communication::initUartGpio(const CSdrivers::UartId instance) {
        const auto& config = CSdrivers::BOARD;
        
        // Get the specific UART configuration
        const auto& uart_config = (instance == CSdrivers::UartId::UART0) ?
                                  config.uart.uartConfig0 : config.uart.uartConfig1;
        
        // Double-check that this UART should be initialized
        const bool board_supports_uart = (instance == CSdrivers::UartId::UART0) ?
                                   config.capabilities.hasUART0 : config.capabilities.hasUART1;
                                   
        if (!board_supports_uart || !uart_config.enabled || !uart_config.isValid()) {
            return;
        }
        
        gpio_init(uart_config.rx_pin);
        gpio_init(uart_config.tx_pin);
        gpio_set_function(uart_config.tx_pin, GPIO_FUNC_UART);
        gpio_set_function(uart_config.rx_pin, GPIO_FUNC_UART);
    }

    void Communication::serialOutput(const std::string &output) {
        // Send to USB if connected - ie if UART is not enabled.

        switch (getActiveCommInterface()) {
            case CommInterface::USB:
                std::cout << output << std::flush;
                break;

            case CommInterface::UART:
                // ReSharper disable once CppDFAConstantConditions
                if (uart_inst_t* uart_hw = getCommandUartHardware(); nullptr != uart_hw) {
                    uart_puts(uart_hw, output.c_str());
                }
                break;

            case CommInterface::NONE:
                // No output interface available - could log error or buffer
                break;
        }
    }

    void Communication::serialOutputLine(const std::string& output) {
        std::stringstream ss;
        ss << output << sCR << sLF;
        serialOutput(ss.str());
    }


    void Communication::serialOutputFocus500(const std::string &msg) {
        std::stringstream ss;
        // First make sure that all LF turn into CRLF. Shouldn't hurt Linux, but a must for MS and for ftdi
        for (auto& ch : msg) {
            if ('\n' == ch) {
                ss << '\r';
            }
            ss << ch;   // This could give CRCRLF. Should not hurt.
        }
        std::string const msg2 = ss.str();
        ss.str("");
        ss.clear();

        ss << sSTX << msg2 << sETX << FOCUS500_MESSAGE_TERMINATOR;
        std::string const s = ss.str();
        serialOutput(s);
    }

    void Communication::serialOutputGamma(const std::string& msg) {
        std::stringstream ss;
        // ReSharper disable once CppDFAConstantConditions
        if (DisplayMode::WRAPPED_GAMMA_DISPLAY == getDisplayMode()) {
            ss << sSTX << msg << sETX << GAMMA_MESSAGE_TERMINATOR;
        } else {
            ss << msg << GAMMA_MESSAGE_TERMINATOR;
        }
        serialOutput(ss.str());
    }

    void Communication::displayStandardGammaError() {
        serialOutputGamma(STANDARD_GAMMA_ERROR);
    }

    void Communication::displayStandardGammaPositiveResponse() {
        serialOutputGamma(STANDARD_GAMMA_SUCCESS);
    }

    uint Communication::getCommandUartBaudRate() {
        if (CommInterface::UART == getActiveCommInterface()) {
            return actualUartBaudRate_[static_cast<size_t>(CSdrivers::BOARD.uart.getCommandHandlerId())];
        } else {
            return 0;
        }
    }

    std::string Communication::getActiveInterfaceInfo() {
        switch (getActiveCommInterface()) {
            case CommInterface::USB:
                return "USB";
            case CommInterface::UART: {
                CSdrivers::UartId uartId = CSdrivers::BOARD.uart.getCommandHandlerId();
                return "UART" + std::to_string(static_cast<int>(uartId));
            }
            case CommInterface::NONE:
                return "None";
        }
        return "Unknown";
    }

    void Communication::recordCommands(const std::string &commandString) {
        std::vector<std::string> tokens;
        pystring::split(commandString, tokens, sSEMI);
        for (const auto& token : tokens) {
            CScommands::Command::recordCommandString(token);
        }
    }
}
