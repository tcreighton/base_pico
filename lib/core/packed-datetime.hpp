
#pragma once
#ifndef PACKED_DATE_HPP_
#define PACKED_DATE_HPP_

#include <cstdint>

namespace CScommands {

    using PackedDateTime_t = uint32_t;

    class PackedDateTime {

    public:


        PackedDateTime (uint16_t year,
                        uint8_t month,
                        uint8_t day,
                        uint8_t hour = 0,
                        uint8_t minutes = 0,
                        uint8_t seconds = 0);

        explicit PackedDateTime (PackedDateTime_t packedDateTime);

        ~PackedDateTime () = default;

        enum class PackedDateTimeStatus {
            OK,
            INVALID_YEAR_BEFORE_BIAS,   // nothing allowed prior to 2024
            INVALID_YEAR_AFTER_BIAS,    // nothing after 2024 + 0x7f (2151)
            INVALID_MONTH_BELOW_ONE,    // months are 1 to 12.
            INVALID_MONTH_ABOVE_TWELVE, // months are 1 to 12.
            INVALID_DAY_BELOW_ONE,      // days are 1 to 31 depending on month.
            INVALID_DAY_FOR_MONTH,      // days are 1 to 31 depending on month.
            INVALID_HOUR_ABOVE_TWENTY_THREE, // hours are 0 to 23.
            INVALID_MINUTES_ABOVE_FIFTY_NINE, // minutes are 0 to 59.
            INVALID_SECONDS_ABOVE_FIFTY_NINE // seconds are 0 to 59.
        };

        struct DateTimeStruct {
            uint8_t seconds;    // Seconds are ignored for packed date. But this can be used outside of packed date time
            uint8_t minutes;
            uint8_t hours;
            uint8_t day;
            uint8_t month;
            uint16_t year;
        };

        [[nodiscard]] PackedDateTimeStatus getStatus () const {return status_;}
        [[nodiscard]] PackedDateTime_t getPackedDateTime () const;
        [[nodiscard]] DateTimeStruct getUnpackedDateTime () const {return unpackedDateTime_;}

        static DateTimeStruct getBuildDateTime ();
        static PackedDateTime_t getPackedBuildDateTime ();
        static PackedDateTime_t getPackedBuildDate ();  // zeros out hours, minutes, seconds.
        static DateTimeStruct getUnpackedDateTime (const PackedDateTime_t packedDateTime) {
            return PackedDateTime(packedDateTime).getUnpackedDateTime();
        }

        [[nodiscard]] uint16_t getYear () const {return unpackedDateTime_.year;}
        [[nodiscard]] uint8_t getMonth () const {return unpackedDateTime_.month;}
        [[nodiscard]] uint8_t getDay () const {return unpackedDateTime_.day;}
        [[nodiscard]] uint8_t getHour () const {return unpackedDateTime_.hours;}
        [[nodiscard]] uint8_t getMinutes () const {return unpackedDateTime_.minutes;}
        [[nodiscard]] uint8_t getSeconds () const {return unpackedDateTime_.seconds;}
        /**
         * @brief Checks if year, month, day are legit and can be represented by a biased date.
         * @param year must be between 2024 and 2151, inclusive
         * @param month must be between 1 and 12, inclusive.
         * @param day must be valid for the month and year (leap year.)
         * @param hour must be < 24
         * @param minutes must be < 60
         * @param seconds must be < 60
         * @return OK if all is well.
         */
        static PackedDateTimeStatus checkValidity ( uint16_t year,
                                                    uint8_t month,
                                                    uint8_t day,
                                                    uint8_t hour,
                                                    uint8_t minutes,
                                                    uint8_t seconds = 0);

        /**
         * @brief Checks if the parameter produces a valid date when decoded.
         * @param packedDateTime is an unsigned 32-bit integer that represents a struct of type BiasedDateStruct.
         * @return OK if the parameter value can be decoded to a valid date.
         */
        static PackedDateTimeStatus checkValidity (PackedDateTime_t packedDateTime);

        static constexpr uint16_t YEAR_BIAS = 2025;    // this is the base year: nothing earlier.
        static constexpr uint16_t MAX_YEAR = YEAR_BIAS + 0x7f; // keep the biased year to 7 bits

    private:

        struct PackedDateTimeStruct { // No support for seconds!
            uint8_t minute: 6 = 0;  // straight-up
            uint8_t hour:   5 = 0;  // midnight
            uint8_t day:    5 = 1;  // 1st
            uint8_t month:  4 = 1;  // January
            uint8_t year:   2 = 0;  // 2025 - 2028
        };


        union PackedDateTimeUnion {
            PackedDateTimeStruct    packedDateTime{0,0,1,1,0}; // January 1, 2025 @ midnight
            PackedDateTime_t        packedValue;    // uint32_t
        };


        static PackedDateTimeStatus validateYear    (uint16_t year);
        static PackedDateTimeStatus validateMonth   (uint8_t month);
        static PackedDateTimeStatus validateDate    (uint16_t year, uint8_t month, uint8_t day);
        static PackedDateTimeStatus validateTime    (uint8_t hour, uint8_t minutes, uint8_t seconds = 0);


        PackedDateTimeStatus status_ = PackedDateTimeStatus::OK;

        DateTimeStruct unpackedDateTime_{0,0,0,1,1, YEAR_BIAS};
    };
}

#endif  // PACKED_DATE_HPP_
