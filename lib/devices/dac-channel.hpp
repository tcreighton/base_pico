#pragma once

#ifndef DAC_CHANNEL_HPP_
#define DAC_CHANNEL_HPP_


#include "component.hpp"
#include "csi2c.hpp"
#include "dac-declarations.hpp"


namespace CScore {

/**
 * @brief - The DacChannel class provides interface information for MCP4728 and MCP4725 DACs.
 * Other types of DACs could possibly be added to this and thereby allow for ease of changing
 * among the DAC chips as needed.
 *
 */

    class DacChannel final : public Component {

    public:

        DacChannel(const std::string &label,
                   const ControllerId controllerId,
                   const DacId dacId,
                   const DacChannelIds channelId) :
                                    dacId_(dacId),
                                    controllerId_(controllerId),
                                    channelId_(channelId),
                                    i2cTimeOut_(CsI2C::getI2CTimeoutPerByte_us()) {
            setClassName("DacChannel");
            setLabel(label);
        }

        DacChannel(const DacChannel& other) = delete;
        DacChannel& operator=(const DacChannel& other) = delete;
        ~DacChannel() override = default;

        [[nodiscard]] ControllerId getControllerId() const { return controllerId_; }
        [[nodiscard]] DacChannelIds getChannelId() const { return channelId_; }
        [[nodiscard]] DacId getDacId() const { return dacId_; }
        [[nodiscard]] DacPowerDownValues getPowerMode() const { return powerMode_; }
        [[nodiscard]] DacGainValues getGain() const { return gain_; }
        [[nodiscard]] DacVrefValues getVref() const { return vref_; }
        [[nodiscard]] uint getI2cTimeOut() const { return i2cTimeOut_; }

        static std::string dacChannelIdToString(DacChannelIds channelId);

        bool init () override;

        void setPowerMode (const DacPowerDownValues mode) {
            powerMode_ = mode;
        }
        void setGain(const DacGainValues gain) {
            gain_ = gain;
        }
        void setVref(const DacVrefValues vref) {
            vref_ = vref;
        }
        void setI2cTimeOut(const uint i2cTimeOut) {
            i2cTimeOut_ = i2cTimeOut;
        }

    private:

        /**
         * @brief - These declarations have default values. They can be overridden by the constructor.
         */
        DacId dacId_;
        ControllerId controllerId_;
        DacChannelIds channelId_; // Default IS overridden in the constructor initialization.
        DacPowerDownValues powerMode_ = DacPowerDownValues::PD_OFF; // Default must be overridden in constructor
        DacGainValues gain_ = DacGainValues::GAIN_1;                // Default must be overridden in constructor
        DacVrefValues vref_ = DacVrefValues::VREF_INT;              // Default must be overridden in constructor
        uint i2cTimeOut_{};// Default must be overridden in constructor


    };

} // namespace CSconverters

#endif

