

#include <cstring>

#include "eeprom0.hpp"
#include "eeprom0-page-declarations.hpp"
#include "logger.hpp"
#include "mcp-24lc32.hpp"
#include "packed-datetime.hpp"
#include "utilities.hpp"

namespace CSdrivers {


    bool EeProm0::init() {

        auto retCode = Mcp24Lc32::init(); // Let the base classes have a chance to initialize.

        if (!checkSignature()) {    // This means we need to format and initialize the block.
            formatEEProm();
            retCode = Mcp24Lc32::initializeEEProm();
        }

        return retCode;
    }

    /*
    EeProm0& EeProm0::getEEProm0() {
        return CSfocus500::Focus500Container::getEEProm0();
    }
    */

    void EeProm0::computeSignature(uint8_t *signature) { // assumes that signature points at an array large enough.
        const CScore::PackedDateTime_t dt = CScore::PackedDateTime::getPackedBuildDate();
        CScore::UInt32ToBytes dateBytes;
        dateBytes.data = dt;
        memcpy(signature, SIGNATURE_DEFAULT, sizeof(SIGNATURE_DEFAULT));    // copy in the const data
        memcpy(signature + SIGNATURE_BUILD_DATE_OFFSET,
                dateBytes.bytes,
                sizeof(dateBytes.bytes));
    }

    bool EeProm0::checkSignature() {
        bool retCode = false;


        if (PageSignature_t signaturePage{};
            readBytes(SIGNATURE_PAGE, signaturePage.buffer)) {
                PageSignature pageSignature;
                computeSignature(pageSignature.signature);  // Get the correct signature to compare against.
                retCode = (memcmp(signaturePage.buffer, pageSignature.signature, sizeof(pageSignature.signature)) == 0);
            }


        return retCode;
    }

    bool EeProm0::initializePage(const PageId pageId) {
        auto retValue = false;

        switch (pageId) {
            case COMPANY_NAME_PAGE: {
                PageCompany_t newData;
                retValue = writePage(COMPANY_NAME_PAGE, newData);
            }
                break;

            case PRODUCT_INFO_1: {
                PageProductInfo1_t newData;
                retValue = writePage(PRODUCT_INFO_1, newData);
            }
                break;

            case PRODUCT_INFO_2: {
                PageProductInfo2_t newData;
                retValue = writePage(PRODUCT_INFO_2, newData);
            }
                break;

            case PRODUCT_INFO_3: {
                PageProductInfo3_t newData;
                retValue = writePage(PRODUCT_INFO_3, newData);
            }
                break;

            case GRID1_CONTROL_DATA_PAGE_1: {
                /*
                PSControlDataPage1_t newData;
                newData.data = getControlDataPage1FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G1));
                retValue = writePage(GRID1_CONTROL_DATA_PAGE_1, newData);
            */
            }
                break;

            case GRID1_CONTROL_DATA_PAGE_2: {
                /*
                PSControlDataPage2_t newData;
                newData.data = getControlDataPage2FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G1));
                retValue = writePage(GRID1_CONTROL_DATA_PAGE_2, newData);
            */
            }
                break;

            case GRID2_CONTROL_DATA_PAGE_1: {
                /*
                PSControlDataPage1_t newData;
                newData.data = getControlDataPage1FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G2));
                retValue = writePage(GRID2_CONTROL_DATA_PAGE_1, newData);
            */
            }
                break;

            case GRID2_CONTROL_DATA_PAGE_2: {
                /*
                PSControlDataPage2_t newData;
                newData.data = getControlDataPage2FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G2));
                retValue = writePage(GRID2_CONTROL_DATA_PAGE_2, newData);
            */
            }
                break;

            case GRID3_CONTROL_DATA_PAGE_1: {
                /*
                PSControlDataPage1_t newData;
                newData.data = getControlDataPage1FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G3));
                retValue = writePage(GRID3_CONTROL_DATA_PAGE_1, newData);
            */
            }
                break;

            case GRID3_CONTROL_DATA_PAGE_2: {
                /*
                PSControlDataPage2_t newData;
                newData.data = getControlDataPage2FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::G3));
                retValue = writePage(GRID3_CONTROL_DATA_PAGE_2, newData);
            */
            }
                break;

            case HEATER_CONTROL_DATA_PAGE_1: {
                /*
                PSControlDataPage1_t newData;
                newData.data = getControlDataPage1FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::HEATER));
                retValue = writePage(HEATER_CONTROL_DATA_PAGE_1, newData);
            */
            }
                break;

            case HEATER_CONTROL_DATA_PAGE_2: {
                /*
                PSControlDataPage2_t newData;
                newData.data = getControlDataPage2FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::HEATER));
                retValue = writePage(HEATER_CONTROL_DATA_PAGE_2, newData);
            */
            }
                break;

            case HV_CONTROL_DATA_PAGE_1: {
                /*
                PSControlDataPage1_t newData;
                newData.data = getControlDataPage1FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::HV));
                retValue = writePage(HV_CONTROL_DATA_PAGE_1, newData);
            */
            }
                break;

            case HV_CONTROL_DATA_PAGE_2: {
                /*
                PSControlDataPage2_t newData;
                newData.data = getControlDataPage2FromControlData(
                                    CSfactory::getControlDataFromFactory(
                                        CSpowersupply::PowerSupplyID::HV));
                retValue = writePage(HV_CONTROL_DATA_PAGE_2, newData);
            */
            }
                break;

            case SIGNATURE_PAGE: {
                PageSignature_t newData;
                computeSignature(newData.data.signature);    // Get the correct signature to compare against.

                retValue = writePage(SIGNATURE_PAGE, newData);
            }
                break;

            default:
                retValue = true;
                break;
        }

        if (!retValue) {
            logPageInitError(pageId);
        }

        return retValue;
    }

    /*
    CSfactory::ControlData EeProm0::getControlDataFromStorage(  const ControlDataPage1 &controlDataPage1,
                                                                const ControlDataPage2 &controlDataPage2) {
        CSfactory::ControlData controlData;
        controlData.voltageReadControlData.mVoltageSlope            = controlDataPage1.vrcMVoltageSlope;
        controlData.voltageReadControlData.bVoltageOffset           = controlDataPage1.vrcBVoltageOffset;
        controlData.voltageReadControlData.vConversionCoeff         = controlDataPage1.vrcVConversionCoeff;
        controlData.currentReadControlData.mCurrentSlope            = controlDataPage1.crcMVoltageSlope;
        controlData.currentReadControlData.bCurrentOffset           = controlDataPage1.crcBVoltageOffset;
        controlData.currentReadControlData.v2cCoeff                 = controlDataPage1.crcV2CCoeff;
        controlData.currentReadControlData.vOutScaleMultiplier      = controlDataPage1.crcVOutScaleMultiplier;
        controlData.voltageWriteControlData.mVoltageSlope           = controlDataPage1.vwcMVoltageSlope;
        controlData.voltageWriteControlData.bVoltageOffset          = controlDataPage2.vwcBVoltageOffset;
        controlData.voltageWriteControlData.dacVConversionCoeff     = controlDataPage2.vwcVConversionCoeff;
        controlData.pidControlData.kpValue                          = controlDataPage2.pdcKpValue;
        controlData.pidControlData.kiValue                          = controlDataPage2.pdcKiValue;
        controlData.pidControlData.kdValue                          = controlDataPage2.pdcKdValue;
        controlData.pidControlData.minControlVariable               = controlDataPage2.pdcMinControlVariable;
        controlData.pidControlData.maxControlVariable               = controlDataPage2.pdcMaxControlVariable;
        controlData.pidControlData.collectionWindowSize             = controlDataPage2.pdcCollectionWindowSize;

        return controlData;
    }
    */

    /*
    CSfactory::ControlData EeProm0::getControlDataFromStorage(CSpowersupply::PowerSupplyID psId) {
            PSControlDataPage1_t page1;
            PSControlDataPage2_t page2;
            CSfactory::ControlData controlData;
            EeProm0& eeProm0 = getEEProm0();
            bool result = false;

            switch (normalizeID(psId)) {
                default:
                    // just return the initialized controlData.
                    break;
                case CSpowersupply::NORMALIZED_G1:
                    result =  eeProm0.getG1ControlDataPage1(page1);
                    result &= eeProm0.getG1ControlDataPage2(page2);

                    if (result) {
                        controlData = getControlDataFromStorage(page1.data, page2.data);
                    }
                    break;

                case CSpowersupply::NORMALIZED_G2:
                    result =  eeProm0.getG2ControlDataPage1(page1);
                    result &= eeProm0.getG2ControlDataPage2(page2);

                    if (result) {
                        controlData = getControlDataFromStorage(page1.data, page2.data);
                    }
                    break;

                case CSpowersupply::NORMALIZED_G3:
                    result =  eeProm0.getG3ControlDataPage1(page1);
                    result &= eeProm0.getG3ControlDataPage2(page2);

                    if (result) {
                        controlData = getControlDataFromStorage(page1.data, page2.data);
                    }
                    break;

                case CSpowersupply::NORMALIZED_HEATER:
                    result =  eeProm0.getHeaterControlDataPage1(page1);
                    result &= eeProm0.getHeaterControlDataPage2(page2);

                    if (result) {
                        controlData = getControlDataFromStorage(page1.data, page2.data);
                    }
                    break;

                case CSpowersupply::NORMALIZED_HIGHVOLTAGE:
                    result =  eeProm0.getHVControlDataPage1(page1);
                    result &= eeProm0.getHVControlDataPage2(page2);

                    if (result) {
                        controlData = getControlDataFromStorage(page1.data, page2.data);
                    }
                    break;
            }

            return controlData;

    }
    */

    /*
    void EeProm0::setControlDataInStorage(CSpowersupply::PowerSupplyID psId,
                                                const CSfactory::ControlData &controlData) {
        PSControlDataPage1 page1 {};
        PSControlDataPage2 page2 {};

        page1.data = getControlDataPage1FromControlData(controlData);
        page2.data = getControlDataPage2FromControlData(controlData);

        EeProm0& eeProm0 = getEEProm0();

        switch (CSpowersupply::PowerSupplyID normalizedId = normalizeID(psId)) {
            default:
                break;
            case CSpowersupply::PowerSupplyID::G1:
                eeProm0.writePage(GRID1_CONTROL_DATA_PAGE_1, page1);
                eeProm0.writePage(GRID1_CONTROL_DATA_PAGE_2, page2);
                break;
            case CSpowersupply::PowerSupplyID::G2:
                eeProm0.writePage(GRID2_CONTROL_DATA_PAGE_1, page1);
                eeProm0.writePage(GRID2_CONTROL_DATA_PAGE_2, page2);
                break;
            case CSpowersupply::PowerSupplyID::G3:
                eeProm0.writePage(GRID3_CONTROL_DATA_PAGE_1, page1);
                eeProm0.writePage(GRID3_CONTROL_DATA_PAGE_2, page2);
                break;
            case CSpowersupply::PowerSupplyID::H:
                eeProm0.writePage(HEATER_CONTROL_DATA_PAGE_1, page1);
                eeProm0.writePage(HEATER_CONTROL_DATA_PAGE_2, page2);
                break;
            case CSpowersupply::PowerSupplyID::HV:
                eeProm0.writePage(HV_CONTROL_DATA_PAGE_1, page1);
                eeProm0.writePage(HV_CONTROL_DATA_PAGE_2, page2);
                break;

        }
    }
    */

    /*
    void EeProm0::setControlDataInStorage(const CSpowersupply::PowerSupplyID psId) {
        setControlDataInStorage(psId,
                CSfocus500::Focus500Container::getPowerSupply(psId).getPSControlData().getControlData());
    }

    ControlDataPage1 EeProm0::getControlDataPage1FromControlData(const CSfactory::ControlData &controlData)  {
        ControlDataPage1 controlDataPage1;
        controlDataPage1.vrcMVoltageSlope       = controlData.voltageReadControlData.mVoltageSlope;
        controlDataPage1.vrcBVoltageOffset      = controlData.voltageReadControlData.bVoltageOffset;
        controlDataPage1.vrcVConversionCoeff    = controlData.voltageReadControlData.vConversionCoeff;
        controlDataPage1.crcMVoltageSlope       = controlData.currentReadControlData.mCurrentSlope;
        controlDataPage1.crcBVoltageOffset      = controlData.currentReadControlData.bCurrentOffset;
        controlDataPage1.crcV2CCoeff            = controlData.currentReadControlData.v2cCoeff;
        controlDataPage1.crcVOutScaleMultiplier = controlData.currentReadControlData.vOutScaleMultiplier;
        controlDataPage1.vwcMVoltageSlope       = controlData.voltageWriteControlData.mVoltageSlope;
        return controlDataPage1;
    }
    */

    /*
    ControlDataPage2 EeProm0::getControlDataPage2FromControlData(const CSfactory::ControlData &controlData) {
        ControlDataPage2 controlDataPage2;
        controlDataPage2.vwcBVoltageOffset          = controlData.voltageWriteControlData.bVoltageOffset;
        controlDataPage2.vwcVConversionCoeff        = controlData.voltageWriteControlData.dacVConversionCoeff;
        controlDataPage2.pdcKpValue                 = controlData.pidControlData.kpValue;
        controlDataPage2.pdcKiValue                 = controlData.pidControlData.kiValue;
        controlDataPage2.pdcKdValue                 = controlData.pidControlData.kdValue;
        controlDataPage2.pdcMinControlVariable      = controlData.pidControlData.minControlVariable;
        controlDataPage2.pdcMaxControlVariable      = controlData.pidControlData.maxControlVariable;
        controlDataPage2.pdcCollectionWindowSize    = controlData.pidControlData.collectionWindowSize;
        return controlDataPage2;
    }
    */

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void EeProm0::logPageInitError(const PageId pageId) {

#if defined (LOG_GROUP_TWO)

        switch (pageId) {
            case PageId::PAGE_000:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_COMPANY_NAME.");
                break;

            case PageId::PAGE_001:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PRODUCT_INFO_1.");
                break;

            case PageId::PAGE_002:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PRODUCT_INFO_2.");
                break;

            case PageId::PAGE_003:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PRODUCT_INFO_3.");
                break;

            case PageId::PAGE_010:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PS_G1_SETTINGS.");
                break;

            case PageId::PAGE_011:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PS_G2_SETTINGS.");
                break;

            case PageId::PAGE_012:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PS_G3_SETTINGS.");

            case PageId::PAGE_013:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                        getClassName(),
                                        "initializeEEProm0",
                                        "Failed to initialize EEP0_PS_HEATER_SETTINGS.");
                break;

            case PageId::PAGE_014:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_PS_HIGH_VOLTAGE_SETTINGS.");
                break;

            case PageId::PAGE_127:
                CSutilities::logger_.log(CSutilities::LogLevel::Error,
                                         getClassName(),
                                         "initializeEEProm0",
                                         "Failed to initialize EEP0_SIGNATURE_PAGE.");
                break;

            default:

                break;

        }
#endif    // LOG_GROUP_TWO

    }

}
