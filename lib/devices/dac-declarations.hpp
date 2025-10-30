#pragma once
#ifndef DAC_DECLARATIONS_HPP_
#define DAC_DECLARATIONS_HPP_

#include <algorithm>
#include <cstdint>

namespace CSdevices {

/**
 * @brief dac-declarations.hpp declares many types, structs, and const values for use with DACs.
 * Specifically, we are dealing with the MCP4728, however we attempt to be
 * flexible enough to handle other DAC types, perhaps with additional declarations.
 *
 * The channel concepts instantiates a collection of values useful when working with the dac device. We simply call the
 * channel CHANNEL_A, but that gets ignored by the DAC interface commands.
 *
 * Some of these are declared outside the class scope just to make them easier to write/read.
 *
 */

    enum class DacId {
        DAC_00 = 0b00,
//        DAC_01
    };
/**
 * @brief - Defines values for four dac channels (IDs)
 * The MCP4728 has 4 channels. The MCP4725 really doesn't have the concept.
 * For the MCP4725, we will use CHANNEL_A, but the value is ignored.
 */

    enum class DacChannelIds {
        CHANNEL_A = 0b00,
        CHANNEL_B,
        CHANNEL_C,
        CHANNEL_D,
        NOT_A_CHANNEL_ID
    };


    /**
 * @brief - Gain determines whether the dac doubles the output range from 0-2.048V to 0-4.096V.
 * Relevant only to the MCP4728
 */
    enum class DacGainValues {
        GAIN_1 = 0b0,  // output range 0-2.048V
        GAIN_2,         // output range 0-4.096V
        NOT_A_GAIN_VALUE
    };

/**
 * @brief - Voltage reference (Vref) can be relative to en external source (VDD)  or an internal source (INT).
 * Relevant only to the MCP4728
 */
    enum class DacVrefValues {
        VREF_VDD = 0b0, // voltage relative to external source
        VREF_INT,       // voltage relative to internal source (provided by the DAC)
        NOT_A_VREF_VALUE
    };

/**
 * @brief The power down options determine how the circuitry surrounding either DAC deals with turning off the circuit.
 * The options are to cut power, or to use a pull-down resistor to drain the circuit. The size of the resistor varies.
 *
 * PD1 | PD0 | Function
 *  0  |  0  | Normal mode
 *  0  |  1  | 1 kOhm resistor to ground
 *  1  |  0  | 100 kOhm resistor to ground
 *  1  |  1  | 500 kOhm resistor to ground
 *
 * Relevant only to the MCP4728
*/
    enum class DacPowerDownValues {
        PD_OFF = 0b00,
        PD_001K,
        PD_100K,
        PD_500K,
        NOT_A_PD_VALUE
    };


/**
 * @brief The following are structures to make bit masking easy and less error-prone.
 * The bit layout for different commands is different between the devices.
 * Declarations unique to MCP4725 and MCP4728 are found in their respective hpp files.
 */

    struct MCPAddressBits {
//        uint8_t readWrite: 1 {0}; // The SDK will shift this left and or in 1 for write, 0 for read!
        uint8_t i2cAddress: 3 {0};
        uint8_t deviceCode: 4 {0};
        uint8_t unused:     1 {0};
    };

    union MCPAddressByte {
        MCPAddressBits bits {0};
        uint8_t addressByte;
    };

    using MCPAddressField_t = MCPAddressByte;   // This type can be used to define the address byte.

    constexpr uint16_t MAX_DAC_COUNTS = 4095;


    static auto clampDacCounts(const uint16_t counts) -> uint16_t {
        return std::min(counts, MAX_DAC_COUNTS);
    }

}   // namespace CSconverters

#endif
