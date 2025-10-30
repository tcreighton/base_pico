
#include <hardware/gpio.h>
#include "gpio.hpp"
#include "board-config.hpp"

namespace CScore {

    void Gpio::init() {
        // Initialize different pin groups
        initI2CPins();
        initInputPins();
        initOutputPins();
        
        // Set safe initial states
        setInitialOutputStates();

    }

    void Gpio::initI2CPins() {
        const auto& config = BOARD;
        
        // Initialize I2C pins
        gpio_init(config.i2c.c0_sda);
        gpio_init(config.i2c.c0_scl);
        gpio_init(config.i2c.c1_sda);
        gpio_init(config.i2c.c1_scl);

        // Set I2C function and pull-ups
        gpio_set_function(config.i2c.c0_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c0_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c0_sda);
        gpio_pull_up(config.i2c.c0_scl);
        
        gpio_set_function(config.i2c.c1_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c1_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c1_sda);
        gpio_pull_up(config.i2c.c1_scl);
    }

    void Gpio::initInputPins() {
        const auto& config = BOARD;
        
        /*
        // Only initialize input pins if the board has interlocks
        if (!config.capabilities.hasInterlocks) {
            return;
        }
        */

        // Always initialize. If they aren't used, that's fine.

        // Initialize input pins
        gpio_init(config.inputs.door_interlock);
        gpio_init(config.inputs.hv_enabled_interlock);
        gpio_init(config.inputs.voltage_48v_enabled);
        gpio_init(config.inputs.pump_interlock);
        gpio_init(config.inputs.flow_interlock);
        gpio_init(config.inputs.dac_ready);
        gpio_init(config.inputs.adc1_ready);
        gpio_init(config.inputs.adc2_ready);

        // Set input directions
        gpio_set_dir(config.inputs.door_interlock, GPIO_IN);
        gpio_set_dir(config.inputs.hv_enabled_interlock, GPIO_IN);
        gpio_set_dir(config.inputs.voltage_48v_enabled, GPIO_IN);
        gpio_set_dir(config.inputs.pump_interlock, GPIO_IN);
        gpio_set_dir(config.inputs.flow_interlock, GPIO_IN);
        gpio_set_dir(config.inputs.dac_ready, GPIO_IN);
        gpio_set_dir(config.inputs.adc1_ready, GPIO_IN);
        gpio_set_dir(config.inputs.adc2_ready, GPIO_IN);

        // Configure pull resistors as needed
        // Note: Pump interlock needs no pulls
        gpio_set_pulls(config.inputs.pump_interlock, false, false);
        
        // Set default pulls for other inputs (adjust as needed based on hardware)
        // gpio_set_pulls(config.inputs.door_interlock, false, true);  // pull down
        // gpio_set_pulls(config.inputs.hv_enabled_interlock, false, true);  // pull down
        // gpio_set_pulls(config.inputs.voltage_48v_enabled, false, true);  // pull down
        // gpio_set_pulls(config.inputs.flow_interlock, false, true);  // pull down
    }

    void Gpio::initOutputPins() {
        const auto& config = BOARD;
        
        /*
        // Only initialize output pins if the board has HV control
        if (!config.capabilities.hasHVControl) {
            return;
        }
        */
        // Always initialize. If they aren't used, that's fine.

        // Initialize output pins
        gpio_init(config.outputs.heater_shutdown);
        gpio_init(config.outputs.g1_shutdown);
        gpio_init(config.outputs.g2_shutdown);
        gpio_init(config.outputs.g3_shutdown);
        gpio_init(config.outputs.hv_shutdown);
        gpio_init(config.outputs.hv_enable);
        gpio_init(config.outputs.ldac);

        // Set output directions
        gpio_set_dir(config.outputs.heater_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.g1_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.g2_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.g3_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.hv_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.hv_enable, GPIO_OUT);
        gpio_set_dir(config.outputs.ldac, GPIO_OUT);
    }

    void Gpio::setInitialOutputStates() {
        // Only set initial states if the board has HV control
        if (const auto& config = BOARD; !config.capabilities.hasHVControl) {
            return;
        }

        // Set safe initial states - shutdown everything initially
        // ReSharper disable once CppDFAUnreachableCode
        shutDownHeater();    // Pull up (shutdown)
        shutDownG1();        // Pull up (shutdown)
        shutDownG2();        // Pull up (shutdown)
        shutDownG3();        // Pull up (shutdown)
        shutDownHV();        // Pull up (shutdown)
        disableHVRelay();    // Pull down (disable HV relay)
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Gpio::isDoorClosed() {
        if constexpr (!BOARD.capabilities.hasInterlocks) {
            return false; // Safe default for boards without interlocks
        }
        // ReSharper disable once CppDFAUnreachableCode
        return gpio_get(BOARD.inputs.door_interlock);
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Gpio::isHVEnabled() {
        if constexpr (!BOARD.capabilities.hasInterlocks) {
            return false;
        }
        // ReSharper disable once CppDFAUnreachableCode
        return gpio_get(BOARD.inputs.hv_enabled_interlock);
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Gpio::is48VoltEnabled() {
        if constexpr (!BOARD.capabilities.hasInterlocks) {
            return false;
        }
        // ReSharper disable once CppDFAUnreachableCode
        return gpio_get(BOARD.inputs.voltage_48v_enabled);
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Gpio::isIonPumpEnabled() {
        if constexpr (!BOARD.capabilities.hasInterlocks) {
            return false;
        }
        // ReSharper disable once CppDFAUnreachableCode
        return gpio_get(BOARD.inputs.pump_interlock);
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Gpio::isFlowEnabled() {
        if constexpr (!BOARD.capabilities.hasInterlocks) {
            return false;
        }
        // ReSharper disable once CppDFAUnreachableCode
        return gpio_get(BOARD.inputs.flow_interlock);
    }

    void Gpio::setHeaterShutdown(const bool shutdown) {
        if (BOARD.capabilities.hasHVControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.heater_shutdown, shutdown);
        }
    }

    void Gpio::setG1Shutdown(const bool shutdown) {
        gpio_put(BOARD.outputs.g1_shutdown, shutdown);
    }

    void Gpio::setG2Shutdown(const bool shutdown) {
        gpio_put(BOARD.outputs.g2_shutdown, shutdown);
    }

    void Gpio::setG3Shutdown(const bool shutdown) {
        if (BOARD.capabilities.hasHVControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.g3_shutdown, shutdown);
        }
    }

    void Gpio::setHVShutdown(const bool shutdown) {
        if (BOARD.capabilities.hasHVControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.hv_shutdown, shutdown);
        }
    }

    void Gpio::setHVEnabled(const bool enabled) {
        if (BOARD.capabilities.hasHVControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.hv_enable, enabled);
        }
    }

    /*
    constexpr bool Gpio::boardHasUART0() {
        return BOARD.capabilities.hasUART0;
    }
    */

    constexpr bool Gpio::boardHasUART0() {
        return BOARD.capabilities.hasUART0;
    }

    constexpr bool Gpio::boardHasUART1() {
        return BOARD.capabilities.hasUART1;
    }

    constexpr bool Gpio::boardHasInterlocks() {
        return BOARD.capabilities.hasInterlocks;
    }

    constexpr bool Gpio::boardHasHVControl() {
        return BOARD.capabilities.hasHVControl;
    }

    constexpr bool Gpio::boardHasUART() {
        // ReSharper disable once CppDFAUnreachableCode
        // ReSharper disable once CppRedundantBooleanExpressionArgument
        return boardHasUART0() || boardHasUART1();
    }

    /*
    constexpr BoardRevision Gpio::getBoardRevision() {
        return BOARD.revision;
    }
    */



} // namespace CScore
