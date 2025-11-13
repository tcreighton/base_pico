#pragma once
#ifndef MCP4725_HPP_
#define MCP4725_HPP_


#include <string>

#include "csi2c.hpp"
#include "dac-declarations.hpp"

namespace CSdevices {

    class Mcp4725 final : public Component {
    public:


        Mcp4725( const std::string& label,
                 const ControllerId controllerId,
                 const DacPowerDownValues powerDownValue = DacPowerDownValues::PD_OFF) :// Probably only value we'll see.
                            powerDownValue_(powerDownValue),
                            controllerId_(controllerId) {

            setClassName("DacMcp4725");
            setLabel(label);

        }

        Mcp4725() = delete;
        Mcp4725(const Mcp4725& other) = delete;
        Mcp4725& operator=(const Mcp4725& other) = delete;
        ~Mcp4725 () override = default;

        [[nodiscard]] ControllerId getControllerId () const {
            return controllerId_;
        }

        [[nodiscard]] bool writeDacInputRegister(uint16_t data) const;


        [[nodiscard]] DacPowerDownValues getDacPowerDownValues () const {return powerDownValue_;}
        void setDacPowerDownValues (const DacPowerDownValues value) {powerDownValue_ = value;}

    private:

        // Fast mode: byte 1: device addressing: 1 1 0 0 A2 A1 A0; then the two bytes below.
        // The address bits A2 A1 are defined at factory. Default is 0 0. Customer can have them programmed.
        // The last address bit A0 logic state of A0 pin.
        // Fast mode: bits are C2 C1 PD1 PD0 D11 D10 D09 D08 | D07 D06 D05 D04 D03 D02 D01 D00
        // Fast mode command is indicated by C2 and C1 both 0.
        // If we want fast mode, we need more declarations here.

        // To write the DAC input register, start with the address byte as above. Note that our struct will
        // always put 0 as the MSB. That's because the SDK will shift it left and insert Read/~Write bit.
        // So the address byte looks like 0x60 typically. Perhaps 0x61.
        // Write DAC register without EEPROM. If we want EEPROM programming, we need more declarations.
        // Write DAC register: C2 C1 C0 are 0 1 0.
        // The address byte struct is in dac-declarations.hpp.

        static constexpr uint8_t MCP4725_CMD_SINGLE_WRITE = 0b010;      // These belong in the top of the command byte

        struct MCP4725_Cmd_Ctl_Bits {
            uint8_t unused0:    1;
            uint8_t powerDown:  2;      // see DacPowerDownValues
            uint8_t unused1:    2;
            uint8_t command:    3;      // MCP4725_CMD_SINGLE_WRITE, etc.
        };

        union Mcp4725_Cmd_Ctl {
            MCP4725_Cmd_Ctl_Bits    bits;
            uint8_t                 byte;
        };

        using Mcp4725CmdCtl_t = Mcp4725_Cmd_Ctl;

        DacPowerDownValues powerDownValue_;
        ControllerId controllerId_;

    };
    
}
#endif

/**
 * @brief Write command control bits
 *
 *|------------------------------------------------------------------------------------------------------------
 *| C2 | C1 | C0 | Command Name       | Function
 *|------------------------------------------------------------------------------------------------------------
 *|  0 |  0 |  X | Fast Mode          | This command is used to change the DAC register. EEPROM is not affected
 *|------------------------------------------------------------------------------------------------------------
 *|  0 |  1 |  0 | Write DAC Register | Load configuration bits and data code to the DAC Register
 *|------------------------------------------------------------------------------------------------------------
 *|  0 |  1 |  1 | Write DAC Register | (a) Load configuration bits and data code to the DAC Register and
 *|    |    |    |    and EEPROM      | (b) also write the EEPROM
 *|------------------------------------------------------------------------------------------------------------
 *|  1 |  0 |  0 |      Reserved      | Reserved for future use
 *|------------------------------------------------------------------------------------------------------------
 *|  1 |  0 |  1 |      Reserved      | Reserved for future use
 *|------------------------------------------------------------------------------------------------------------
 *|  1 |  1 |  0 |      Reserved      | Reserved for future use
 *|------------------------------------------------------------------------------------------------------------
 *|  1 |  1 |  1 |      Reserved      | Reserved for future use
 *|------------------------------------------------------------------------------------------------------------
 */
