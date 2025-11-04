#pragma once
#ifndef ADS1114_DECLARATIONS_
#define ADS1114_DECLARATIONS_

#include "ads1113-definitions.hpp"

namespace CSdevices {

    // Programmable Gain Amplifier (PGA) Config register bits [2:0]
    enum class AdsGainValues : uint8_t {  // Programmable Gain Amplifier (PGA) Settings
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
    constexpr uint8_t adsGainValuesToNumber (AdsGainValues gain) {
        return static_cast<uint8_t>(gain);
    }
    constexpr AdsGainValues numberToAdsGainValues (uint8_t number) {
        return static_cast<AdsGainValues>
        (number >= adsGainValuesToNumber(AdsGainValues::MAX_GAIN) ?
         adsGainValuesToNumber(AdsGainValues::MAX_GAIN) : number);
    }

    using AdsGain_t = AdsGainValues;

    inline float adsGetFSR (const AdsGain_t gain) { // FSR: Full-Scale Range
        auto gainValue = 0.0f;
        constexpr auto gainOne = 2.048f * 2.0f;
        switch (gain) {
            case AdsGainValues::GAIN_0p256V:
                gainValue =  gainOne * .0625f;   // 0.256V
                break;
            case AdsGainValues::GAIN_0p512V:
                gainValue =  gainOne * 0.125f;   // 0.512V
                break;
            case AdsGainValues::GAIN_1p024V:
                gainValue =  gainOne * 0.25f;    // 1.024V
                break;
            case AdsGainValues::GAIN_2p048V:
                gainValue =  gainOne * 0.5f;     // 2.048V
                break;
            case AdsGainValues::GAIN_4p096V:
                gainValue =  gainOne * 1.0f;     // 4.096V
                break;
            case AdsGainValues::GAIN_6p144V:
                gainValue =  gainOne * 1.5f;     // 6.144V
                break;
        }
        return gainValue;
    }
    // FSR: Full-Scale Range

    inline float adsGetFSR (const uint8_t number) {
        return adsGetFSR(numberToAdsGainValues(number));
    }

    constexpr uint16_t ADS_RANGE = 0x7fff;

    inline float adsGetFSRRatio (const AdsGain_t gain) {
        return adsGetFSR(gain) / static_cast<float>(ADS_RANGE);
    }

    constexpr std::string adsPGAToString (const AdsGainValues gain) {
        std::string retVal;

        switch (gain) {
            case AdsGainValues::GAIN_0p256V:
                retVal =  "GAIN_0p256V";
                break;
            case AdsGainValues::GAIN_0p512V:
                retVal =  "GAIN_0p512V";
                break;
            case AdsGainValues::GAIN_1p024V:
                retVal =  "GAIN_1p024V";
                break;
            case AdsGainValues::GAIN_2p048V:
                retVal =  "GAIN_2p048V";
                break;
            case AdsGainValues::GAIN_4p096V:
                retVal =  "GAIN_4p096V";
                break;
            case AdsGainValues::GAIN_6p144V:
                retVal =  "GAIN_6p144V";
                break;
        }
        return retVal;
    }


}

#endif
