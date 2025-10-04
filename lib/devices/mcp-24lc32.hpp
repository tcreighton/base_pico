
#pragma once
#ifndef MCP_24LC32_AT_HPP_
#define MCP_24LC32_AT_HPP_

// This represents the EEPROM
// 4096 bytes arranged in 128 pages of 32 bytes

#include "csi2c.hpp"
#include "mcp-eeprom-declarations.hpp"

namespace CSdevices {

class Mcp24Lc32 : public Component {

    public:

        Mcp24Lc32 (   const std::string &className,
                        const std::string &label,
                        const uint8_t eePromAddress,    // 3-bit address. Use a different controller to get more blocks.
                        CsI2C* controller) : Component(className,label), controller_(controller) {
            setEePromAddress(eePromAddress);
        }

        Mcp24Lc32 (const Mcp24Lc32& other) = delete;
        Mcp24Lc32& operator=(const Mcp24Lc32& other) = delete;
        ~Mcp24Lc32() override = default;


        [[nodiscard]] CsI2C* getController () const {
            return controller_;
        }

        // Override this if your block needs to support a signature check.
        // This always returns true. So if a block does not support signatures, it's true. If it does, but it's not
        // found, that's a false return.
        /**
         * @brief This always returns true. An override in a sublclass can check and return true or false.
         * @return true always.
         */
        virtual bool checkSignature() {return true;}    // Default for those pages not supporting a signature.

        /**
         * @brief This formats the block for use. All pages are given some pattern.
         * The subclass can override this implementation if it desires, otherwise it can just let this do it.
         * This method simply calls formatPage for all pages in the block.
         * @return true if the format procedure worked. Otherise it returns false.
         */
        virtual bool formatEEProm ();

        /**
         * @brief This formats a single page.
         * The subclass can override this if needed. The default format is to write random data in all the bytes.
         * @param pageId The id of the page: MIN_PAGE_ID through MAX_PAGE_ID.
         * @param pageBuffer The data to write to the page in question. Typically the same for all pages in a block.
         * @return true if the page was correctly formatted.
         */
        virtual bool formatPage (PageId pageId, EepBuffer_t pageBuffer);

        /**
         * @brief This initializes all pages of the block.
         * Note that it is not necessary to override this to do custom initialization.
         * The subclass could simply override the initializePage method to give custom initialization to some pages.
         * However, this will call all page initializations.
         * @return true always unless overridden. If overridden, the subclass should return true IFF all pages are true.
         */
        virtual bool initializeEEProm (); // If a page does not need initialization, just formatting...

        /**
         * @brief This will call
         * Override this if you actually want the subclass to do something to initialize the page.
         * @param pageId The id of the page: MIN_PAGE_ID through MAX_PAGE_ID.
         * @return true if succes.
         */
        virtual bool initializePage (const PageId pageId) {return initializePage(PageIdToNumber(pageId));}
        virtual bool initializePage (uint8_t pageNumber) {return true;}

        template <typename PageType>
        bool updatePage (const PageId pageId, PageType& newPage) {
            auto retCode = false;

            if (PageType oldPage{}; readPage(pageId, oldPage)) {
                oldPage.data = newPage.data;
                retCode = writePage(pageId, oldPage);
            }
            return retCode;
        }

        template <typename PageType>
        bool readPage (const PageId pageId, PageType& page) {
            const auto retCode = readBytes(pageId, page.buffer);

            return retCode; // declaring retCode can make debug easier.
        }

        template <typename PageType>
        bool writePage (const PageId pageId, PageType &page) {
            const auto retCode = writeBytes(pageId, page.buffer);

            return retCode; // declaring retCode can make debug easier.
        }


        /**
         * @brief Reads the designated page.
         * There is no need to worry about endianess for the data. But yes for the address.
         * @param pageId
         * @param buffer - where to put the data.
         * @return true if a page of buffer were read, else false.
         */
        bool readBytes (const PageId pageId, uint8_t* buffer) {
            return readBytes(PageIdToNumber(pageId) * MCP_EEPROM_PAGE_SIZE, buffer);
        }

        bool readBytes (uint16_t address, uint8_t* buffer);

        /**
         * @brief Writes the designated page.
         * @param pageId Id of the page to write.
         * @param buffer Pointer to the data for the write.
         * @return true if all is well.
         */
        bool writeBytes (const PageId pageId, const uint8_t* buffer) {
            return writeBytes(PageIdToNumber(pageId) * MCP_EEPROM_PAGE_SIZE, buffer);
        }

        bool writeBytes (uint16_t address, const uint8_t* buffer);

    protected:

        static constexpr uint8_t EEPromWriteSettlingTime_ms = 5;
        static constexpr uint8_t MAX_ATTEMPTS_AT_SETTLING = 3;

        // This will call itself up to twice more before bailing out.
        // Treat a false return as an error.
        bool isEEPromWriteReady (ControlByte_t controlByte, uint8_t tryNumber = MAX_ATTEMPTS_AT_SETTLING);

        [[nodiscard]] inline absolute_time_t getReadyTime () const {
            return readyTime_;
        }

        void setReadyTime (const uint32_t eePromWriteSettlingTime_ms = EEPromWriteSettlingTime_ms) {
            readyTime_ = make_timeout_time_ms(eePromWriteSettlingTime_ms);
        }

        [[nodiscard]] ControlByte_t getControlByte () const;
        [[nodiscard]] uint8_t getEePromAddress () const {return eePromAddress_;}


    private:


        void setEePromAddress (const uint8_t eePromAddress) {eePromAddress_ = eePromAddress  & 0x07;}

        // This is used as a timer for when the eeprom is ready after a write.
        // Initializing it just is a precaution in case it's checked before it should be.
        absolute_time_t readyTime_ = get_absolute_time();
        uint8_t eePromAddress_ = 0;
        CsI2C* controller_;

    };
}

#endif  // MCP_24LC32_AT_HPP_

