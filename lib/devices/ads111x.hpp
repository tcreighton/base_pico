#pragma once
#ifndef ADS111X_HPP_
#define ADS111X_HPP_

#include "ads111x-definitions.hpp"
#include "component.hpp"
#include "csi2c.hpp"

namespace CSdevices {

    class Ads111x : public Component {
    public:

        Ads111x (   const ControllerId          controllerId,
                    const uint8_t               i2cAddress,
                    const Ads111xSampleRates    sampleRate = Ads111xSampleRates::SR_860SPS) :
                    controllerId_(controllerId), i2cAddress_(i2cAddress), sampleRate_(sampleRate) {}
        ~Ads111x () override = default;

        [[nodiscard]] ControllerId getControllerId () const {return controllerId_;}
        [[nodiscard]] uint8_t getI2CAddress () const {return i2cAddress_;}
        uint8_t* getDataBufferPointer () {return dataBuffer_;}

        static uint32_t Ads111xGetExpectedConversionTime_us (const Ads111xSampleRates rate) {
            uint32_t delay;

            switch (rate) {
                case Ads111xSampleRates::SR_8SPS:       // 0b000
                    delay = CONVERSION_TIME_008us;
                    break;
                case Ads111xSampleRates::SR_16SPS:      // 0b001
                    delay = CONVERSION_TIME_016us;
                    break;
                case Ads111xSampleRates::SR_32SPS:      // 0b010
                    delay = CONVERSION_TIME_032us;
                    break;
                case Ads111xSampleRates::SR_64SPS:      // 0b011
                    delay = CONVERSION_TIME_064us;
                    break;
                case Ads111xSampleRates::SR_128SPS:     // 0b100
                    delay = CONVERSION_TIME_128us;
                    break;
                case Ads111xSampleRates::SR_250SPS:     // 0b101
                    delay = CONVERSION_TIME_250us;
                    break;
                case Ads111xSampleRates::SR_475SPS:     // 0b110
                    delay = CONVERSION_TIME_475us;      // delay = 1500 * (860/475)
                    break;
                default:
                case Ads111xSampleRates::SR_860SPS:     // 0b111
                    delay = CONVERSION_TIME_860us;      // 1500 us or 1.5 ms
                    break;
            }

            return delay;
        }

        /**
         * @brief Writes the register address. Then reads the register data.
         * @param registerAddress
         * @param result is where the result of the read goes. The result is either the register requested or an error.
         * @return true if the read was successful.
         */
        // ReSharper disable once CppParameterNamesMismatch
        bool setAndReadRegister (Ads111xRegisterAddresses registerAddress, uint16_t * result);

        /**
         * @brief Reads the config register.
         * @return The read config register. Sets status to NO_ERROR if all is well.
         */
        //bool readConfigRegister (Ads111xConfigRegister_t * reg) {
        bool readConfigRegister (uint16_t * reg) {

            const auto retVal = setAndReadRegister(Ads111xRegisterAddresses::ADS111X_CONFIG_REG_ADDR,
                                                        reg);

            return retVal;
        }

        /**
         * @brief Reads the conversion register - the current counts from the ADC.
         * @return the read conversion register.
         */
        bool readConversionRegister (uint16_t * conversion) {

            const auto retVal = setAndReadRegister(Ads111xRegisterAddresses::ADS111X_CONVERSION_REG_ADDR,
                                                        conversion);

            return retVal;
        }

        /**
         * @brief Writes the address pointer register.
         * The value written indicates what kind of register is next to be read or written.
         * See Ads1115RegisterAddresses for the bit values.
         * @param registerAddress The enum type of address
         * @return true on success. false on any error.
         */
        bool writeAddressRegister(Ads111xRegisterAddresses registerAddress);

        /**
         * @brief Writes the configuration register to the device.
         * @param config
         * @return
         */
        bool writeConfigRegister (uint16_t config);

        bool readConversion(int16_t* value, bool forceRead = false);
        bool readConversionNow(int16_t* value) { return readConversion(value, true); }


    protected:

        ControllerId        controllerId_;              // Controller id everywhere until just before use.
        uint8_t             i2cAddress_;                // This is the 7-bit i2c address for the device
        uint8_t             dataBuffer_ [3] = {0,0,0};  // Having this as an instance variable reduces stack impact.
        int16_t             cachedCounts_ = 0;          // This caches a record of the last value read from converison.
        bool                cachedValid_ = false;       // Indicates whether lastCountsRead_ is valid.
        absolute_time_t     earliestNextConversion_ = get_absolute_time();
        Ads111xSampleRates  sampleRate_;
    };





}

#endif
