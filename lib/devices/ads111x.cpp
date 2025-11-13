
#include "ads111x.hpp"

#include "devicesContainer.hpp"
#include "utilities.hpp"

namespace CSdevices {

    bool Ads111x::setAndReadRegister(const Ads111xRegisterAddresses registerAddress, uint16_t * result) {
        auto retVal = false;

        if (writeAddressRegister(registerAddress)) {
            // The address register was written, now we can read what it pointed at.
            const auto bytesRead = getController(controllerId_).
                                        readBuffer(i2cAddress_, dataBuffer_, 2);
            if (2 == bytesRead) {
                *result = CScore::networkByteOrderToLocalUint16(dataBuffer_);
                retVal = true;
            }
        }
        return retVal;
    }

    bool Ads111x::writeAddressRegister(const Ads111xRegisterAddresses registerAddress) {
        auto retCode = false;
        dataBuffer_[0] = ads111xRegisterAddressesToNumber(registerAddress);
        const auto result = getController(controllerId_).writeBuffer(i2cAddress_, dataBuffer_, 1);
        retCode = (1 == result);
        return retCode;
    }

    bool Ads111x::writeConfigRegister(const uint16_t config) {

        dataBuffer_[0] = ads111xRegisterAddressesToNumber(Ads111xRegisterAddresses::ADS111X_CONFIG_REG_ADDR);
        CScore::localUint16ToNetworkByteOrder(config, &dataBuffer_[1]);
        constexpr auto bytesToWrite = 3;
        const auto bytesWritten = getController(controllerId_).writeBuffer(i2cAddress_, dataBuffer_, bytesToWrite);
        const bool retCode = (bytesWritten == bytesToWrite);

        return retCode;
    }

    bool Ads111x::readConversion(int16_t *value, const bool forceRead) {
        // Return the cached value if timing hasn't expired.
        if (!forceRead && !time_reached(earliestNextConversion_) && cachedValid_) {
            *value = cachedCounts_;
            return true;
        }

        // Read a new balue from the ADC.
        uint16_t rawValue;
        const auto retVal = readConversionRegister(&rawValue);

        if (retVal) {
            cachedCounts_ = static_cast<int16_t>(rawValue);
            *value = cachedCounts_;
            cachedValid_ = true;
            earliestNextConversion_ = make_timeout_time_us(Ads111xGetExpectedConversionTime_us(sampleRate_));
        }
        return retVal;
    }
}
