
#include "ads1115.hpp"
#include "ads1115-config.hpp"
#include "assertion.hpp"
#include "devicesContainer.hpp"
#include "logger.hpp"
#include "utilities.hpp"

using namespace CScore;

namespace CSdevices {

    bool Ads1115::writeConfigRegister(const Ads1115ConfigRegister_t configRegister) {

        /*logger_.log(LogLevel::Debug,
                    getClassName(),
                    std::string(__func__),
                    "config.getConfigRegister(): " +
                    int_to_hex_0x(configRegister.shortWord));*/

//        dataBuffer_[0] = dataBuffer_[1] = dataBuffer_[2] = 0;

        dataBuffer_[0] = ads1113RegisterAddressesToNumber(Ads1113RegisterAddresses::ADS1113_CONFIG_REG_ADDR);
        CScore::localUint16ToNetworkByteOrder(configRegister.shortWord, &dataBuffer_[1]);
        constexpr auto bytesToWrite = 3;

        const auto bytesWritten = getController().writeBuffer(i2cAddress_, dataBuffer_, bytesToWrite);

        const bool retCode = (bytesWritten == bytesToWrite);
        if (! retCode) {
            /*
            setStatus(CSerrors::StatusCode::ERROR);
            setResponseCode(CSerrors::picoErrorToF500Response(bytesWritten));
            setErrorMessage(std::string("Error writing config register."));

            logger_.log(LogLevel::Error,
                        getClassName(),
                        std::string(__func__),
                        "bytesWritten: " + int_to_hex_0x(bytesWritten));
        */
        }
        return retCode;
    }

    bool Ads1115::writeAddressRegister(const Ads1113RegisterAddresses reg) {
        auto retCode = false;
        dataBuffer_[0] = ads1113RegisterAddressesToNumber(reg);

        const auto result = getController().writeBuffer(getDeviceAddress(), dataBuffer_, 1);
/*
        if ((retCode = (1 == result))) { // The register address was set.
            setCurrentRegisterAddress(registerAddress);
        }
*/
        retCode = (1 == result);
        return retCode;
    }

    uint16_t Ads1115::setAndReadRegister(Ads1113RegisterAddresses registerAddress) {
        uint16_t value = 0;

        if (writeAddressRegister(registerAddress)) { // The register address was set.
            // The register write succeeded. Now read the register we just indicated.
            if (const auto result = getController().readBuffer(getDeviceAddress(), dataBuffer_, 2);
                2 == result) {
                value = networkByteOrderToLocalUint16(dataBuffer_);
            } else {
                logger_.log(LogLevel::Error,
                            getClassName(),
                            __func__,
                            "Error reading register. Register: " + registerAddressToName(registerAddress) +
                            " Result: " + int_to_hex_0x(result));
            }
        } else {
            logger_.log(LogLevel::Error,
                        getClassName(),
                        __func__,
                        "Error writing register address. Register: " + registerAddressToName(registerAddress));
        }

#if defined (LOG_GROUP_ADC)
        logger_.log(LogLevel::Debug,
                    getClassName(),
                    __func__,
                    "Register: " + registerAddressToName(registerAddress) +
                    "; Value after read: " + int_to_hex_0x(value) + "\n");
#endif

        return value;
    }

    Ads1115ConfigRegister_t Ads1115::buildConfigRegister() {

        Ads1115Config config;

        // These set values to the defaults as listed in the Datasheet.
        config.setOperationalStatus(Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE)
                .setMux(Ads1115Channel::AIN0_1_DIFFERENTIAL)
                .setPGA(AdsGainValues::GAIN_2p048V)
                .setOperatingMode(Ads111xOperatingMode::SINGLE_SHOT)
                .setDataRate(Ads111xDataRates::DR_860SPS)
                .setComparatorMode(Ads1115ComparatorMode::TRADITIONAL_COMPARATOR)
                .setComparatorPolarity(Ads1115ComparatorPolarity::ACTIVE_LOW)
                .setLatchingComparator(Ads1115LatchingComparator::NON_LATCHING)
                .setComparatorQueue(Ads1115ComparatorQueue::DISABLE_COMPARATOR);

        return config.getConfigRegister();
    }

    Ads1115ConfigRegister_t Ads1115::buildConfigRegister(const Ads1115Channel channel,
                                                         const Ads111xOperationalStatus opStatus) {

        const Ads1115ConfigRegister_t configRegister = buildConfigRegister(); // Get the defaults

        Ads1115Config config;

        config.setData(configRegister)
                .setMux(channel)
                .setOperationalStatus(opStatus);

        return config.getConfigRegister();
    }


    /**
     * @brief Returns an expected conversion time in microseconds.
     * This is based on estimates of 1.2 ms for a conversion where sample rate is 860sps.
     * @return expected conversion time in microseconds.
     */
    [[maybe_unused]] uint32_t Ads1115::getExpectedConversionTime_us() const {
        uint32_t delay = CONVERSION_TIME_860us;  // The baseline estimate for 860 SPS.

        switch (getDataRate()) {
            case Ads111xDataRates::DR_8SPS:     // 0b000
                delay = CONVERSION_TIME_008us;
                break;
            case Ads111xDataRates::DR_16SPS:    // 0b001
                delay = CONVERSION_TIME_016us;
                break;
            case Ads111xDataRates::DR_32SPS:    // 0b010
                delay = CONVERSION_TIME_032us;
                break;
            case Ads111xDataRates::DR_64SPS:    // 0b011
                delay = CONVERSION_TIME_064us;
                break;
            case Ads111xDataRates::DR_128SPS:   // 0b100
                delay = CONVERSION_TIME_128us;
                break;
            case Ads111xDataRates::DR_250SPS:   // 0b101
                delay = CONVERSION_TIME_250us;
                break;
            case Ads111xDataRates::DR_475SPS:   // 0b110
                delay = CONVERSION_TIME_475us;
                break;
            default:
            case Ads111xDataRates::DR_860SPS:   // 0b111
                delay = CONVERSION_TIME_860us;   // 1500 us or 1.5 ms
                break;
        }
        return delay;
    }


    Ads111xOperationalStatus Ads1115::checkConversionReady(const Ads1115Channel_t channel) {
        auto reg = readConfigRegister();
        Assertion::hardAssertTrue (Assertion::ON_FAILURE,
                                           reg.shortWord != 0x8583,
                                           getClassName(),
                                           __func__ ,
                                           __FILE__,
                                           __LINE__);

        const auto muxValue = getMuxValue(channel);
        constexpr auto testOpStatus =
                (ads1115OperationalStatusToNumber(
                        Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE));

        const auto correctChannel = (muxValue == reg.fields.mux);
        const auto opStatus = (testOpStatus == reg.fields.opStatus);
        Ads111xOperationalStatus retCode;

        if (!correctChannel) {
            /*
            setStatus(CSerrors::StatusCode::ERROR);
            setResponseCode(CSerrors::ResponseCode::F500_CONVERSION_ERROR);
            setErrorMessage(std::string("Wrong channel for conversion."));
            */
            retCode = Ads111xOperationalStatus::OPERATIONAL_ERROR_STATE;
            logger_.log(LogLevel::Error,
                        getClassName(),
                        __func__,
                        static_cast<std::string>("Wrong channel for conversion.\n\t") +
                        "Expected Channel: " + ads1115ChannelToString(channel) +
                        "; Channel read: ");
        /*
        } else if (CSerrors::StatusCode::READY != getStatus()) {
            retCode = Ads1115OperationalStatus::OPERATIONAL_ERROR_STATE;
        */
        } else if (!opStatus) { // Need to check back again.
            retCode = Ads111xOperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION;
        } else {    // Hurray! All is good.
            retCode = Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE;
            setConversionPendingState(retCode);
        }

        return retCode;
    }

    bool Ads1115::startConversion (const Ads1115ConfigRegister_t configRegister) {
        bool returnValue = false;

        // check that we are not in conversion
        if (Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE == isConversionPending()) {
            returnValue = writeConfigRegister(configRegister); // This starts the conversion.
            if (returnValue) {
                setConversionPendingState(Ads111xOperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION);
                setConversionTimeout();  // This sets the timeout time when we can look for the result.
            }
        } else {
            logger_.log(LogLevel::Error,
                        getClassName(),
                        __func__,
                        "Can't start conversion. Conversion is already pending.");
        }

        return returnValue;
    }


    std::string Ads1115::registerAddressToName (const Ads1113RegisterAddresses address) {

        std::string name;

        switch (address) {
            case Ads1113RegisterAddresses::ADS1113_CONVERSION_REG_ADDR:
                name = "ADS1113_CONVERSION_REG_ADDR";
                break;
            case Ads1113RegisterAddresses::ADS1113_CONFIG_REG_ADDR:
                name = "ADS1113_CONFIG_REG_ADDR";
                break;
            case Ads1113RegisterAddresses::ADS1114_LO_THRESH_REG_ADDR:
                name = "ADS1114_LO_THRESH_REG_ADDR";
                break;
            case Ads1113RegisterAddresses::ADS1114_HI_THRESH_REG_ADDR:
                name = "ADS1114_HI_THRESH_REG_ADDR";
                break;
        }

        return name;
    }

    CsI2C & Ads1115::getController() const {
        return CSdevices::getController(getControllerId());
    }


    //-------------------------------------------------------------------------------------------------------
// The following methods are the only public methods from Ads1115
//-------------------------------------------------------------------------------------------------------


    bool Ads1115::startConversion (const Ads1115Channel_t channel) {
        const Ads1115ConfigRegister_t configRegister =
                buildConfigRegister( channel,
                                     Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE);

        return startConversion(configRegister);
    }

    /**
     * @brief reads the conversion register
     * This will block on isConversionReady. It should always be ready when this is called, but if
     * it isn't, this is a precaution so we don't read an old conversion register.
     * For testing, this could be called almost immediately after startConversion.
     *
     * Note that the channel returns a signed integer. This level (Ads1115) returns the signed value.
     * That can be interpreted anyway that's needed by the caller (client) of Ads1115.
     *
     * @param channel
     * @return
     */
    int16_t Ads1115::completeConversion(const Ads1115Channel_t channel) {
#if defined (LOG_GROUP_ADC)
        logger_.logMethodEntry(LogLevel::Trace,
                               getClassName(),
                               std::string(__func__) ,
                               "Channel: " + ads1115ChannelToString(channel));
#endif

        int16_t retValue = 0;
        auto i = 2;
        auto pending = true;
        auto error = false; // checking getStatus is somewhat redundant with this, but I'm paranoid about missing it.

        do {
            waitOnConversionTimeout();
            --i;
            const Ads111xOperationalStatus check = checkConversionReady(channel);
            pending = (check == Ads111xOperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION);
            error = (check == Ads111xOperationalStatus::OPERATIONAL_ERROR_STATE);
        } while (i > 0 && /*(getStatus() != CSerrors::StatusCode::ERROR) &&*/ pending && ! error);

        if (! pending && ! error) {// must have completed.
            // get the actual count value!
            retValue = static_cast<int16_t>(readRegister(Ads1113RegisterAddresses::ADS1113_CONVERSION_REG_ADDR));
            // This may have set the status code to ERROR and return 0.
        }


        /*
        if (getStatus() != CSerrors::StatusCode::READY) {
#if defined (LOG_GROUP_ADC)

            logger_.log(LogLevel::Debug,
                        getClassName(),
                        std::string(__func__),
                        "Status: " + statusToLabel(getStatus()) +
                        " Error Code: " + responseToString(getResponseCode()) +
                        " Error Message: " + getErrorMessage());
#endif


        }
*/
#if defined (LOG_GROUP_ADC)

        logger_.logMethodExit(LogLevel::Trace,
                              getClassName(),
                              std::string(__func__) ,
                              "retValue: " + std::to_string(retValue));
#endif
        
        return retValue;
    }

    /*
    CSerrors::StatusCode Ads1115::init() {
        const auto retCode = Component::init();
#if defined LOG_GROUP_INIT

        logger_.log(LogLevel::Debug, getLabel() + " **LOG_GROUP_INIT**" +
                    __func__ + " Initializing: ");
#endif

        return retCode;
    }
    */


} // namespace CSdevices
