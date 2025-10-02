
#pragma once
#ifndef BOARD_INIT_HPP_
#define BOARD_INIT_HPP_

#include "../utils/communication.hpp"
#include "pico/stdio.h"

namespace CSconfig {

    // This initializes the low-level devices: gpio, i2c, communication.
    // All of these need to be in place before pretty much anything else.
    // There is a little bit of confusion since this centralization of the init functions happened late in the game.
    // This is called from main.
    inline bool boardInit () {
        auto retCode = true;

        if (!stdio_init_all()) {    // get set to handle stdio
            exit(-1);
        }
        retCode |= (CSerrors::StatusCode::READY == CSfocus500::Focus500Container::getGpio().init());
        if (!CScommunication::Communication::initUsb()) { // if no usb, try uart; only return one or the other
            retCode |= CScommunication::Communication::initUart();
        }
        /*
        retCode |= (CSerrors::StatusCode::READY == CSfocus500::Focus500Container::getController0().init());
        retCode |= (CSerrors::StatusCode::READY == CSfocus500::Focus500Container::getController1().init());
        */

        return retCode;
    }

}


#endif  // BOARD_INIT_HPP_
