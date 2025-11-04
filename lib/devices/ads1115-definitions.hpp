#pragma once

#ifndef ADS1115_DEFINITIONS_
#define ADS1115_DEFINITIONS_

#include <cstdint>
#include <string>

#include "ads1114-definitions.hpp"

namespace CSdevices {

    //AKA Mux
    enum class Ads1115Channel : uint8_t {     // Four channels available per ADC; AKA Mux
        AIN0_1_DIFFERENTIAL = 0b000,// AIN0 and AIN1
        AIN0_3_DIFFERENTIAL,        // AIN0 and AIN3
        AIN1_3_DIFFERENTIAL,        // AIN1 and AIN3
        AIN2_3_DIFFERENTIAL,        // AIN2 and AIN3
        AIN0_SINGLE_SHOT = 0b100,   // AIN0 - value 100; assignment is redundant, I know
        AIN1_SINGLE_SHOT,           // AIN1 - value 101
        AIN2_SINGLE_SHOT,           // AIN2 - value 110
        AIN3_SINGLE_SHOT,           // AIN3 - value 111
        MAX_CHANNEL = AIN3_SINGLE_SHOT
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115ChannelToNumber (Ads1115Channel channel) {
        return static_cast<uint8_t>(channel);
    }
    constexpr Ads1115Channel numberToAds1115Channel (uint8_t number) {
        return static_cast<Ads1115Channel>
        (number >= ads1115ChannelToNumber(Ads1115Channel::MAX_CHANNEL) ?
         ads1115ChannelToNumber(Ads1115Channel::MAX_CHANNEL) : number);
    }
    constexpr std::string ads1115ChannelToString (const Ads1115Channel channel) {
        std::string retVal;

        switch (channel) {
            case Ads1115Channel::AIN0_1_DIFFERENTIAL:
                retVal = "AIN0_1_DIFFERENTIAL";
                break;
            case Ads1115Channel::AIN0_3_DIFFERENTIAL:
                retVal = "AIN0_3_DIFFERENTIAL";
                break;
            case Ads1115Channel::AIN1_3_DIFFERENTIAL:
                retVal = "AIN1_3_DIFFERENTIAL";
                break;
            case Ads1115Channel::AIN2_3_DIFFERENTIAL:
                retVal = "AIN2_3_DIFFERENTIAL";
                break;
            case Ads1115Channel::AIN0_SINGLE_SHOT:
                retVal = "AIN0_SINGLE_SHOT";
                break;
            case Ads1115Channel::AIN1_SINGLE_SHOT:
                retVal = "AIN1_SINGLE_SHOT";
                break;
            case Ads1115Channel::AIN2_SINGLE_SHOT:
                retVal = "AIN2_SINGLE_SHOT";
                break;
            case Ads1115Channel::AIN3_SINGLE_SHOT:
                retVal = "AIN3_SINGLE_SHOT";
                break;

        }

        return retVal;
    }

    using Ads1115Channel_t = Ads1115Channel;


    /* Register 0x01 (CONFIG) definition
     * |-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |   Bit 15  |   Bit 14  |   Bit 13  |   Bit 12  |   Bit 11  |   Bit 10  |   Bit 9   |   Bit 8   |   Bit 7   |   Bit 6   |   Bit 5   |   Bit 4   |   Bit 3   |   Bit 2   |   Bit 1   |   Bit 0   |
     * |-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     OS    |              MUX[2:0]             |              PGA[2:0]             |    MODE   |              DR[2:0]              | COMP_MODE |  COMP_POL |  COMP_LAT |     COMP_QUE[1:0]     |
     * |-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     */

    enum class Ads1115ComparatorMode : uint8_t {
        TRADITIONAL_COMPARATOR  = 0b0,     // Default
        WINDOW_COMPARATOR       = 0b1,
        MAX_MODE = WINDOW_COMPARATOR
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115ComparatorModeToNumber (Ads1115ComparatorMode compMode) {
        return static_cast<uint8_t>(compMode);
    }

    constexpr Ads1115ComparatorMode numberToAds1115ComparatorMode (uint8_t number) {
        return static_cast<Ads1115ComparatorMode>
        (number >= ads1115ComparatorModeToNumber(Ads1115ComparatorMode::MAX_MODE) ?
            ads1115ComparatorModeToNumber(Ads1115ComparatorMode::MAX_MODE) : number);
    }

    enum class Ads1115ComparatorPolarity : uint8_t {
        ACTIVE_LOW  = 0b0,      // Default
        ACTIVE_HIGH = 0b1,
        MAX_POLARITY = ACTIVE_HIGH
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115ComparatorPolarityToNumber (Ads1115ComparatorPolarity compPolarity) {
        return static_cast<uint8_t>(compPolarity);
    }

    constexpr Ads1115ComparatorPolarity NumberToAds1115ComparatorPolarity (uint8_t number) {
        return static_cast<Ads1115ComparatorPolarity>
        (number >= ads1115ComparatorPolarityToNumber(Ads1115ComparatorPolarity::MAX_POLARITY) ?
         ads1115ComparatorPolarityToNumber(Ads1115ComparatorPolarity::MAX_POLARITY) : number);
    }

    enum class Ads1115LatchingComparator : uint8_t {
        NON_LATCHING    = 0b0,  // default
        LATCHING        = 0b1,
        MAX_LATCHING    = LATCHING
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115LatchingComparatorToNumber (Ads1115LatchingComparator latchingComp) {
        return static_cast<uint8_t>(latchingComp);
    }

    constexpr Ads1115LatchingComparator numberToAds1115LatchingComparator (uint8_t number) {
        return static_cast<Ads1115LatchingComparator>
        (number >= ads1115LatchingComparatorToNumber(Ads1115LatchingComparator::MAX_LATCHING) ?
         ads1115LatchingComparatorToNumber(Ads1115LatchingComparator::MAX_LATCHING) : number);
    }

    enum class Ads1115ComparatorQueue : uint8_t {
        ASSERT_AFTER_ONE    = 0b00,         // Assert the ALERT/RDY pin after one conversion
        ASSERT_AFTER_TWO    = 0b01,         // Assert the ALERT/RDY pin after two conversions
        ASSERT_AFTER_FOUR   = 0b10,         // Assert the ALERT/RDY pin after four conversions
        DISABLE_COMPARATOR  = 0b11,         // Disable the comparator and assert ALERT/RDY pin (default)
        MAX_COMP_QUEUE      = DISABLE_COMPARATOR
    };

    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t ads1115ComparatorQueueToNumber (Ads1115ComparatorQueue compQueue) {
        return static_cast<uint8_t>(compQueue);
    }

    constexpr Ads1115ComparatorQueue numberToAds1115ComparatorQueue (uint8_t number) {
        return static_cast<Ads1115ComparatorQueue>
        (number >= ads1115ComparatorQueueToNumber(Ads1115ComparatorQueue::MAX_COMP_QUEUE) ?
         ads1115ComparatorQueueToNumber(Ads1115ComparatorQueue::MAX_COMP_QUEUE) : number);
    }

    struct Ads1115ConfigLowByte {
        uint8_t compQue         : 2;    // (COMP_QUE) default 00
        uint8_t compLatch       : 1;    // (COMP_LAT) default 0
        uint8_t compPolarity    : 1;    // (COMP_POL) default 0
        uint8_t compMode        : 1;    // (COMP_MODE) default 0
        uint8_t dataRate        : 3;    // (DR) default 0 means 8 SPS; use DR_860SPS; default 100: 128SPS
    };

    struct Ads1115ConfigHighByte {
        uint8_t operatingMode   : 1;    // (MODE) 0: continuous-conversion; 1: single-shot
        uint8_t gain            : 3;    // (PGA) GAIN_2p048V sets to +/- 2.048 V
        uint8_t mux             : 3;    // (MUX) Which channel to use. SINGLE_SHOT + AIN0-AIN3.
        uint8_t opStatus        : 1;    // (OS)  Set to 0 when writing; Turns to 1 when conversion is complete.
    };                                  // opStatus AKA single-shot start;

    struct Ads1115ConfigAllFields {
        uint16_t compQue         : 2;    // (COMP_QUE) default 00
        uint16_t compLatch       : 1;    // (COMP_LAT) default 0
        uint16_t compPolarity    : 1;    // (COMP_POL) default 0
        uint16_t compMode        : 1;    // (COMP_MODE) default 0
        uint16_t dataRate        : 3;    // (DR) default 0 means 8 SPS; use DR_860SPS; default 100: 128SPS
        uint16_t operatingMode   : 1;    // (MODE) 0: continuous-conversion; 1: single-shot
        uint16_t gain            : 3;    // (PGA) GAIN_2p048V sets to +/- 2.048 V
        uint16_t mux             : 3;    // (MUX) Which channel to use. SINGLE_SHOT + AIN0-AIN3.
        uint16_t opStatus        : 1;    // (OS)  Set to 0 when writing; Turns to 1 when conversion is complete.
    };



    union ACLowByte {
        Ads1115ConfigLowByte lowByteFields;
        uint8_t     lowByte;
    };

    union ACHighByte {
        Ads1115ConfigHighByte highByteFields;
        uint8_t     highByte;
    };

    struct Ads1115ConfigRegisterBytes {
        Ads1115ConfigLowByte    lowByte;
        Ads1115ConfigHighByte   highByte;
    };

    union Ads1115ConfigUnion {
        Ads1115ConfigAllFields fields;
        uint16_t shortWord;
    };
    using Ads1115ConfigRegister_t = Ads1115ConfigUnion;

    /**
     * Here are definitions of the config register fields. Least significant down to most significant.
     *
     * Comparator queue (COMP_QUE) causes comparator to wait for 1, 2, or 4 readings after threshold prior to asserting.
     * Comparator Latch (COMP_LAT) causes comparator to latch or not (see DS 9.3.7)
     * Comparator Polarity (COMP_POL) controls polarity of ALERT/RDY pin: 0 - Active low (default) 1 - Active high
     * Comparator Mode (COMP_MODE) configures comparator as traditional or window (see DS 9.3.7)
     * Data Rate (DR): 3 bits specify data output rate (see DS 9.3.6)
     *
     * Operating mode (MODE): 0 - continuous-conversion mode; 1 - single-shot mode or power-down state
     * Programmable gain amplifier config (PGA) determines the gain setting
     * Input Multiplexer (MUX): bits configure the input. AKA channel
     * Operational state (OS): single-shot conversion start
     */


//------------------------------------
// Convenience methods for getting values from a config register
/*
        struct Ads1115ConfigAllFields {
            uint16_t compQue         : 2;    // (COMP_QUE) default 00
            uint16_t compLatch       : 1;    // (COMP_LAT) default 0
            uint16_t compPolarity    : 1;    // (COMP_POL) default 0
            uint16_t compMode        : 1;    // (COMP_MODE) default 0
            uint16_t dataRate        : 3;    // (DR) default 0 means 8 SPS; use DR_860SPS; default 100: 128SPS
            uint16_t operatingMode   : 1;    // (MODE) 0: continuous-conversion; 1: single-shot
            uint16_t gain            : 3;    // (PGA) GAIN_2p048V sets to +/- 2.048 V
            uint16_t mux             : 3;    // (MUX) Which channel to use. SINGLE_SHOT + AIN0-AIN3.
            uint16_t opStatus        : 1;    // (OS)  Set to 0 when writing; Turns to 1 when conversion is complete.
        };
*/
    constexpr uint16_t getOpStatusFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return configRegister.fields.opStatus;
    }
    constexpr uint16_t getMuxFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return configRegister.fields.mux;
    }
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint16_t getAINFromMux (const uint16_t mux) {
        return mux % 4; // VALID ONLY FOR SINGLE_SHOT!
    }
    // ReSharper disable once CppDFAUnreachableFunctionCall
    constexpr uint8_t getGainNumberFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return configRegister.fields.gain & 0x7;    // just three bits.
    }
    constexpr AdsGainValues getGainValueFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return numberToAdsGainValues(getGainNumberFromConfigRegister(configRegister));
    }

//------------------------------------

    constexpr uint8_t getAinFromChannel (const Ads1115Channel_t channel) {
        return getAINFromMux(ads1115ChannelToNumber(channel));
    }
    constexpr uint8_t getMuxValue (const Ads1115Channel_t channel) {
        return ads1115ChannelToNumber(channel); // Just the ordinal number for the enum.
    }
    constexpr uint8_t getOperatingModeValue (const Ads111xOperatingMode mode) {
        return ads111xOperatingModeToNumber(mode);  // Just the ordinal number for the enum.
    }
    constexpr uint8_t getAddressRegisterValue (const Ads1113RegisterAddresses reg) {
            return ads1113RegisterAddressesToNumber(reg);   // Just the ordinal number for the enum.
    }

}   // namespace CSconverters

#endif // ADS1115_DEFINITIONS_

