#include <hardware/gpio.h>
#include "gpio.hpp"

namespace CSconfig {

    void Gpio::init() {
        // Initialize different pin groups
        initI2CPins();
        initInputPins();
        initOutputPins();
        
        // Set safe initial states
        setInitialOutputStates();

    }

    void Gpio::initI2CPins() {
        const auto& config = CSboard::BOARD;
        
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
        const auto& config = CSboard::BOARD;
        
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
        const auto& config = CSboard::BOARD;
        
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
        if (const auto& config = CSboard::BOARD; !config.capabilities.hasHVControl) {
            return;
        }

        // Set safe initial states - shutdown everything initially
        shutDownHeater();    // Pull up (shutdown)
        shutDownG1();        // Pull up (shutdown)
        shutDownG2();        // Pull up (shutdown)
        shutDownG3();        // Pull up (shutdown)
        shutDownHV();        // Pull up (shutdown)
        disableHVRelay();    // Pull down (disable HV relay)
    }

} // namespace CScomponents