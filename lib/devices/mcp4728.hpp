#pragma once

#ifndef MCP4728_HPP_
#define MCP4728_HPP_

#include <string>
#include "component.hpp"
#include "csi2c.hpp"
#include "dac-channel.hpp"

namespace CSdevices {

    CsI2C& getController(ControllerId controllerId); // A little forward reference.

    class DacChannel;   // forward reference to DacChannel

    class Mcp4728 final : public Component {

    public:

        Mcp4728( const std::string& label,
                 const ControllerId controllerId,
                 const DacId dacId) :
                            controllerId_(controllerId),
                            dacId_(dacId) {

            setClassName("DacMcp4728");
            setLabel(label);
        }

        Mcp4728() = delete;
        Mcp4728(const Mcp4728& other) = delete;
        Mcp4728& operator=(const Mcp4728& other) = delete;
        ~Mcp4728 () override = default;

        [[nodiscard]] CsI2C& getController () const {
            return CSdevices::getController(getControllerId());
        }
        [[nodiscard]] ControllerId getControllerId () const {
            return controllerId_;
        }

        [[nodiscard]] bool writeDacInputRegister(const DacChannel& dacChannel, uint16_t data) const;
        [[nodiscard]] DacId getDacType() const { return dacId_; }

    private:
        /**
         * @brief - Commands for MCP4728 & MCP4725
         * Only the commands we are likely to need. Not all are listed.
         * A command block consists of five bits for the 4728: C2 C1 C0 W1 W0
         * The C values are commands and the Ws indicate a write function.
         * The bits defined here are placed in the MSB position in the command byte for MCP4728.
         *
         * The MCP4725 does not define the W1 and W0 write function bits. Their corresponding
         * positions in the command block are labeled Unused. We set the values to 0.
         */
        // MCP4728 Commands --------------------------------------------------------------
        // C0 is X, W1 & W0 are not used: XX. Fast write to all 4 channel input registers
        static constexpr uint8_t MCP4728_CMD_FAST_WRITE = 0b00000;
        // MULTI_WRITE starts with the indicated channel and writes each input register from that channel through D.
        // But if only data for one channel is given, this just writes to one input register.
        static constexpr uint8_t MCP4728_CMD_MULTI_WRITE = 0b01000;  // Writes to input registers, not EEPROM.
        // MULTI_WRITE_EEPROM works like MULTI_WRITE but writes each register value to the EEPROM as well.
        static constexpr uint8_t MCP4728_CMD_MULTI_WRITE_EEPROM = 0b01010;  // Writes to input registers, and EEPROM.
        // SINGLE_WRITE_EEPROM works like MULTI_WRITE_EEPROM, but just for one channel register and EEPROM.
        static constexpr uint8_t MCP4728_CMD_SINGLE_WRITE_EEPROM = 0b01011;  // Writes to one input register, and EEPROM
/**
 * @brief Device code and i2c address. These can differ. But this is pretty much the standard approach.
*/
        static constexpr uint8_t DEVICE_CODE = 0b1100;   // This is the device code for MCP4728
        // Note that the Device code gets placed into the byte like this: 0b01100 0000; bottom 3 bits are I2C_ADDRESS
        // This can be changed via general call, etc. If so, we won't use this constant.
        static constexpr uint8_t I2C_ADDRESS = 0b000;
        static constexpr uint8_t READ_BIT = 0b1;    // these get set by the sdk!
        static constexpr uint8_t WRITE_BIT = 0b0;

        struct Mcp4728_Control_Bits {   // Includes 4 bits of data.
            uint8_t dataNibble: 4;    // This is the most significant 4 bits of the data word
            uint8_t gain: 1;    // gain is MCP4728_GAIN1 or MCP4728_GAIN2
            uint8_t powerDown: 2;    // PD is normal or resistance. (See above.)
            uint8_t vref: 1;    // vref is Vdd or internal (2.048 V.)
        };

        union Mcp4728_Control {
            Mcp4728_Control_Bits bits;
            uint8_t byte;
        };

        using Mcp4728Controls_t = Mcp4728_Control;


        struct Mcp4728CommandBits {
            uint8_t udac: 1;
            uint8_t channel: 2;
            uint8_t command: 5;
        };

        union Mcp4728CommandByte {
            Mcp4728CommandBits bits;
            uint8_t byte;
        };

        using Mcp4728CommandWord_t = Mcp4728CommandByte;


        ControllerId controllerId_;

        DacId dacId_;
    };

}   // namespace CSdevices

#endif

