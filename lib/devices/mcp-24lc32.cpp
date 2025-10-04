
#include <cstring>

#include "logger.hpp"
#include "mcp-24lc32.hpp"

#include "devicesContainer.hpp"
#include "random.hpp"
#include "utilities.hpp"

namespace CSdevices {


    // Format all even if one fails.
    bool Mcp24Lc32::formatEEProm() {
        EepBuffer_t pageBuffer;

        // range-based for loop
        for(auto & ix : pageBuffer) {
            // generate a random value for each byte in the buffer.
            ix = CScore::RandomGenerator::getInstance().generateUint8();
        }

        auto retCode = true;
        for (uint16_t i = MIN_PAGE_ID; i <= MAX_PAGE_ID; i++) {
            retCode &= formatPage(NumberToPageId(i), pageBuffer); // if at least one fails, return false.
        }
        return retCode;
    }

    bool Mcp24Lc32::formatPage(const PageId pageId, EepBuffer_t pageBuffer) {
        const auto retCode = writeBytes(pageId, pageBuffer);

        return retCode;
    }

    /**
     * @brief Calls initializePage for each page of the chip.
     * The for loop essentially implements the alltrue idiom in loop.
     * @return true if all pages returned true.
     */
    bool Mcp24Lc32::initializeEEProm() {
        auto retVal = true;
        auto pageId = PageId::PAGE_MINIMUM;

        // If one is false, all are false. But go ahead and initialize all.

        do {
            retVal &= initializePage(pageId);
        } while (PageId::PAGE_MAXIMUM > pageId++);


        return retVal;
    }

    bool Mcp24Lc32::readBytes(const uint16_t address, uint8_t *buffer) {
        bool retCode = false;
        const ControlByte_t controlByte = getControlByte();
        uint8_t addressBigEndian[2];
        int bytesRead = -2, bytesWritten = -4;

        CScore::localUint16ToNetworkByteOrder(address, addressBigEndian);

        // Check that no prior write is pending.
        if ((retCode = isEEPromWriteReady(controlByte))) {
            bytesWritten = getController().writeBuffer(controlByte.byte,
                                                       addressBigEndian,
                                                       sizeof(addressBigEndian),
                                                       false);

            if (sizeof(addressBigEndian) ==  bytesWritten) {
                bytesRead = getController().readBuffer(controlByte.byte,
                                                             buffer,
                                                             MCP_EEPROM_PAGE_SIZE,
                                                             false); // send the stop bit
//                constexpr int foo = MCP_EEPROM_PAGE_SIZE;   // Why foo? so the static analyzer won't freak out
//                retCode = (foo == bytesRead);               // about comparing signed & unsigned.
                retCode = (MCP_EEPROM_PAGE_SIZE == bytesRead);
            }
        } else {
            CScore::logger_.log(CScore::LogLevel::Error,
                                     getClassName(),
                                     "readBytes; bytes read, written: " +
                                     std::to_string(bytesRead),
                                     std::to_string(bytesWritten));
        }
        return retCode;
    }

    bool Mcp24Lc32::writeBytes(const uint16_t address, const uint8_t *buffer) {
        bool retCode;

        // Check that no prior write is pending.
        if (const ControlByte_t controlByte = getControlByte(); (retCode = isEEPromWriteReady(controlByte))) {
            uint8_t bigEndianAddressPlusData[2 + MCP_EEPROM_PAGE_SIZE];

            CScore::localUint16ToNetworkByteOrder(address, bigEndianAddressPlusData);

            std::memcpy(&bigEndianAddressPlusData[2], buffer, MCP_EEPROM_PAGE_SIZE);

            const auto bytesWritten = getController().writeBuffer(controlByte.byte,
                                                                     bigEndianAddressPlusData,
                                                                     MCP_EEPROM_PAGE_SIZE + 2,
                                                                     false); // send the stop bit

            if ((retCode = (MCP_EEPROM_PAGE_SIZE + 2 == bytesWritten))) {
                setReadyTime();
            }
        }
        return retCode;
    }

    ControlByte_t Mcp24Lc32::getControlByte() const {
        ControlByte_t result{};
        result.byte = 0;

        result.bits.chipSelect    = getEePromAddress();    // snag the lower 3 bits
        result.bits.i2cDevice     = CONTROL_I2C_DEVICE;

        return result;
    }

    // Note that this is called recursively.
    bool Mcp24Lc32::isEEPromWriteReady(const ControlByte_t controlByte, const uint8_t tryNumber) {
        bool retCode = false;

        sleep_until(getReadyTime());
        // Now we can check if the device is ready to go.
        // This is done by writing 0 bytes to the device. If it answers >= 0, then ready.

        constexpr uint8_t value = 0;  // dummy value for the control byte write

        const auto bytesWritten = getController().writeBuffer(controlByte.byte,
                                                                 &value,
                                                                 0,       // Not really writing. Just checking
                                                                 false); // send the stop bit

        if (bytesWritten < 0 && tryNumber > 0) {
            // Did not work. Let's try once more.
            setReadyTime();
            retCode = isEEPromWriteReady(controlByte, tryNumber - 1);
        }

        if (!retCode) {
            retCode = bytesWritten >= 0;
        }
        return retCode;
    }

}
