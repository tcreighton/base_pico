#pragma once
#ifndef DAC_HPP_
#define DAC_HPP_

#include "component.hpp"
#include "dac-channel.hpp"
#include "devicesContainer.hpp"

namespace CSdrivers {

    ///
    /// This class hides the details of dac channels, dac device, etc.
    /// It gets instantiated for as many dac channel:dac device combos exist.
    /// The caller needs to know the controllerId for each dac device and which channel is to be used.
    /// But it doesn't have to actually instantiate any of the lower-level objects.
    /// Since the MCP4728 class is instantiated as many times as needed in an application, and
    /// since each instantiation can be used for several channels and therefore several of these DacMcp4728
    /// instances, we depend on those being instantiated in the CSdevices container. Access via getDac...
    ///
    class DacMcp4728 final : public CSdevices::Component {

    public:
        DacMcp4728 (const std::string &label,
                    const std::string &dacLabel,
                    const CSdevices::ControllerId controllerId,
                    const CSdevices::DacId dacId,
                    const CSdevices::DacChannelIds channelId) :
                            dacChannel_(dacLabel, controllerId, dacId, channelId) {
            setClassName("DacMcp4728");
            setLabel (label);
        }
        ~DacMcp4728 () override = default;

        [[nodiscard]] bool writeDacInputRegister (const uint16_t data) const {
            return getDac(dacChannel_.getDacId()).writeDacInputRegister(dacChannel_, data);
        }

    private:
        CSdevices::DacChannel dacChannel_;
    };

}

#endif

