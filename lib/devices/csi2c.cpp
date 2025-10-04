

#include <string>
#include <utility>
#include <hardware/i2c.h>

#include "csi2c.hpp"
#include "../core/logger.hpp"
#include "../core/utilities.hpp"

using namespace CScore;
using namespace CScore;

namespace CSdevices {

/**
 *
 * @param requestedBaudRate
 */


    int CsI2C::writeBuffer( const uint8_t deviceAddress,
                            const uint8_t *pBuffer,
                            const size_t length,
                            const bool nostop) {
        logger_.logMethodEntry(LogLevel::Trace,
                       std::string(getClassName()),
                       __func__);
        int retValue = 0;

#if defined (LOG_GROUP_CSI2C)

        logger_.log(LogLevel::Debug, getLabel() + " " +
                    __func__ +
                    "; I2C Bus Controller: " + (ControllerId::I2C_CONTROLLER_0 == getControllerId() ? "0." : "1.") +
                    ",\n\t deviceAddress: " +
                    int_to_hex_0x(deviceAddress) + ", " +
                    " buffer[0..2]: " +
                    int_to_hex_0x(pBuffer[0]) +
                    ", " + int_to_hex_0x(pBuffer[1]) +
                    ", " + int_to_hex_0x(pBuffer[2]) +
                    " length: " + std::to_string(length) +
                    " nostop: " + std::to_string(nostop) +
                    "\n");
#endif
        /*
        std::cout << "writeBuffer length: " << length << ". I2C Bus: Controller " <<
            (ControllerId::I2C_CONTROLLER_0 == getControllerId() ? "0." : "1.") << std::endl;
            */

        retValue = i2c_write_blocking(getI2cInstance(),
                                        deviceAddress,
                                        pBuffer,
                                        length,
                                        nostop);

        if (std::cmp_not_equal(retValue ,length)) {
            // Report the error somehow. TODO: Figure out the error handling.

            logger_.log(LogLevel::Error, std::string(getClassName()), __func__,
                                        " Failed to write buffer to device.\n");
            logger_.log(LogLevel::Error,
                                "deviceAddress: " + int_to_hex_0x(deviceAddress) +
                                "; length: " + std::to_string(length) +
                                "; buffer: [0],[1],[2]: " +
                                int_to_hex_0x(*pBuffer) + "," +
                                int_to_hex_0x(*(pBuffer+1)) + "," +
                                int_to_hex_0x(*(pBuffer+2)) + "\n");
            logger_.log(LogLevel::Error, "\n\tretValue: " +
                                                         std::to_string(retValue) + "\n");
        }

        logger_.logMethodExit(LogLevel::Trace, std::string(getClassName()),
                                                __func__);
        return retValue;
    }

    int CsI2C::readBuffer(const uint8_t deviceAddress, uint8_t *pBuffer, const size_t length, const bool nostop) {
        logger_.logMethodEntry(LogLevel::Trace, std::string(getClassName()), __func__);

#if defined (LOG_GROUP_CSI2C)

        logger_.log(LogLevel::Debug, getLabel() + " " +
                    __func__ +
                    "; deviceAddress: " +
                    int_to_hex_0x(deviceAddress) + ", " +
                    " buffer[0..2]: " +
                    int_to_hex_0x(pBuffer[0]) +
                    ", " + int_to_hex_0x(pBuffer[1]) +
                    ", " + int_to_hex_0x(pBuffer[2]) +
                    " length: " + std::to_string(length) +
                    " nostop: " + std::to_string(nostop) +
                    "\n");
#endif

        const auto retValue =
                i2c_read_blocking(getI2cInstance(), deviceAddress, pBuffer, length, nostop);


        // Check for error on read.

        if (retValue < 0) {
            // This represents an error code.
            // Report the error somehow. TODO: Figure out the error handling.

            logger_.log(LogLevel::Error, "Failed to read buffer from device.\n");

        }

        logger_.logMethodExit(  LogLevel::Trace,
                                std::string(getClassName()),
                                __func__,
                                "retValue:" +
                                std::to_string(retValue));
        return retValue;
    }






//-----------------------------------------------------------------------------

} // namespace CSdrivers
