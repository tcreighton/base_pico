

#include <string>
#include <utility>

#include "hardware/i2c.h"
#include "csi2c.hpp"
#include "errors.hpp"
#include "logger.hpp"
#include "utilities.hpp"

namespace CSdrivers {


    CsI2C::CsI2C(const std::string &label,
                       i2c_inst_t* i2c,
                 const ControllerId controllerId,
                 const BaudRate baudRateInHz) :
                                                controllerId_(controllerId),
                                                i2cInstance_(i2c),
                                                requestedBaudRate_(baudRateInHz){
        setActualBaudRate(baudRateInHz);
    }


/**
 *
 * @param requestedBaudRate
 */

    void CsI2C::setRequestedBaudRate(const uint requestedBaudRate) {
        if (requestedBaudRate >= MIN_KHZ && requestedBaudRate <= MAX_KHZ) {
            setActualBaudRate(i2c_set_baudrate(i2cInstance_,
                                               requestedBaudRate_ = requestedBaudRate));
        }
    }

    void CsI2C::setActualBaudRate(const uint baudRate) {
        if (baudRate >= MIN_KHZ && baudRate <= MAX_KHZ) {
            actualBaudRate_ = i2c_set_baudrate(i2cInstance_,
                                               baudRate);

        }
    }

    int CsI2C::writeBuffer( const uint8_t deviceAddress,
                            const uint8_t *pBuffer,
                            const size_t length,
                            const bool nostop) {
        logger_.logMethodEntry( LogLevel::Trace,
                                getClassName(), __func__, getLabel());
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

        retValue = i2c_write_blocking(i2cInstance_,
                                        deviceAddress,
                                        pBuffer,
                                        length,
                                        nostop);

        if (std::cmp_not_equal(retValue ,length)) {
            setGlobalResponseCode(setResponseCode(picoErrorToF500Response(retValue)));
            setGlobalStatusCode(setStatus(StatusCode::ERROR));
            setGlobalResponseType(ResponseType::ERROR);
            setGlobalErrorMessage(setErrorMessage("I2C Error! \n"));

            logger_.log(LogLevel::Error, getLabel() + "\n\tFailed to write buffer to device.\n");
            logger_.log(LogLevel::Error,
                        "deviceAddress: " + int_to_hex_0x(deviceAddress) +
                        "; length: " + std::to_string(length) +
                        "; buffer: [0],[1],[2]: " +
                        int_to_hex_0x(*pBuffer) + "," +
                        int_to_hex_0x(*(pBuffer+1)) + "," +
                        int_to_hex_0x(*(pBuffer+2)) + "\n");
            logger_.log(LogLevel::Error, getLabel() + "\n\tretValue: " +
                                                     std::to_string(retValue) + "\n");
        }

        logger_.logMethodExit(LogLevel::Trace, getClassName(), __func__, getLabel());
        return retValue;
    }

    int CsI2C::readBuffer(const uint8_t deviceAddress, uint8_t *pBuffer, const size_t length, const bool nostop) {

        logger_.logMethodEntry(LogLevel::Trace, getClassName(), __func__, getLabel());

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
                i2c_read_blocking(i2cInstance_, deviceAddress, pBuffer, length, nostop);


        // Check for error on read.

        if (retValue < 0) {
            // This represents an error code.
            setGlobalResponseCode(setResponseCode(CSerrors::picoErrorToF500Response(retValue)));
            setGlobalStatusCode(setStatus(CSerrors::StatusCode::ERROR));
            setGlobalErrorMessage(setErrorMessage(setErrorMessage("I2C Error! \n")));
            setGlobalResponseType(ResponseType::ERROR);
            logger_.log(LogLevel::Error, getLabel() + " " + "Failed to read buffer from device.\n");
            logger_.log(LogLevel::Error, getLabel() + " " + "Error: " +
                                                     responseToString(getResponseCode()) + "\n");
        }

        logger_.logMethodExit(  LogLevel::Trace,
                                getClassName(),
                                __func__,
                                "retValue:" +
                                std::to_string(retValue));
        return retValue;
    }






//-----------------------------------------------------------------------------

} // namespace CScomponents
