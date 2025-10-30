
#include <iostream>

#include "dac-channel.hpp"
#include "dac-declarations.hpp"
#include "logger.hpp"
#include "mcp4728.hpp"
#include "utilities.hpp"

namespace CSdevices {

/**
 * @brief - This is the concrete method for writing to an input register on MCP4728
 * @param dacChannel
 * @param data
 * @return true if no errors.
 */
    bool Mcp4728::writeDacInputRegister(const DacChannel& dacChannel, uint16_t data) const {

#if defined (LOG_GROUP_DAC)
        logger_.logMethodEntry(LogLevel::Trace,
                              getClassName(),
                              std::string(__func__),
                              getLabel() + "\n");
#endif

        auto retCode = false;


        // Now we fill the communications buffer.
        // buffer[0] holds the i2c command and the channel number
        // buffer[1].high holds the command, the vref, the power down mode, and the gain.
        // buffer[1].low holds the high-order nibble of the data.
        // buffer[2] holds the middle and low-order nibbles of the data.

        // This is what we have to be able to call:
        //i2c_write_timeout_us(i2c, addr, pBuf, len, false, len * I2C_TIMEOUT_CHAR);;

        MCPAddressField_t addressField;
        uint8_t buffer[3] = {0, 0, 0};

        addressField.addressByte = 0;   // Set the word to all 0.
        addressField.bits.deviceCode = DEVICE_CODE;
        addressField.bits.i2cAddress = I2C_ADDRESS;
        addressField.bits.unused = 0;
//        addressField.bits.readWrite = WRITE_BIT;  // This is added in by the sdk!

        Mcp4728CommandWord_t cmd{0};
        cmd.byte = 0;   // initialize (superfluous)
        cmd.bits.command = MCP4728_CMD_MULTI_WRITE;
        cmd.bits.channel = static_cast<uint8_t>(dacChannel.getChannelId());
        cmd.bits.udac = 0;   // redundant with initialize line.

        buffer[0] = cmd.byte;   // Set command byte

        Mcp4728Controls_t controlByte;
        controlByte.byte = 0;
        controlByte.bits.dataNibble = 0;  // This is the most significant nibble of the data word.
        controlByte.bits.gain = static_cast<uint8_t>(dacChannel.getGain());
        controlByte.bits.powerDown = static_cast<uint8_t>(dacChannel.getPowerMode());
        controlByte.bits.vref = static_cast<uint8_t>(dacChannel.getVref());

#if defined (LOG_GROUP_TEN)
        if (data >= 4095) {
            std::cout << "Error: data is too large for MCP4728: " << data << std::endl;
        }
#endif

        data = std::min(data, static_cast<uint16_t>(4095)); // 12-bit limit.
        CScore::localUint16ToNetworkByteOrder(data, &buffer[1]);    // Now the data is in the lower nibble of [1] && buffer[2]
        buffer[1] |= (controlByte.byte & 0xf0);   // Set control byte to live in upper nibble.
        // This leaves the lower nibble untouched.


#if defined (LOG_GROUP_DAC)

        logger_.log(LogLevel::Info, getLabel() + "\n\t\t" +
                    "addField.addressByte: " + int_to_hex_0x(addressField.addressByte) + ",\n\t\t\t" +
                    "addField.bits.unused: " + int_to_hex_0x(addressField.bits.unused) + ",\n\t\t\t"
                    "addField.bits.deviceCode: " + int_to_hex_0x(addressField.bits.deviceCode) + ",\n\t\t\t" +
                    "addField.bits.i2cAddress: " + int_to_hex_0x(addressField.bits.i2cAddress));

        logger_.log(LogLevel::Info, getLabel() + "\n\t\t" +
                    "cmd.byte: " + int_to_hex_0x(cmd.byte) + ",\n\t\t\t"
                    "cmd.bits.command: " + int_to_hex_0x(cmd.bits.command) + ",\n\t\t\t" +
                    "cmd.bits.channel: " + int_to_hex_0x(cmd.bits.channel) + ",\n\t\t\t" +
                    "cmd.bits.udac: " + int_to_hex_0x(cmd.bits.udac));

        logger_.log(LogLevel::Info, getLabel() + "\n\t\t" +
                    "ctlByte.byte: " + int_to_hex_0x(controlByte.byte) + ",\n\t\t\t" +
                    "ctlByte.bits.vref: " + int_to_hex_0x(controlByte.bits.vref) + ",\n\t\t\t" +
                    "ctlByte.bits.powerDown: " + int_to_hex_0x(controlByte.bits.powerDown) + ",\n\t\t\t" +
                    "ctlByte.bits.gain: " + int_to_hex_0x(controlByte.bits.gain) + ",\n\t\t\t" +
                    "ctlByte.bits.dataNibble: "+ int_to_hex_0x(controlByte.bits.dataNibble));

        logger_.log(LogLevel::Info, getLabel() + "\n\t\t" +
                    "writing 12 bits of data:" + int_to_hex_0x(data) + " to the Dac Input Register");

        logger_.log(LogLevel::Info, getLabel() + "\n\t\t" +
                    "i2c_write; addressByte: " +
                    int_to_hex_0x(addressField.addressByte) +
                    " buffer[0..2]: " +
                    int_to_hex_0x(buffer[0]) +
                    ", " + int_to_hex_0x(buffer[1]) +
                    ", " + int_to_hex_0x(buffer[2]));

#endif
        const auto i2cReturn = getController().writeBuffer(addressField.addressByte, buffer, sizeof(buffer));
        retCode = (sizeof(buffer) == i2cReturn);
        if (!retCode) {
            // Error reporting happened already.
            /*
            setGlobalResponseCode(setResponseCode(CSerrors::picoErrorToF500Response(i2cReturn)));
            CSerrors::setGlobalErrorMessage(setErrorMessage("I2C Error on i2c_write_timeout_per_char_us! Code: " +
                                                            responseToString(getResponseCode()) + "\n"));
            setGlobalStatusCode(setStatus(CSerrors::StatusCode::ERROR));
            CScore::logger_.log(CScore::LogLevel::Error, getLabel() + " " + getErrorMessage());
            */

        }
#if defined (LOG_GROUP_DAC)

        logger_.logMethodExit(LogLevel::Trace,
                              getClassName(),
                              std::string(__func__),
                              getLabel());
#endif

        return retCode;
    }



} // namespace CSconverters


