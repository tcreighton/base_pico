#pragma once
#ifndef ADS111X_DEFS_HPP_
#define ADS111X_DEFS_HPP_

#include <cstdint>
#include <string>

namespace CSdevices {

    enum class Ads111xSampleRates {
        SR_8SPS     = 0b000,
        SR_16SPS    = 0b001,
        SR_32SPS    = 0b010,
        SR_64SPS    = 0b011,
        SR_128SPS   = 0b100,
        SR_250SPS   = 0b101,
        SR_475SPS   = 0b110,
        SR_860SPS   = 0b111,
        MAX_SPS = SR_860SPS
    };
    // ReSharper disable once CppDFAUnreachableFunctionCall
    // ReSharper disable once CppDFAConstantParameter
    // ReSharper disable once CppDFAConstantFunctionResult
    constexpr uint8_t ads111xSampleRatesToNumber (Ads111xSampleRates sampleRate) {
        return static_cast<uint8_t>(sampleRate);
    }
    constexpr Ads111xSampleRates numberToAds111xSampleRates (uint8_t number) {
        return static_cast<Ads111xSampleRates>
        (number >= ads111xSampleRatesToNumber(Ads111xSampleRates::MAX_SPS) ?
         ads111xSampleRatesToNumber(Ads111xSampleRates::MAX_SPS) : number);
    }
    constexpr std::string ads111xSampleRatesToString (const Ads111xSampleRates sampleRate) {
        std::string retVal;
        switch (sampleRate) {
            case Ads111xSampleRates::SR_860SPS:
                retVal = "SR_860SPS";
                break;
            case Ads111xSampleRates::SR_475SPS:
                retVal = "SR_475SPS";
                break;
            case Ads111xSampleRates::SR_250SPS:
                retVal = "SR_250SPS";
                break;
            case Ads111xSampleRates::SR_128SPS:
                retVal = "SR_128SPS";
                break;
            case Ads111xSampleRates::SR_64SPS:
                retVal = "SR_64SPS";
                break;
            case Ads111xSampleRates::SR_32SPS:
                retVal = "SR_32SPS";
                break;
            case Ads111xSampleRates::SR_16SPS:
                retVal = "SR_16SPS";
                break;
            case Ads111xSampleRates::SR_8SPS:
                retVal = "SR_8SPS";
                break;
        }
        return retVal;
    }


    enum class Ads111xOperationalStatus {   // Read the config register and check the top bit - OS for AVAILABLE
        NO_EFFECT_OR_PERFORMING_CONVERSION, // Read: Currently performing conversion; Write: Nothing
        START_CONVERSION_OR_CONVERSION_COMPLETE,// Read: The conversion is complete. Write: starts a new conversion.
        OPERATIONAL_ERROR_STATE,               // Read: The device is in an error state. Write: Error state
        MAX_OP_STATUS = OPERATIONAL_ERROR_STATE
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    // ReSharper disable once CppDFAConstantFunctionResult
    // ReSharper disable once CppDFAConstantParameter
    constexpr uint8_t ads111xOperationalStatusToNumber (Ads111xOperationalStatus opStatus) {
        return static_cast<uint8_t>(opStatus);
    }
    constexpr Ads111xOperationalStatus numberToAds111xOperationalStatus (uint8_t number) {
        return static_cast<Ads111xOperationalStatus>
            (number >= ads111xOperationalStatusToNumber(Ads111xOperationalStatus::MAX_OP_STATUS) ?
             ads111xOperationalStatusToNumber(Ads111xOperationalStatus::MAX_OP_STATUS) : number);
    }
    constexpr std::string ads111xOperationalStatusToString (Ads111xOperationalStatus(opStatus)) {
        std::string retVal;

        switch (opStatus) {

            case Ads111xOperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION:
                retVal = "NO_EFFECT_OR_PERFORMING_CONVERSION";
                break;
            case Ads111xOperationalStatus::START_CONVERSION_OR_CONVERSION_COMPLETE:
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
    // ReSharper disable once CppDFAConstantFunctionResult
    // ReSharper disable once CppDFAConstantParameter
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
    //
    // Conversion time baseline for 860 SPS
    // Theoretical minimum: 1163μs (1/860)
    // Chosen value: 1500μs (~29% margin)
    //
    // Rationale for 29% safety margin:
    // - Accounts for I2C transaction overhead (~200-400μs)
    // - Protects against timing jitter and clock inaccuracy
    // - Cost of reading too early: 2x I2C transactions (expensive!)
    // - Cost of 0.34ms extra delay: negligible in practice
    //
    // 1500 microseconds or 1.5 milliseconds. Expected 1.2ms for 860 sps
    constexpr uint32_t BASE_CONVERSION_TIME_us = static_cast<uint32_t>(1500);
    // This is so it can be compile-time evaluated.
    constexpr uint32_t CONVERSION_TIME_860us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(1); // 860/860
    constexpr uint32_t CONVERSION_TIME_475us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(2); // 860/475
    constexpr uint32_t CONVERSION_TIME_250us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(3); // 860/250
    constexpr uint32_t CONVERSION_TIME_128us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(7); // 860/128
    constexpr uint32_t CONVERSION_TIME_064us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(13);// 860/64
    constexpr uint32_t CONVERSION_TIME_032us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(27);// 860/32
    constexpr uint32_t CONVERSION_TIME_016us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(54);// 860/16
    // 1500 * 108 = 162000us = 162ms
    constexpr uint32_t CONVERSION_TIME_008us = BASE_CONVERSION_TIME_us * static_cast<uint32_t>(108);// 860/8

    /**
     * These values determine which register we will read.
     * First send the address pointer register with one of these values.
     * Then read to get one of the four registers back.
     */
    enum class Ads111xRegisterAddresses : uint8_t {
        ADS111X_CONVERSION_REG_ADDR = 0b00,         // Applicable to 1113, 1114, 1115
        ADS111X_CONFIG_REG_ADDR     = 0b01,         // Applicable to 1113, 1114, 1115
        ADS111X_LO_THRESH_REG_ADDR  = 0b10,         // Applicable to       1114, 1115
        ADS111X_HI_THRESH_REG_ADDR  = 0b11,         // Applicable to       1114, 1115
        MAX_REG_ADDR = ADS111X_HI_THRESH_REG_ADDR
    };
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads111xRegisterAddressesToNumber (Ads111xRegisterAddresses addr) {
        return static_cast<uint8_t>(addr);
    }
    constexpr Ads111xRegisterAddresses numberToAds111xRegisterAddresses (uint8_t number) {
        return static_cast<Ads111xRegisterAddresses>
        (number >= ads111xRegisterAddressesToNumber(Ads111xRegisterAddresses::MAX_REG_ADDR)  ?
         ads111xRegisterAddressesToNumber(Ads111xRegisterAddresses::MAX_REG_ADDR) : number);
    }

    // Don't really need these. Just use a uint8_t and assign a value from Ads111xRegisterAddresses.

    struct Ads111xARegisterFields {
        uint8_t registerId  : 2;
        uint8_t reserved    : 6;
    };

    union Ads111xARegisterUnion {
        Ads111xARegisterFields fields;
        uint8_t byteValue;
    };

    struct Ads111xRegBits {
        uint16_t bit0   : 1;
        uint16_t bit1   : 1;
        uint16_t bit2   : 1;
        uint16_t bit3   : 1;
        uint16_t bit4   : 1;
        uint16_t bit5   : 1;
        uint16_t bit6   : 1;
        uint16_t bit7   : 1;
        uint16_t bit8   : 1;
        uint16_t bit9   : 1;
        uint16_t bit10  : 1;
        uint16_t bit11  : 1;
        uint16_t bit12  : 1;
        uint16_t bit13  : 1;
        uint16_t bit14  : 1;
        uint16_t bit15  : 1;
    };

    union Ads111xRegUnion {
        Ads111xRegBits bits;
        uint16_t shortWord;
    };

    // The following type definitions are to make it easier to deal with all the register types the same way.
    using Ads111xConfigRegister_t       = Ads111xRegUnion;
    using Ads111xConversionRegister_t   = Ads111xRegUnion;
    using Ads111xLoThreshold_t          = Ads111xRegUnion;
    using Ads111xHiThreshold_t          = Ads111xRegUnion;

}


#endif
