

#include "ads1113-definitions.hpp"
#include "ads1113.hpp"

#include "ads111x.hpp"
#include "utilities.hpp"

namespace CSdevices {

    Ads1113ConfigRegister_t Ads1113::getNewConfigRegister() {
        const Ads1113ConfigRegister_t config;
        return config;
    }

    Ads1113ConfigRegister_t Ads1113::getNewConfigRegister(  const Ads111xOperationalStatus os,
                                                            const Ads111xOperatingMode mode) {
        auto config = Ads1113ConfigRegister_t();

        config.bytes.highByte.opStatus = ads111xOperationalStatusToNumber(os);
        config.bytes.highByte.operatingMode = ads111xOperatingModeToNumber(mode);
        return config;
    }

    bool Ads1113::startContinuousConversion() {

        const Ads1113ConfigRegister_t config = getNewConfigRegister(
                                        Ads111xOperationalStatus::START_CONVERSION_OR_CONVERSION_COMPLETE,
                                        Ads111xOperatingMode::CONTINUOUS_CONVERSION);
        const auto retVal = writeConfigRegister(config);
        if (retVal) {
            setConversionTimeout(make_timeout_time_us(Ads111xGetExpectedConversionTime_us(sampleRate_)));
        }

        return retVal;
    }

    bool Ads1113::isDataReady() {
        Ads1113ConfigRegister_t config{};
        auto retVal = false;

        if (time_reached(conversionTimeout_)) {
            retVal = readConfigRegister(config); // Temporarily use retVal to enable debug.
            retVal = (retVal && 0b01 == config.bytes.highByte.opStatus);
        }

        return retVal;
    }

    bool Ads1113::readConfigRegister(Ads1113ConfigRegister_t& configRegister) {
        uint16_t regValue = 0;
        const auto retVal = Ads111x::readConfigRegister(&regValue);
        if (retVal) {
            configRegister.shortWord = regValue;
        }
        return retVal;
    }

    bool Ads1113::writeConfigRegister(const Ads1113ConfigRegister_t configRegister) {
        const auto config = configRegister.shortWord;
        const auto retVal = Ads111x::writeConfigRegister(config); // Local variable for ease of debugging.

        return retVal;
    }
}
