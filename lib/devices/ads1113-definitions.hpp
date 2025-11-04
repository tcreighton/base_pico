
#pragma once
#ifndef ADS1113_DECLARATIONS_
#define ADS1113_DECLARATIONS_

#include <cstdint>
#include <string>

namespace CSdevices {

    enum class Ads111xDataRates {
        DR_8SPS     = 0b000,
        DR_16SPS    = 0b001,
        DR_32SPS    = 0b010,
        DR_64SPS    = 0b011,
        DR_128SPS   = 0b100,
        DR_250SPS   = 0b101,
        DR_475SPS   = 0b110,
        DR_860SPS   = 0b111,
        MAX_SPS = DR_860SPS
    };
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads111xDataRatesToNumber (Ads111xDataRates dataRate) {
        return static_cast<uint8_t>(dataRate);
    }
    constexpr Ads111xDataRates numberToAds111xDataRates (uint8_t number) {
        return static_cast<Ads111xDataRates>
        (number >= ads111xDataRatesToNumber(Ads111xDataRates::MAX_SPS) ?
         ads111xDataRatesToNumber(Ads111xDataRates::MAX_SPS) : number);
    }
    constexpr std::string ads1115DataRatesToString (const Ads111xDataRates dataRate) {
        std::string retVal;
        switch (dataRate) {
            case Ads111xDataRates::DR_860SPS:
                retVal = "DR_860SPS";
                break;
            case Ads111xDataRates::DR_475SPS:
                retVal = "DR_475SPS";
                break;
            case Ads111xDataRates::DR_250SPS:
                retVal = "DR_250SPS";
                break;
            case Ads111xDataRates::DR_128SPS:
                retVal = "DR_128SPS";
                break;
            case Ads111xDataRates::DR_64SPS:
                retVal = "DR_64SPS";
                break;
            case Ads111xDataRates::DR_32SPS:
                retVal = "DR_32SPS";
                break;
            case Ads111xDataRates::DR_16SPS:
                retVal = "DR_16SPS";
                break;
            case Ads111xDataRates::DR_8SPS:
                retVal = "DR_8SPS";
                break;
        }
        return retVal;
    }


    enum class Ads111xOperationalStatus {   // Read the config register and check the top bit - OS for AVAILABLE
        NO_EFFECT_OR_PERFORMING_CONVERSION, // Read: Currently performing conversion; Write: Nothing
        START_NEW_CONVERSION_OR_CONVERSION_COMPLETE,// Read: The conversion is complete. Write: starts a new conversion.
        OPERATIONAL_ERROR_STATE,               // Read: The device is in an error state. Write: Error state
        MAX_OP_STATUS = OPERATIONAL_ERROR_STATE
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115OperationalStatusToNumber (Ads111xOperationalStatus opStatus) {
        return static_cast<uint8_t>(opStatus);
    }
    constexpr Ads111xOperationalStatus numberToAds111xOperationalStatus (uint8_t number) {
        return static_cast<Ads111xOperationalStatus>
            (number >= ads1115OperationalStatusToNumber(Ads111xOperationalStatus::MAX_OP_STATUS) ?
             ads1115OperationalStatusToNumber(Ads111xOperationalStatus::MAX_OP_STATUS) : number);
    }
    constexpr std::string ads111xOperationalStatusToString (Ads111xOperationalStatus(opStatus)) {
        std::string retVal;

        switch (opStatus) {

            case Ads111xOperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION:
                retVal = "NO_EFFECT_OR_PERFORMING_CONVERSION";
                break;
            case Ads111xOperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE:
                retVal = "START_NEW_CONVERSION_OR_CONVERSION_COMPLETE";
                break;
            default: ;
        }
        return retVal;
    }
    enum class Ads111xOperatingMode : uint8_t {
        CONTINUOUS_CONVERSION = 0b0,    // Set this for continuous conversion mode
        SINGLE_SHOT,                    // Set this for single-shot or power-down state - default
        MAX_MODE = SINGLE_SHOT
    };
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads111xOperatingModeToNumber (Ads111xOperatingMode mode) {
        return static_cast<uint8_t>(mode);
    }
    constexpr Ads111xOperatingMode numberToAds111xOperatingMode (uint8_t number) {
        // If number is out of range, treat it as SINGLE_SHOT
        return static_cast<Ads111xOperatingMode>
            (number >= ads111xOperatingModeToNumber(Ads111xOperatingMode::MAX_MODE)  ?
             ads111xOperatingModeToNumber(Ads111xOperatingMode::MAX_MODE) : number);
    }
    constexpr std::string ads111xOperatingModeToString (const Ads111xOperatingMode mode) {
        std::string retVal;

        switch (mode) {
            case Ads111xOperatingMode::SINGLE_SHOT:
                retVal = "SINGLE_SHOT";
                break;
            case Ads111xOperatingMode::CONTINUOUS_CONVERSION:
                retVal = "CONTINUOUS_CONVERSION";
                break;
        }

        return retVal;
    }

    // Some possibly useful info, then the constants.
    // Given we operate at 860 SPS, then the time to convert is 1/860 = 0.0011627906976744186 or 1.16279 ms

    // Following constants need to be evaluated at compile time.
    // The basis is the (informed) assumption that the delay time for conversion at 860 sps is about 1.2ms.
    // We bump that to 1.5ms for buffer. Then calculate the other values based on this value.
    // So for 475 sps: delay = 1500 * (860/475). By doing the arithmetic myself, I can round it up and provide
    // a compile-time constant.

    // 1500 microseconds or 1.5 millisecsonds. Expected 1.2ms for 860 sps
    constexpr uint32_t BASE_CONVERSION_TIME_us = static_cast<uint32_t>(1500);
    // This is so it can be compile-time evaluated.
    constexpr uint32_t CONVERSION_TIME_860us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(1);
    constexpr uint32_t CONVERSION_TIME_475us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(2);
    constexpr uint32_t CONVERSION_TIME_250us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(3);
    constexpr uint32_t CONVERSION_TIME_128us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(7);// 1500 * 7 = 10500us
    constexpr uint32_t CONVERSION_TIME_064us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(13);
    constexpr uint32_t CONVERSION_TIME_032us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(27);
    constexpr uint32_t CONVERSION_TIME_016us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(54);
    // 1500 * 108 = 162000us = 162ms
    constexpr uint32_t CONVERSION_TIME_008us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(108);



    /* Register 0x01 (CONFIG) definition
    * |------------------------------------------------------------------------------------------------|
     * |   Bit 15  |   Bit 14  |   Bit 13  |   Bit 12  |   Bit 11  |   Bit 10  |   Bit 9   |   Bit 8   |
     * |-----------------------------------------------------------------------------------------------|
     * |     OS    |                                RESERVED                               |    MODE   |
     * |-----------------------------------------------------------------------------------------------|
     *
     * |-----------------------------------------------------------------------------------------------|
     * |   Bit 7   |   Bit 6   |   Bit 5   |   Bit 4   |   Bit 3   |   Bit 2   |   Bit 1   |   Bit 0   |
     * |-----------------------------------------------------------------------------------------------|
     * |              DR[2:0]              |                    RESERVED                               |
     * |-----------------------------------------------------------------------------------------------|
*/
    struct Ads1113ConfigLowByte {
        uint8_t reserved: 5;
        uint8_t dataRate: 3;
    };

    struct Ads1113ConfigHighByte {
        uint8_t operatingMode:  1;    // Device operating mode (MODE); see Ads111xOperatingMode
        uint8_t reserved:       3;    // unused in 1113
        uint8_t opStatus:       1;    // operational status (OS); see Ads111xOperationalStatus
    };

    struct Ads1113ConfigRegisterBytes {
        Ads1113ConfigLowByte    lowByte;
        Ads1113ConfigHighByte   highByte;
    };

    union Ads1113ConfigUnion {
        Ads1113ConfigRegisterBytes fields;
        uint16_t shortWord;
    };
    using Ads1113ConfigRegister_t = Ads1113ConfigUnion;

/**
 * These values determine which register we will read.
 * First send the address pointer register with one of these values.
 * Then read to get one of the four registers back.
 */
    enum class Ads1113RegisterAddresses : uint8_t {
        ADS1113_CONVERSION_REG_ADDR = 0b00,         // Applicable to 1113, 1114, 1115
        ADS1113_CONFIG_REG_ADDR     = 0b01,         // Applicable to 1113, 1114, 1115
        ADS1114_LO_THRESH_REG_ADDR  = 0b10,         // Applicable to       1114, 1115
        ADS1114_HI_THRESH_REG_ADDR  = 0b11,         // Applicable to       1114, 1115
        MAX_REG_ADDR = ADS1114_HI_THRESH_REG_ADDR
    };
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1113RegisterAddressesToNumber (Ads1113RegisterAddresses addr) {
        return static_cast<uint8_t>(addr);
    }
    constexpr Ads1113RegisterAddresses numberToAds1113RegisterAddresses (uint8_t number) {
        return static_cast<Ads1113RegisterAddresses>
        (number >= ads1113RegisterAddressesToNumber(Ads1113RegisterAddresses::MAX_REG_ADDR)  ?
         ads1113RegisterAddressesToNumber(Ads1113RegisterAddresses::MAX_REG_ADDR) : number);
    }

    // Don't really need these. Just use a uint8_t and assign a value from Ads1113RegisterAddresses.

    struct Ads1113ARegisterFields {
        uint8_t registerId  : 2;
        uint8_t reserved    : 6;
    };

    union Ads1113ARegisterUnion {
        Ads1113ARegisterFields fields;
        uint8_t byteValue;
    };


}

#endif
