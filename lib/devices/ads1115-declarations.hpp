#pragma once

#ifndef ADS1115_DEFINITIONS_
#define ADS1115_DEFINITIONS_

#include <cstdint>
#include <string>

namespace CSdevices {

    // Programmable Gain Amplifier (PGA) Config register bits [2:0]
    enum class Ads1115GainValues : uint8_t {  // Programmable Gain Amplifier (PGA) Settings
        GAIN_6p144V =   0b000,  // 000: FSR +/- 6.144 V (ARDUINO: GAIN_TWOTHIRDS)
        GAIN_4p096V =   0b001,  // 001: FSR +/- 4.096 V (ARDUINO: GAIN_ONE)
        GAIN_2p048V =   0b010,  // 010: FSR +/- 2.048 V (ARDUINO: GAIN_TWO)
        GAIN_1p024V =   0b011,  // 011: FSR +/- 1.024 V (ARDUINO: GAIN_FOUR)
        GAIN_0p512V =   0b100,  // 100: FSR +/- 0.512 V (ARDUINO: GAIN_EIGHT)
        GAIN_0p256V =   0b101,  // 101: FSR +/- 0.256 V (ARDUINO: GAIN_SIXTEEN)
        MAX_GAIN = GAIN_0p256V
        // 0b110 and 0b111 undefined
    };

    // ReSharper disable once CppDFAConstantFunctionResult
    // ReSharper disable once CppDFAConstantParameter
    constexpr uint8_t ads1115GainValuesToNumber (Ads1115GainValues gain) {
        return static_cast<uint8_t>(gain);
    }
    constexpr Ads1115GainValues numberToAds1115GainValues (uint8_t number) {
        return static_cast<Ads1115GainValues>
        (number >= ads1115GainValuesToNumber(Ads1115GainValues::MAX_GAIN) ?
         ads1115GainValuesToNumber(Ads1115GainValues::MAX_GAIN) : number);
    }

    using Ads1115Gain_t = Ads1115GainValues;

    inline float ads1115GetFSR (const Ads1115Gain_t gain) { // FSR: Full-Scale Range
        auto gainValue = 0.0f;
        constexpr auto gainOne = 2.048f * 2.0f;
        switch (gain) {
            case Ads1115GainValues::GAIN_0p256V:
                gainValue =  gainOne * .0625f;   // 0.256V
                break;
            case Ads1115GainValues::GAIN_0p512V:
                gainValue =  gainOne * 0.125f;   // 0.512V
                break;
            case Ads1115GainValues::GAIN_1p024V:
                gainValue =  gainOne * 0.25f;    // 1.024V
                break;
            case Ads1115GainValues::GAIN_2p048V:
                gainValue =  gainOne * 0.5f;     // 2.048V
                break;
            case Ads1115GainValues::GAIN_4p096V:
                gainValue =  gainOne * 1.0f;     // 4.096V
                break;
            case Ads1115GainValues::GAIN_6p144V:
                gainValue =  gainOne * 1.5f;     // 6.144V
                break;
        }
        return gainValue;
    }
    // FSR: Full-Scale Range

    inline float ads1115GetFSR (const uint8_t number) {
        return ads1115GetFSR(numberToAds1115GainValues(number));
    }

    constexpr uint16_t ADS_1115_RANGE = 0x7fff;

    inline float ads1115GetFSRRatio (const Ads1115Gain_t gain) {
        return ads1115GetFSR(gain) / static_cast<float>(ADS_1115_RANGE);
    }
    // This one is for when you know the ordinal for the Gain value
    /*
    inline float Ads1115GetFSRRatio (const uint8_t number) {// must be in range or default is GAIN_0p256V
        return Ads1115GetFSRRatio(NumberToAds1115GainValues(number));
    }
    */
    /*inline float Ads1115GetFSRRatio (const float gain) {
        return gain / Ads1115Range;
    }*/

    constexpr std::string ads1115PGAToString (const Ads1115GainValues gain) {
        std::string retVal;

        switch (gain) {
            case Ads1115GainValues::GAIN_0p256V:
                retVal =  "GAIN_0p256V";
                break;
            case Ads1115GainValues::GAIN_0p512V:
                retVal =  "GAIN_0p512V";
                break;
            case Ads1115GainValues::GAIN_1p024V:
                retVal =  "GAIN_1p024V";
                break;
            case Ads1115GainValues::GAIN_2p048V:
                retVal =  "GAIN_2p048V";
                break;
            case Ads1115GainValues::GAIN_4p096V:
                retVal =  "GAIN_4p096V";
                break;
            case Ads1115GainValues::GAIN_6p144V:
                retVal =  "GAIN_6p144V";
                break;
        }
        return retVal;
    }

    //AKA Mux - For Focus Rack 500, we only do single shot
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


    enum class Ads1115DataRates {
        DR_8SPS     = 0b000,    // 0b000
        DR_16SPS    = 0b001,   // 0b001
        DR_32SPS    = 0b010,   // 0b010
        DR_64SPS    = 0b011,   // 0b011
        DR_128SPS   = 0b100,  // 0b100
        DR_250SPS   = 0b101,  // 0b101
        DR_475SPS   = 0b110,  // 0b110
        DR_860SPS   = 0b111,  // 0b111
        MAX_SPS = DR_860SPS
    };
    constexpr uint8_t ads1115DataRatesToNumber (Ads1115DataRates dataRate) {
        return static_cast<uint8_t>(dataRate);
    }
    constexpr Ads1115DataRates numberToAds1115DataRates (uint8_t number) {
        return static_cast<Ads1115DataRates>
        (number >= ads1115DataRatesToNumber(Ads1115DataRates::MAX_SPS) ?
         ads1115DataRatesToNumber(Ads1115DataRates::MAX_SPS) : number);
    }
    constexpr std::string ads1115DataRatesToString (const Ads1115DataRates dataRate) {
        std::string retVal;
        switch (dataRate) {
            case Ads1115DataRates::DR_860SPS:
                retVal = "DR_860SPS";
                break;
            case Ads1115DataRates::DR_475SPS:
                retVal = "DR_475SPS";
                break;
            case Ads1115DataRates::DR_250SPS:
                retVal = "DR_250SPS";
                break;
            case Ads1115DataRates::DR_128SPS:
                retVal = "DR_128SPS";
                break;
            case Ads1115DataRates::DR_64SPS:
                retVal = "DR_64SPS";
                break;
            case Ads1115DataRates::DR_32SPS:
                retVal = "DR_32SPS";
                break;
            case Ads1115DataRates::DR_16SPS:
                retVal = "DR_16SPS";
                break;
            case Ads1115DataRates::DR_8SPS:
                retVal = "DR_8SPS";
                break;
        }
        return retVal;
    }

    enum class Ads1115OperationalStatus {   // Read the config register and check the top bit - OS for AVAILABLE
        NO_EFFECT_OR_PERFORMING_CONVERSION, // Read: Currently performing conversion; Write: Nothing
        START_NEW_CONVERSION_OR_CONVERSION_COMPLETE,// Read: The conversion is complete. Write: starts a new conversion.
        OPERATIONAL_ERROR_STATE,               // Read: The device is in an error state. Write: Error state
        MAX_OP_STATUS = OPERATIONAL_ERROR_STATE
    };

    constexpr uint8_t ads1115OperationalStatusToNumber (Ads1115OperationalStatus opStatus) {
        return static_cast<uint8_t>(opStatus);
    }
    constexpr Ads1115OperationalStatus numberToAds1115OperationalStatus (uint8_t number) {
        return static_cast<Ads1115OperationalStatus>
            (number >= ads1115OperationalStatusToNumber(Ads1115OperationalStatus::MAX_OP_STATUS) ?
             ads1115OperationalStatusToNumber(Ads1115OperationalStatus::MAX_OP_STATUS) : number);
    }
    constexpr std::string ads1115OperationalStatusToString (Ads1115OperationalStatus(opStatus)) {
        std::string retVal;

        switch (opStatus) {

            case Ads1115OperationalStatus::NO_EFFECT_OR_PERFORMING_CONVERSION:
                retVal = "NO_EFFECT_OR_PERFORMING_CONVERSION";
                break;
            case Ads1115OperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE:
                retVal = "START_NEW_CONVERSION_OR_CONVERSION_COMPLETE";
                break;
            default: ;
        }
        return retVal;
    }
    enum class Ads1115OperatingMode : uint8_t {
        CONTINUOUS_CONVERSION,          // Set this for continuous conversion mode
        SINGLE_SHOT,                     // Set this for single-shot or power-down state - default
        MAX_MODE = SINGLE_SHOT
    };
    constexpr uint8_t ads1115OperatingModeToNumber (Ads1115OperatingMode mode) {
        return static_cast<uint8_t>(mode);
    }
    constexpr Ads1115OperatingMode numberToAds1115OperatingMode (uint8_t number) {
        // If number is out of range, treat it as SINGLE_SHOT
        return static_cast<Ads1115OperatingMode>
            (number >= ads1115OperatingModeToNumber(Ads1115OperatingMode::MAX_MODE)  ?
             ads1115OperatingModeToNumber(Ads1115OperatingMode::MAX_MODE) : number);
    }
    constexpr std::string ads1115OperatingModeToString (const Ads1115OperatingMode mode) {
        std::string retVal;

        switch (mode) {
            case Ads1115OperatingMode::SINGLE_SHOT:
                retVal = "SINGLE_SHOT";
                break;
            case Ads1115OperatingMode::CONTINUOUS_CONVERSION:
                retVal = "CONTINUOUS_CONVERSION";
                break;
        }

        return retVal;
    }

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

    union ConfigUnion { // Declare one of these then copy the Ads1115ConfigRegister into fields & out of shortWord
        Ads1115ConfigAllFields fields;
        uint16_t shortWord;
    };
    using Ads1115ConfigRegister_t = ConfigUnion;

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

    struct Ads1115ARegisterFields {
        uint8_t registerId  : 2;
        uint8_t reserved    : 6;
    };

    union Ads1115ARegisterUnion {
        Ads1115ARegisterFields fields;
        uint8_t byteValue;
    };

    /**
     * These values determine which register we will read.
     * First send the address pointer register with one of these values.
     * Then read to get one of the four registers back.
     */
    enum class Ads1115RegisterAddresses : uint8_t {
        ADS1115_CONVERSION_REG_ADDR = 0b00,
        ADS1115_CONFIG_REG_ADDR     = 0b01,
        ADS1115_LO_THRESH_REG_ADDR  = 0b10,
        ADS1115_HI_THRESH_REG_ADDR  = 0b11,
        MAX_REG_ADDR = ADS1115_HI_THRESH_REG_ADDR
    };
    constexpr uint8_t ads1115RegisterAddressesToNumber (Ads1115RegisterAddresses addr) {
        return static_cast<uint8_t>(addr);
    }
    constexpr Ads1115RegisterAddresses numberToAds1115RegisterAddresses (uint8_t number) {
        return static_cast<Ads1115RegisterAddresses>
        (number >= ads1115RegisterAddressesToNumber(Ads1115RegisterAddresses::MAX_REG_ADDR)  ?
         ads1115RegisterAddressesToNumber(Ads1115RegisterAddresses::MAX_REG_ADDR) : number);
    }


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
    constexpr uint16_t getAINFromMux (uint16_t mux) {
        return mux % 4; // VALID ONLY FOR SINGLE_SHOT!
    }
    constexpr uint8_t getGainNumberFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return configRegister.fields.gain & 0x7;    // just three bits.
    }
    constexpr Ads1115GainValues getGainValueFromConfigRegister (const Ads1115ConfigRegister_t configRegister) {
        return numberToAds1115GainValues(getGainNumberFromConfigRegister(configRegister));
    }

//------------------------------------

    constexpr uint8_t getAinFromChannel (const Ads1115Channel_t channel) {
        return getAINFromMux(ads1115ChannelToNumber(channel));
    }
    constexpr uint8_t getMuxValue (const Ads1115Channel_t channel) {
        return ads1115ChannelToNumber(channel); // Just the ordinal number for the enum.
    }
    constexpr uint8_t getOperatingModeValue (const Ads1115OperatingMode mode) {
        return ads1115OperatingModeToNumber(mode);  // Just the ordinal number for the enum.
    }
    constexpr uint8_t getAddressRegisterValue (const Ads1115RegisterAddresses reg) {
            return ads1115RegisterAddressesToNumber(reg);   // Just the ordinal number for the enum.
    }

}   // namespace CSconverters

#endif // ADS1115_DEFINITIONS_

