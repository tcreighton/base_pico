
#include <unordered_map>
#include "packed-datetime.hpp"
#include "pystring.h"
#include "utilities.hpp"

namespace CScore {


    PackedDateTime::PackedDateTime( const uint16_t year,
                                    const uint8_t month,
                                    const uint8_t day,
                                    const uint8_t hour,
                                    const uint8_t minutes,
                                    const uint8_t seconds) {

        if (const auto retCode = checkValidity(year, month, day, hour, minutes);
            (status_ = PackedDateTimeStatus::OK) == retCode) {
            unpackedDateTime_.year = year;
            unpackedDateTime_.month = month;
            unpackedDateTime_.day = day;
            unpackedDateTime_.hours = hour;
            unpackedDateTime_.minutes = minutes;
            unpackedDateTime_.seconds = seconds;
        }
    }

    PackedDateTime::PackedDateTime(const PackedDateTime_t packedDateTime) {
        if (const auto retCode = checkValidity(packedDateTime);
            (status_ = PackedDateTimeStatus::OK) == retCode) {
            PackedDateTimeUnion dtUnion;
            dtUnion.packedValue = packedDateTime;
            unpackedDateTime_.year      = dtUnion.packedDateTime.year + YEAR_BIAS;
            unpackedDateTime_.month     = dtUnion.packedDateTime.month;
            unpackedDateTime_.day       = dtUnion.packedDateTime.day;
            unpackedDateTime_.hours     = dtUnion.packedDateTime.hour;
            unpackedDateTime_.minutes   = dtUnion.packedDateTime.minute;
            unpackedDateTime_.seconds   = 0;
        }

    }

    PackedDateTime_t PackedDateTime::getPackedDateTime() const {
        PackedDateTimeUnion dtUnion;
        dtUnion.packedDateTime.year = unpackedDateTime_.year - YEAR_BIAS;
        dtUnion.packedDateTime.month = unpackedDateTime_.month;
        dtUnion.packedDateTime.day = unpackedDateTime_.day;
        dtUnion.packedDateTime.hour = unpackedDateTime_.hours;
        dtUnion.packedDateTime.minute = unpackedDateTime_.minutes;
        return dtUnion.packedValue;
    }

    PackedDateTime::DateTimeStruct PackedDateTime::getBuildDateTime() {
        DateTimeStruct dt{};    // Place to put the build date and time
        const std::string buildDate = pystring::lower(__DATE__);
        const std::string buildTime = pystring::lower(__TIME__);
        std::vector<std::string> dateTokens;
        std::vector<std::string> timeTokens;
        pystring::split(buildDate, dateTokens);
        pystring::split(buildTime, timeTokens, ":");
        std::unordered_map<std::string, int> months = {
            {"jan", 1}, {"feb", 2}, {"mar", 3}, {"apr", 4},
            {"may", 5}, {"jun", 6}, {"jul", 7}, {"aug", 8},
            {"sep", 9}, {"oct", 10}, {"nov", 11}, {"dec", 12}
        };

        dt.year = std::stoi(dateTokens[2]);
        dt.month = months[dateTokens[0]];       // look up the month by name
        dt.day = std::stoi(dateTokens[1]);
        dt.hours = std::stoi(timeTokens[0]);
        dt.minutes = std::stoi(timeTokens[1]);
        dt.seconds = std::stoi(timeTokens[2]);

        return dt;
    }

    PackedDateTime_t PackedDateTime::getPackedBuildDateTime() {
        auto [seconds, minutes, hours, day, month, year]
                                                            = getBuildDateTime ();
        const auto packed = PackedDateTime( year,
                                            month,
                                            day,
                                            hours,
                                            minutes,
                                            seconds);

        return packed.getPackedDateTime();
    }

    PackedDateTime_t PackedDateTime::getPackedBuildDate() {
        auto [seconds, minutes, hours, day, month, year]
                                                            = getBuildDateTime ();
        const auto packed = PackedDateTime( year,
                                            month,
                                            day);

        return packed.getPackedDateTime();
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::checkValidity (const uint16_t year,
                                                                        const uint8_t month,
                                                                        const uint8_t day,
                                                                        const uint8_t hour,
                                                                        const uint8_t minutes,
                                                                        const uint8_t seconds) {
        auto retCode = PackedDateTimeStatus::OK;

        if (PackedDateTimeStatus::OK == (retCode = validateDate(year, month, day))) {
            retCode = validateTime(hour, minutes, seconds);
        }

        return retCode;
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::validateYear(const uint16_t year) {
        auto retCode = PackedDateTimeStatus::OK;

        if (year < YEAR_BIAS) {
            retCode = PackedDateTimeStatus::INVALID_YEAR_BEFORE_BIAS;
        } else if (year > MAX_YEAR) {
            retCode = PackedDateTimeStatus::INVALID_YEAR_AFTER_BIAS;
        }

        return retCode;
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::validateMonth(const uint8_t month) {
        auto retCode = PackedDateTimeStatus::OK;

        if (month < 1) {
            retCode = PackedDateTimeStatus::INVALID_MONTH_BELOW_ONE;
        } else if (month > 12) {
            retCode = PackedDateTimeStatus::INVALID_MONTH_ABOVE_TWELVE;
        }

        return retCode;
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::validateDate(  const uint16_t year,
                                                                        const uint8_t month,
                                                                        const uint8_t day) {

        auto retCode = PackedDateTimeStatus::OK;

        if (PackedDateTimeStatus::OK != (retCode = validateYear(year))) {
            return retCode; // Quick fail
        }

        if (PackedDateTimeStatus::OK != (retCode = validateMonth(month))) {
            return retCode; // Quick fail
        }

        if (day < 1) {
            retCode = PackedDateTimeStatus::INVALID_DAY_BELOW_ONE;
        } else {
            switch (month) {

                case 9:     // Thirty days hath September, April, June, and November
                case 4:
                case 6:
                case 11:
                    if (day > 30) {
                        retCode = PackedDateTimeStatus::INVALID_DAY_FOR_MONTH;
                    }
                    break;
                case 1:     // All the rest have 31
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    if (day > 31) {
                        retCode = PackedDateTimeStatus::INVALID_DAY_FOR_MONTH;
                    }
                    break;
                default:    // Except February...

                    if (day == 29) {
                        // Check for leap year
                        if (!(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)) {
                            retCode = PackedDateTimeStatus::INVALID_DAY_FOR_MONTH;
                        }
                    } else if (day > 28) {
                        retCode = PackedDateTimeStatus::INVALID_DAY_FOR_MONTH;
                    }
                    break;
            }
        }
        return retCode;
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::validateTime(  const uint8_t hour,
                                                                        const uint8_t minutes,
                                                                        const uint8_t seconds) {
        auto retCode = PackedDateTimeStatus::OK;

        if (hour > 23) {
            retCode = PackedDateTimeStatus::INVALID_HOUR_ABOVE_TWENTY_THREE;
        } else if (minutes > 59) {
            retCode = PackedDateTimeStatus::INVALID_MINUTES_ABOVE_FIFTY_NINE;
        } else if (seconds > 59) {
            retCode = PackedDateTimeStatus::INVALID_SECONDS_ABOVE_FIFTY_NINE;
        }

        return retCode;
    }

    PackedDateTime::PackedDateTimeStatus PackedDateTime::checkValidity(const PackedDateTime_t packedDateTime) {

        PackedDateTimeUnion dtUnion;
        dtUnion.packedValue = packedDateTime;
        DateTimeStruct dt{};
        dt.year = dtUnion.packedDateTime.year + YEAR_BIAS;
        dt.month = dtUnion.packedDateTime.month;
        dt.day = dtUnion.packedDateTime.day;
        dt.hours = dtUnion.packedDateTime.hour;
        dt.minutes = dtUnion.packedDateTime.minute;
        return checkValidity(dt.year, dt.month, dt.day, dt.hours, dt.minutes);

    }
}
