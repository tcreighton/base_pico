#pragma once
#ifndef DEVICES_CONTAINER_HPP_

#include "ads1115.hpp"
#include "csi2c.hpp"
#include "dac-channel.hpp"
#include "mcp4728.hpp"

namespace CScore {

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

    inline Mcp4728& getGridDac () {
        static Mcp4728 gridDac_ {std::string("MCP4728 gridDac"),
                                    ControllerId::I2C_CONTROLLER_0,
                                    DacId::GRID_DAC};
        return gridDac_;
    }

    inline Mcp4728& getAnodeDac () {
        static Mcp4728 anodeDac_ {std::string("MCP4728 anodeDac"),
                                    ControllerId::I2C_CONTROLLER_1,
                                    DacId::ANODE_DAC};
        return anodeDac_;
    }

    inline DacChannel& getGridDacChannelA () {    // Heater PS
        static DacChannel gridDacChannelA_{std::string("gridDacChannelA"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::GRID_DAC,
                                                          DacChannelIds::CHANNEL_A};
        return gridDacChannelA_;
    }

    inline DacChannel& getGridDacChannelB () {    // Suppressor PS
        static DacChannel gridDacChannelB_{std::string("gridDacChannelB"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::GRID_DAC,
                                                          DacChannelIds::CHANNEL_B};
        return gridDacChannelB_;
    }

    inline DacChannel& getGridDacChannelC () {    // Extractor PS
        static DacChannel gridDacChannelC_{std::string("gridDacChannelC"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::GRID_DAC,
                                                          DacChannelIds::CHANNEL_C};
        return gridDacChannelC_;
    }

    inline DacChannel& getGridDacChannelD () {    // Focus PS
        static DacChannel gridDacChannelD_{std::string("gridDacChannelD"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::GRID_DAC,
                                                          DacChannelIds::CHANNEL_D};
        return gridDacChannelD_;
    }

    inline DacChannel& getAnodeDacChannelA () {    // Anode PS
        static DacChannel gridAnodeChannelA_{std::string("gridAnodeChannelA"),
                                                          ControllerId::I2C_CONTROLLER_0,
                                                          DacId::ANODE_DAC,
                                                          DacChannelIds::CHANNEL_A};
        return gridAnodeChannelA_;
    }

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
