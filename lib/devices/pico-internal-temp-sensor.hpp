#pragma once
#ifndef INTERNAL_TEMP_SENSOR_
#define INTERNAL_TEMP_SENSOR_
#include "pico-adc.hpp"

namespace CSdevices {

    class InternalTempSensor final : public PicoAdc {
    public:
        InternalTempSensor(const std::string& label, const PicoAin ainSelect) : PicoAdc(label, ainSelect) {}

        ~InternalTempSensor() override = default;

        /**
         * @brief This returns the cached temperature. That gets updated whenever readValue is called.
         * @return The cached temperature.
         */
        [[nodiscard]] double getTemperature() const {return cachedTemperature_;}

    private:

        /**
         *
         * @param counts This is the value returned from the ADC.
         * @return The temperature read by the onboard temperature sensor.
         */
        double processCounts (uint16_t counts) override;

        [[nodiscard]] double getCachedTemperature () const {return cachedTemperature_;}
        void setCachedTemperature (const double temp) {cachedTemperature_ = temp;}

        static constexpr double INT_REFERENCE_TEMP          = 27.0f;        // The temp used by RP to characterize this
        static constexpr double INT_REFERENCE_TEMP_VOLTS    = 0.706f;       // The voltage at reference temperature 27°
        static constexpr double INT_DEGREE_VOLTS_SLOPE      = 0.001721f;    // The rate of change of voltage per degree

        double cachedTemperature_ = 0.0;
    };

}


#endif
