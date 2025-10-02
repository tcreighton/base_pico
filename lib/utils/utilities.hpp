#pragma once

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <chrono>
#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision
#include <numeric>
#include <vector>
#include <cmath>
#include <cstring>
#include <pico/time.h>

#include "packed-datetime.hpp"


/**
 * This file holds useful macros and declarations. Also global constants.
 */
namespace CSutilities {

    #define COMPANY_NAME  "Creighton Scientific, Inc." // This needs to be a #define, not constexpr
    #define PRODUCT_NAME  "Focus Rack 50-500"
  //  constexpr std::string_view UTITITIES_getEncodedPicoBoardId_STRING = ("0.3." + __DATE__ + " " + __TIME__);

    constexpr uint8_t FIRMWARE_MAJOR_VERSION = 0x00;
    constexpr uint8_t FIRMWARE_MINOR_VERSION = 0x09;
#if defined BOARD_FOCUS500_REV_0
    constexpr std::string CIRCUIT_BOARD_REVISION = "0";
#elif defined BOARD_FOCUS500_REV_A
    constexpr std::string CIRCUIT_BOARD_REVISION = "A";
#elif defined BOARD_FOCUS500_REV_B
    constexpr std::string CIRCUIT_BOARD_REVISION = "B";
#endif

//    std::string getEncodedPicoBoardId ();
    std::string getPicoBoardId ();
    std::string getFirmwareVersion ();

    inline std::string getCompanyName () {return COMPANY_NAME;}
    inline std::string getProductName () {return PRODUCT_NAME;}



    struct ProductInfo {
        std::string         companyName             = COMPANY_NAME;
        std::string         productName             = PRODUCT_NAME;
        uint8_t             majorVersion            = FIRMWARE_MAJOR_VERSION;
        uint8_t             minorVersion            = FIRMWARE_MINOR_VERSION;
        PackedDateTime_t    buildNumber             = PackedDateTime::getPackedBuildDateTime();// encoded build date
        std::string         circuitBoardRevision    = CSboard::CURRENT_BOARD_NAME;
        std::string         encodedPicoBoardId      = getPicoBoardId();// AKA product serial number
    };
    using ProductInfo_t = ProductInfo;  // Not really necessary.

    ProductInfo_t getProductInfo ();

    constexpr bool SIMULATE_ADC = false;

    // Lambda functions to make it easier to & together multiple methods returning true or false.
    inline auto allTrue = [](bool& res, const bool val) {res &= val;};
    inline auto allFalse = [](bool& res, const bool val) {res |= val;};


    using Milliseconds_t = uint32_t;  // Just making it clear what we are dealing with.

    [[nodiscard]] inline Milliseconds_t getNow () {
        return  to_ms_since_boot(get_absolute_time());
    }

    /**
     *
     * @param khz
     * @return khz converted to hz
     */
    inline uint khz_to_hz (const uint khz) {
        return khz * 1000;
    }

    /**
     * @brief Calculate the log of x for base 'base'
     * @param x
     * @param base
     * @return log base base of x
     */
    inline double logNBaseOfX (const double x, const double base) {
        return log(x) / log(base);
    }

    /**
     * @brief Convenience method for those of us who forget the built-in support for log10
     * @param x
     * @return log base 10 of x
     */
    inline double logBase10 (const double x) {
        return log10(x);
    }

    /**
     * @brief Convenience method for those of us who forget the built-in support for ln
     * @param x
     * @return natural log of x
     */
    inline double ln (const double x) {
        return log(x);
    }

// !! Pay attention to bit field structures! The first field defined (on top) is the low-order field.
// As an example, check out test-bitfields.hpp.


    struct ByteStruct {
        uint8_t lowByte = 0;
        uint8_t highByte = 0;
    };

    union UInt16ToBytes {
        ByteStruct bytes;
        uint16_t data = 0;
    };

    using UInt16ToBytes_t = UInt16ToBytes;

    union UInt32ToBytes { // This does not address endian concerns!
        uint8_t bytes[4];
        uint32_t data = 0;
    };

    inline bool isLittleEndian() {
        return (true);
    }


/**
 * @brief Returns the lower byte of a 16-bit integer.
 * This is perhaps more convenient in some cases than getTwoBytes.
 * It also works regardless of endianness.
 * @param uint16
 * @return
 */
/*
    uint8_t getLowerByte (uint16_t uint16) {
        auto result = static_cast<uint8_t>(uint16  & 0x00ff);

        return result;
    }
*/

    inline uint8_t getLowerByte(uint16_t uint16) {
        return static_cast<uint8_t>(uint16  & 0x00ff);
    }

/**
 * @brief Returns the upper byte of a 16-bit integer.
 * This is perhaps more convenient in some cases than getTwoBytes.
 * It also works regardless of endianness.
 * @param int16
 * @return
 */
    inline int8_t getLowerByte(const int16_t int16) {
        return static_cast<int8_t>(getLowerByte(static_cast<uint16_t>(int16)));
    }

    inline uint8_t getUpperByte(uint16_t uint16) {
        return static_cast<uint8_t>((uint16 >> 8) & 0xff);
    }

/*
    int8_t getUpperByte (int16_t int16) {
        auto value = static_cast<int8_t>(getUpperByte(static_cast<uint16_t>(int16)));


        return value;
    }
*/
    inline int8_t getUpperByte(int16_t int16) {
        return static_cast<int8_t>(getUpperByte(static_cast<uint16_t>(int16)));
    }

    inline uint16_t composeUint16 (uint8_t highByte, uint8_t lowByte) {
        return (static_cast<uint16_t>(highByte) << 8) | static_cast<uint16_t>(lowByte);
    }

    inline int16_t composeInt16(int8_t highByte, int8_t lowByte) {
        return static_cast<int16_t>(composeUint16(static_cast<uint8_t>(highByte),
                                                  static_cast<uint8_t>(lowByte)));
    }

//    UInt16ToBytes_t getTwoBytes(uint16_t source);
    inline UInt16ToBytes_t getTwoBytes (uint16_t source) {

        UInt16ToBytes combo = {0};
        combo.bytes.highByte   = ((source >> 8) & 0xff);
        combo.bytes.lowByte    = (source & 0xff);

        return combo;
    }

// Network order is Big-Endian.

/**
 * @brief Copies the data from source to destination. If needed, the bytes are swapped.
 * networkData must have 2 bytes available!
 *
 *
 * @param sourceData
 * @param networkData
 */

    inline void localUint16ToNetworkByteOrder (const uint16_t sourceData, uint8_t* networkData) {
        networkData[0] = getUpperByte(sourceData);
        networkData[1] = getLowerByte(sourceData);
    }

    inline void localInt16ToNetworkByteOrder (const int16_t sourceData, int8_t* networkData) {
        localUint16ToNetworkByteOrder(sourceData, reinterpret_cast<uint8_t *>(networkData));
    }

    inline uint16_t networkByteOrderToLocalUint16 (const uint8_t *sourceData) {
        return composeUint16(sourceData[0], sourceData[1]);
    }

    inline int16_t networkByteOrderToLocalInt16 (const int8_t *sourceData) {
        return composeInt16(sourceData[0], sourceData[1]);
    }

/**
 * @brief - This is where we define some convenience methods.
 */

// Just use std::hex from <iomanip>?
/**
 * @brief Takes a type (int, etc) and converts data of that type to string hex with fixed width.
 * @tparam T the type of the value to convert
 * @param val the value to convert
 * @param width sets the width of the result. 2 x the width of the type
 * @return A string with hex characters.
 */
    template<typename T>
    std::string int_to_hex(T val, const size_t width = sizeof(T) * 2) {
        std::stringstream ss;

        //TODO: Figure out why int_to_hex of uint16_t value 3 has so wide a display.
        ss << std::setfill('0') << std::setw(static_cast<int>(width)) << std::hex << (val | 0);
//        ss << std::setfill('0') << std::setw(static_cast<int>(width)) << std::hex << val;
        return ss.str();
    }


    inline std::string byteArrayToHexString(const char data[], const size_t length, const bool withSpaces = false) {
        std::stringstream ss;
        for (size_t i = 0; i < length; i++) {
            ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);
            if (withSpaces) {
                ss << " ";
            }
        }
        return ss.str();
    }
// Change to :
/*
    template< typename T >
    std::string int_to_hex( T i )
    {
        std::stringstream stream;
        stream << "0x"
               << std::setfill ('0') << std::setw(sizeof(T)*2)
               << std::hex << i;
        return stream.str();
    }
*/

/**
 * @brief This just prepends 0x in front of the return from int_to_hex.
 * That way you get it any way you want it!
 *
 */
    template<typename T>
    inline std::string int_to_hex_0x(T val) {
        return "0x" + int_to_hex(val);
    }


    template<typename T>
    struct LinearRegressionResult {
        T m;    // slope
        T b;    // y intercept
    };

    /**
     * @brief Template for computing linear regression.
     * @param yData A vector with the y values to use for line fit.
     *
     * @return
     */
    template<typename T>
    LinearRegressionResult<T> getLinearFit(const std::vector<T> &yData) {
        size_t n = yData.size();
        std::vector<T> xData(n);
        std::iota(xData.begin(), xData.end(), static_cast<T>(0));   // Generate x values

        // Calculate sums for slope and intercept.
        T sumX = std::accumulate(xData.begin(), xData.end(),
                                 static_cast<T>(0));    // accumulate (sum) values of x from 0
        T sumY = std::accumulate(yData.begin(), yData.end(),
                                 static_cast<T>(0));    // accumulate (sum) values of y from 0
        T sumXX = std::inner_product(xData.begin(), xData.end(), xData.begin(), static_cast<T>(0)); // sum x * x
        T sumXY = std::inner_product(xData.begin(), xData.end(), yData.begin(), static_cast<T>(0)); // sum x * y

        // Calculate slope (m) and intercept (b)
        T denominator = n * sumXX - sumX * sumY;
        T slope = (n * sumXY - sumX * sumY) / denominator;
        T intercept = (sumY * sumXX - sumX * sumXY) / denominator;

        // Now create the result struct and return it.

        LinearRegressionResult<T> result = {slope, intercept};

        return result;
    }


    inline float roundToTwoDecimalPlaces (float value) {
        return std::round(value * 100) / 100;
    }

    inline float roundToFourDecimalPlaces (float value) {
        return std::round(value * 10000) / 10000;
    }

    inline float roundToSixDecimalPlaces (float value) {
        return std::round(value * 1000000) / 1000000;
    }

    inline float roundToXDecimalPlaces (const float value, const uint8_t x) {
        float y;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(x) << value;
        ss >> y;
        return y;
    }

    inline double roundToXDecimalPlaces (const double value, const uint8_t x) {
        double y;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(x) << value;
        ss >> y;
        return y;
    }

    inline std::string formatFloatForDisplay (const float volts, const int precision) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << volts;
        std::cout.unsetf(std::ios::fixed);      // Unset the fixed flag
        std::cout.precision(6);                 // Reset the precision
        return oss.str();
    }
//--------------------------------------------------------------------------------------------------------------
    // Internally we track volts. Externally, it's sometimes kV
    // The conversion happens in handling i/o to the console.
    // yV / KILOVOLTS = y/1000 kV
    // ykV * KILOVOLTS = y1000 V

    // Internally  we tack milliamps. Externally Amps, milliamps, microamps.

    constexpr float VOLTS_PER_KILOVOLT = 1000.0f;
    constexpr float KILOVOLTS_PER_VOLT = 1.0f / VOLTS_PER_KILOVOLT;
    constexpr float MILLIVOLTS_PER_VOLT = 1000.0f;
    constexpr float VOLTS_PER_MILLIVOLT = 1.0f / MILLIVOLTS_PER_VOLT;
    constexpr float MICROVOLTS_PER_MILLIVOLT = 1000.0f;
    constexpr float MILLIVOLTS_PER_MICROVOLT = 1.0f / MICROVOLTS_PER_MILLIVOLT;

    constexpr float MILLIAMPS_PER_AMP = 1000.0f;
    constexpr float MICROAMPS_PER_MILLIAMP = 1000.0f;
    constexpr float AMPS_PER_MILLIAMP = 1.0f / MILLIAMPS_PER_AMP;
    constexpr float MILLIAMPS_PER_MICROAMP = 1.0f / 1000.0f;
    constexpr float MICROAMPS_PER_AMP = MICROAMPS_PER_MILLIAMP * MILLIAMPS_PER_AMP;
    constexpr float AMPS_PER_MICROAMP = 1.0f / MICROAMPS_PER_AMP;

    inline float voltsToKvolts (const float volts) {
        return volts * KILOVOLTS_PER_VOLT;
    }
    inline float kVoltsToVolts (const float kVolts) {
        return kVolts * VOLTS_PER_KILOVOLT;
    }
    inline float mVToVolts (const float mV) {
        return mV * VOLTS_PER_MILLIVOLT;
    }
    inline float voltsToMv (const float volts) {
        return volts * MILLIVOLTS_PER_VOLT;
    }
    inline float milliAmpsToMicroAmps (const float milliAmps) {
        return milliAmps * MICROAMPS_PER_MILLIAMP;
    }
    inline float milliAmpsToAmps (const float milliAmps) {
        return milliAmps * AMPS_PER_MILLIAMP;
    }
    inline float ampsToMilliAmps (const float amps) {
        return amps * MILLIAMPS_PER_AMP;
    }
    inline float microAmpsToMilliAmps (const float microAmps) {
        return microAmps * MILLIAMPS_PER_MICROAMP;
    }
    inline float microAmpsToAmps (const float microAmps) {
        return microAmps * AMPS_PER_MICROAMP;
    }
    inline float ampsToMicroAmps (const float amps) {
        return amps * MICROAMPS_PER_AMP;
    }
    inline float convertVoltage (const float volts, const float from, const float to) {
        return (volts * to) / from;
    }


    inline std::string toString(const bool value) {
        return value ? "true" : "false";
    }

    inline bool areEqual (const float a, const float b, const float c) {
        return a == b && b == c;
    }

    inline std::string_view getBuildTime () {
        return __TIME__;
    }

    inline std::string_view getBuildDate () {
        return __DATE__;
    }

    std::string replaceAllOccurrences (std::string str, const std::string& from, const std::string& to);

    inline std::string replaceAllSpaces (const std::string &str) {
        return replaceAllOccurrences(str, " ", "");
    }

}   // namespace CSutilities

#endif  // UTILITIES_HPP_

