
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/regs/adc.h" // For direct register access if needed
#include "pico-adc.hpp"


namespace CSdevices {

    void PicoAdc::startConversion() const {
        adc_select_input(static_cast<std::underlying_type_t<PicoAin>>(getAinSelect()));
        hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);
    }

    uint16_t PicoAdc::readValue() {
        while (!(adc_hw->cs & ADC_CS_READY_BITS))
            tight_loop_contents();

        cachedValue_ = static_cast<uint16_t>(adc_hw->result & 0xffff);
        processCounts (cachedValue_);

        return cachedValue_;
    }

}

