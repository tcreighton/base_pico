#pragma once
#ifndef EXTERNAL_THERMISTOR_
#define EXTERNAL_THERMISTOR_
#include "pico-adc.hpp"

namespace CSdevices {

    // This class name may have to change if we decide to have more than one type of external thermistor.
    class ExternalThermistor final : public PicoAdc {
    public:

        ExternalThermistor(const std::string& label, const PicoAin ainSelect) : PicoAdc(label, ainSelect) {}

        ~ExternalThermistor() override = default;

        /**
         * @brief This returns the cached temperature. That gets updated whenever readValue is called.
         * @return The cached temperature.
         */
        [[nodiscard]] double getTemperature() const {return cachedTemperature_;}

    private:
        static constexpr auto REF_K = 273.15;                         // 0C == 273.15K

        /**
         *
         * @param counts This is the value returned from the ADC.
         * @return The temperature read by the onboard temperature sensor.
         */
        double processCounts (uint16_t counts) override;

        [[nodiscard]] double getCachedTemperature () const {return cachedTemperature_;}
        [[nodiscard]] auto getCachedTemperatureInC () const {return getCachedTemperature() - REF_K;};
        void setCachedTemperature (const double temp) {cachedTemperature_ = temp;}


        double cachedTemperature_ = 0.0;

    };
}


#endif
