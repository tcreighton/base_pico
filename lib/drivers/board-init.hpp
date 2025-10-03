
#pragma once
#ifndef BOARD_INIT_HPP_
#define BOARD_INIT_HPP_

#include "gpio.hpp"
#include "../utils/communication.hpp"
#include "pico/stdio.h"

namespace CSdrivers {

    // This initializes the low-level devices: gpio, i2c, communication.
    // All of these need to be in place before pretty much anything else.
    // There is a little bit of confusion since this centralization of the init functions happened late in the game.
    // This is called from main.
    inline bool boardInit () {
        auto retCode = true;

        if (!stdio_init_all()) {    // get set to handle stdio
            exit(-1);
        }

        Gpio::init();

        if (!CScommunication::Communication::initUsb()) { // if no usb, try uart; only return one or the other
            retCode |= CScommunication::Communication::initUart();
        }

        return retCode;
    }

}


#endif  // BOARD_INIT_HPP_
