
#include "errors.hpp"

namespace CSerrors {
    std::string responseToString(const ResponseCode error) {
        switch (error) {
            case ResponseCode::PICO_ERROR_RESOURCE_IN_USE_ERROR:            return "Resource in use";          // -21
            case ResponseCode::PICO_ERROR_VERSION_MISMATCH_ERROR:           return "Version mismatch";         // -20
            case ResponseCode::PICO_ERROR_LOCK_REQUIRED_ERROR:              return "Lock required";            // -19
            case ResponseCode::PICO_ERROR_UNSUPPORTED_MODIFICATION_ERROR:   return "Unsupported modification"; // -18
            case ResponseCode::PICO_ERROR_NOT_FOUND_ERROR:                  return "Not found";                // -17
            case ResponseCode::PICO_ERROR_INVALID_DATA_ERROR:               return "Invalid data";             // -16
            case ResponseCode::PICO_ERROR_MODIFIED_DATA_ERROR:              return "Modified data";            // -15
            case ResponseCode::PICO_ERROR_PRECONDITION_NOT_MET_ERROR:       return "Precondition not met";     // -14
            case ResponseCode::PICO_ERROR_BUFFER_TOO_SMALL_ERROR:           return "Buffer too small";         // -13
            case ResponseCode::PICO_ERROR_INVALID_STATE_ERROR:              return "Invalid state";            // -12
            case ResponseCode::PICO_ERROR_BAD_ALIGNMENT_ERROR:              return "Bad alignment";            // -11
            case ResponseCode::PICO_ERROR_INVALID_ADDRESS_ERROR:            return "Invalid address";          // -10
            case ResponseCode::PICO_INSUFFICIENT_RESOURCES_ERROR:           return "Insufficient resources";   //  -9
            case ResponseCode::PICO_CONNECT_FAILED_ERROR:                   return "Connect failed";           //  -8
            case ResponseCode::PICO_AUTHENTICATION_ERROR:                   return "Authentication failed";    //  -7
            case ResponseCode::PICO_IO_ERROR:                               return "IO error";                 //  -6
            case ResponseCode::PICO_INVALID_ARGUMENT_ERROR:                 return "Invalid argument";         //  -5
            case ResponseCode::PICO_NOT_PERMITTED_ERROR:                    return "Not permitted";            //  -4
            case ResponseCode::PICO_NO_DATA_ERROR:                          return "No data";                  //  -3
            case ResponseCode::PICO_TIMEOUT_ERROR:                          return "Timeout";                  //  -2
            case ResponseCode::PICO_GENERIC_ERROR:                          return "Generic error";            //  -1
            case ResponseCode::NOT_AN_ERROR:                                return "Not an error";             //   0
            // Start of Application error codes - vs. pico generated codes
            default:
            case ResponseCode::APP_UNKNOWN_ERROR:                           return "Unknown error";           //   01
            case ResponseCode::APP_CONVERSION_ERROR:                        return "Conversion error";        //   02
            case ResponseCode::APP_I2C_DEVICE_UNREACHABLE:                  return "I2C device unreachable";  //   03
            case ResponseCode::APP_INVALID_COMMAND_ERROR:                   return "Invalid command";         //   04
            case ResponseCode::APP_INVALID_PARAMETER_ERROR:                 return "Invalid parameter";       //   05
            case ResponseCode::APP_UNIMPLEMENTED_COMMAND_ERROR:             return "Unimplemented command";   //   06
            case ResponseCode::APP_POWER_SUPPLY_NOT_ENABLED_ERROR:          return "Power supply not enabled";//   07
            case ResponseCode::APP_INVALID_POWER_SUPPLY_ERROR:              return "Invalid power supply";    //   08
            case ResponseCode::APP_INVALID_RESPONSE_MODE_ERROR:             return "Invalid mode";            //   09
            case ResponseCode::APP_INVALID_G2_CONTROLLER_ERROR:             return "Invalid G2 controller";   //   10
            case ResponseCode::APP_POWER_SUPPLY_EXCEEDED_CRITICAL_VOLTAGE:  return "Power supply exceeded critical voltage"; // 11
            case ResponseCode::APP_POWER_SUPPLY_VOLTAGE_OUT_OF_RANGE_ERROR: return "Power supply voltage out of range"; // 12
            case ResponseCode::APP_POWER_SUPPLY_CURRENT_OUT_OF_RANGE_ERROR: return "Power supply current out of range"; // 13
            case ResponseCode::APP_EEPROM_READ_ERROR:                       return "Can't read EEPROM";       //   14
            case ResponseCode::APP_EEPROM_WRITE_ERROR:                      return "Can't write to EEPROM";   //   15
            case ResponseCode::APP_EEPROM_FORMAT_ERROR:                     return "Can't format EEPROM";     //   16

            case ResponseCode::APP_MESSAGE_OK:                              return "OK";                      // 1000
            }
    }
}
