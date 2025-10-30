

#pragma once
#ifndef EEPROM0_PAGE_DECLARATIONS_HPP_
#define EEPROM0_PAGE_DECLARATIONS_HPP_

#include <cstdint>

#include "mcp-eeprom-declarations.hpp"
#include "product-info.hpp"

namespace CSdrivers {

    namespace ee = CSdevices;   // alias the namespace to make it less verbose in this file.
    /**
     *  The following constant expressions give 'usage names' to pageIds.
     *  Also here are some declarations unique to each page.
     *  Note that this is always grouped by EEPromId because the pages have differing declarations within each EEProm.
     */

    // Write string data along with trailing null character. Set the field length to include the null.
    // However, upon reading it in, don't rely on the null character being there. Instead, read the full width
    // of the field (obviously since it's a page at a time.) Then explictly apply the null character.
    // This will handle the case of the page never having been written.
    // Remember that a #define constant of a char array will always include the null byte in its size.

    //*************************************************************************************************************

    //----------PAGE_000---------------
    constexpr auto COMPANY_NAME_PAGE = ee::EEPromPageId::PAGE_000;

    struct PageCompany {
        char companyName [sizeof(COMPANY_NAME)] = COMPANY_NAME;  // Includes null byte
        uint8_t unused [ee::EEP_PAGE_SIZE >= sizeof(companyName)
                            ? ee::EEP_PAGE_SIZE - sizeof(companyName)
                            : 0]{};  // Expansion area
    };
    static_assert(sizeof(PageCompany) <= ee::EEP_PAGE_SIZE, "EeProm0PageCompany declaration too big!!");

    union PageCompanyBuffer {
        PageCompany data{};
        ee::EepBuffer_t buffer;
    };

    using PageCompany_t = PageCompanyBuffer;

    //----------PAGE_001---------------
    constexpr auto PRODUCT_INFO_1 = ee::EEPromPageId::PAGE_001;

    struct PageProductInfo1 {
        char productName [sizeof(PRODUCT_NAME)] = PRODUCT_NAME;  // Includes null byte
        uint8_t unused [ee::EEP_PAGE_SIZE >= sizeof(productName)
                            ? ee::EEP_PAGE_SIZE - sizeof(productName)
                            : 0]{};  // Expansion area
    };
    static_assert(sizeof(PageProductInfo1) <= ee::EEP_PAGE_SIZE, "EeProm0PageProductInfo1 declaration too big!!");

    union PageProductInfo1Buffer {
        PageProductInfo1 data{};
        ee::EepBuffer_t buffer;
    };

    using PageProductInfo1_t = PageProductInfo1Buffer;

    //----------PAGE_002---------------
    constexpr auto PRODUCT_INFO_2 = ee::EEPromPageId::PAGE_002; // part # (10+1); serial # (16+1)

    #define PART_NUMBER_PATTERN       "Part#00000"        // part # (10+1); serial # (16+1)
    #define SERIAL_NUMBER_PATTERN     "Serial#000000000"  // This just makes the right length w/o null

    struct PageProductInfo2 {
        char partNumber [sizeof("Part#00000")] = "Part#00000";
        char serialNumber [sizeof("Serial#000000000")] = "Serial#000000000";
        uint8_t unused [ee::EEP_PAGE_SIZE >= (sizeof(partNumber) + sizeof(serialNumber))
                        ? ee::EEP_PAGE_SIZE - (sizeof(partNumber) + sizeof(serialNumber))
                        : 0]{};  // Expansion area
    };
    static_assert(sizeof(PageProductInfo2) <= ee::EEP_PAGE_SIZE, "EeProm0PageProductInfo2 declaration too big!!");

    union PageProductInfo2Buffer {
        PageProductInfo2 data{};
        ee::EepBuffer_t buffer;
    };

    using PageProductInfo2_t = PageProductInfo2Buffer;


    //----------PAGE_003---------------
    constexpr auto PRODUCT_INFO_3 = ee::EEPromPageId::PAGE_003; // firmware version (3 bytes)

    struct PageProductInfo3 {
        uint8_t majorVersion = 0;
        uint8_t minorVersion = 0;
        uint16_t buildNumber = 0;
        uint8_t unused [ee::EEP_PAGE_SIZE >= (sizeof(majorVersion) + sizeof(minorVersion) + sizeof(buildNumber))
                        ? ee::EEP_PAGE_SIZE - (sizeof(majorVersion) + sizeof(minorVersion) + sizeof(buildNumber))
                        : 0]{};  // Expansion area
    };
    static_assert(sizeof(PageProductInfo3) <= ee::EEP_PAGE_SIZE, "EeProm0PageProductInfo3 declaration too big!!");

    union PageProductInfo3Buffer {
        PageProductInfo3 data{};
        ee::EepBuffer_t buffer;
    };

    using PageProductInfo3_t = PageProductInfo3Buffer;

    /// @brief Page layout for eeprom support of control data.
    /// The following two structs declare how to layout the control data in two pages of Mcp24Lc32At eeprom.
    ///

    struct ControlDataPage1 {
        float   vrcMVoltageSlope        = 0.0f;
        float   vrcBVoltageOffset       = 0.0f;
        float   vrcVConversionCoeff     = 0.0f;
        float   crcMVoltageSlope        = 0.0f;
        float   crcBVoltageOffset       = 0.0f;
        float   crcV2CCoeff             = 0.0f;
        float   crcVOutScaleMultiplier  = 0.0f;
        float   vwcMVoltageSlope        = 0.0f;
    };

    constexpr size_t CD_PAGE1_SIZE = sizeof(ControlDataPage1);
    static_assert(CD_PAGE1_SIZE <= ee::MCP_EEPROM_PAGE_SIZE, "Page1 size exceeds MCP24LC32_PAGE_SIZE");

    struct ControlDataPage2 {
        float       vwcBVoltageOffset                       = 0.0f;
        uint16_t    vwcVConversionCoeff                     = 0;
        float       pdcKpValue                              = 0.0f;
        float       pdcKiValue                              = 0.0f;
        float       pdcKdValue                              = 0.0f;
        float       pdcMinControlVariable                   = 0.0f;
        float       pdcMaxControlVariable                   = 0.0f;
//        CSfactory::ErrorWindowSize_t pdcCollectionWindowSize  = 0;
        uint8_t pdcCollectionWindowSize  = 0;
    };

    constexpr size_t CD_PAGE2_SIZE = sizeof(ControlDataPage2);
    static_assert(CD_PAGE2_SIZE <= ee::MCP_EEPROM_PAGE_SIZE, "Page2 size exceeds MCP24LC32_PAGE_SIZE");


    // Declarations for power supply control data pages. Two for each ps.
    union PSControlDataPage1 {
        ControlDataPage1 data{};
        ee::EepBuffer_t buffer;
    };
    using PSControlDataPage1_t = PSControlDataPage1;

    union PSControlDataPage2 {
        ControlDataPage2 data{};
        ee::EepBuffer_t buffer;
    };
    using PSControlDataPage2_t = PSControlDataPage2;

    //----------PAGE_030---------------
    // Grid1 Control Data Page One of Two
    constexpr auto GRID1_CONTROL_DATA_PAGE_1 = ee::EEPromPageId::PAGE_030;

    //----------PAGE_031---------------
    // Grid1 Control Data Page Two of Two
    constexpr auto GRID1_CONTROL_DATA_PAGE_2 = ee::EEPromPageId::PAGE_031;

    //----------PAGE_032---------------
    // Grid2 Control Data Page One of Two
    constexpr auto GRID2_CONTROL_DATA_PAGE_1 = ee::EEPromPageId::PAGE_032;

    //----------PAGE_033---------------
    // Grid2 Control Data Page Two of Two
    constexpr auto GRID2_CONTROL_DATA_PAGE_2 = ee::EEPromPageId::PAGE_033;

    //----------PAGE_034---------------
    // Grid3 Control Data Page One of Two
    constexpr auto GRID3_CONTROL_DATA_PAGE_1 = ee::EEPromPageId::PAGE_034;

    //----------PAGE_035---------------
    // Grid3 Control Data Page Two of Two
    constexpr auto GRID3_CONTROL_DATA_PAGE_2 = ee::EEPromPageId::PAGE_035;

    //----------PAGE_036---------------
    // Heater Control Data Page One of Two
    constexpr auto HEATER_CONTROL_DATA_PAGE_1 = ee::EEPromPageId::PAGE_036;

    //----------PAGE_037---------------
    // Heater Control Data Page Two of Two
    constexpr auto HEATER_CONTROL_DATA_PAGE_2 = ee::EEPromPageId::PAGE_037;

    //----------PAGE_038---------------
    // HighVoltage Control Data Page One of Two
    constexpr auto HV_CONTROL_DATA_PAGE_1 = ee::EEPromPageId::PAGE_038;

    //----------PAGE_039---------------
    // HighVoltage Control Data Page Two of Two
    constexpr auto HV_CONTROL_DATA_PAGE_2 = ee::EEPromPageId::PAGE_039;


    //----------PAGE_127---------------
    // This page is used to determine if the block has been written to by us.
    // It's a page of random values, however, we've added a signature to the beginning seven bytes.
    constexpr auto SIGNATURE_PAGE = ee::EEPromPageId::PAGE_127;

    constexpr auto SIGNATURE_LENGTH = 1+5+1+4; // just to make things obvious
    constexpr uint8_t SIGNATURE_DEFAULT [SIGNATURE_LENGTH] {
                                                0x00,
                                                0x44 - 0x04, 0x69 - 0x04, 0x61 - 0x04, 0x6e - 0x04, 0x65 - 0x04,
                                                0x00,
                                                0x00, 0x00, 0x00, 0x00};
    struct PageSignature { // 1 byte 00, 5 'random' bytes, 1 byte 00, 4 bytes packed date
        uint8_t signature [SIGNATURE_LENGTH];
        uint8_t unused [ee::EEP_PAGE_SIZE >= sizeof(signature)
                        ? ee::EEP_PAGE_SIZE - sizeof(signature)
                        : 0]{};  // Expansion area
    };
    static_assert(sizeof(PageSignature) <= ee::EEP_PAGE_SIZE, "EeProm0PageSignature declaration too big!!");
    constexpr uint8_t SIGNATURE_BUILD_DATE_OFFSET = 0x07; // location of the packed build date
    union PageSignatureBuffer {
        PageSignature data{};
        ee::EepBuffer_t   buffer; // Initializer is necessary or struct is immediately destroyed.
    };

    using PageSignature_t = PageSignatureBuffer;


    //*************************************************************************************************************

}

#endif  //EEPROM0_PAGE_DECLARATIONS_HPP_

