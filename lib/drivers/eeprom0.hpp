
#pragma once

#ifndef EEPROM0_HPP_
#define EEPROM0_HPP_

#include "csi2c.hpp"
#include "eeprom0-page-declarations.hpp"
#include "mcp-24lc32.hpp"


namespace CSdrivers {

    class EeProm0 final : public Mcp24Lc32 {

        public:

        explicit EeProm0 (const ControllerId controllerId) :
                                Mcp24Lc32(EEPromIDToNumber(EEPromId::EEPROM_0), controllerId) {
            setClassName("EEProm0");
            setLabel("SW EEProm 0");
        }


        EeProm0 (const EeProm0& other) = delete;
        EeProm0& operator=(const EeProm0& other) = delete;
        ~EeProm0() override = default;

        [[nodiscard]] static EEPromId getEePromId () {
            return EEPromId::EEPROM_0;
        }

        // This gets called during system initialization - StateMachine::doInitialEEPromState
        bool init () override;

        ///
        /// @return The global instance of EEProm0 from focus500 container.
        ///
        static EeProm0& getEEProm0 ();

        static void computeSignature (uint8_t* signature);
        /**
         * @brief This tests the signature used in eeprom0. Each eeprom MAY have a signature.
         * Basically, if we define a signature (sequence of bytes) to appear in
         * one or more pages for a given block (prom) then this will retrieve that page or those pages
         * and check the byte sequence.
         *
         * If a block (eeprom) does not support signature, it can either override the base class implementation
         * and return true, or just not override it, as that will always return true.
         * @return true if the signature was found. Return false if not.
         */
        bool checkSignature () override;

        /**
         * @brief This 'formats' the eeprom. That consists of writing a page of random values to all the pages.
         * It's the same random values for all pages, but it generates the values new each time format is called.
         * THIS ERASES ALL VALUES IN THE EEPROM!
         * @return true if all is well
         */
        bool formatEEProm () override {
            return Mcp24Lc32::formatEEProm(); // Just delegate to the base class.
        }

        /**
         * @brief Initialization writes the values that are supposed to be in each page. They may be just
         * a starting point. But they are the right format at least.
         * @return
         */
        [[nodiscard]] bool initializeEEProm () override {
            return Mcp24Lc32::initializeEEProm(); // Just delegate to the base class. It will call back per page.
        }

        // Initialization of pages here. These are called from initializeEEProm.

        bool initializePage (PageId pageId) override;
        
        /// @brief Converts the two pages of eeprom associated with control data into ControlData
        /// @param controlDataPage1 First page as declared for eeprom;
        /// @param controlDataPage2 Second page as declared for eeprom;
        /// @return A copy of ControlData.
        /*
        static CSfactory::ControlData getControlDataFromStorage (  const ControlDataPage1& controlDataPage1,
                                                                   const ControlDataPage2& controlDataPage2);

        static CSfactory::ControlData getControlDataFromStorage (CSpowersupply::PowerSupplyID psId);

        static void setControlDataInStorage (CSpowersupply::PowerSupplyID psId,
                                                const CSfactory::ControlData& controlData);

        static void setControlDataInStorage (CSpowersupply::PowerSupplyID psId);

        static void setFactoryControlDataInStorage (const CSpowersupply::PowerSupplyID psId) {
            setControlDataInStorage(psId, CSfactory::getControlDataFromFactory(psId));
        }

        static ControlDataPage1 getControlDataPage1FromControlData (const CSfactory::ControlData& controlData);

        static ControlDataPage2 getControlDataPage2FromControlData (const CSfactory::ControlData& controlData);

        */
        // Page methods here.

        /**
         * @brief Reads the G1 ControlDataPage1 group of control data values.
         * @param g1ControlData1 Struct holding the values.
         */
        bool getG1ControlDataPage1 (PSControlDataPage1_t& g1ControlData1) {
            return readPage(GRID1_CONTROL_DATA_PAGE_1, g1ControlData1);
        }

        /**
         * @brief Writes the G1 ControlDataPage1 group of control data values.
         * @param g1ControlData1 Struct holding the values.
         */
        bool setG1ControlDataPage1 (PSControlDataPage1_t& g1ControlData1) {
            return writePage(GRID1_CONTROL_DATA_PAGE_1, g1ControlData1);
        }

        /**
         * @brief Reads the G1 ControlDataPage2 group of control data values.
         * @param g1ControlData2 Struct holding the values.
         */
        bool getG1ControlDataPage2 (PSControlDataPage2_t& g1ControlData2) {
            return readPage(GRID1_CONTROL_DATA_PAGE_2, g1ControlData2);
        }

        /**
         * @brief Writes the G1 ControlDataPage2 group of control data values.
         * @param g1ControlData2 Struct holding the values.
         */
        bool setG1ControlDataPage2 (PSControlDataPage2_t& g1ControlData2) {
            return writePage(GRID1_CONTROL_DATA_PAGE_2, g1ControlData2);
        }

        /**
         * @brief Reads the G2 ControlDataPage1 group of control data values.
         * @param g2ControlData1 Struct holding the values.
         */
        bool getG2ControlDataPage1 (PSControlDataPage1_t& g2ControlData1) {
            return readPage(GRID2_CONTROL_DATA_PAGE_1, g2ControlData1);
        }

        /**
         * @brief Writes the G2 ControlDataPage1 group of control data values.
         * @param g2ControlData1 Struct holding the values.
         */
        bool setG2ControlDataPage1 (PSControlDataPage1_t& g2ControlData1) {
            return writePage(GRID2_CONTROL_DATA_PAGE_1, g2ControlData1);
        }

        /**
         * @brief Reads the G2 ControlDataPage2 group of control data values.
         * @param g2ControlData2 Struct holding the values.
         */
        bool getG2ControlDataPage2 (PSControlDataPage2_t& g2ControlData2) {
            return readPage(GRID2_CONTROL_DATA_PAGE_2, g2ControlData2);
        }

        /**
         * @brief Writes the G2 ControlDataPage2 group of control data values.
         * @param g2ControlData2 Struct holding the values.
         */
        bool setG2ControlDataPage2 (PSControlDataPage2_t& g2ControlData2) {
            return writePage(GRID2_CONTROL_DATA_PAGE_2, g2ControlData2);
        }

        /**
         * @brief Reads the G3 ControlDataPage1 group of control data values.
         * @param g3ControlData1 Struct holding the values.
         */
        bool getG3ControlDataPage1 (PSControlDataPage1_t& g3ControlData1) {
            return readPage(GRID3_CONTROL_DATA_PAGE_1, g3ControlData1);
        }

        /**
         * @brief Writes the G3 ControlDataPage1 group of control data values.
         * @param g3ControlData1 Struct holding the values.
         */
        bool setG3ControlDataPage1 (PSControlDataPage1_t& g3ControlData1) {
            return writePage(GRID3_CONTROL_DATA_PAGE_1, g3ControlData1);
        }

        /**
         * @brief Reads the G3 ControlDataPage2 group of control data values.
         * @param g3ControlData2 Struct holding the values.
         */
        bool getG3ControlDataPage2 (PSControlDataPage2_t& g3ControlData2) {
            return readPage(GRID3_CONTROL_DATA_PAGE_2, g3ControlData2);
        }

        /**
         * @brief Writes the G3 ControlDataPage2 group of control data values.
         * @param g3ControlData2 Struct holding the values.
         */
        bool setG3ControlDataPage2 (PSControlDataPage2_t& g3ControlData2) {
            return writePage(GRID3_CONTROL_DATA_PAGE_2, g3ControlData2);
        }

        /**
         * @brief Reads the Heater ControlDataPage1 group of control data values.
         * @param heaterControlData1 Struct holding the values.
         */
        bool getHeaterControlDataPage1 (PSControlDataPage1_t& heaterControlData1) {
            return readPage(HEATER_CONTROL_DATA_PAGE_1, heaterControlData1);
        }

        /**
         * @brief Writes the Heater ControlDataPage1 group of control data values.
         * @param heaterControlData1 Struct holding the values.
         */
        bool setHeaterControlDataPage1 (PSControlDataPage1_t& heaterControlData1) {
            return writePage(HEATER_CONTROL_DATA_PAGE_1, heaterControlData1);
        }

        /**
         * @brief Reads the Heater ControlDataPage2 group of control data values.
         * @param heaterControlData2 Struct holding the values.
         */
        bool getHeaterControlDataPage2 (PSControlDataPage2_t& heaterControlData2) {
            return readPage(HEATER_CONTROL_DATA_PAGE_2, heaterControlData2);
        }

        /**
         * @brief Writes the Heater ControlDataPage2 group of control data values.
         * @param heaterControlData2 Struct holding the values.
         */
        bool setHeaterControlDataPage2 (PSControlDataPage2_t& heaterControlData2) {
            return writePage(HEATER_CONTROL_DATA_PAGE_2, heaterControlData2);
        }

        /**
         * @brief Reads the HV ControlDataPage1 group of control data values.
         * @param hvControlData1 Struct holding the values.
         */
        bool getHVControlDataPage1 (PSControlDataPage1_t& hvControlData1) {
            return readPage(HV_CONTROL_DATA_PAGE_1, hvControlData1);
        }

        /**
         * @brief Writes the HV ControlDataPage1 group of control data values.
         * @param hvControlData1 Struct holding the values.
         */
        bool setHVControlDataPage1 (PSControlDataPage1_t& hvControlData1) {
            return writePage(HV_CONTROL_DATA_PAGE_1, hvControlData1);
        }

        /**
         * @brief Reads the HV ControlDataPage2 group of control data values.
         * @param hvControlData2 Struct holding the values.
         */
        bool getHVControlDataPage2 (PSControlDataPage2_t& hvControlData2) {
            return readPage(HV_CONTROL_DATA_PAGE_2, hvControlData2);
        }

        /**
         * @brief Writes the HV ControlDataPage2 group of control data values.
         * @param hvControlData2 Struct holding the values.
         */
        bool setHVControlDataPage2 (PSControlDataPage2_t& hvControlData2) {
            return writePage(HV_CONTROL_DATA_PAGE_2, hvControlData2);
        }

    private:

        void logPageInitError (PageId pageId);
    };


}

#endif    // EEPROM0_HPP_


