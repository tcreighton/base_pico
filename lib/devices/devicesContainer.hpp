#pragma once
#ifndef DEVICES_CONTAINER_HPP_

#include "ads1115.hpp"
#include "csi2c.hpp"
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

    /*
    inline DacChannel& getGridDacChannelA () {    // Heater PS
        static DacChannel gridDacChannelA_{std::string("gridDacChannelA"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::DAC_00,
                                                          DacChannelIds::CHANNEL_A};
        return gridDacChannelA_;
    }

    inline DacChannel& getGridDacChannelB () {    // Suppressor PS
        static DacChannel gridDacChannelB_{std::string("gridDacChannelB"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::DAC_00,
                                                          DacChannelIds::CHANNEL_B};
        return gridDacChannelB_;
    }

    inline DacChannel& getGridDacChannelC () {    // Extractor PS
        static DacChannel gridDacChannelC_{std::string("gridDacChannelC"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::DAC_00,
                                                          DacChannelIds::CHANNEL_C};
        return gridDacChannelC_;
    }

    inline DacChannel& getGridDacChannelD () {    // Focus PS
        static DacChannel gridDacChannelD_{std::string("gridDacChannelD"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::DAC_00,
                                                          DacChannelIds::CHANNEL_D};
        return gridDacChannelD_;
    }

    inline DacChannel& getAnodeDacChannelA () {    // Anode PS
        static DacChannel gridAnodeChannelA_{std::string("gridAnodeChannelA"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::DAC_01,
                                                          DacChannelIds::CHANNEL_A};
        return gridAnodeChannelA_;
    }
    */

    inline Ads1115& getAdc0 () {
        static Ads1115 adc0_{std::string(   "ADC For Heater & Grid 1"),
                                                ControllerId::I2C_CONTROLLER_0,
                                            0x48};
        return adc0_;
    }

    inline Ads1115& getAdc1 () {
        static Ads1115 adc1_{std::string(   "ADC For Grid 2 & Grid 3"),
                                                ControllerId::I2C_CONTROLLER_0,
                                            0x49};
        return adc1_;
    }

    inline Ads1115& getAdc2 () {
        static Ads1115 adc2_{std::string(   "ADC For Anode"),
                                                ControllerId::I2C_CONTROLLER_1,
                                            0x48};
        return adc2_;
    }

}

#endif  // DEVICES_CONTAINER_HPP_
