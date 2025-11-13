
#include "pico-adc.hpp"
#include "pico-internal-temp-sensor.hpp"

namespace CSdevices {


    double InternalTempSensor::processCounts(uint16_t counts) {
        const double adcOutputVoltage = static_cast<double>(counts) * VOLTAGE_CONVERSION_RATIO;
        const double temp =
                    INT_REFERENCE_TEMP - (adcOutputVoltage - INT_REFERENCE_TEMP_VOLTS) / INT_DEGREE_VOLTS_SLOPE;
        setCachedTemperature(temp);
        return temp;
    }

}
