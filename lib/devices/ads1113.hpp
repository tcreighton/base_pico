#pragma once
#ifndef ADS1113_HPP_
#define ADS1113_HPP_

#include "ads1113-definitions.hpp"
#include "ads111x.hpp"
#include "component.hpp"
#include "csi2c.hpp"

namespace CSdevices {

    class Ads1113 final : public Ads111x {

    public:
        Ads1113 (const std::string& label,
                 const ControllerId controllerId,
                 const uint8_t      i2cAddress) : Ads111x (controllerId, i2cAddress) {
            setClassName("Ads1113");
            setLabel(label);
        }
        ~Ads1113 () override = default;

        static Ads1113ConfigRegister_t getNewConfigRegister ();   // Default values: OS: Start conversion; mode: single-shot
        static Ads1113ConfigRegister_t getNewConfigRegister (Ads111xOperationalStatus os, Ads111xOperatingMode mode);

        bool startContinuousConversion ();
        bool isDataReady ();
        bool readConfigRegister (Ads1113ConfigRegister_t& configRegister);
        bool writeConfigRegister (Ads1113ConfigRegister_t configRegister);


    protected:
        void setConversionPending (const bool conversionPending) {conversionPending_ = conversionPending;}
        void setConversionTimeout (const uint32_t conversionTimeout) {conversionTimeout_ = conversionTimeout;}

    private:
        absolute_time_t     conversionTimeout_{};    // This gets set when we start a conversion.
        Ads111xSampleRates  sampleRate_ = Ads111xSampleRates::SR_860SPS;    // This is constant for the 1113.
        bool                conversionPending_ = false;

    };

}

#endif
