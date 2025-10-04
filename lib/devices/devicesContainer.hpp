#pragma once
#ifndef DEVICES_CONTAINER_HPP_
#include "csi2c.hpp"

namespace CSdevices {

    static CsI2C& getController0 () {
        using ControllerId::I2C_CONTROLLER_0;
        static CsI2C controller0_ {std::string("I2C_Controller_0 is i2c0"), I2C_CONTROLLER_0};
        return controller0_;
    }

    static CsI2C& getController1 () {
        using ControllerId::I2C_CONTROLLER_1;
        static CsI2C controller1_ {std::string("I2C_Controller_1 is i2c1"), I2C_CONTROLLER_1};
        return controller1_;
    };

    static CsI2C& getController (const ControllerId controllerId) {
        return ControllerId::I2C_CONTROLLER_0 == controllerId ? getController0() : getController1();
    }


}

#endif  // DEVICES_CONTAINER_HPP_
