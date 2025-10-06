#pragma once

#ifndef CS_I2C_HPP_
#define CS_I2C_HPP_

#include <cstdint>
#include "hardware/i2c.h"
#include "../devices/component.hpp"

namespace CSdevices {

    // Baud rates
    enum class BaudRate : uint32_t {
        MIN_KHZ          = (100 * 1000),        // Define them in terms of HZ
        ONE_HUNDRED_KHZ  = MIN_KHZ,
        FOUR_HUNDRED_KHZ = (400 * 1000),
        ONE_THOUSAND_KHZ = (1000 * 1000),
        ONE_GHZ          = ONE_THOUSAND_KHZ,
        MAX_KHZ          = ONE_GHZ
    };

    enum class ControllerId : uint8_t {  // Controller - AKA Bus
        I2C_CONTROLLER_0 = 0,
        I2C_CONTROLLER_1
    };


    class CsI2C final : public Component {    // Creighton Scientific wrapper around much of I2C support in Pi Pico SDK

    public:

        explicit CsI2C (    const ControllerId controllerId,
                            const BaudRate baudrateInKHz = BaudRate::FOUR_HUNDRED_KHZ) :
                                controllerId_(controllerId),
                                requestedBaudRate_(baudrateInKHz) {
            setClassName("CsI2C");
            setLabel("I2C Controller");
            prependToHierarchy("CsI2C");
            setBaudRate(baudrateInKHz);
        }

        CsI2C (const std::string& label,
            const ControllerId controllerId,
            const BaudRate baudrateInKHz = BaudRate::FOUR_HUNDRED_KHZ) :
                controllerId_(controllerId),
                requestedBaudRate_(baudrateInKHz) {
            setClassName("CsI2C");
            setLabel(label);
            prependToHierarchy("CsI2C");
            setBaudRate(baudrateInKHz);
        }

        CsI2C (const CsI2C& other) = delete;    // No copy constructor!
        CsI2C& operator=(const CsI2C& other ) = delete;
        ~CsI2C() override = default;

        /**
         * @brief This allows a device user to check if the device is actually listening on the bus.
         * @param deviceAddress I2C address
         * @return
         */
        bool i2cDeviceReady (const uint8_t deviceAddress) {
            constexpr uint8_t buffer = 0; // This is dummy data.
            const int result = writeBuffer(deviceAddress, &buffer, 0);
            return (result >= 0);
        }

        void setBaudRate (const BaudRate requestedBaudRate) {
            requestedBaudRate_ = requestedBaudRate;
            actualBaudRate_ = i2c_set_baudrate(getI2cInstance(), static_cast<uint32_t>(requestedBaudRate_));
        }

        /**
         * @brief Gets requestedBaudRate_
         * @return requestedBaudRate_
         */
        [[nodiscard]] BaudRate getRequestedBaudRate() const {
            return requestedBaudRate_;
        }

        /**
         * @brief Gets actualBaudRate_
         * The system will only attempt to set communications rate at what is requested.
         * It returns the actual set rate that it chooses.
         * @return actualBaudRate_
         */
        [[nodiscard]] uint32_t getActualBaudRate() const {
            return actualBaudRate_;
        }

        /**
         * @brief Returns the timeout duration per byte for I2C communication.
         *        It is a generous timeout accommodating various I2C scenarios such as transaction times,
         *        clock stretching, and EEPROM write cycles, while maintaining a safety margin to catch
         *        genuine communication failures.
         * @return Timeout duration in microseconds per byte for I2C communication.
         */
        static uint32_t getI2CTimeoutPerByte_us() {
            // @ 100 kHz, 10us/bit; 8 bits data + 1 ACK: 90us. (maybe use 100)
            // @ 400 kHz: 90us/4 = 22us.
            // @ 1000 kHz: 90us/100 = 1us.

            /* This code could be used if we want fine-grained control over time per byte - not worth it.
            constexpr uint baseTimePerByte_us = 100;

            uint timeout = 100000;

            switch (getRequestedBaudRate()) {
                case BaudRate::MIN_KHZ:
                    timeout = baseTimePerByte_us / 1;
                    break;
                default:
                case BaudRate::FOUR_HUNDRED_KHZ:
                    timeout = baseTimePerByte_us / 4;
                    break;
                case BaudRate::ONE_THOUSAND_KHZ:
                    timeout = baseTimePerByte_us / 100;
                    break;
            }
            return timeout;
        */

            // Generous 100 ms timeout handles most I2C scenarios:
            // - Typical transactions complete in < 1 ms
            // - Clock stretching for sensor conversions: 1-50 ms typical
            // - EEPROM write cycles: 5-ert10 ms
            // This provides a comfortable safety margin for edge cases
            // while still catching genuine communication failures
            return 100000;  // 100 ms
        }

        /**
         * @brief Gets the controllerId for this CsI2C.
         * @return
         */
        [[nodiscard]] ControllerId getControllerId() const {
            return controllerId_;
        }

        /**
             * @brief Gets the i2c instance pointer for this CsI2C.
             * @return i2c_inst_t* pointer to the SDK i2c instance
             */
        [[nodiscard]] i2c_inst_t* getI2cInstance() const {
            return ControllerId::I2C_CONTROLLER_0 == controllerId_ ? i2c0 : i2c1;
        }



        /**
         * @brief - Writes length bytes starting at *pBuffer.
         * @param deviceAddress
         * @param pBuffer: source of the data to write
         * @param length
         * @param nostop
         * @return bytes written
         * @return [error] PICO_ERROR_GENERIC on some error
         * @return [error] PICO_ERROR_TIMEOUT on timeout of operation
         */
        int writeBuffer(uint8_t deviceAddress, const uint8_t *pBuffer, size_t length, bool nostop);

        int writeBuffer(const uint8_t deviceAddress, const uint8_t *pBuffer, const size_t length) {
            return writeBuffer(deviceAddress, pBuffer, length, false);
        }

        /**
         * @brief Reads from i2c bus. Blocks on read or until time.
         *
         * @param deviceAddress I2C device address
         * @param pBuffer       Pointer to the data area
         * @param length        Number of bytes to read
         * @param nostop        If true, master retains control of the bus at the end of the transfer
         * @return The number of bytes read or an error code if there was a read error.
         */
        int readBuffer(uint8_t deviceAddress, uint8_t *pBuffer, size_t length, bool nostop);

        int readBuffer(const uint8_t deviceAddress, uint8_t *pBuffer, const size_t length) {
            return readBuffer(deviceAddress, pBuffer, length, false);
        }


    private:

        /**
         * @brief Sets the controllerId_.
         * @param controllerId
         */
        void setControllerId(const ControllerId controllerId) {
            controllerId_ = controllerId;
        }

        ControllerId controllerId_;
        BaudRate requestedBaudRate_ = BaudRate::FOUR_HUNDRED_KHZ; // this is always in kHz. Actual is always in Hz.
        uint32_t actualBaudRate_ = 0;    // This gets set to the return from a call to i2c_init within a constructor.
    };

//-----------------------------------------------------------------------------

}   // namespace CSdrivers

#endif
