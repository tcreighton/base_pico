#pragma once
#ifndef ADC1113_HPP
#define ADC1113_HPP
#include "adc.hpp"
#include "component.hpp"


namespace CSdrivers {

    class Adc1113 final : public Adc {

    public:
        Adc1113 (const std::string &label) {
            setClassName("Adc1113");
            setLabel(label);
        }

        ~Adc1113 () override = default;

    };

}

#endif
