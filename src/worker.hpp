#pragma once
#ifndef WORKER_HPP_
#define WORKER_HPP_
#include "csi2c.hpp"

namespace CSworkers {

    class Worker final {
    public:
        Worker () = default;
        ~Worker() = default;

        bool doWork();



        //-------------------------------------------------------------------------------------------------
        //  Singleton component declarations here!

        static CSdevices::CsI2C& getController0 () {
            using CSdevices::ControllerId::I2C_CONTROLLER_0;
            static CSdevices::CsI2C controller0_ {std::string("I2C_Controller_0 is i2c0"), I2C_CONTROLLER_0};
            return controller0_;
        }

        static CSdevices::CsI2C& getController1 () {
            using CSdevices::ControllerId::I2C_CONTROLLER_1;
            static CSdevices::CsI2C controller1_ {std::string("I2C_Controller_1 is i2c1"), I2C_CONTROLLER_1};
            return controller1_;
        };



    };
};



#endif  // WORKER_HPP_
