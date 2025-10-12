#pragma once
#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <hardware/gpio.h>


namespace CScore {

    /**
     * GPIO management class that handles board-specific pin configurations
     */
    class Gpio final {

    private:
        Gpio() = default;
        ~Gpio() = default;

    public:
        /**
         * Initialize GPIO pins based on the current board configuration
         * Note: UART pins are initialized separately in Communication::initUart()
         */
        static void init();

        /**
         * Initialize only I2C pins - can be called independently
         */
        static void initI2CPins();

        /**
         * Initialize only input pins - can be called independently
         */
        static void initInputPins();

        /**
         * Initialize only output pins - can be called independently
         */
        static void initOutputPins();

        // Input pin reading functions
        // This changes based on the compile-time definition of BOARD.
        // As such, it's not always going to return false.
        static bool isDoorClosed();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        static bool isHVEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        static bool is48VoltEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        static bool isIonPumpEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        static bool isFlowEnabled();

        // Heater control functions
        static void shutDownHeater() { setHeaterShutdown(true); }
        static void startUpHeater() { setHeaterShutdown(false); }
        static void setHeaterShutdown(bool shutdown);

        // G1 control functions
        static void shutDownG1() { setG1Shutdown(true); }
        static void startUpG1() { setG1Shutdown(false); }
        static void setG1Shutdown(bool shutdown);

        // G2 control functions
        static void shutDownG2() { setG2Shutdown(true); }
        static void startUpG2() { setG2Shutdown(false); }
        static void setG2Shutdown(bool shutdown);

        // G3 control functions
        static void shutDownG3() { setG3Shutdown(true); }
        static void startUpG3() { setG3Shutdown(false); }
        static void setG3Shutdown(bool shutdown);

        // HV control functions
        static void shutDownHV() { setHVShutdown(true); }
        static void startUpHV() { setHVShutdown(false); }
        static void setHVShutdown(bool shutdown);

        // HV relay control functions
        static void enableHVRelay() { setHVEnabled(true); }
        static void disableHVRelay() { setHVEnabled(false); }
        static void setHVEnabled(bool enabled);

        static constexpr bool boardHasUART0();
        static constexpr bool boardHasUART1();
        static constexpr bool boardHasInterlocks();
        static constexpr bool boardHasHVControl();

        // Utility functions for board information
        static constexpr bool boardHasUART();


    private:
        /**
         * Set initial safe states for all output pins
         */
        static void setInitialOutputStates();
    };

} // namespace CScore

#endif // GPIO_HPP_