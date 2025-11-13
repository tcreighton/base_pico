
#pragma once
#ifndef ADS1113_DEFINITIONS_
#define ADS1113_DEFINITIONS_

#include <cstdint>

#include "ads111x-definitions.hpp"

namespace CSdevices {

///
/// NOTE! All of this project assumes little-endian.
/// While some accomodations are made via utility functions for big-endian, The bit and byte layouts here
/// would need to be refactored to work in that mode.
///

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
     * |              SR[2:0]              |                    RESERVED                               |
     * |-----------------------------------------------------------------------------------------------|
*/
    struct Ads1113ConfigLowByte {
        Ads1113ConfigLowByte() = default;

        uint8_t reserved: 5 =   0;
        uint8_t sampleRate: 3 =   ads111xSampleRatesToNumber(Ads111xSampleRates::SR_860SPS);
    };

    struct Ads1113ConfigHighByte {
        Ads1113ConfigHighByte() = default;

        // Device operating mode (MODE); see Ads111xOperatingMode
        uint8_t operatingMode:  1 = ads111xOperatingModeToNumber(Ads111xOperatingMode::CONTINUOUS_CONVERSION);
        uint8_t reserved:       3 = 0;  // unused in 1113
        // operational status (OS); see Ads111xOperationalStatus
        uint8_t opStatus:       1 =
            ads111xOperationalStatusToNumber(Ads111xOperationalStatus::START_CONVERSION_OR_CONVERSION_COMPLETE);
    };

    struct Ads1113ConfigRegisterBytes {
        Ads1113ConfigRegisterBytes() = default;

        Ads1113ConfigLowByte    lowByte;
        Ads1113ConfigHighByte   highByte;
    };

    union Ads1113ConfigUnion {
        Ads1113ConfigUnion() : shortWord(0) {}

        Ads1113ConfigRegisterBytes bytes;
        uint16_t shortWord;
    };
    using Ads1113ConfigRegister_t = Ads1113ConfigUnion;



}

#endif
