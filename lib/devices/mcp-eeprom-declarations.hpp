
#pragma once
#ifndef EEPROM_DECLARATIONS_HPP_
#define EEPROM_DECLARATIONS_HPP_

#include <algorithm>
#include <cstdint>


namespace CSdevices {


    constexpr uint8_t I2C_EEPROM_ADDRESS = 0x50;       // For multiple eeproms on same controller see EEPromID.
    constexpr uint8_t CONTROL_I2C_DEVICE = 0b101;       // This will combine with the chip select bits: 101xabc
    constexpr uint8_t MCP_EEPROM_PAGE_COUNT = 128;     // 128 pages per eeprom. pages 0-127
    constexpr uint8_t MCP_EEPROM_PAGE_SIZE = 32;       // 32 byte pages

    // The ControlBits struct basically is the i2cAddress.
    // 0x50 is the base address. The device address is always 7 bits.
    // MSB is always 0, though I think it's always ignored.
    // Take 0x50 and or in the three chip_select bits and you have 7 bits:
    // 0b01010ABC where ABC are select bits.
    // This is weird because the bit layout makes it look like things are shifted left.
    // Most people will do something like controlByte = (0x50 | chipSelect); where chipSelect is 0-7
    // To use the bit fields it looks a little different:
    // ControlByte_t foo{0}; foo.i2cDevice = 0b101; foo.chipSelect = EEPromIDToNumber(eePromId) & 0x07;
    struct ControlBits {    // For the first (0th) device: 0x50
        uint8_t chipSelect  : 3 = 0;    // A0, A1, A2 - Chip select. Up to 8 chips per i2c bus.
        uint8_t unused      : 1 = 0;    // This is unused.
        uint8_t i2cDevice   : 3 = 0;    // Device address is 0x50. Need a different? Too bad - switch controller.
        uint8_t unused_msb  : 1 = 0;    // This is unused. REMINDER: MSB is always last in a struct. LSB is always first
    };

    union ControlByte {
        ControlBits bits;
        uint8_t     byte = 0;
    };

    using ControlByte_t = ControlByte;

    /**
     * @brief EEPromId identifies a particular EEProm Chip. This is also called a block (of addresses.)
     * A single I2C bus can have just eight devices defined on it.
     * This software can support up to 16 devices, requiring them to be split between I2C busses.
     * However, some minor code changes are needed to enable each device beyond the first.
     * The device is identified by I2C address 0x1010 and EEPromId. And so the software needs to know
     * which bus the target device is on. To facilitate this, we use four bits in the EEPromId. The
     * high-order bit indicates the bus or controller.
     * At the end of the day, the eeprom support software design is object oriented and so we pretty
     * much ignore the upper byte. Each device is manifested by an extension of the Mcp24Lc32At.
     * The extension is named in a way to identify which device it is. And the constructor requires
     * a reference to CsI2C instance. The eeprom class declaration is like: class Eeprom0 final : public Mcp24Lc32At
     */

    // These IDs correspond with the A0, A1, A2 address bits for any given i2c controller.
    enum class EEPromId : uint8_t {
        EEPROM_0    = 0,    // 0b0000
        EEPROM_1,           // 0b0001
        EEPROM_2,           // 0b0010
        EEPROM_3,           // 0b0011
        EEPROM_4,           // 0b0100
        EEPROM_5,           // 0b0101
        EEPROM_6,           // 0b0110
        EEPROM_7,           // 0b0111
        /* Focus500 only supports one set. Devices 0 - 7. To support the other 8, you need to use two i2c controllers.
        EEPROM_8,           // 0b1000
        EEPROM_9,           // 0b1001
        EEPROM_A,           // 0b1010
        EEPROM_B,           // 0b1011
        EEPROM_C,           // 0b1100
        EEPROM_D,           // 0b1101
        EEPROM_E,           // 0b1110
        EEPROM_F            // 0b1111
    */
    };
    constexpr uint8_t EEPromIDToNumber (EEPromId eePromId) {
        return static_cast<uint8_t>(eePromId);
    }
    constexpr uint8_t MIN_PROM_ID = EEPromIDToNumber(EEPromId::EEPROM_0);
    constexpr uint8_t MAX_PROM_ID = EEPromIDToNumber(EEPromId::EEPROM_7);
    constexpr EEPromId NumberToEEPromID (const uint8_t promIdNumber) {
        return static_cast<EEPromId>(std::min(MAX_PROM_ID, promIdNumber));
    }
    // This controls how many devices are actually available in a given implementation
    constexpr auto MAX_EEPROM_ID = EEPromId::EEPROM_0; // If you add another, fix this.


    // Page IDs can have the same value, but they better be used in separate memory devices (ie. different EEPROMs.)
    enum class PageId : uint8_t {
        PAGE_000,PAGE_001,PAGE_002,PAGE_003,PAGE_004,PAGE_005,PAGE_006,PAGE_007,
        PAGE_008,PAGE_009,PAGE_010,PAGE_011,PAGE_012,PAGE_013,PAGE_014,PAGE_015,
        PAGE_016,PAGE_017,PAGE_018,PAGE_019,PAGE_020,PAGE_021,PAGE_022,PAGE_023,
        PAGE_024,PAGE_025,PAGE_026,PAGE_027,PAGE_028,PAGE_029,PAGE_030,PAGE_031,
        PAGE_032,PAGE_033,PAGE_034,PAGE_035,PAGE_036,PAGE_037,PAGE_038,PAGE_039,
        PAGE_040,PAGE_041,PAGE_042,PAGE_043,PAGE_044,PAGE_045,PAGE_046,PAGE_047,
        PAGE_048,PAGE_049,PAGE_050,PAGE_051,PAGE_052,PAGE_053,PAGE_054,PAGE_055,
        PAGE_056,PAGE_057,PAGE_058,PAGE_059,PAGE_060,PAGE_061,PAGE_062,PAGE_063,
        PAGE_064,PAGE_065,PAGE_066,PAGE_067,PAGE_068,PAGE_069,PAGE_070,PAGE_071,
        PAGE_072,PAGE_073,PAGE_074,PAGE_075,PAGE_076,PAGE_077,PAGE_078,PAGE_079,
        PAGE_080,PAGE_081,PAGE_082,PAGE_083,PAGE_084,PAGE_085,PAGE_086,PAGE_087,
        PAGE_088,PAGE_089,PAGE_090,PAGE_091,PAGE_092,PAGE_093,PAGE_094,PAGE_095,
        PAGE_096,PAGE_097,PAGE_098,PAGE_099,PAGE_100,PAGE_101,PAGE_102,PAGE_103,
        PAGE_104,PAGE_105,PAGE_106,PAGE_107,PAGE_108,PAGE_109,PAGE_110,PAGE_111,
        PAGE_112,PAGE_113,PAGE_114,PAGE_115,PAGE_116,PAGE_117,PAGE_118,PAGE_119,
        PAGE_120,PAGE_121,PAGE_122,PAGE_123,PAGE_124,PAGE_125,PAGE_126,PAGE_127,
        PAGE_MINIMUM = PAGE_000,PAGE_MAXIMUM = PAGE_127, PAGE_TOO_BIG = PAGE_127 + 1
        // Note: PAGE_TOO_BIG is not a valid PageId.
    };

    constexpr uint8_t PageIdToNumber (PageId pageId) {
        return static_cast<uint8_t>(pageId);
    }
    constexpr uint8_t MIN_PAGE_ID = PageIdToNumber(PageId::PAGE_MINIMUM);
    constexpr uint8_t MAX_PAGE_ID = PageIdToNumber(PageId::PAGE_MAXIMUM);
    constexpr PageId NumberToPageId (uint8_t pageId) {
        return static_cast<PageId>(pageId > MAX_PAGE_ID ? MAX_PAGE_ID : pageId);
    }
    constexpr uint16_t getPageBaseAddress (const PageId pageId) {
        return PageIdToNumber(pageId) * MCP_EEPROM_PAGE_SIZE;
    }
    constexpr uint16_t MAX_PAGE_ADDRESS = MCP_EEPROM_PAGE_COUNT * MCP_EEPROM_PAGE_SIZE;
    constexpr uint8_t EEP_PAGE_SIZE = MCP_EEPROM_PAGE_SIZE; // Not totally necessary. Just makes it a bit more flexible.
    using EepBuffer_t = uint8_t [EEP_PAGE_SIZE];

// These operator declarations are not all used. However, this is a nice place to remember how to code this idiom.
// It's easy to see from this why most c++ programmers will use the prefix operator vs postfix, unless there is
// a need to use the value first and then increment. It's slightly more efficient.

    // Prefix increment operator
    inline PageId& operator++ (PageId& pageId) { // To make this safer, we should clamp result to the max value.
//        uint8_t i = static_cast<uint8_t>(pageId) + 1;
//        pageId = static_cast<PageId>(i > MAX_PAGE_ID ? MIN_PAGE_ID : i); // make this wrap around, just as uint8_t.
        pageId = pageId < PageId::PAGE_MAXIMUM
                    ? NumberToPageId(PageIdToNumber(pageId) + 1)
                    : PageId::PAGE_MINIMUM; // make sure we don't go too far.
        return pageId;
    }
    // Postfix increment operator. This is here just to remember how. It has to be able to return the value prior to ++.
    inline PageId operator++(PageId& pageId, int) { // Note the int. This simply distinguishes signature. Required!
        const auto originalValue = pageId;  // save for return
        ++pageId;                           // This could be replaced by the body of the prefix version.
        return originalValue;
    }
    // Prefix decrement operator
    inline PageId& operator-- (PageId& pageId) {
        uint8_t i = static_cast<uint8_t>(pageId) - 1;
        pageId = static_cast<PageId>(i > MAX_PAGE_ID ? MAX_PAGE_ID : i);
        return pageId;
    }
    // Postfix decrement operator
    inline PageId operator--(PageId &pageId, int) {
        const auto originalValue = pageId;  // save for return
        --pageId;
        return originalValue;
    }


}

#endif  // EEPROM_DECLARATIONS_HPP_
