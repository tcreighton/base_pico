#pragma once
#ifndef DRIVERS_CONTAINER_HPP_

#include "eeprom0.hpp"

namespace CSdrivers {

    inline EeProm0& getEEProm0 () {
        static EeProm0 eeProm0_{CSdevices::ControllerId::I2C_CONTROLLER_1};
        return eeProm0_;
    }

    inline CSdevices::Mcp24Lc32& getEEProm (const CSdevices::EEPromId eepromId) {
        switch (eepromId) {
            default:

            case CSdevices::EEPromId::EEPROM_0:
                return getEEProm0();

            /* future expansion
            case EEPromId::EEPROM_1:
                return getEEProm1();
            case EEPromId::EEPROM_2:
                return getEEProm2();
            case EEPromId::EEPROM_3:
                return getEEProm3();
            case EEPromId::EEPROM_4:
                return getEEProm4();
            case EEPromId::EEPROM_5:
                return getEEProm5();
            case EEPromId::EEPROM_6:
                return getEEProm6();
            case EEPromId::EEPROM_7:
                return getEEProm7();
                */

        }
    }
}


#endif  // DRIVERS_CONTAINER_HPP_
