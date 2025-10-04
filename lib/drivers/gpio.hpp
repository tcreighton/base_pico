#pragma once
#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <hardware/gpio.h>

#include "board-config.hpp"

namespace CSdevices {

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
        // ReSharper disable once CppDFAConstantFunctionResult
        static bool isDoorClosed() {
            if constexpr (!CSdevices::BOARD.capabilities.hasInterlocks) {
                return false; // Safe default for boards without interlocks
            }
            return gpio_get(CSdevices::BOARD.inputs.door_interlock);
        }

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        // ReSharper disable once CppDFAConstantFunctionResult
        static bool isHVEnabled() {
            if constexpr (!CSdevices::BOARD.capabilities.hasInterlocks) {
                return false;
            }
            return gpio_get(CSdevices::BOARD.inputs.hv_enabled_interlock);
        }

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        // ReSharper disable once CppDFAConstantFunctionResult
        static bool is48VoltEnabled() {
            if constexpr (!CSdevices::BOARD.capabilities.hasInterlocks) {
                return false;
            }
            return gpio_get(CSdevices::BOARD.inputs.voltage_48v_enabled);
        }

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        // ReSharper disable once CppDFAConstantFunctionResult
        static bool isIonPumpEnabled() {
            if constexpr (!CSdevices::BOARD.capabilities.hasInterlocks) {
                return false;
            }
            return gpio_get(CSdevices::BOARD.inputs.pump_interlock);
        }

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
        // ReSharper disable once CppDFAConstantFunctionResult
        static bool isFlowEnabled() {
            if constexpr (!CSdevices::BOARD.capabilities.hasInterlocks) {
                return false;
            }
            return gpio_get(CSdevices::BOARD.inputs.flow_interlock);
        }

        // Heater control functions
        static void shutDownHeater() { setHeaterShutdown(true); }
        static void startUpHeater() { setHeaterShutdown(false); }
        static void setHeaterShutdown(const bool shutdown) {
            if (CSdevices::BOARD.capabilities.hasHVControl) {
                gpio_put(CSdevices::BOARD.outputs.heater_shutdown, shutdown);
            }
        }

        // G1 control functions
        static void shutDownG1() { setG1Shutdown(true); }
        static void startUpG1() { setG1Shutdown(false); }
        static void setG1Shutdown(const bool shutdown) {
            gpio_put(CSdevices::BOARD.outputs.g1_shutdown, shutdown);
        }

        // G2 control functions
        static void shutDownG2() { setG2Shutdown(true); }
        static void startUpG2() { setG2Shutdown(false); }
        static void setG2Shutdown(const bool shutdown) {
            gpio_put(CSdevices::BOARD.outputs.g2_shutdown, shutdown);
        }

        // G3 control functions
        static void shutDownG3() { setG3Shutdown(true); }
        static void startUpG3() { setG3Shutdown(false); }
        static void setG3Shutdown(const bool shutdown) {
            if (CSdevices::BOARD.capabilities.hasHVControl) {
                gpio_put(CSdevices::BOARD.outputs.g3_shutdown, shutdown);
            }
        }

        // HV control functions
        static void shutDownHV() { setHVShutdown(true); }
        static void startUpHV() { setHVShutdown(false); }
        static void setHVShutdown(const bool shutdown) {
            if (CSdevices::BOARD.capabilities.hasHVControl) {
                gpio_put(CSdevices::BOARD.outputs.hv_shutdown, shutdown);
            }
        }

        // HV relay control functions
        static void enableHVRelay() { setHVEnabled(true); }
        static void disableHVRelay() { setHVEnabled(false); }
        static void setHVEnabled(const bool enabled) {
            if (CSdevices::BOARD.capabilities.hasHVControl) {
                gpio_put(CSdevices::BOARD.outputs.hv_enable, enabled);
            }
        }

        // Utility functions for board information
        static constexpr bool boardHasUART() {
            return boardHasUART0() || boardHasUART1();
        }

        static constexpr bool boardHasUART0() {
            return CSdevices::BOARD.capabilities.hasUART0;
        }

        static constexpr bool boardHasUART1() {
            return CSdevices::BOARD.capabilities.hasUART1;
        }

        static constexpr bool boardHasInterlocks() {
            return CSdevices::BOARD.capabilities.hasInterlocks;
        }

        static constexpr bool boardHasHVControl() {
            return CSdevices::BOARD.capabilities.hasHVControl;
        }

        static constexpr CSdevices::BoardRevision getBoardRevision() {
            return CSdevices::BOARD.revision;
        }

    private:
        /**
         * Set initial safe states for all output pins
         */
        static void setInitialOutputStates();
    };

} // namespace CSconfig

#endif // GPIO_HPP_