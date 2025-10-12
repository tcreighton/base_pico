#pragma once
#ifndef ERRORS_HPP_

#include <string>
#include <pico/error.h>


namespace CSerrors {

    enum class ResponseCode {
        PICO_ERROR_RESOURCE_IN_USE_ERROR            = PICO_ERROR_RESOURCE_IN_USE,           // -21
        PICO_ERROR_VERSION_MISMATCH_ERROR           = PICO_ERROR_VERSION_MISMATCH,          // -20
        PICO_ERROR_LOCK_REQUIRED_ERROR              = PICO_ERROR_LOCK_REQUIRED,             // -19
        PICO_ERROR_UNSUPPORTED_MODIFICATION_ERROR   = PICO_ERROR_UNSUPPORTED_MODIFICATION,  // -18
        PICO_ERROR_NOT_FOUND_ERROR                  = PICO_ERROR_NOT_FOUND,                 // -17
        PICO_ERROR_INVALID_DATA_ERROR               = PICO_ERROR_INVALID_DATA,              // -16
        PICO_ERROR_MODIFIED_DATA_ERROR              = PICO_ERROR_MODIFIED_DATA,             // -15
        PICO_ERROR_PRECONDITION_NOT_MET_ERROR       = PICO_ERROR_PRECONDITION_NOT_MET,      // -14
        PICO_ERROR_BUFFER_TOO_SMALL_ERROR           = PICO_ERROR_BUFFER_TOO_SMALL,          // -13
        PICO_ERROR_INVALID_STATE_ERROR              = PICO_ERROR_INVALID_STATE,             // -12
        PICO_ERROR_BAD_ALIGNMENT_ERROR              = PICO_ERROR_BAD_ALIGNMENT,             // -11

        PICO_ERROR_INVALID_ADDRESS_ERROR            = PICO_ERROR_INVALID_ADDRESS,           // -10
        PICO_INSUFFICIENT_RESOURCES_ERROR           = PICO_ERROR_INSUFFICIENT_RESOURCES,    // -9
        PICO_CONNECT_FAILED_ERROR                   = PICO_ERROR_CONNECT_FAILED,            // -8
        PICO_AUTHENTICATION_ERROR                   = PICO_ERROR_BADAUTH,                   // -7
        PICO_IO_ERROR                               = PICO_ERROR_IO,                        // -6
        PICO_INVALID_ARGUMENT_ERROR                 = PICO_ERROR_INVALID_ARG,               // -5
        PICO_NOT_PERMITTED_ERROR                    = PICO_ERROR_NOT_PERMITTED,             // -4
        PICO_NO_DATA_ERROR                          = PICO_ERROR_NO_DATA,                   // -3
        PICO_TIMEOUT_ERROR                          = PICO_ERROR_TIMEOUT,                   // -2
        PICO_GENERIC_ERROR                          = PICO_ERROR_GENERIC,                   // -1

        NOT_AN_ERROR                                = PICO_OK,                              //  0 NOT AN ERROR

        APP_UNKNOWN_ERROR, // Who knows? Should be tracked down and identified!             //  1 - 999 Error Code!
        APP_CONVERSION_ERROR,                                                               //  2
        APP_I2C_DEVICE_UNREACHABLE,                                                         //  3
        APP_INVALID_COMMAND_ERROR,                                                          //  4
        APP_INVALID_PARAMETER_ERROR,                                                        //  5
        APP_UNIMPLEMENTED_COMMAND_ERROR,                                                    //  6
        APP_POWER_SUPPLY_NOT_ENABLED_ERROR,                                                 //  7
        APP_INVALID_POWER_SUPPLY_ERROR,                                                     //  8
        APP_INVALID_RESPONSE_MODE_ERROR,                                                    //  9
        APP_INVALID_G2_CONTROLLER_ERROR,                                                    // 10
        APP_POWER_SUPPLY_EXCEEDED_CRITICAL_VOLTAGE,                                         // 11
        APP_POWER_SUPPLY_VOLTAGE_OUT_OF_RANGE_ERROR,                                        // 12
        APP_POWER_SUPPLY_EXCEEDED_CRITICAL_CURRENT,                                         // 13
        APP_POWER_SUPPLY_CURRENT_OUT_OF_RANGE_ERROR,                                        // 14
        APP_EEPROM_READ_ERROR,                                                              // 15
        APP_EEPROM_WRITE_ERROR,                                                             // 16
        APP_EEPROM_FORMAT_ERROR,                                                            // 17

        //  These are non-error responses.
        APP_MESSAGE_OK                     = 1000,                                          // 1000
        
    };
    std::string responseToString(ResponseCode error);

}

#endif
