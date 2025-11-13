#pragma once
#ifndef PICO_ADC_HPP_
#define PICO_ADC_HPP_

#include <cstdint>
#include "hardware/adc.h"
#include "component.hpp"


namespace CSdevices {

    enum class PicoAin: uint8_t {
        PICO_AINSEL_0   = 0x00,     // 2350 QFN-80 or 2040/2350 QFN-60
        PICO_AINSEL_1   = 0x01,     // 2350 QFN-80 or 2040/2350 QFN-60
        PICO_AINSEL_2   = 0x02,     // 2350 QFN-80 or 2040/2350 QFN-60
        PICO_AINSEL_3   = 0x03,     // 2350 QFN-80 or 2040/2350 QFN-60
        PICO_AINSEL_4   = 0x04,     // 2350 QFN-80 (adc) or 2040/2350 QFN-60 (temperature sensor)
        PICO_AINSEL_5   = 0x05,     // 2350 QFN-80
        PICO_AINSEL_6   = 0x06,     // 2350 QFN-80
        PICO_AINSEL_7   = 0x07,     // 2350 QFN-80
        PICO_AINSEL_8   = 0x08      // 2350 QFN-80 (temperature sensor)
    };

    class PicoAdc : public CSdevices::Component {

    public:
        PicoAdc (const std::string& label,
                 const PicoAin ainSelect) : ainSelect_(ainSelect) {

            setClassName("PicoAdc");
            setLabel(label);

        }

        ~PicoAdc () override = default;

        /**
         * @brief Begins a conversion process using the AIN in ainSelect_.
         * This does not block. It selects the device and then starts the conversion. Then immediately returns.
         * Note that all the ADCs (0-8) are essentially, if not actually, channels. Only one can be doing a conversion
         * at one time. So you can't start many and then expect to go back and collect the data. To do that you
         * need to code for the continuous conversion and use a FIFO or handle an interrupt to collect results.
         * These devices finish conversions in about 2 microseconds.
         */
        void startConversion () const;

        /**
         * @brief Tests if the current conversion is complete. This does not block.
         * @return Returns true if there is data in the adc register. Ie when conversion is complete.
         */
        static bool isAdcReady() {return (adc_hw->cs & ADC_CS_READY_BITS) != 0;}

    protected:
        [[nodiscard]] PicoAin getAinSelect() const {return ainSelect_;};

        /**
         * @brief This will block until the one-shot conversion completes.
         * Note that the actual result will be 12 bits.
         * This needs to be 'processed' to something that represents the measurement being made by the sensor.
         * @return Returns the result of an ADC conversion. Result comes from the adc register.
         */
        virtual uint16_t readValue ();

        /**
         * @brief This function turns the counts taken from the adc and processes the value to produce what the
         * sensor is trying to measure. What that is isn't understood here, it's just a double value.
         * @param counts Counts were read in readValue. It's the result of an ADC conversion.
         * @return The sensor's value (counts) converted to sensor-centric units, eg. temperature, pressure...
         *
         */
        virtual double processCounts (uint16_t counts) = 0;

        [[nodiscard]] uint16_t getCachedValue() const {return cachedValue_;}

        // Constants used in conversion from voltage read from the ADC pin to a temperature.
        // Also, some constants are used in the conversions for the external thermistors.
        // See section 12.4.6 of the 2350 datasheet.
        // Vbe at 27° C is typically 0.706v.
        // The slope is -1.721mV/°C

        // INT stands for internal; ie. the onboard temp sensor.
        static constexpr double VREF                        = 3.0f;         // This is different from the spec's 3.3v
        static constexpr double BIT_RANGE                   = 0x1000;       // 12 bit adcs
        static constexpr double VOLTAGE_CONVERSION_RATIO    = VREF / BIT_RANGE; // The LSB size. How many volts per bit.

    private:

        PicoAin ainSelect_;
        uint16_t cachedValue_ = 0;

    };


}

#endif
