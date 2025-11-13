#pragma once
#ifndef DEVICES_CONTAINER_HPP_

#include "csi2c.hpp"
#include "external-thermistor.hpp"
#include "pico-internal-temp-sensor.hpp"
#include "mcp4728.hpp"

namespace CSdevices {

    inline CsI2C& getController0 () {   // inline ensures just one instance linked from multiple declarations
        using ControllerId::I2C_CONTROLLER_0;
        static CsI2C controller0_ {std::string("I2C_Controller_0 is i2c0"), I2C_CONTROLLER_0};
        return controller0_;
    }

    inline CsI2C& getController1 () {
        using ControllerId::I2C_CONTROLLER_1;
        static CsI2C controller1_ {std::string("I2C_Controller_1 is i2c1"), I2C_CONTROLLER_1};
        return controller1_;
    };

    inline CsI2C& getController (const ControllerId controllerId) {
        return ControllerId::I2C_CONTROLLER_0 == controllerId ? getController0() : getController1();
    }

    inline Mcp4728& getDac0 () {
        static Mcp4728 dac0_ {std::string("MCP4728 Dac 0"),
                                    ControllerId::I2C_CONTROLLER_0,
                                    DacId::DAC_00};
        return dac0_;
    }

    /*
    inline Mcp4728& getDac1 () {
        static Mcp4728 dac1_ {std::string("MCP4728 Dac 1"),
                                    ControllerId::I2C_CONTROLLER_1,
                                    DacId::DAC_01};
        return dac1_;
    }
    */


    inline Mcp4728& getDac (const DacId dacId) {
        switch (dacId) {
            case DacId::DAC_00: return getDac0();
//            case DacId::DAC_01: return getDac1();
        }

        assert(false && "Invalid DacId value!");
        std::abort();
    }

    inline InternalTempSensor& getOnboardTemperatureSensor () {
        static InternalTempSensor onboardTemperatureSensor_(std::string("OnboardTemperatureSensor"),
                                                            PicoAin::PICO_AINSEL_4);
        return onboardTemperatureSensor_;
    }

    inline ExternalThermistor& getExternalThermistor0 () {
        static ExternalThermistor externalThermistor0_(std::string("ExternalThermistor0"),
                                                   PicoAin::PICO_AINSEL_0);
        return externalThermistor0_;
    }

    inline ExternalThermistor& getExternalThermistor1 () {
        static ExternalThermistor externalThermistor1_(std::string("ExternalThermistor1"),
                                                   PicoAin::PICO_AINSEL_1);
        return externalThermistor1_;
    }

    inline ExternalThermistor& getExternalThermistor2 () {
        static ExternalThermistor externalThermistor2_(std::string("ExternalThermistor2"),
                                                   PicoAin::PICO_AINSEL_2);
        return externalThermistor2_;
    }

    inline ExternalThermistor& getExternalThermistor (const PicoAin ain) {
        switch (ain) {
            default:
            case PicoAin::PICO_AINSEL_0:
                return getExternalThermistor0();
            case PicoAin::PICO_AINSEL_1:
                return getExternalThermistor1();
            case PicoAin::PICO_AINSEL_2:
                return getExternalThermistor2();
        }
    }


}

#endif  // DEVICES_CONTAINER_HPP_
