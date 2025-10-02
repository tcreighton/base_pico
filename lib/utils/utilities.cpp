
#include <sstream>
#include <pico/unique_id.h>
#include "logger.hpp"
#include "packed-datetime.hpp"
#include "utilities.hpp"

namespace CSutilities {

    // This doesn't actually do anything right now. We have to decide if it's worth the effort.
    /*
    std::string getEncodedPicoBoardId () {
        const std::stringstream ss;
        pico_unique_board_id_t id;

        pico_get_unique_board_id(&id);

        return ss.str();
    }
    */

    std::string getPicoBoardId () {
        char focus500Id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

        pico_get_unique_board_id_string(&focus500Id[0], sizeof(focus500Id));

        return std::string(focus500Id);
    }

    std::string getFirmwareVersion () {
        std::stringstream version;

        const auto packed = PackedDateTime::getPackedBuildDateTime();
//int_to_hex(T val, const size_t width = sizeof(T) * 2)
        version << static_cast<int>(FIRMWARE_MAJOR_VERSION) << "."
                << static_cast<int>(FIRMWARE_MINOR_VERSION) << "."
                << int_to_hex(packed);
        return version.str();
    }

    ProductInfo_t getProductInfo() {
        ProductInfo_t info;
        // If changes from the declared initializers are needed, this is a good place for them.
        return info;
    }

    std::string replaceAll (std::string str, const std::string &from, const std::string &to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }


}
