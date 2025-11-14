#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdint>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <simpleble/Adapter.h>

namespace tools {
#define NAMEOF(variable) #variable

    /**
     * @brief Conversion function that transforms hex value into a vector of raw bytes
     *
     * @param hex string representation of a hex value
     * @returns vector of raw bytes
     */
    std::vector<uint8_t> hex_to_bytes(const std::string& hex);

    /**
     * @brief Conversion function that transforms a vector of raw bytes into a hex value
     *
     * @param string buffer containing raw bytes in form of a string
     * @returns string representation of a hex value
     */
    std::string bytes_to_hex(const std::string& buf);

    /**
     * @brief Conversion function that transforms a vector of raw bytes into a hex value
     *
     * @param buf buffer containing raw bytes
     * @param len length of the raw bytes buffer
     * @returns string representation of a hex value
     */
    std::string bytes_to_hex(const uint8_t* buf, size_t len);

    /**
     * @brief String formatting function. Takes unformatted string and
     *        variadic number of arguments and puts them into the provided string
     *
     * @param buf buffer containing raw bytes
     * @param len length of the raw bytes buffer
     * @returns string representation of a hex value
     */
    std::string format_string(const std::string& format, ...);

    /**
     * @brief Retrieve available BLE adapters from the system
     *
     * @returns optional with the adapter as an object
     */
    std::optional<SimpleBLE::Adapter> getAdapter();

    /**
     * @brief Map given value from one range to another
     *
     * @param value value to be mapped
     * @param min_in_range lower bound of the mapped range
     * @param max_in_range upper bound of the mapped range
     * @param min_out_range lower bound of the range to be mapped to
     * @param max_out_range upper bound of the range to be mapped to
     * @returns mapped value
     */
    float map_range(float value, float min_in_range, float max_in_range, float min_out_range, float max_out_range);

} // namespace tools

#endif /* TOOLS_H_ */
