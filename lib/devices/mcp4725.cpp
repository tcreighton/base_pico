
#include "mcp4725.hpp"

#include "utilities.hpp"

namespace CSdevices {
    bool Mcp4725::writeDacInputRegister(uint16_t data) const {
        auto retValue = false;
        uint8_t buffer[3] = {0, 0, 0};

        MCPAddressField_t deviceAddress{};
        deviceAddress.bits.deviceCode = DEVICE_CODE;
        deviceAddress.bits.i2cAddress = I2C_ADDRESS;

        Mcp4725CmdCtl_t cmdCtl{};
        cmdCtl.bits.command = MCP4725_CMD_SINGLE_WRITE;
        cmdCtl.bits.powerDown = static_cast<std::underlying_type_t<DacPowerDownValues>>(DacPowerDownValues::PD_OFF);

        buffer[0] = cmdCtl.byte;

        data = std::min(data, static_cast<uint16_t>(4095)); // 12-bit limit.
        data <<= 4; // The format of the data is 12 bits starting at the MSB of 16, with the LSB 3-0 are 0.
        CScore::localUint16ToNetworkByteOrder(data, &buffer[1]);
        const auto i2cReturn = CsI2C::writeBuffer(  getControllerId(),  // Calling the static method!
                                               deviceAddress.addressByte,
                                               buffer,
                                               sizeof(buffer));
        retValue = (sizeof(buffer) == i2cReturn);
        return retValue;
    }

}
