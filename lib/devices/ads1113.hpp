#pragma once
#ifndef ADS1113_HPP_
#define ADS1113_HPP_

#include "component.hpp"
#include "csi2c.hpp"

namespace CSdevices {

    class Ads1113 final : public Component {

    public:
        Ads1113 (const std::string& label,
                 const ControllerId controllerId,
                 const uint8_t      i2cAddress) :
                                    controllerId_(controllerId),
                                    i2cAddress_(i2cAddress) {
            setClassName("Ads1113");
            setLabel(label);
        }
        ~Ads1113 () override = default;



    private:
        ControllerId    controllerId_;
        uint8_t         i2cAddress_;

    };

}

#endif
