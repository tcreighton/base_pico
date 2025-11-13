
#include <cmath>
#include "external-thermistor.hpp"

namespace CSdevices {
    /**
     * @brief This function converts the counts from the ADC into a temperature value.
     * The temperature will be computed in degrees Kelvin. It can be retrieved in degrees C from a different method.
     * The formula is documented elsewhere since it's had as an image. But here is the transformation into characters:
     * Tk = 1 / ((1 / beta) * ln((R1 * ((Vin/Vout) - 1)) / R0) + (1 / T0))
     *
     * @param counts Value returned from the adc.
     * @return Temperature in degrees Kelvin.
     */
    double ExternalThermistor::processCounts(const uint16_t counts) {
        // Conversions for the ABNTC-0805-103-3950
        // EXT stands for External - ie. Not the onboard temp sensor
        static constexpr auto R0 = 10.0;                                // thermistor resistance at T0
        static constexpr auto R1 = 680.0;                               // external resistor
        static constexpr auto T0 = 25.0 + REF_K;                 // External reference temp in degrees K
        static constexpr auto T0_RECIPROCAL = 1/T0;
        static constexpr auto BETA = 3950.0;                            // ABNTC_0805_103_3950_BETA Temp sensor
        static constexpr auto RESISTOR_RATIO = R1/R0;
        static constexpr auto betaReciprocal = 1 / BETA;
        const auto adcOutputVoltage = static_cast<double>(counts) * VOLTAGE_CONVERSION_RATIO;
        const auto voltageRatio = VREF / adcOutputVoltage;
        const auto tempInK = 1.0/(betaReciprocal * std::log(RESISTOR_RATIO * (voltageRatio - 1)) + T0_RECIPROCAL);

        setCachedTemperature (tempInK);

        return getCachedTemperature ();
    }
}
